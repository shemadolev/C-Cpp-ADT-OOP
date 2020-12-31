#ifndef _CLUSTER_H_
#define _CLUSTER_H_
#include "defs.h"
#include "point.h"

/* @define	DISTANCE_UPBOUND	Assuming square distance will always be lower than this value. */
#define DISTANCE_UPBOUND 10000;
#define FAIL_INT		-1;

typedef struct Cluster_* PCluster;

/* @brief	Creates a new empty intsance of point cluster.
 * @param	dimension	Required dimension, will be the same for
						all future points in this cluster. 
 * @return	A pointer to (an empty) Cluster ADT, or NULL if fails.
 */
PCluster ClusterCreate(int dimension);

/* @brief	Frees all allocated data of a Cluster instance.
 * @param	cluster	A pointer to Cluster which will be destroyed.
 */
void ClusterDestroy(PCluster cluster);

/* @brief	Adds a Point to a Cluster.
 * @param	cluster	A pointer to a Cluster instance which the point will be added to.
 * @param	point	A pointer to a Point instance, which will be added to the cluster.
 * @return	SUCCESS if point was added successfully, or FAIL else.
 */
Result ClusterAddPoint(PCluster cluster, PPoint point);

/* @brief	Calculates the minimum distance between a cluster and a point.
 * @param	cluster	A pointer to a Cluster.
 * @param	point	A pointer to a Point.
 * @return	Calculated minimum distance (Notice: '0' means point is already in the cluster!)
 */
int ClusterGetMinDistance(PCluster cluster, PPoint point);

/* @brief	Prints a Cluster's dimnesion, a list of all of its Points,
			and its minimum square distance.
 * @param	cluster	A pointer to a Cluster.
 */
void ClusterPrint(PCluster cluster);

#endif
