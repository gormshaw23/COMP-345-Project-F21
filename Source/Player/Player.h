#pragma once
// poggers cards
// soya map

#include <vector>
#include <queue>
#include <iostream>
#include <string>

#include "Order/Orders.h"

class Card;
class Hand;
//class Order;
//class OrdersList;
class Territory;

enum class EOrderTypes;

/*
	Summary: The Player class represents the player in a game of warzone.
*/
class Player {
public:
	/* ctors */
	Player();
	Player(std::string inPlayerName);
	Player(const Player& inPlayer);
	~Player();
	/* op overrides */
	Player& operator= (const Player& inPlayer);
	friend std::ostream& operator<<(std::ostream& out, const Player& inPlayer);
	/* public member functions */
	void setPlayerName(std::string inPlayerName);
	const std::string getPlayerName() const;
	const std::vector<Territory*> toDefend(std::vector<Territory*> inTerritories) const;
	const std::vector<Territory*>  toAttack(std::vector<Territory*> inTerritories) const;

	// Unreal Engine style standard for long function declarations
	void issueOrder
	(
		const EOrderTypes inOrderType,
		const std::vector<Territory*> inTerritories,
		const std::vector<int> inNumArmies
	);

	OrdersList* getOrders() const;
	Hand* getCurrentHand() const;
	std::vector<Territory*>& getTerritoriesOwned();
	void setTerritoriesOwned(std::vector<Territory*> newTerritoriesOwned);
	int getRPool_temp(); //TEMP reinforcement pool getter
	void setRPool_temp(int newRPool_temp); //TEMP reinforcement pool setter

protected:
	Hand* _hand = nullptr;
	OrdersList* _orders = nullptr;

private:
	std::size_t _id = 0;
	std::vector<Territory*> _territoriesOwned;
	std::string _playerName = "";

	static std::size_t _globalID;
	
	int rPool_temp = 0; //TEMP Reinforcement pool
};