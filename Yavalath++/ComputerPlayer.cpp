#include "stdafx.h"

#include "Player.h"
#include "Position.h"
#include "Utils.h"

#include <set>
#include <string>
#include <iostream>
#include <algorithm>



std::string ComputerPlayer::read_input()
{
	std::string input, move;
	bool validInput = false; // valid input (not valid move!)

	static_eval_counter = 0;
	eval_position = *pos;

	if (take_middle()) return "X";

	move = alphabeta(eval_position, this->getPiece(), 0, -INFINITY, INFINITY, "").second;

	std::cout << "Static evals: " << static_eval_counter << " for move " << move << std::endl;

	return move;
}

/// <summary>Load configurations</summary>
/// <returns>void</returns>
void ComputerPlayer::load(int d, int h)
{
	int heur = -1;
	std::string line;

	// if depth hasn't been set
	if (d == -1)
	{
		std::cout << "Max depth? [" << DEFAULT_DEPTH << "] (max:" << MAX_DEPTH << ")" << std::endl;
		std::getline(std::cin, line);

		if (Utils::is_number(line, heur) && heur >= MIN_DEPTH && heur <= MAX_DEPTH)
			depth = heur;
		else
		{
			depth = DEFAULT_DEPTH;
			std::cout << "Loading default value" << std::endl;
		}
	}
	else
		depth = d;
	
	// if heuristic hasn't been set 
	if (h == -1)
	{
		do
		{
			std::cout <<
				"[0] Killer heuristic; [1] History heuristic; [2] Neither of the preceding ones"
				<< std::endl;
			std::getline(std::cin, line);
			heur = std::stoi(line);
		} while (line.size() == 0 || heur < Killer || heur > None);
		heuristic = (Heuristic)heur;
	}
	else
		heuristic = (Heuristic)h;

	static_eval_functions.push_back([&](const std::vector<std::vector<std::string>>& v, 
		const char thisP, const char oppP)
	{
		return SEPatternEPPEP_0(v, thisP, oppP);
	});
	static_eval_functions.push_back([&](const std::vector<std::vector<std::string>>& v, 
		const char thisP, const char oppP)
	{
		return SEPatternEPEEP_0(v, thisP, oppP);
	});
	static_eval_functions.push_back([&](const std::vector<std::vector<std::string>>& v, 
		const char thisP, const char oppP)
	{
		return SEPatternEPPE_0(v, thisP, oppP);
	});
	static_eval_functions.push_back([&](const std::vector<std::vector<std::string>>& v, 
		const char thisP, const char oppP)
	{
		return SEPatternEPEPE_0(v, thisP, oppP);
	});
	static_eval_functions.push_back([&](const std::vector<std::vector<std::string>>& v, 
		const char thisP, const char oppP)
	{
		return SEPatternPPPP_0(v, thisP, oppP);
	});
	static_eval_functions.push_back([&](const std::vector<std::vector<std::string>>& v, 
		const char thisP, const char oppP)
	{
		return SEPatternPPPE_0(v, thisP, oppP);
	});

	// remember the positions in the middle, to use 'X' when possible
	// this is like an opening book
	middle_positions.push_back("E4");
	middle_positions.push_back("E5");
	middle_positions.push_back("E6");
	middle_positions.push_back("D3");
	middle_positions.push_back("D4");
	middle_positions.push_back("D5");
	middle_positions.push_back("F4");
	middle_positions.push_back("F5");

}

/// <summary></summary>
/// <returns>true iff the middle can be taken by 'X'</returns>
bool ComputerPlayer::take_middle()
{
	if (this->getPosition().get_moves_played() != 1) return false;

	for (auto it = middle_positions.cbegin(); it != middle_positions.cend(); ++it)
		if (eval_position[*it].val != Position::empty_char_cell)
			return true;
		
	return false;
}

/// <summary>Static evaluations</summary>
/// <param>p</param> current position
/// <param>thisP</param> current piece
/// <param>oppP</param> opponent piece
/// <returns>computed static evaluation</returns>
double ComputerPlayer::static_eval(const Position& p, const char thisP, const char oppP) const
{
	double result = 0.;
	std::vector<std::vector<std::string>> all_lines;
	all_lines.push_back(p.get_line_in_string(E));
	all_lines.push_back(p.get_line_in_string(SE));
	all_lines.push_back(p.get_line_in_string(SW));
	for (auto f : static_eval_functions)
		result += f(all_lines, thisP, oppP);
	
	return result;
}

// Empty-piece-piece-empty-piece (good)
double ComputerPlayer::SEPatternEPPEP_0(const std::vector<std::vector<std::string>>& v,
	const char thisP, const char oppP) const
{
	double patternValue = 34;
	double result = 0.;

	std::string thisPattern{ ' ', thisP, thisP, ' ', thisP };
	std::string oppPattern{ ' ', oppP, oppP, ' ', oppP };

	result += look_for_pattern(thisPattern, oppPattern, patternValue, v);
	result += look_for_pattern(Utils::reverse_string(thisPattern), oppPattern, patternValue, v);
	result += look_for_pattern(thisPattern, Utils::reverse_string(oppPattern), patternValue, v);
	result += look_for_pattern(Utils::reverse_string(thisPattern),
							   Utils::reverse_string(oppPattern), patternValue, v);
	return result;
}

// Empty-piece-empty-empty-piece (good)
double ComputerPlayer::SEPatternEPEEP_0(const std::vector<std::vector<std::string>>& v,
	const char thisP, const char oppP) const
{
	double patternValue = 16;
	double result = 0.;

	std::string thisPattern{ ' ', thisP, ' ', ' ', thisP };
	std::string oppPattern{ ' ', oppP, ' ', ' ', oppP };

	result += look_for_pattern(thisPattern, oppPattern, patternValue, v);
	result += look_for_pattern(Utils::reverse_string(thisPattern), oppPattern, patternValue, v);
	result += look_for_pattern(thisPattern, Utils::reverse_string(oppPattern), patternValue, v);
	result += look_for_pattern(Utils::reverse_string(thisPattern),
		Utils::reverse_string(oppPattern), patternValue, v);
	return result;
}

// Empty-piece-piece-empty (bad)
double ComputerPlayer::SEPatternEPPE_0(const std::vector<std::vector<std::string>>& v, 
	const char thisP, const char oppP) const
{
	double patternValue = 9;
	double result = 0.;

	std::string thisPattern{ ' ', thisP, thisP, ' ' };
	std::string oppPattern{ ' ', oppP, oppP, ' ' };

	return look_for_pattern(thisPattern, oppPattern, patternValue, v);
}

// Empty-piece-empty-piece-empty (bad)
double ComputerPlayer::SEPatternEPEPE_0(const std::vector<std::vector<std::string>>& v,
	const char thisP, const char oppP) const
{
	double patternValue = 3;

	std::string thisPattern{ ' ', thisP, ' ', thisP, ' ' };
	std::string oppPattern{ ' ', oppP, ' ', oppP, ' ' };

	return look_for_pattern(thisPattern, oppPattern, patternValue, v);
}

double ComputerPlayer::SEPatternPPPP_0(const std::vector<std::vector<std::string>>& v,
	const char thisP, const char oppP) const
{
	double val = 1000;

	std::string thisPattern(4, thisP);
	std::string oppPattern(4, oppP);

	return look_for_pattern(thisPattern, oppPattern, val, v);
}

double ComputerPlayer::SEPatternPPPE_0(const std::vector<std::vector<std::string>>& v,
	const char thisP, const char oppP) const
{
	double patternValue = 100, result = 0.;

	std::string thisPattern{ thisP, thisP, thisP, ' ' };
	std::string oppPattern{ oppP, oppP, oppP, ' ' };

	result += look_for_pattern(thisPattern, oppPattern, patternValue, v);
	result += look_for_pattern(Utils::reverse_string(thisPattern), oppPattern, patternValue, v);
	result += look_for_pattern(thisPattern, Utils::reverse_string(oppPattern), patternValue, v);
	result += look_for_pattern(Utils::reverse_string(thisPattern),
		Utils::reverse_string(oppPattern), patternValue, v);
	return result;
}

double ComputerPlayer::look_for_pattern(
	const std::string& thisPattern, const std::string& opponentPattern,
	double patternValue,
	const std::vector<std::vector<std::string>>& v) const
{
	double value = 0.;
	bool border = thisPattern[0] == Position::border_char_cell;
	for (const std::vector<std::string>& lines : v)
	{
		for (const std::string& line : lines)
			if (line.find(thisPattern) != std::string::npos ||
				(border && line.find((thisPattern.c_str() + 1)) != std::string::npos))
			{
				value += patternValue;
			}
			else if (line.find(opponentPattern) != std::string::npos ||
				(border && line.find((opponentPattern.c_str() + 1)) != std::string::npos))
			{
				value -= patternValue;
			}
	}
	return value;
}

/// <summary>Alphabeta heuristic</summary>
/// <param>pos</param> current position to be evaluated
/// <param>piece</param> piece to position when making moves
/// <param>d</param> depth
/// <param>a</param> alpha
/// <param>b</param> beta
/// <param>lm</param> last move
/// <returns>evaluation of the current board</returns>
std::pair<double, std::string> ComputerPlayer::alphabeta(Position& pos, char piece, uint32_t d, 
	double a, double b, const std::string& lm)
{
	std::vector<std::string> pm = pos.get_possible_moves();
	std::pair<double, std::string> tmp;
	char next_piece = piece == this->getPiece() ? this->getOpponentPiece() : this->getPiece();
	if (d == depth || pm.size() == 0)
	{
		++this->static_eval_counter;
		return{
			static_eval(pos, piece, next_piece), lm
		};
	}

	// maps already sorted
	switch (heuristic)
	{
	case Killer: std::sort(pm.begin(), pm.end(), 
		[d, &map = killer_heuristic_map](const std::string& m1, const std::string& m2) {
		return map[m1][d] < map[m2][d];
	}); break;
	case History: std::sort(pm.begin(), pm.end(),
		[d, &map = history_heuristic_map](const std::string& m1, const std::string& m2) {
		return map[m1] < map[m2];
	}); break;
	default: break;
	}
	
	for (const std::string& m : pm)
	{
		pos[m].val = next_piece;
		tmp = this->alphabeta(pos, piece, d + 1, -b, -a, m);
		tmp.first *= -1;
		pos[m].val = Position::empty_char_cell;
		if (tmp.first >= b)
		{
			// increment
			switch (heuristic)
			{
			case Killer: ++killer_heuristic_map[m][d]; break;
			case History: history_heuristic_map[m] += std::pow(2, depth - d); break;
			default: break;
			}
			return tmp;
		}
		a = std::max(tmp.first, a);
	}
	return{ a, tmp.second };
}
