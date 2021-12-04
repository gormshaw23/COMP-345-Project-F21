#pragma once

class Territory;

class Utilities {
public:
	// sorts by decreasing order
	static bool SortByArmySize(const Territory* lhs, const Territory* rhs);
	// sorts by increasing order
	static bool SortByArmyAscendingSize(const Territory* lhs, const Territory* rhs);
};