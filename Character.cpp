#include <math.h>
#include "Character.h"
#include "Dice.h"
#include "Equipable.h"
#include "Inventory.h"
#include <vector>
using std::vector;


Character::Character() : Character(
	100, "", "",
	Dice::roll("3d6"),
	Dice::roll("3d6"),
	Dice::roll("3d6"),
	Dice::roll("3d6"),
	Dice::roll("3d6"),
	Dice::roll("3d6"), 1)
{
	// Left intentionaly blank
}

Character::Character(int id, string name, string description, int str, int dex, int con, int intel, int wis, int charis, int level) : GameObject(pair<int, int>(0, 0), id, false , GameObject::Type::Character)
{
	m_stats.push_back(str);
	m_stats.push_back(dex);
	m_stats.push_back(con);
	m_stats.push_back(intel);
	m_stats.push_back(wis);
	m_stats.push_back(charis);
	m_stats.shrink_to_fit();

	m_name = name;
	m_description = description;

	for (int i = 0; i < 9; i++) {
		m_modifiers[i] = 0;
	}
	m_maxHitPoints = 0;
	m_currentHitPoints = m_maxHitPoints;
	m_experience = 0;
	m_level = 0;
	m_baseArmorClass = 10;
	setLevel(level);

	
	m_dead = false;

	m_backpack = Inventory();
	m_equipment = Inventory();

	m_moveSpeed = 6;

	m_helmet = nullptr;
	m_belt = nullptr;
	m_boots = nullptr;
	m_ring = nullptr;
	m_shield = nullptr;
	m_armor = nullptr;
	m_weapon = nullptr;
}


bool Character::validateNewCharacter()
{
	for (int i = 0; i < 6; i++) {
		if (m_stats[i] < 3 || 18 < m_stats[i]) return false;
	}
	return true;
}

// Calls setLevel() with desired level as current level + 1
void Character::levelUp() {
	setLevel(m_level + 1);
	notify();
}

// The setLevel() method increases the maximum hit point, the current hit points and
// populates the base attack bonus vector accordingly. Each entry in the BAB vector
// is an attack per round
void Character::setLevel(int level)
{
	if (m_level == level || m_level == 20) return;
	int hpIncrease = 0;
	do {
		m_level++;
		if (m_level == 1) hpIncrease += 10;
		else hpIncrease += Dice::roll("1d10");
		if (m_level % 5 == 1) {
			m_bab.push_back(0);
		}
		int size = m_bab.size();
		for (int j = 0; j < size; j++) 
			m_bab[j] += 1;
	} while (m_level < level && m_level < 20);
	m_maxHitPoints += hpIncrease;
	m_currentHitPoints += hpIncrease;
	if (m_experience < experienceTable[m_level - 1]) m_experience = experienceTable[m_level - 1];

	
}

std::string Character::getFighterTypeString()
{
	switch (m_fighterType) {
	case FighterType::BOSS:
		return "Boss";
	case FighterType::BULLY:
		return "Bully";
	case FighterType::NIMBLE:
		return "Nimble";
	case FighterType::TANK:
		return "Tank";
	default :
		return "error";
	}
}

void Character::print()
{
	cout << "=========================================================================\n";
	cout << "==================          Character Viewer         ====================\n";
	cout << "=========================================================================\n";
	cout << "Name : " << getName() << endl;
	cout << "Description : " << getDescription() << endl;
	cout << "Id : " << getId() << endl;
	cout << "Fighter type : " << getFighterTypeString() << endl;
	cout << "Level : " << getLevel() << endl;
	cout << "Experience :" << getExperience() << "/" << getExperienceNextLevel() << endl;
	cout << endl;
	cout << "Strenght : " << getStats()[0] << endl;
	cout << "Dexterity : " << getStats()[1] << endl;
	cout << "Constitution : " << getStats()[2] << endl;
	cout << "Intelligence : " << getStats()[3] << endl;
	cout << "Wisdom : " << getStats()[4] << endl;
	cout << "Charisma : " << getStats()[5] << endl;
	cout << endl;
	cout << "Health : " << getCurrentHitPoints() << "/" << getMaxHitPoints() << endl;
	cout << "Armor class : " << getAC() << endl;
	cout << endl;
	cout << "Weapon damage dice : " << (getWeapon() == nullptr ? "No weapon equipped" : getWeapon()->getDamageDie()) << endl;
	cout << "Damage bonus : " << getDamageBonus() << endl;
	cout << "Attack bonus (per attack): ";
	for (auto el : getBAB()) {
		cout << el + getAttackBonus() << " ";
	}
	cout << endl << endl;
	cout << "Equipment : \n";
	m_equipment.print();
	cout << "---------------------------\n";
	cout << "Backpack : \n";
	m_backpack.print();
	cout << endl;
	cout << "=========================================================================\n\n";
}

void Character::loot(Item *item)
{
	m_backpack.store(item);
	notify();
}
int Character::attack(int AC)
{
	int roll;
	int damage = 0;
	string damageDie;
	if (m_weapon == nullptr) damageDie = "1d3";
	else damageDie = m_weapon->getDamageDie();
	bool crit = false;
	bool hit = false;
	for (auto el : m_bab) {
		GameLogger::log(LogType::Attack, m_name + " Attacks");
		roll = Dice::roll("1d20");
		while (roll == 20) {
			hit = true;
			GameLogger::log(LogType::Attack, m_name + " Critical Hit");
			crit = true;
			damage += Dice::roll(damageDie);
			damage += getDamageBonus();
			roll = Dice::roll("1d20");
			if (roll == 20) continue;
			if (roll + el + getAttackBonus() >= AC) {
				GameLogger::log(LogType::Attack, m_name + " Critical Hit Confirmed on AC " + std::to_string(AC) + " with " + std::to_string(el + getAttackBonus()) + " modifiers.");
				damage += Dice::roll(damageDie);
				damage += getDamageBonus();
			}
		}
		if (!crit && roll + el + getAttackBonus() >= AC) {
			hit = true;
			GameLogger::log(LogType::Attack, m_name + " Hit on AC " + std::to_string(AC) + " with " + std::to_string(el + getAttackBonus()) + " modifiers.");
			damage += Dice::roll(damageDie);
			GameLogger::log(LogType::Attack, "Damage bonus " + std::to_string(getDamageBonus()));
			damage += getDamageBonus();
		}
		GameLogger::log(LogType::Attack, m_name + " Miss on AC " + std::to_string(AC) + " with " + std::to_string(el + getAttackBonus()) + " modifiers.");
	}
	if (hit && damage < 1) damage = 1;
	damage = damage < 0 ? 0 : damage;
	GameLogger::log(LogType::Attack, m_name + " Deals " + std::to_string(damage));
	return damage;
}
int Character::getAttackRange()
{
	if (m_weapon == nullptr) return 1;
	else return m_weapon->getRange();
}
int Character::getInitBonus()
{
	return bonusFromStat(getStats()[(int)Stats::DEXTERITY]);
}
void Character::unequipAll()
{
	for (auto el : m_equipment.takeAll()) {
		m_backpack.store(el);
	}
}
void Character::restoreFromSave(int hp, int exp)
{
	m_maxHitPoints = hp;
	m_currentHitPoints = m_maxHitPoints;
	m_experience = exp;
}


// Helper method to compute the bonus from stats
int Character::bonusFromStat(int stat)
{
	return (stat / 2) - 5;
}


// Decreases the chacacters down to zero at most, flags as dead if hp reach 0
void Character::hit(int damage)
{
	if (damage >= getCurrentHitPoints()) {
		GameLogger::log(LogType::Attack, m_name + " has died.");
		m_currentHitPoints = 0;
		m_dead = true;
		setTraversable(true);
		unequipAll();
		m_name = "Loot drop";
		m_description = "The remains of fallen foes";
		m_level = 0;
		m_maxHitPoints = 0;
		for (int i = 0; i < 6; i++) {
			m_stats[i] = 0;
		}
	}
	else {
		m_currentHitPoints -= damage;
	}
	notify();
}

// Increase hp up to the maximum health, unflags as dead if hp were 0
void Character::heal(int amount) {
	if (m_dead) 
	{
		m_currentHitPoints += amount;
		m_dead = false;
	}
	else 
	{
		if (getCurrentHitPoints() + amount > getMaxHitPoints()) {
			m_currentHitPoints = m_maxHitPoints;
		}
		else {
			m_currentHitPoints += amount;
		}
	}
	notify();
}

// Note that the current hit points visible from inside this class to not reflect the modifiers or bonuses
// The getCurrentHitPoints method in the public interface allows outside classes to see what the "real" total hit points are
int Character::getCurrentHitPoints() {
	if (m_dead) return 0;
	return
		m_currentHitPoints 
		+ m_level * bonusFromStat
		(
			m_stats[(int)Stats::CONSTITUTION]
			+ m_modifiers[(int)Enchantment::Type::CONSTITUTION]
		);
}

// Returns max hit points
int Character::getMaxHitPoints() {
	if (m_dead) return 0;
	return 
		m_maxHitPoints 
		+ m_level * bonusFromStat
		(
			m_stats[(int)Stats::CONSTITUTION]
			+ m_modifiers[(int)Enchantment::Type::CONSTITUTION]
		);
}

// Returns current level
int Character::getLevel()
{
	return m_level;
}

// Returns true if character has 0 hp
bool Character::isDead() {
	return m_dead;
}

int Character::getMoveSpeed()
{
	return m_moveSpeed;
}

// Returns base attack bonus as a vector of int, each entry is an attack per round
vector<int> Character::getBAB() {
	return m_bab;
}

void Character::giveExperience(int exp)
{
	m_experience += exp;

	while (m_experience >= experienceTable[getLevel()]) {
		if (m_level == 20) return;
		levelUp();
	}
	
}

// Returns attack bonus from modifiers (this should be added to the appropriate BAB bonus)
// to determine the total attack bonus
int Character::getAttackBonus() 
{
	return
		bonusFromStat
		(
		(m_weapon != nullptr && m_weapon->getRange() > 1) ?
			getStats()[(int)Stats::DEXTERITY] :
			getStats()[(int)Stats::STRENGTH]
		) 
		+ m_modifiers[(int)Enchantment::Type::ATTACK];
}

// Returns armor class
int Character::getAC() {
	return
		m_baseArmorClass 
		+ bonusFromStat
		(
			m_stats[(int)Stats::DEXTERITY] +
			m_modifiers[(int)Enchantment::Type::DEXTERITY]
		)
		+ m_modifiers[(int)Enchantment::Type::ARMOR_CLASS]
		+ ((m_shield != nullptr) ? m_shield->getArmorClass() : 0)
		+ ((m_armor != nullptr) ? m_armor->getArmorClass() : 0)
		+ ((m_helmet != nullptr) ? m_helmet->getArmorClass() : 0)
		+ ((m_belt != nullptr) ? m_belt->getArmorClass() : 0)
		+ ((m_boots != nullptr) ? m_boots->getArmorClass() : 0);
}


// Returns damage bonus
int Character::getDamageBonus() {
	int out = 0;
	if (m_weapon == nullptr || m_weapon->getRange() == 1) out += bonusFromStat(getStats()[(int)Stats::STRENGTH]);
	out += m_modifiers[(int)Enchantment::Type::DAMAGE];
	return out;
}


// Infers from the item type where to equip it
// Unequips current item in the slot with the aid of a helper method
// Applies changes to modifier arrays as needed
void Character::equip(const Equipable* item, bool bypassTransfer)
{
	switch(item->getType())
	{
		case Item::Type::Armor:
			unequip(m_armor);
			m_armor = const_cast<Armor*>(static_cast<const Armor*>(item));
			break;
		case Item::Type::Belt:
			unequip(m_belt);
			m_belt = const_cast<Belt*>(static_cast<const Belt*>(item));
			break;
		case Item::Type::Boots:
			unequip(m_boots);
			m_boots = const_cast<Boots*>(static_cast<const Boots*>(item));
			break;
		case Item::Type::Helmet:
			unequip(m_helmet);
			m_helmet = const_cast<Helmet*>(static_cast<const Helmet*>(item));
			break;
		case Item::Type::Ring:
			unequip(m_ring);
			m_ring = const_cast<Ring*>(static_cast<const Ring*>(item));
			break;
		case Item::Type::Shield:
			unequip(m_shield);
			m_shield = const_cast<Shield*>(static_cast<const Shield*>(item));
			break;
		case Item::Type::Weapon:
			unequip(m_weapon);
			m_weapon = const_cast<Weapon*>(static_cast<const Weapon*>(item));
			break;
	}
	for (auto& el : item->getEnchantments()) {
		m_modifiers[(int)el.getType()] += el.getBonus();
	}
	if (bypassTransfer) {
		m_equipment.store(const_cast<Equipable*>(item));
	}
	else {
		m_equipment.store(m_backpack.take(item->getId()));
	}

	notify();
}


// unequipItem and unequipWeapon work very similarly
// Before emptying the proper equipment slot, it removes the current
// bonuses applied to the modifier arrays
void Character::unequip(const Equipable* item, bool bypassTransfer)
{
	if (item == nullptr) return;
	for (auto& el : item->getEnchantments()) {
		m_modifiers[(int)el.getType()] -= el.getBonus();
	}
	if (bypassTransfer) {
		m_equipment.remove(item->getId());
	}
	else {
		m_backpack.store(m_equipment.take(item->getId()));
	}

	switch(item->getType())
	{
		case Item::Type::Armor:
			m_armor = nullptr;
			break;
		case Item::Type::Belt:
			m_belt = nullptr;
			break;
		case Item::Type::Boots:
			m_boots = nullptr;
			break;
		case Item::Type::Helmet:
			m_helmet = nullptr;
			break;
		case Item::Type::Ring:
			m_ring = nullptr;
			break;
		case Item::Type::Shield:
			m_shield = nullptr;
			break;
		case Item::Type::Weapon:
			m_weapon = nullptr;
			break;
	}

	notify();
}

void Character::drop(Item * item)
{
	m_backpack.remove(item->getId());
}

vector<int> Character::getStats()
{
	vector<int> out;
	for (int i = 0; i < 6; i++) {
		out.push_back(m_stats[i] + m_modifiers[i]);
	}
	return out;
}

// Return equipped weapon
Weapon* Character::getWeapon()
{
	return m_weapon;
}


Character::~Character()
{
	m_weapon = nullptr;
}
