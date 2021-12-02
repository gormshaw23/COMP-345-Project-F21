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

enum class EOrderType;

/*
	Summary: The Player class represents the player in a game of warzone.
*/
class Player {
public:
	enum class EPlayerTurnPhase {
		DeployingArmies,
		AdvancingArmies,
		PlayingCards,
		EndOfTurn
	};

	/* ctors */
	Player();
	Player(std::string inPlayerName);
	Player(const Player& inPlayer);
	~Player();
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
	std::vector<Territory*> toDefend();
	// returns list of adjacent enemy territories in order of army strength
	std::vector<Territory*> toAttack();

	std::vector<Territory*>& getTerritoriesToDefend();
	const std::vector<Territory*>& getTerritoriesToDefend() const;
	std::vector<Territory*>& getTerritoriesToAttack();
	const std::vector<Territory*>& getTerritoriesToAttack() const;

	void AddTerritoryToAttack(Territory * inTerritoryToAttack);
	void AddTerritoryToDefend(Territory* inTerritoryToDefend);
	void SetTerritoriesToAttack(std::vector<Territory*> inTerritories);
	void SetTerritoriesToDefend(std::vector<Territory*> inTerritories);	

	// Unreal Engine style standard for long function declarations
	void issueOrder
	(
		const EOrderType inOrderType,
		const std::vector<Player*> inPlayers,
		const std::vector<Territory*> inTerritories,
		const std::vector<int> inNumArmies
	);

	void issueOrder();

	OrdersList* getOrders() const;
	Hand* getCurrentHand() const;
	void setCurrentHand(Hand* newHand);
	std::vector<Territory*>& getTerritoriesOwned();
	void setTerritoriesOwned(std::vector<Territory*> newTerritoriesOwned);
	std::vector<Player*>& getNotAttackablePlayers();

	void setReinforcementPool(const std::size_t inPoolSize);
	std::size_t getReinforcementPoolSize() const;

	void setCapturedTerritoryFlag(bool bInFlag);
	bool getCapturedTerritoryFlag() const;

protected:
	Hand* _hand = nullptr;
	OrdersList* _orders = nullptr;

	// helper functions for issuing orders to the orders list
	void IssueDeployOrder(Territory* inDst, uint32 inArmiesToDeploy);
	void IssueAdvanceOrder(Territory* inSrc, Territory* inDst, uint32 inArmiesToAdvance);
	void IssueBombOrder(Territory* inDst);
	void IssueBlockadeOrder(Territory* inDst);
	void IssueAirliftOrder(Territory* inSrc, Territory* inDst, std::size_t inArmiesToAirlift);
	void IssueNegotiateOrder(Player* inTarget);

private:
	// Human specific Player functions, requiring human input
	// For specifying parameters for Orders to be issued
	void DeployArmies_Human();
	void AdvanceArmies_Human();
	void PlayingCards_Human();
	void PlayingBombCard_Human();
	void PlayingBlockadeCard_Human();
	void PlayingAirliftCard_Human();
	void PlayingDiplomacyCard_Human();

	// divides up the issueingOrders phase for the player
	void setPlayerTurnPhase(EPlayerTurnPhase inPhase);
	void setPlayerTurnPhase(int inPhase);
	EPlayerTurnPhase getPlayerTurnPhase() const;

	// Helper functions for displaying territories to the human player
	void DisplayToriesToDefendAndAdjacencts();
	void DisplayPlayerToriesToDefendAndAttack();
	void DisplayPlayerToriesToDefend();
	void DisplayPlayerToriesToAttack();

	// tracks subphase of issueingOrders phase
	EPlayerTurnPhase _currentPhase;

	std::vector<Territory*> _territoriesOwned;
	std::vector<Territory*> _territoriesToAttack;
	std::vector<Territory*> _territoriesToDefend;

	std::size_t availableReinforcements = 0;

	// flag for whether territory was taken this turn
	bool bTookTerritory = false;

	std::vector<Player*> _playersNotToAttack;

	/* Essential member attributes */
	GameEngine* currentGameInstance;

	PlayerStrategies* _playerStrategy;

	std::string _playerName = "";

	// auto-incrementing ID
	std::size_t _id = 0;
	static std::size_t _globalID;
	/* End Essential member attributes */
};