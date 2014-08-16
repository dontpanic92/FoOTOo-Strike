#include "Log.h"
#include <cstdio>
using namespace AGE;

void Log::Error(const char* fmt){
	printf("Error in %s, line %u: %s\n", __FILE__, __LINE__, fmt);
}
