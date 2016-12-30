#ifndef WEAPON_H
#define WEAPON_H

#include "Equipable.h"

class Weapon : public Equipable
{
public:

	//!
	//! \brief Shield Constructor
	//! \param id id of the item
	//! \param name name of the item
	//! \param description description of the item
	//! \param damageDie damage die to be rolled
	//!	\param isTwoHanded if the weapon is two handed
	//!
	Weapon(int id, const std::string& name, const std::string& description, const std::string& damageDie, bool isTwoHanded);
	//!
	//! \brief Shield Constructor
	//! \param id id of the item
	//! \param name name of the item
	//! \param description description of the item
	//! \param damageDie damage die to be rolled
	//! \param range range of the weapon
	//!	\param isTwoHanded if the weapon is two handed
	//!
	Weapon(int id, const std::string& name, const std::string& description, const std::string& damageDie, int range, bool isTwoHanded);
	//!
	//! \brief Shield Constructor
	//! \param id id of the item
	//! \param name name of the item
	//! \param description description of the item
	//! \param weight weight of the item
	//! \param level level of the item
	//! \param damageDie damage die to be rolled
	//! \param range range of the weapon
	//!	\param isTwoHanded if the weapon is two handed
	//!
	Weapon(int id, const std::string& name, const std::string& description, int weight, int value, int level, const std::string& damageDie, int range, bool isTwoHanded);

	// default methods
	Weapon(const Weapon& other) = default;
	Weapon& operator =(const Weapon& other) = default;
	~Weapon() = default;

	Item* clone() override;

	//!
	//! \brief addEnchantment
	//!
	//! \param enchantment
	//! \return false if the Enchantment is not valid
	//!
	bool addEnchantment(const Enchantment &enchantment) override;

	//!
	//! \brief getDamageDie
	//! \return the damageDie in a valid format for the Dice class
	//!
	std::string getDamageDie() const
	{
		return damageDie_;
	}

	//!
	//! \brief isTwoHanded
	//! \return true if the weapon is two handed
	//!
	bool isTwoHanded() const
	{
		return isTwoHanded_;
	}

	int getRange() const
	{
		return range_;
	}

	Item::Type getType() const override
	{
		return Item::Type::Weapon;
	}

private:
	// normal dice role format
	std::string damageDie_;
	int range_;
	bool isTwoHanded_;
};

#endif // WEAPON_H
