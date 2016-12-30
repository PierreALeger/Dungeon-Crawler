#ifndef HELMET_H
#define HELMET_H

#include "Armor.h"

class Helmet : public Armor
{
public:
	//!
	//! \brief Helmet Constructor
	//! \param id id of the item
	//! \param name name of the item
	//! \param description description of the item
	//! \param weight weight of the item
	//! \param level level of the item
	//! \param armorClass armor class of the armor
	//!
	Helmet(int id, const std::string& name, const std::string& description, int weight, int value, int level, int armorClass);

	// default methods
	Helmet(const Helmet& other) = default;
	Helmet& operator =(const Helmet& other) = default;
	~Helmet() = default;

	Item* clone() override;

	//!
	//! \brief addEnchantment
	//!
	//! \param enchantment
	//! \return false if the Enchantment is not valid
	//!
	bool addEnchantment(const Enchantment &enchantment) override;

	Item::Type getType() const override
	{
		return Item::Type::Helmet;
	}
};

#endif // HELMET_H
