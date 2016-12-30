#pragma once
#include <vector>
#include <queue>
#include <set>
#include <tuple>
#include <iostream>
#include <string>
#include <list>
#include <deque>
#include <SFML/Graphics/Texture.hpp>
#include "GameObject.h"
#include "Character.h"
#include "Exit.h"
#include "Entrance.h"
#include "Chest.h"

using std::vector;
using std::pair;
using std::priority_queue;
using std::set;
using std::tuple;
using std::list;
using std::string;

#include "Observable.h"

class Map : public Observable
{
private:
	class NavNode;		// Forward declarations

public:

	/**
	Optional enum type to make code easier to read when checking tiles
	*/
	enum class TileType {
		EMPTY = 0,
		WALL,
		FLOOR
	};

	Map();
	~Map();

	/**
	Constructor.
	*/
	Map(int id, string name, string description, int x, int y, vector<int> tiles);
	Map(int id, string name, string description, int x, int y);

	void createMapImage();
	void createStaticImage(bool update = false);

	/**
	Generate a bidirected graph that will be stored in m_navMap.
	The size and indexes of the nav map mirror those of the tile map. Each index holds a nav map object that
	can be used to fetch the neighbooring tiles and their cost (1.5 for diag move, 1 for staight).
	The navMap and this method should be kept private in the future and will only be called by validatePath()
	or getRange().
	@param bool characterLess Pass true to disregard character objects while creating the graph (map validation)
	*/
	void generateNavMap(bool characterLess = false, bool noClipping = false);

	/**
	Range of tiles for movement.
	Returns a vector of pairs that contain all available tiles to which the character can move/fire.
	Implements a uniform cost search (Dijsktra) on the nav map to fectch all tiles "in range".
	Use a range of -1 to scan the entire map, set bool to true to disregard character collisions.
	@param from pair<int,int> Starting coordinates
	@param range int the range of tiles in integers
	@param isBlank bool set to true to disregard game objects
	*/
	vector<pair<int, int>> getRange(pair<int,int> from, int range, bool characterLess = false, bool noClipping = false);

	/**
	Path for movement.
	Returns a linked list as the path of tiles towards a destination. Setting the range to a positive integer
	will crop the path and return only the segment that is withing the character's range of motion.
	@param from pair<int,int> start
	@param dest pair<int,int> destination
	@param int range optional, will crop path at this range
	*/
	list<pair<int, int>> getPath(pair<int, int> from, pair<int, int> dest, int range = -1, bool characterLess = false);

	vector<int> getTileMap() { return m_tileMap; }

	// Returns all info at coordinates, tile type, game object
	pair <Map::TileType, std::deque<GameObject*>> getInfoAt(pair<int, int> xy);

	// Getters
	vector <Entrance*> getEntrances() { return m_entrances; }
	vector <Exit*> getExits() { return m_exits; }
	vector <Character*> getCharacters() { return m_characters; }
	vector <Chest*> getChests() { return m_chests; }
	Character* getHero() { return m_hero; }
	int getId() { return m_id; }
	void setId(int id) { m_id = id; }
	string getName() { return m_name; }
	string getDescription() { return m_description; }

	// Give it a pointer and a destination, make sure the destination is a valid position returned from either getRange() or getPath()
	bool move(Character* character, pair<int, int> dest);

	// Open/close the nav node at the determined position (reconnects or deconnects it to neighbooring nodes)
	void navMapUpdate(pair<int, int> pos, bool openNode);


	int getHeight() const { return HEIGHT;}
	int getWidth() const { return WIDTH; }
	sf::Image* getMapImage() { return &m_compositeImage; }

	void removeGameObject(GameObject * gameObject, bool andDelete = true);

	void setTileAt(pair<int, int> xy, int tile);
	void setName(string name) { m_name = name; }
	void setDescription(string descr) { m_description = descr; }

	bool validateMap();   // Generic validator that will check to find at least one pair of connecting entrance/exit

	bool addHero(Character*, pair<int, int>);
	bool addCharacter(Character*, pair<int,int>);
	bool addChest(vector<Item*>, pair<int,int>);
	bool addEntrance(pair<int, int>);
	bool addExit(pair<int, int>);

	bool checkLineOfSight(pair<int, int> from, pair<int, int> to);

	void print();

	vector<NavNode> getNav() { return m_navMap; }
	vector<pair<float, pair<int, int>>> getNeighbors(pair<int, int> pos);

	friend class MakerMap; // Not sure I need this one... better leave it for now

private:
	const int HEIGHT;
	const int WIDTH;
	const int SIZE;
	int m_id;
	string m_name;
	string m_description;
	sf::Image m_mapImage;
	sf::Image m_staticImage;
	sf::Image m_compositeImage;

	vector<int> m_tileMap;

	vector<std::deque<GameObject*>> m_gameObjectMap;
	vector<NavNode> m_navMap;

	vector<Character*> m_characters;
	vector<Entrance*> m_entrances;
	vector<Exit*> m_exits;
	vector<Chest*> m_chests;
	Character* m_hero;
	
	
	bool canTraverse(pair<int, int> coord, bool characterLess = false, bool noCliping = false);

	class NavNode
	{
	private:
		struct Child {
			float m_cost;
			NavNode* navNode;
		};
		vector<Child> m_children;
		pair<int,int> m_pos;

	public:
		NavNode() {}
		NavNode(pair<int,int>);
		~NavNode() {}
		vector<Child> getChildren();
		void addChild(float m_cost, NavNode* child);
		void removeChild(NavNode* child);
		pair<int, int> getPos();
	};

};
