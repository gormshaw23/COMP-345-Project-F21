#pragma once

enum class ECardTypes
{
	Bomb,
	Reinforcement,
	Blockade,
	Airlift,
	Diplomacy
};

enum class EPlayerTurnPhase {
	DeployingArmies,
	AdvancingArmies,
	PlayingCards,
	EndOfTurn
};

typedef signed int int32;
typedef unsigned int uint32;