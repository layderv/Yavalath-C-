#include "stdafx.h"

#include "Player.h"

#include <set>
#include <string>
#include <iostream>


std::string HumanPlayer::read_input()
{
	std::string input;
	bool validInput = false; // valid input (not valid move!)

	std::cout << "Reading input: " << std::endl;
	
	do
	{
		std::getline(std::cin, input);
		if (input.size() > 0)
		{
			validInput = true;
		}
		else
			std::cout << "Invalid input" << std::endl;
	} while (!validInput);
	
	return input;
}

void HumanPlayer::load(int d, int h)
{
}
