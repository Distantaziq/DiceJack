#include "CHandleInput.h"

std::string CHandleInput::ProcessInput(const std::string& toInt)
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

const int CHandleInput::HandleInput()
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
