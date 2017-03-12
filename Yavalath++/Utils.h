#pragma once
#include "stdafx.h"
#include <string>
#include <vector>

class Utils
{
public:
	static void toupper(std::string& s);
	inline static int abs(int x) { return x < 0 ? -x : x; }
	static std::string reverse_string(const std::string& s);
	static bool is_number(const std::string& s, int& result);

	//template <typename T>
	static bool remove_str_from_vector(std::vector<std::string>&, const std::string&);

};