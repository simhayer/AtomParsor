UNAME := $(shell uname)
CC = gcc
CFLAGS = -Wall -std=c11 -g
LDFLAGS= -L.

ifeq ($(UNAME), Linux)
	XML_PATH = /usr/include/libxml2
endif
ifeq ($(UNAME), Darwin)
	XML_PATH = /System/Volumes/Data/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/libxml2
endif

parser: bin/libkmlparser.so

bin/libkmlparser.so: bin/libkmlparser.o bin/LinkedListAPI.o
	$(CC) -I$(XML_PATH) -lxml2 -shared -fpic -o bin/libkmlparser.so bin/libkmlparser.o bin/LinkedListAPI.o -Iinclude/ -Isrc/ -Ibin/ -I$(XML_PATH)

bin/libkmlparser.o: src/KMLParser.c include/KMLParser.h include/helper.h
	$(CC) $(CFLAGS) -I$(XML_PATH) -c -fpic src/KMLParser.c -o bin/libkmlparser.o -Iinclude/ -Isrc/ -lxml2 -I$(XML_PATH)

bin/LinkedListAPI.o: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) $(CFLAGS) -c -fpic -Iinclude/ -Isrc/ src/LinkedListAPI.c -o bin/LinkedListAPI.o

clean:
	rm -rf *.o *.so ./bin/*.so ./bin/*.o

###################################################################################################
