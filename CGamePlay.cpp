#include"CGamePlay.h"
//#define DEBUG

int CGamePlay::HandleInput()
{
	std::cin.clear();
	int userInput{};
	std::cin >> userInput;
	std::endl(std::cout);

	return userInput;
}

const int CGamePlay::RandomInteger(int Min, int Max)
{
	std::uniform_int_distribution<int> diceRoller(Min, Max);
	return diceRoller(_randomGenerator);
}

const int CGamePlay::DiceRoll()
{
	//old random
	/*if (!IsSeedSet)
	{
		SetSeed();
		IsSeedSet = true;
	}
	return (rand() % DICEMAX) + DICEMIN;
	*/

	return RandomInteger(DICEMIN, DICEMAX);
}

//Player initiated roll
void CGamePlay::PlayerDiceRoll()
{
	int dieOne = DiceRoll();
	int dieTwo = DiceRoll();
#ifdef DEBUG
	std::cerr << ">>PlayerDice is " << dieOne << " and " << dieTwo << " which equals " << dieOne + dieTwo << std::endl;
#endif
	AddPlayerSum(dieOne, dieTwo);
	AIDiceRoll();

#ifndef DEBUG
	system("cls");
#endif
	std::cout << "You have " << _userCredits << " credits. Your bet is " << _currentBet << " credits."
		<< "\n===================================================================="
		<< "\nI have rolled 2 six-sided dice and they came up with " << dieOne << " and " << dieTwo << "."
		<< "\n\n Your score is currently " << _currentSum << "."
		<< "\n The house score is currently " << _currentAISum << ".\n" << std::endl;

	if (IsHouseStuck)
	{
		std::cout << "The house is stuck.\n" << std::endl;
	}
}

//AI initiated roll
void CGamePlay::AIDiceRoll()
{
	if (ShouldAIRoll() && !IsHouseStuck && _currentSum <= 21)
	{
		int AIDieOne = DiceRoll();
		int AIDieTwo = DiceRoll();
#ifdef DEBUG
		std::cerr << ">>AIDice is " << AIDieOne << " and " << AIDieTwo << std::endl;
		std::cerr << "++Adding " << AIDieOne + AIDieTwo << " to the AISum" << std::endl;
#endif
		AddAISum(AIDieOne, AIDieTwo);
#ifdef DEBUG
		std::cerr << "**Current AISum: " << _currentAISum << std::endl;
#endif
	}
}

const bool CGamePlay::ShouldAIRoll()
{
	if (IsPlayerStuck && _currentSum < 21 && _currentAISum > _currentSum)
	{
		IsHouseStuck = true;
	}
	else if (_currentAISum >= 20 && _currentAISum <= 21 && !IsHouseStuck)
	{
		IsHouseStuck = true;
	}
	else if (_currentAISum >= 16 && !IsHouseStuck)
	{
		int WillAIRoll{ DiceRoll() };
#ifdef DEBUG
		std::cerr << "**AIScore is equal to or more than 16, the die went " << WillAIRoll << std::endl;
#endif

		if (WillAIRoll >= 4)
		{
#ifdef DEBUG
			std::cerr << "**AI will roll" << std::endl;
#endif
			return true;
		}
		else
		{
#ifdef DEBUG
			std::cerr << "**AI will not roll, house sticks" << std::endl;
#endif
			IsHouseStuck = true;
			return false;
		}
	}
	else if (_currentAISum >= 10 && _currentAISum <= 15 && !IsHouseStuck)
	{
		int WillAIRoll{ DiceRoll() };
#ifdef DEBUG
		std::cerr << "**AIScore is equal to or more than 10, the die went " << WillAIRoll << std::endl;
#endif

		if (WillAIRoll >= 2)
		{
#ifdef DEBUG
			std::cerr << "**AI will roll" << std::endl;
#endif
			return true;
		}
		else
		{
#ifdef DEBUG
			std::cerr << "**AI will not roll, house sticks" << std::endl;
#endif
			IsHouseStuck = true;
			return false;
		}
	}
	else
	{
#ifdef DEBUG
		std::cerr << "**AIScore is less than 10, rolling" << std::endl;
#endif
		return true;
	}

	return true;
}

const bool CGamePlay::ValidRange(const int Bet)
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

bool CGamePlay::PlaceBet()
{
	bool validBet{ false };
	int userBet{};

	std::cout << "How much would you like to bet (maximum " << MAXBET << " credits)?\n" << std::endl;
	do
	{
		userBet = HandleInput();
		if (ValidAmount(userBet) && ValidRange(userBet))
		{
			_currentBet = userBet;
			validBet = true;
		}
		else
		{
			std::cout << "Incorrect amount. Please try again.\n" << std::endl;
			continue;
		}

	} while (!validBet);

	return true;
}

const void CGamePlay::InitialRoll()
{
	if (PlaceBet())
	{
		PlayerDiceRoll();
	}
}

void CGamePlay::Clear()
{
	_currentBet = 0;
	_currentSum = 0;
	_currentAISum = 0;
	IsHouseStuck = false;
	IsPlayerStuck = false;
}

void CGamePlay::UpdateCredits(const FinishType Outcome)
{

	if (Outcome == FinishType::Win)
	{
		_userCredits += (_currentBet * 2);
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

void CGamePlay::HandleFinish()
{
#ifndef DEBUG
	system("cls");
#endif
	std::cout << "The final score is"
		<< "\n Your total: " << _currentSum
		<< "\n House total: " << _currentAISum << std::endl;

	if (_currentSum == 21 && _currentAISum != 21)
	{
		//If Player has 21
		std::cout << "\nDiceJack! Credits doubled!"
			<< "\n You gained " << _currentBet * 2 << " credits!\n" << std::endl;
		UpdateCredits(FinishType::Win);
		Clear();
	}
	else if (_currentSum == 21 && _currentAISum == 21 || _currentSum == _currentAISum)
	{
		//If both have 21 or end up with the same score below 21
		std::cout << "\nShared win! Money back."
			<< "\n " << _currentBet << " credits was returned.\n" << std::endl;
		Clear();
	}
	else if (_currentSum != 21 && _currentAISum == 21 || _currentAISum < 21 && _currentAISum > _currentSum)
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

		Clear();
	}
	else if (_currentSum < 21 && _currentSum > _currentAISum || _currentAISum > 21 && _currentSum < 21)
	{
		//If Player has less than 21 but more than AI OR the AI got fat
		std::cout << "\nThe house lost. You win, credits doubled!"
			<< "\n You gained " << _currentBet * 2 << " credits!\n" << std::endl;
		UpdateCredits(FinishType::Win);
		Clear();
	}
	else
	{
		//If the Player is fat
		std::cout << "\nSorry, that's too much. You lost the bet."
			<< "\n You lost " << _currentBet << " credits!\n" << std::endl;
		UpdateCredits(FinishType::Lost);
		Clear();
	}
}

void CGamePlay::HandleRound()
{
	do
	{
		std::cout << "You will go bust if you score over " << MAXJACK
			<< ".\nWhat would you like to do next?\n"
			<< " [1] Roll Again\n"
			<< " [2] Stick (and halve your losses)\n" << std::endl;

		int userInput{ HandleInput() };

		//We want to continue rolling
		if (userInput == 1)							
		{
			PlayerDiceRoll();
		}
		//We don't want to play more this round
		else if (userInput == 2)					
		{
			IsPlayerStuck = true;
			while (!IsHouseStuck)
			{
				AIDiceRoll();
			}
			break;
		}
		else
		{
			std::cout << "Incorrect input. Please try again.\n" << std::endl;
			continue;
		}
	} while (IsPlayerScoreValid() && IsAIScoreValid());				//As long as no one is fat

	HandleFinish();
}

const bool CGamePlay::IsUserCreditsValid()
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

int CGamePlay::MainGame(bool& isGameOver)
{
	do
	{
		if (!IsUserCreditsValid())
		{
			isGameOver = true;
		}
		else
		{
			std::cout << "You have " << _userCredits << " credits.\n"
				<< "What would you like to do next?\n"
				<< " [1] Play a round\n"
				<< " [2] Leave the table\n" << std::endl;

			int userInput{ HandleInput() };
			//We want to play a round
			if (userInput == 1)
			{
				//Place bet
				InitialRoll();	
				//Do we want to continue the round
				HandleRound();									
				continue;
			}
			//We don't want to play anymore
			else if (userInput == 2)
			{
				isGameOver = true;
				return 0;
			}
			else
			{
				std::cout << "Incorrect input. Please try again.\n" << std::endl;
				continue;
			}
		}

	} while (!isGameOver);
}

const int CGamePlay::MainLoop()
{
	bool gameOver{ false };

	std::cout << "Welcome to DiceJack! Please take a seat at the table!\n"
		<< "You start with " << _userCredits << " credits. You win if you get " << GOAL << " credits!\n" << std::endl;

	while (!gameOver && _userCredits != GOAL)
	{
		MainGame(gameOver);
	}

	std::cout << "\nThank you for playing! Have a nice day!" << std::endl;

	return 0;
}
