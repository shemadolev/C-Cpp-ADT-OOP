#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "point.h"
#include "cluster.h"



//typedef struct Cluster_
//{
//	int dimension;
//	PList pointList;
//	int minSqDist;
//} Cluster, *PCluster;
typedef struct Cluster_
{
	int dimension;
	PList pointList;
	int minSqDist;
} Cluster;

PCluster ClusterCreate(int dimension)
{
	PCluster cluster = (PCluster)malloc(sizeof(Cluster));
	if (cluster == NULL)
		return NULL;
	cluster->dimension = dimension;
	cluster->pointList = ListCreate(PointCopy, PointDestroy, PointCompare, PointPrint);
	if (cluster->pointList == NULL)
	{
		ClusterDestroy(cluster);
		return NULL;
	}
	cluster->minSqDist = DISTANCE_UPBOUND; //Assumption: Min square distance will be shorter than DISTANCE_UPBOUND. 
	return cluster;
}

void ClusterDestroy(PCluster cluster)
{
	if (cluster == NULL)
		return;
	ListDestroy(cluster->pointList);
	free(cluster);
}

Result ClusterAddPoint(PCluster cluster, PPoint point)
{
	if (cluster == NULL || point == NULL || (PointGetDimension(point) != cluster->dimension))
		return FAIL; //at least one param is NULL, or dimensions don't match
	int minDist = DISTANCE_UPBOUND;
	//Check if this is the first point in the cluster
	if (ListGetFirst(cluster->pointList) != NULL)
	{
		minDist = ClusterGetMinDistance(cluster, point);
		if (minDist == 0) // Meaning the point is already in the cluster
			return FAIL;
	}
	if (ListAdd(cluster->pointList, point) == FAIL)
		return FAIL;
	//Check if minSqDist should be updated
	if (minDist < cluster->minSqDist)
		cluster->minSqDist = minDist;
	return SUCCESS;
}

int ClusterGetMinDistance(PCluster cluster, PPoint point)
{
	if (cluster == NULL || point == NULL) {
		return FAIL_INT;
	}
	//Assumption: point and cluster have same dimension.
	int minDist, curDist;
	PPoint iteratePoints = (PPoint)ListGetFirst(cluster->pointList);
	if(iteratePoints == NULL) //Check if the cluster is empty
		return FAIL_INT;
	minDist = DISTANCE_UPBOUND;
	while (iteratePoints != NULL)
	{
		curDist = PointGetSqrDistance(iteratePoints, point);
		if (curDist == 0) //This is the lowest value, also meaning points are equal
			return 0;
		if (curDist < minDist)
			minDist = curDist;
		iteratePoints = (PPoint)ListGetNext(cluster->pointList);
	}
	return minDist;
}


void ClusterPrint(PCluster cluster)
{
	if (cluster != NULL) {
		printf("Cluster's dimension: %d\n", cluster->dimension);
		ListPrint(cluster->pointList);
		printf("Minimum Square Distance: %d\n", cluster->minSqDist);
	}
}

