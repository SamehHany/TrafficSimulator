#ifndef CARLIST_H
#define CARLIST_H

class CarList;

#include "Car.h"

struct CarNode
{
	Car *car;
	CarNode *next;
	CarNode();
};

class CarList
{
	private:
		CarNode *root;
		CarNode *last;
		size_t list_len;
	public:
		CarList();
		CarList(Car *car);
		void append(Car *car);
		Car *get(int id);
		CarNode *getNode(size_t index);
		CarNode *getRoot();
		Car *getCarAt(size_t index);
		void remove(int id);
		size_t length();
};

#endif // CARLIST_H
