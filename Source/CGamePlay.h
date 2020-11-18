#pragma once
#include<array>
#include<cstdlib>
#include<ctime>
#include<iostream>
#include<random>
#include<string>


class CGamePlay
{
public:
	//old random
	//const void SetSeed() { srand( (unsigned int)time(nullptr) ); };

	bool IsAIScoreValid() const { return _currentAISum < 21; }
	bool IsPlayerScoreValid() const { return _currentSum < 21; }
	void AddPlayerSum(const int firstDie, const int secondDie) { _currentSum += firstDie + secondDie; }
	void AddAISum(const int firstDie, const int secondDie) { _currentAISum += firstDie + secondDie; }

	std::string ProcessInput(const std::string& toInt);
	const int HandleInput();

	int RandomInteger(const int Min, const int Max);
	int DiceRoll();
	void PlayerDiceRoll();
	void AIDiceRoll();
	bool ShouldAIRoll();

	bool ValidAmount(const int Bet) const { return Bet <= _userCredits; }
	bool ValidRange(const int Bet) const;
	bool PlaceBet();

	void InitialRoll();

	void Clear();

	enum class FinishType
	{
		DiceJack,
		Win,
		Stuck,
		Lost
	};

	void UpdateCredits(const FinishType);
	void HandleFinish();
	void HandleRound();

	bool IsUserCreditsValid() const;
	void MainGame(bool& isGameOver);
	void MainLoop();

private:
	const int DICEMIN{ 1 };
	const int DICEMAX{ 6 };
	const int MINBET{ 1 };
	const int MAXBET{ 50 };
	std::array<char, 10> _Numbers
	{
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
	};

	const int MAXJACK{ 21 };
	const int CREDITSGOAL{ 300 };

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

