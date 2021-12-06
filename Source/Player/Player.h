#pragma once
// poggers cards
// soya map

#include <vector>
#include <queue>
#include <iostream>
#include <string>

#include "Order/Orders.h"
#include "Common/CommonTypes.h"

class Card;
class Hand;
class Order;
class OrdersList;
class Territory;
class PlayerStrategies;
class GameEngine;
class Command;
class CommandProcessor;

enum class EOrderType;
enum class EPlayerTurnPhase;

/*
	Summary: The Player class represents the player in a game of warzone.
*/
class Player {
public:
	/* ctors */
	Player();
	Player(std::string inPlayerName);
	Player(std::string inPlayerName, PlayerStrategies* inPS);
	Player(PlayerStrategies* inPS);
	Player(const Player& inPlayer);
	Player(PlayerStrategies* playerStrat, std::string playerName);
	~Player();

	friend class PlayerStrategies;

	/* op overrides */
	bool operator==(const Player& inRHS) const;
	bool operator!=(const Player& inRHS) const;
	Player& operator= (const Player& inPlayer);
	friend std::ostream& operator<<(std::ostream& out, const Player& inPlayer);
	/* public member functions */
	void setPlayerName(std::string inPlayerName);
	const std::string getPlayerName() const;

	void setCurrentGameInstance(GameEngine* inInstance);
	GameEngine* getCurrentGameInstance() const;

	void setPlayerStrategy(PlayerStrategies* inPlayerStrategy);
	PlayerStrategies* getPlayerStrategy() const;


	const std::size_t getPlayerID() const;
	// returns list of owned territories adjacent to enemy territories in order of owned army strength
	void toDefend();
	// returns list of adjacent enemy territories in order of army strength
	void toAttack();

	std::vector<Territory*>& getTerritoriesToDefend();
	const std::vector<Territory*>& getTerritoriesToDefend() const;
	std::vector<Territory*>& getTerritoriesToAttack();
	const std::vector<Territory*>& getTerritoriesToAttack() const;

	void AddTerritoryToAttack(Territory * inTerritoryToAttack);
	void AddTerritoryToDefend(Territory* inTerritoryToDefend);
	void SetTerritoriesToAttack(std::vector<Territory*> inTerritories);
	void SetTerritoriesToDefend(std::vector<Territory*> inTerritories);

	// divides up the issueingOrders phase for the player
	void setPlayerTurnPhase(EPlayerTurnPhase inPhase);
	void setPlayerTurnPhase(int inPhase);
	EPlayerTurnPhase getPlayerTurnPhase() const;

	void issueOrder();

	OrdersList* getOrders() const;
	Hand* getCurrentHand() const;
	void setCurrentHand(Hand* newHand);
	std::vector<Territory*>& getTerritoriesOwned();
	void setTerritoriesOwned(std::vector<Territory*> newTerritoriesOwned);
	std::vector<Player*>& getNotAttackablePlayers();

	void setReinforcementPool(const std::size_t inPoolSize);
	std::size_t getReinforcementPoolSize() const;

	int numDeployments = 0;

	// sets that the current player has taken territory
	void setCapturedTerritoryFlag(bool bInFlag);
	// gets that the current player has taken territory
	bool getCapturedTerritoryFlag() const;

	// sets that the current player had their territory taken
	void setPlayerWasAttacked(bool bInFlag);
	// gets that the current player had their territory taken
	bool getPlayerWasAttacked() const;

	void clearPlayersNotToAttack();

	void setCommandProcessor(CommandProcessor* inProcessor);

	// helper functions for issuing orders to the orders list
	void IssueDeployOrder(Territory* inDst, uint32 inArmiesToDeploy);
	void IssueAdvanceOrder(Territory* inSrc, Territory* inDst, uint32 inArmiesToAdvance);
	void IssueBombOrder(Territory* inDst);
	void IssueBlockadeOrder(Territory* inDst);
	void IssueAirliftOrder(Territory* inSrc, Territory* inDst, std::size_t inArmiesToAirlift);
	void IssueNegotiateOrder(Player* inTarget);

protected:
	Hand* _hand = nullptr;
	OrdersList* _orders = nullptr;

private:
	void HandleSaveEffect(Command* inCommand, std::string inMsg);

	// tracks subphase of issueingOrders phase
	EPlayerTurnPhase _currentPhase;

	std::vector<Territory*> _territoriesOwned;
	std::vector<Territory*> _territoriesToAttack;
	std::vector<Territory*> _territoriesToDefend;

	std::size_t availableReinforcements = 0;

	// flag for whether territory was taken this turn
	bool bTookTerritory = false;
	bool bWasAttacked = false;

	std::vector<Player*> _playersNotToAttack;

	/* Essential member attributes */
	GameEngine* currentGameInstance;

	PlayerStrategies* _playerStrategy;

	std::string _playerName = "";

	CommandProcessor* commandProcess;

	// auto-incrementing ID
	std::size_t _id = 0;
	static std::size_t _globalID;
	/* End Essential member attributes */
};