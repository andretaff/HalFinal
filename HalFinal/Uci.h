#pragma once
#include <string>
#include "Game.h"


void ucirun();
void uciEnviarComandoParaEngine(std::string comando);
void uciEnviarComandoParaInterface(std::string comando);