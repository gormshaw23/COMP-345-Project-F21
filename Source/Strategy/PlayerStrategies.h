#pragma once

// PlayerStrategies class
enum class EPlayerStrategy {
	Human,
	Aggressive,
	Benevolent,
	Neutral,
	Cheater
};

class Player;

class PlayerStrategies {
public:
	virtual void issueOrder() = 0;
	virtual void toAttack() = 0;
	virtual void toDefend() = 0;
protected:
	Player* _player = nullptr;
private:
};

class HumanPlayerStrategy : public PlayerStrategies {
public:
	virtual void issueOrder() override;
	virtual void toAttack() override;
	virtual void toDefend() override;
private:

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