#pragma once
#include <vector>
#include "Core/Core.h"
#include "Item.h"

class Inventory
{
public:
	Inventory();
	virtual ~Inventory();

	void addItem(uint32);
	void pickUpItem(Item*);
	void selectItem(uint32);
	void removeItem();
	void swapItem(uint32);

private:
	uint32* m_maxSlots;
	uint32* m_currentSlots;
	uint32* m_money;

	Item* m_selected;

	Item* m_hat;
	Item* m_armor;
	Item* m_glove;
	Item* m_boots;
	Item* m_weapon;
	Item* m_amulet;

	static std::vector<Item*> inventoryArray;
};

