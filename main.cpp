#define _WIN32_WINNT 0x0602  // Windows 8

#include <iostream>
#include "windows.h"
#include "logger.h"
#include "SabaccUI.h"
#include <map>

//point d'entrée
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	//active les accents et les couleurs dans la console
	//je le laisse au cas où je voudrais réutiliser la console
	//SetConsoleOutputCP(CP_UTF8);
	//HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//DWORD dwMode = 0;
	//GetConsoleMode(hOut, &dwMode);
	//SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	
	//crée l'UI qui va s'occuper du modèle, de la view, et de tout traiter.	
	srand(time(NULL));
	SabaccUI m_UI;
	m_UI.boucle();

	LOGWARNING << "Appuyez sur Entrée pour continuer" << std::endl;
	std::cin.get();
}