#ifndef BOOTS_H
#define BOOTS_H

#include "Armor.h"

class Boots : public Armor
{
public:
	//!
	//! \brief Boots Constructor
	//! \param id id of the item
	//! \param name name of the item
	//! \param description description of the item
	//! \param weight weight of the item
	//! \param level level of the item
	//! \param armorClass armor class of the armor
	//!
	Boots(int id, const std::string& name, const std::string description, int weight, int value, int level, int armorClass);

	// default methods
	Boots(const Boots& other) = default;
	Boots& operator =(const Boots& other) = default;
	~Boots() = default;

	Item* clone() override;

	//!
	//! \brief addEnchantment
	//!
	//! \param enchantment
	//! \return false if the Enchantment is not valid
	//!
	bool addEnchantment(const Enchantment &enchantment);

	Item::Type getType() const override
	{
		return Item::Type::Boots;
	}
};

#endif // BOOTS_H
