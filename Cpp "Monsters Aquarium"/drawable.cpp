#include "drawable.h"

struct mini_gui* Drawable::mg = nullptr;

/*
 * @brief Set the mini-gui pointer to all drawable objects
 */
void Drawable::set_mg_provider(struct mini_gui* mg) {
	Drawable::mg = mg;
}

/*
 * @brief Initiate this with bounding box
 */
Drawable::Drawable(struct rect bb) : bounding_box(bb) {}

Drawable::~Drawable() {
	mini_gui_clear_rect(mg, bounding_box);
}

/*
 * @brief Returns true iff this collides with other
 */
bool Drawable::collide(const Drawable& other) {
	bool i_inside_another =
		   (bounding_box.x >= other.bounding_box.x) &&
		   (bounding_box.x < other.bounding_box.x + other.bounding_box.width) &&
		   (bounding_box.y >= other.bounding_box.y) &&
		   (bounding_box.y < other.bounding_box.y + other.bounding_box.height);
	bool other_inside_me = 
		   (other.bounding_box.x >= bounding_box.x) &&
		   (other.bounding_box.x < bounding_box.x + bounding_box.width) &&
		   (other.bounding_box.y >= bounding_box.y) &&
		   (other.bounding_box.y < bounding_box.y + bounding_box.height);
	return i_inside_another || other_inside_me;
}
