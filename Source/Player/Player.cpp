#include "Player.h"
#include "Order/Orders.h"
#include "Cards/Cards.h"
#include "Map/map.h"
#include "Strategy/PlayerStrategies.h"
#include "Common/CommonTypes.h"

// SIZE_MAX should mean it will auto increment and overflow back to 0
std::size_t Player::_globalID = SIZE_MAX;

Player::Player()
{
	this->_id = ++_globalID;
	this->_hand = new Hand();
	this->_orders = new OrdersList();
}

Player::Player(std::string inPlayerName)
	: _playerName(inPlayerName)
{
	this->_id = ++_globalID;
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

	// return the existing object so we can chain this operator
	return *this;
}


const std::vector<Territory*> Player::toAttack() const
{
	return _territoriesToAttack;
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

const std::vector<Territory*> Player::toDefend() const
{
	return _territoriesToDefend;
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