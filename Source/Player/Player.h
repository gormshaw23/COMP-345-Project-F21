#pragma once
// poggers cards
// soya map

#include <vector>
#include <queue>
#include <iostream>

class Card;
class Order;
class Territory;

class Player {
public:
	Player() {};

	Player(const Player& pl) { }
	Player&	operator= (const Player& pl) {}
	friend std::ostream& operator << (std::ostream& out, const Player& c);
	friend std::istream& operator >> (std::istream& in, Player& c);

	const std::vector<Territory*> toDefend() const { return _territoriesOwned; }
	const std::vector<Territory*>  toAttack() const;
	void issueOrder();
private:
	std::vector<Territory*> _territoriesOwned;
	std::vector<Card*> _cardsOwned;
	std::queue<Order*> _orders;
};

std::ostream& operator << (std::ostream& out, const Player& c)
{
	//out << c.real;
	out << "Player out" << std::endl;
	return out;
}

/*
	I imagine for the input stream its something like a series of inputs
	possibly from a file to auto setup the player class with required
	values? Need more information.
*/
std::istream& operator >> (std::istream& in, Player& c)
{
	//in >> c.real;
	//std::cout << "Player input ";
	//in >> c.imag;
	return in;
}