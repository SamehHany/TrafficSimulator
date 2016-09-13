#include "CarList.h"

CarNode::CarNode()
{
	car = 0;
	next = 0;
}

CarList::CarList()
{
	root = 0;
	last = 0;
	list_len = 0;
}

CarList::CarList(Car *car)
{
	root = new CarNode;
	root->car = car;
	last = root;
	list_len = 1;
}

void CarList::append(Car *car)
{
	if (last != 0) {
		last->next = new CarNode;
		last = last->next;
		last->car = car;
	} else {
		root = new CarNode;
		root->car = car;
		last = root;
	}
	list_len++;
}

Car *CarList::get(int id)
{
	CarNode *n = root;
	
	while (n != 0) {
		if (n->car->getID() == id)
			return n->car;
		n = n->next;
	}
	
	return 0;
}

CarNode *CarList::getNode(size_t index)
{
	if (index >= list_len)
		return 0;
	if (index == 0)
		return root;
	CarNode *n = root;
	size_t i = 0;
	while (i++ < index) {
		n = n->next;
	}
	return n;
}

CarNode *CarList::getRoot() {
	return root;
}

Car *CarList::getCarAt(size_t index)
{
	if (index >= list_len)
		return 0;
	if (index == 0)
		return root->car;
	CarNode *n = root;
	size_t i = 0;
	while (i++ < index) {
		n = n->next;
	}
	return n->car;
}

void CarList::remove(int id)
{
	CarNode *previous = root;
	CarNode *current = root;
	CarNode *tmp;
	if (current != 0) {
		if (current->car->getID() == id) {
			tmp = current;
			if (last == root)
				last = current->next;
			root = current->next;
			delete [] tmp;
			list_len--;
			return;
		}
		
		current = current->next;
	}
	while (current != 0) {
		if (current->car->getID() == id) {
			if (last == current)
				last = previous;
			tmp = current;
			previous->next = current->next;
			if (current == root)
				root = root->next;
			delete [] tmp;
			list_len--;
			return;
		}
		previous = current;
		current = current->next;
	}
}

size_t CarList::length() {
	return list_len;
}
