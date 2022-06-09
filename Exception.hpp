#pragma once

#include <string>

inline std::string ToString(int line)
{
	char s[100];
	sprintf_s(s, "%d", line);
	return s;
}

#define THROW throw std::string(__FILE__) + std::string(":") + ToString(__LINE__)
