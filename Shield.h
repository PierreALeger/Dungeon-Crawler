#ifndef SHIELD_H
#define SHIELD_H

#include "Armor.h"

class Shield : public Armor
{
public:
	//!
	//! \brief Shield Constructor
	//! \param id id of the item
	//! \param name name of the item
	//! \param description description of the item
	//! \param weight weight of the item
	//! \param level level of the item
	//! \param armorClass armor class of the armor
	//!
	Shield(int id, const std::string& name, const std::string& description, int weight, int value, int level, int armorClass);

	// default methods
	Shield(const Shield& other) = default;
	Shield& operator =(const Shield& other) = default;
	~Shield() = default;

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
		return Item::Type::Shield;
	}
};

#endif // SHIELD_H
