#include "Engine.h"
#include <iostream>

using namespace std;

int main(){
	AGE::Engine::GetInstance()->StartUp();
	AGE::Engine::GetInstance()->Run();
}