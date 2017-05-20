#pragma once
#include "Core/Core.h"

enum class itemId {
	//Armor
	AR_CHEST_BRONZE, AR_CHEST_COPPER, AR_CHEST_GUARD, AR_CHEST_ORCISH, AR_CHEST_BRASS, AR_CHEST_MITHRIL, AR_CHEST_GOLD, AR_CHEST_IRON, AR_CHEST_OBBY, AR_CHEST_DIVINE, AR_CHEST_SCALE, AR_CHEST_DRAGON,
	AR_BOOT_COPPER, AR_BOOTS_MITHRIL, AR_BOOTS_BRONZE, AR_BOOTS_SCALE, AR_BOOTS_ORCISH, AR_BOOTS_BRASS, AR_BOOTS_IRON, AR_BOOTS_GOLD, AR_BOOTS_DRAGON, AR_BOOTS_DIVINE,
	AR_HAT_PAN, AR_HAT_MITHRIL, AR_HAT_GUARD, AR_HAT_IRON, AR_HAT_GOLD, AR_HAT_ORCISH, AR_HAT_COPPER, AR_HAT_BRASS, AR_HAT_BRONZE, AR_HAT_FEDORA, AR_HAT_MAGI, AR_HAT_POPE, AR_HAT_RING, AR_HAT_CROWN1, AR_HAT_CROWN2,
	AR_GLOVE_COPPER, AR_GLOVE_BRASS, AR_GLOVE_BRONZE, AR_GLOVE_DIVINE,
	//Weapons
	WE_DAGGER_DIAMOND, WE_DAGGER_IRON, WE_DAGGER_BRASS, WE_DAGGER_DIVINE, WE_DAGGER_DRAGON, WE_DAGGER_MITHRIL, WE_DAGGER_GOLD, WE_LANCE_DIAMOND, WE_SWORD, DRAGON, WE_SWORD_DIVINE, WE_DAGGER, TOOTH,
	//Amulet Accessory
	AC_AMULET_KNOB, AC_AMULET_SILVER, AC_AMULET_TIGER, AC_AMULET, RUBY, AC_AMULET_GOLD, AC_AMULET_EMERALD, AC_AMULET_SAPPHIRE, AC_AMULET_AQUA, AC_AMULET_DIVNIE,
	//Potions
	POT_HEALTH, POT_STR, POT_POISON, POT_SPEED, POT_AGI, POT_FIRE_RES, POT_GHOST, POT_WEAK, POT_CRIPPLE, POT_ALL_RES, POT_VULN
};

enum class effect {
	STR,SPEED,HEALTH,POISON,AGI,FIRE_RES,GHOST,WEAK,CRIPPLE,ALL_RESS,VULN
};

class Item
{
public:
	Item();
	~Item();

private:
	itemId id;
	uint32 damage;
	uint32 defense;
	uint32 agility;
	effect itemEffect;
	string name;
	string description;

};
