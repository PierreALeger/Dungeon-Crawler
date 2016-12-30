#ifndef INVENTORY_H
#define INVENTORY_H

#include "ItemContainer.h"
#include <list>
#include <iostream>

//!
//! \brief The Inventory class implements the ItemContainer
//!
class Inventory : public ItemContainer
{
public:
	//!
	//! \brief Inventory Constructor
	//!				Is empty by default
	//!
	Inventory();
	//!
	//! \brief Inventory Copy Constructor
	//! \param other Inventory to be copied
	//!
	Inventory(const Inventory& other);
	//!
	//! \brief operator =
	//! \param other Inventory to be copied
	//! \return itself with the same items ass other
	//!
	Inventory& operator =(const Inventory& other);
	~Inventory();

	Item* take(int itemId) override;
	std::list<Item*> takeAll() override;
	const Item* inspect(int itemId) const override;
	const std::vector<const Item*> inspectAll() const override;
	void remove(int itemId) override;

	//!
	//! \brief store Stores item in the inventory
	//!				The inventory will free the memory
	//! \param item The item pointer given to store
	//!
	void store(Item* item);

	//!
	//! \brief print temp method to show inside inventory
	//!
	void print() const
	{
		for(auto& el : inventory_)
			{
				std::cout << el->getName() << " --- " << el->getDescription() << std::endl;
			}
	}

	std::list<Item*> getInventory() const
	{
		return inventory_;
	}

private:
	std::list<Item*> inventory_;
};

#endif // INVENTORY_H
