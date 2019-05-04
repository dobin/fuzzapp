#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#ifndef _DATASTRUCTURES
#define _DATASTRUCTURES

#define BUF_SIZE 1024

/********************/
/* As written in file */

typedef struct _DobSectionFile {
	uint8_t type;
	uint16_t dataOffset; 
	uint16_t dataSize;
} DobSectionFile;


typedef struct _DobHeaderFile {
	uint16_t magic;
	uint16_t version;

	size_t dobSectionOffset; 
	uint8_t dobSectionCount;
} DobHeaderFile;


/*********************/
/* for us, in memory */

typedef struct _DobSection {
	uint8_t type;
	uint16_t dataSize;
	char* data;
} DobSection;


typedef struct _DobHeader {
	uint16_t magic;
	uint16_t version;

	uint8_t dobSectionCount;
	DobSection** dobSections;
} DobData;


/* payloads */

typedef struct _SectionType1 {
	int size;
	char data[64];
} SectionType1;


typedef struct _SectionType2 {
	int index;
	int data;
} SectionType2;





/* some functions */ 

int makeExampleFile(char **buffer, int *bufferLen);
void printDobData(DobData *dobData);
void freeDobData(DobData *dobData);
DobData* parseBuffer(char *buffer, int bufferLen);

#endif
