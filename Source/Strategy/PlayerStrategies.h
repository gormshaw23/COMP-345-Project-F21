#pragma once
#include <string>

// PlayerStrategies class
enum class EPlayerStrategy {
	Human,
	Aggressive,
	Benevolent,
	Neutral,
	Cheater
};

class Player;
class Command;
class CommandProcessor;

class PlayerStrategies {
public:
	virtual void issueOrder() = 0;
	virtual void toAttack() = 0;
	virtual void toDefend() = 0;

	virtual Player* getPlayer() const;
protected:
	virtual void HandleSaveEffect(Command* inCommand, std::string inMsg);

	Player* _player = nullptr;
	CommandProcessor* commandProcess = nullptr;
private:
};

class HumanPlayerStrategy : public PlayerStrategies {
public:
	virtual void issueOrder() override;
	virtual void toAttack() override;
	virtual void toDefend() override;

	HumanPlayerStrategy(Player* inPlayer);
protected:
	HumanPlayerStrategy();
private:
	// Human specific Player functions, requiring human input
	// For specifying parameters for Orders to be issued
	void DeployArmies_Human(int& inAvailableReserves);
	void AdvanceArmies_Human();
	void PlayingCards_Human();
	void PlayingBombCard_Human();
	void PlayingBlockadeCard_Human();
	void PlayingAirliftCard_Human();
	void PlayingDiplomacyCard_Human();

	// Helper functions for displaying territories to the human player
	void DisplayToriesToDefendAndAdjacents();
	void DisplayPlayerToriesToDefendAndAttack();
	void DisplayPlayerToriesToDefend();
	void DisplayPlayerToriesToAttack();

	std::string GetUserInput(Command*& userCommand);
};

class AggressivePlayerStrategy : public PlayerStrategies {
public:
	virtual void issueOrder() override;
	virtual void toAttack() override;
	virtual void toDefend() override;
private:

};

class BenevolentPlayerStrategy : public PlayerStrategies {
public:
	virtual void issueOrder() override;
	virtual void toAttack() override;
	virtual void toDefend() override;
private:

};

class NeutralPlayerStrategy : public PlayerStrategies {
public:
	virtual void issueOrder() override;
	virtual void toAttack() override;
	virtual void toDefend() override;
private:

};

class CheaterPlayerStrategy : public PlayerStrategies {
public:
	virtual void issueOrder() override;
	virtual void toAttack() override;
	virtual void toDefend() override;
private:

};