#include <iostream>
#include "windows.h"
#include "logger.h"
#include "SabaccUI.h"
#include <map>

Action choisisActionHumain(JeuSabacc& jeu, PCGAlea& rng) {
	std::vector<Action> actions;
	jeu.listerActionsLegales(actions);
	uint32_t index = 1;
	for (Action a : actions) {
		LOGDEBUG << "  " << index << " " << a << std::endl;
		index++;
	}
	index = 0;
	while (index == 0 || index > actions.size()) {
		std::string line;
		while (!std::getline(std::cin, line)) {
			index = 0;//  Error reading number of questions...
		}
		std::istringstream tmp(line);
		if (!(tmp >> index)) {
			index = 0;//  Error: input not an int...
		}
	}
	return actions[index - 1];
}



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	//active les accents et les couleurs dans la console
	SetConsoleOutputCP(CP_UTF8);	
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	
	//crée l'UI qui va s'occuper du modèle, de la view, et de tout traiter.	
	srand(time(NULL));
	SabaccUI m_UI;
	m_UI.boucle();

	LOGWARNING << "Appuyez sur Entrée pour continuer" << std::endl;
	std::cin.get();
}