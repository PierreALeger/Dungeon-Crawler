#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <list>
#include <iostream>

class Observer;

class Observable
{
public:
	//!
	//! \brief attach
	//!	adds an observer to the list of observers
	//! \param observer to add
	//!
	void attach(Observer* observer);

	//!
	//! \brief detach
	//! remove an observer to the list of observers
	//! \param observer to remove
	//!
	void detach(Observer* observer);

	//!
	//! \brief notify
	//!	calls the update method of all its observer
	void notify() const;

private:
	std::list<Observer*> observers_;
};

#endif // OBSERVABLE_H
