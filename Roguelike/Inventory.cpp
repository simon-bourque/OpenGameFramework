#include "Inventory.h"



Inventory::Inventory(): m_selected(nullptr) {
}

void Inventory::pickUpItem(Item* item) {
	if (m_currentSlots < m_maxSlots) {
		inventoryArray.push_back(item);
	}
	else {
		//TODO INVENTORY IS FULL
	}
}

void Inventory::addItem(uint32 inventoryIndex) {
	inventoryArray[inventoryIndex] = m_selected;
	m_selected = nullptr;
}

void Inventory::selectItem(uint32 inventoryIndex) {
	m_selected = inventoryArray.at(inventoryIndex);
	inventoryArray[inventoryIndex] = nullptr;
}

void Inventory::removeItem() {
	m_selected = nullptr;
}

void Inventory::swapItem(uint32 inventoryIndex) {
	Item* temp = inventoryArray.at(inventoryIndex);
	inventoryArray[inventoryIndex] = m_selected;
	m_selected = temp;
}

Inventory::~Inventory() {
}
