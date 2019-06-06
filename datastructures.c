#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "datastructures.h"

int makeExampleFile(char **buffer, int *bufferLen) {
	char *p;
	int sectionCount = 3;
	int baseOffset = 0;
	DobHeaderFile dobHeaderFile = {
		42,
		1,
		sizeof(DobHeaderFile),
		sectionCount
	};
	
	*buffer = malloc(BUF_SIZE);
	memcpy(*buffer, &dobHeaderFile, sizeof(DobHeaderFile));
	p = *buffer + sizeof(DobHeaderFile);
	baseOffset = sizeof(DobHeaderFile) + (sectionCount * sizeof(DobSectionFile));

	/* Section 0x1 */
	SectionType1 secType1 = {
		0x1,
		"AAAAAAAAAAAAAAAAAAAAA"
	};
	DobSectionFile dobSection1 = {
		0x1,
		baseOffset,
		sizeof(secType1)
	};
	memcpy(p, &dobSection1, sizeof(DobSectionFile));
	p += sizeof(DobSectionFile);
	baseOffset += sizeof(secType1);

	/* Section 0x2 */
	SectionType2 secType2 = {
		0x2,
		"BBBBBBBBBBBBBB"
	};
	DobSectionFile dobSection2 = {
		0x2,
		baseOffset,
		sizeof(secType2)
	};
	memcpy(p, &dobSection2, sizeof(DobSectionFile));
	p += sizeof(DobSectionFile);
	baseOffset += sizeof(secType2);

	/* Section 0x3 */
	SectionType3 secType3 = {
		0x3,
		"CCCCCCCCCCCCC"
	};
	DobSectionFile dobSection3 = {
		0x3,
		baseOffset,
		sizeof(secType2)
	};
	memcpy(p, &dobSection3, sizeof(DobSectionFile));
	p += sizeof(DobSectionFile);
	baseOffset += sizeof(secType3);

	memcpy(p, &secType1, sizeof(secType1));
	p += sizeof(secType1);
	memcpy(p, &secType2, sizeof(secType2));
	p += sizeof(secType2);
	memcpy(p, &secType3, sizeof(secType3));
	p += sizeof(secType3);

	*bufferLen = p - *buffer;

	return 1;
}


void printDobData(DobData *dobData) {
	printf("DobHeaderFile Magic:   %i\n", dobData->magic);
	printf("DobHeaderFile Version: %i\n", dobData->version);
	printf("DobHeaderFile SectionCount: %hhu\n", dobData->dobSectionCount);

	for(int n=0; n<dobData->dobSectionCount; n++) {
		printf("DobSectionFile %i Size:   %i\n", n, dobData->dobSections[n]->dataSize);
		printf("DobSectionFile %i Type:   %i\n", n, dobData->dobSections[n]->type);
		//printf("DobSectionFile %i Data:   %s\n", n, dobData->dobSections[n]->data);
	}
}


void freeDobData(DobData *dobData) {
	if (dobData == NULL) {
		return;
	}
	for(int n=0; n<dobData->dobSectionCount; n++) {
		free(dobData->dobSections[n]->data);
		free(dobData->dobSections[n]);
	}
	free(dobData->dobSections);
	free(dobData);
}


DobData* parseBuffer(char *buffer, int bufferLen) {
	DobData* dobData = malloc(sizeof(DobData)); // SAFE: was: dobData
	DobSectionFile* dobDataSec;

	/* SAFE */
	if (buffer == NULL || bufferLen < sizeof(DobHeaderFile)) {
		return NULL;
	}

	DobHeaderFile *dobHeaderFile = (DobHeaderFile *) buffer;

	dobData->magic = dobHeaderFile->magic;
	dobData->version = dobHeaderFile->version;
	dobData->dobSectionCount = dobHeaderFile->dobSectionCount;

#ifdef DEBUG
	printf("Magic: %i\n", dobData->magic);
	printf("Version: %i\n", dobData->version);
	printf("Section cnt: %i\n", dobData->dobSectionCount);
#endif
	dobData->dobSections = malloc(sizeof(DobSection*) * dobData->dobSectionCount);

	for (int n=0; n<dobHeaderFile->dobSectionCount; n++) {
		size_t off = sizeof(DobHeaderFile) + (n * sizeof(DobSectionFile));
#ifdef DEBUG
		printf("Off: %i\n", off);
		printf("Buffer Len: %i\n", bufferLen);
#endif
		/* SAFE */
		if (off + sizeof(DobSectionFile) > bufferLen) {
			for(int i=0; i<n; i++) {
				free(dobData->dobSections[i]->data);
				free(dobData->dobSections[i]);
			}
			free(dobData->dobSections);
			free(dobData);
			return NULL;
		}

		DobSectionFile *dobSectionFile = buffer + off;
		DobSection* dobSection = malloc(sizeof(DobSection));
		dobSection->type = dobSectionFile->type;
		dobSection->dataSize = dobSectionFile->dataSize;

#ifdef DEBUG
		printf("%i Type: %i\n", n, dobSectionFile->type);
		printf("%i Size: %i\n", n, dobSectionFile->dataSize);
#endif
		/* SAFE */
		if (dobSection->dataSize > 1024 || (dobSectionFile->dataOffset + dobSectionFile->dataSize > bufferLen)) {
			for(int i=0; i<n; i++) {
				free(dobData->dobSections[i]->data);
				free(dobData->dobSections[i]);
			}
			free(dobSection);
			free(dobData->dobSections);
			free(dobData);
			return NULL;
		}
		dobSection->data = malloc(dobSection->dataSize);
		memcpy(dobSection->data, buffer + dobSectionFile->dataOffset, dobSectionFile->dataSize);
		dobData->dobSections[n] = dobSection;
	}

	//printDobData(dobData);
	
	return dobData;
}

