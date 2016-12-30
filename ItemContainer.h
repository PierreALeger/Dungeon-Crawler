#ifndef ITEMCONTAINER_H
#define ITEMCONTAINER_H

#include "Item.h"
#include <list>
#include <vector>
#include "Observable.h"

//!
//! \brief The ItemContainer class
//! All ItemContainers should extend this class
//!
class ItemContainer : public Observable
{
public:
	//!
	//! \brief take item from container
	//!				 It also removes it from container
	//! \param itemId id of the item to take
	//! \return pointer to Item
	//!
	virtual Item* take(int itemId) = 0;

	//!
	//! \brief inspect item from container
	//!				Will not remove item from container
	//! \param itemId if of item to take
	//! \return const pointer to const Item
	//!
	virtual const Item* inspect(int itemId) const = 0;


	virtual const std::vector<const Item*> inspectAll() const = 0;

	//!
	//! \brief takeAll takes all item from the container
	//!				leaving it empty
	//! \return a list of all the items
	//!
	virtual std::list<Item*> takeAll() = 0;

	//!
	//! \brief remove item from container
	//! \param itemId id of item to remove
	//!
	virtual void remove(int itemId) = 0;
};

#endif // ITEMCONTAINER_H
