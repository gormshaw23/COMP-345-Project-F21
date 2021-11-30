#include "Player.h"
#include "Order/Orders.h"
#include "Cards/Cards.h"
#include "Map/map.h"
#include "Engine/GameEngine.h"
#include "Strategy/PlayerStrategies.h"
#include "Common/CommonTypes.h"
#include "Common/localization.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

// SIZE_MAX should mean it will auto increment and overflow back to 0
std::size_t Player::_globalID = SIZE_MAX;

Player::Player()
{
	this->_id = ++_globalID;
	this->currentGameInstance = nullptr;
	this->_playerStrategy = nullptr;
	this->_hand = new Hand();
	this->_orders = new OrdersList();
}

Player::Player(std::string inPlayerName)
	: _playerName(inPlayerName)
{
	this->_id = ++_globalID;
	this->currentGameInstance = nullptr;
	this->_playerStrategy = nullptr;
	this->_hand = new Hand();
	this->_orders = new OrdersList();
}

Player::Player(const Player& inPlayer)
{
	if (this == &inPlayer)
		return;

	// if data exists in the current object, delete it
	if (this->_hand != nullptr) delete _hand;

	if (this->_orders != nullptr) delete _orders;

	_territoriesOwned.clear();
	_territoriesOwned.shrink_to_fit();

	// copy the data from str to the implicit object
	this->_hand = new Hand(*inPlayer._hand);
	this->_orders = new OrdersList(*inPlayer._orders);

	for (int i = 0; i < inPlayer._territoriesOwned.size(); i++)
	{
		this->_territoriesOwned.push_back(new Territory(*inPlayer._territoriesOwned[i]));
	}

	this->_id = inPlayer._id;

	this->_playerName = inPlayer._playerName;

	this->currentGameInstance = inPlayer.currentGameInstance;
}

Player::~Player()
{
	delete this->_hand;
	delete this->_orders;

	_territoriesOwned.clear();
	_territoriesOwned.shrink_to_fit();

	if (this->_playerStrategy != nullptr)
	{
		delete this->_playerStrategy;
		this->_playerStrategy = nullptr;
	}

	this->currentGameInstance = nullptr;
}

bool Player::operator==(const Player& inRHS) const
{
	return this->getPlayerID() == inRHS.getPlayerID();
}

bool Player::operator!=(const Player& inRHS) const
{
	return !(*this == inRHS);
}

Player& Player::operator=(const Player& inPlayer)
{
	if (this == &inPlayer)
		return *this;

	// if data exists in the current object, delete it
	if (this->_hand != nullptr) delete _hand;

	if (this->_orders != nullptr) delete _orders;

	_territoriesOwned.clear();
	_territoriesOwned.shrink_to_fit();

	// copy the data from str to the implicit object
	this->_hand = new Hand(*inPlayer._hand);
	this->_orders = new OrdersList(*inPlayer._orders);

	for (int i = 0; i < inPlayer._territoriesOwned.size(); i++)
	{
		this->_territoriesOwned.push_back(new Territory(*inPlayer._territoriesOwned[i]));
	}

	// I'm not sure about the design of this
	this->_id = inPlayer._id;

	this->_playerName = inPlayer._playerName;

	this->currentGameInstance = inPlayer.currentGameInstance;

	// return the existing object so we can chain this operator
	return *this;
}

void Player::setCurrentGameInstance(GameEngine* inInstance)
{
	this->currentGameInstance = inInstance;
}

GameEngine* Player::getCurrentGameInstance() const
{
	return this->currentGameInstance;
}

bool SortByArmySize(const Territory* lhs, const Territory* rhs)
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

std::vector<Territory*> Player::toAttack()
{
	// toAttack will turn a list of all adjacent enemy territories, in order of territories with the most troops
	const std::vector<Territory*> allEnemyTerritories = getCurrentGameInstance()->GetEnemyTerritoriesOfCurrentPlayer(this);

	std::vector<Territory*> enemyTerritoryByPriority;

	const std::vector<Territory*> allCurrentPlayerTerritories = this->getTerritoriesOwned();

	// loop through all territories adjacent 
	for (auto& ownedTerritory : allCurrentPlayerTerritories)
	{
		if (ownedTerritory != nullptr)
		{
			for (auto& adjacentTerritory : ownedTerritory->getBorderList())
			{
				if (*adjacentTerritory->getPlayer() != *((Player*)this))
				{
					// making sure no dupes
					if (enemyTerritoryByPriority.empty() || 
						std::find
						(
							enemyTerritoryByPriority.begin(),
							enemyTerritoryByPriority.end(),
							adjacentTerritory
						) == enemyTerritoryByPriority.end())
					{
						enemyTerritoryByPriority.push_back(adjacentTerritory);
					}
				}
			}
		}
	}

	std::sort(enemyTerritoryByPriority.begin(), enemyTerritoryByPriority.end(), SortByArmySize);

	_territoriesToAttack.clear();

	_territoriesToAttack.insert(_territoriesToAttack.begin(), enemyTerritoryByPriority.begin(), enemyTerritoryByPriority.end());

	return _territoriesToAttack;
}

std::vector<Territory*> Player::toDefend()
{
	const std::vector<Territory*> currentPlayerTories = this->getTerritoriesOwned();

	std::vector<std::pair<Territory*, int>> currentPlayerToriesByPriority;

	// loop through all territories adjacent 
	for (auto& ownedTerritory : currentPlayerTories)
	{
		if (ownedTerritory != nullptr)
		{
			// if territory has an enemy territory adjacent to it OR has troops present, return it
			bool enemyAdjacent = false;
			// players are generally interested in territories where their armies are
			int threat = ownedTerritory->getNbArmy();

			for (auto& adjacentTerritory : ownedTerritory->getBorderList())
			{
				enemyAdjacent = true;
				// threat to the current territory is increased linearly according to
				// the nuber of enemy armies that are adjacent to it
				if (*adjacentTerritory->getPlayer() != *((Player*)this))
				{
					threat += adjacentTerritory->getNbArmy();
				}
			}

			if (enemyAdjacent || threat > 0)
			{
				currentPlayerToriesByPriority.push_back(std::pair<Territory*, int>(ownedTerritory, threat));
			}
		}
	}

	// hopefully sorts in descending order
	std::sort
	(
		currentPlayerToriesByPriority.begin(),
		currentPlayerToriesByPriority.end(),
		[](const std::pair<Territory*, int>& a, std::pair<Territory*, int>& b) -> bool 
		{
			return a.second > b.second;
		}
	);

	_territoriesToDefend.clear();

	for (auto& tory : currentPlayerToriesByPriority)
	{
		_territoriesToDefend.push_back(tory.first);
	}

	return _territoriesToDefend;
}

void Player::setPlayerName(std::string inPlayerName)
{
	this->_playerName = inPlayerName;
}

const std::string Player::getPlayerName() const
{
	return this->_playerName;
}

void Player::setPlayerStrategy(PlayerStrategies* inPlayerStrategy)
{
	this->_playerStrategy = inPlayerStrategy;
}

PlayerStrategies* Player::getPlayerStrategy() const
{
	return this->_playerStrategy;
}

const std::size_t Player::getPlayerID() const
{
	return this->_id;
}

/*
* Issues a player order
*
* @param inOrderType The order type
* @param inTerritories A vector of input territories
* @param inNumArmies A vector of input armies
*/
void Player::issueOrder
(
	const EOrderType inOrderType,
	const std::vector<Player*> inPlayers,
	const std::vector<Territory*> inTerritories,
	const std::vector<int> inNumArmies
)
{
	
	////Sort input territories between toAttack and toDefend
	std::vector<Territory*> territoriesToAttack;
	for (Territory* t : inTerritories)
	{
		//If the territory belongs to another player, add to territories to attack
		if (t->getPlayer()->getPlayerName().compare(this->getPlayerName()) != 0)
			territoriesToAttack.push_back(t);
	}
	this->SetTerritoriesToAttack(territoriesToAttack);

	std::vector<Territory*> territoriesToDefend;
	for (Territory* t : inTerritories)
	{
		//If the territory belongs to the player, add to territories to defend
		if (t->getPlayer()->getPlayerName().compare(this->getPlayerName()) == 0)
			territoriesToDefend.push_back(t);
	}

	this->SetTerritoriesToDefend(territoriesToDefend);

	for (Territory* t : territoriesToDefend)
	{
		switch (inOrderType) {
		case EOrderType::Deploy:
			this->_orders->add(new Deploy());
			break;

		case EOrderType::Advance:
			this->_orders->add(new Advance());
			break;

		case EOrderType::Bomb:
			this->_orders->add(new Bomb());
			break;

		case EOrderType::Blockade:
			this->_orders->add(new Blockade());
			break;

		case EOrderType::Airlift:
			this->_orders->add(new Airlift());
			break;

		case EOrderType::Negotiate:
			this->_orders->add(new Negotiate());
			break;
		}
	}

	//Create order(s) based on territories to attack
	for (Territory* t : territoriesToAttack) {
		switch (inOrderType) {
		case EOrderType::Deploy:
			this->_orders->add(new Deploy());
			break;

		case EOrderType::Advance:
			this->_orders->add(new Advance());
			break;

		case EOrderType::Bomb:
			this->_orders->add(new Bomb());
			break;

		case EOrderType::Blockade:
			this->_orders->add(new Blockade());
			break;

		case EOrderType::Airlift:
			this->_orders->add(new Airlift());
			break;

		case EOrderType::Negotiate:
			this->_orders->add(new Negotiate());
			break;
		}
	}
}

OrdersList* Player::getOrders() const
{
	return this->_orders;
}

Hand* Player::getCurrentHand() const
{
	return this->_hand;
}

void Player::setCurrentHand(Hand* newHand) {
	this->_hand = newHand;
}

std::vector<Territory*>& Player::getTerritoriesOwned()
{
	return this->_territoriesOwned;
}

std::vector<Player*>& Player::getNotAttackablePlayers()
{
	return this->_playersNotToAttack;
}

void Player::setReinforcementPool(const std::size_t inPoolSize)
{
	this->availableReinforcements = inPoolSize;
}

std::size_t Player::getReinforcementPoolSize() const
{
	return availableReinforcements;
}

void Player::setCapturedTerritoryFlag(bool bInFlag)
{
	bTookTerritory = bInFlag;
}

bool Player::getCapturedTerritoryFlag() const
{
	return bTookTerritory;
}

void Player::setTerritoriesOwned(std::vector<Territory*> newTerritoriesOwned)
{
	this->_territoriesOwned = newTerritoriesOwned;
}

std::ostream& operator<<(std::ostream& out, const Player& inPlayer)
{

	out << "Player: " << inPlayer.getPlayerName()
		<< "("
		<< "ID: " << inPlayer._id << ", "
		<< "# of Territories Owned: " << inPlayer._territoriesOwned.size()
		<< ")";

	return out;
}

void Player::AddTerritoryToAttack(Territory* inTerritoryToAttack)
{
	this->_territoriesToAttack.push_back(inTerritoryToAttack);
}

void Player::AddTerritoryToDefend(Territory* inTerritoryToDefend)
{
	this->_territoriesToDefend.push_back(inTerritoryToDefend);
}

void Player::SetTerritoriesToAttack(std::vector<Territory*> inTerritories)
{
	this->_territoriesToAttack = inTerritories;
}

void Player::SetTerritoriesToDefend(std::vector<Territory*> inTerritories)
{
	this->_territoriesToDefend = inTerritories;
}