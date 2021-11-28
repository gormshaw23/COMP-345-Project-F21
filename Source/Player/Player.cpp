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

std::vector<Territory*> Player::toAttack()
{
	const std::vector<Territory*> allEnemyTerritories = getCurrentGameInstance()->GetEnemyTerritoriesOfCurrentPlayer(this);

	std::vector<Territory*> enemyTerritoryByPriority;
	enemyTerritoryByPriority.insert(enemyTerritoryByPriority.begin(), allEnemyTerritories.begin(), allEnemyTerritories.end());

	int toryNum = 0; 
	int priori = 0;
	std::string inputStr = "";

	while (true)
	{
		std::cout << "Enemy Territories, " << setw(10) << "Enemy Territories by Priority" << std::endl;
		std::cout << setfill('*') << setw(50) << std::endl;
		for (int i = 0; i < allEnemyTerritories.size(); ++i)
		{
			std::cout << i << ": " << allEnemyTerritories[i] << setw(10) << enemyTerritoryByPriority[i] << std::endl;
		}
		std::cout << CHANGE_TERRITORY_PRIORITY_MSG << std::endl;
		std::getline(std::cin, inputStr);
		std::stringstream myStream(inputStr);
		std::vector<std::string> words;
		std::string tmp;
		while (myStream >> tmp)
		{
			words.push_back(tmp);
		}

		if (words.size() == 2)
		{
			// handle new priority
			toryNum = std::stoi(words[0]);
			priori = std::stoi(words[1]);

			if (toryNum >= 0 && toryNum < allEnemyTerritories.size())
			{
				int toryAtPriori = std::clamp(priori, 0, (int)allEnemyTerritories.size() - 1);
				std::swap(enemyTerritoryByPriority[toryNum], enemyTerritoryByPriority[toryAtPriori]);
			}
			else
			{
				std::cout << "Please choose a valid territory." << std::endl;
			}
		}
		else if (words.size() == 1)
		{
			// check if player is done
			if (words[0].compare("done") || 
				words[0].compare("Done") ||
				words[0].compare("DONE"))
			{
				break;
			}
		}
		else
		{
			// try again bucko
			std::cout << "Please provide valid input." << std::endl;
		}
	}

	_territoriesToAttack.clear();

	_territoriesToAttack.insert(_territoriesToAttack.begin(), enemyTerritoryByPriority.begin(), enemyTerritoryByPriority.end());

	return _territoriesToAttack;
}

std::vector<Territory*> Player::toDefend()
{
	const std::vector<Territory*> currentPlayerTories = this->getTerritoriesOwned();

	std::vector<Territory*> currentPlayerToriesByPriority;
	currentPlayerToriesByPriority.insert(currentPlayerTories.begin(), currentPlayerTories.begin(), currentPlayerTories.end());

	int toryNum = 0;
	int priori = 0;
	std::string inputStr = "";

	while (true)
	{
		std::cout << "Your Territories, " << setw(10) << "Your Territories to Defend by Priority" << std::endl;
		std::cout << setfill('*') << setw(50) << std::endl;
		for (int i = 0; i < currentPlayerTories.size(); ++i)
		{
			std::cout << i << ": " << currentPlayerTories[i] << setw(10) << currentPlayerToriesByPriority[i] << std::endl;
		}
		std::cout << CHANGE_TERRITORY_PRIORITY_MSG << std::endl;
		std::getline(std::cin, inputStr);
		std::stringstream myStream(inputStr);
		std::vector<std::string> words;
		std::string tmp;
		while (myStream >> tmp)
		{
			words.push_back(tmp);
		}

		if (words.size() == 2)
		{
			// handle new priority
			toryNum = std::stoi(words[0]);
			priori = std::stoi(words[1]);

			if (toryNum >= 0 && toryNum < currentPlayerTories.size())
			{
				int toryAtPriori = std::clamp(priori, 0, (int)currentPlayerTories.size() - 1);
				std::swap(currentPlayerToriesByPriority[toryNum], currentPlayerToriesByPriority[toryAtPriori]);
			}
			else
			{
				std::cout << "Please choose a valid territory." << std::endl;
			}
		}
		else if (words.size() == 1)
		{
			// check if player is done
			if (words[0].compare("done") ||
				words[0].compare("Done") ||
				words[0].compare("DONE"))
			{
				break;
			}
		}
		else
		{
			// try again bucko
			std::cout << "Please provide valid input." << std::endl;
		}
	}

	_territoriesToDefend.clear();

	_territoriesToDefend.insert(_territoriesToAttack.begin(), currentPlayerToriesByPriority.begin(), currentPlayerToriesByPriority.end());

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