#pragma once
#include "stdafx.h"

#include <vector>
#include "Player.h"
#include "Position.h"
#include "Utils.h"

static const int nRows = 11, nCols = 11;
class GameEngine
{
public:

	GameEngine() = default;
	~GameEngine() = default;

	GameEngine& operator=(GameEngine) = delete;
	GameEngine(const GameEngine&) = delete;

	void load(IPlayer* p1, IPlayer* p2, 
		std::pair<int, int> d = { -1, -1 }, std::pair<int, int> h = { -1,-1 });
	void start(bool);
	bool is_move_valid(const std::string& s);
	
	bool has_game_ended(const Position& p);


private:
	Position pos;
	uint32_t playerID;
	std::vector<IPlayer*> players;
	bool winner_found;
	std::vector<std::string> all_possible_moves;

	char playersPieces[2] = { 'X', 'O' }; 

	int number_of_values_in_line(int, int, const Cell&, const Cell&) const;
	void make_move(const std::string&, const IPlayer*);

};