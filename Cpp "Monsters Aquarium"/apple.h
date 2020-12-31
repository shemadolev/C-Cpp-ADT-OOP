#ifndef APPLE_H
#define APPLE_H

#define APPLE_ID 3

#include "drawable.h"

class Apple : public Drawable {
	//Indicates if the object was drawn, since should be drawn only once every screen refresh.
	bool drawFlag;
public:

	/**
	* @brief Initiates an apple in x,y
	*/
	Apple(unsigned short x, unsigned short y);

	/**
	* @brief Does nothing. Apples don't move.
	*/
	virtual void move(direction_t direction);

	/**
	* @brief Draw the object
	*/
	virtual void draw();

	/**
	 * @brief Return an ID that is unique to 
	 * the dynamid type of the drawable object.
	 */
	virtual int id();

	/**
	 * @brief Is called whenever any refresh is required
	 */
	virtual void refresh();

	/**
	 * @brief Does nothing. Apples stay put every step.
	 * @param lst A list of all drawable objects in the world
	 */
	void step(DrawableList& lst);
};

#endif 
