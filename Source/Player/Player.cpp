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
	this->_playerStrategy = nullptr;
	this->_currentPhase = EPlayerTurnPhase::DeployingArmies;
	this->commandProcess;
}

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
				if (adjacentTerritory->getPlayer() == nullptr)
				{
					std::cout << "Warning! Territory has no owner!" << std::endl;
					continue;
				}

				if (adjacentTerritory->getPlayer()->getPlayerID() != getPlayerID())
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
	setPlayerTurnPhase(EPlayerTurnPhase::DeployingArmies);

	while (getPlayerTurnPhase() != EPlayerTurnPhase::EndOfTurn)
	{
		switch (getPlayerTurnPhase())
		{
		case EPlayerTurnPhase::DeployingArmies:			
			DeployArmies_Human();
			break;
		case EPlayerTurnPhase::AdvancingArmies:
			AdvanceArmies_Human();
			break;
		case EPlayerTurnPhase::PlayingCards:
			PlayingCards_Human();
			break;
		case EPlayerTurnPhase::EndOfTurn:
			std::cout << "End of " << getPlayerName() << "'s turn" << std::endl;
			break;
		default:
			setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
			break;
		}
	}
}

void Player::DeployArmies_Human()
{
	Command* userCommand = nullptr;

	const std::vector<Territory*> plToriesToDefend = this->getTerritoriesToDefend();

	std::cout << "You have " << getReinforcementPoolSize() << " armies remaining to deploy." << std::endl;

	if (getReinforcementPoolSize() > 0)
	{
		
		DisplayPlayerToriesToDefend();
		std::cout << "Please select a territory from the list and the number of armies to deploy." << std::endl;
		std::cout << "Or type 'done' to skip:" << std::endl;

		std::string inputStr = GetUserInput(userCommand);
		std::stringstream myStream(inputStr);
		std::vector<std::string> words;
		std::string tmp;
		int selectedTory = 0;
		int amount = 0;
		while (myStream >> tmp)
		{
			words.push_back(tmp);
		}

		if (words.size() == 2)
		{
			if (!(std::stoi(words[0]) >> selectedTory) || !(std::stoi(words[1]) >> amount))
			{
				std::cout << "Invalid input." << std::endl;
			}
			else
			{
				amount = std::min(amount, (int)getReinforcementPoolSize());
				if (selectedTory >= 0 && selectedTory < plToriesToDefend.size())
				{
					IssueDeployOrder(plToriesToDefend[selectedTory], amount);
					HandleSaveEffect(userCommand, "Deploy order issued");
					setReinforcementPool(getReinforcementPoolSize() - amount);
				}
				else
				{
					std::cout << "Invalid territory selected input." << std::endl;
				}
			}
		}
		else if (words.size() == 1)
		{
			if (words[0].compare("done") || words[0].compare("Done") || words[0].compare("DONE"))
			{
				std::cout << "User ended deployment orders phase..." << std::endl;
				setPlayerTurnPhase(EPlayerTurnPhase::AdvancingArmies);
				HandleSaveEffect(userCommand, "Entering advancing armies step");
			}
			else
			{
				std::cout << "Invalid input." << std::endl;
			}
		}
		else
		{
			std::cout << "Invalid input." << std::endl;
		}
	}
	else
	{
		std::cout << "End of deployment orders phase..." << std::endl;
		setPlayerTurnPhase(EPlayerTurnPhase::AdvancingArmies);
		HandleSaveEffect(userCommand, "Entering advancing armies step");
	}
}

void Player::AdvanceArmies_Human()
{
	Command* userCommand = nullptr;

	const std::vector<Territory*> plToriesToDefend = this->getTerritoriesToDefend();
	const int toriesToDefendEndIndex = plToriesToDefend.size();

	const std::vector<Territory*> plToriesToAttack = this->getTerritoriesToAttack();

	std::vector<Territory*> toriesToDefendAndAttack;
	toriesToDefendAndAttack.insert(toriesToDefendAndAttack.end(), plToriesToDefend.begin(), plToriesToDefend.end());
	toriesToDefendAndAttack.insert(toriesToDefendAndAttack.end(), plToriesToAttack.begin(), plToriesToAttack.end());

	DisplayPlayerToriesToDefendAndAttack();
	std::cout << "Please select a territory from the list to move troops from," << std::endl;
	std::cout << "And select a territory from the list to move troops to." << std::endl;
	std::cout << "And select the number of troops to Advance." << std::endl;

	DisplayToriesToDefendAndAdjacencts();
	std::cout << "For convenience, here is every territory toDefend along with" << std::endl;
	std::cout << "every adjacent territory, its owner, and the number of armies present." << std::endl;

	std::string inputStr = GetUserInput(userCommand);
	std::stringstream myStream(inputStr);
	std::vector<std::string> words;
	std::string tmp;
	int selectedTorySrc = 0;
	int selectedToryDst = 0;
	int armiesToAdvance = 0;
	while (myStream >> tmp)
	{
		words.push_back(tmp);
	}

	if (words.size() == 3)
	{
		if ((std::stoi(words[0]) >> selectedTorySrc) &&
			(std::stoi(words[1]) >> selectedToryDst) &&
			(std::stoi(words[2]) >> armiesToAdvance))
		{
			if ((selectedTorySrc >= 0 && selectedTorySrc < toriesToDefendAndAttack.size()) &&
				(selectedToryDst >= 0 && selectedToryDst < toriesToDefendAndAttack.size()))
			{
				IssueAdvanceOrder(
					toriesToDefendAndAttack[selectedTorySrc],
					toriesToDefendAndAttack[selectedToryDst],
					armiesToAdvance
				);
				HandleSaveEffect(userCommand, "Advance order issued");
			}
			else
			{
				std::cout << "Invalid territory selected as input." << std::endl;
			}
		}
		else
		{
			std::cout << "Invalid input." << std::endl;
		}
	}
	else if (words.size() == 1)
	{
		if (words[0].compare("done") || words[0].compare("Done") || words[0].compare("DONE"))
		{
			std::cout << "End of advance orders phase..." << std::endl;
			setPlayerTurnPhase(EPlayerTurnPhase::PlayingCards);
			HandleSaveEffect(userCommand, "Entering playing cards step");
		}
		else
		{
			std::cout << "Invalid input." << std::endl;
		}
	}
	else
	{
		std::cout << "Invalid input." << std::endl;
	}
}

void Player::PlayingCards_Human()
{
	Command* userCommand = nullptr;

	if (this->getCurrentHand()->getHand().size() > 0)
	{
		std::cout << getPlayerName() << " has " << this->getCurrentHand()->getHand().size() << " cards to play." << std::endl;
		std::cout << "Displaying current hand:" << std::endl;

		for (int i = 0; i < this->getCurrentHand()->getHand().size(); i++)
		{
			std::cout << i << " : " << this->getCurrentHand()->getHand()[i] << std::endl;
		}

		std::cout << "Please select a card to play, or 'done' to skip." << std::endl;

		std::string inputStr = GetUserInput(userCommand);
		std::stringstream myStream(inputStr);
		std::vector<std::string> words;
		std::string tmp;
		int selectedCard = 0;

		while (myStream >> tmp)
		{
			words.push_back(tmp);
		}

		if (words.size() == 1)
		{
			if (std::stoi(words[0]) >> selectedCard)
			{
				if (selectedCard >= 0 && selectedCard < this->getCurrentHand()->getHand().size())
				{
					// play card
					std::cout << "Playing card... " << this->getCurrentHand()->getHand()[selectedCard] << std::endl;

					switch (this->getCurrentHand()->getHand()[selectedCard]->getCardType())
					{
					case ECardTypes::Bomb:
						PlayingBombCard_Human();
						break;
					case ECardTypes::Blockade:
						PlayingBlockadeCard_Human();
						break;
					case ECardTypes::Airlift:
						PlayingAirliftCard_Human();
						break;
					case ECardTypes::Diplomacy:
						PlayingDiplomacyCard_Human();
						break;
					default:
						std::cout << "Invalid card" << std::endl;
						break;
					}

					// change state
					setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
				}
				else
				{
					std::cout << "Invalid input" << std::endl;
				}
			}
			else if (words[0].compare("done") || words[0].compare("Done") || words[0].compare("DONE"))
			{
				setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
				HandleSaveEffect(userCommand, "End of the player's turn");
			}
			else
			{
				std::cout << "Invalid input" << std::endl;
			}
		}
		else
		{
			std::cout << "Invalid input" << std::endl;
		}
	}
	else
	{
		setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
	}
}

void Player::PlayingBombCard_Human()
{
	Command* userCommand = nullptr;

	const std::vector<Territory*> plToriesToAttack = this->getTerritoriesToAttack();

	DisplayPlayerToriesToAttack();

	std::cout << "Please select a territory to bomb" << std::endl;

	std::string inputStr = GetUserInput(userCommand);
	std::stringstream myStream(inputStr);
	std::vector<std::string> words;
	std::string tmp;
	int selectedToryDst = 0;
	while (myStream >> tmp)
	{
		words.push_back(tmp);
	}

	if (words.size() == 1)
	{
		if ((std::stoi(words[0]) >> selectedToryDst))
		{
			if (selectedToryDst >= 0 && selectedToryDst < plToriesToAttack.size())
			{
				IssueBombOrder(plToriesToAttack[selectedToryDst]);
				HandleSaveEffect(userCommand, "Bomb order issued");
			}
			else
			{
				std::cout << "Invalid range" << std::endl;
			}
		}
		else
		{
			std::cout << "Invalid input" << std::endl;
		}
	}
	else
	{
		std::cout << "Invalid input" << std::endl;
	}
}

void Player::PlayingBlockadeCard_Human()
{
	Command* userCommand = nullptr;

	const std::vector<Territory*> plToriesToDefend = this->getTerritoriesToDefend();

	DisplayPlayerToriesToDefend();

	std::cout << "Please select a territory to blockade" << std::endl;

	std::string inputStr = GetUserInput(userCommand);
	std::stringstream myStream(inputStr);
	std::vector<std::string> words;
	std::string tmp;
	int selectedToryDst = 0;
	while (myStream >> tmp)
	{
		words.push_back(tmp);
	}

	if (words.size() == 1)
	{
		if ((std::stoi(words[0]) >> selectedToryDst))
		{
			if (selectedToryDst >= 0 && selectedToryDst < plToriesToDefend.size())
			{
				IssueBlockadeOrder(plToriesToDefend[selectedToryDst]);
				HandleSaveEffect(userCommand, "Blockade order issued");
			}
			else
			{
				std::cout << "Invalid range" << std::endl;
			}
		}
		else
		{
			std::cout << "Invalid input" << std::endl;
		}
	}
	else
	{
		std::cout << "Invalid input" << std::endl;
	}
}

void Player::PlayingAirliftCard_Human()
{
	Command* userCommand = nullptr;

	const std::vector<Territory*> plToriesToDefend = this->getTerritoriesToDefend();

	DisplayPlayerToriesToDefend();

	std::cout << "Please select a territory to airlift troops from, a destination to airlift them to" << std::endl;
	std::cout << "and a number of troops to be airlifted." << std::endl;

	std::string inputStr = GetUserInput(userCommand);
	std::stringstream myStream(inputStr);
	std::vector<std::string> words;
	std::string tmp;
	int selectedTorySrc = 0;
	int selectedToryDst = 0;
	int armiesToAirlift = 0;
	while (myStream >> tmp)
	{
		words.push_back(tmp);
	}

	if (words.size() == 3)
	{
		if ((std::stoi(words[0]) >> selectedTorySrc) &&
			(std::stoi(words[1]) >> selectedToryDst) &&
			(std::stoi(words[2]) >> armiesToAirlift))
		{
			if ((selectedTorySrc >= 0 && selectedTorySrc < plToriesToDefend.size()) &&
				(selectedToryDst >= 0 && selectedToryDst < plToriesToDefend.size()))
			{
				IssueAirliftOrder(
					plToriesToDefend[selectedTorySrc],
					plToriesToDefend[selectedToryDst],
					armiesToAirlift
				);
				HandleSaveEffect(userCommand, "Airlift order issued");
			}
			else
			{
				std::cout << "Invalid territories selected" << std::endl;
			}
		}
		else
		{
			std::cout << "Invalid input" << std::endl;
		}
	}
	else
	{
		std::cout << "Invalid input" << std::endl;
	}
}

void Player::PlayingDiplomacyCard_Human()
{
	Command* userCommand = nullptr;

	GameEngine* gameInstance = getCurrentGameInstance();
	if (gameInstance == nullptr)
	{
		return;
	}

	const std::vector<Player*> currentPlayers = gameInstance->getPlayerList();

	for (int i = 0; i < currentPlayers.size(); ++i)
	{
		if (currentPlayers[i] != nullptr)
		{
			std::cout << i << " : " << currentPlayers[i]->getPlayerName() << std::endl;
		}
	}

	std::cout << "Please select a player to negotiate with, note that you cannot negotiate with yourself." << std::endl;

	std::string inputStr = GetUserInput(userCommand);
	std::stringstream myStream(inputStr);
	std::vector<std::string> words;
	std::string tmp;
	int selectedPlayer = 0;
	while (myStream >> tmp)
	{
		words.push_back(tmp);
	}

	if (words.size() == 1)
	{
		if ((std::stoi(words[0]) >> selectedPlayer))
		{
			if (selectedPlayer >= 0 && selectedPlayer < currentPlayers.size())
			{
				IssueNegotiateOrder(currentPlayers[selectedPlayer]);
				HandleSaveEffect(userCommand, "Negotiate order issued");
			}
			else
			{
				std::cout << "Invalid player selected" << std::endl;
			}
		}
		else
		{
			std::cout << "Invalid input" << std::endl;
		}
	}
	else
	{
		std::cout << "Invalid input" << std::endl;
	}
}

void Player::DisplayPlayerToriesToDefendAndAttack()
{
	const std::vector<Territory*> plToriesToDefend = this->getTerritoriesToDefend();
	const int toriesToDefendEndIndex = plToriesToDefend.size();

	const std::vector<Territory*> plToriesToAttack = this->getTerritoriesToAttack();

	std::vector<Territory*> toriesToDefendAndAttack;
	toriesToDefendAndAttack.insert(toriesToDefendAndAttack.end(), plToriesToDefend.begin(), plToriesToDefend.end());
	toriesToDefendAndAttack.insert(toriesToDefendAndAttack.end(), plToriesToAttack.begin(), plToriesToAttack.end());

	std::cout << "Displaying " << getPlayerName() << "'s Territories to Defend." << std::endl;
	std::cout << std::setfill('*') << std::setw(50) << std::endl;
	std::cout << std::setfill(' ') << "* Territory #:  " << std::setw(10) << " Territory Name " << std::setw(15) << "# of Armies" << std::endl;
	std::cout << std::setfill('*') << std::setw(50) << std::endl;
	std::cout << std::setfill(' ') << std::endl;
	for (int i = 0; i < toriesToDefendEndIndex; ++i)
	{
		if (toriesToDefendAndAttack[i] != nullptr)
		{
			std::cout << i << std::setw(5) << " : " << toriesToDefendAndAttack[i] << std::setw(15) << " : " << toriesToDefendAndAttack[i]->getNbArmy() << std::endl;
		}
	}

	std::cout << "Displaying " << getPlayerName() << "'s Territories to Attack." << std::endl;
	std::cout << std::setfill('*') << std::setw(50) << std::endl;
	std::cout << "* Territory #:  " << std::setw(10) << " Territory Name " << std::setw(15) << "# of Armies" << std::endl;
	std::cout << std::setfill('*') << std::setw(50) << std::endl;

	for (int i = toriesToDefendEndIndex; i < toriesToDefendAndAttack.size(); ++i)
	{
		if (toriesToDefendAndAttack[i] != nullptr)
		{
			std::cout << i << std::setw(5) << " : " << toriesToDefendAndAttack[i]->getName() << std::setw(15) << " : " << toriesToDefendAndAttack[i]->getNbArmy() << std::endl;
		}
	}
}

void Player::DisplayToriesToDefendAndAdjacencts()
{
	const std::vector<Territory*> plToriesToDefend = this->getTerritoriesToDefend();

	for (int i = 0; i < plToriesToDefend.size(); ++i)
	{
		if (plToriesToDefend[i] != nullptr)
		{
			std::cout << plToriesToDefend[i]->getName() << std::setw(10)
					  << "(" << plToriesToDefend[i]->getNbArmy() << ") : " << std::endl;

			std::vector<Territory*> allies;
			std::map<int, std::vector<Territory*>> adjacentEnemyTerritories;
			// sort adjacent territories into friendly or enemy
			for (auto& neighbour : plToriesToDefend[i]->getBorderList())
			{
				if (neighbour != nullptr)
				{
					if (neighbour->getPlayer() == this)
					{
						allies.push_back(neighbour);
					}
					else
					{
						adjacentEnemyTerritories[neighbour->getPlayer()->getPlayerID()].push_back(neighbour);
					}
				}
			}

			// display the result in a informative format
			std::cout << "[" << this->getPlayerName() << ":";
			int count = 0;
			for (auto& allyTory : allies)
			{
				std::cout << allyTory->getName() << "(" << allyTory->getNbArmy() << ")";
				if (count < allies.size())
				{
					std::cout << ",";
				}
				count++;
			}
			std::cout << "]";


			if (adjacentEnemyTerritories.size() > 0)
			{
				int adjacentEnemyCounter = 0;
				std::cout << ",";
				for (const auto& [key, value] : adjacentEnemyTerritories)
				{
					std::string plName = "";
					if (value[0] != nullptr && value[0]->getPlayer())
					{
						plName = value[0]->getPlayer()->getPlayerName();
					}
					std::cout << "[" << plName << ":";

					int enemyToryCount = 0;
					for (const auto& enemyTory : value)
					{
						if (enemyTory != nullptr)
						{
							std::cout << enemyTory->getName() << "(" << enemyTory->getNbArmy() << ")";
							if (enemyToryCount < value.size())
							{
								std::cout << ",";
							}
							enemyToryCount++;
						}
					}

					std::cout << "]";
					if (adjacentEnemyCounter < adjacentEnemyTerritories.size())
					{
						std::cout << ",";
					}
					adjacentEnemyCounter++;
				}
			}
		}
	}
}

void Player::DisplayPlayerToriesToAttack()
{
	const std::vector<Territory*> plToriesToAttack = this->getTerritoriesToAttack();

	std::cout << "Displaying " << getPlayerName() << "'s Territories to Attack." << std::endl;
	std::cout << std::setfill('*') << std::setw(50) << std::endl;
	std::cout << "* Territory #:  " << std::setw(10) << " Territory Name " << std::setw(15) << "# of Armies" << std::endl;
	std::cout << std::setfill('*') << std::setw(50) << std::endl;

	for (int i = 0; i < plToriesToAttack.size(); ++i)
	{
		if (plToriesToAttack[i] != nullptr)
		{
			std::cout << i << std::setw(5) << " : " << plToriesToAttack[i] << std::setw(15) << " : " << plToriesToAttack[i]->getNbArmy() << std::endl;
		}
	}
}

void Player::DisplayPlayerToriesToDefend()
{
	const std::vector<Territory*> plToriesToDefend = this->getTerritoriesToDefend();

	//std::cout << std::endl;
	std::cout << "Displaying " << getPlayerName() << "'s Territories to Defend." << std::endl;
	std::cout << std::setfill('*') << std::setw(50) << std::endl;
	std::cout << "* Territory #:  " << std::setw(10) << " Territory Name " << std::setw(15) << "# of Armies" << std::endl;
	std::cout << std::setfill('*') << std::setw(50) << std::endl;
	for (int i = 0; i < plToriesToDefend.size(); ++i)
	{
		if (plToriesToDefend[i] != nullptr)
		{
			std::cout << i << std::setw(5) << " : " << plToriesToDefend[i]->getName() << std::setw(15) << " : " << plToriesToDefend[i]->getNbArmy() << std::endl;
		}
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

Player::EPlayerTurnPhase Player::getPlayerTurnPhase() const
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

std::string Player::GetUserInput(Command* userCommand)
{
	if (commandProcess == nullptr)
	{
		std::cout << "Command Process isn't assigned to player." << std::endl;
		return "";
	}

	userCommand = commandProcess->getCommand();

	if (userCommand == nullptr)
	{
		std::cout << "User Command is null." << std::endl;
		return "";
	}

	return userCommand->getCommand();
}

void Player::HandleSaveEffect(Command* userCommand, std::string inMsg)
{
	if (commandProcess == nullptr || userCommand == nullptr)
	{
		std::cout << "Command Process or the user command is null." << std::endl;
		return;
	}

	userCommand->saveEffect(inMsg);
}