// Name - Simrat Hayer
// Student ID - 1122322

extern char *strtok_r(char *, const char *, char **);

List* getNamespacelist(xmlNode * a_node);
XMLNamespace* createNamespace(xmlNs * KMLNamespace);

int checkPlacemark(xmlNode * a_node);
PathPlacemark* createPathPlacemark(xmlNode * a_node);
Line* createPath(xmlNode * a_node);
List* createCoordinateList(xmlNode* a_node);
Coordinate* createLineCoordinate(const char* a_node);
void deleteLine(void* data);

KML* parseTree(xmlNode * a_node);

List* recurPoint(xmlNode * a_node);
List* readPoint(List* listPoint, xmlNode * a_node);

Coordinate* createCoordinate(xmlNode * a_node);
Point* createPoint(xmlNode * a_node);
PointPlacemark* createPointPlacemark(xmlNode * a_node);
KMLElement* createKMLElement(xmlNode * a_node);

Style* createStyle(xmlNode* a_node);

StyleMap* createStyleMap(xmlNode* a_node);

int schemaValid(xmlDoc *doc ,const char* schemaFile);
double getDistance(Coordinate* coord1, Coordinate* coord2);
PathPlacemark* copyPath(PathPlacemark* curPath);
bool manualValid(const KML *doc);

void deleteSecPathPlacemark(void* data);