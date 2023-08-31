// Name - Simrat Hayer
// Student ID - 1122322
//include "~/assign1/include"
#include "KMLParser.h"
#include "helper.h"
#include <stdlib.h>

bool validateDoc(xmlDoc *doc, const char* schemaFile);

char globalOutput[100];

char* getOutput(){
	if(globalOutput != NULL){
		return globalOutput;
	}
	else{
		return "";
	}
}
//Assignment 2

xmlDoc * KMLtoDoc(const KML* mainKML){

	xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL, node = NULL, node1 = NULL, node2 = NULL, node3 = NULL;/* node pointers */

    LIBXML_TEST_VERSION;

	int length = 0;
	length = getNumPoints(mainKML) + getNumPaths(mainKML);

	/* 
     * Creates a new document, a node and set it as a root node
     */
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "kml");
    xmlDocSetRootElement(doc, root_node);

	ListIterator itr2;

	ListIterator itr = createIterator(mainKML->namespaces);
	XMLNamespace* data = nextElement(&itr);
	xmlNsPtr tmpNs;

	for(int i = 0; i < getNumXMLNamespaces(mainKML) ; i++){
		if(i == 0){
			tmpNs = xmlNewNs(root_node,BAD_CAST data->value,BAD_CAST data->prefix);
			xmlSetNs(root_node,tmpNs);
		}
		else{
			tmpNs = xmlNewNs(root_node,BAD_CAST data->value,BAD_CAST data->prefix);
		}
		data = nextElement(&itr);
	}

	if(length > 1 || getNumStyles(mainKML) > 0){
		node = xmlNewChild(root_node, NULL, BAD_CAST "Document", NULL);
	}
	else{
		node = root_node;
	}
	
	itr = createIterator(mainKML->styleMaps);
	StyleMap* tmpStyleMap = nextElement(&itr);

	for(int i = 0; i < getNumStyleMaps(mainKML); i++){
		node1 = xmlNewChild(node, NULL, BAD_CAST "StyleMap",NULL);
		xmlNewProp(node1, BAD_CAST "id", (xmlChar*)tmpStyleMap->id);
	
		node2 = xmlNewChild(node1, NULL, BAD_CAST "Pair",NULL);

		if(tmpStyleMap->key1 != NULL){
			node3 = xmlNewChild(node2, NULL, BAD_CAST "key",(xmlChar*)tmpStyleMap->key1);
		}
		if(tmpStyleMap->url1 != NULL){
			node3 = xmlNewChild(node2, NULL, BAD_CAST "styleUrl",(xmlChar*)tmpStyleMap->url1);
		}

		node2 = xmlNewChild(node1, NULL, BAD_CAST "Pair",NULL);

		if(tmpStyleMap->key2 != NULL){
			node3 = xmlNewChild(node2, NULL, BAD_CAST "key",(xmlChar*)tmpStyleMap->key2);
		}
		if(tmpStyleMap->url2 != NULL){
			node3 = xmlNewChild(node2, NULL, BAD_CAST "styleUrl",(xmlChar*)tmpStyleMap->url2);
		}
		
		tmpStyleMap = nextElement(&itr);
	}

	itr = createIterator(mainKML->styles);
	Style* tmpStyle = nextElement(&itr);
	char str[100];

	for(int i = 0; i < getNumStyles(mainKML); i++){
		node1 = xmlNewChild(node, NULL, BAD_CAST "Style",NULL);
		xmlNewProp(node1, BAD_CAST "id", (xmlChar*)tmpStyle->id);
	
		node2 = xmlNewChild(node1, NULL, BAD_CAST "LineStyle",NULL);

		if(tmpStyle->colour != NULL){
			node3 = xmlNewChild(node2, NULL, BAD_CAST "color",(xmlChar*)tmpStyle->colour);
		}
		if(tmpStyle->width != -1){
			sprintf(str, "%d", tmpStyle->width);
			node3 = xmlNewChild(node2, NULL, BAD_CAST "width",(xmlChar*)str);
		}

		if(tmpStyle->fill != -1){
			sprintf(str, "%d", tmpStyle->width);
			node3 = xmlNewChild(node2, NULL, BAD_CAST "fill",(xmlChar*)str);
		}
	
		tmpStyle = nextElement(&itr);
	}

	itr = createIterator(mainKML->pointPlacemarks);
	PointPlacemark* tmpPoint = nextElement(&itr);

	for(int i = 0; i < getNumPoints(mainKML); i++){
		node1 = xmlNewChild(node, NULL, BAD_CAST "Placemark",NULL);

		if(tmpPoint->name != NULL){
			node2 = xmlNewChild(node1, NULL, BAD_CAST "name",(xmlChar*)tmpPoint->name);
		}

		itr2 = createIterator(tmpPoint->otherElements);
		KMLElement* tmpOther = nextElement(&itr2);
		for(int j = 0; j < getLength(tmpPoint->otherElements); j++){
			node2 = xmlNewChild(node1, NULL,(xmlChar*)tmpOther->name,(xmlChar*)tmpOther->value);
			tmpOther = nextElement(&itr2);
		}

		itr2 = createIterator(tmpPoint->point->otherElements);
		tmpOther = nextElement(&itr2);

		node2 = xmlNewChild(node1, NULL, BAD_CAST "Point",NULL);

		for(int j = 0; j < getLength(tmpPoint->point->otherElements); j++){
			node3 = xmlNewChild(node2, NULL,(xmlChar*)tmpOther->name,(xmlChar*)tmpOther->value);
			tmpOther = nextElement(&itr2);
		}


		if(tmpPoint->point->coordinate->altitude != DBL_MAX){
			sprintf(str, "%0.7f,%0.7f,%0.7f", tmpPoint->point->coordinate->longitude,tmpPoint->point->coordinate->latitude,tmpPoint->point->coordinate->altitude);
		}
		else{
			sprintf(str, "%0.7f,%0.7f", tmpPoint->point->coordinate->longitude,tmpPoint->point->coordinate->latitude);
		}

		node3 = xmlNewChild(node2, NULL, BAD_CAST "coordinates",(xmlChar*)str);

		tmpPoint = nextElement(&itr);
	}


	itr = createIterator(mainKML->pathPlacemarks);
	PathPlacemark* tmpPath = nextElement(&itr);
	char* str2;
	str2 = malloc(sizeof(char*));
	strcpy(str2,"");

	for(int i = 0; i < getNumPaths(mainKML); i++){
		node1 = xmlNewChild(node, NULL, BAD_CAST "Placemark",NULL);

		if(tmpPath->name != NULL){
			node2 = xmlNewChild(node1, NULL, BAD_CAST "name",(xmlChar*)tmpPath->name);
		}

		itr2 = createIterator(tmpPath->otherElements);
		KMLElement* tmpOther = nextElement(&itr2);
		for(int j = 0; j < getLength(tmpPath->otherElements); j++){
			node2 = xmlNewChild(node1, NULL,(xmlChar*)tmpOther->name,(xmlChar*)tmpOther->value);
			tmpOther = nextElement(&itr2);
		}

		itr2 = createIterator(tmpPath->pathData->otherElements);
		tmpOther = nextElement(&itr2);

		node2 = xmlNewChild(node1, NULL, BAD_CAST "LineString",NULL);

		for(int j = 0; j < getLength(tmpPath->pathData->otherElements); j++){
			node3 = xmlNewChild(node2, NULL,(xmlChar*)tmpOther->name,(xmlChar*)tmpOther->value);
			tmpOther = nextElement(&itr2);
		}

		itr2 = createIterator(tmpPath->pathData->coordinates);
		Coordinate* tmpCoord = nextElement(&itr2);

		for(int j = 0; j < getLength(tmpPath->pathData->coordinates); j++){
			if(tmpCoord->altitude != DBL_MAX){
				sprintf(str, "%0.7f,%0.7f,%0.7f", tmpCoord->longitude,tmpCoord->latitude,tmpCoord->altitude);
			}
			else{
				sprintf(str, "%0.7f,%0.7f", tmpCoord->longitude,tmpCoord->latitude);
			}

			if(str2 == NULL){
				str2 = realloc(str2,strlen(str2) + 100);
			}
			else{
				str2 = realloc(str2,strlen(str2) + 100);
			}
			
			strcat(str2,str);
			strcat(str2," ");
			tmpCoord = nextElement(&itr2);
		}

		node3 = xmlNewChild(node2, NULL, BAD_CAST "coordinates",(xmlChar*)str2);
		tmpPath = nextElement(&itr);
	}

	if(node3 == NULL){
		
	}

	free(str2);

	return doc;
}

KML* createValidKML(const char *fileName, const char* schemaFile){
	if(fileName == NULL || schemaFile == NULL){
		return NULL;
	}

	xmlDoc *doc = NULL;
	int valid = 0;
	
	KML* mainKML;
	mainKML = NULL;
	/*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

	/*parse the file and get the DOM */
    doc = xmlReadFile(fileName, NULL, 0);
	
	if (doc == NULL) {
		//globalOutput = malloc(100);
        printf("error: could not parse file %s\n", fileName);
		sprintf(globalOutput,"error: could not parse file ");
		xmlCleanupParser();
		xmlMemoryDump();
		xmlDictCleanup ();
		return NULL;
    }

	valid = schemaValid(doc, schemaFile);

	//valid = validateKML(doc, schemaFile);

	//ool validateKML(const KML *doc, const char* schemaFile)

	if(valid == 1){
		mainKML = createKML(fileName);
	}
	else{
		mainKML = NULL;
	}


	xmlFreeDoc(doc);
	
	xmlCleanupParser();
	xmlMemoryDump();
	xmlDictCleanup ();
	//xmlDictFree();

	return mainKML;

}

int schemaValid(xmlDoc *doc ,const char* schemaFile){
	
	int valid = 0;

	xmlSchemaPtr schema = NULL;
	xmlSchemaParserCtxtPtr ctxt;

	xmlLineNumbersDefault(1);

	ctxt = xmlSchemaNewParserCtxt(schemaFile);

	xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
	schema = xmlSchemaParse(ctxt);
	xmlSchemaFreeParserCtxt(ctxt);

	if (doc == NULL){
		fprintf(stderr, "Could not parse\n");
		sprintf(globalOutput, "Could not parse");
	}
	else{
		xmlSchemaValidCtxtPtr ctxt;
		int ret;

		ctxt = xmlSchemaNewValidCtxt(schema);
		xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
		ret = xmlSchemaValidateDoc(ctxt, doc);
		
		if (ret == 0){
			valid = 1;
			printf("validates\n");
			sprintf(globalOutput, "validates");
		}
		else if (ret > 0){
			printf("fails to validate\n");
			sprintf(globalOutput, "fails to validate");
		}
		else{
			printf("validation generated an internal error\n");
			sprintf(globalOutput, "validation generated an internal error");
		}
		xmlSchemaFreeValidCtxt(ctxt);
		//xmlFreeDoc(doc);
	}

	// free the resource
	if(schema != NULL){
		xmlSchemaFree(schema);
	}

	xmlSchemaCleanupTypes();
	xmlCleanupParser();
	xmlMemoryDump();

	return valid;
}

bool writeValidKML(const KML* doc, const char* fileName){

	bool manCheck;

	manCheck = manualValid(doc);
	if(manCheck == false){
		sprintf(globalOutput, "fails to validate");
		return false;
	}
	else{
		sprintf(globalOutput, "validates");
	}

	bool writeCheck;

	writeCheck = writeKML(doc,fileName);

	return writeCheck;
}


bool writeKML(const KML* doc, const char* fileName){
	if(doc == NULL || fileName == NULL){
		return false;
	}

	xmlDoc* doc1;

	doc1 = KMLtoDoc(doc);
	
	bool validCheck;
	validCheck = validateDoc(doc1,"ogckml22.xsd");

	
	if(validCheck == true){
		sprintf(globalOutput, "validates");
	}
	else{
		sprintf(globalOutput, "fails to validate");
		return false;
	}

	if(doc1 == NULL){
		return false;
	}

	xmlSaveFormatFileEnc(fileName, doc1, "UTF-8", 1);
	
	/*free the document */
    xmlFreeDoc(doc1);

	xmlCleanupParser();
    xmlMemoryDump();

	return true;
}


bool validateDoc(xmlDoc *doc, const char* schemaFile){
	
	if(doc == NULL){
		return false;
	}

	xmlDoc* doc1 = doc;

	int valid = 0;

	valid = schemaValid(doc1 ,schemaFile);

	//xmlDictFree();

	if(valid == 0){
		return false;
	}

	return true;
}


bool validateKML(const KML *doc, const char* schemaFile){
	if(doc == NULL || schemaFile == NULL){
		return false;
	}

	bool manCheck;
	manCheck = manualValid(doc);
	if(manCheck == false){
		return false;
	}

	xmlDoc* doc1;

	doc1 = KMLtoDoc(doc);

	if(doc1 == NULL){
		return false;
	}

	int valid = 0;

	valid = schemaValid(doc1 ,schemaFile);

	xmlFreeDoc(doc1);
	
	xmlCleanupParser();
	xmlMemoryDump();
	xmlDictCleanup ();
	//xmlDictFree();

	if(valid == 0){
		return false;
	}

	return true;
}


bool manualValid(const KML *doc){

	if(doc == NULL || doc->namespaces == NULL || doc->pathPlacemarks == NULL || doc->pointPlacemarks == NULL || doc->styleMaps == NULL || doc->styles == NULL){
		return false;
	}

	if(getNumXMLNamespaces(doc) == 0){
		return false;
	}

	ListIterator itr2;
	KMLElement* tmpKML;

	ListIterator itr = createIterator(doc->namespaces);
	XMLNamespace* tmpNamespace = nextElement(&itr);

	for(int i = 0; i < getNumXMLNamespaces(doc) ; i++){
		if(tmpNamespace->value == NULL){
			return false;
		}

		if(strcmp(tmpNamespace->value,"") == 0){
			return false;
		}

	}


	itr = createIterator(doc->styleMaps);
	StyleMap* tmpStyleMap = nextElement(&itr);

	for(int i = 0; i < getNumStyleMaps(doc) ; i++){
		if(tmpStyleMap->id == NULL){
			return false;
		}

		if(strcmp(tmpStyleMap->id,"") == 0){
			return false;
		}

	}

	itr = createIterator(doc->styles);
	Style* tmpStyle = nextElement(&itr);

	for(int i = 0; i < getNumStyles(doc) ; i++){
		if(tmpStyle->id == NULL){
			return false;
		}

		if(strcmp(tmpStyle->id,"") == 0){
			return false;
		}

		if(tmpStyle->colour == NULL){
			return false;
		}

		if(strcmp(tmpStyle->colour,"") == 0){
			return false;
		}

	}


	itr = createIterator(doc->pointPlacemarks);
	PointPlacemark* tmpPoint = nextElement(&itr);

	for(int i = 0; i < getNumPoints(doc) ; i++){
		if(tmpPoint->point == NULL){
			return false;
		}
		if(tmpPoint->otherElements == NULL){
			return false;
		}

		if(tmpPoint->point->coordinate == NULL){
			return false;
		}

		if(tmpPoint->point->otherElements == NULL){
			return false;
		}

		itr2 = createIterator(tmpPoint->otherElements);
		tmpKML = nextElement(&itr2);

		for(int j = 0 ; j < getLength(tmpPoint->otherElements); j++){
			if(tmpKML->name == NULL || tmpKML->value == NULL){
				return false;
			}

			if((strcmp(tmpKML->name,"") == 0) || (strcmp(tmpKML->value,"") == 0)){
				return false;
			}
		}

		itr2 = createIterator(tmpPoint->point->otherElements);
		tmpKML = nextElement(&itr2);

		for(int j = 0 ; j < getLength(tmpPoint->point->otherElements); j++){
			if(tmpKML->name == NULL || tmpKML->value == NULL){
				return false;
			}

			if((strcmp(tmpKML->name,"") == 0) || (strcmp(tmpKML->value,"") == 0)){
				return false;
			}
		}

	}


	itr = createIterator(doc->pathPlacemarks);
	PathPlacemark* tmpPath = nextElement(&itr);

	for(int i = 0; i < getNumPaths(doc) ; i++){
		if(tmpPath->pathData == NULL){
			return false;
		}
		if(tmpPath->otherElements == NULL){
			return false;
		}

		if(tmpPath->pathData->coordinates == NULL){
			return false;
		}

		if(getLength(tmpPath->pathData->coordinates) < 2){
			return false;
		}

		if(tmpPath->pathData->otherElements == NULL){
			return false;
		}


		itr2 = createIterator(tmpPath->otherElements);
		tmpKML = nextElement(&itr2);

		for(int j = 0 ; j < getLength(tmpPath->otherElements); j++){
			if(tmpKML->name == NULL || tmpKML->value == NULL){
				return false;
			}

			if((strcmp(tmpKML->name,"") == 0) || (strcmp(tmpKML->value,"") == 0)){
				return false;
			}
		}

		itr2 = createIterator(tmpPath->pathData->otherElements);
		tmpKML = nextElement(&itr2);

		for(int j = 0 ; j < getLength(tmpPath->pathData->otherElements); j++){
			if(tmpKML->name == NULL || tmpKML->value == NULL){
				return false;
			}

			if((strcmp(tmpKML->name,"") == 0) || (strcmp(tmpKML->value,"") == 0)){
				return false;
			}
		}

	}

	return true;

}

double getPathLen(const PathPlacemark *ppm){

	if(ppm == NULL || getLength(ppm->pathData->coordinates) < 2 ){
		return 0.0;
	}

	double d = 0.0;

	ListIterator itr = createIterator(ppm->pathData->coordinates);
	Coordinate* curCoord = nextElement(&itr);

	Coordinate* nextCoord = nextElement(&itr);


	for(int i = 0 ; i < getLength(ppm->pathData->coordinates) - 1;i++){

		d = d + getDistance(curCoord,nextCoord);

		curCoord = nextCoord;

		nextCoord = nextElement(&itr);

	}

	return d;
}

double getDistance(Coordinate* coord1, Coordinate* coord2){

    if(coord1 == NULL || coord2 == NULL){
        return 0.0;
    }
	
	double d = 0.0;
	const double R = 6371*1000;	
	
	double phi1;
	double phi2;
	double delphi;
	double delta;

	double a;
	double a_1;
	double sqa;
	double sqa_1;
	double c;

	double lat1;
	double lat2;
	double lon1;
	double lon2;


	lat1 = coord1->latitude;
	lat2 = coord2->latitude;

	lon1 = coord1->longitude;
	lon2 = coord2->longitude;

	phi1 = lat1*((M_PI)/180);
	phi2 = lat2*((M_PI)/180);

	delphi = (lat2 - lat1) * ((M_PI)/180);
	delta = (lon2 - lon1) * ((M_PI)/180);


	a = sin(delphi/2) * sin(delphi/2) + cos(phi1)*cos(phi2)*sin(delta/2)*sin(delta/2);;

	a_1 = 1 - a;
	sqa = sqrt(a);
	sqa_1 = sqrt(a_1);
	c = 2 * atan2(sqa,sqa_1);
	
	d = R*c; 

	return d;
}

bool isLoopPath(const PathPlacemark* ppm, double delta){
	if(ppm == NULL || getLength(ppm->pathData->coordinates) < 4 ){
		return false;
	}

	ListIterator itr = createIterator(ppm->pathData->coordinates);
	Coordinate* curCoord = nextElement(&itr);

	Coordinate* nextCoord = nextElement(&itr);


	for(int i = 0 ; i < getLength(ppm->pathData->coordinates) - 2;i++){

		nextCoord = nextElement(&itr);
	}

	double dist = 0.0;

	dist = getDistance(curCoord,nextCoord);
	if(dist < delta){
		return true;
	}

	return false;

}

List* getPathsWithLength(const KML *doc, double len, double delta){

    if(doc == NULL || doc->pathPlacemarks == NULL){
        return NULL;
    }

	if(getNumPaths(doc) == 0){
		return NULL;
	}

	List* listPath = initializeList(&pathPlacemarkToString, &deleteSecPathPlacemark, &comparePathPlacemarks);

	ListIterator itr1 = createIterator(doc->pathPlacemarks);
	PathPlacemark* curPath;
	curPath = nextElement(&itr1);

	double d = 0.0;
	double x1 = 0.0;
	double x2 = 0.0;

	ListIterator itr;

	for(int j = 0; j < getLength(doc->pathPlacemarks); j++){

		d = 0.0;
		itr = createIterator(curPath->pathData->coordinates);
		Coordinate* curCoord = nextElement(&itr);
		Coordinate* nextCoord = nextElement(&itr);


		for(int i = 0 ; i < getLength(curPath->pathData->coordinates) - 1;i++){

			d = d + getDistance(curCoord,nextCoord);

			curCoord = nextCoord;
			nextCoord = nextElement(&itr);
			x1 = d - len;
			x2 = len - d;
			
		}

		if(0 < x1 && x1 < delta){
			insertBack(listPath,(void*)curPath);
		}
		else if(0 < x2 && x2< delta){
			insertBack(listPath,(void*)curPath);
		}

		curPath = nextElement(&itr1);
	}


	return listPath;

}


void deleteSecPathPlacemark(void* data){
	return;
}

//****END ASSIGNMENT 2

KML* createKML(const char* fileName){
	
	xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
	
	KML* mainKML;
	/*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

	/*parse the file and get the DOM */
    doc = xmlReadFile(fileName, NULL, 0);
	
	 if (doc == NULL) {
        printf("error: could not parse file %s\n", fileName);
		return NULL;
    }
	
	/*Get the root element node */
    root_element = xmlDocGetRootElement(doc);
	
    mainKML = parseTree(root_element);
	
	xmlFreeDoc(doc);
	
	xmlCleanupParser();

	return mainKML;

}


KML* parseTree(xmlNode * a_node){

	KML* mainKML;
	mainKML = malloc(sizeof(KML));

    xmlNode *cur_node1 = NULL;
    xmlNode *temp_node = NULL;
    xmlNode *cur_node = NULL;

	PointPlacemark* tmpPointPlacemark;
	
	List* listPointPlacemark = initializeList(&pointPlacemarkToString, &deletePointPlacemark, &comparePointPlacemarks);
	
	int choice = 0;
	PathPlacemark* tmpPathPlacemark;
	
	List* listPathPlacemark = initializeList(&pathPlacemarkToString, &deletePathPlacemark, &comparePathPlacemarks);

    cur_node1 = a_node->children->next;

    if(strcmp((char*)cur_node1->name,"Document") == 0){
        cur_node1 = cur_node1->children->next;
    }
	
	Style* tmpStyle;
	List* listStyle = initializeList(&styleToString, &deleteStyle, &compareStyles);

	StyleMap* tmpStyleMap;
	List* listStyleMap = initializeList(&styleMapToString, &deleteStyleMap, &compareStyleMaps);
	
	mainKML->namespaces = getNamespacelist(a_node);
	

	for (cur_node = cur_node1; cur_node != NULL; cur_node = cur_node->next){
		if (cur_node->type == XML_ELEMENT_NODE) {
			//printf("node type: Element, name: %s ,content %s\n", cur_node->name, cur_node->content);

			if(cur_node->content == NULL){
		    	temp_node = cur_node->children;
		    	if(temp_node == NULL){
			    	continue;
				}
			
			  	cur_node->content = temp_node->content;
			}

			if((strcmp((char*)cur_node->name,"Placemark") == 0)){
				
				choice = checkPlacemark(cur_node);
				
				if(choice == 1){
					tmpPointPlacemark = createPointPlacemark(cur_node);
					insertBack(listPointPlacemark,(void*)tmpPointPlacemark);
				}
				else{
					tmpPathPlacemark = createPathPlacemark(cur_node);
					insertBack(listPathPlacemark,(void*)tmpPathPlacemark);
				}
    		} 
			
			if((strcmp((char*)cur_node->name,"Style") == 0)){
				tmpStyle = createStyle(cur_node);
                if(tmpStyle == NULL){
					
                }
                else{
                    insertBack(listStyle,(void*)tmpStyle);
                }
			}

			if((strcmp((char*)cur_node->name,"StyleMap") == 0)){
				tmpStyleMap = createStyleMap(cur_node);
				insertBack(listStyleMap,(void*)tmpStyleMap);
			}
		}
	}

	mainKML->pointPlacemarks = listPointPlacemark;
	mainKML->pathPlacemarks = listPathPlacemark;
	mainKML->styles = listStyle;
	mainKML->styleMaps = listStyleMap;

	return mainKML;

}


//***PART2

int getNumPoints(const KML* doc){
	if(doc == NULL){
		return 0;
	}

    int sum = 0;

    if(doc->pointPlacemarks != NULL){
        sum =  getLength(doc->pointPlacemarks);
    }

    return sum;
}

int getNumPaths(const KML* doc){
	if(doc == NULL){
		return 0;
	}
    int sum = 0;

    if(doc->pathPlacemarks != NULL){
        sum =  getLength(doc->pathPlacemarks);
    }

    return sum;
}

int getNumXMLNamespaces(const KML* doc){
	if(doc == NULL){
		return 0;
	}
    int sum = 0;

    if(doc->namespaces != NULL){
        sum =  getLength(doc->namespaces);
    }

    return sum;
}

int getNumStyles(const KML* doc){
	if(doc == NULL){
		return 0;
	}
    int sum = 0;

    if(doc->styles != NULL){
        sum =  getLength(doc->styles);
    }

    return sum;
}

int getNumStyleMaps(const KML* doc){
	if(doc == NULL){
		return 0;
	}
    int sum = 0;

    if(doc->styleMaps != NULL){
        sum =  getLength(doc->styleMaps);
    }

    return sum;
}

int getNumKMLElements(const KML* doc){
	if(doc == NULL){
		return 0;
	}
    int sum = 0;
	ListIterator itr;
	PathPlacemark* tmpPath;
	PointPlacemark* tmpPoint;

	if(doc->pathPlacemarks != NULL){
		itr = createIterator(doc->pathPlacemarks);

		tmpPath = nextElement(&itr);
		
		while (tmpPath != NULL)
		{	
			sum = sum + getLength(tmpPath->otherElements) + getLength(tmpPath->pathData->otherElements);
			tmpPath = nextElement(&itr);
		}
	}



	if(doc->pointPlacemarks != NULL){
		itr = createIterator(doc->pointPlacemarks);

		tmpPoint = nextElement(&itr);
		
		while (tmpPoint != NULL)
		{	
			
			sum = sum + getLength(tmpPoint->otherElements) + getLength(tmpPoint->point->otherElements);
			tmpPoint = nextElement(&itr);
		}
	}

    return sum;
}


PointPlacemark* getPointPlacemark(const KML* doc, char* name){
	if(doc == NULL){
		return NULL;
	}

	if(doc->pointPlacemarks == NULL){
		return NULL;
	}
	ListIterator itr = createIterator(doc->pointPlacemarks);

	PointPlacemark* data = nextElement(&itr);
	while (data != NULL)
	{	
		if(data->name != NULL){
			if (strcmp((char*)data->name, name) == 0){
			return data;
			}
		}
		data = nextElement(&itr);
	}

    return NULL;
}

PathPlacemark* getPathPlacemark(const KML* doc, char* name){
	if(doc == NULL){
		return NULL;
	}

	if(doc->pathPlacemarks == NULL){
		return NULL;
	}


    ListIterator itr = createIterator(doc->pathPlacemarks);

	PathPlacemark* data = nextElement(&itr);
	while (data != NULL)
	{
		if (strcmp((char*)data->name, name) == 0){
			return data;
		}

		data = nextElement(&itr);
	}

    return NULL;
}

StyleMap* getMapFromPath(const KML *doc, const PathPlacemark *ppm){
	if(doc == NULL || ppm == NULL || doc->styleMaps == NULL){
		return NULL;
	}

	ListIterator itr = createIterator(ppm->otherElements);

	KMLElement* data = nextElement(&itr);
	while (data != NULL)
	{
		if (strcmp((char*)data->name, "styleUrl") == 0){
			break;
		}

		data = nextElement(&itr);
	}

	if(data == NULL){
		return NULL;
	}

	char tmpchar[200];
	char tmpchar1[200] = "#";

	ListIterator itr1 = createIterator(doc->styleMaps);

	StyleMap* data1 = nextElement(&itr1);
	while (data1 != NULL)
	{	
		strcpy(tmpchar,data1->id);
		strcat(tmpchar1,tmpchar);
		if (strcmp(tmpchar1,data->value) == 0){
			return data1;
		}

		data1 = nextElement(&itr);
	}


    return NULL;
}


Style* getStyleFromMap(const KML *doc, const StyleMap *map, int index){


	if(doc == NULL || map == NULL ||  index < 0 || index > 1 || doc->styles == NULL){
		return NULL;
	}

	char tmpUrl[200];
	char tmpchar[200];
	char tmpchar1[200];

	if(index == 0){
		strcpy(tmpUrl,map->url1);
	}
	else{
		strcpy(tmpUrl,map->url2);
	}


	ListIterator itr = createIterator(doc->styles);

	Style* data = nextElement(&itr);
	while (data != NULL)
	{	
		strcpy(tmpchar1,"#");
		strcpy(tmpchar,data->id);
		strcat(tmpchar1,tmpchar);
		if (strcmp(tmpchar1,tmpUrl ) == 0){
			return data;
		}

		data = nextElement(&itr);
	}


    return NULL;

}
//**PART2 END

void deleteStyleMap( void* data){

	StyleMap* tmpStyleMap = (StyleMap*) data;

	if(tmpStyleMap->id != NULL){
		free(tmpStyleMap->id);
	}
	if(tmpStyleMap->key1 != NULL){
		free(tmpStyleMap->key1);
	}
	if(tmpStyleMap->url1 != NULL){
		free(tmpStyleMap->url1);
	}
	if(tmpStyleMap->key2 != NULL){
		free(tmpStyleMap->key2);
	}
	if(tmpStyleMap->url2 != NULL){
		free(tmpStyleMap->url2);
	}
	free(tmpStyleMap);
	return;
}

char* styleMapToString( void* data){

	char* tmpStr;
	
	StyleMap* tmpStyleMap;
	
	if(data != NULL){
		tmpStyleMap = (StyleMap*)data;
	}

	tmpStr = (char*)malloc(sizeof(char)*50);
	
    sprintf(tmpStr,"%s, %s, %s, %s, %s\n",tmpStyleMap->id,tmpStyleMap->key1, tmpStyleMap->url1, tmpStyleMap->key2, tmpStyleMap->url2);

	return tmpStr;
}

int compareStyleMaps(const void *first, const void *second){
	return 0;
}

StyleMap* createStyleMap(xmlNode* a_node){

	xmlNode* cur_node;
	xmlNode* cur_node1;
	xmlNode* temp_node;
	StyleMap* tmpStyleMap;
	
	tmpStyleMap = malloc(sizeof(StyleMap));
	tmpStyleMap->id = NULL;
	
	cur_node = a_node;
	xmlAttr *attr;
        for (attr = cur_node->properties; attr != NULL; attr = attr->next)
        {
            xmlNode *value = attr->children;
			tmpStyleMap->id = (char *)(xmlNodeGetContent(value));
        }

	tmpStyleMap->key1= NULL;
	tmpStyleMap->url1 = NULL;
	tmpStyleMap->key2= NULL;
	tmpStyleMap->url2 = NULL;

	cur_node = a_node->children->next;
	cur_node1 = cur_node;

	for (cur_node = cur_node->children; cur_node != NULL; cur_node = cur_node->next){
		
		if (cur_node->type == XML_ELEMENT_NODE) {

			if(cur_node->content == NULL){
		    	temp_node = cur_node->children;
		    	if(temp_node == NULL){
			    	continue;
				}
			
			  	cur_node->content = temp_node->content;
			}
		
			if(strcmp((char*)cur_node->name,"key") == 0){
				tmpStyleMap->key1 = (char*)xmlNodeGetContent(cur_node);
			}

			if(strcmp((char*)cur_node->name,"styleUrl") == 0){
				tmpStyleMap->url1 = (char*)xmlNodeGetContent(cur_node);
			}
		}
	}

	cur_node = cur_node1->next->next;
	
	for (cur_node = cur_node->children; cur_node != NULL; cur_node = cur_node->next){
		
		if (cur_node->type == XML_ELEMENT_NODE) {

			if(cur_node->content == NULL){
		    	temp_node = cur_node->children;
		    	if(temp_node == NULL){
			    	continue;
				}
			
			  	cur_node->content = temp_node->content;
			}
		
		
			if(strcmp((char*)cur_node->name,"key") == 0){
				tmpStyleMap->key2 = (char*)xmlNodeGetContent(cur_node);
			}

			if(strcmp((char*)cur_node->name,"styleUrl") == 0){
				tmpStyleMap->url2 = (char*)xmlNodeGetContent(cur_node);
			}
		}
	}

	if(tmpStyleMap->id == NULL){
		deleteStyleMap(tmpStyleMap);
		return NULL;
	}

	return tmpStyleMap;
}


Style* createStyle(xmlNode* a_node){
	
	xmlNode* cur_node;
	xmlNode* temp_node;
	Style* tmpStyle;

	tmpStyle = malloc(sizeof(Style));
	tmpStyle->id = NULL;
	
	cur_node = a_node;
	xmlAttr *attr;
        for (attr = cur_node->properties; attr != NULL; attr = attr->next)
        {
            xmlNode *value = attr->children;
			tmpStyle->id = (char *)(xmlNodeGetContent(value));
        }
	tmpStyle->width= -1;
	tmpStyle->fill = -1;
    tmpStyle->colour = NULL;

	cur_node = a_node->children->next;
	
	for (cur_node = cur_node->children; cur_node != NULL; cur_node = cur_node->next){
		
		if (cur_node->type == XML_ELEMENT_NODE) {

			if(cur_node->content == NULL){
		    	temp_node = cur_node->children;
		    	if(temp_node == NULL){
			    	continue;
				}
			
			  	cur_node->content = temp_node->content;
			}
		
			if(strcmp((char*)cur_node->name,"color") == 0){
				tmpStyle->colour = (char*)xmlNodeGetContent(cur_node);
			}

			if(strcmp((char*)cur_node->name,"width") == 0){
				tmpStyle->width = atoi((char*)cur_node->content);
			}
			if(strcmp((char*)cur_node->name,"fill") == 0){
				tmpStyle->fill = atoi((char*)cur_node->content);
			}
		}
	}

    if(tmpStyle->colour == NULL){
		deleteStyle(tmpStyle);
        //free(tmpStyle);
        return NULL;
    }
	if(tmpStyle->id == NULL){
		deleteStyle(tmpStyle);
        return NULL;
    }
	
	return tmpStyle;
}


void deleteStyle( void* data){	
	
	Style* tmpStyle = (Style*) data;

	if(tmpStyle->id != NULL){
		free(tmpStyle->id);
	}
	if(tmpStyle->colour != NULL){
		free(tmpStyle->colour);
	}
	
	free(tmpStyle);
	
}


char* styleToString( void* data){
	
	char* tmpStr;
	
	Style* tmpStyle;
	
	if(data != NULL){
		tmpStyle = (Style*)data;
	}

	tmpStr = (char*)malloc(sizeof(char)*50);
	
    sprintf(tmpStr,"%s, %s, %d, %d\n",tmpStyle->id,tmpStyle->colour, tmpStyle->width, tmpStyle->fill);

	return tmpStr;
}


int compareStyles(const void *first, const void *second){
	return 0;
}


List* getNamespacelist(xmlNode * a_node){
	
	List* listNamespace = initializeList(&XMLNamespaceToString, &deleteXMLNamespace, &compareXMLNamespace);
	
	xmlNs *tmpNamespace = a_node->nsDef;
	xmlNs *curNamespace;
	
	XMLNamespace* KMLNamespace;
	int count = 0;
	
	for(curNamespace = tmpNamespace; curNamespace != NULL; curNamespace = curNamespace->next ){
		KMLNamespace = createNamespace(curNamespace);
		if(count == 0){
			KMLNamespace -> prefix = NULL;
			count++;
		}
		insertBack(listNamespace,(void*)KMLNamespace);
	}
	
	return listNamespace;	
}

XMLNamespace* createNamespace(xmlNs * KMLNamespace){
	
	XMLNamespace* tmpNamespace = malloc(sizeof(XMLNamespace));
	
	if(KMLNamespace->prefix !=NULL){
		tmpNamespace->prefix = malloc(strlen((char*)KMLNamespace->prefix) + 1);
		strcpy(tmpNamespace->prefix,(char*)KMLNamespace->prefix);
	}
	else{
		tmpNamespace->prefix = NULL;
	}
	
	
	tmpNamespace->value = malloc(strlen((char*)KMLNamespace->href) + 1);
	strcpy(tmpNamespace->value,(char*)KMLNamespace->href);
	
	return tmpNamespace;
}


void deleteXMLNamespace( void* data){
	
	XMLNamespace* tmpNamespace = (XMLNamespace*)data;
	
	if(tmpNamespace->prefix !=NULL){
		free(tmpNamespace->prefix);
	}
	
	free(tmpNamespace->value);
	
	free(tmpNamespace);
}


char* XMLNamespaceToString( void* data){
	
	XMLNamespace* tmpNamespace;
	
	if(data != NULL){
		tmpNamespace = (XMLNamespace*)data;
	}
	
	char* tmpStr;
	
	if(tmpNamespace->prefix == NULL){
		tmpStr = (char*)malloc(50);
		sprintf(tmpStr,"NULL, %s\n",tmpNamespace->value);
	}
	else{
		tmpStr = (char*)malloc(100);
		sprintf(tmpStr,"%s, %s\n",tmpNamespace->prefix,tmpNamespace->value);
	}
	
	return tmpStr;	
}


int compareXMLNamespace(const void *first, const void *second){
	return 0;
}


int checkPlacemark(xmlNode * a_node){
	xmlNode * cur_node;
	
	int choice = 0;
	
	for (cur_node = a_node->children; cur_node != NULL; cur_node = cur_node->next){
					
		
		if(strcmp((char*)cur_node->name,"Point") == 0){
			choice = 1;
		}
		
		if(strcmp((char*)cur_node->name,"LineString") == 0){
			choice = 2;
		}

	}
	
	return choice;
}



void deletePathPlacemark(void* data){
	
	PathPlacemark* tmpPathPlacemark;
	
	if(data != NULL){
		tmpPathPlacemark = (PathPlacemark*)data;
	}
	
	if(tmpPathPlacemark->name != NULL){
		free(tmpPathPlacemark->name);
	}
	
	deleteLine(tmpPathPlacemark->pathData);
	
	if(tmpPathPlacemark->otherElements != NULL){
		clearList(tmpPathPlacemark->otherElements);
		free(tmpPathPlacemark->otherElements);
	}
	
	free(tmpPathPlacemark);
}


void deleteLine(void* data){
	if(data == NULL){
		return;
	}
	
	Line* tmpLine = (Line*)data;
	
	if(tmpLine->coordinates != NULL){
		clearList(tmpLine->coordinates);
		free(tmpLine->coordinates);
	}
	
	if(tmpLine->otherElements != NULL){
		clearList(tmpLine->otherElements);
		free(tmpLine->otherElements);
	}
	
	free(tmpLine);
}


char* pathPlacemarkToString(void* data){
	PathPlacemark* tmpPathPlacemark;
	
	if(data != NULL){
		tmpPathPlacemark = (PathPlacemark*)data;
	}

	if(tmpPathPlacemark->name != NULL){
		printf("Name = %s", tmpPathPlacemark->name);
	}

	return toString(tmpPathPlacemark->pathData->coordinates);
}


int comparePathPlacemarks(const void *first, const void *second){
	return 0;
	
}


PathPlacemark* createPathPlacemark(xmlNode * a_node){

	xmlNode * cur_node;
	xmlNode * temp_node;
	
	PathPlacemark* tmpPathPlacemark;
	tmpPathPlacemark = malloc(sizeof(PathPlacemark));
	
	List* otherElements = initializeList(&KMLElementToString, &deleteKMLElement, &compareKMLElements);
	
	KMLElement* tmpOther;
	tmpPathPlacemark->name = NULL;
	
	for (cur_node = a_node->children; cur_node != NULL; cur_node = cur_node->next){
		
		if (cur_node->type == XML_ELEMENT_NODE) {

			if(cur_node->content == NULL){
			    temp_node = cur_node->children;
			    if(temp_node != NULL){
				    cur_node->content = temp_node->content;
			    }
		    }

			if(strcmp((char*)cur_node->name,"name") == 0){
				tmpPathPlacemark->name = (char*)xmlNodeGetContent (cur_node);
			}
			else if(strcmp((char*)cur_node->name,"LineString") == 0){
				tmpPathPlacemark->pathData = createPath(cur_node);
			}
			else{
				if(cur_node->type == XML_ELEMENT_NODE){
					tmpOther = createKMLElement(cur_node);
					insertBack(otherElements,(void*)tmpOther);
				}
			}
		}
	}
	
	tmpPathPlacemark->otherElements = otherElements;
	
	return tmpPathPlacemark;
}


Line* createPath(xmlNode * a_node){
	Line* tmpLine;
	
	tmpLine = malloc(sizeof(Line));
	
	xmlNode * cur_node;
	xmlNode * temp_node;
	
	List* otherElements = initializeList(&KMLElementToString, &deleteKMLElement, &compareKMLElements);
	
	
	KMLElement* tmpOther;
	
	for (cur_node = a_node->children; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
			
			if(cur_node->content == NULL){
			    temp_node = cur_node->children;
			    if(temp_node != NULL){
				    cur_node->content = temp_node->content;
			    }
		    }
		
			
		    if(strcmp((char*)cur_node->name,"coordinates") == 0){
		
	    		if(cur_node->content != NULL){
				    tmpLine->coordinates = createCoordinateList(cur_node);
			    }

		    }
		    else{
				if(cur_node->type == XML_ELEMENT_NODE){
			
					tmpOther = createKMLElement(cur_node);
					insertBack(otherElements,(void*)tmpOther);
				}
		    }
		}
    }

    tmpLine->otherElements = otherElements;
	
	return tmpLine;
}

List* createCoordinateList(xmlNode* a_node){
	
	xmlNode temp_node;
	temp_node = *a_node;
	
	if(temp_node.content == NULL){
		return NULL;
	}
	
	if(temp_node.children->type == XML_ELEMENT_NODE){
		return NULL;
	}
	
	List* lineList = initializeList(&coordinateToString, &deleteCoordinate, &compareCoordinates);
	Coordinate* tmpCoordinate = NULL;
	
	
	//char str[500];
	char* str;
	str = malloc(strlen((char*)temp_node.content) + 1);
	
	strcpy(str,(char*)temp_node.content);
	
	char delim[] = " \n\t\0";
	
	char* saveptr;
	char *ptr = strtok_r(str, delim,&saveptr);
	
	char tempchar[200];
	
	

	while(ptr != NULL)
	{	
		strcpy(tempchar,ptr);
		tmpCoordinate = createLineCoordinate(tempchar);
		insertBack(lineList,(void*)tmpCoordinate);
		
		ptr = strtok_r(NULL, delim,&saveptr);
	}

	free(str);
	
	return lineList;
}


Coordinate* createLineCoordinate(const char* a_node){
	
	if(strlen(a_node) == 0){
		return NULL;
	}
	
	char temp_node[200];
	strcpy(temp_node,a_node);
	
	Coordinate* tmpCoordinate = NULL;
	
	tmpCoordinate = (Coordinate*)malloc(sizeof(Coordinate));
	
	char str[200];
	strcpy(str,temp_node);
	
	char delim[] = ", \0\n\t";
	
	int i = 0;
	char *array[3];

	char *ptr = strtok(str, delim);
	

	while(ptr != NULL)
	{
		array[i] = ptr;
		i= i + 1;
		ptr = strtok(NULL, delim);
	}
	
	char *temp;
    
	tmpCoordinate->longitude = strtod(array[0],&temp);

	tmpCoordinate->latitude = strtod(array[1],&temp);
	
	if(i >= 3){
		tmpCoordinate-> altitude = strtod(array[2],&temp);
	}
	else{
		tmpCoordinate->altitude = DBL_MAX;
	}
	
	return tmpCoordinate;
}


PointPlacemark* createPointPlacemark(xmlNode * a_node){
	
	xmlNode * cur_node;
	xmlNode * temp_node;
	
	PointPlacemark* tmpPointPlacemark;
	tmpPointPlacemark = malloc(sizeof(PointPlacemark));
	
	List* otherElements = initializeList(&KMLElementToString, &deleteKMLElement, &compareKMLElements);
	
	KMLElement* tmpOther;
	tmpPointPlacemark->name = NULL;
	
	
	for (cur_node = a_node->children; cur_node != NULL; cur_node = cur_node->next){
		
		if (cur_node->type == XML_ELEMENT_NODE) {

			if(cur_node->content == NULL){
			    temp_node = cur_node->children;
			    if(temp_node != NULL){
				    cur_node->content = temp_node->content;
			    }
		    }
		
			if(strcmp((char*)cur_node->name,"name") == 0){
				tmpPointPlacemark->name = (char*)xmlNodeGetContent (cur_node);
			}
			else if(strcmp((char*)cur_node->name,"Point") == 0){
						
				tmpPointPlacemark->point = createPoint(cur_node);
			}
			else{
				if(cur_node->type == XML_ELEMENT_NODE){
					tmpOther = createKMLElement(cur_node);
					insertBack(otherElements,(void*)tmpOther);
				}
			}
		}
	}
	
	tmpPointPlacemark->otherElements = otherElements;
	
	return tmpPointPlacemark;
}


void deletePointPlacemark(void* data){
	
	PointPlacemark* tmpPointPlacemark;
	
	if(data != NULL){
		tmpPointPlacemark = (PointPlacemark*)data;
	}
	
	if(tmpPointPlacemark->name != NULL){
		free(tmpPointPlacemark->name);
	}
	
	deletePoint(tmpPointPlacemark->point);
	
	if(tmpPointPlacemark->otherElements != NULL){
		clearList(tmpPointPlacemark->otherElements);
		free(tmpPointPlacemark->otherElements);
	}
	
	free(tmpPointPlacemark);
	
}

char* pointPlacemarkToString(void* data){
	PointPlacemark* tmpPointPlacemark;
	
	if(data != NULL){
		tmpPointPlacemark = (PointPlacemark*)data;
	}
	
	return pointToString(tmpPointPlacemark->point);	
}


int comparePointPlacemarks(const void *first, const void *second){
	return 0;
}

Point* createPoint(xmlNode * a_node){
	
	Point* tmpPoint;
	
	tmpPoint = malloc(sizeof(Point));
	
	xmlNode * cur_node;
	xmlNode * temp_node;
	
	List* otherElements = initializeList(&KMLElementToString, &deleteKMLElement, &compareKMLElements);
	
	KMLElement* tmpOther;
	
	for (cur_node = a_node->children; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
			
			if(cur_node->content == NULL){
			    temp_node = cur_node->children;
			    if(temp_node != NULL){
				    cur_node->content = temp_node->content;
			    }
		    }
		
			
		    if(strcmp((char*)cur_node->name,"coordinates") == 0){
	    		if(cur_node->content != NULL){
				    tmpPoint->coordinate = createCoordinate(cur_node);
			    }

		    }
		    else{
				if(cur_node->type == XML_ELEMENT_NODE){
					tmpOther = createKMLElement(cur_node);
					insertBack(otherElements,(void*)tmpOther);
				}
		    }
		}
    }

    tmpPoint->otherElements = otherElements;
	
	return tmpPoint;
}


void deletePoint(void* data){
	
	Point* tmpPoint = (Point*)data;
	
	deleteCoordinate(tmpPoint->coordinate);

    if(tmpPoint->otherElements != NULL){
        clearList(tmpPoint->otherElements);
		free(tmpPoint->otherElements);
    }
	
	free(tmpPoint);
}


char* pointToString(void* data){
	
	Point* tmpPoint = (Point*)data;
	
	return coordinateToString(tmpPoint->coordinate);
}


int comparePoints(const void *first, const void *second){
	return 0;
}


Coordinate* createCoordinate(xmlNode * a_node){
	
	xmlNode temp_node;
	temp_node = *a_node;
	
	if(temp_node.content == NULL){
		return NULL;
	}
	
	if(temp_node.children->type == XML_ELEMENT_NODE){
		return NULL;
	}
	
	Coordinate* tmpCoordinate = NULL;
	
	tmpCoordinate = (Coordinate*)malloc(sizeof(Coordinate));
	
	char str[200];
	strcpy(str,(char*)temp_node.content);
	
	char delim[] = ", ";
	
	int i = 0;
	char *array[3];

	char *ptr = strtok(str, delim);
	
	while(ptr != NULL)
	{	
		array[i] = ptr;
		i= i + 1;
		ptr = strtok(NULL, delim);
	}
	
	char *temp;
	
	tmpCoordinate->longitude = atof(array[0]);
	
	tmpCoordinate->latitude = strtod(array[1],&temp);
		
	if(i >= 3){
		tmpCoordinate-> altitude = atof(array[2]);
	}
	else{
		tmpCoordinate->altitude = DBL_MAX;
	}
	
	return tmpCoordinate;
}


void deleteCoordinate(void* data){
	
	Coordinate* tmpCoordinate;
	
	if(data!=NULL){
		tmpCoordinate = (Coordinate*)data;
	}
	
	free(tmpCoordinate);
}



char* coordinateToString(void* data){
	
	char* tmpStr;
	
	Coordinate* tmpCoordinate;
	
	if(data != NULL){
		tmpCoordinate = (Coordinate*)data;
	}

	tmpStr = (char*)malloc(sizeof(char)*50);
	
	if(tmpCoordinate->altitude == DBL_MAX){
        sprintf(tmpStr,"%lf, %lf\n",tmpCoordinate->longitude,tmpCoordinate->latitude);
	}
	else{
        sprintf(tmpStr,"%lf, %lf, %lf\n",tmpCoordinate->longitude,tmpCoordinate->latitude, tmpCoordinate->altitude);
	}
	
	return tmpStr;
}


int compareCoordinates(const void *first, const void *second){
	
	return 0;
}


KMLElement* createKMLElement(xmlNode * a_node){
	xmlNode * cur_node = a_node;
	
	KMLElement* tmpOther;
	tmpOther = malloc(sizeof(KMLElement));

    tmpOther->name = malloc(strlen((char*)cur_node->name) + 1);
    strcpy(tmpOther->name,(char*)cur_node->name);
	
	tmpOther->value = (char*)xmlNodeGetContent (cur_node);

    return tmpOther;
}

void deleteKMLElement( void* data){
	
    KMLElement* tmpElement;
	
	if(data != NULL){
		tmpElement = (KMLElement*)data;
	}
	
	if(tmpElement->name != NULL){
		free(tmpElement->name);
	}
	
	if(tmpElement->value != NULL){
		free(tmpElement->value);
	}
   
    free(tmpElement);
}

char* KMLElementToString( void* data){

    char* tmpStr;
	
	KMLElement* tmpElement;
	
	if(data != NULL){
		tmpElement = (KMLElement*)data;
	}

	tmpStr = (char*)malloc(sizeof(char)*50);
	
    sprintf(tmpStr,"%s, %s\n",tmpElement->name,tmpElement->value);

	return tmpStr;	
}


int compareKMLElements(const void *first, const void *second){
	return 0;
}


void deleteKML(KML* doc){

	if(doc == NULL){
		return;
	}
	
	if(doc->pointPlacemarks != NULL){
		clearList(doc->namespaces);
		free(doc->namespaces);
	}

	if(doc->pointPlacemarks != NULL){
		clearList(doc->pointPlacemarks);
		free(doc->pointPlacemarks);
	}
	
	if(doc->pathPlacemarks != NULL){
		clearList(doc->pathPlacemarks);
		free(doc->pathPlacemarks);
	}
	if(doc->styles != NULL){
		clearList(doc->styles);
		free(doc->styles);
	}

	if(doc->styleMaps != NULL){
		clearList(doc->styleMaps);
		free(doc->styleMaps);
	}

	free(doc);
}

//A3
char* getStyleColor(KML* tmpKML,int i){
	if(tmpKML == NULL || tmpKML->styles == NULL){
		return "";
	}

	if( i > getNumStyles(tmpKML)){
		return "";
	}

	Style* tmpStyle;
	char* tmpColor;

	ListIterator itr = createIterator(tmpKML->styles);
	tmpStyle = nextElement(&itr);

	for(int j = 0; j < i - 1; j++){
		tmpStyle = nextElement(&itr);
	}


	if(tmpStyle->colour == NULL){
		return "";
	}

	tmpColor = malloc(strlen(tmpStyle->colour) + 1);
	strcpy(tmpColor,tmpStyle->colour);
	return tmpColor;
}

int getStyleWidth(KML* tmpKML,int i){
	if(tmpKML == NULL || tmpKML->styles == NULL){
		return -1;
	}

	if( i > getNumStyles(tmpKML)){
		return -1;
	}

	Style* tmpStyle;
	int tmpWidth;

	ListIterator itr = createIterator(tmpKML->styles);
	tmpStyle = nextElement(&itr);

	for(int j = 0; j < i - 1; j++){
		tmpStyle = nextElement(&itr);
	}


	if(tmpStyle->width == -1){
		return -1;
	}

	tmpWidth = tmpStyle->width;
	return tmpWidth;
}

int getStyleFill(KML* tmpKML,int i){
	if(tmpKML == NULL || tmpKML->styles == NULL){
		return -1;
	}

	if( i > getNumStyles(tmpKML)){
		return -1;
	}

	Style* tmpStyle;
	int tmpFill;

	ListIterator itr = createIterator(tmpKML->styles);
	tmpStyle = nextElement(&itr);

	for(int j = 0; j < i - 1; j++){
		tmpStyle = nextElement(&itr);
	}


	if(tmpStyle->fill == -1){
		return -1;
	}

	tmpFill = tmpStyle->fill;
	return tmpFill;
}

void setStyleColor(KML* tmpKML,int i,char* setColor){
	if(tmpKML == NULL || tmpKML->styles == NULL){
		return;
	}

	if( i > getNumStyles(tmpKML)){
		return;
	}

	Style* tmpStyle;

	ListIterator itr = createIterator(tmpKML->styles);
	tmpStyle = nextElement(&itr);

	for(int j = 0; j < i - 1; j++){
		tmpStyle = nextElement(&itr);
	}

	if (strcmp(tmpStyle->colour,"")== 0){
		return;
	}

	tmpStyle->colour = malloc(strlen(setColor) + 1);
	strcpy(tmpStyle->colour,setColor);

	return;
}



void setStyleWidth(KML* tmpKML,int i,int setWidth){
	if(tmpKML == NULL || tmpKML->styles == NULL){
		return;
	}

	if( i > getNumStyles(tmpKML)){
		return;
	}

	Style* tmpStyle;

	ListIterator itr = createIterator(tmpKML->styles);
	tmpStyle = nextElement(&itr);

	for(int j = 0; j < i - 1; j++){
		tmpStyle = nextElement(&itr);
	}

	if (tmpStyle->width == -1){
		return;
	}

	tmpStyle->width = setWidth;
	return;
}



bool isLoopPathIndex(KML* tmpKML, double delta,int i){
	if(tmpKML == NULL || tmpKML->pathPlacemarks == NULL){
		return false;
	}

	if( i > getLength(tmpKML->pathPlacemarks)){
		return false;
	}

	PathPlacemark* tmpPath;

	ListIterator itr = createIterator(tmpKML->pathPlacemarks);
	tmpPath = nextElement(&itr);

	for(int j = 0; j < i - 1; j++){
		tmpPath = nextElement(&itr);
	}

	bool isLoopBool;

	isLoopBool = isLoopPath(tmpPath,delta);

	return isLoopBool;
}


double getPathLenIndex(KML* tmpKML,int i){
	if(tmpKML == NULL || tmpKML->pathPlacemarks == NULL){
		return 0.0;
	}

	if( i > getLength(tmpKML->pathPlacemarks)){
		return 0.0;
	}

	PathPlacemark* tmpPath;

	ListIterator itr = createIterator(tmpKML->pathPlacemarks);
	tmpPath = nextElement(&itr);

	for(int j = 0; j < i - 1; j++){
		tmpPath = nextElement(&itr);
	}

	double len;

	len = getPathLen(tmpPath);

	return len;

}



double getLong(KML* tmpKML,int i){
	if(tmpKML == NULL || tmpKML->pointPlacemarks == NULL){
		return 0.0;
	}

	if( i > getLength(tmpKML->pointPlacemarks)){
		return 0.0;
	}

	PointPlacemark* tmpPoint;

	ListIterator itr = createIterator(tmpKML->pointPlacemarks);
	tmpPoint = nextElement(&itr);

	for(int j = 0; j < i - 1; j++){
		tmpPoint = nextElement(&itr);
	}

	return tmpPoint->point->coordinate->longitude;
}

double getLat(KML* tmpKML,int i){
	if(tmpKML == NULL || tmpKML->pointPlacemarks == NULL){
		return 0.0;
	}

	if( i > getLength(tmpKML->pointPlacemarks)){
		return 0.0;
	}

	PointPlacemark* tmpPoint;

	ListIterator itr = createIterator(tmpKML->pointPlacemarks);
	tmpPoint = nextElement(&itr);

	for(int j = 0; j < i - 1; j++){
		tmpPoint = nextElement(&itr);
	}

	return tmpPoint->point->coordinate->latitude;
}

double getAlt(KML* tmpKML,int i){
	if(tmpKML == NULL || tmpKML->pointPlacemarks == NULL){
		return 0.0;
	}

	if( i > getLength(tmpKML->pointPlacemarks)){
		return 0.0;
	}

	PointPlacemark* tmpPoint;

	ListIterator itr = createIterator(tmpKML->pointPlacemarks);
	tmpPoint = nextElement(&itr);

	for(int j = 0; j < i - 1; j++){
		tmpPoint = nextElement(&itr);
	}

	return tmpPoint->point->coordinate->altitude;
}


char* getPointName(KML* tmpKML,int i){
	if(tmpKML == NULL || tmpKML->pointPlacemarks == NULL){
		return "";
	}

	if( i > getLength(tmpKML->pointPlacemarks)){
		return "";
	}

	PointPlacemark* tmpPoint;
	char* tmpName;

	ListIterator itr = createIterator(tmpKML->pointPlacemarks);
	tmpPoint = nextElement(&itr);

	for(int j = 0; j < i - 1; j++){
		tmpPoint = nextElement(&itr);
	}

	if(tmpPoint->name == NULL){
		return "";
	}

	tmpName = malloc(strlen(tmpPoint->name) + 1);
	strcpy(tmpName,tmpPoint->name);
	return tmpName;
}


void setPointName(KML* tmpKML,int i,char* setName){
	if(tmpKML == NULL || tmpKML->pointPlacemarks == NULL){
		return;
	}

	if( i > getLength(tmpKML->pointPlacemarks)){
		return;
	}

	PointPlacemark* tmpPoint;
	//char* tmpName;

	ListIterator itr = createIterator(tmpKML->pointPlacemarks);
	tmpPoint = nextElement(&itr);

	for(int j = 0; j < i - 1; j++){
		tmpPoint = nextElement(&itr);
	}

	tmpPoint->name = malloc(strlen(setName) + 1);
	strcpy(tmpPoint->name,setName);
	return;
}


char* getPathName(KML* tmpKML,int i){
	if(tmpKML == NULL || tmpKML->pathPlacemarks == NULL){
		return "";
	}

	if( i > getLength(tmpKML->pathPlacemarks)){
		return "";
	}

	PathPlacemark* tmpPath;
	char* tmpName;

	ListIterator itr = createIterator(tmpKML->pathPlacemarks);
	tmpPath = nextElement(&itr);

	for(int j = 0; j < i - 1; j++){
		tmpPath = nextElement(&itr);
	}

	if(tmpPath->name == NULL){
		return "";
	}

	tmpName = malloc(strlen(tmpPath->name) + 1);
	strcpy(tmpName,tmpPath->name);
	return tmpName;
}


void setPathName(KML* tmpKML,int i,char* setName){
	if(tmpKML == NULL || tmpKML->pathPlacemarks == NULL){
		return;
	}

	if( i > getLength(tmpKML->pathPlacemarks)){
		return;
	}

	PathPlacemark* tmpPath;
	//char* tmpName;

	ListIterator itr = createIterator(tmpKML->pathPlacemarks);
	tmpPath = nextElement(&itr);

	for(int j = 0; j < i - 1; j++){
		tmpPath = nextElement(&itr);
	}

	tmpPath->name = malloc(strlen(setName) + 1);
	strcpy(tmpPath->name,setName);
	return;
}

/*char* getPointName(PointPlacemark* tmpPoint){
	char* tmpName;

	tmpName = malloc(strlen(tmpPoint->name) + 1);
	strcpy(tmpName,tmpPoint->name);

	return tmpName;
}*/


/*int main(int argc, char **argv){
	
	if (argc != 2){
        printf("usage: xmlExample <someXMLfile>\n");
        return(1);
    }

	KML* mainKML;
	bool write;
	bool loopTest;

	mainKML = createValidKML(argv[1], "ogckml22.xsd");
	//mainKML = createKML(argv[1]);

	if(mainKML == NULL){
		return(1);
	}

	loopTest = validateKML(mainKML,"ogckml22.xsd");
	printf("Valid ");
	if(loopTest == true){
		printf("Loop - true\n");
	}
	else{
		printf("Loop - false\n");
		return(1);
	}

	write  = writeKML(mainKML,"trial.kml");

	if(write == false){
		return(1);
	}
	
	//mainKML = createKML(argv[1]);

    printf("num Points = %d\n",getNumPoints(mainKML));
    printf("num Paths = %d\n",getNumPaths(mainKML));
    printf("num XMLNamespaces = %d\n",getNumXMLNamespaces(mainKML));
    printf("num NumStyles = %d\n",getNumStyles(mainKML));
    printf("num NumStyleMaps = %d\n",getNumStyleMaps(mainKML));
    //printf("num NumKMLElements = %d\n",getNumKMLElements(mainKML));

	char * tmpPrint = toString(mainKML->namespaces);
    printf("Print Namespaces %s\n",tmpPrint);
    free(tmpPrint);

	tmpPrint = toString(mainKML->pointPlacemarks);
    printf("Print Point Placemark %s\n",tmpPrint);
    free(tmpPrint);

	tmpPrint = toString(mainKML->pathPlacemarks);
    printf("Print Path Placemark %s\n",tmpPrint);
    free(tmpPrint);
	
	tmpPrint = toString(mainKML->styles);
    printf("Print Style %s\n",tmpPrint);
    free(tmpPrint);

	tmpPrint = toString(mainKML->styleMaps);
    printf("Print styleMAPS %s\n",tmpPrint);
    free(tmpPrint);

	PointPlacemark* test;
	test = getPointPlacemark(mainKML,"Simple placemark");


	ListIterator itr = createIterator(mainKML->pathPlacemarks);
	PathPlacemark* data = nextElement(&itr);

	printf("1PATH LENGTH - %f \n", getPathLen(data));


	loopTest = isLoopPath(data,10);
	if(loopTest == true){
		printf("Loop - true\n");
	}
	else{
		printf("Loop - false\n");
	}

	StyleMap* test1;
	test1 = getMapFromPath(mainKML,data);


	itr = createIterator(mainKML->styleMaps);
	StyleMap* data1 = nextElement(&itr);

	Style* test2;
	test2 =  getStyleFromMap(mainKML,data1,0);

	if(test2 != NULL){
		printf("Style to map %s\n",styleToString(test2));
	}

	List* tmpTest;

	tmpTest = getPathsWithLength(mainKML,39.8075818,10);

	char* testStr;

	if(tmpTest != NULL){
		testStr = toString(tmpTest);
		printf("Here %s\n",testStr);
		free(testStr);
	}

	clearList(tmpTest);
	free(tmpTest);
	

	deleteKML(mainKML);
	
}*/