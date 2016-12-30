#ifndef BELT_H
#define BELT_H

#include "Armor.h"

class Belt : public Armor
{
public:
	//!
	//! \brief Belt Constructor
	//! \param id id of the item
	//! \param name name of the item
	//! \param description description of the item
	//! \param weight weight of the item
	//! \param level level of the item
	//! \param armorClass armor class of the armor
	//!
	Belt(int id, const std::string& name, const std::string& description, int weight, int value, int level, int armorClass);

	// default methods
	Belt(const Belt& other) = default;
	Belt& operator=(const Belt& other) = default;
	~Belt() = default;

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
		return Item::Type::Belt;
	}
};

#endif // BELT_H
