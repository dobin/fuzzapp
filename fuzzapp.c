#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datastructures.h"
#include "parser.h"


void writeExampleFile(char *filename) {
	int fileSock;
	char *buffer;
	int bufferLen;

	printf("Write File: %s\n", filename);

	fileSock = open(filename, O_WRONLY | O_CREAT, 0644);
	if (fileSock < 0) {
		printf("Error");
		return;
	}

	makeExampleFile(&buffer, &bufferLen);
	write(fileSock, buffer, bufferLen);
	free(buffer);
	close(fileSock);
}


DobData* readExampleFile(char *filename) {
	int fileSock;
	char* buffer;
	int bufferLen;
	DobData* dobData;

	printf("Read File: %s\n", filename);

	fileSock = open(filename, O_RDONLY);
	if (fileSock < 0) {
		printf("Error");
		return NULL;
	}

	buffer = malloc(BUF_SIZE);
	read(fileSock, buffer, BUF_SIZE);

	dobData = parseBuffer(buffer, BUF_SIZE);
	
	free(buffer);
	close(fileSock);
	
	return dobData;
}


int main(int argc, char **argv) {
	if(argc <= 1) {
		printf("Nope\n");
		return 1;
	}

	if (strcmp(argv[1], "writeExample") == 0) {
		writeExampleFile("example.dob");
		return 0;
	}
	if (strcmp(argv[1], "readExample") == 0) {
		DobData *dobData;
		dobData = readExampleFile("example.dob");
		parseDobData(dobData);
		freeDobData(dobData);
		return 0;
	}

	if (strcmp(argv[1], "read") == 0) {
		DobData *dobData;
		dobData = readExampleFile(argv[2]);
		parseDobData(dobData);
		freeDobData(dobData);
		return 0;
	}
}
