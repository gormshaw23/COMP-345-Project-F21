#include "Player.h"
#include "Order/Orders.h"
#include "Cards/Cards.h"
#include "Map/map.h"
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
	std::vector<Territory*> _territoriesToAttack;
	std::vector<Territory*> _tempBorder;

	Player* player = nullptr;

	// arbitrary list of territories
	_territoriesToAttack = {
		new Territory(0, 0, player, 0, 0, 0, std::string("Prov1"), _tempBorder),
		new Territory(1, 0, player, 10, 0, 1, std::string("Prov2"), _tempBorder),
		new Territory(2, 0, player, 2, 0, 2, std::string("Prov3"), _tempBorder)
	};

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

const std::vector<Territory*> Player::toDefend() const
{
	std::vector<Territory*> _territoriesToDefend;
	std::vector<Territory*> _tempBorder;

	Player* player = nullptr;

	// arbitrary list of territories
	_territoriesToDefend = {
		new Territory(3, 0, player, 3, 0, 3, std::string("Prov4"), _tempBorder),
		new Territory(4, 0, player, 5, 0, 4, std::string("Prov5"), _tempBorder)
	};

	return _territoriesToDefend;
}

void Player::issueOrder
(
	const EOrderTypes inOrderType,
	const std::vector<Territory*> inTerritories,
	const size_t inNumArmies,
	const std::vector<Player*> inPlayers
)
{
	this->_orders->add(new Order());
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

std::ostream& operator<<(std::ostream& out, const Player& inPlayer)
{

	out << "Player: " << inPlayer.getPlayerName()
		<< "("
		<< "ID: " << inPlayer._id << ", "
		<< "# of Territories Owned: " << inPlayer._territoriesOwned.size()
		<< ")";

	return out;
}