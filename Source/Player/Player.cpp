#include "Player.h"

const std::vector<Territory*> Player::toAttack() const
{
	return std::vector<Territory*>();
}

const std::vector<Territory*> Player::toDefend() const
{
	return std::vector<Territory*>();
}

void Player::issueOrder()
{
	Order* order;
	//this->_orders.push(new Order());
}

std::ostream& operator<<(std::ostream& out, const Player& c)
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
std::istream& operator>>(std::istream& in, Player& c)
{
	//in >> c.real;
	//std::cout << "Player input ";
	//in >> c.imag;
	return in;
}