#include "Player/Player.h"
#include "PlayerStrategies.h"
#include "Map/map.h"
#include "Engine/GameEngine.h"
#include "Cards/Cards.h"
#include "CommandProcessor/CommandProcessor.h"
#include "Common/Utilities.h"
#include "Common/CommonTypes.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <random>

Player* PlayerStrategies::getPlayer() const
{
	return this->_player;
}

void PlayerStrategies::setPlayer(Player* inPlayer)
{
	this->_player = inPlayer;
}

void PlayerStrategies::HandleSaveEffect(Command* inCommand, std::string inMsg)
{
	if (commandProcess == nullptr)
	{
		std::cout << "Command Process is null." << std::endl;
		return;
	}

	if (inCommand != nullptr)
	{
		inCommand->saveEffect(inMsg);
	}
}

HumanPlayerStrategy::HumanPlayerStrategy()
{
	this->_player = nullptr;
}

HumanPlayerStrategy::HumanPlayerStrategy(Player* inPlayer)
{
	this->_player = inPlayer;
}

void HumanPlayerStrategy::issueOrder()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	toAttack();
	toDefend();
	// Get territories to attack in order of priority
	const std::vector<Territory*> plToriesToAttack = currentPlayer->getTerritoriesToAttack();
	// Get territories to defend in order of priority
	const std::vector<Territory*> plToriesToDefend = currentPlayer->getTerritoriesToDefend();

	std::cout << "Issuing orders for " << currentPlayer->getPlayerName() << "\n";
	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::DeployingArmies);
	int availableReserves = currentPlayer->getReinforcementPoolSize();

	while (currentPlayer->getPlayerTurnPhase() != EPlayerTurnPhase::EndOfTurn)
	{
		switch (currentPlayer->getPlayerTurnPhase())
		{
		case EPlayerTurnPhase::DeployingArmies:
			DeployArmies(availableReserves);
			break;
		case EPlayerTurnPhase::AdvancingArmies:
			AdvanceArmies();
			break;
		case EPlayerTurnPhase::PlayingCards:
			PlayingCards();
			break;
		case EPlayerTurnPhase::EndOfTurn:
			std::cout << "End of " << currentPlayer->getPlayerName() << "'s turn" << std::endl;
			break;
		default:
			currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
			break;
		}
	}
}

void HumanPlayerStrategy::toAttack()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	// toAttack will turn a list of all adjacent enemy territories, in order of territories with the most troops
	const std::vector<Territory*> allEnemyTerritories = currentPlayer->getCurrentGameInstance()->GetEnemyTerritoriesOfCurrentPlayer(currentPlayer);

	std::vector<Territory*> enemyTerritoryByPriority;

	const std::vector<Territory*> allCurrentPlayerTerritories = currentPlayer->getTerritoriesOwned();

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

				if (adjacentTerritory->getPlayer()->getPlayerID() != currentPlayer->getPlayerID())
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

	std::sort(enemyTerritoryByPriority.begin(), enemyTerritoryByPriority.end(), Utilities::SortByArmySize);

	currentPlayer->getTerritoriesToAttack().clear();

	currentPlayer->getTerritoriesToAttack().insert(
		currentPlayer->getTerritoriesToAttack().begin(), 
		enemyTerritoryByPriority.begin(), 
		enemyTerritoryByPriority.end()
	);

}

void HumanPlayerStrategy::toDefend()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	const std::vector<Territory*> currentPlayerTories = currentPlayer->getTerritoriesOwned();

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

	currentPlayer->getTerritoriesToDefend().clear();

	for (auto& tory : currentPlayerToriesByPriority)
	{
		currentPlayer->getTerritoriesToDefend().push_back(tory.first);
	}
}

void HumanPlayerStrategy::DeployArmies(int& availableReserves)
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	Command* userCommand = nullptr;

	const std::vector<Territory*> plToriesToDefend = currentPlayer->getTerritoriesToDefend();

	std::map<int, Territory*> toriesForDeploy;
	for (const auto& tory : plToriesToDefend)
	{
		toriesForDeploy[tory->getID()] = tory;
	}

	std::cout << "You have " << availableReserves << " armies remaining to deploy." << std::endl;

	if (availableReserves > 0)
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
			if ((std::stringstream(words[0]) >> selectedTory) && (std::stringstream(words[1]) >> amount))
			{
				amount = std::max(0, amount);

				Territory* dst = nullptr;

				std::map<int, Territory*>::iterator itor;

				itor = toriesForDeploy.find(selectedTory);

				if (itor != toriesForDeploy.end())
				{
					dst = toriesForDeploy[selectedTory];
				}

				currentPlayer->IssueDeployOrder(dst, amount);
				HandleSaveEffect(userCommand, "Deploy order issued");
				availableReserves -= amount;
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
				std::cout << "User ended deployment orders phase..." << std::endl;
				currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::AdvancingArmies);
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
		currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::AdvancingArmies);
		//HandleSaveEffect(userCommand, "Entering advancing armies step");
	}
}

void HumanPlayerStrategy::AdvanceArmies()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	Command* userCommand = nullptr;

	const std::vector<Territory*> plToriesToDefend = currentPlayer->getTerritoriesToDefend();
	const int toriesToDefendEndIndex = plToriesToDefend.size();

	const std::vector<Territory*> plToriesToAttack = currentPlayer->getTerritoriesToAttack();

	std::vector<Territory*> toriesToDefendAndAttack;
	toriesToDefendAndAttack.insert(toriesToDefendAndAttack.end(), plToriesToDefend.begin(), plToriesToDefend.end());
	toriesToDefendAndAttack.insert(toriesToDefendAndAttack.end(), plToriesToAttack.begin(), plToriesToAttack.end());

	std::map<int, Territory*> toriesForAdvance;
	for (const auto& tory : toriesToDefendAndAttack)
	{
		toriesForAdvance[tory->getID()] = tory;
	}

	DisplayPlayerToriesToDefendAndAttack();
	std::cout << "Please select a territory from the list to move troops from," << std::endl;
	std::cout << "And select a territory from the list to move troops to." << std::endl;
	std::cout << "And select the number of troops to Advance." << std::endl;

	DisplayToriesToDefendAndAdjacents();
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
		if ((std::stringstream(words[0]) >> selectedTorySrc) &&
			(std::stringstream(words[1]) >> selectedToryDst) &&
			(std::stringstream(words[2]) >> armiesToAdvance))
		{
			Territory* src = nullptr;
			Territory* dst = nullptr;

			std::map<int, Territory*>::iterator srcItor;
			std::map<int, Territory*>::iterator dstItor;

			srcItor = toriesForAdvance.find(selectedTorySrc);

			if (srcItor != toriesForAdvance.end())
			{
				src = toriesForAdvance[selectedTorySrc];
			}

			dstItor = toriesForAdvance.find(selectedToryDst);

			if (dstItor != toriesForAdvance.end())
			{
				dst = toriesForAdvance[selectedToryDst];
			}

			currentPlayer->IssueAdvanceOrder(
				src,
				dst,
				std::max(0, armiesToAdvance)
			);

			HandleSaveEffect(userCommand, "Advance order issued");
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
			currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::PlayingCards);
			//HandleSaveEffect(userCommand, "Entering playing cards step");
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

void HumanPlayerStrategy::PlayingCards()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	if (currentPlayer->getCurrentHand() == nullptr)
	{
		return;
	}

	Hand* currentHand = currentPlayer->getCurrentHand();

	Command* userCommand = nullptr;

	if (currentHand->getHand().size() > 0)
	{
		std::cout << currentPlayer->getPlayerName() << " has " << currentHand->getHand().size()
			<< " cards to play." << std::endl;
		std::cout << "Displaying current hand:" << std::endl;

		for (int i = 0; i < currentHand->getHand().size(); i++)
		{
			if (currentHand->getHand()[i] != nullptr)
			{
				std::cout << i << " : " << *currentHand->getHand()[i] << std::endl;
			}
			else
			{
				std::cout << "A card in the player's hand is null." << std::endl;
			}
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
			if ((std::stringstream(words[0]) >> selectedCard))
			{
				if (selectedCard >= 0 && selectedCard < currentHand->getHand().size())
				{
					// play card
					std::cout << "Playing card... " << *currentHand->getHand()[selectedCard] << std::endl;

					switch (currentHand->getHand()[selectedCard]->getCardType())
					{
					case ECardTypes::Bomb:
						PlayingBombCard();
						break;
					case ECardTypes::Blockade:
						PlayingBlockadeCard();
						break;
					case ECardTypes::Airlift:
						PlayingAirliftCard();
						break;
					case ECardTypes::Diplomacy:
						PlayingDiplomacyCard();
						break;
					default:
						std::cout << "Invalid card" << std::endl;
						break;
					}

					delete currentHand->getHand()[selectedCard];

					currentHand->getHand().erase(
						currentHand->getHand().begin(),
						currentHand->getHand().begin() + selectedCard
					);

					// change state
					currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
				}
				else
				{
					std::cout << "Invalid input" << std::endl;
				}
			}
			else if (words[0].compare("done") || words[0].compare("Done") || words[0].compare("DONE"))
			{
				currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
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
		currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
	}
}

void HumanPlayerStrategy::PlayingBombCard()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	Command* userCommand = nullptr;

	const std::vector<Territory*> plToriesToAttack = currentPlayer->getTerritoriesToAttack();

	std::map<int, Territory*> toriesForBombing;
	for (const auto& tory : plToriesToAttack)
	{
		toriesForBombing[tory->getID()] = tory;
	}

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
		if ((std::stringstream(words[0]) >> selectedToryDst))
		{
			Territory* dst = nullptr;

			std::map<int, Territory*>::iterator dstItor;

			dstItor = toriesForBombing.find(selectedToryDst);

			if (dstItor != toriesForBombing.end())
			{
				dst = toriesForBombing[selectedToryDst];
			}

			currentPlayer->IssueBombOrder(dst);
			HandleSaveEffect(userCommand, "Bomb order issued");
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

void HumanPlayerStrategy::PlayingBlockadeCard()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	Command* userCommand = nullptr;

	const std::vector<Territory*> plToriesToDefend = currentPlayer->getTerritoriesToDefend();

	std::map<int, Territory*> toriesForBlockading;
	for (const auto& tory : plToriesToDefend)
	{
		toriesForBlockading[tory->getID()] = tory;
	}

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
		if ((std::stringstream(words[0]) >> selectedToryDst))
		{
			Territory* dst = nullptr;

			std::map<int, Territory*>::iterator dstItor;

			dstItor = toriesForBlockading.find(selectedToryDst);

			if (dstItor != toriesForBlockading.end())
			{
				dst = toriesForBlockading[selectedToryDst];
			}

			currentPlayer->IssueBlockadeOrder(dst);
			HandleSaveEffect(userCommand, "Blockade order issued");
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

void HumanPlayerStrategy::PlayingAirliftCard()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	Command* userCommand = nullptr;

	const std::vector<Territory*> plToriesToDefend = currentPlayer->getTerritoriesToDefend();

	std::map<int, Territory*> toriesForAirlifting;
	for (const auto& tory : plToriesToDefend)
	{
		toriesForAirlifting[tory->getID()] = tory;
	}

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
		if ((std::stringstream(words[0]) >> selectedTorySrc) &&
			(std::stringstream(words[1]) >> selectedToryDst) &&
			(std::stringstream(words[2]) >> armiesToAirlift))
		{
			Territory* src = nullptr;
			Territory* dst = nullptr;

			std::map<int, Territory*>::iterator srcItor;
			std::map<int, Territory*>::iterator dstItor;

			srcItor = toriesForAirlifting.find(selectedTorySrc);

			if (srcItor != toriesForAirlifting.end())
			{
				src = toriesForAirlifting[selectedTorySrc];
			}

			dstItor = toriesForAirlifting.find(selectedToryDst);

			if (dstItor != toriesForAirlifting.end())
			{
				dst = toriesForAirlifting[selectedToryDst];
			}

			currentPlayer->IssueAirliftOrder(
				src,
				dst,
				std::max(0, armiesToAirlift)
			);
			HandleSaveEffect(userCommand, "Airlift order issued");
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

void HumanPlayerStrategy::PlayingDiplomacyCard()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	Command* userCommand = nullptr;

	GameEngine* gameInstance = currentPlayer->getCurrentGameInstance();
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
		if ((std::stringstream(words[0]) >> selectedPlayer))
		{
			if (selectedPlayer >= 0 && selectedPlayer < currentPlayers.size())
			{
				currentPlayer->IssueNegotiateOrder(currentPlayers[selectedPlayer]);
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

void HumanPlayerStrategy::DisplayPlayerToriesToDefendAndAttack()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	const std::vector<Territory*> plToriesToDefend = currentPlayer->getTerritoriesToDefend();
	const int toriesToDefendEndIndex = plToriesToDefend.size();

	const std::vector<Territory*> plToriesToAttack = currentPlayer->getTerritoriesToAttack();

	std::vector<Territory*> toriesToDefendAndAttack;
	toriesToDefendAndAttack.insert(toriesToDefendAndAttack.end(), plToriesToDefend.begin(), plToriesToDefend.end());
	toriesToDefendAndAttack.insert(toriesToDefendAndAttack.end(), plToriesToAttack.begin(), plToriesToAttack.end());

	std::cout << "Displaying " << currentPlayer->getPlayerName() << "'s Territories to Defend & Attack." << std::endl;
	std::cout << std::setfill('*') << std::setw(50) << "" << std::endl;
	std::cout << std::setfill(' ') << "* Territory #:  " << std::setw(10) << " Territory Name " << std::setw(15) << "# of Armies" << std::endl;
	std::cout << std::setfill('*') << std::setw(50) << "" << std::endl;
	std::cout << std::setfill(' ') << std::endl;
	for (int i = 0; i < toriesToDefendEndIndex; ++i)
	{
		if (toriesToDefendAndAttack[i] != nullptr)
		{
			std::cout << toriesToDefendAndAttack[i]->getID() << std::setw(5 - std::to_string(i).size()) << " : " << std::setw(40) << toriesToDefendAndAttack[i]->getName() << " : " << toriesToDefendAndAttack[i]->getNbArmy() << std::endl;
		}
		else
		{
			std::cout << "(1) Territory in DisplayPlayerToriesToDefendAndAttack is null" << std::endl;
		}
	}

	std::cout << std::endl;

	std::cout << "Displaying " << currentPlayer->getPlayerName() << "'s Territories to Attack." << std::endl;
	std::cout << std::setfill('*') << std::setw(50) << "" << std::endl;
	std::cout << std::setfill(' ') << "* Territory #:  " << std::setw(10) << " Territory Name " << std::setw(15) << "# of Armies" << std::endl;
	std::cout << std::setfill('*') << std::setw(50) << "" << std::endl;
	std::cout << std::setfill(' ') << std::endl;

	for (int i = toriesToDefendEndIndex; i < toriesToDefendAndAttack.size(); ++i)
	{
		if (toriesToDefendAndAttack[i] != nullptr)
		{
			std::cout << toriesToDefendAndAttack[i]->getID() << std::setw(5 - std::to_string(i).size()) << " : " << std::setw(40) << toriesToDefendAndAttack[i]->getName() << " : " << toriesToDefendAndAttack[i]->getNbArmy() << std::endl;
		}
		else
		{
			std::cout << "(2) Territory in DisplayPlayerToriesToDefendAndAttack is null" << std::endl;
		}
	}

	std::cout << std::endl;
}

void HumanPlayerStrategy::DisplayToriesToDefendAndAdjacents()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	std::cout << std::setfill(' ');

	const std::vector<Territory*> plToriesToDefend = currentPlayer->getTerritoriesToDefend();

	for (int i = 0; i < plToriesToDefend.size(); ++i)
	{
		if (plToriesToDefend[i] != nullptr)
		{
			std::cout << "{ID:" << plToriesToDefend[i]->getID() << "}" << plToriesToDefend[i]->getName()
				<< "(units:" << plToriesToDefend[i]->getNbArmy() << "): " << std::endl;

			std::vector<Territory*> allies;
			std::map<int, std::vector<Territory*>> adjacentEnemyTerritories;
			// sort adjacent territories into friendly or enemy
			for (const auto& neighbour : plToriesToDefend[i]->getBorderList())
			{
				if (neighbour != nullptr)
				{
					if (neighbour->getPlayer() == nullptr)
					{
						std::cout << "Territory not assigned to a player: DisplayToriesToDefendAndAdjacents." << std::endl;
						continue;
					}

					if (neighbour->getPlayer()->getPlayerID() == currentPlayer->getPlayerID())
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
			if (allies.size() > 0)
			{
				std::cout << "\t\t[" << currentPlayer->getPlayerName() << ":";
				int count = 0;
				for (auto& allyTory : allies)
				{
					std::cout << "{ID:" << allyTory->getID() << "}:" << allyTory->getName() << "(units:"
						<< allyTory->getNbArmy() << ")";
					if (count < allies.size() - 1)
					{
						std::cout << ",";
					}
					count++;
				}
				std::cout << "]" << std::endl;
			}

			if (adjacentEnemyTerritories.size() > 0)
			{
				/*int adjacentEnemyCounter = 0;*/
				for (const auto& [key, value] : adjacentEnemyTerritories)
				{
					std::string plName = "";
					if (value[0] != nullptr && value[0]->getPlayer())
					{
						plName = value[0]->getPlayer()->getPlayerName();
					}
					std::cout << "\t\t[" << plName << ":";

					int enemyToryCount = 0;
					for (const auto& enemyTory : value)
					{
						if (enemyTory != nullptr)
						{
							std::cout << "{ID:" << enemyTory->getID() << "}:" << enemyTory->getName()
								<< "(units:" << enemyTory->getNbArmy() << ")";
							if (enemyToryCount < value.size() - 1)
							{
								std::cout << ",";
							}
							enemyToryCount++;
						}
					}

					std::cout << "]";
				}
			}

			std::cout << std::endl;
		}
	}

	std::cout << std::endl;
}

void HumanPlayerStrategy::DisplayPlayerToriesToAttack()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	const std::vector<Territory*> plToriesToAttack = currentPlayer->getTerritoriesToAttack();

	std::cout << "Displaying Territories " << currentPlayer->getPlayerName() << " can Attack." << std::endl;
	std::cout << std::setfill('*') << std::setw(50) << "" << std::endl;
	std::cout << std::setfill(' ') << "* Territory #:  " << std::setw(10) << " Territory Name " << std::setw(15) << "# of Armies" << std::endl;
	std::cout << std::setfill('*') << std::setw(50) << "" << std::endl;
	std::cout << std::setfill(' ') << std::endl;

	for (int i = 0; i < plToriesToAttack.size(); ++i)
	{
		if (plToriesToAttack[i] != nullptr)
		{
			std::cout << plToriesToAttack[i]->getID() << std::setw(5 - std::to_string(i).size()) << " : " << std::setw(40) << plToriesToAttack[i]->getName() << " : " << plToriesToAttack[i]->getNbArmy() << std::endl;
		}
		else
		{
			std::cout << "A territory is null" << std::endl;
		}
	}

	std::cout << std::endl;
}

void HumanPlayerStrategy::DisplayPlayerToriesToDefend()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	const std::vector<Territory*> plToriesToDefend = currentPlayer->getTerritoriesToDefend();

	//std::cout << std::endl;
	std::cout << "Displaying " << currentPlayer->getPlayerName() << "'s Territories to Defend." << std::endl;
	std::cout << std::setfill('*') << std::setw(100) << "" << std::endl;
	std::cout << std::setfill(' ') << "* Territory #:  " << std::setw(10) << " Territory Name "
		<< std::setw(15) << "# of Armies" << std::setw(5) << "Adjacent Enemy Territories" << std::endl;
	std::cout << std::setfill('*') << std::setw(100) << "" << std::endl;
	std::cout << std::setfill(' ') << std::endl;
	for (int i = 0; i < plToriesToDefend.size(); ++i)
	{
		if (plToriesToDefend[i] != nullptr)
		{
			std::cout << plToriesToDefend[i]->getID() << std::setw(5 - std::to_string(i).size()) << " : "
				<< plToriesToDefend[i]->getName() << " : " << plToriesToDefend[i]->getNbArmy()
				<< std::endl;

			std::map<int, std::vector<Territory*>> neighbourPlTories;

			for (const auto& neighbour : plToriesToDefend[i]->getBorderList())
			{
				if (neighbour == nullptr || neighbour->getPlayer() == nullptr)
				{
					std::cout << "A neighbour territory is null" << std::endl;
					continue;
				}

				if (neighbour->getPlayer() == nullptr)
				{
					std::cout << "A neighbour territory player is null" << std::endl;
					continue;
				}

				neighbourPlTories[neighbour->getPlayer()->getPlayerID()].push_back(neighbour);
			}

			for (const auto& [key, value] : neighbourPlTories)
			{
				std::cout << std::setw(35);
				std::cout << value[0]->getPlayer()->getPlayerName() << ":" << std::endl;
				for (const auto& tory : value)
				{
					std::cout << std::setw(35 + tory->getName().size());
					std::cout << tory->getName() << "(" << tory->getNbArmy() << ")" << std::endl;
				}
			}
		}
		else
		{
			std::cout << "A territory in DisplayPlayerToriesToDefend is null." << std::endl;
		}
	}

	std::cout << std::endl;
}

std::string HumanPlayerStrategy::GetUserInput(Command*& userCommand)
{
	if (commandProcess == nullptr)
	{
		std::cout << "Command Process isn't assigned to player." << std::endl;
		return "";
	}

	userCommand = commandProcess->getCommand();

	if (&userCommand == nullptr)
	{
		std::cout << "User Command is null." << std::endl;
		return "";
	}

	return userCommand->getCommand();
}

/*
	******************************************
	***** AI Controlled Players Below!!! *****
	******************************************
*/

void AggressivePlayerStrategy::issueOrder()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	toAttack();
	toDefend();
	// Get territories to attack in order of priority
	const std::vector<Territory*> plToriesToAttack = currentPlayer->getTerritoriesToAttack();
	// Get territories to defend in order of priority
	const std::vector<Territory*> plToriesToDefend = currentPlayer->getTerritoriesToDefend();

	std::cout << "Issuing orders for " << currentPlayer->getPlayerName() << "\n";
	std::cout << "They are a CPU! " << std::endl;

	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::DeployingArmies);
	int availableReserves = currentPlayer->getReinforcementPoolSize();
	currentPlayer->numDeployments = 0;

	while (currentPlayer->getPlayerTurnPhase() != EPlayerTurnPhase::EndOfTurn)
	{
		switch (currentPlayer->getPlayerTurnPhase())
		{
		case EPlayerTurnPhase::DeployingArmies:
			DeployArmies(availableReserves);
			break;
		case EPlayerTurnPhase::AdvancingArmies:
			AdvanceArmies();
			break;
		case EPlayerTurnPhase::PlayingCards:
			PlayingCards();
			break;
		case EPlayerTurnPhase::EndOfTurn:
			std::cout << "End of " << currentPlayer->getPlayerName() << "'s turn" << std::endl;
			break;
		default:
			currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
			break;
		}
	}
}

void AggressivePlayerStrategy::toAttack()
{
	/*
		The Aggressive player will prioritize territories with the least troops to Advance
	*/

	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	// toAttack will turn a list of all adjacent enemy territories, in order of territories with the least troops
	std::vector<Territory*> enemyTerritoryByPriority;

	const std::vector<Territory*> allCurrentPlayerTerritories = currentPlayer->getTerritoriesOwned();

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

				if (adjacentTerritory->getPlayer()->getPlayerID() != currentPlayer->getPlayerID())
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

	std::sort(enemyTerritoryByPriority.begin(), enemyTerritoryByPriority.end(), Utilities::SortByArmyAscendingSize);

	currentPlayer->getTerritoriesToAttack().clear();

	currentPlayer->getTerritoriesToAttack().insert(
		currentPlayer->getTerritoriesToAttack().begin(),
		enemyTerritoryByPriority.begin(),
		enemyTerritoryByPriority.end()
	);
}

void AggressivePlayerStrategy::toDefend()
{
	/*
		Aggressive player will prioritize territories with the most troops to deploy troops
	*/
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	const std::vector<Territory*> currentPlayerTories = currentPlayer->getTerritoriesOwned();

	std::vector<Territory*> plToriesSorted;
	plToriesSorted.insert(plToriesSorted.begin(), currentPlayerTories.begin(), currentPlayerTories.end());
	// sorts player's territories in descending order by troop numbers
	std::sort(plToriesSorted.begin(), plToriesSorted.end(), Utilities::SortByArmySize);

	currentPlayer->getTerritoriesToDefend().clear();
	currentPlayer->getTerritoriesToDefend().insert(
		currentPlayer->getTerritoriesToDefend().begin(), currentPlayerTories.begin(), currentPlayerTories.end()
	);
}

void AggressivePlayerStrategy::DeployArmies(int& inAvailableReserves)
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	if (inAvailableReserves <= 0)
	{
		currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::AdvancingArmies);
		return;
	}

	if (currentPlayer->getTerritoriesToDefend().size() > 0)
	{
		const int numTerritoriesToDefend = currentPlayer->getTerritoriesToDefend().size();
		const int totalAvailableReserves = inAvailableReserves;

		int index = currentPlayer->numDeployments % currentPlayer->getTerritoriesToDefend().size();

		int enemyAdjTories = 0;
		int enemyThreat = 0;
		std::vector<Territory*> neighbours = currentPlayer->getTerritoriesToDefend()[index]->getBorderList();
		for (auto& adjTory : neighbours)
		{
			if (adjTory != nullptr)
			{
				if (adjTory->getPlayer() != currentPlayer)
				{
					enemyAdjTories++;
					enemyThreat += adjTory->getNbArmy();
				}
			}
		}

		enemyThreat = std::max(0, enemyThreat - currentPlayer->getTerritoriesToDefend()[index]->getNbArmy());

		int numToDeploy = std::max(0, std::min(totalAvailableReserves, std::max(5, 2 + enemyAdjTories + enemyThreat)));

		currentPlayer->IssueDeployOrder(
			currentPlayer->getTerritoriesToDefend()[index],
			numToDeploy
		);

		inAvailableReserves -= numToDeploy;

		currentPlayer->numDeployments++;
	}
	else
	{
		currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::AdvancingArmies);
	}
}

void AggressivePlayerStrategy::AdvanceArmies()
{
	// find territories with troops and advance them into player territories with the least
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	for (const auto& plTory : currentPlayer->getTerritoriesToDefend())
	{
		if (plTory->getNbArmy() > 0)
		{
			std::vector<Territory*> adjacents;
			adjacents.insert(adjacents.begin(), plTory->getBorderList().begin(), plTory->getBorderList().end());
			std::sort(adjacents.begin(), adjacents.end(), Utilities::SortByArmyAscendingSize);

			bool enemyfound = false;
			for (const auto& adjTory : adjacents)
			{
				if (adjTory->getPlayer() != currentPlayer)
				{
					enemyfound = true;
					currentPlayer->IssueAdvanceOrder(plTory, adjTory, plTory->getNbArmy());
				}
			}

			// if there's no enemy then just move troops to territory with the most armies
			if (!enemyfound)
			{
				Territory* dst = nullptr;
				if (adjacents.size() > 0)
				{
					dst = adjacents[adjacents.size() - 1];
				}
				currentPlayer->IssueAdvanceOrder(plTory, dst, plTory->getNbArmy());
			}
		}
	}

	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::PlayingCards);
}

void AggressivePlayerStrategy::PlayingCards()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	if (currentPlayer->getCurrentHand() == nullptr)
	{
		return;
	}

	Command* userCommand = nullptr;

	// AI will play a card at random
	if (currentPlayer->getCurrentHand()->getHand().size() > 0)
	{
		Hand* hand = currentPlayer->getCurrentHand();
		auto randomClock = std::chrono::high_resolution_clock::now();
		auto seed = randomClock.time_since_epoch().count();
		auto randomizer = std::default_random_engine(seed);
		std::shuffle(hand->getHand().begin(), hand->getHand().end(), randomizer);

		Card* card = hand->drawCard_Hand();

		switch (card->getCardType())
		{
		case ECardTypes::Bomb:
			PlayingBombCard();
			break;
		case ECardTypes::Blockade:
			PlayingBlockadeCard();
			break;
		case ECardTypes::Airlift:
			PlayingAirliftCard();
			break;
		case ECardTypes::Diplomacy:
			PlayingDiplomacyCard();
			break;
		default:
			std::cout << "Invalid card" << std::endl;
			break;
		}

		delete card;
	}
	// change state
	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
}

void AggressivePlayerStrategy::PlayingBombCard()
{
	// Aggressive player will bomb the neighbour territory with the most troops
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	Command* userCommand = nullptr;

	const std::vector<Territory*> plToriesToAttack = currentPlayer->getTerritoriesToAttack();

	if (plToriesToAttack.size() > 0)
	{
		currentPlayer->IssueBombOrder(plToriesToAttack[plToriesToAttack.size() - 1]);
	}
}

void AggressivePlayerStrategy::PlayingBlockadeCard()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	const std::vector<Territory*> currentPlayerBorderTories = currentPlayer->getTerritoriesToDefend();

	// I think the smart decision is to blow up your territory that's at most risk of
	// falling to the enemy
	std::vector<std::pair<Territory*, int>> currentPlayerToriesByPriority;

	// loop through all territories adjacent 
	for (auto& ownedTerritory : currentPlayerBorderTories)
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

	// sorts in descending order
	std::sort
	(
		currentPlayerToriesByPriority.begin(),
		currentPlayerToriesByPriority.end(),
		[](const std::pair<Territory*, int>& a, std::pair<Territory*, int>& b) -> bool
		{
			return a.second > b.second;
		}
	);

	// pick first element (one with highest threat) to blockade
	if (currentPlayerToriesByPriority.size() > 0)
	{
		currentPlayer->IssueBlockadeOrder(currentPlayerToriesByPriority[0].first);
	}
}

void AggressivePlayerStrategy::PlayingAirliftCard()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	// find all territories that aren't on the border
	std::vector<Territory*> innerTories;

	const std::vector<Territory*> plTories = currentPlayer->getTerritoriesOwned();
	const std::vector<Territory*> plToriesToDefend = currentPlayer->getTerritoriesToDefend();

	for (const auto& tory : plTories)
	{
		bool isInner = true;
		for (const auto& adjTory : tory->getBorderList())
		{
			if (adjTory->getPlayer() != currentPlayer)
			{
				isInner = false;
				break;
			}
		}

		if (isInner)
		{
			innerTories.push_back(tory);
		}
	}

	std::sort(innerTories.begin(), innerTories.end(), Utilities::SortByArmySize);

	if (innerTories.size() > 0 && plToriesToDefend.size() > 0)
	{
		currentPlayer->IssueAirliftOrder(innerTories[0], plToriesToDefend[0], innerTories[0]->getNbArmy());
	}
}

void AggressivePlayerStrategy::PlayingDiplomacyCard()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	// Wants to NAP the most threatening player
	const std::vector<Territory*> plToriesToAttack = currentPlayer->getTerritoriesToAttack();

	if (currentPlayer->getCurrentGameInstance() == nullptr)
	{
		return;
	}

	std::vector<Player*> allPlayers = currentPlayer->getCurrentGameInstance()->getPlayerList();

	std::map<Player*, int> otherPlayers;

	for (const auto& player : allPlayers)
	{
		if (player != currentPlayer)
		{
			otherPlayers[player] = 0;
		}
	}

	for (const auto& tory : plToriesToAttack)
	{
		otherPlayers[tory->getPlayer()] += tory->getNbArmy();
	}

	std::vector<std::pair<int, Player*>> sortedPlayers;

	for (const auto& [key, value] : otherPlayers)
	{
		sortedPlayers.push_back(std::pair<int, Player*>(value, key));
	}

	// descending order
	std::sort(sortedPlayers.begin(), sortedPlayers.end(), [=](std::pair<int, Player*>& a, std::pair<int, Player*>& b) {
		return a.first > b.first;
	});

	if (sortedPlayers.size() > 0)
	{
		currentPlayer->IssueNegotiateOrder(sortedPlayers[0].second);
	}
}


void BenevolentPlayerStrategy::issueOrder()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	toAttack();
	toDefend();
	// Get territories to attack in order of priority
	const std::vector<Territory*> plToriesToAttack = currentPlayer->getTerritoriesToAttack();
	// Get territories to defend in order of priority
	const std::vector<Territory*> plToriesToDefend = currentPlayer->getTerritoriesToDefend();

	std::cout << "Issuing orders for " << currentPlayer->getPlayerName() << "\n";
	std::cout << "They are a CPU! " << std::endl;

	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::DeployingArmies);
	int availableReserves = currentPlayer->getReinforcementPoolSize();
	currentPlayer->numDeployments = 0;

	while (currentPlayer->getPlayerTurnPhase() != EPlayerTurnPhase::EndOfTurn)
	{
		switch (currentPlayer->getPlayerTurnPhase())
		{
		case EPlayerTurnPhase::DeployingArmies:
			DeployArmies(availableReserves);
			break;
		case EPlayerTurnPhase::AdvancingArmies:
			AdvanceArmies();
			break;
		case EPlayerTurnPhase::PlayingCards:
			PlayingCards();
			break;
		case EPlayerTurnPhase::EndOfTurn:
			std::cout << "End of " << currentPlayer->getPlayerName() << "'s turn" << std::endl;
			break;
		default:
			currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
			break;
		}
	}
}

void BenevolentPlayerStrategy::toAttack()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	currentPlayer->getTerritoriesToAttack().clear();
}

void BenevolentPlayerStrategy::toDefend()
{
	/*
		Benevolent player will prioritize territories with the least troops to deploy troops
	*/
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	const std::vector<Territory*> currentPlayerTories = currentPlayer->getTerritoriesOwned();

	std::vector<Territory*> plToriesSorted;
	plToriesSorted.insert(plToriesSorted.begin(), currentPlayerTories.begin(), currentPlayerTories.end());
	// sorts player's territories in ascending order by troop numbers
	std::sort(plToriesSorted.begin(), plToriesSorted.end(), Utilities::SortByArmyAscendingSize);

	currentPlayer->getTerritoriesToDefend().clear();
	currentPlayer->getTerritoriesToDefend().insert(
		currentPlayer->getTerritoriesToDefend().begin(), currentPlayerTories.begin(), currentPlayerTories.end()
	);
}

void BenevolentPlayerStrategy::DeployArmies(int& inAvailableReserves)
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	if (inAvailableReserves <= 0)
	{
		currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::AdvancingArmies);
		return;
	}

	if (currentPlayer->getTerritoriesToDefend().size() > 0)
	{
		const int numTerritoriesToDefend = currentPlayer->getTerritoriesToDefend().size();
		const int totalAvailableReserves = inAvailableReserves;

		int index = currentPlayer->numDeployments % currentPlayer->getTerritoriesToDefend().size();

		int enemyAdjTories = 0;
		int enemyThreat = 0;
		std::vector<Territory*> neighbours = currentPlayer->getTerritoriesToDefend()[index]->getBorderList();
		for (auto& adjTory : neighbours)
		{
			if (adjTory != nullptr)
			{
				if (adjTory->getPlayer() != currentPlayer)
				{
					enemyAdjTories++;
					enemyThreat += adjTory->getNbArmy();
				}
			}
		}

		enemyThreat = std::max(0, enemyThreat - currentPlayer->getTerritoriesToDefend()[index]->getNbArmy());

		int numToDeploy = std::max(0, std::min(totalAvailableReserves, std::max(5, enemyAdjTories + enemyThreat)));

		currentPlayer->IssueDeployOrder
		(
			currentPlayer->getTerritoriesToDefend()[index],
			numToDeploy
		);

		inAvailableReserves -= numToDeploy;

		currentPlayer->numDeployments++;
	}
	else
	{
		currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::AdvancingArmies);
	}
}

void BenevolentPlayerStrategy::AdvanceArmies()
{
	// find territories with troops and advance them into player territories with the least
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	const std::vector<Territory*> allTories = currentPlayer->getTerritoriesOwned();
	const std::vector<Territory*> toriesToDefend = currentPlayer->getTerritoriesToDefend();

	for (const auto& plTory : allTories)
	{
		if (plTory->getNbArmy() == 0)
		{
			continue;
		}

		std::vector<Territory*> allPlAdjTories;
		const std::vector<Territory*> neighbours = plTory->getBorderList();

		for (const auto& plAdjTory : neighbours)
		{
			if (plAdjTory->getPlayer() == currentPlayer)
			{
				allPlAdjTories.push_back(plAdjTory);
			}
		}

		std::vector<Territory*> borderPlAdjTories;

		for (const auto& plAdjTory : allPlAdjTories)
		{
			if (std::find(toriesToDefend.begin(), toriesToDefend.end(), plAdjTory) != toriesToDefend.end())
			{
				borderPlAdjTories.push_back(plAdjTory);
			}
		}

		std::sort(borderPlAdjTories.begin(), borderPlAdjTories.end(), Utilities::SortByArmyAscendingSize);
		std::sort(allPlAdjTories.begin(), allPlAdjTories.end(), Utilities::SortByArmyAscendingSize);
		
		if (borderPlAdjTories.size() > 0)
		{
			// if there's a border territory nearby move to the one with the least troops
			currentPlayer->IssueAdvanceOrder(plTory, borderPlAdjTories[0], plTory->getNbArmy());
		}
		else
		{
			if (allPlAdjTories.size() > 0)
			{
				// otherwise move to the territory with the least troops
				currentPlayer->IssueAdvanceOrder(plTory, allPlAdjTories[0], plTory->getNbArmy());
			}
		}
	}

	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::PlayingCards);
}

void BenevolentPlayerStrategy::PlayingCards()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	if (currentPlayer->getCurrentHand() == nullptr)
	{
		return;
	}

	Command* userCommand = nullptr;

	// AI will play a card at random
	if (currentPlayer->getCurrentHand()->getHand().size() > 0)
	{
		Hand* hand = currentPlayer->getCurrentHand();
		auto randomClock = std::chrono::high_resolution_clock::now();
		auto seed = randomClock.time_since_epoch().count();
		auto randomizer = std::default_random_engine(seed);
		std::shuffle(hand->getHand().begin(), hand->getHand().end(), randomizer);

		Card* card = hand->drawCard_Hand();

		switch (card->getCardType())
		{
		case ECardTypes::Bomb:
			PlayingBombCard();
			break;
		case ECardTypes::Blockade:
			PlayingBlockadeCard();
			break;
		case ECardTypes::Airlift:
			PlayingAirliftCard();
			break;
		case ECardTypes::Diplomacy:
			PlayingDiplomacyCard();
			break;
		default:
			std::cout << "Invalid card" << std::endl;
			break;
		}

		delete card;
	}
	// change state
	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
}

void BenevolentPlayerStrategy::PlayingBombCard()
{
	// player will bomb the neighbour territory with the most troops
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	Command* userCommand = nullptr;

	const std::vector<Territory*> plToriesToAttack = currentPlayer->getTerritoriesToAttack();

	if (plToriesToAttack.size() > 0)
	{
		currentPlayer->IssueBombOrder(plToriesToAttack[plToriesToAttack.size() - 1]);
	}
}

void BenevolentPlayerStrategy::PlayingBlockadeCard()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	const std::vector<Territory*> currentPlayerBorderTories = currentPlayer->getTerritoriesToDefend();

	// I think the smart decision is to blow up your territory that's at most risk of
	// falling to the enemy
	std::vector<std::pair<Territory*, int>> currentPlayerToriesByPriority;

	// loop through all territories adjacent 
	for (auto& ownedTerritory : currentPlayerBorderTories)
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

	// sorts in descending order
	std::sort
	(
		currentPlayerToriesByPriority.begin(),
		currentPlayerToriesByPriority.end(),
		[](const std::pair<Territory*, int>& a, std::pair<Territory*, int>& b) -> bool
		{
			return a.second > b.second;
		}
	);

	// pick first element (one with highest threat) to blockade
	if (currentPlayerToriesByPriority.size() > 0)
	{
		currentPlayer->IssueBlockadeOrder(currentPlayerToriesByPriority[0].first);
	}
}

void BenevolentPlayerStrategy::PlayingAirliftCard()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	// defencive player will move troops to shore up the weakest part of the border
	// find all territories that aren't on the border
	std::vector<Territory*> innerTories;

	const std::vector<Territory*> plTories = currentPlayer->getTerritoriesOwned();
	const std::vector<Territory*> plToriesToDefend = currentPlayer->getTerritoriesToDefend();

	for (const auto& tory : plTories)
	{
		bool isInner = true;
		for (const auto& adjTory : tory->getBorderList())
		{
			if (adjTory->getPlayer() != currentPlayer)
			{
				isInner = false;
				break;
			}
		}

		if (isInner)
		{
			innerTories.push_back(tory);
		}
	}

	std::sort(innerTories.begin(), innerTories.end(), Utilities::SortByArmyAscendingSize);

	if (innerTories.size() > 0 && plToriesToDefend.size() > 0)
	{
		currentPlayer->IssueAirliftOrder(innerTories[0], plToriesToDefend[0], innerTories[0]->getNbArmy());
	}
}

void BenevolentPlayerStrategy::PlayingDiplomacyCard()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	// Wants to NAP the most threatening player
	const std::vector<Territory*> plToriesToAttack = currentPlayer->getTerritoriesToAttack();

	if (currentPlayer->getCurrentGameInstance() == nullptr)
	{
		return;
	}

	std::vector<Player*> allPlayers = currentPlayer->getCurrentGameInstance()->getPlayerList();

	std::map<Player*, int> otherPlayers;

	for (const auto& player : allPlayers)
	{
		if (player != currentPlayer)
		{
			otherPlayers[player] = 0;
		}
	}

	for (const auto& tory : plToriesToAttack)
	{
		otherPlayers[tory->getPlayer()] += tory->getNbArmy();
	}

	std::vector<std::pair<int, Player*>> sortedPlayers;

	for (const auto& [key, value] : otherPlayers)
	{
		sortedPlayers.push_back(std::pair<int, Player*>(value, key));
	}

	// descending order
	std::sort(sortedPlayers.begin(), sortedPlayers.end(), [=](std::pair<int, Player*>& a, std::pair<int, Player*>& b) {
		return a.first > b.first;
	});

	if (sortedPlayers.size() > 0)
	{
		currentPlayer->IssueNegotiateOrder(sortedPlayers[0].second);
	}
}


void NeutralPlayerStrategy::issueOrder()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	toAttack();
	toDefend();
	// Get territories to attack in order of priority
	const std::vector<Territory*> plToriesToAttack = currentPlayer->getTerritoriesToAttack();
	// Get territories to defend in order of priority
	const std::vector<Territory*> plToriesToDefend = currentPlayer->getTerritoriesToDefend();

	std::cout << "Issuing orders for " << currentPlayer->getPlayerName() << "\n";
	std::cout << "They are a Neutral CPU and will do nothing unless attacked!" << std::endl;

	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::DeployingArmies);
	int availableReserves = currentPlayer->getReinforcementPoolSize();

	while (currentPlayer->getPlayerTurnPhase() != EPlayerTurnPhase::EndOfTurn)
	{
		switch (currentPlayer->getPlayerTurnPhase())
		{
		case EPlayerTurnPhase::DeployingArmies:
			DeployArmies(availableReserves);
			break;
		case EPlayerTurnPhase::AdvancingArmies:
			AdvanceArmies();
			break;
		case EPlayerTurnPhase::PlayingCards:
			PlayingCards();
			break;
		case EPlayerTurnPhase::EndOfTurn:
			std::cout << "End of " << currentPlayer->getPlayerName() << "'s turn" << std::endl;
			break;
		default:
			currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
			break;
		}
	}
}

void NeutralPlayerStrategy::toAttack()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	currentPlayer->getTerritoriesToAttack().clear();
}

void NeutralPlayerStrategy::toDefend()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	currentPlayer->getTerritoriesToDefend().clear();
}

void NeutralPlayerStrategy::DeployArmies(int& inAvailableReserves)
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
}

void NeutralPlayerStrategy::AdvanceArmies()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
}

void NeutralPlayerStrategy::PlayingCards()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
}

void NeutralPlayerStrategy::PlayingBombCard()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
}

void NeutralPlayerStrategy::PlayingBlockadeCard()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
}

void NeutralPlayerStrategy::PlayingAirliftCard()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
}

void NeutralPlayerStrategy::PlayingDiplomacyCard()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
}

void CheaterPlayerStrategy::issueOrder()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	toAttack();

	// Get territories to attack in order of priority
	std::vector<Territory*> plToriesToAttack = currentPlayer->getTerritoriesToAttack();

	// once per turn take over all bordering territories
	for (auto& tory : plToriesToAttack)
	{
		Player* defender = tory->getPlayer();
		if (defender == nullptr) continue;

		// move to occupy
		std::vector<Territory*>::iterator itor;
		itor = std::find(defender->getTerritoriesOwned().begin(), defender->getTerritoriesOwned().end(), tory);
		if (itor != defender->getTerritoriesOwned().end())
		{
			defender->getTerritoriesOwned().erase(itor);
			tory->setPlayer(currentPlayer);
			currentPlayer->getTerritoriesOwned().push_back(tory);

			std::cout << currentPlayer->getPlayerName() << " is a cheater! They have stolen " << tory->getName()
				<< "!" << std::endl;
		}
		else
		{
			std::cout << "error, territory not found to remove" << std::endl;
		}
	}

	// refresh
	toDefend();
	toAttack();

	plToriesToAttack = currentPlayer->getTerritoriesToAttack();

	// Get territories to defend in order of priority
	const std::vector<Territory*> plToriesToDefend = currentPlayer->getTerritoriesToDefend();

	std::cout << "Issuing orders for " << currentPlayer->getPlayerName() << "\n";
	std::cout << "They are a CPU! " << std::endl;

	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::DeployingArmies);
	int availableReserves = currentPlayer->getReinforcementPoolSize();

	while (currentPlayer->getPlayerTurnPhase() != EPlayerTurnPhase::EndOfTurn)
	{
		switch (currentPlayer->getPlayerTurnPhase())
		{
		case EPlayerTurnPhase::DeployingArmies:
			DeployArmies(availableReserves);
			break;
		case EPlayerTurnPhase::AdvancingArmies:
			AdvanceArmies();
			break;
		case EPlayerTurnPhase::PlayingCards:
			PlayingCards();
			break;
		case EPlayerTurnPhase::EndOfTurn:
			std::cout << "End of " << currentPlayer->getPlayerName() << "'s turn" << std::endl;
			break;
		default:
			currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
			break;
		}
	}
}

void CheaterPlayerStrategy::toAttack()
{
	/*
		The Cheater player will prioritize territories with the least troops to Advance
	*/

	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	// toAttack will turn a list of all adjacent enemy territories, in order of territories with the least troops
	std::vector<Territory*> enemyTerritoryByPriority;

	const std::vector<Territory*> allCurrentPlayerTerritories = currentPlayer->getTerritoriesOwned();

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

				if (adjacentTerritory->getPlayer()->getPlayerID() != currentPlayer->getPlayerID())
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

	std::sort(enemyTerritoryByPriority.begin(), enemyTerritoryByPriority.end(), Utilities::SortByArmyAscendingSize);

	currentPlayer->getTerritoriesToAttack().clear();

	currentPlayer->getTerritoriesToAttack().insert(
		currentPlayer->getTerritoriesToAttack().begin(),
		enemyTerritoryByPriority.begin(),
		enemyTerritoryByPriority.end()
	);
}

void CheaterPlayerStrategy::toDefend()
{
	/*
		Cheat player will prioritize territories with the most troops to deploy troops
	*/
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	const std::vector<Territory*> currentPlayerTories = currentPlayer->getTerritoriesOwned();

	std::vector<Territory*> plToriesSorted;
	plToriesSorted.insert(plToriesSorted.begin(), currentPlayerTories.begin(), currentPlayerTories.end());
	// sorts player's territories in descending order by troop numbers
	std::sort(plToriesSorted.begin(), plToriesSorted.end(), Utilities::SortByArmySize);

	currentPlayer->getTerritoriesToDefend().clear();
	currentPlayer->getTerritoriesToDefend().insert(
		currentPlayer->getTerritoriesToDefend().begin(), currentPlayerTories.begin(), currentPlayerTories.end()
	);
}

void CheaterPlayerStrategy::DeployArmies(int& inAvailableReserves)
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	if (inAvailableReserves <= 0)
	{
		currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::AdvancingArmies);
		return;
	}

	if (currentPlayer->getTerritoriesToDefend().size() > 0)
	{
		currentPlayer->IssueDeployOrder(currentPlayer->getTerritoriesToDefend()[0], inAvailableReserves);
		inAvailableReserves -= inAvailableReserves;
	}

	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::AdvancingArmies);
}

void CheaterPlayerStrategy::AdvanceArmies()
{
	// find territories with troops and advance them into player territories with the least
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	for (const auto& plTory : currentPlayer->getTerritoriesToDefend())
	{
		if (plTory->getNbArmy() > 0)
		{
			std::vector<Territory*> adjacents;
			adjacents.insert(adjacents.begin(), plTory->getBorderList().begin(), plTory->getBorderList().end());
			std::sort(adjacents.begin(), adjacents.end(), Utilities::SortByArmyAscendingSize);

			bool enemyfound = false;
			for (const auto& adjTory : adjacents)
			{
				if (adjTory->getPlayer() != currentPlayer)
				{
					enemyfound = true;
					currentPlayer->IssueAdvanceOrder(plTory, adjTory, plTory->getNbArmy());
				}
			}

			// if there's no enemy then just move troops to territory with the most armies
			if (!enemyfound)
			{
				Territory* dst = nullptr;
				if (adjacents.size() > 0)
				{
					dst = adjacents[adjacents.size() - 1];
				}
				currentPlayer->IssueAdvanceOrder(plTory, dst, plTory->getNbArmy());
			}
		}
	}

	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::PlayingCards);
}

void CheaterPlayerStrategy::PlayingCards()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	if (currentPlayer->getCurrentHand() == nullptr)
	{
		return;
	}

	Command* userCommand = nullptr;

	PlayingAirliftCard(); // cheater gets a free airlift

	// AI will play a card at random
	if (currentPlayer->getCurrentHand()->getHand().size() > 0)
	{
		Hand* hand = currentPlayer->getCurrentHand();
		auto randomClock = std::chrono::high_resolution_clock::now();
		auto seed = randomClock.time_since_epoch().count();
		auto randomizer = std::default_random_engine(seed);
		std::shuffle(hand->getHand().begin(), hand->getHand().end(), randomizer);

		Card* card = hand->drawCard_Hand();

		switch (card->getCardType())
		{
		case ECardTypes::Bomb:
			PlayingBombCard();
			break;
		case ECardTypes::Blockade:
			PlayingBlockadeCard();
			break;
		case ECardTypes::Airlift:
			PlayingAirliftCard();
			break;
		case ECardTypes::Diplomacy:
			PlayingDiplomacyCard();
			break;
		default:
			std::cout << "Invalid card" << std::endl;
			break;
		}

		delete card;
	}

	// change state
	currentPlayer->setPlayerTurnPhase(EPlayerTurnPhase::EndOfTurn);
}

void CheaterPlayerStrategy::PlayingBombCard()
{
	// Aggressive player will bomb the neighbour territory with the most troops
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	Command* userCommand = nullptr;

	const std::vector<Territory*> plToriesToAttack = currentPlayer->getTerritoriesToAttack();

	if (plToriesToAttack.size() > 0)
	{
		currentPlayer->IssueBombOrder(plToriesToAttack[plToriesToAttack.size() - 1]);
	}
}

void CheaterPlayerStrategy::PlayingBlockadeCard()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	const std::vector<Territory*> currentPlayerBorderTories = currentPlayer->getTerritoriesToDefend();

	// I think the smart decision is to blow up your territory that's at most risk of
	// falling to the enemy
	std::vector<std::pair<Territory*, int>> currentPlayerToriesByPriority;

	// loop through all territories adjacent 
	for (auto& ownedTerritory : currentPlayerBorderTories)
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

	// sorts in descending order
	std::sort
	(
		currentPlayerToriesByPriority.begin(),
		currentPlayerToriesByPriority.end(),
		[](const std::pair<Territory*, int>& a, std::pair<Territory*, int>& b) -> bool
		{
			return a.second > b.second;
		}
	);

	// pick first element (one with highest threat) to blockade
	if (currentPlayerToriesByPriority.size() > 0)
	{
		currentPlayer->IssueBlockadeOrder(currentPlayerToriesByPriority[0].first);
	}
}

void CheaterPlayerStrategy::PlayingAirliftCard()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	// find all territories that aren't on the border
	std::vector<Territory*> innerTories;

	const std::vector<Territory*> plTories = currentPlayer->getTerritoriesOwned();
	const std::vector<Territory*> plToriesToDefend = currentPlayer->getTerritoriesToDefend();

	for (const auto& tory : plTories)
	{
		bool isInner = true;
		for (const auto& adjTory : tory->getBorderList())
		{
			if (adjTory->getPlayer() != currentPlayer)
			{
				isInner = false;
				break;
			}
		}

		if (isInner)
		{
			innerTories.push_back(tory);
		}
	}

	std::sort(innerTories.begin(), innerTories.end(), Utilities::SortByArmySize);

	if (innerTories.size() > 0 && plToriesToDefend.size() > 0)
	{
		currentPlayer->IssueAirliftOrder(innerTories[0], plToriesToDefend[0], innerTories[0]->getNbArmy());
	}
}

void CheaterPlayerStrategy::PlayingDiplomacyCard()
{
	Player* currentPlayer = getPlayer();
	if (currentPlayer == nullptr)
	{
		return;
	}

	// Wants to NAP the most threatening player
	const std::vector<Territory*> plToriesToAttack = currentPlayer->getTerritoriesToAttack();

	if (currentPlayer->getCurrentGameInstance() == nullptr)
	{
		return;
	}

	std::vector<Player*> allPlayers = currentPlayer->getCurrentGameInstance()->getPlayerList();

	std::map<Player*, int> otherPlayers;

	for (const auto& player : allPlayers)
	{
		if (player != currentPlayer)
		{
			otherPlayers[player] = 0;
		}
	}

	for (const auto& tory : plToriesToAttack)
	{
		otherPlayers[tory->getPlayer()] += tory->getNbArmy();
	}

	std::vector<std::pair<int, Player*>> sortedPlayers;

	for (const auto& [key, value] : otherPlayers)
	{
		sortedPlayers.push_back(std::pair<int, Player*>(value, key));
	}

	// descending order
	std::sort(sortedPlayers.begin(), sortedPlayers.end(), [=](std::pair<int, Player*>& a, std::pair<int, Player*>& b) {
		return a.first > b.first;
	});

	if (sortedPlayers.size() > 0)
	{
		currentPlayer->IssueNegotiateOrder(sortedPlayers[0].second);
	}
}