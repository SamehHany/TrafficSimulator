#ifndef COLLISIONGRID_H
#define COLLISIONGRID_H

class CollisionGrid;

#include "CarList.h"

/*struct CollisionNode
{
	CarList list;
	//Car *crash(Car *car);
};*/

struct CollisionGrid
{
		CarList **lists;
		int N;
		int M;
		//bool (*crashfunc)(Car*, Car*);
		CollisionGrid();
		CollisionGrid(int N, int M);
		
		~CollisionGrid();
		
		//void crash(int i, int j);
		
		//void setCrash( bool (*crashfunc)(Car*, Car*) );
};

#endif // COLLISIONGRID_H
