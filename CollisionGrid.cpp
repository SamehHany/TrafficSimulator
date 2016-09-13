#include "CollisionGrid.h"

/*Car *CollisionNode::crash(Car *car)
{
	
	return 0;
}*/

/*void CollisionGrid::crash(int i, int j)
{
	if (crashfunc == 0)
		return;
	CarNode *first = lists[i][j].getRoot();
	if (first == 0)
		return;
	CarNode *second;
	while (first->next != 0) {
		second = first->next;
		while (second != 0) {
			if ( (*crashfunc)(first->car, second->car) ) {
				first->car->crash();
				second->car->crash();
			}
			second = second->next;
		}
		first = first->next;
	}
	// Commented Block
	while (n != 0) {
		if ( (*crashfunc)(car, n->car) ) {
			car->crash();
			n->car->crash();
		}
		n = n->next;
	}
	//End of Block
}*/

CollisionGrid::CollisionGrid()
{
	lists = 0;
	N = 0;
	M = 0;
	//this->crashfunc = crashfunc;
}

CollisionGrid::CollisionGrid(int N, int M)
{
	this->N = N;
	this->M = M;
	lists = new CarList* [N];
	for (int i = 0; i < N; i++)
		lists[i] = new CarList[M];
	//this->crashfunc = crashfunc;
}

CollisionGrid::~CollisionGrid()
{
	for(int i = 0; i < N; i++)
		delete [] lists[i];
	delete [] lists;
}

/*void CollisionGrid::setCrash( bool (*crashfunc)(Car*, Car*) ) {
	this->crashfunc = crashfunc;
}*/
