#pragma once
#include<cstdlib>
#include<ctime>
#include<iostream>
#include<random>


class CGamePlay
{
public:
	//old random
	//const void SetSeed() { srand(time(nullptr)); };

	const bool IsAIScoreValid() { return _currentAISum < 21; }
	const bool IsPlayerScoreValid() { return _currentSum < 21; }
	void AddPlayerSum(const int firstDie, const int secondDie) { _currentSum += firstDie + secondDie; }
	void AddAISum(const int firstDie, const int secondDie) { _currentAISum += firstDie + secondDie; }

	int HandleInput();

	const int RandomInteger(const int Min, const int Max);
	const int DiceRoll();
	void PlayerDiceRoll();
	void AIDiceRoll();
	const bool ShouldAIRoll();

	const bool ValidAmount(const int Bet) { return Bet <= _userCredits; }
	const bool ValidRange(const int Bet);
	bool PlaceBet();

	const void InitialRoll();

	void Clear();

	enum class FinishType
	{
		Win,
		Stuck,
		Lost
	};

	void UpdateCredits(const FinishType);
	void HandleFinish();
	void HandleRound();

	const bool IsUserCreditsValid();
	int MainGame(bool& isGameOver);
	const int MainLoop();

	const int MAXJACK{ 21 };
	const int GOAL{ 300 };

private:
	const int DICEMIN{ 1 };
	const int DICEMAX{ 6 };
	const int MINBET{ 1 };
	const int MAXBET{ 50 };

	int _userCredits{ 100 };
	int _currentBet{ 0 };
	int _currentSum{ 0 };
	int _currentAISum{ 0 };

	bool IsHouseStuck{ false };
	bool IsPlayerStuck{ false };
	//old random
	//bool IsSeedSet{ false };

	std::random_device _randomDevice;
	std::mt19937 _randomGenerator = std::mt19937(_randomDevice());

};

