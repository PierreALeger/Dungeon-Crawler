#ifndef ENCHANTMENT_H
#define ENCHANTMENT_H

#include <iostream>

//!
//! \brief The Enchantment class
//!				Only valid Enchantment should exists
//!
class Enchantment
{
public:
	//!
	//! \brief The Type enum
	//!					Contains all the valid Enchantment Types
	//!
	enum class Type
	{
		STRENGTH = 0,
		DEXTERITY,
		CONSTITUTION,
		INTELLIGENCE,
		WISDOM,
		CHARISMA,
		ARMOR_CLASS,
		ATTACK,
		DAMAGE
	};
	//!
	//! \brief Enchantment Constructor
	//! \param type type of the Enchantment
	//! \param bonus bonus of the Enchantment
	//!
	Enchantment(Type type, int bonus);

	//!
	//! \brief Enchantment
	//!				Deleted default constructor
	//!
	Enchantment() = delete;

	// default methods
	Enchantment(const Enchantment& other) = default;
	Enchantment& operator=(const Enchantment& other) = default;
	Enchantment(Enchantment&& other) = default;

	//!
	//! \brief getType
	//! \return type of the Enchantment
	//!
	Type getType() const
	{
		return type_;
	}

	//!
	//! \brief getBonus
	//! \return bonus given by the Enchantment
	//!
	int getBonus() const
	{
		return bonus_;
	}

private:
	Type type_;
	int bonus_;

	static const int MIN_BONUS = 1;
	static const int MAX_BONUS = 5;
};

bool operator==(const Enchantment& lhs, const Enchantment& rhs);

#endif // ENCHANTMENT_H
