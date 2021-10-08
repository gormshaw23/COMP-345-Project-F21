#pragma once
// poggers cards
// soya map

#include <vector>
#include <queue>
#include <iostream>
#include <string>

class Card;
class Hand;
class Order;
class OrdersList;
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
	const std::vector<Territory*> toDefend() const;
	const std::vector<Territory*>  toAttack() const;

	// Unreal Engine style standard for long function declarations
	void issueOrder
	(
		const EOrderTypes inOrderType,
		const std::vector<Territory*> inTerritories,
		const size_t inNumArmies,
		const std::vector<Player*> inPlayers
	);

	OrdersList* getOrders() const;
	Hand* getCurrentHand() const;
	std::vector<Territory*>& getTerritoriesOwned();

protected:
	Hand* _hand = nullptr;
	OrdersList* _orders = nullptr;

private:
	std::size_t _id = 0;
	std::vector<Territory*> _territoriesOwned;
	std::string _playerName = "";

	static std::size_t _globalID;
};