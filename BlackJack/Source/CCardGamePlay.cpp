#include "CCardGamePlay.h"
//#define AIDEBUG // ##
//#define DEBUG // %%
//#define RESETDECKDEBUG // >>
//#define DECKDEBUG // >>
//#define DRAWDEBUG // **
//#define NITTYDEBUG // <<
//#define VALUEDEBUG // @@

#define RELEASE			//DISABLE if AIDEBUG is enabled

//#define DEBUG
#ifdef DEBUG
#  define DEBUG(x) { std::cerr << x << std::endl; }
#else
#  define DEBUG(x)
#endif

//#define AIDEBUG				//ENABLE if RELEASE is disabled
#ifdef AIDEBUG
#  define AIDEBUG(x) { std::cerr << x << std::endl; }
#else
#  define AIDEBUG(x)
#endif


//#define VALUEDEBUG
#ifdef VALUEDEBUG
#  define VALUEDEBUG(x) { std::cerr << x << std::endl; }
#else
#  define VALUEDEBUG(x)
#endif


int CCardGamePlay::RandomInteger(const int Min, const int Max)
{
	std::uniform_int_distribution<int> Ret(Min, Max);
	return Ret(_randomGenerator);

}

void CCardGamePlay::ResetDeck(std::vector<Card>& DeckToReset)
{
	DeckToReset.resize(0);
}

void CCardGamePlay::AddCardToDeck(std::vector<Card>& Deck, const std::string& currentSuit, const int currentValue )
{
	DEBUG("\n<<Adding Card to Deck");

	Card tempCard{ currentSuit, currentValue };
	Deck.push_back(tempCard);

	DEBUG("\n<<New Deck size: " << _SDeck.size());
}

bool CCardGamePlay::IsCardValid(const std::string& currentSuit, const int currentValue)
{
	if (_SDeck.size() > 0)
	{
		for (size_t i{ 0 }; i < _SDeck.size(); ++i)
		{

			if (_SDeck[i].CardSuit == currentSuit && _SDeck[i].CardValue == currentValue)
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		DEBUG("\n>>Deck is Empty");
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

std::string CCardGamePlay::CardValueToString(const Card& CardToCheck) const
{
	std::string Ret{ "" };
	if (CardToCheck.CardValue > 10 && CardToCheck.CardValue < 14)
	{
		return Ret + std::to_string(FACECARDVALUE);
	}
	else if (CardToCheck.CardValue == 14)
	{
		Ret = "1/11";
		return Ret;
	}
	else
	{
		return Ret + std::to_string(CardToCheck.CardValue);
	}
}

void CCardGamePlay::PrintCard(const Card& CardToPrint)
{
	std::cout << GetName(CardToPrint.CardValue) << "(" << CardValueToString(CardToPrint) << ") of " << CardToPrint.CardSuit;
}

std::string CCardGamePlay::GetCardString(const Card& CardToPrint)
{
	std::string Ret{ GetName(CardToPrint.CardValue) + "(" + CardValueToString(CardToPrint) + ") of " + CardToPrint.CardSuit };
	return Ret;
}

/*
void CCardGamePlay::PrintCard(const Card&& CardToPrint)
{
	std::cout << GetName(CardToPrint.CardValue) << "(" << GetValue(CardToPrint) << ") of " << CardToPrint.CardSuit;
}
*/

void CCardGamePlay::AddUserValue(int ValueToAdd)
{
	_userScore += ValueToAdd;
}

void CCardGamePlay::AddAIValue(int ValueToAdd)
{
	if (_IsFirstRound)
	{
		_publicAIScore += ValueToAdd;
		_AIScore += ValueToAdd;
		_IsFirstRound = false;

		VALUEDEBUG("@@Current hidden AIScore is " << _AIScore 
			<< "\n@@Current publicAIScore is " << _publicAIScore);
	}
	else
	{
		_AIScore += ValueToAdd;

		VALUEDEBUG("@@Current hidden AIScore is " << _AIScore);
	}
}

void CCardGamePlay::AddCardValue(const Card& CardToCheck, const bool IsAI = false)
{
	//Is this a face card?
	if (CardToCheck.CardValue > 10 && CardToCheck.CardValue < 14)
	{
		if (IsAI)
		{
			VALUEDEBUG("\n@@This is a Face Card, adding " << FACECARDVALUE << " to the AIScore");

			AddAIValue(FACECARDVALUE);
		}
		else
		{
			VALUEDEBUG("\n@@This is a Face Card, adding " << FACECARDVALUE << " to the userScore");

			AddUserValue(FACECARDVALUE);
		}
	}
	//Is this an ace?
	else if (CardToCheck.CardValue == 14)
	{
		if (IsAI)
		{
			VALUEDEBUG("\n@@This is an Ace, adding " << FACECARDVALUE + 1 << " to the AIScore");

			if (GetAIScore() == 0 || GetAIScore() + (FACECARDVALUE + 1) == 21)
			{
				AddAIValue(FACECARDVALUE + 1);
			}
			else
			{
				AddAIValue(1);
			}
		}
		else
		{
			VALUEDEBUG("\n@@This is an Ace, adding " << FACECARDVALUE + 1 << " to the userScore");

			if (GetUserScore() == 0 || GetUserScore() + (FACECARDVALUE + 1) == 21)
			{
				AddUserValue(FACECARDVALUE + 1);
			}
			else
			{
				AddUserValue(1);
			}
		}
	}
	else
	{
		if (IsAI)
		{
			VALUEDEBUG("\n@@This is a common card, adding " << CardToCheck.CardValue << " to the AIScore");

			AddAIValue(CardToCheck.CardValue);
		}
		else
		{
			VALUEDEBUG("\n@@This is a common card, adding " << CardToCheck.CardValue << " to the userScore");

			AddUserValue(CardToCheck.CardValue);
		}
	}

	//Ace refactoring, was our first card an Ace but we never reached 21?
	if (IsAI)
	{
		if(_LastAICard.CardValue == 14 && _SCurrentRoundAIDeck.size() == 1)
		{
			VALUEDEBUG("\n@@The first AI card was an Ace and we did not BlackJack on first round. \n@@Removing " << FACECARDVALUE);
			_AIScore -= FACECARDVALUE;
			_publicAIScore -= FACECARDVALUE;
		}
	}
	else
	{
		if (_LastUserCard.CardValue == 14 && _SCurrentRoundUserDeck.size() == 1)
		{
			VALUEDEBUG("\n@@The first user card was an Ace and we did not BlackJack on first round. \n@@Removing " << FACECARDVALUE);
			_userScore -= FACECARDVALUE;
		}
	}

}

void CCardGamePlay::SetLastCard(const Card& CardToCheck, const bool IsAI = false)
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

CCardGamePlay::Card CCardGamePlay::DrawCard( const bool IsAI = false)
{
#ifdef DEBUG
	int GenerateCount{ 0 };
#endif

	Card newCard{ "NULL", 0 };
	bool FindValidCard{ false };
	do
	{
		DEBUG(">>Generating Card");

		newCard.CardSuit = GetSuit(RandomInteger(SUITMIN, SUITMAX)); 
		newCard.CardValue = RandomInteger(CARDVALUEMIN, CARDVALUEMAX);

		if (IsCardValid(newCard.CardSuit, newCard.CardValue))
		{
			FindValidCard = true;
			AddCardToDeck(_SDeck, newCard.CardSuit, newCard.CardValue);
		
			if (IsAI)
			{
				DEBUG("**This is the AI draw nr " << GenerateCount);
				AddCardValue(newCard, IsAI);
				SetLastCard(newCard, IsAI); 

				DEBUG(">>Current Deck size: " << _SDeck.size());
				AddCardToDeck(_SCurrentRoundAIDeck, newCard.CardSuit, newCard.CardValue);
			}
			else
			{
				DEBUG("**This is the Player draw nr " << GenerateCount);
				AddCardValue(newCard, IsAI);
				SetLastCard(newCard, IsAI);

				DEBUG(">>Current Deck size: " << _SDeck.size());
				AddCardToDeck(_SCurrentRoundUserDeck, newCard.CardSuit, newCard.CardValue);
			}
		}
#ifdef DEBUG
		++GenerateCount;
#endif
	} while (!FindValidCard);

	return newCard;
}

void CCardGamePlay::Clear()
{
	_currentBet = 0;
	_userScore = 0;
	_AIScore = 0;
	_publicAIScore = 0;
	_IsHouseStuck = false;
	_IsPlayerStuck = false;
	_IsFirstRound = true;
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
	CHandleInput handleInput;
	std::cout << "How much would you like to bet (maximum " << MAXBET << " credits)?\n" << std::endl;
	do
	{
		userBet = handleInput.HandleInput();
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
	system("cls");

	return true;
}

void CCardGamePlay::AIInitialDraw()
{
	bool IsAI{ true };
	std::cout << "\n\n The house drew " << GetCardString(DrawCard(IsAI)) << " as well as another card, face-down.\n";
	
	AIDEBUG("##AI second card is \n" << GetCardString(DrawCard(IsAI)) << "\n\n");
#ifdef RELEASE
	DrawCard(IsAI);
#endif
}

void CCardGamePlay::PrintTopBar() const
{
	std::cout << "You have " << GetCredits() << " credits. Your bet is " << GetBet() << " credits."
		<< "\n====================================================================" << std::endl;
}

void CCardGamePlay::InitialDraw()
{
	PlaceBet();					//has a CLS
	PrintTopBar();
	std::cout << "\n You have drawn " << GetCardString(DrawCard()) << " and " << GetCardString(DrawCard()) << ".";
	DEBUG("\n\n");
}

void CCardGamePlay::ContinueDraw()
{
#ifdef RELEASE
	system("cls");
#endif
	PrintTopBar();
	std::cout << "\nYour new card is " << GetCardString(DrawCard()) << "." << std::endl;

	//If we want the House to draw alongside the player
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
	if (GetAIScore() < 21 && ShouldAIDraw())
	{
		std::cout << "\n\nThe house's new card is " << GetCardString(DrawCard(IsAI)) << ".\n";

		AIDEBUG("##Current AIScore: " << _AIScore << "\n" 
			<< "##AI Public Score is " << _publicAIScore << "\n");
	}
	else
	{
		HouseSticks();
		std::cout << "\n\n The house sticks." << std::endl;
	}
}

bool CCardGamePlay::ShouldAIDraw()
{
	//from 1 to 14
	int WillAIDraw{ RandomInteger(CARDVALUEMIN, CARDVALUEMAX) };

	AIDEBUG("##WillAIDraw is " << WillAIDraw << "\n"
		<< "##UserScore is " << _userScore << "\n"
		<< "##AIScore is " << _AIScore << "\n"
		<< "##AI Public Score is " << _publicAIScore);

	//If player is not stuck
	if (!IsPlayerStuck())
	{
		if (GetAIScore() >= 16)
		{
			AIDEBUG("\n##NOTSTUCK AIScore is equal to or more than 16, the random went " << WillAIDraw);
			if (WillAIDraw >= 10)
			{
				AIDEBUG("\n##NOTSTUCK >=16 AI will draw");
				return true;
			}
			else
			{
				AIDEBUG("\n##NOTSTUCK >=16 AI will not draw, house sticks");
				return false;
			}
		}
		else
		{
			if (WillAIDraw >= 2)
			{
				AIDEBUG("\n##NOTSTUCK <16 AI will draw");
				return true;
			}
			else
			{
				AIDEBUG("\n##NOTSTUCK <16 AI will not draw, house sticks");
				return false;
			}
		}
	}
	//If player is stuck
	else
	{
		if (GetAIScore() < GetUserScore())
		{
			if (GetAIScore() >= 16)
			{
				AIDEBUG("\n##STUCK AIScore is equal to or more than 16, the random went " << WillAIDraw);
				if (WillAIDraw >= 10)
				{
					AIDEBUG("\n##STUCK LOWSCORE >=16 AI will draw");
					return true;
				}
				else
				{
					AIDEBUG("\n##STUCK LOWSCORE >=16 AI will not draw, house sticks");
					return false;
				}
			}
			else if (GetAIScore() >= 11)
			{
				if (WillAIDraw >= 4)
				{
					AIDEBUG("\n##STUCK LOWSCORE >=11 AI will draw");
					return true;
				}
				else
				{
					AIDEBUG("\n##STUCK LOWSCORE >=11 AI will not draw, house sticks");
					return false;
				}
			}
			else
			{
				AIDEBUG("\n##STUCK LOWSCORE <11 AI will roll");
				return true;
			}
		}
		else
		{
			AIDEBUG("\n##STUCK HIGHSCORE AI will not draw, house sticks");
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

void CCardGamePlay::PrintCurrentHand(const std::vector<Card>& DeckToPrint)
{
	for (size_t i{ 0 }; i < DeckToPrint.size(); ++i)
	{
		std::cout << "\n " << GetName(DeckToPrint[i].CardValue) << "(" << CardValueToString(DeckToPrint[i]) << ") of " << DeckToPrint[i].CardSuit;
	}
}

void CCardGamePlay::PrintFinalCards()
{
	std::cout << "\n====================================================================";
	std::cout << "\nRound finished!\n"
		<< "\nYour cards are";
	PrintCurrentHand(_SCurrentRoundUserDeck);
	if (!IsPlayerStuck())
	{
		std::cout << "\n Your final card was ";
		PrintCard(_LastUserCard);
		std::cout << ".";
	}

	std::cout << "\n\nThe house cards are";
	PrintCurrentHand(_SCurrentRoundAIDeck);
	if (!IsHouseStuck())
	{
		std::cout << "\n The house's final card was ";
		PrintCard(_LastAICard);
		std::cout << ".";
	}
	std::cout << "\n\n====================================================================";
}

void CCardGamePlay::CheckDeck(int DeckSize)
{
	if (DeckSize >= 40)
	{
		DEBUG("\n>>Current Deck size is " << _SDeck.size() << ", resetting deck");
		ResetDeck(_SDeck);
		std::cout << "\nThe deck has been shuffled.\n" << std::endl;
	}
}