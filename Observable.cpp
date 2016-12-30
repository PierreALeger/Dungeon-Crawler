#include "Observable.h"
#include "Observer.h"
#include <algorithm>

void Observable::attach(Observer *observer)
{
		observers_.push_back(observer);
}

void Observable::detach(Observer *observer)
{
		observers_.remove(observer);
}

void Observable::notify() const
{
	for(auto& observer : observers_)
	{
		observer->update();
	}
}
