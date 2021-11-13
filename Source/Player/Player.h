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

enum class EOrderType;

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
	bool operator==(const Player& inRHS) const;
	Player& operator= (const Player& inPlayer);
	friend std::ostream& operator<<(std::ostream& out, const Player& inPlayer);
	/* public member functions */
	void setPlayerName(std::string inPlayerName);
	const std::string getPlayerName() const;
	const std::size_t getPlayerID() const;
	const std::vector<Territory*> toDefend() const;
	const std::vector<Territory*>  toAttack() const;

	// Unreal Engine style standard for long function declarations
	void issueOrder
	(
		const EOrderType inOrderType,
		const std::vector<Territory*> inTerritories,
		const size_t inNumArmies,
		const std::vector<Player*> inPlayers
	);

	OrdersList* getOrders() const;
	Hand* getCurrentHand() const;
	std::vector<Territory*>& getTerritoriesOwned();
	std::vector<Player*>& getNotAttackablePlayers();

	void setReinforcementPool(const std::size_t inPoolSize);
	std::size_t getReinforcementPoolSize() const;

	void setCapturedTerritoryFlag(bool bInFlag);
	bool getCapturedTerritoryFlag() const;

protected:
	Hand* _hand = nullptr;
	OrdersList* _orders = nullptr;

private:
	std::size_t _id = 0;
	std::vector<Territory*> _territoriesOwned;
	std::string _playerName = "";

	std::size_t availableReinforcements = 0;
	bool bTookTerritory = false;

	std::vector<Player*> _playersNotToAttack;

	static std::size_t _globalID;
};