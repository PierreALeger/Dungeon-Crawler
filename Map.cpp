#include "Map.h"
#include "Character.h"
#include "GameLogger.h"
#include <iostream>
#include <fstream>
#include <cmath>
using std::endl;
using std::cout;

#include <algorithm>

Map::Map() : HEIGHT(0), WIDTH(0), SIZE(0)
{
	// left intentionnaly blank
}

Map::~Map()
{
	for (auto el : m_characters) {
		delete el;
	}
	for (auto el : m_chests) {
		delete el;
	}
	for (auto el : m_entrances) {
		delete el;
	}
	for (auto el : m_exits) {
		delete el;
	}
}

Map::Map(int id, string name, string description, int x, int y, vector<int> tiles) 
	: m_id(id), m_name(name), m_description(description), HEIGHT(y), WIDTH(x), SIZE(x*y)
{
	for (auto& el : tiles) {
		m_tileMap.push_back(el);
		m_gameObjectMap.push_back(std::deque<GameObject*>{nullptr});
	}
}

Map::Map(int id, string name, string description, int x, int y) 
	: m_id(id), m_name(name), m_description(description), HEIGHT(y), WIDTH(x), SIZE(x*y)

{
	for (int i = 0; i < SIZE; i++) {
		m_tileMap.push_back(2);
		m_gameObjectMap.push_back(std::deque<GameObject*>{nullptr});
	}
}

void Map::createMapImage()
{
	
	struct TextureFinder {
		int packed;
		TextureFinder() : TextureFinder(0, 0, 0, 0, 0, 0, 0, 0) {}
		TextureFinder(int n, int ne, int e, int se, int s, int sw, int w, int nw) {
			int m[8]; // N, NE, E, SE, S, SW, W, NE
			m[0] = (n != 2) ? 1 : 2;
			m[1] = (ne != 2) ? 1 : 2;
			m[2] = (e != 2) ? 1 : 2;
			m[3] = (se != 2) ? 1 : 2;
			m[4] = (s != 2) ? 1 : 2;
			m[5] = (sw != 2) ? 1 : 2;
			m[6] = (w != 2) ? 1 : 2;
			m[7] = (nw != 2) ? 1 : 2;
			packed = 0;
			for (int i = 0; i < 8; i++) {
				packed += m[i];
				packed *= 10;
			}
			packed /= 10;
		}

		sf::IntRect getTileImage() {
			std::ifstream ifs("./assets/spriteSheets/MapSheetInfo.txt");
			int buffer = 0;
			while (ifs.good()) {
				ifs >> buffer;
				if (buffer == packed) break;
				ifs >> buffer;
			}
			
			int textureIndex;
			ifs >> textureIndex;
			ifs.close();
			return sf::IntRect(((textureIndex ) % 7) * 40, ((textureIndex) / 7) * 40, 40, 40);
		}
		
		
	};

	vector<TextureFinder> textureFinders;
	textureFinders.resize(WIDTH*HEIGHT);

	int n, s, e, w;
	int ne, se, sw, nw;
	for (int i = 0; i < m_tileMap.size(); i++) {
		
		ne = se = sw = nw = 2;
		n = (i < WIDTH) ?  1 : m_tileMap[i - WIDTH];
		e = (i % WIDTH == WIDTH - 1) ? 1 : m_tileMap[i + 1];
		s = (i >= (HEIGHT - 1) * WIDTH) ?  1: m_tileMap[i + WIDTH];
		w = (i % WIDTH == 0) ? 1 : m_tileMap[i - 1];
		if (n != 2 && e != 2) ne = (i < WIDTH || i % WIDTH == WIDTH - 1) ? 1 : m_tileMap[i + 1 - WIDTH];
		if (s != 2 && e != 2) se = (i >= (HEIGHT - 1) * WIDTH || i % WIDTH == WIDTH - 1) ? 1 : m_tileMap[i + 1 + WIDTH];
		if (s != 2 && w != 2) sw = (i >= (HEIGHT - 1) * WIDTH || i % WIDTH == 0) ? 1 : sw = m_tileMap[i - 1 + WIDTH];
		if (n != 2 && w != 2) nw = (i < WIDTH || i % WIDTH == 0) ? 1 : m_tileMap[i - 1 - WIDTH];

		textureFinders[i] = TextureFinder(n, ne, e, se, s, sw, w, nw);

	}

	sf::Image blank;
	blank.create(40, 40);

	sf::Image sheet;
	sheet.loadFromFile("./assets/spriteSheets/MapSheet.png");

	sf::Image floor;
	floor.loadFromFile("./assets/spriteSheets/floor.png");

	m_mapImage.create(40 * WIDTH, 40 * HEIGHT);
	
	sf::IntRect dest;
	dest.width = 40;
	dest.height = 40;

	for (int i = 0; i < m_tileMap.size(); i++) {
		dest.left = (i % WIDTH) * 40;
		dest.top = (i / WIDTH) * 40;
		switch (m_tileMap[i]) {
		case 0:
			m_mapImage.copy(blank, dest.left, dest.top);
			break;
		case 2:
			m_mapImage.copy(floor, dest.left, dest.top);
			break;
		case 1:
			m_mapImage.copy(sheet, dest.left, dest.top, textureFinders[i].getTileImage());
			break;
		default:
			break;
		}
	}

	m_compositeImage.create(WIDTH * 40, HEIGHT * 40);
	m_compositeImage.copy(m_mapImage, 0, 0, sf::IntRect(0, 0, WIDTH * 40, HEIGHT * 40));
	createStaticImage();
	m_compositeImage.copy(m_staticImage, 0, 0, sf::IntRect(0, 0, WIDTH * 40, HEIGHT * 40), true);

}


void Map::createStaticImage(bool update)
{
	m_staticImage.create(WIDTH * 40, HEIGHT * 40, sf::Color::Transparent);

	sf::IntRect dest;

	sf::Image chestImage;
	chestImage.loadFromFile("./assets/spriteSheets/chest.png");
	sf::Image entranceImage;
	entranceImage.loadFromFile("./assets/spriteSheets/entrance.png");
	sf::Image exitImage;
	exitImage.loadFromFile("./assets/spriteSheets/exit.png");

	for (auto &el : getChests()) {
		dest.left = el->getPos().first * 40;
		dest.top = el->getPos().second * 40;
		m_staticImage.copy(chestImage, dest.left, dest.top, sf::IntRect(0, 0, 40, 40), true);
	}

	for (auto &el : getEntrances()) {
		dest.left = el->getPos().first * 40;
		dest.top = el->getPos().second * 40;
		m_staticImage.copy(entranceImage, dest.left, dest.top, sf::IntRect(0, 0, 40, 40), true);
	}

	for (auto &el : getExits()) {
		dest.left = el->getPos().first * 40;
		dest.top = el->getPos().second * 40;
		m_staticImage.copy(exitImage, dest.left, dest.top, sf::IntRect(0, 0, 40, 40), true);
	}

	if (update) {
		m_compositeImage.create(WIDTH * 40, HEIGHT * 40);
		m_compositeImage.copy(m_mapImage, 0, 0, sf::IntRect(0, 0, WIDTH * 40, HEIGHT * 40), true);
		m_compositeImage.copy(m_staticImage, 0, 0, sf::IntRect(0, 0, WIDTH * 40, HEIGHT * 40), true);
	}
}


void Map::generateNavMap(bool characterLess, bool noClipping)
{
	int x;
	int y;
	pair<int, int> xy;
	NavNode* other;
	m_navMap.clear();
	for (int i = 0; i < SIZE; i++) {
		x = i % WIDTH;
		y = i / WIDTH;
		xy.first = x;
		xy.second = y;
		m_navMap.push_back(NavNode(xy));
	}
	for (int i = 0; i < SIZE; i++) {
		x = i % WIDTH;
		y = i / WIDTH;
		xy.first = x;
		xy.second = y;
		if (x - 1 >= 0)
		{
			other = &m_navMap[i - 1];
			if ( canTraverse(other->getPos(), characterLess, noClipping) ) m_navMap[i].addChild(1, other);
		}
		if (x + 1 < WIDTH)
		{
			other = &m_navMap[i + 1];
			if (canTraverse(other->getPos(), characterLess, noClipping)) m_navMap[i].addChild(1, other);
		}
		if (y - 1 >= 0)
		{
			other = &m_navMap[i - WIDTH];
			if (canTraverse(other->getPos(), characterLess, noClipping)) m_navMap[i].addChild(1, other);
		}
		if (y + 1 < HEIGHT)
		{
			other = &m_navMap[i + WIDTH];
			if (canTraverse(other->getPos(), characterLess, noClipping)) m_navMap[i].addChild(1, other);
		}
		if (x - 1 >= 0 && y - 1 >= 0)
		{
			other = &m_navMap[i - 1 - WIDTH];
			if (canTraverse(other->getPos(), characterLess, noClipping)) m_navMap[i].addChild(1.5f, other);
		}
		if (x + 1 < WIDTH && y + 1 < HEIGHT)
		{
			other = &m_navMap[i + 1 + WIDTH];
			if (canTraverse(other->getPos(), characterLess, noClipping)) m_navMap[i].addChild(1.5f, other);
		}
		if (x - 1 >= 0 && y + 1 < HEIGHT)
		{
			other = &m_navMap[i - 1 + WIDTH];
			if (canTraverse(other->getPos(), characterLess, noClipping)) m_navMap[i].addChild(1.5f, other);
		}
		if (x + 1 < WIDTH && y - 1 >= 0)
		{
			other = &m_navMap[i + 1 - WIDTH];
			if (canTraverse(other->getPos(), characterLess, noClipping)) m_navMap[i].addChild(1.5f, other);
		}
	}
}



pair< Map::TileType, std::deque<GameObject*>> Map::getInfoAt(pair<int, int> xy)
{
	pair<Map::TileType, std::deque<GameObject*>> out;
	
	int index = xy.first + xy.second*WIDTH;
	Map::TileType tile = (Map::TileType)m_tileMap[index];
	out.first = tile;
	auto go = m_gameObjectMap[index];
	out.second = go;

	return out;
}

bool Map::move(Character* character, pair<int, int> dest)
{
	if(dest.first < 0 || WIDTH-1 < dest.first || dest.second < 0 || HEIGHT-1 < dest.second)
		return false;
	if (canTraverse(dest))
	{
		pair<int, int> pos = character->getPos();
		navMapUpdate(pos, true);

		int from = pos.first + pos.second * WIDTH;
		int to = dest.first + dest.second * WIDTH;

		m_gameObjectMap[from].pop_front();

		character->setPos(dest);
		navMapUpdate(dest, false);
		m_gameObjectMap[to].push_front(character);
		GameLogger::log(LogType::Movement, character->getName() + " moved to (" + std::to_string(dest.first) + "," + std::to_string(dest.second) + ")");
		notify();
		return true;
	}
	else return false;
}

void Map::navMapUpdate(pair<int, int> pos, bool openNode)
{
	NavNode* current = &m_navMap[pos.first + pos.second * WIDTH];
	if (openNode) {
		for (auto el : getNeighbors(pos)) {
			m_navMap[el.second.first + el.second.second * WIDTH].addChild(el.first, &m_navMap[pos.first + pos.second * WIDTH]);
		}
	}
	else {
		for (auto el : getNeighbors(pos)) {
			m_navMap[el.second.first + el.second.second * WIDTH].removeChild(&m_navMap[pos.first + pos.second * WIDTH]);
		}
	}
}


void Map::removeGameObject(GameObject * gameObject, bool andDelete)
{
	if (gameObject == nullptr) return;
	GameObject::Type type = gameObject->getType();

	pair<int, int> xy = gameObject->getPos();
	int index = xy.first + xy.second*WIDTH;
	std::deque<GameObject*>::iterator it;
	it = find(m_gameObjectMap[index].begin(), m_gameObjectMap[index].end(), gameObject);
	if (it != m_gameObjectMap[index].end()) {
		m_gameObjectMap[index].erase(it);
	}

	if (type == GameObject::Type::Character)
	{
		vector<Character*>::iterator it;
		it = find(m_characters.begin(), m_characters.end(), static_cast<Character*>(gameObject));
		if (it != m_characters.end()) {
			m_characters.erase(it);
			if (andDelete) delete gameObject;
			return;
		}
	}
	else if (type == GameObject::Type::Chest) 
	{
		vector<Chest*>::iterator it;
		it = find(m_chests.begin(), m_chests.end(), static_cast<Chest*>(gameObject));
		if (it != m_chests.end()) {
			m_chests.erase(it);
			if (andDelete) delete gameObject;
			createStaticImage(true);
			return;
		}
	}
	else if (type == GameObject::Type::Entrance)
	{
		vector<Entrance*>::iterator it;
		it = find(m_entrances.begin(), m_entrances.end(), static_cast<Entrance*>(gameObject));
		if (it != m_entrances.end()) {
			m_entrances.erase(it);
			if (andDelete) delete gameObject;
			createStaticImage(true);
			return;
		}
	}
	else if (type == GameObject::Type::Exit)
	{
		vector<Exit*>::iterator it;
		it = find(m_exits.begin(), m_exits.end(), static_cast<Exit*>(gameObject));
		if (it != m_exits.end()) {
			m_exits.erase(it);
			if (andDelete) delete gameObject;
			createStaticImage(true);
			return;
		}
	}


}

void Map::setTileAt(pair<int, int> xy, int tile)
{
	m_tileMap[xy.first + xy.second * WIDTH] = tile;
	if (tile == 2) return;

	GameObject* go = getInfoAt(xy).second.front();
	if (go != nullptr) removeGameObject(go);
}

bool Map::validateMap()
{
	if (m_entrances.size() == 0 || m_exits.size() == 0) return false;
	for (auto start : m_entrances) {
		for (auto end : m_exits) {
			if (getPath(start->getPos(), end->getPos()).size() != 0) return true;
		}
	}
	return false;
}

bool Map::addHero(Character* hero, pair<int,int> xy)
{
	if (!canTraverse(xy)) return false;

	hero->setPos(xy);
	m_gameObjectMap[xy.first + xy.second*WIDTH].push_front(hero);
	m_hero = hero;
	return true;
}

bool Map::addCharacter(Character* opponent, pair<int, int> xy)
{
	if (!canTraverse(xy)) return false;

	opponent->setPos(xy);
	m_gameObjectMap[xy.first + xy.second*WIDTH].push_front(opponent);
	m_characters.push_back(opponent);
	return true;
}

bool Map::addChest(vector<Item*> items, pair<int,int> xy)
{
	if (!canTraverse(xy)) return false;

	Chest* chest = new Chest(xy);
	for (auto el : items) {
		chest->store(el);
	}
	m_gameObjectMap[xy.first + xy.second*WIDTH].push_front(chest);
	m_chests.push_back(chest);
	createStaticImage(true);

	return true;
}

bool Map::addEntrance(pair<int, int> xy)
{
	if (!canTraverse(xy)) return false;

	Entrance* entrance = new Entrance(xy);
	m_gameObjectMap[xy.first + xy.second*WIDTH].push_front(entrance);
	m_entrances.push_back(entrance);
	createStaticImage(true);

	return true;
}

bool Map::addExit(pair<int, int> xy)
{
	if (!canTraverse(xy)) return false;
	Exit* exit = new Exit(xy);
	m_gameObjectMap[xy.first + xy.second*WIDTH].push_front(exit);
	m_exits.push_back(exit);
	createStaticImage(true);

	return true;
}

void Map::print()
{
	int current = 0;
	pair<int, int> xy;
	GameObject* go = nullptr;
	cout << "     " << getName() << endl;
	cout << "     " << getDescription() << endl;
	cout << "     ";
	for (int i = 0; i < m_tileMap.size(); i++) {
		xy.first = i % WIDTH;
		xy.second = i / WIDTH;
		if (i % WIDTH == 0 && i != 0) cout << endl << "     ";
		current = m_tileMap[i];
		switch (current) {
		case 0:
			cout << "   ";
			break;

		case 1:
			cout << "|#|";
			break;

		case 2:
			go = getInfoAt(xy).second.front();
			if (go != nullptr) {
				switch (go->getType()) {
				case GameObject::Type::Character:
					cout << "|X|";
					break;
				case GameObject::Type::Chest:
					cout << "|$|";
					break;
				case GameObject::Type::Entrance:
					cout << "|s|";
					break;
				case GameObject::Type::Exit:
					cout << "|g|";
					break;
				default:
					break;
				}
			}
			else { cout << "|_|"; }

		default:
			break;
		}
	}
}






bool Map::canTraverse(pair<int,int> coord, bool characterLess, bool noClipping)
{
	int index = coord.first + coord.second * WIDTH;
	if (m_tileMap[index] == (int)TileType::WALL) return false;
	if (noClipping) return true;
	if (m_gameObjectMap[index].front() == nullptr) return true;
	GameObject* go = m_gameObjectMap[index].front();
	if (!characterLess && go->getType() == GameObject::Type::Character) {
		return go->isTraversable();
	}
	else if (characterLess && go->getType() == GameObject::Type::Character) return true;
	else return go->isTraversable();
}

vector<pair<float, pair<int, int>>> Map::getNeighbors(pair<int, int> pos)
{
	vector<pair<float, pair<int, int>>> out;
	pair<int, int> current;
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			current.first = pos.first + i;
			current.second = pos.second + j;
			if (i == 0 && j == 0) continue;
			if (current.first < 0 || current.first > WIDTH - 1) continue;
			if (current.second < 0 || current.second > HEIGHT - 1) continue;
			if (canTraverse(current, true)) {
				if (i == 0 || j == 0) out.push_back(std::make_pair(1, current));
				else { out.push_back(std::make_pair(1.5, current)); }
			}
		}
	}
	return out;
}

Map::NavNode::NavNode(pair<int,int> pos)
{
	NavNode::m_pos = pos;
}

vector<Map::NavNode::Child> Map::NavNode::getChildren()
{
	return m_children;
}

void Map::NavNode::addChild(float m_cost, NavNode * child)
{
	m_children.push_back(Child{ m_cost, child });
}

void Map::NavNode::removeChild(NavNode * child)
{
	std::vector<Child>::iterator it;
	it = m_children.begin();
	while (it != m_children.end()) {
		if (it->navNode == child) {
			m_children.erase(it);
			break;
		}
		it++;
	}
}

pair<int,int> Map::NavNode::getPos()
{
	return m_pos;
}


vector<pair<int, int>> Map::getRange(pair<int,int> from, int range, bool characterLess, bool noClipping)
{
	
	if (characterLess || noClipping) {
		generateNavMap(characterLess, noClipping);
	}

	//generateNavMap(characterLess, noClipping);

	vector<pair<int, int>> output;

	struct UniCostNode {
		NavNode* navNode;
		float accruedCost;
	};
	auto comparator =
		[](const UniCostNode& lhs, const UniCostNode& rhs) { return lhs.accruedCost > rhs.accruedCost; };

	priority_queue < UniCostNode, std::vector<UniCostNode>, decltype(comparator) > openList(comparator);

	set<pair<int,int>> closedList;

	openList.push(UniCostNode{ &m_navMap[from.first + from.second * WIDTH], 0 });
	closedList.insert(from);

	while (!openList.empty()) 
	{
		output.push_back(openList.top().navNode->getPos());

		UniCostNode child;

		for (auto& el : openList.top().navNode->getChildren()) {
			child = UniCostNode{ el.navNode, el.m_cost + openList.top().accruedCost};
			if (closedList.insert(child.navNode->getPos()).second) {
				if (range == -1 || (int)child.accruedCost <= range) 
				{
					openList.push(child);
				}
			}
		}
		openList.pop();

	}

	if (characterLess || noClipping) {
		generateNavMap();
	}


	output.erase(output.begin());
	return output;
}

list<pair<int, int>> Map::getPath(pair<int, int> from, pair<int, int> dest, int range, bool characterLess)
{
	if (characterLess) {
		generateNavMap(true);
	}

	//generateNavMap(characterLess);

	list<pair<int, int>> output;

	bool valid = false;

	struct AStarNode {
		NavNode* navNode;
		double accruedCost;
		AStarNode* parent;
		double heuristic;
	};

	auto comparator =
		[](const AStarNode* lhs, const AStarNode* rhs) { return (lhs->accruedCost + lhs->heuristic) > (rhs->accruedCost + rhs->heuristic); };

	priority_queue < AStarNode*, std::vector<AStarNode*>, decltype(comparator) > openList(comparator);

	set<pair<int, int>> closedList;
	
	auto heuristicFunction = 
		[](pair<int, int> from, pair<int,int> dest) {
		double deltaX, deltaY, diag;
		deltaX = std::abs(from.first - dest.first);
		deltaY = std::abs(from.second - dest.second);
		if (deltaY == 0) return deltaX;
		if (deltaX == 0) return deltaY;
		diag = std::min(deltaX, deltaY);
		return diag * 1.5 + std::max(deltaX, deltaY) - diag;
	};
	
	AStarNode* current = new AStarNode{ &m_navMap[from.first + from.second * WIDTH], 0, nullptr, heuristicFunction(from,dest) };
	openList.push(current);
	closedList.insert(current->navNode->getPos());

	AStarNode child;
	vector<AStarNode*> toDelete;

	while (openList.size() != 0 && openList.top()->navNode->getPos() != dest)
	{
		current = openList.top();
		for (auto& el : current->navNode->getChildren()) {

			child = AStarNode{ el.navNode, el.m_cost + current->accruedCost, current, heuristicFunction(el.navNode->getPos(),dest) };

			if (closedList.insert(child.navNode->getPos()).second) {
				openList.push(new AStarNode(child));
			}
		}
		toDelete.push_back(openList.top());
		openList.pop();
	}
	
	if (openList.size() != 0) {
		valid = true;
		current = openList.top();
		toDelete.push_back(openList.top());
		openList.pop();

		while (current->parent != nullptr) {
			if (range == -1 || (int)current->accruedCost <= range) output.push_front(current->navNode->getPos());
			current = current->parent;
		}
	}
	for (auto el : toDelete) {
		delete el;
	}
	while (openList.size() != 0) {
		delete openList.top();
		openList.pop();
	}

	if (characterLess) {
		generateNavMap();
	}

	if (valid) return output;
	else return list<pair<int, int>>{};
}

bool Map::checkLineOfSight(pair<int,int> from, pair<int,int> to)
{
	bool valid = true;
	int x1, x0, y1, y0;
	x1 = to.first;
	x0 = from.first;
	y1 = to.second;
	y0 = from.second;

	int dy = y0 - y1;
	int dx = x0 - x1;
	int stepx, stepy;

	if (dy < 0) { dy = -dy;  stepy = -1; }
	else { stepy = 1; }
	if (dx < 0) { dx = -dx;  stepx = -1; }
	else { stepx = 1; }
	dy <<= 1;        // dy is now 2*dy
	dx <<= 1;        // dx is now 2*dx

	int index = x1 + y1*WIDTH;
	if (this->m_tileMap.at(index) == 1)
		valid = false;
	if (dx > dy)
	{
		int fraction = dy - (dx >> 1);  // same as 2*dy - dx
		while (x1 != x0)
		{
			if (fraction >= 0)
			{
				y1 += stepy;
				fraction -= dx;          // same as fraction -= 2*dx
			}
			x1 += stepx;
			fraction += dy;              // same as fraction -= 2*dy
			int index = x1 + y1*WIDTH;
			if (this->m_tileMap.at(index) == 1)
				valid = false;
		}
	}
	else {
		int fraction = dx - (dy >> 1);
		while (y1 != y0) {
			if (fraction >= 0) {
				x1 += stepx;
				fraction -= dy;
			}
			y1 += stepy;
			fraction += dx;
			int index = x1 + y1*WIDTH;
			if (this->m_tileMap.at(index) == 1)
				valid = false;
		}
	}
	return valid;
}

