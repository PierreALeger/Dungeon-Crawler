#ifndef RING_H
#define RING_H

#include "Equipable.h"

class Ring : public Equipable
{
public:
	//!
	//! \brief Ring Constructor
	//! \param id id of the item
	//! \param name name of the item
	//! \param description description of the item
	//! \param weight weight of the item
	//! \param level level of the item
	//!
	Ring(int id, const std::string& name, const std::string& description, int weight, int value, int level);

	// default methods
	Ring(const Ring& other) = default;
	Ring& operator =(const Ring& other) = default;
	~Ring() = default;

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
		return Item::Type::Ring;
	}
};

#endif // RING_H
