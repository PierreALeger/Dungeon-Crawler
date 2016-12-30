#ifndef ARMOR_H
#define ARMOR_H

#include "Equipable.h"

class Armor : public Equipable
{
public:
	//!
	//! \brief The Type enum
	//!				Contains all the valid Armor Types
	//!
	enum class Type
	{
		LIGHT,
		MEDIUM,
		HEAVY
	};
	//!
	//! \brief Armor Constructor
	//! \param id id of the item
	//! \param name name of the item
	//! \param description description of the item
	//! \param weight weight of the item
	//! \param level level of the item
	//! \param armorClass armor class of the armor
	//!
	Armor(int id, const std::string& name, const std::string& description, int weight, int value, int level, int armorClass);

	// default methods
	Armor(const Armor& other) = default;
	Armor& operator =(const Armor& other) = default;
	~Armor() = default;

	Item* clone() override;

	//!
	//! \brief addEnchantment
	//!
	//! \param enchantment
	//! \return false if the Enchantment is not valid
	//!
	bool addEnchantment(const Enchantment &enchantment);

	//!
	//! \brief getArmorClass
	//! \return the armor class value
	//!
	int getArmorClass() const
	{
		return armorClass_;
	}

	Item::Type getType() const override
	{
		return Item::Type::Armor;
	}

private:
	int armorClass_;
};

#endif // ARMOR_H
