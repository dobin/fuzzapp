#include <stddef.h>
#include <stdint.h>

#include "datastructures.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
	DobData* dobData = parseBuffer(data, size);
	freeDobData(dobData);
	return 0;
}
