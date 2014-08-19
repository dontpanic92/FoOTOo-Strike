#include "Log.h"
#include <cstdio>
using namespace AGE;

void Log::Error(const char* fmt){
	printf("Error %s\n", fmt);
}
