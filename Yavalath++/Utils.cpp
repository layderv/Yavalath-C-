#include "stdafx.h"
#include "Utils.h"

#include <cctype>
#include <algorithm>
#include <vector>
#include <string>

/// <summary>String to upper : 
/// s will contain every character in [a-z] as their correspondent in [A-z]</summary>
/// <param>s</param> input and output string
/// <returns>void</returns>
void Utils::toupper(std::string& s)
{
	for (auto c = s.begin(); c != s.end(); ++c)
		if (*c >= 'a' && *c <= 'z')
			*c = std::toupper(*c);
}

/// <summary>Reverse a string</summary>
/// <param>s</param> input string
/// <returns>reverse string of s</returns>
std::string Utils::reverse_string(const std::string& s)
{
	std::string r;
	for (auto c = s.crbegin(); c != s.crend(); ++c)
		r.push_back(*c);
	return r;
}

/// <summary>Remove the first occurrence of an object from a vector</summary>
/// <param>v</param> input vector
/// <param>x</param> input object
/// <returns>true iff an object has been removed</returns>
//template <typename T>
bool Utils::remove_str_from_vector(std::vector<std::string>& v, const std::string& x)
{
	for (auto it = v.begin(); it != v.end(); ++it)
		if (*it == x) { v.erase(it); return true; }
	return false;
}

/// <summary>Converts a string to a number if the string is a number</summary>
/// <param>s</param> input string
/// <param>result</param> output int result (modified iff the string is a number)
/// <returns>true iff result contains the input string as int</returns>
bool Utils::is_number(const std::string & s, int & result)
{
	short sgn = 1;
	int r = 0;

	if (s.size() == 0) return false;
	for (int i = 0; i != s.size(); ++i)
	{
		if (s[i] <= '9' && s[i] >= '0')
			r += (10 * r + s[i] - '0');
		else if (s[i] == '-' && i == 0)
			sgn = -1;
		else return false;
	}
	result = sgn * r;
	return true;
}
