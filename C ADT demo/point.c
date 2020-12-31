#include "point.h"
#include "point.h"
#include "defs.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Point_
{
	int		dim;
	int		size;
	PList	coordinates;
}Point;

//------------PRIVATE FUNCTIONS----------
BOOL IntCompare(PElem coord1, PElem coord2) {
	if (coord1 == NULL || coord2 == NULL)
		return FALSE;
	int* coordinate1 = (int*)coord1;
	int* coordinate2 = (int*)coord2;
	return *coordinate1 == *coordinate2;
}

void IntPrint(PElem coord) {
	if(coord != NULL)
		printf("%d ", *(int*)coord);
}

PElem IntCopy(PElem coord) {
	if (coord == NULL)
		return NULL;
	int* copy_coord = (int*)malloc(sizeof(int));
	if (copy_coord == NULL)
		return NULL;
	*copy_coord = *(int*)coord;
	return copy_coord;
}

void IntDestroy(PElem coord) {
	free((int*)coord);
}


//------------PUBLIC FUNCTION------------

PPoint PointCreate(int dimension){
	PPoint point = (PPoint)malloc(sizeof(Point));
	if (point == NULL)
		return NULL;
	point->coordinates = ListCreate(IntCopy, IntDestroy, IntCompare, IntPrint);
	if (point->coordinates == NULL)
		return NULL;
	point->dim = dimension;
	point->size = 0;
	return point;
}

void PointDestroy(PElem point) {
	PPoint destroyPoint = (PPoint)point;
	if (destroyPoint == NULL)
		return;
	ListDestroy(destroyPoint->coordinates);
	free(destroyPoint);
}

Result PointAddCoordinate(PPoint point, int coordinate) {
	if (point == NULL)
		return FAIL;
	if (point->size < point->dim && ListAdd(point->coordinates, &coordinate)) {
		point->size++;
		return SUCCESS;
	}
	return FAIL;
}

int PointGetFirstCoordinate(PPoint point) {
	if (point == NULL)
		return 0;
	int* coord = (int*)ListGetFirst(point->coordinates);
	if (coord == NULL)
		return 0;
	return *coord;
}

int PointGetNextCoordinate(PPoint point) {
	if (point == NULL)
		return 0;
	int* coord = (int*)ListGetNext(point->coordinates);
	if (coord == NULL)
		return 0;
	return *coord;
}

void PointPrint(PElem point) {
	PPoint printPoint = (PPoint)point;
	if (printPoint == NULL)
		return;
	printf("Point Dimension: %d, Size: %d, Coordinates: ", printPoint->dim, printPoint->size);
	ListPrint(printPoint->coordinates);
}

BOOL PointCompare(PElem point1, PElem point2)
{
	PPoint pnt1 = (PPoint)point1;
	PPoint pnt2 = (PPoint)point2;
	if (pnt1 == NULL || pnt2 == NULL || pnt1->size != pnt2->size)
		return FALSE;
	return (PointGetSqrDistance(pnt1, pnt2)==0);
}

PElem PointCopy(PElem src)
{	
	PPoint source = (PPoint)src;
	if (source == NULL)
		return NULL;
	PPoint cpy = PointCreate(source->dim);
	if(cpy == NULL)
		return NULL;
	int* coord = (int*)ListGetFirst(source->coordinates);
	while (coord != NULL)
	{
		if (PointAddCoordinate(cpy, *coord) == FAIL)
		{
			PointDestroy(cpy);
			return NULL;
		}
		coord = (int*)ListGetNext(source->coordinates);
	}
	return cpy;
}

int PointGetDimension(PPoint point)
{
	if (point == NULL)
		return 0;
	return point->dim;
}

int PointGetSqrDistance(PPoint point1, PPoint point2)
{
	int distSum = 0, * coord1, * coord2;
	//Assumption: The points have the same dimension, and have "dimension" num of coordinates.
	coord1 = (int*)ListGetFirst(point1->coordinates);
	coord2 = (int*)ListGetFirst(point2->coordinates);
	while (coord1 != NULL) {
		distSum += (*coord1 - *coord2) * (*coord1 - *coord2);
		coord1 = (int*)ListGetNext(point1->coordinates);
		coord2 = (int*)ListGetNext(point2->coordinates);
	}
	return distSum;
}
