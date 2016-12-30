#pragma once
#include <vector>
using std::pair;
using std::vector;

class GameObject
{
public:

	enum class Type {
		Null,
		Character,
		Chest,
		Entrance,
		Exit
	};

	GameObject();
	GameObject(pair<int,int> pos, int id, bool traversable, GameObject::Type);
	pair<int,int> getPos() const;
	void setPos(pair<int,int>);
	int getId() const { return m_id; }
	void setId(int id) { m_id = id; }
	bool isTraversable() { return m_traversable; }
	void setTraversable(bool isTraversable) { m_traversable = isTraversable; }
	virtual ~GameObject();
	GameObject::Type getType() const { return m_type; }

private:
	pair<int, int> m_pos;
	int m_id;
	bool m_traversable;
	GameObject::Type m_type;
};

