#include "Random.hpp"
#include <random>
#include <ctime>

unsigned __int64 GetRandomNumber()
{
	return std::mt19937_64((unsigned __int64)time(nullptr))();
}
