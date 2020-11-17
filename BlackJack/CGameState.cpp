#include "CGameState.h"

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
			std::cout << "Incorrect input. Please try again.\n" << std::endl;
			continue;
		}
	} while (IsScoreValid());
#ifndef DEBUG
	system("cls");
#endif
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