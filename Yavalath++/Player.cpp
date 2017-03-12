#include "stdafx.h"
#include "Player.h"

#include <iostream>
#include <string>

std::ostream& operator<< (std::ostream& os, const IPlayer& p)
{
	os << "Player #" << std::to_string(p.getID()) << " using " << p.getPiece() << std::endl;
	return os;
}