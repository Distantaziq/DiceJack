#include "CGameState.h"

void CGameState::HandleFinish()
{
	PrintFinalCards();

	std::cout << "\n\nThe final score is"
		<< "\n Your total: " << GetUserScore()
		<< "\n House total: " << GetAIScore() << std::endl;

	if (GetUserScore() == 21 && GetAIScore() != 21)
	{
		//If Player has 21
		std::cout << "\nBlackJack! Credits doubled!"
			<< "\n You gained " << GetBet() * 2 << " credits!\n" << std::endl;
		UpdateCredits(FinishType::BlackJack);
	}
	else if (GetUserScore() == GetAIScore())
	{
		//If both have 21 or end up with the same score below 21
		std::cout << "\nShared win! Money back."
			<< "\n Your bet was returned.\n" << std::endl;
	}
	else if (GetUserScore() != 21 && GetAIScore() == 21 || GetAIScore() < 21 && GetAIScore() > GetUserScore())
	{
		//If AI has 21 OR if AI has less than 21 but more than Player
		if (IsPlayerStuck())
		{
			std::cout << "\nThe house wins. You stuck but lost the bet."
				<< "\n Sticking: You lost " << GetBet() / 2 << " credits!\n" << std::endl;
			UpdateCredits(FinishType::Stuck);
		}
		else
		{
			std::cout << "\nThe house wins. You lost the bet."
				<< "\n You lost " << GetBet() << " credits!\n" << std::endl;
			UpdateCredits(FinishType::Lost);
		}
	}
	else if (GetUserScore() < 21 && GetUserScore() > GetAIScore() || GetAIScore() > 21 && GetUserScore() < 21)
	{
		//If Player has less than 21 but more than AI OR the AI went bust
		std::cout << "\nThe house lost. You win the bet!"
			<< "\n You gained " << GetBet() << " credits!\n" << std::endl;
		UpdateCredits(FinishType::Win);
	}
	else
	{
		//If the Player went bust
		std::cout << "\nSorry, that's too much. You lost the bet."
			<< "\n You lost " << GetBet() << " credits!\n" << std::endl;
		UpdateCredits(FinishType::Lost);
	}
	Clear();
	std::cout << "====================================================================\n";
}

void CGameState::PrintUpdatedScore()
{
	std::cout << "\n===================================================================="
		<< "\n Your score is currently " << GetUserScore() << "."
		<< "\n The house's (known) score is currently ";
	if (GetPublicAIScore() == 1)
	{
		std::cout << "an Ace.\n\n";
	}
	else
	{
		std::cout << GetPublicAIScore() << ".\n" << std::endl;
	}

	if (IsHouseStuck() && GetUserScore() != 21)
	{
		std::cout << "The house is stuck.\n" << std::endl;
	}
}

bool CGameState::IsUserCreditsValid() const
{
	if (GetCredits() >= 300)
	{
		std::cout << "You have " << GetCredits() << " credits!"
			<< "\nYou beat the house! Lady luck is on your side!" << std::endl;
		return false;
	}
	else if (GetCredits() <= 0)
	{
		std::cout << "You are all out of credits! The game is over!" << std::endl;
		return false;
	}
	else
	{
		return true;
	}
}

void CGameState::HandleRound()
{
	do
	{
		PrintUpdatedScore();
		CheckDeck(GetDeckSize());
		std::cout << "You will go bust if you score over " << GetMaxJack()
			<< ".\nWhat would you like to do next?\n"
			<< " [1] Draw another card\n"
			<< " [2] Stick (and halve your losses)\n" << std::endl;
		CHandleInput handleInput;
		int userInput{ handleInput.HandleInput() };

		if (userInput == 1)
		{
			ContinueDraw();
		}
		else if (userInput == 2)
		{
			PlayerSticks();
			while (!IsHouseStuck())
			{
				AIDraw();
			}
			break;
		}
		else
		{
			std::cout << "Incorrect input. Please try again." << std::endl;
			continue;
		}
	} while (IsScoreValid());

	system("cls");
	HandleFinish();
}

void CGameState::MainGame(bool& IsGameOver)
{
	do
	{
		if (!IsUserCreditsValid())
		{
			IsGameOver = true;
		}
		else
		{
			std::cout << "You have " << GetCredits() << " credits.\n"
				<< "What would you like to do next?\n"
				<< " [1] Play a round\n"
				<< " [2] Leave the table\n" << std::endl;
			CHandleInput handleInput;
			int userInput{ handleInput.HandleInput() };

			if (userInput == 1)
			{
				InitialDraw();
				AIInitialDraw();
				//If someone got >= 21 on the initial round
				if (!IsScoreValid())
				{
					if (GetUserScore() >= 21)
					{
						if (GetUserScore() > 21)
						{
							std::cout << "\nYour score went bust." << std::endl;
						}
						else
						{
							std::cout << "\nYou got a BlackJack!" << std::endl;
						}
					}

					if (GetAIScore() >= 21)
					{
						if (GetAIScore() > 21)
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

void CGameState::MainLoop()
{
	bool IsGameOver{ false };
	std::cout << "Welcome to BlackJack! Please take a seat at the table!\n"
		<< "You start with " << GetCredits() << " credits. You win if you get " << GetCreditsGoal() << " credits!\n" << std::endl;

	while (!IsGameOver)
	{
		MainGame(IsGameOver);
	}

	std::cout << "\nThank you for playing! Have a nice day!" << std::endl;
}