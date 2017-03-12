// Yavalath++.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>

#include "GameEngine.h"
#include "Player.h"
#include "Tests.h"

using namespace std;

int main()
{
	int n;
	string input;
	IPlayer *p1 = nullptr, *p2 = nullptr;
	GameEngine ge;
	bool hasChosenPlayers = false;
	bool is_testing = false;

	std::cout << "Yavalath! Players in this game can be P (human player) "
			  << "or C (computer player) or both" << std::endl;
	
	std::cout << "Run tests? y/N" << std::endl;
	std::getline(std::cin, input);
	if (input.size() > 0 && input[0] == 'y')
	{
		run_tests();
		return 0;
	}
	
	while (!hasChosenPlayers)
	{
		std::cout << "[0]: PvP\t [1]: PvC\t [2]: CvP\t[3]: CvC" << std::endl;
		std::getline(std::cin, input);
		if (input.size() == 1 && Utils::is_number(input, n))
		{
			switch (n)
			{
			case 0: p1 = new HumanPlayer; p2 = new HumanPlayer; hasChosenPlayers = true; break;
			case 1: p1 = new HumanPlayer; p2 = new ComputerPlayer; hasChosenPlayers = true; break;
			case 2: p1 = new ComputerPlayer; p2 = new HumanPlayer; hasChosenPlayers = true; break;
			case 3: p1 = new ComputerPlayer; p2 = new ComputerPlayer; hasChosenPlayers = true; break;
			default: break;
			}
		}
	}

	ge.load(p1, p2);
	ge.start(true);
	
	delete p1;
	delete p2;

    return 0;
}

