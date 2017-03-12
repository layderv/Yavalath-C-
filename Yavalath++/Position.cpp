#include "stdafx.h"
#include "Position.h"
#include "Utils.h"

#include <iostream>
#include <windows.h>


using std::size_t;
using size_type = std::string::size_type;

Position::Position() : invalid_cell(new Cell(0, 0, 0)), //board(new Cell*[nRows]),
					   moves_played(0)
{
	char c;

	//for (size_t i = 0; i != nCols; ++i)
	{
		//board[i] = new Cell[nCols];
	}
	for (size_t i = 0; i != nRows; ++i)
		for (size_t j = 0; j != nCols; ++j)
		{
			c = cell_row_char(i);
			board[i][j].row = c;
			board[i][j].col = j;//cell_column(c, j);
			if (valid[i][j] == 0 &&
				((j < nCols - 1 && valid[i][j + 1] != 0) ||
				(j > 0 && valid[i][j - 1] != 0)))
				board[i][j].val = Position::border_char_cell;
			else if (valid[i][j] == 0)
				board[i][j].val = Position::unusable_char_cell;
			else
				board[i][j].val = Position::empty_char_cell;
		}

}

Position::Position(const Position & p2) : moves_played(p2.moves_played),
	last_move(p2.last_move), //board(p2.board), 
	invalid_cell(p2.invalid_cell)
{
	for (size_t i = 0; i != nRows; ++i)
		for (size_t j = 0; j != nCols; ++j)
			board[i][j] = p2.board[i][j];
}

bool Cell::operator==(const Cell& c2)
{
	return this->col == c2.col && this->row == c2.row && this->val == c2.val;
}

bool Cell::operator!=(const Cell& c2)
{
	return !(*this == c2);
}

char Position::cell_row_char(int r) const
{
	return r + 'A' - 1;
}

int Position::cell_row_int(char row) const
{
	return row - 'A' + 1;
}

int Position::cell_column(char row, int col) const
{
	int left = 5;

	if (row <= 'E') left = left + ('A' - row) + col - 1;
	else left = col;

	return left;
}

int Position::max_col_for_row(char r) const
{
	if (r <= 'E') return r - 'A' + 5;
	else return 9 - (r - 'E');
}

int Position::min_col_for_row(char r) const
{
	if (r >= 'E') return 1;
	return 5 - (r - 'A');
}

bool Position::is_border_cell(Cell & c) const
{
	if (board[cell_row_int(c.row)][cell_column(c.row, c.col)].val == Position::border_char_cell)
		return true;
	return false;
}

std::vector<std::string> Position::get_possible_moves() const
{
	std::vector<std::string> v;
	char val, row;
	std::string col;
	for (size_t i = 0; i != nRows; ++i)
	{
		val = board[i][0].val;
		row = this->cell_row_char(i);
		for (size_t j = 0; j != nCols; ++j, val = board[i][j].val)
		{
			col = std::to_string(board[i][j].col - min_col_for_row(row) + 1);
			if (val != Position::unusable_char_cell &&
				val != Position::border_char_cell &&
				val == Position::empty_char_cell)
				v.push_back(std::string(1, row) + col);
		}
	}
	return v;
}

bool Position::set(const Cell& c)
{
	last_move = board[cell_row_int(c.row)][cell_column(c.row, c.col)] = c;
	
	return true;
}

bool Position::unset()
{
	// unset on 'X'?
	board[cell_row_int(last_move.row)][cell_column(last_move.row, last_move.col)].val = 
		Position::empty_char_cell;
	return true;
}

std::vector<std::string> Position::get_board_in_strings() const
{
	std::vector<std::string> v;
	std::string row;
	for (size_t i = 0; i != nRows; ++i)
	{
		for (size_t j = 0; j != nCols; ++j)
			row.push_back(board[i][j].val);
		v.push_back(row);
		row.clear();
	}
	return v;
}

std::string Position::get_board_in_string() const
{
	std::string s;
	for (auto row : get_board_in_strings())
		s += row;
	return s;
}

std::vector<std::string> Position::get_line_in_string(CellDirection direct) const
{
	std::vector<std::string> v;
	std::string str;
	Cell cell = board[0][0];
	int d;
	switch (direct)
	{
	case NW:
	case SE:
		d = 11;
		break;
	case NE:
	case SW:
		d = 10;
		break;
	default: break;
	}
	switch (direct)
	{
	case NW:
	case SE:
	case NE:
	case SW:
		for (int k = 0; k < nCols; ++k)
		{
			for (int i = 0, j = k;
				i < nRows && j < nRows * nCols;
				j += d, ++i)
				str += board[i][j % nCols].val;

			v.push_back(str);
			str.clear();
		}
		break;
	case W:
	case E:
		for (int i = 0; i < nRows; ++i)  
		{
			for (int j = 0; j < nCols; ++j)
				str += board[i][j % nCols].val;

			v.push_back(str);
			str.clear();
		}
		break;
	default: break;
	}
	return v;
}

void Position::print_raw() const
{
	for (size_t i = 0; i != nRows; ++i)
	{
		for (size_t j = 0; j != nCols; ++j)
			std::cout << board[i][j].val;
		std::cout << std::endl;
	}
}

void Position::print(std::ostream& os) const
{
	static const std::map<std::string, int> COLORS = {
		{"RED", 12},
		{"WHITE", 7},
		{"GREEN", 10},
	};
	std::string indentLevel = "";
	char c;
	bool color;
	HANDLE output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	for (size_t i = 0; i < nRows; ++i)
	{
		if (i >= 1 && i < nRows - 1)
			os << cell_row_char(i);
		os << indentLevel + " "; // printing just one
		for (int j = 0; j < nCols; ++j)
		{
			c = board[i][j].val;
			color = c == this->last_move.val &&
				this->cell_column(last_move.row, last_move.col) == j &&
				this->cell_row_int(last_move.row) == i;
			if (color)
				SetConsoleTextAttribute(output_handle, COLORS.at("RED"));
			os << (c == border_char_cell || c == unusable_char_cell ? ' ' : c) << " ";
			if (color)
				SetConsoleTextAttribute(output_handle, COLORS.at("WHITE"));
			switch (borders[i][j])
			{
			case 4: case 5: case 7: os << "|"; break;
			default: os << " "; break;
			}
			os << " ";
		}
		os << std::endl;
		os << indentLevel;
		for (int j = 0; j < nCols; ++j)
		{
			switch (borders[i][j])
			{
			case 2: case 3: case 7: os << "\\"; break;
			default: os << " "; break;
			}
			os << " ";
			switch (borders[i][j])
			{
			case 1: case 3: case 5: case 7: os << "/"; break;
			default: os << " "; break;
			}
			os << " ";
		}
		os << std::endl;
		indentLevel += "  ";
	}

	os << std::endl;
}

Cell Position::adjacent(const Cell& c, CellDirection direct) const
{
	switch (direct)
	{
	case NW: return board[cell_row_int(c.row) - 1][Utils::abs((cell_column(c.row, c.col + 10)) % nCols)];
	case NE: return board[cell_row_int(c.row) - 1][Utils::abs((cell_column(c.row, c.col + 12)) % nCols)];
	case W:  return board[cell_row_int(c.row)][cell_column(c.row, c.col) - 1];
	case E:  return board[cell_row_int(c.row)][cell_column(c.row, c.col) + 1];
	case SW: return board[cell_row_int(c.row) + 1][(cell_column(c.row, c.col) + 10) % nCols];
	case SE: return board[cell_row_int(c.row) + 1][(cell_column(c.row, c.col) + 12) % nCols];
	}
	return *invalid_cell;
}

std::string Position::represent_cell(const Cell & c) const
{
	uint32_t r = cell_row_int(c.row);
	return std::string(1, c.row) + 
		std::to_string('A' + (c.row, board[r][c.col].col - min_col_for_row(r) + 1));
}

Cell& Position::operator[](const std::string& s)
{
	char row = s[0];
	char col = s[1];
	return board
		[cell_row_int(row)]
		[cell_column(row, std::stoi(std::string(1, col)))];
}

std::ostream& operator<<(std::ostream& os, const Position& p)
{
	p.print(os);
	return os;
}






const char Position::borders[nRows][nCols] = {
	0,0,0,0,0,1,3,3,3,3,2,
	0,0,0,0,5,7,7,7,7,7,2,
	0,0,0,5,7,7,7,7,7,7,2,
	0,0,5,7,7,7,7,7,7,7,2,
	0,5,7,7,7,7,7,7,7,7,2,
	4,7,7,7,7,7,7,7,7,7,0,
	4,7,7,7,7,7,7,7,7,0,0,
	4,7,7,7,7,7,7,7,0,0,0,
	4,7,7,7,7,7,7,0,0,0,0,
	4,7,7,7,7,7,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,
};

const char Position::valid[nRows][nCols] = {
	0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,
	0,0,0,0,1,1,1,1,1,1,0,
	0,0,0,1,1,1,1,1,1,1,0,
	0,0,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,0,0,0,
	0,1,1,1,1,1,1,0,0,0,0,
	0,1,1,1,1,1,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,
};
