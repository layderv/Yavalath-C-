#pragma once
#include "stdafx.h"

#include <string>
#include <map>
#include <set>
#include <vector>
#include <memory>

class Cell
{
public:
	// eg: A3 X
	char row; // row 
	unsigned int col;  // col
	char val; // cell value (eg player's piece)

	Cell() = default;
	Cell(unsigned int col, char row, char val) : row(row), col(col), val(val) {}
	Cell(const Cell& c2) : row(c2.row), col(c2.col), val(c2.val) {}

	bool operator==(const Cell&);
	bool operator!=(const Cell&);

};

enum CellDirection { NW, SE, W, E, SW, NE };

class Position
{
	friend class GameEngine;
public:
	static constexpr unsigned nRows = 11;
	static constexpr unsigned nCols = 11;
	static constexpr unsigned max_possible_moves = 61;
	static const char empty_char_cell = ' ';
	static const char unusable_char_cell = '/';
	static const char border_char_cell = '|';
	std::shared_ptr<Cell> invalid_cell;

	static const char borders[nRows][nCols];
	static const char valid[nRows][nCols];

	Position();
	~Position() = default;
	Position(const Position&);

	// "A1" returns its corresponding Cell, and so on
	Cell& operator[](const std::string& s);
	friend std::ostream& operator<<(std::ostream&, const Position&);
	

	std::vector<std::string> get_possible_moves() const;
	std::vector<std::string> get_board_in_strings() const;
	std::string get_board_in_string() const;
	std::vector<std::string> get_line_in_string(CellDirection) const;
	Cell get_last_move() const { return this->last_move; }
	std::string get_last_move_str() const { return this->last_move_str; }
	uint32_t get_moves_played() const { return this->moves_played; }

	
	Cell adjacent(const Cell&, CellDirection) const;
	std::string represent_cell(const Cell&) const;

	void set_last_move_str(const std::string& v) { this->last_move_str = v; }

	bool set(const Cell&);
	bool unset();


	void print_raw() const;
	void print(std::ostream& os) const;

private:
	Cell board[nRows][nCols];
	uint32_t moves_played;
	Cell last_move;
	std::string last_move_str;

	char cell_row_char(int row) const;
	int cell_row_int(char row) const;

	// A1, .., A5, B1, .., B6, C1, .. so 1 for A5, ..
	int cell_column(char row, int col) const;

	// A: 5, B: 6, C: 7, .., I: 5
	int max_col_for_row(char r) const;
	// A begins in board[][] at [1][5], B at [2][4], ..
	int min_col_for_row(char r) const;

	bool is_border_cell(Cell& c) const;

	
	
};

std::ostream& operator<<(std::ostream& os, const Position& p);

