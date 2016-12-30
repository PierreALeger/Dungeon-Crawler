#ifndef EQUIPABLE_H
#define EQUIPABLE_H

#include "Item.h"
#include <vector>
#include "Enchantment.h"

class Equipable : public Item
{
public:
	//!
	//! \brief Equipable Contructor
	//! \param id id of the item
	//! \param name name of the item
	//! \param description description of the item
	//! \param level level of the item
	//!
	Equipable(int id, const std::string& name, const std::string& description, int level);
	//!
	//! \brief Equipable Contructor
	//! \param id id of the item
	//! \param name name of the item
	//! \param description description of the item
	//! \param weight weight of the item
	//! \param level level of the item
	//!
	Equipable(int id, const std::string& name, const std::string& description, int weight, int value, int level);
	virtual ~Equipable() {}

	//!
	//! \brief addEnchantment
	//! \param enchantment Enchantment to add to the Item
	//! \return false if the Enchantment was invalid for the Type of Equipable
	//!
	virtual bool addEnchantment(const Enchantment& enchantment);

	//!
	//! \brief removeEnchantment
	//! \param enchantment Enchantment to remove from the item
	//!
	void removeEnchantment(const Enchantment& enchantment);

	//!
	//! \brief clearEnchantment
	//!				removes all the Enchantments from the item
	//!
	void clearEnchantment();

	//!
	//! \brief getLevel
	//! \return the level of the item
	//!
	int getLevel() const
	{
		return level_;
	}

	//!
	//! \brief getEnchantments
	//! \return return a vector with all the enchantments
	//!
	std::vector<Enchantment> getEnchantments() const
	{
		return enchantments_;
	}
private:
	int level_;
	std::vector<Enchantment> enchantments_;
};

#endif // EQUIPABLE_H
