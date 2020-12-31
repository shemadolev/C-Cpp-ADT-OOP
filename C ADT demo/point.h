#ifndef _POINT_H_
#define _POINT_H_

#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct Point_* PPoint;

/* @brief	Create new point with no coordinates.
 * @param	dimension	Dimension of point.
 * @return	Pointer to the new point if success, NULL otherwise.
 */
PPoint PointCreate(int dimension);

/* @brief	Removes point and allocated memory.
 * @param	Point to be destroyed.
 */
void PointDestroy(PElem point);

/* @brief	Add coordinate to a given point.
 * @param	Point to add coordinate to.
 * @param	New coordinate to add.
 * @return	SUCCESS if coordinate was added, FAIL otherwise.
 */
Result PointAddCoordinate(PPoint point, int coordinate);

/* @brief	Returns first coordinate of the point.
 * @param	Point to extract first coordinate.
 * @return	First coordinate of point if exists, '0' otherwise.
 */
int PointGetFirstCoordinate(PPoint point);

/* @brief	Returns next coordinate.
 * @param	Point to extract next coordinate.
 * @return	Next coordinate of point if exists, '0' otherwise.
 */
int PointGetNextCoordinate(PPoint point);

/* @brief	Prints point.
 * @param	Point to print.
 */
void PointPrint(PElem point);

// Below are functions that we added, and were not described explicitly on HW instructions

/* @brief
 * @param	point1
			point2
 * @return
 */
BOOL PointCompare(PElem point1, PElem point2);

/* @brief
 * @param	source
 * @return
 */
PElem PointCopy(PElem src);

/* @brief
 * @param	point
 * @return
 */
int PointGetDimension(PPoint point);

/* @brief	Calculates a defined "distance" of 2 points.
 * @param	point1
			point2
 * @return
 */
int PointGetSqrDistance(PPoint point1, PPoint point2);

#endif
