#pragma once
#include <vector>
#include <set>
#include <iostream>
#include "Map.h"
#include "MakerMap.h"
#include "Observer.h"

using std::vector;
using std::cout;
using std::endl;

/*
Not all entrances and exits need to be bound, we only need one valid path for it to work. If we want these changes
to reflect visually, a simple call to the moveToNext or moveToPrevious will return false if those entrances/exits
aren't bound to anything. This could lead to some interesting (and fast) campaign design where we could just have
a bunch of square rooms with entrances and exits around them. The 2 directional nature of the entrances/exits implementation
in the MapContainer struc means that we don't even need to consider them as exits or entrances, but rather doors that lead to 
somewhere else.

This implementation also allows the user to create circular loops, map validation still works and never gets caught in an
infinite loop (cause reasons). Campaign validation also checks the maps internally to ensure a proper path exits.
*/


class Campaign {
public:

	struct MapContainer {
		MapContainer(int id, bool levelOnExit) : map(nullptr), mapId(id), levelUpOnExit(levelOnExit) {}
		Map* map;
		int mapId;
		void initMap(int level = -1) {
			map = MakerMap::load(mapId, level);
		}
		bool levelUpOnExit;
		// pair consists of  [ <exit position>, (<dest containerIndex>, <dest pos>) ] 
		vector<  pair< pair<int, int>, pair<int, pair<int, int>> >  > toNext;
		vector<  pair< pair<int, int>, pair<int, pair<int, int>> >  > toPrevious;
	};

	Campaign();
	Campaign(Campaign* other);
	~Campaign();

	// Call this once before the game loop starts
	void start(Character* hero);

	// ALWAYS CHECK THIS BEFORE ANYTHING ELSE
	bool goalReached();

	Map* getCurrentMap() { return m_mapContainers[m_current].map; }

	// moveToNext() and moveToPrevious() take care of removing the hero from the map
	// the player has left and placing him in the new map. The owning class only needs to call the
	// campaign methods and everything should be done automatically (level up when entering a new map also taken care of). 
	// This begs the question as to who should eventually delete the hero. It could be possible for the campaign to do this eventually.

	/**
	Returns false if trying to move beyond an unbound exit.
	Entering the goal exit will a flag that should be tested
	for in the "managing class" before a new map is displayed. This would be the victory check in
	our game loop. IMPORTANT TO CHECK VICTORY CONDITIONS BEFORE LOADING THE MAP
	*/
	bool moveToNext(pair<int, int> pos);
	/**
	Returns false if trying to move before an unbound entrance
	*/
	bool moveToPrevious(pair<int, int> pos);

	void addMap(int mapId, bool levelUpOnExit);
	bool connectMap(int containerStart, pair<int, int> exitPos, int containerDest, pair<int, int> entrancePos);

	bool disconnect(int container, pair<int, int> pos);

	bool setStart(int start, pair<int,int> entrancePos);
	bool setGoal(int exit, pair<int,int> exitPos);

	vector<MapContainer>* getMapContainers() { return &m_mapContainers; }

	bool validateCampaign();

	void setName(std::string name) { m_name = name; }
	void setDescription(std::string desc) { m_desc = desc; }
	void setId(int id) { m_id = id; }

	std::string getName() { return m_name; }
	std::string getDescription() { return m_desc; }
	int getId() { return m_id; }

	int getCurrentIndex() { return m_current; }

	pair<int, pair<int, int>> getStart() { return m_start; }
	pair<int, pair<int, int>> getGoal() { return m_exitToGoal; }

	void print();

	Character* getHero() { return m_hero; }

	friend class CampaignSaver;


private:

	int m_current;
	pair<int, pair<int, int>> m_exitToGoal;
	pair<int, pair<int, int>> m_start;
	std::string m_name;
	std::string m_desc;
	int m_id;

	bool m_goalReached = false;

	vector<MapContainer> m_mapContainers;
	Character* m_hero;

};
