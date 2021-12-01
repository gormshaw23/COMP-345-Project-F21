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
	this->_playerStrategy = nullptr;
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

	this->_playerStrategy = inPlayer._playerStrategy;
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
			int threat = ownedTerritory->CalculateValue(enemyAdjacent);

			if (enemyAdjacent || ownedTerritory->getNbArmy() > 0)
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

std::vector<Territory*>& Player::getTerritoriesToDefend()
{
	return this->_territoriesToDefend;
}
const std::vector<Territory*>& Player::getTerritoriesToDefend() const
{
	return this->_territoriesToDefend;
}
std::vector<Territory*>& Player::getTerritoriesToAttack()
{
	return this->_territoriesToAttack;
}
const std::vector<Territory*>& Player::getTerritoriesToAttack() const
{
	return this->_territoriesToAttack;
}

void Player::issueOrder()
{
	// Get territories to attack in order of priority
	const std::vector<Territory*> plToriesToAttack = toAttack();
	// Get territories to defend in order of priority
	const std::vector<Territory*> plToriesToDefend = toDefend();

	// Get the player's current hand 
	Hand* currentPlayerHand = getCurrentHand();

	std::cout << "Issuing orders for " << getPlayerName() << "\n";
	setPlayerTurnPhase(EPlayerTurnPhase::PlayingReinforcementCards);

	while (getPlayerTurnPhase() != EPlayerTurnPhase::EndOfTurn)
	{
		switch (getPlayerTurnPhase())
		{
		case EPlayerTurnPhase::PlayingReinforcementCards:
			// The player starts by playing any reinforcement cards they have
			// Before they deploy their reinforcement armies
			PlayReinforcementCards();
			break;
		case EPlayerTurnPhase::DeployingArmies:
			//DisplayPlayerToriesToDefend(plToriesToDefend);
			break;
		case EPlayerTurnPhase::AdvancingArmies:
			break;
		case EPlayerTurnPhase::PlayingOtherCards:
			break;
		default:
			setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
			break;
		}
	}
}

void Player::PlayReinforcementCards()
{
	Hand* currentHand = getCurrentHand();

	if (currentHand == nullptr)
	{
		return;
	}

	std::vector<Card*> reinforcementCards;
	for (auto& card : currentHand->getHand())
	{
		if (card->getCardType() == ECardTypes::Reinforcement)
		{
			reinforcementCards.push_back(card);
		}
	}

	int reinforcementInput;
	std::string inputStr = "";

	if (reinforcementCards.size() > 0)
	{
		std::cout << "You have " << reinforcementCards.size() << " Reinforcement cards. Would you like to issue any now?" << std::endl;
		std::cout << "Type either 'yes' or 'no'" << std::endl;
		inputStr = "";
		std::getline(std::cin, inputStr);
		std::stringstream myStream(inputStr);
		std::vector<std::string> words;
		std::string tmp;
		while (myStream >> tmp)
		{
			words.push_back(tmp);
		}

		if (words[0].compare("yes") || words[0].compare("Yes") || words[0].compare("YES"))
		{
			setReinforcementPool(getReinforcementPoolSize() + REINFORCEMENT_SIZE);
		}
		else if (words[0].compare("no") || words[0].compare("No") || words[0].compare("NO"))
		{
			setPlayerTurnPhase(EPlayerTurnPhase::DeployingArmies);
		}
		else
		{
			std::cout << "Input is invalid" << std::endl;
		}
	}
	else
	{
		setPlayerTurnPhase(EPlayerTurnPhase::DeployingArmies);
	}
}

void Player::DisplayPlayerToriesToDefend(const std::vector<Territory*> inPlToriesToDefend)
{

	std::cout << "Displaying " << getPlayerName() << "'s Territories to Defend." << std::endl;
	std::cout << std::setfill('*') << std::setw(50) << std::endl;
	std::cout << "* Territory #:  " << std::setw(10) << " Territory Name " << std::setw(15) << "# of Armies" << std::endl;
	std::cout << std::setfill('*') << std::setw(50) << std::endl;
	for (int i = 0; i < inPlToriesToDefend.size(); ++i)
	{
		if (inPlToriesToDefend[i] != nullptr)
		{
			std::cout << i << std::setw(5) << " : " << inPlToriesToDefend[i] << std::setw(15) << " : " << inPlToriesToDefend[i]->getNbArmy() << std::endl;
		}
	}
}

void Player::setPlayerTurnPhase(int inPhase)
{
	switch (inPhase)
	{
	case 1:
		setPlayerTurnPhase(EPlayerTurnPhase::PlayingReinforcementCards);
		break;
	case 2:
		setPlayerTurnPhase(EPlayerTurnPhase::DeployingArmies);
		break;
	case 3:
		setPlayerTurnPhase(EPlayerTurnPhase::AdvancingArmies);
		break;
	case 4:
		setPlayerTurnPhase(EPlayerTurnPhase::PlayingOtherCards);
		break;
	default:
		setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
		break;
	}
}

void Player::setPlayerTurnPhase(EPlayerTurnPhase inPhase)
{
	this->_currentPhase = inPhase;
}

Player::EPlayerTurnPhase Player::getPlayerTurnPhase() const
{
	return this->_currentPhase;
}