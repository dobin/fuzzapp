#include <stdio.h>

#include "datastructures.h"


void handleSectionVuln1(SectionType1* section) {
	printf("Vuln 1\n");
	printf("Data: %s\n", section->data);
}
void handleSectionVuln2(SectionType2* section) {
	printf("Vuln 2\n");
	printf("Data: %s\n", section->data);
}
void handleSectionVuln3(SectionType3* section) {
	printf("Vuln 3\n");
	printf("Data: %s\n", section->data);
}


int parseDobSection(DobSection* dobSection) {
	switch(dobSection->type) {
		case 0x1:
			handleSectionVuln1((SectionType1*) dobSection->data);
			break;
		case 0x2:
			handleSectionVuln2((SectionType2*) dobSection->data);
			break;
		case 0x3:
			handleSectionVuln3((SectionType3*) dobSection->data);
			break;
		default:
			printf("Error: unknown section type: %i\n", dobSection->type);

	}

	return 0;
}


int parseDobData(DobData *dobData) {
	int ret = 0;
	printf("Parsing\n");

	for (int n=0; n<dobData->dobSectionCount; n++) {
		ret = parseDobSection(dobData->dobSections[n]);
	}
	return 0;
}



