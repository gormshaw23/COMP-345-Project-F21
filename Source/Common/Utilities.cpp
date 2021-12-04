#include "Utilities.h"
#include "Map/map.h"

bool Utilities::SortByArmySize(const Territory* lhs, const Territory* rhs)
{
	if (lhs == nullptr)
	{
		return false;
	}

	if (rhs == nullptr)
	{
		return true;
	}

	return (lhs->getNbArmy() > rhs->getNbArmy());
}

bool Utilities::SortByArmyAscendingSize(const Territory* lhs, const Territory* rhs)
{
	if (lhs == nullptr)
	{
		return false;
	}

	if (rhs == nullptr)
	{
		return true;
	}

	return (lhs->getNbArmy() <= rhs->getNbArmy());
}