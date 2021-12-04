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
	virtual void setPlayer(Player* inPlayer);
protected:
	virtual void DeployArmies(int& inAvailableReserves) = 0;
	virtual void AdvanceArmies() = 0;
	virtual void PlayingCards() = 0;
	virtual void PlayingBombCard() = 0;
	virtual void PlayingBlockadeCard() = 0;
	virtual void PlayingAirliftCard() = 0;
	virtual void PlayingDiplomacyCard() = 0;

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

	void DeployArmies(int& inAvailableReserves) override;
	void AdvanceArmies() override;
	void PlayingCards() override;
	void PlayingBombCard() override;
	void PlayingBlockadeCard() override;
	void PlayingAirliftCard() override;
	void PlayingDiplomacyCard() override;

private:
	// Helper functions for displaying territories to the human player
	void DisplayToriesToDefendAndAdjacents();
	void DisplayPlayerToriesToDefendAndAttack();
	void DisplayPlayerToriesToDefend();
	void DisplayPlayerToriesToAttack();

	std::string GetUserInput(Command*& userCommand);
};

/*
	Aggressive Player:
	computer player that focuses on attack (deploys or advances armies on its strongest
	country, then always advances to enemy territories until it cannot do so anymore).

	BB notes: I think I will have them push generally into the weakest territories on their border
	and avoid attritioning themselves against strong opponents.

*/
class AggressivePlayerStrategy : public PlayerStrategies {
public:
	virtual void issueOrder() override;
	virtual void toAttack() override;
	virtual void toDefend() override;
protected:
	void DeployArmies(int& inAvailableReserves) override;
	void AdvanceArmies() override;
	void PlayingCards() override;
	void PlayingBombCard() override;
	void PlayingBlockadeCard() override;
	void PlayingAirliftCard() override;
	void PlayingDiplomacyCard() override;
private:

};

class BenevolentPlayerStrategy : public PlayerStrategies {
public:
	virtual void issueOrder() override;
	virtual void toAttack() override;
	virtual void toDefend() override;
protected:
	void DeployArmies(int& inAvailableReserves) override;
	void AdvanceArmies() override;
	void PlayingCards() override;
	void PlayingBombCard() override;
	void PlayingBlockadeCard() override;
	void PlayingAirliftCard() override;
	void PlayingDiplomacyCard() override;
private:

};

class NeutralPlayerStrategy : public PlayerStrategies {
public:
	virtual void issueOrder() override;
	virtual void toAttack() override;
	virtual void toDefend() override;
protected:
	void DeployArmies(int& inAvailableReserves) override;
	void AdvanceArmies() override;
	void PlayingCards() override;
	void PlayingBombCard() override;
	void PlayingBlockadeCard() override;
	void PlayingAirliftCard() override;
	void PlayingDiplomacyCard() override;
private:

};

class CheaterPlayerStrategy : public PlayerStrategies {
public:
	virtual void issueOrder() override;
	virtual void toAttack() override;
	virtual void toDefend() override;
protected:
	void DeployArmies(int& inAvailableReserves) override;
	void AdvanceArmies() override;
	void PlayingCards() override;
	void PlayingBombCard() override;
	void PlayingBlockadeCard() override;
	void PlayingAirliftCard() override;
	void PlayingDiplomacyCard() override;
private:

};