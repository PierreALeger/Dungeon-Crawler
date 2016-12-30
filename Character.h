
#pragma once


#include "Equipable.h"
#include "Weapon.h"
#include "Inventory.h"
#include "Helmet.h"
#include "Belt.h"
#include "Boots.h"
#include "Ring.h"
#include "Shield.h"
#include "GameObject.h"
#include "ItemFactory.h"
#include "Observable.h"
#include "GameLogger.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

class Map;

/**
Character class for the player and enemy instances.

Character class used to generate character game objects based on the d20 fighter class. Includes most attributes modifiers and roll modifers required
to implement the rule subset as described in the accompanying document.

Rules implemented :

- Random stats and level 1 character creation
- Custom stats and custom level character creation
- isDead querying
- Anything hit point related (hit, heal, get current hit points or max hit points)
- Level up character
- Equip/Unequip items or weapons
- MoveSpeed
- Attack bonus computed from stats and equipment
- Base attack bonus for each attack/round
- Armor class
- Damage bonus computed from stats and equipment
- Experience
- Character health can not fall below 0. At 0, the character is dead.
*/

class Character : public GameObject , public Observable
{
public:

	enum class Stats {
		STRENGTH = 0,
		DEXTERITY,
		CONSTITUTION,
		INTELLIGENCE,
		WISDOM,
		CHARISMA
	};

	enum class FighterType {
		BULLY = 0,
		NIMBLE,
		TANK,
		BOSS
	};

	/**
	Constructor.
	Overloaded constructor takes 7 int arguments. First 6 for stats and the last for the level.
	@param str Strength attribute
	@param dex Dexterity attribute
	@param con Constitution attribute
	@param intel Intelligence attribute
	@param wis Wisom attribute
	@param charis Charisma attribute
	@param level Current level
	*/
	Character(int id, string name, string description, int str, int dex, int con, int intel, int wis, int charis, int level);

	/**
	Constructor.
	Default constructor calls the overloaded constructor with random stats and level set to 1
	*/
	Character();

	/**
	Deconstructor.
	Deletes the inventory of type ItemContainer and takes care of dangling pointers.
	*/
	~Character();

	/**
	Verifies that the stats are within the 3-18 range
	@return True if character base stats are valid
	*/
	bool validateNewCharacter();


	/**
	Deals damage to the character.
	Removes hit points from the character. If hit points fall below 0, the dead flag is set to true.
	@param damage Integer damage dealt to character
	*/
	void hit(int dammage);

	/**
	Heals character hit points.
	Increase hit points up to character's maximum health. Healing a character at 0 hit points will set the dead flag to false.
	@param amount Integer hit points to heal
	*/
	void heal(int amount);

	/**
	Computes and returns characters current hit points.
	@return Current hit points
	*/
	int getCurrentHitPoints();

	/**
	Computes and returns character's maximum hit points.
	@return Maximum hit points
	*/
	int getMaxHitPoints();

	/**
	Returns character's current level.
	@return Integer value
	*/
	int getLevel();

	/**
	Flag set to true if character's current health is 0.
	This flag is manipulated by the hit() and heal() methods.
	@return True if character has 0 health
	*/
	bool isDead();

	/**
	Returns character move speed in feet.
	@return Integer move speed in feet for one move action
	*/
	int getMoveSpeed();

	/**
	Equips an item.
	If an item is already equipped at the same equipment slot, it will unequip it by calling the unequipItem() method and then equip the new item.
	This method applies the appropriate modifiers to the modifiers[] array. Is is assumed that the character equips items stored in his inventory.
	@param item A pointer to an item to equip
	*/
	void equip(const Equipable* item, bool bypassTransfer = false);

	/**
	Unequips the item at specified equipment slot.
	Can be called by itself to unequip an item in a given slot. Is usually called implicitely by the equip() method.
	Removes modifiers applied by this item from the modifiers[] array.
	@param slot The equipment slot as an enum of type ItemType
	*/
	void unequip(const Equipable *item, bool bypassTransfer = false);

	/**
	Removes and item from the backpack.
	@param item A pointer to an item in the backpack
	*/
	void drop(Item* item);

	/**
	Getter.
	Returns character's stats. Indexes mirror the enum Stats.
	@return A vector of 6 integers for the character's core attributes (with modifiers).
	*/
	vector<int> getStats();

	/**
	Getter.
	Returns a pointer to the character's equipped weapon. The Weapon pointed to should be in the player's inventory.
	@return A pointer to a type Weapon of the character's current equipped weapon.
	*/
	Weapon* getWeapon();

	/**
	Getter.
	Computes and returns character's current armor class with modifiers.
	@return Armor class as an integer value
	*/
	int getAC();

	/**
	Getter.
	Returns current experience.
	@return Current experience as an integer.
	*/
	inline int getExperience() { return m_experience; }

	/**
	Increase current experience.
	Will check experienceTable to see if character needs to level up. Once the character reachers level 20, he will continue to accrue experience
	but will no longer gain any levels.
	@param exp Integer amount to add to current experience
	*/
	void giveExperience(int exp);

	/**
	Getter.
	Returns attack bonus from modifiers only. Base attack bonus not included since it varies depending on the number of attacks per turn.
	@return Attack bonus from modifiers as an int
	*/
	int getAttackBonus();

	/**
	Getter.
	Returns character'a base attack bonus as a vector of integers. If the character's level allows him to have more than one attack per round,
	each attack's BAB will have its own index. Must add this to the "global" attack bonus with the getAttackBonus() method to get the total attack bonus.
	@return A vector of integers, each index is one attack
	*/
	vector<int> getBAB();

	/**
	Getter.
	Computes and returns the character's damage bonus.
	@return Damage bonus as an integer
	*/
	int getDamageBonus();

	/**
	Increase current level by 1.
	Is called implicetely if needed when giving experience to the player. Calls setLevel() with currentLevel+1 passed as actual parameter.
	*/
	void levelUp();

	/**
	Setter.
	Sets current character level, increases hit points and base attack bonus accordingly. Will not allow level to increase above 20.
	Will check players current experience and adjust it if needed. Trying to set level beyond 20 will level up the character up to 20 and then stop.
	@param level The character's new level
	*/
	void setLevel(int m_level);
	
	/** 
	Add item to inventory.
	@param item  Add this item to the inventory
	*/
	void loot(Item* item);

	/**
	Returns damage dealt to an opponent
	@param AC Opponent's armor class
	*/
	int attack(int AC);

	/**
	Returns attack range (depends on equipped weapon)
	*/
	int getAttackRange();

	/**
	Returns initiave roll bonus
	*/
	int getInitBonus();

	/**
	Transfers all equipment to the backpack
	*/
	void unequipAll();

	// Various setters and getters, self explanatory
	string getName() { return m_name; }
	string getDescription() { return m_description; }
	bool isHostile() { return m_hostile; }
	void setName(string name) { m_name = name; }
	void setDescription(string description) { m_description = description; }
	void setHostile(bool isHostile) {
		m_hostile = isHostile;
	}

	Inventory* getBackpack() { return &m_backpack; }
	Inventory* getEquipment() { return &m_equipment; }
	inline Weapon* getWeapon() const { return m_weapon; }
	inline Helmet* getHelmet() const { return m_helmet; }
	inline Armor* getArmor() const { return m_armor; }
	inline Boots* getBoots() const { return m_boots; }
	inline Ring* getRing() const { return m_ring; }
	inline Belt* getBelt() const { return m_belt; }
	inline Shield* getShield() const { return m_shield; }
	inline int getExperienceNextLevel() { return experienceTable[m_level]; }
	inline FighterType getFighterType() { return m_fighterType; }
	std::string getFighterTypeString();

	// Outputs information on the current character to the console
	void print();

	//! Methods used by the CharacterBuilder class
	void inline setStrenght(int str) { m_stats[0] = str; }
	void inline setDexterity(int dex) { m_stats[1] = dex; }
	void inline setConstitution(int con) { m_stats[2] = con; }
	void inline setIntelligence(int intel) { m_stats[3] = intel; }
	void inline setWisdom(int wis) { m_stats[4] = wis; }
	void inline setCharisma(int cha) { m_stats[5] = cha; }

	void restoreFromSave(int hp, int exp);
	
	friend class MakerCharacter;
	friend class CharacterGenerator;
	void inline setFighterType(FighterType type) { m_fighterType = type; }

public:

	//! Returns a pointer to the character's inventory
	const Inventory* getInventory()
	{
		return &m_backpack;
	}
	
private:

	//! True if character is hostile
	bool m_hostile;

	//! Name of character
	string m_name;

	//! Description of character
	string m_description;
	
	//! Max hit points without modifiers
	int m_maxHitPoints;

	//! Current hit points without modifiers
	int m_currentHitPoints;

	//! For our implementation of the rules, all characters have a base move speed of 30.
	int m_moveSpeed;

	//! Stores base armor class with no modifiers
	int m_baseArmorClass;

	//! False if character's HP > 0, true otherwise
	bool m_dead;

	//! Character's current level
	int m_level;
	
	//! Vector for the character's core attributes
	vector<int> m_stats;
	
	//! The modifiers arrays is heavily used in this class
	//! All modifiers to stats, AC, attack bonus and damage from items/weapons are stored here
	//! The total modifier for a given attribute is determined by (modifier from modifed stat) + (modifier)
	
	/**
	Array of integer modifers.
	This array contains all modifiers for the core attribuites as well as attack, AC and damage bonuses. Values indexes mirror the Modifiers enum indexes.
	*/
	int m_modifiers[9];

	//! Each index in the base attack bonus array is an attack/round
	vector<int> m_bab;

	//! Current character experience
	int m_experience;

	//! Experience table used to determine when the player levels up
	int experienceTable[21] = {
		0,
		2000,
		5000,
		9000,
		15000,
		23000,
		35000,
		51000,
		75000,
		105000,
		155000,
		220000,
		315000,
		445000,
		635000,
		890000,
		1300000,
		1800000,
		2550000,
		3600000,
		0
	};

	//! Helper method for computing the bonus associated with a given stat
	int bonusFromStat(int);

	/**
	Pointer to character's inventory.
	*/
	Inventory m_backpack;
	Inventory m_equipment;

	// Pointers to character's equipped weapon
	Weapon* m_weapon;
	Helmet* m_helmet;
	Belt* m_belt;
	Boots* m_boots;
	Ring* m_ring;
	Shield* m_shield;
	Armor* m_armor;

	FighterType m_fighterType;
};
