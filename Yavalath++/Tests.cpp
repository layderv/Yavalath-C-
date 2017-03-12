#include "stdafx.h"

#include "Player.h"
#include "GameEngine.h"
#include "Tests.h"

#include <string>
#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>

void run_tests()
{
	GameEngine* g;
	std::clock_t start_time;
	ComputerPlayer* p1, *p2;
	// Killer, History, None

	for (int i = Killer; i < None + 1; ++i)
	{
		for (int j = Killer; j < None + 1; ++j)
		{
			for (int d0 = 1; d0 < 5; ++d0)
				for (int d1 = d0; d1 < 5; ++d1) // symmetric
				{
					g = new GameEngine;
					p1 = new ComputerPlayer;
					p2 = new ComputerPlayer;

					std::cout << "Tests running with: " << std::endl;
					std::cout << "heuristics: {" << int_to_heuristic(i) << ", " 
						<< int_to_heuristic(j) << "}" << std::endl;
					std::cout << "depths: {" << d0 << ", " << d1 << "}" << std::endl;

					std::this_thread::sleep_for(std::chrono::seconds(10));
					start_time = std::clock();
					g->load(p1, p2, { d0, d1 }, { i, j });
					g->start(false);
					std::cout << "Game ended in: " <<
						(static_cast<double>(std::clock() - start_time) / CLOCKS_PER_SEC)
						<< std::endl;

					std::this_thread::sleep_for(std::chrono::seconds(10));

					delete p2;
					delete p1;
					delete g;
				}
		}
	}
}

std::string int_to_heuristic(int x)
{
	return 
		x == Killer ? std::string("Killer") : (
		x == History ? std::string("History") :
		std::string("None")
		);
}