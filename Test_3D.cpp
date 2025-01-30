#include "includy.h"

#include "Engine.h"
#include "light.h"
#include "const.h"



/**
* @brief Funkcja Main
* Uruchamia inicjalizaję Engine, a następnie uruchamia okienko programu
*/
int main(int argc, char** argv) {

	Engine::initialize(argc, argv);
	Engine::run();
	return 0;
}