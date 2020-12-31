#include "apple.h"
#include "drawable.h"

/**
* @brief Initiates an apple in x,y
*/
Apple::Apple(unsigned short x, unsigned short y) :
	Drawable((struct rect) { x, y, 1, 1 }),
	drawFlag(false) {};

/**
* @brief Does nothing. Apples don't move.
*/
void Apple::move(direction_t direction){}

/**
* @brief Draw the object
*/
void Apple::draw() {
	if (!drawFlag) {
		mini_gui_print_rect(mg, bounding_box, APPLE);
		drawFlag = true; //So it won't be drawn again (unless screen is refreshed).
	}
}

/**
* @brief Return an ID that is unique to 
* the dynamid type of the drawable object.
*/
int Apple::id() {
return APPLE_ID;
}

/**
* @brief Is called whenever any refresh is required, re-enables drawing of the object.
*/
void Apple::refresh(){}

/**
 * @brief Does nothing. Apples stay put every step.
 * @param lst A list of all drawable objects in the world
 */
void Apple::step(DrawableList& lst){}

