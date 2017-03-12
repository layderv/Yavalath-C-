#include "stdafx.h"

#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

#include "GameEngine.h"


void GameEngine::load(IPlayer* p1, IPlayer* p2, std::pair<int, int> d, std::pair<int, int> h)
{
	// from the initial position
	this->all_possible_moves = pos.get_possible_moves();

	players.push_back(p1);
	players.push_back(p2);

	for (auto p : players)
	{
		p->setID(playerID);
		p->setPiece(playersPieces[playerID]);
		p->setOpponentPiece(playersPieces[++playerID % players.size()]);
		if (d.first > -1 && h.first > -1)
			p->load(playerID == 1 ? d.first : d.second,
					playerID == 1 ? h.first : h.second);
		else p->load();
	}
	
}

void GameEngine::start(bool wait_to_exit)
{
	bool hasGameEnded = false,
		 valid = false;

	std::string move_str;
	IPlayer* winner = nullptr;
	
	do
	{
		for (IPlayer* player : players)
		{
			std::cout << pos;
			std::cout << "Moving: player #" << player->getID() 
				<< " (" << player->getPiece() << ")" << std::endl;

			if (hasGameEnded) break;
			do
			{
				player->set_current_position(&pos);
				move_str = player->read_input();
				Utils::toupper(move_str);
				valid = is_move_valid(move_str);
				if (!valid)
				{
					std::cout << "Invalid move" << std::endl;
				}
				else
				{
					make_move(move_str, player);
				}
			} while (!valid);
			hasGameEnded = has_game_ended(pos);
			winner = player;
		}
	} while (!hasGameEnded);
	std::cout << std::endl;
	std::cout << pos;
	if (winner && this->winner_found)
		std::cout << "Winner: " << std::string(1, winner->getPiece()) << std::endl;
	else
		std::cout << "Draw" << std::endl;
	std::cout << "Enter 'q' to exit.. ";

	if (wait_to_exit)
		while (std::getchar() != 'q');
	playerID = 0;
}

bool GameEngine::is_move_valid(const std::string& s)
{
	if (s.size() == 1 && s[0] == 'X' && pos.moves_played == 1) return true;
	if (s.size() != 2) return false;
	
	return Utils::remove_str_from_vector(all_possible_moves, s);
}


bool GameEngine::has_game_ended(const Position & p)
{
	bool found;
	if (all_possible_moves.size() == 0) return true;
	// directions
	found = false;
	for (int i = NW; i < NE && !found; i += 2)
		found = 
		(1 + number_of_values_in_line(i, 0, p.get_last_move(), p.adjacent(p.get_last_move(), (CellDirection) i))
		+ number_of_values_in_line(i+1, 0, p.get_last_move(), p.adjacent(p.get_last_move(), (CellDirection)(i+1)))) >= 4;

	return this->winner_found = found;
}



int GameEngine::number_of_values_in_line(int n, int counter, const Cell& c, const Cell& iter) const
{
	if (iter.val != c.val) return counter;
	if (iter.val == c.val && counter == 3) return counter;
	return number_of_values_in_line(n, counter + 1, c, pos.adjacent(iter, (CellDirection)n));
}

void GameEngine::make_move(const std::string & m, const IPlayer* p)
{
	Cell move;
	if (m[0] == 'X' && pos.moves_played == 1)
	{
		move = pos.get_last_move();
		move.val = p->getPiece();
		pos.set(move);
	}
	else
	{
		move.col = std::stoi(std::string(1, m[1]));
		move.row = m[0];
		move.val = p->getPiece();
		pos.set(move);
	}
	Utils::remove_str_from_vector(all_possible_moves, m);
	++pos.moves_played;
}
