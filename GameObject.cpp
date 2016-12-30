#include "GameObject.h"


GameObject::GameObject() : m_pos(pair<int,int>(0,0)), m_id(0), m_traversable(true), m_type(GameObject::Type::Null)
{
}

GameObject::GameObject(pair<int, int> pos, int id, bool traversable, GameObject::Type type) : m_pos(pos), m_id(id), m_traversable(traversable), m_type(type)
{

}

pair<int,int> GameObject::getPos() const
{
	
	return m_pos;
}

void GameObject::setPos(pair<int,int> pos)
{
	m_pos.first = pos.first;
	m_pos.second = pos.second;
}


GameObject::~GameObject()
{
}

