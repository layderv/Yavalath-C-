#pragma once
#include "stdafx.h"
#include "Position.h"
#include <functional>

class IPlayer
{
public:
	virtual ~IPlayer() {}

	void setPiece(char p) { piece = p; }
	void setID(uint32_t id) { this->id = id; }
	void setOpponentPiece(char p) { this->opponentPiece = p; }
	void set_current_position(Position* p) { this->pos = p; }

	char getPiece() const { return this->piece; }
	uint32_t getID() const { return this->id; }
	char getOpponentPiece() const { return this->opponentPiece; }
	Position& getPosition() { return *this->pos; }

	virtual void load(int d = -1, int h = -1) = 0;
	virtual std::string read_input() = 0;

private:
	char piece;
	uint32_t id;
	char opponentPiece;

protected:
	Position* pos;
	
};

class HumanPlayer : public IPlayer
{
public:
	HumanPlayer() = default;
	~HumanPlayer() {};

	std::string read_input() override;
	void load(int d = -1, int h = -1) override;

private:
	
};


enum Heuristic { Killer, History, None };

class ComputerPlayer : public IPlayer
{
public:
	ComputerPlayer() : depth(DEFAULT_DEPTH) {};
	~ComputerPlayer() {};

	std::string read_input() override;
	void load(int d = -1, int h = -1) override;

private:

	static const uint32_t DEFAULT_DEPTH = 3;
	static const uint32_t MAX_DEPTH = 5;
	static const uint32_t MIN_DEPTH = 1;
	
	std::vector<std::string> middle_positions;
	
	std::vector< std::function<
		double(const std::vector<std::vector<std::string>>&, const char, const char) 
	> > static_eval_functions;
	uint32_t depth;
	uint32_t static_eval_counter;
	Heuristic heuristic;
	

	Position eval_position;
	std::string best_move;

	std::map<std::string, int> history_heuristic_map;
	std::map<std::string, std::map<int, int>> killer_heuristic_map;

	bool take_middle();
	double static_eval(const Position&, const char, const char) const;
	double SEPatternEPPEP_0(const std::vector<std::vector<std::string>>&, const char, const char) const;
	double SEPatternEPEEP_0(const std::vector<std::vector<std::string>>&, const char, const char) const;
	double SEPatternEPPE_0(const std::vector<std::vector<std::string>>&, const char, const char) const;
	double SEPatternEPEPE_0(const std::vector<std::vector<std::string>>&, const char, const char) const;
	double SEPatternPPPP_0(const std::vector<std::vector<std::string>>&, const char, const char) const;
	double SEPatternPPPE_0(const std::vector<std::vector<std::string>>&, const char, const char) const;

	double look_for_pattern(const std::string& thisPattern, const std::string& opponentPattern,
		double patternValue, const std::vector<std::vector<std::string>>& v) const;

	std::pair<double, std::string> alphabeta(Position&, char, uint32_t, double, double, const std::string&);
};

