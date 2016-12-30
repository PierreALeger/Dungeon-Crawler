#include "Campaign.h"
using std::make_pair;

bool Campaign::moveToNext(pair<int, int> pos)
{
	if (m_current == m_exitToGoal.first && pos == m_exitToGoal.second) {
		m_goalReached = true;
		return true;
	}
	int dest = -1;
	pair<int, int> newPos;
	for (auto el : m_mapContainers[m_current].toNext) {
		if (el.first == pos) {
			dest = el.second.first;
			newPos = el.second.second;
		}
	}
	if (dest == -1) return false;
	m_mapContainers[m_current].map->removeGameObject(m_hero, false);
	if (m_mapContainers[dest].map == nullptr) {
		if (m_mapContainers[m_current].levelUpOnExit) {
			m_hero->levelUp();
			m_mapContainers[m_current].levelUpOnExit = false;
		}
		m_mapContainers[dest].initMap(m_hero->getLevel());

		// This will delete all unbounded entrances and exits from the maps

		bool found;
		int containerNumber = dest;

		auto container = m_mapContainers[dest];

		for (auto &entranceMap : container.map->getEntrances())
		{
			found = false;

			for (auto &entranceCamp : container.toPrevious)
			{
				if (entranceMap->getPos().first == entranceCamp.first.first &&
					entranceMap->getPos().second == entranceCamp.first.second)
				{
					found = true;
					break;
				}
			}
			if (!found) {
				container.map->removeGameObject(entranceMap);
			}
		}
		for (auto &exitMap : container.map->getExits())
		{
			found = false;

			for (auto &exitCamp : container.toNext)
			{
				if (exitMap->getPos().first == exitCamp.first.first &&
					exitMap->getPos().second == exitCamp.first.second)
				{
					found = true;
					break;
				}
			}
			if (!found) {
				if (containerNumber != m_exitToGoal.first ||
					exitMap->getPos().first != m_exitToGoal.second.first ||
					exitMap->getPos().second != m_exitToGoal.second.second)
					container.map->removeGameObject(exitMap);
			}
		}

		containerNumber++;

		m_mapContainers[m_current].levelUpOnExit = false;
	}
	m_mapContainers[dest].map->addHero(m_hero, newPos);
	m_current = dest;
	return true;
}

Campaign::Campaign()
	: m_start(std::make_pair(-1, std::make_pair(-1,-1))), 
	m_exitToGoal(std::make_pair(-1, std::make_pair(-1, -1))),
	m_goalReached(false)
{
}

Campaign::Campaign(Campaign * other) : m_goalReached(false)
{
	vector<MapContainer>* otherContainers = other->getMapContainers();
	for (int i = 0; i < otherContainers->size(); i++) {
		addMap(otherContainers->at(i).mapId, 1);
		for (auto el : otherContainers->at(i).toNext) {
			m_mapContainers.back().toNext.push_back(el);
		}
		for (auto el : otherContainers->at(i).toPrevious) {
			m_mapContainers.back().toPrevious.push_back(el);
		}
	}
	m_start = other->getStart();
	m_exitToGoal = other->getGoal();
}


Campaign::~Campaign()
{
	for (auto el : m_mapContainers) {
		delete el.map;
	}
}

void Campaign::start(Character* hero)
{
	m_hero = hero;
	m_mapContainers[m_start.first].map = MakerMap::load(m_mapContainers[m_start.first].mapId, m_hero->getLevel());
	m_mapContainers[m_start.first].map->addHero(m_hero, m_start.second);
	m_current = m_start.first;

	// Remove unbounded exits/entrances

	bool found;
	int containerNumber = 0;

	
	for (auto &entranceMap : m_mapContainers[m_start.first].map->getEntrances())
	{
		found = false;

		for (auto &entranceCamp : m_mapContainers[m_start.first].toPrevious)
		{
			if (entranceMap->getPos().first == entranceCamp.first.first &&
				entranceMap->getPos().second == entranceCamp.first.second)
			{
				found = true;
				break;
			}
		}
		if (!found) {
			m_mapContainers[m_start.first].map->removeGameObject(entranceMap);
		}
	}
	for (auto &exitMap : m_mapContainers[m_start.first].map->getExits())
	{
		found = false;

		for (auto &exitCamp : m_mapContainers[m_start.first].toNext)
		{
			if (exitMap->getPos().first == exitCamp.first.first &&
				exitMap->getPos().second == exitCamp.first.second)
			{
				found = true;
				break;
			}
		}
		if (!found) {
			if (containerNumber != m_exitToGoal.first ||
				exitMap->getPos().first != m_exitToGoal.second.first ||
				exitMap->getPos().second != m_exitToGoal.second.second)
				m_mapContainers[m_start.first].map->removeGameObject(exitMap);
		}
	}

}


bool Campaign::goalReached()
{
	return m_goalReached;
}

bool Campaign::moveToPrevious(pair<int, int> pos)
{
	if (m_current == m_start.first && pos == m_start.second) {
		return false;
	}
	int dest = -1;
	pair<int, int> newPos;
	for (auto el : m_mapContainers[m_current].toPrevious) {
		if (el.first == pos) {
			dest = el.second.first;
			newPos = el.second.second;
		}
	}
	if (dest == -1) return false;
	if (m_mapContainers[dest].map == nullptr) return false;
	m_mapContainers[m_current].map->removeGameObject(m_hero, false);
	m_current = dest;
	m_mapContainers[m_current].map->addHero(m_hero, newPos);
	return true;
}

void Campaign::addMap(int mapId, bool levelUpOnExit)
{
	m_mapContainers.push_back(MapContainer(mapId, levelUpOnExit));

}

bool Campaign::connectMap(int containerStart, pair<int, int> exitPos, int containerDest, pair<int, int> entrancePos)
{

	m_mapContainers[containerStart].toNext.push_back(make_pair(exitPos, make_pair(containerDest, entrancePos)));
	m_mapContainers[containerDest].toPrevious.push_back(make_pair(entrancePos, make_pair(containerStart, exitPos)));
	return true;
}

bool Campaign::disconnect(int container, pair<int, int> pos)
{
	vector<  pair< pair<int, int>, pair<int, pair<int, int>> >  >::iterator next;
	vector<  pair< pair<int, int>, pair<int, pair<int, int>> >  >::iterator previous;

	next = m_mapContainers[container].toNext.begin();
	while (next != m_mapContainers[container].toNext.end()) 
	{
		if (next->first.first == pos.first && next->first.second == pos.second) 
		{
			previous = m_mapContainers[next->second.first].toPrevious.begin();

			while (previous != m_mapContainers[next->second.first].toPrevious.end())
			{
				if (previous->first.first == next->second.second.first && previous->first.second == next->second.second.second)
				{
					break;
				}
				previous++;
			}
			break;
		}
		next++;
	}
	if (next != m_mapContainers[container].toNext.end() && previous != m_mapContainers[next->second.first].toPrevious.end()) {
		m_mapContainers[next->second.first].toPrevious.erase(previous);
		m_mapContainers[container].toNext.erase(next);
		return true;
	}
	return false;
}


bool Campaign::setStart(int start, pair<int, int> entrancePos)
{
	m_start.first = start;
	m_start.second = entrancePos;
	return true;
}

bool Campaign::setGoal(int exit, pair<int, int> exitPos)
{
	m_exitToGoal.first = exit;
	m_exitToGoal.second = exitPos;
	return true;
}

bool Campaign::validateCampaign()
{
	if (m_start.first == -1 || m_exitToGoal.first == -1) return false;
	Campaign* candidate = new Campaign(this);

	for (int i = 0; i < candidate->getMapContainers()->size(); i++) {
		candidate->getMapContainers()->at(i).initMap();
		candidate->getMapContainers()->at(i).map->generateNavMap(true);
	}

	list<  pair< int, pair<int,int> >  > openList;
	set<  pair< int, pair<int,int> >  > closedList;

	openList.push_front(candidate->getStart());
	closedList.insert(candidate->getStart());

	pair< int, pair<int,int> > current;
	pair<int, int> currentPos;
	pair<int, int> exit;

	bool valid = false;

	while (true) {

		current = openList.front();
		if (current.first == candidate->getGoal().first) {
			if (candidate->getMapContainers()->at(current.first).map->getPath(current.second, candidate->getGoal().second).size() != 0) {
				valid = true;
				break;
			}
		}
		currentPos = current.second;
		openList.pop_front();
		
		for (auto &el : candidate->getMapContainers()->at(current.first).toNext) {
			exit = el.first;
			if (candidate->getMapContainers()->at(current.first).map->getPath(currentPos, exit).size() != 0) {
				if (closedList.insert(el.second).second) {
					openList.push_back(el.second);
				}
			}
		}
		if (openList.size() == 0) {
			break;
		}
	}
	delete candidate;
	return valid;
	
}

void Campaign::print()
{
	cout << "=========================================================================\n";
	cout << "==================           Campaign viewer         ====================\n";
	cout << "=========================================================================\n";
	cout << "Name : " << getName() << "\n";
	cout << "Description : " << getDescription() << "\n";
	cout << "Id : " << getId() << "\n";
	cout << "\n";
	cout << "Start : ";
	if (m_start.first == -1) {
		cout << "not set\n";
	}
	else {
		cout << "Container #" << m_start.first << " at entrance (" << m_start.second.first << "," << m_start.second.second << ")\n";
	}
	cout << "End : ";
	if (m_exitToGoal.first == -1) {
		cout << "not set\n";
	}
	else {
		cout << "Container #" << m_exitToGoal.first << " at exit (" << m_exitToGoal.second.first << "," << m_exitToGoal.second.second << ")\n";
	}
	cout << endl;
	cout << "-----------Loaded map ids-----------\n";
	for (int i = 0; i < m_mapContainers.size(); i++) {
		cout << "Container #" << i << " - Map id #" << m_mapContainers[i].mapId << "\n";
	}
	cout << "\n";
	cout << "-----Active Connections (2-way)-----\n";
	for (int i = 0; i < m_mapContainers.size(); i++) {
		if (m_mapContainers[i].toNext.size() == 0) continue;
		cout << "Container #" << i << endl;
		for (auto el : m_mapContainers[i].toNext) {
			cout << "   Exit at (" << el.first.first << "," << el.first.second << ") ";
			cout << "leads to Map Container #" << el.second.first;
			cout << ", entrance at (" << el.second.second.first << "," << el.second.second.second << ")\n";
		}
	}
	cout << "\n";
	cout << "------Unbounded Entrances/Exits-----\n";
	bool bound;
	for (int i = 0; i < m_mapContainers.size(); i++) {
		if (m_mapContainers[i].toNext.size() == 0 && m_mapContainers[i].toPrevious.size() == 0) continue;
		cout << "Container #" << i << endl;
		for (auto &map : m_mapContainers[i].map->getExits()) {
			bound = false;
			if (m_exitToGoal.first == i && map->getPos().first == m_exitToGoal.second.first && map->getPos().second == m_exitToGoal.second.second) {
				bound = true;
			}
			for (auto &container : m_mapContainers[i].toNext) {
				if (map->getPos().first == container.first.first && map->getPos().second == container.first.second) {
					bound = true;
					break;
				}
				
			}
			if (!bound) cout << "   Exit at (" << map->getPos().first << "," << map->getPos().second << ")\n";
		}
		for (auto &map : m_mapContainers[i].map->getEntrances()) {
			bound = false;
			if (m_start.first == i && map->getPos().first == m_start.second.first && map->getPos().second == m_start.second.second) {
				bound = true;
			}
			for (auto &container : m_mapContainers[i].toPrevious) {
				if (map->getPos().first == container.first.first && map->getPos().second == container.first.second) {
					bound = true;
					break;
				}
			}
			if (!bound) cout << "   Entrance at (" << map->getPos().first << "," << map->getPos().second << ")\n";
		}
	}
	cout << endl;
	cout << "=========================================================================\n";


}
