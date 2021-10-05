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
	friend std::ostream& operator<<(std::ostream& out, const Player& c);
	friend std::istream& operator>>(std::istream& in, Player& c);

	const std::vector<Territory*> toDefend() const;
	const std::vector<Territory*>  toAttack() const;
	void issueOrder();
private:
	std::vector<Territory*> _territoriesOwned;
	std::vector<Card*> _cardsOwned;
	std::queue<Order*> _orders;
};