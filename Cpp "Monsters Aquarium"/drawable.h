#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "mini_gui.h"
#include "ascii_objects.h"

// Dummy declaration
class DrawableList;

typedef enum {
	left, right, up, down
} direction_t;

/**
 * @brief An abstract class for drawing ASCII objects
 */
class Drawable {
protected:

	/*
	 * @brief The position & size of the ASCII object
	 */
	struct rect bounding_box;

	/*
	 * @brief A mini-gui pointer, relevant to all drawable objects
	 */
	static struct mini_gui* mg;

public:

	/*
	 * @brief Set the mini-gui pointer to all drawable objects
	 */
	static void set_mg_provider(struct mini_gui* mg);

	/*
	 * @brief Initiate this with bounding box
	 */
	Drawable(struct rect bb);

	virtual ~Drawable();

	/*
	 * @brief Returns true iff this collides with other
	 */
	bool collide(const Drawable& other);

	/*
	 * @brief Move the object in the direction
	 */
	virtual void move(direction_t direction) = 0;

	/*
	 * @brief Draw the object
	 */
	virtual void draw() = 0;
	
	/**
	 * @brief Is called whenever any refresh is required.
	 * See Mini-Gui refresh method.
	 */
	virtual void refresh() = 0;

	/*
	 * @brief Return an ID that is unique to 
	 * the type of the drawable object.
	 */
	virtual int id() = 0;

	/*
	 * @brief Do a step in the 'game of life'
	 * @param lst A list of all drawable objects in the world
	 */
	virtual void step(DrawableList& lst) = 0;
};

#endif