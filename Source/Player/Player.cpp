#include "Player.h"
#include "Order/Orders.h"
#include "Cards/Cards.h"
#include "Map/map.h"
#include "Engine/GameEngine.h"
#include "Strategy/PlayerStrategies.h"
#include "Common/CommonTypes.h"
#include "Common/localization.h"
#include "CommandProcessor/CommandProcessor.h"

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
	this->_currentPhase = EPlayerTurnPhase::DeployingArmies;
	this->commandProcess = nullptr;
}

Player::Player(PlayerStrategies* ps, std::string playerName) {
	this->_id = ++_globalID;
	this->_hand = new Hand();
	this->_orders = new OrdersList();
	this->setPlayerStrategy(ps);
	this->setPlayerName(playerName);
};

Player::Player(std::string inPlayerName)
	: _playerName(inPlayerName)
{
	this->_id = ++_globalID;
	this->currentGameInstance = nullptr;
	this->_playerStrategy = nullptr;
	this->_hand = new Hand();
	this->_orders = new OrdersList();
	this->_currentPhase = EPlayerTurnPhase::DeployingArmies;
	this->commandProcess = nullptr;
}

Player::Player(std::string inPlayerName, PlayerStrategies* inPS)
	: _playerName(inPlayerName)
{
	this->_id = ++_globalID;
	this->currentGameInstance = nullptr;
	this->_playerStrategy = inPS;
	this->_hand = new Hand();
	this->_orders = new OrdersList();
	this->_currentPhase = EPlayerTurnPhase::DeployingArmies;
	this->commandProcess = nullptr;
}

Player::Player(PlayerStrategies* inPS)
{
	this->_id = ++_globalID;
	this->_playerName = "robot" + std::to_string(this->_id);
	this->currentGameInstance = nullptr;
	this->_playerStrategy = inPS;
	this->_hand = new Hand();
	this->_orders = new OrdersList();
	this->_currentPhase = EPlayerTurnPhase::DeployingArmies;
	this->commandProcess = nullptr;
}

Player::Player(const Player& inPlayer) : Player()
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

	this->commandProcess = inPlayer.commandProcess;

	this->_currentPhase = inPlayer._currentPhase;
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

	if (this->_playerStrategy != nullptr)
	{
		delete this->_playerStrategy;
	}
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

	this->commandProcess = inPlayer.commandProcess;

	this->_currentPhase = inPlayer._currentPhase;

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

void Player::toAttack()
{
	if (this->getPlayerStrategy() != nullptr)
	{
		this->getPlayerStrategy()->toAttack();
	}
}

void Player::toDefend()
{
	if (this->getPlayerStrategy() != nullptr)
	{
		this->getPlayerStrategy()->toDefend();
	}
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
	this->_playerStrategy->setPlayer(this);
}

PlayerStrategies* Player::getPlayerStrategy() const
{
	return this->_playerStrategy;
}

const std::size_t Player::getPlayerID() const
{
	return this->_id;
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
	if (this->getPlayerStrategy() != nullptr)
	{
		getPlayerStrategy()->issueOrder();
	}
}

void Player::setPlayerTurnPhase(int inPhase)
{
	switch (inPhase)
	{
	case 1:
		setPlayerTurnPhase(EPlayerTurnPhase::DeployingArmies);
		break;
	case 2:
		setPlayerTurnPhase(EPlayerTurnPhase::AdvancingArmies);
		break;
	case 3:
		setPlayerTurnPhase(EPlayerTurnPhase::PlayingCards);
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

EPlayerTurnPhase Player::getPlayerTurnPhase() const
{
	return this->_currentPhase;
}

void Player::IssueDeployOrder(Territory* inDst, uint32 inArmiesToDeploy)
{
	if (_orders != nullptr)
	{
		_orders->add(new Deploy(this, inArmiesToDeploy, inDst));
	}
}

void Player::IssueAdvanceOrder(Territory* inSrc, Territory* inDst, uint32 inArmiesToAdvance)
{
	if (_orders != nullptr)
	{
		_orders->add(new Advance(this, inSrc, inDst, inArmiesToAdvance));
	}
}

void Player::IssueBombOrder(Territory* inDst)
{
	if (_orders != nullptr)
	{
		_orders->add(new Bomb(this, inDst));
	}
}

void Player::IssueBlockadeOrder(Territory* inDst)
{
	if (_orders != nullptr)
	{
		_orders->add(new Blockade(this->getCurrentGameInstance(), this, inDst));
	}
}

void Player::IssueAirliftOrder(Territory* inSrc, Territory* inDst, std::size_t inArmiesToAirlift)
{
	if (_orders != nullptr)
	{
		_orders->add(new Airlift(this, inSrc, inDst, inArmiesToAirlift));
	}
}

void Player::IssueNegotiateOrder(Player* inTarget)
{
	if (_orders != nullptr)
	{
		_orders->add(new Negotiate(this, inTarget));
	}
}

void Player::setCommandProcessor(CommandProcessor* inProcessor)
{
	this->commandProcess = inProcessor;
}

void Player::HandleSaveEffect(Command* userCommand, std::string inMsg)
{
	if (commandProcess == nullptr)
	{
		std::cout << "Command Process is null." << std::endl;
		return;
	}

	if (userCommand != nullptr)
	{
		userCommand->saveEffect(inMsg);
	}
}

// sets that the current player had their territory taken
void Player::setPlayerWasAttacked(bool bInFlag)
{
	this->bWasAttacked = bInFlag;
}
// gets that the current player had their territory taken
bool Player::getPlayerWasAttacked() const
{
	return this->bWasAttacked;
}

void Player::clearPlayersNotToAttack()
{
	this->_playersNotToAttack.clear();
}