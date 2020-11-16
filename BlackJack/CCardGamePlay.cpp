#include "CCardGamePlay.h"
//#define AIDEBUG // ##
//#define DEBUG // %%
//#define RESETDECKDEBUG // >>
//#define DECKDEBUG // >>
//#define DRAWDEBUG // **
//#define NITTYDEBUG // <<
//#define VALUEDEBUG // @@

std::string CCardGamePlay::ProcessInput(const std::string& toInt)
{
	std::string Ret{ "" };
	for (int i{ 0 }; i < toInt.length(); ++i)
	{
		for (int j{ 0 }; j < _Numbers.size(); ++j)
		{
			if (toInt[i] == _Numbers[j])
			{
				Ret += toInt[i];
			}
		}
	}
	if (Ret == "")
	{
		Ret = '0';
	}

	return Ret;
}

const int CCardGamePlay::HandleInput()
{
	std::cin.clear();
	std::string userInput{ "" };
	do
	{
		std::getline(std::cin, userInput, '\n');
		if (userInput == "")
		{
			std::cout << "Please enter a number." << std::endl;
		}

	} while (userInput == "");

	std::string handledInput{ ProcessInput(userInput) };
#ifdef NITTYDEBUG
	std::cerr << "<<handledInput: " << handledInput << std::endl;
#endif
	const int Ret{ std::stoi(handledInput) };
	std::endl(std::cout);

	return Ret;
}

int CCardGamePlay::RandomInteger(const int Min, const int Max)
{
	if (Max == 4)
	{
		std::uniform_int_distribution<int> Suit(Min, Max);
		return Suit(_randomGenerator);
	}
	else
	{
		std::uniform_int_distribution<int> Face(Min, Max);
		return Face(_randomGenerator);
	}
}

void CCardGamePlay::ResetDeck(std::vector<Card>& DeckToReset)
{
	DeckToReset.resize(0);
}

void CCardGamePlay::AddCardToDeck(std::vector<Card>& Deck, const std::string& currentSuit, const int currentValue )
{
#ifdef NITTYDEBUG
	std::cerr << "<<Adding Card to Deck" << std::endl;
#endif
	Card tempCard{ currentSuit, currentValue };
	Deck.push_back(tempCard);
#ifdef NITTYDEBUG
	std::cerr << "<<New Deck size: " << _SDeck.size() << std::endl;
#endif
}

bool CCardGamePlay::IsCardValid(const std::string& currentSuit, const int currentValue)
{
#ifdef DECKDEBUG

	std::cerr << ">>Current Deck size: " << _SDeck.size() << std::endl;
#endif
	if (_SDeck.size() > 0)
	{
#ifdef DECKDEBUG
		std::cerr << ">>Deck is not Empty, Checking Card" << std::endl;
		std::cerr << ">>New card that is being checked -  Suit: " << currentSuit << " Value: " << currentValue << std::endl;
#endif
		for (size_t i{ 0 }; i < _SDeck.size(); ++i)
		{
#ifdef DECKDEBUG
			std::cerr << ">>Current i: " << i << " Suit: " << _SDeck[i].CardSuit << " Value: " << _SDeck[i].CardValue << std::endl;
#endif
			if (_SDeck[i].CardSuit == currentSuit && _SDeck[i].CardValue == currentValue)
			{
#ifdef DECKDEBUG
				std::cerr << ">>Invalid Card" << std::endl;
#endif
				return false;
			}
		}
		AddCardToDeck(_SDeck, currentSuit, currentValue);
		return true;
	}
	else
	{
#ifdef DECKDEBUG

		std::cerr << ">>Deck is Empty" << std::endl;
#endif
		AddCardToDeck(_SDeck, currentSuit, currentValue);
		return true;
	}
}

int CCardGamePlay::GetValue(const Card& CardToCheck) const
{
	if (CardToCheck.CardValue > 10 && CardToCheck.CardValue < 14)
	{
		return FACECARDVALUE;
	}
	else if (CardToCheck.CardValue == 14)
	{
		return FACECARDVALUE + 1;
	}
	else
	{
		return CardToCheck.CardValue;
	}
}

void CCardGamePlay::PrintCard(const Card& CardToPrint)
{
	std::cout << GetName(CardToPrint.CardValue) << "(" << GetValue(CardToPrint) << ") of " << CardToPrint.CardSuit;
}

void CCardGamePlay::PrintCard(const Card&& CardToPrint)
{
	std::cout << GetName(CardToPrint.CardValue) << "(" << GetValue(CardToPrint) << ") of " << CardToPrint.CardSuit;
}

void CCardGamePlay::AddUserValue(int ValueToAdd)
{
	_userScore += ValueToAdd;
}

void CCardGamePlay::AddAIValue(int ValueToAdd)
{
	if (IsFirstRound)
	{
		_publicAIScore += ValueToAdd;
		_AIScore += ValueToAdd;
		IsFirstRound = false;
#ifdef VALUEDEBUG
		std::cerr << "\n@@This is the first round, adding " << ValueToAdd << " to the publicAIScore" << std::endl;
		std::cerr << "\@@Current hidden AIScore is " << _AIScore << std::endl;
		std::cerr << "\n@@Current publicAIScore is " << _publicAIScore << std::endl;
#endif
	}
	else
	{
		_AIScore += ValueToAdd;
#ifdef VALUEDEBUG
		std::cerr << "\n@@This is NOT the first round, adding " << ValueToAdd << " to the hidden AIScore" << std::endl;
		std::cerr << "\n@@Current hidden AIScore is " << _AIScore << std::endl;
#endif
	}
}

void CCardGamePlay::AddCardValue(const Card& CardToCheck, const bool IsAI)
{
	//Is this a face card?
	if (CardToCheck.CardValue > 10 && CardToCheck.CardValue < 14)
	{
		if (IsAI)
		{
#ifdef VALUEDEBUG
			std::cerr << "\n@@This is a Face Card, adding " << FACECARDVALUE << " to the AIScore" << std::endl;
#endif
			AddAIValue(FACECARDVALUE);
		}
		else
		{
#ifdef VALUEDEBUG
			std::cerr << "\n@@This is a Face Card, adding " << FACECARDVALUE << " to the userScore" << std::endl;
#endif
			AddUserValue(FACECARDVALUE);
		}
	}
	//Is this an ace?
	else if (CardToCheck.CardValue == 14)
	{
		if (IsAI)
		{
#ifdef VALUEDEBUG
			std::cerr << "\n@@This is an Ace, adding " << FACECARDVALUE + 1 << " to the AIScore" << std::endl;
#endif
			if (_AIScore + (FACECARDVALUE + 1) > 21)
			{
				AddAIValue(1);
			}
			else
			{
				AddAIValue(FACECARDVALUE + 1);
			}
		}
		else
		{
#ifdef VALUEDEBUG
			std::cerr << "\n@@This is an Ace, adding " << FACECARDVALUE + 1 << " to the userScore" << std::endl;
#endif
			if (_userScore + (FACECARDVALUE + 1) > 21)
			{
				AddUserValue(1);
			}
			else
			{
				AddUserValue(FACECARDVALUE + 1);
			}
		}
	}
	else
	{
		if (IsAI)
		{
#ifdef VALUEDEBUG
			std::cerr << "\n@@This is a common card, adding " << CardToCheck.CardValue << " to the AIScore" << std::endl;
#endif
			AddAIValue(CardToCheck.CardValue);
		}
		else
		{
#ifdef VALUEDEBUG
			std::cerr << "\n@@This is a common card, adding " << CardToCheck.CardValue << " to the userScore" << std::endl;
#endif
			AddUserValue(CardToCheck.CardValue);
		}
	}
}

void CCardGamePlay::SetLastCard(const Card& CardToCheck, const bool IsAI)
{
	if (IsAI)
	{
		_LastAICard.CardSuit = CardToCheck.CardSuit;
		_LastAICard.CardValue = CardToCheck.CardValue;
	}
	else
	{
		_LastUserCard.CardSuit = CardToCheck.CardSuit;
		_LastUserCard.CardValue = CardToCheck.CardValue;
	}
}

void CCardGamePlay::DrawCard(Card &newCard, const bool IsAI = false)
{
#ifdef DRAWDEBUG
	int GenerateCount{ 0 };
#endif

	bool FindValidCard{ false };
	do
	{
#ifdef DECKDEBUG
		std::cerr << ">>Generating Card" << std::endl;
#endif
		int newValue{ RandomInteger(CARDVALUEMIN, CARDVALUEMAX) };
		std::string newSuit{ GetSuit(RandomInteger(SUITMIN, SUITMAX)) };

		if (IsCardValid(newSuit, newValue))
		{
			FindValidCard = true;
			newCard.CardSuit = newSuit;
			newCard.CardValue = newValue;
		
			if (IsAI)
			{
#ifdef DRAWDEBUG
				std::cerr << "**This is the AI draw nr " << GenerateCount << std::endl;
#endif
				AddCardValue(newCard, IsAI);
				SetLastCard(newCard, IsAI);
				AddCardToDeck(_SCurrentRoundAIDeck, newCard.CardSuit, newCard.CardValue);
			}
			else
			{
#ifdef DRAWDEBUG
				std::cerr << "**This is the Player draw nr " << GenerateCount << std::endl;
#endif
				AddCardValue(newCard, IsAI);
				SetLastCard(newCard, IsAI);
				AddCardToDeck(_SCurrentRoundUserDeck, newCard.CardSuit, newCard.CardValue);
			}
		}
#ifdef DRAWDEBUG
		++GenerateCount;
#endif
	} while (!FindValidCard);
}

CCardGamePlay::Card CCardGamePlay::ReturnCard()
{
#ifdef DECKDEBUG
	std::cerr << "\n>>Returning card!" << std::endl;
#endif
	Card newCard{ "NULL", 0 };
	DrawCard(newCard);
#ifdef DECKDEBUG
	std::cerr << ">>Card to be returned - Suit: " << newCard.CardSuit << " Value: " << newCard.CardValue << std::endl;
#endif
	return newCard;
}

CCardGamePlay::Card CCardGamePlay::ReturnCard(const bool IsAI)
{
#ifdef DECKDEBUG
	std::cerr << "\n>>Returning AI card!" << std::endl;
#endif
	Card newCard{ "NULL", 0 };
	DrawCard(newCard, IsAI);
#ifdef DECKDEBUG
	std::cerr << ">>AI Card to be returned - Suit: " << newCard.CardSuit << " Value: " << newCard.CardValue << std::endl;
#endif
	return newCard;
}

void CCardGamePlay::Clear()
{
	_currentBet = 0;
	_userScore = 0;
	_AIScore = 0;
	_publicAIScore = 0;
	IsHouseStuck = false;
	IsPlayerStuck = false;
	IsFirstRound = true;
	_LastAICard.CardSuit = "NULL";
	_LastAICard.CardValue = 0;
	_LastUserCard.CardSuit = "NULL";
	_LastUserCard.CardValue = 0;
	ResetDeck(_SCurrentRoundAIDeck);
	ResetDeck(_SCurrentRoundUserDeck);
}	

bool CCardGamePlay::ValidRange(const int Bet) const
{
	if (Bet >= MINBET && Bet <= MAXBET)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CCardGamePlay::PlaceBet()
{
	bool IsValidBet{ false };
	int userBet{};
	std::cout << "How much would you like to bet (maximum " << MAXBET << " credits)?\n" << std::endl;
	do
	{
		userBet = HandleInput();
		if (ValidAmount(userBet) && ValidRange(userBet))
		{
			_currentBet = userBet;
			IsValidBet = true;
		}
		else
		{
			std::cout << "Incorrect amount. Please try again.\n" << std::endl;
			continue;
		}

	} while (!IsValidBet);
#ifndef DEBUG
	system("cls");
#endif

	return true;
}

void CCardGamePlay::AIInitialDraw()
{
	bool IsAI{ true };
	std::cout << "\n\n The house drew ";
	PrintCard(ReturnCard(IsAI));
	std::cout << " as well as another card, face-down.";
	std::cout << "\n";
#ifdef AIDEBUG
	std::cerr << "##AI second card is \n";
	PrintCard(ReturnCard(IsAI));
	std::cerr << "\n\n";
#endif
#ifndef AIDEBUG
	ReturnCard(IsAI);
#endif
}

void CCardGamePlay::PrintTopBar() const
{
	std::cout << "You have " << _userCredits << " credits. Your bet is " << _currentBet << " credits."
		<< "\n====================================================================" << std::endl;
}

void CCardGamePlay::InitialDraw()
{
	PlaceBet();					//has a CLS
	PrintTopBar();
		std::cout << "\n You have drawn ";
#ifdef DEBUG
	std::cout << "\n\n";
#endif
	PrintCard(ReturnCard());
#ifdef DEBUG
	std::cout << "\n\n";
#endif
	std::cout << " and ";
#ifdef DEBUG
	std::cout << "\n\n";
#endif
	PrintCard(ReturnCard());
	std::cout << ".";
}

void CCardGamePlay::ContinueDraw()
{
#ifndef DEBUG
	system("cls");
#endif
	PrintTopBar();
	std::cout << "\nYour new card is ";
#ifdef DEBUG
	std::cout << "\n\n";
#endif
	PrintCard(ReturnCard());
	std::cout << "\n";
	/*
	if (!IsHouseStuck)
	{
		AIDraw();
	}
	*/
}

void CCardGamePlay::AIDraw()
{
	bool IsAI{ true };
	//if the AI should roll and AI sum is below 19
	if (_AIScore < 19 && ShouldAIDraw())
	{
		Card AICard{ "NULL", 0 };
		DrawCard(AICard, IsAI);
		std::cout << "\n\nThe house's new card is ";
		PrintCard(AICard);
		std::cout << "\n";
#ifdef AIDEBUG
		std::cerr << "##AICard is - Suit: " << AICard.CardSuit << " Value: " << AICard.CardValue << std::endl;
		std::cerr << "##Current AIScore: " << _AIScore << "\n" << std::endl;
		std::cerr << "##AI Public Score is " << _publicAIScore << std::endl;
#endif
	}
	else
	{
		IsHouseStuck = true;
		std::cout << "\n\n The house sticks." << std::endl;
	}
}

bool CCardGamePlay::ShouldAIDraw()
{
	//from 1 to 14
	int WillAIDraw{ RandomInteger(CARDVALUEMIN, CARDVALUEMAX) };
#ifdef AIDEBUG
	std::cerr << "##WillAIDraw is " << WillAIDraw << std::endl;
	std::cerr << "##UserScore is " << _userScore << std::endl;
	std::cerr << "##AIScore is " << _AIScore << std::endl;
	std::cerr << "##AI Public Score is " << _publicAIScore << std::endl;
#endif
	//If player is not stuck
	if (!IsPlayerStuck)
	{
		if (_AIScore >= 16)
		{
#ifdef AIDEBUG
			std::cerr << "##NOTSTUCK AIScore is equal to or more than 16, the random went " << WillAIDraw << std::endl;
#endif
			if (WillAIDraw >= 10)
			{
#ifdef AIDEBUG
				std::cerr << "##NOTSTUCK >=16 AI will draw" << std::endl;
#endif
				return true;
			}
			else
			{
#ifdef AIDEBUG
				std::cerr << "##NOTSTUCK >=16 AI will not draw, house sticks" << std::endl;
#endif
				return false;
			}
		}
		else
		{
			if (WillAIDraw >= 2)
			{
#ifdef AIDEBUG
				std::cerr << "##NOTSTUCK <16 AI will draw" << std::endl;
#endif
				return true;
			}
			else
			{
#ifdef AIDEBUG
				std::cerr << "##NOTSTUCK <16 AI will not draw, house sticks" << std::endl;
#endif
				return false;
			}
		}
	}
	//If player is stuck
	else
	{
		if (_AIScore < _userScore)
		{
			if (_AIScore >= 16)
			{
#ifdef AIDEBUG
				std::cerr << "##STUCK AIScore is equal to or more than 16, the random went " << WillAIDraw << std::endl;
#endif
				if (WillAIDraw >= 10)
				{
#ifdef AIDEBUG
					std::cerr << "##STUCK LOWSCORE >=16 AI will draw" << std::endl;
#endif
					return true;
				}
				else
				{
#ifdef AIDEBUG
					std::cerr << "##STUCK LOWSCORE >=16 AI will not draw, house sticks" << std::endl;
#endif
					return false;
				}
			}
			else if (_AIScore >= 11)
			{
				if (WillAIDraw >= 4)
				{
#ifdef AIDEBUG
					std::cerr << "##STUCK LOWSCORE >=11 AI will draw" << std::endl;
#endif
					return true;
				}
				else
				{
#ifdef AIDEBUG
					std::cerr << "##STUCK LOWSCORE >=11 AI will not draw, house sticks" << std::endl;
#endif
					return false;
				}
			}
			else
			{
#ifdef AIDEBUG
				std::cerr << "##STUCK LOWSCORE <11 AI will roll" << std::endl;
#endif
				return true;
			}
		}
		else
		{
#ifdef AIDEBUG
			std::cerr << "##STUCK HIGHSCORE AI will not draw, house sticks" << std::endl;
#endif
			return false;
		}
	}
}

void CCardGamePlay::UpdateCredits(const FinishType Outcome)
{
	if (Outcome == FinishType::BlackJack)
	{
		_userCredits += (_currentBet * 2);
	}
	else if (Outcome == FinishType::Win)
	{
		_userCredits += _currentBet;
	}
	else if (Outcome == FinishType::Stuck)
	{
		_userCredits -= (_currentBet / 2);
	}
	else if (Outcome == FinishType::Lost)
	{
		_userCredits -= _currentBet;
	}
	else
	{
		std::cerr << "Error - invalid finish" << std::endl;
	}
}

void CCardGamePlay::PrintDeck(const std::vector<Card>& DeckToPrint)
{
	for (size_t i{ 0 }; i < DeckToPrint.size(); ++i)
	{
		std::cout << "\n " << GetName(DeckToPrint[i].CardValue) << "(" << GetValue(DeckToPrint[i]) << ") of " << DeckToPrint[i].CardSuit;
	}
}


//om spelare förlorar, house inte dra igen

void CCardGamePlay::PrintFinalCards()
{
	std::cout << "\n====================================================================";
	std::cout << "\nRound finished!\n"
		<< "\nYour cards are";
	PrintDeck(_SCurrentRoundUserDeck);
	if (!IsPlayerStuck)
	{
		std::cout << "\n Your final card was ";
		PrintCard(_LastUserCard);
		std::cout << ".";
	}

	std::cout << "\n\nThe house cards are";
	PrintDeck(_SCurrentRoundAIDeck);
	if (!IsHouseStuck)
	{
		std::cout << "\n The house's final card was ";
		PrintCard(_LastAICard);
		std::cout << ".";
	}
	std::cout << "\n\n====================================================================";
}

void CCardGamePlay::HandleFinish()
{
	PrintFinalCards();

	//std::cout << "===================================================================="
		std::cout << "\n\nThe final score is"
		<< "\n Your total: " << _userScore
		<< "\n House total: " << _AIScore << std::endl;

	if (_userScore == 21 && _AIScore != 21)
	{
		//If Player has 21
		std::cout << "\nBlackJack! Credits doubled!"
			<< "\n You gained " << _currentBet * 2 << " credits!\n" << std::endl;
		UpdateCredits(FinishType::BlackJack);
	}
	else if (_userScore == 21 && _AIScore == 21 || _userScore == _AIScore)
	{
		//If both have 21 or end up with the same score below 21
		std::cout << "\nShared win! Money back."
			<< "\n Your bet was returned.\n" << std::endl;
	}
	else if (_userScore != 21 && _AIScore == 21 || _AIScore < 21 && _AIScore > _userScore)
	{
		//If AI has 21 OR if AI has less than 21 but more than Player
		if (IsPlayerStuck)
		{
			std::cout << "\nThe house wins. You stuck but lost the bet."
				<< "\n Sticking: You lost " << _currentBet / 2 << " credits!\n" << std::endl;
			UpdateCredits(FinishType::Stuck);
		}
		else
		{
			std::cout << "\nThe house wins. You lost the bet."
				<< "\n You lost " << _currentBet << " credits!\n" << std::endl;
			UpdateCredits(FinishType::Lost);
		}
	}
	else if (_userScore < 21 && _userScore > _AIScore || _AIScore > 21 && _userScore < 21)
	{
		//If Player has less than 21 but more than AI OR the AI went bust
		std::cout << "\nThe house lost. You win the bet!"
			<< "\n You gained " << _currentBet << " credits!\n" << std::endl;
		UpdateCredits(FinishType::Win);
	}
	else
	{
		//If the Player went bust
		std::cout << "\nSorry, that's too much. You lost the bet."
			<< "\n You lost " << _currentBet << " credits!\n" << std::endl;
		UpdateCredits(FinishType::Lost);
	}
	Clear();
	std::cout << "====================================================================\n";
}

void CCardGamePlay::PrintUpdatedScore()
{
	std::cout << "\n===================================================================="
		<< "\n Your score is currently " << _userScore << "."
		<< "\n The house's (known) score is currently " << _publicAIScore << ".\n" << std::endl;

	if (IsHouseStuck && _userScore != 21)
	{
		std::cout << "The house is stuck.\n" << std::endl;
	}
}

void CCardGamePlay::CheckDeck(std::vector<Card>& Deck)
{
	if (Deck.size() >= 40)
	{
#ifdef RESETDECKDEBUG
		std::cerr << ">>Current Deck size is " << _SDeck.size() << ", resetting deck" << std::endl;
#endif
		ResetDeck(Deck);
		std::cout << "\n\nThe deck has been shuffled.\n" << std::endl;
	}
}

void CCardGamePlay::HandleRound()
{
	do
	{
		PrintUpdatedScore();
		CheckDeck(_SDeck);
		std::cout << "You will go bust if you score over " << MAXJACK
			<< ".\nWhat would you like to do next?\n"
			<< " [1] Draw another card\n"
			<< " [2] Stick (and halve your losses)\n" << std::endl;
		int userInput{ HandleInput() };

		if (userInput == 1)
		{
			ContinueDraw();
		}
		else if (userInput == 2)
		{
			IsPlayerStuck = true;
			while (!IsHouseStuck)
			{
				AIDraw();
			}
			break;
		}
		else
		{
			std::cout << "Incorrect input. Please try again.\n" << std::endl;
			continue;
		}
	} while (IsScoreValid());
#ifndef DEBUG
	system("cls");
#endif
	HandleFinish();
}

bool CCardGamePlay::IsUserCreditsValid() const
{
	if (_userCredits >= 300)
	{
		std::cout << "You have " << _userCredits << " credits!"
			<< "\nYou beat the house! Lady luck is on your side!" << std::endl;
		return false;
	}
	else if (_userCredits <= 0)
	{
		std::cout << "You are all out of credits! The game is over!" << std::endl;
		return false;
	}
	else
	{
		return true;
	}
}

void CCardGamePlay::MainGame(bool& IsGameOver)
{
	do
	{
		if (!IsUserCreditsValid())
		{
			IsGameOver = true;
		}
		else
		{
			std::cout << "You have " << _userCredits << " credits.\n"
				<< "What would you like to do next?\n"
				<< " [1] Play a round\n"
				<< " [2] Leave the table\n" << std::endl;
			int userInput{ HandleInput() };

			if (userInput == 1)
			{
				InitialDraw();
				AIInitialDraw();
				//If someone got >= 21 on the initial round
				if (!IsScoreValid())
				{
					if (_userScore >= 21)
					{
						if (_userScore > 21)
						{
							std::cout << "\nYour score went bust." << std::endl;
						}
						else
						{
							std::cout << "\nYou got a BlackJack!" << std::endl;
						}
					}

					if (_AIScore >= 21)
					{
						if (_AIScore > 21)
						{
							std::cout << "\nThe house score went bust." << std::endl;
						}
						else
						{
							std::cout << "\nThe house got a BlackJack!" << std::endl;
						}
					}

					HandleFinish();
				}
				else
				{ 
					HandleRound();
				}
			}
			else if (userInput == 2)
			{
				IsGameOver = true;
			}
			else
			{
				std::cout << "Invalid input. Please try again." << std::endl;
				continue;
			}
		}
	} while (!IsGameOver);
}

void CCardGamePlay::MainLoop()
{
	bool IsGameOver{ false };
	std::cout << "Welcome to BlackJack! Please take a seat at the table!\n"
		<< "You start with " << _userCredits << " credits. You win if you get " << CREDITSGOAL << " credits!\n" << std::endl;

	while (!IsGameOver)
	{
		MainGame(IsGameOver);
	}

	std::cout << "\nThank you for playing! Have a nice day!" << std::endl;
}
