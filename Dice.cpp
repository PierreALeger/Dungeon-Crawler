/**
 *Comp 345 Assignment 1 Part 4 Dice
 *Developer: Marco Dagostino 29587357
 *
 */


//! @file
//! @brief Implementation file for the Dice class
//!

#include "Dice.h"
#include <string>
#include <random>
#include <regex>
#include <GameLogger.h>


//! Implementation of the rolling dice
//! @param string: a string of the form "xdy[+z]".
//! @return an integer which is the addition of the value of all dice and return -1 when the expression does not follow the right format

int Dice::roll(string str)
{
	//! Built a regex expression that will accept an expression of the type xdy[+z]
	std::regex expression("(\\d)d(4|6|8|10|12|20|100)(([\\+|-])(\\d))?$");

	//!If the string matches the regex it will return the sum of the roll and if not it will return -1
	if (std::regex_match(str, expression))
	{

		//!Splitting the regex into different groups that can be extracted
		std::smatch partsOfRegex;
		std::regex_search(str, partsOfRegex, expression);



		//!Storing the parts need to make the roll
		int numberOfRolls = std::stoi(partsOfRegex[1]);
		int limit = std::stoi(partsOfRegex[2]);
		int mod = 0;
		if (partsOfRegex[5].matched) {
			mod = std::stoi(partsOfRegex[5]);
		}
		int sumOfRolls = 0;

		//!Including a random generator and the roll will be following a uniform distribution
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(1, limit);

		for (int i = 0; i < numberOfRolls; i++)
		{
			sumOfRolls += dis(gen);
		}
		sumOfRolls += mod;

		GameLogger::log(LogType::Dice, "Rolled " + std::to_string(sumOfRolls) + " on " + str);

		return sumOfRolls;

	}else
	{
		return -1;
	}

}
