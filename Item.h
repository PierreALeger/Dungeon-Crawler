#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item
{
public:
	enum class Type : int
	{
		Consumable = 0,
		Armor,
		Belt,
		Boots,
		Helmet,
		Ring,
		Shield,
		Weapon
	};

	Item(int id, const std::string& name, const std::string& description, int weight, int value);
	virtual ~Item() {}

	virtual Item* clone();

	int getId() const
	{
		return id_;
	}

	int getValue() const
	{
		return value_;
	}

	std::string getName() const
	{
		return name_;
	}

	std::string getDescription() const
	{
		return description_;
	}

	int getWeight() const
	{
		return weight_;
	}

	virtual Item::Type getType() const
	{
		return Type::Consumable;
	}

private:
	int id_;
	int value_;
	std::string name_;
	std::string description_;
	int weight_;
};

#endif // ITEM_H
