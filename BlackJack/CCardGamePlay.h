#pragma once
#include<array>
#include<iostream>
#include<random>
#include<string>
#include<string_view>
#include<vector>

//#include "CGameState.h"
#include "CHandleInput.h"

class CCardGamePlay
{
	//friend class CGameState;

	struct Card
	{
	/*	//default constructor
		Card() : CardSuit("NULL"), CardValue(0)
		{
		}*/
		std::string CardSuit{};
		int CardValue{};

	};

public:

	bool IsScoreValid() const { return _AIScore < 21 && _userScore < 21; }
	/*
	std::string ProcessInput(const std::string& toInt);
	const int HandleInput();
	*/

	void ResetDeck(std::vector<Card>& DeckToReset);
	int RandomInteger(const int Min, const int Max);

	void AddCardToDeck(std::vector<Card>& Deck, const std::string& currentSuit, const int currentValue);
	bool IsCardValid(const std::string& currentSuit, const int currentValue);

	std::string GetName(const int currentValue) const { return _CardName[currentValue - 2]; };
	std::string GetSuit(const int currentSuit) const { return _CardSuit[currentSuit - 1]; };
	int GetValue(const Card& CardToCheck) const;

	void PrintCard(const Card& CardToPrint);
	void PrintCard(const Card&& CardToPrint);
	void AddUserValue(int ValueToAdd);
	void AddAIValue(int ValueToAdd);
	void AddCardValue(const Card& CardToCheck, const bool IsAI);
	void SetLastCard(const Card& CardToCheck, const bool IsAI = false);
	void DrawCard(Card &newCard, const bool IsAI);
	Card ReturnCard();
	Card ReturnCard(const bool IsAI);
	
	void Clear();
	bool ValidAmount(const int Bet) const { return Bet <= _userCredits; };
	bool ValidRange(const int Bet) const;
	bool PlaceBet();

	void AIInitialDraw();
	void PrintTopBar() const;
	void InitialDraw();
	void ContinueDraw();
	void AIDraw();
	bool ShouldAIDraw();

	enum class FinishType
	{
		BlackJack,
		Win,
		Stuck,
		Lost
	};

	void UpdateCredits(const FinishType);
	void HandleFinish();
	void PrintCurrentHand(const std::vector<Card>& DeckToPrint);
	void PrintFinalCards();
	void PrintUpdatedScore();
	void CheckDeck(std::vector<Card>& Deck);
	void CheckDeck(int&& Deck);
	bool IsUserCreditsValid() const;

	/*
	void HandleRound();
	void MainGame(bool& IsGameOver);
	void MainLoop();
	*/

	const int GetMaxJack() const { return MAXJACK; };
	const int GetCreditsGoal() const { return CREDITSGOAL; };
	const int GetCredits() const { return _userCredits; };
	const int GetBet() const { return _currentBet; };
	const int GetAIScore() const { return _AIScore; };
	const int GetPublicAIScore() const { return _publicAIScore; };
	const int GetUserScore() const { return _userScore; };
	bool IsHouseStuck() const { return _IsHouseStuck; };
	bool IsPlayerStuck() const { return _IsPlayerStuck; };
	void PlayerSticks() { _IsPlayerStuck = true; };
	void HouseSticks() { _IsHouseStuck = true; };
	int GetDeckSize() const { return _SDeck.size(); };

private:
	const int CARDVALUEMIN{ 2 };
	const int CARDVALUEMAX{ 14 };
	const int SUITMIN{ 1 };
	const int SUITMAX{ 4 };
	const int MINBET{ 1 };
	const int MAXBET{ 50 };
	const int FACECARDVALUE{ 10 };
	/*
	std::array<char, 10> _Numbers
	{
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
	};
	*/
	
	const int MAXJACK{ 21 };
	const int CREDITSGOAL{ 300 };
	int _userCredits{ 100 };

	int _currentBet{ 0 };
	int _userScore{ 0 };
	int _AIScore{ 0 };
	int _publicAIScore{ 0 };

	bool _IsFirstRound{ true };
	bool _IsHouseStuck{ false };
	bool _IsPlayerStuck{ false };

	Card _LastAICard{ "NULL", 0 };
	Card _LastUserCard{ "NULL", 0 };

	std::vector<Card> _SDeck;
	std::vector<Card> _SCurrentRoundUserDeck;
	std::vector<Card> _SCurrentRoundAIDeck;
	std::array<std::string, 13> _CardName
	{ 
		"Two", "Three", "Four", 
		"Five", "Six", "Seven", 
		"Eight", "Nine", "Ten", 
		"Jack", "Queen", "King", "Ace" 
	};
	std::array<std::string, 4> _CardSuit
	{
		"Clubs", "Diamonds", 
		"Hearts", "Spades"
	};

	std::random_device _randomDevice;
	std::mt19937 _randomGenerator = std::mt19937(_randomDevice());

};

