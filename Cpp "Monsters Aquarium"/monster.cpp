#include "monster.h"
#include "apple.h"
#include "drawable.h"
#include "drawable_list.h"

Monster::Monster(unsigned short x, unsigned short y, int direction_hold) :
	Drawable((struct rect){ x, y, 1, 1 }),
	level(1),
	vel(1),
	current_direction(left),
	direction_hold(direction_hold),
	direction_counter(0),
	next_bb(bounding_box) {}

void Monster::move(direction_t direction)
{
	//if (direction_counter >= direction_hold)
	if (direction_counter == 0) //Haven't moved yet / time to change direction
		current_direction = direction;
	// Get world size
	struct rect world_size = mini_gui_get_screen_size(mg);
	switch (current_direction)
	{
		case left:
			if (bounding_box.x - vel > 0)
				next_bb.x -= vel;
			break;
		case right:
			if (bounding_box.x + bounding_box.width + vel < world_size.width)
				next_bb.x += vel;
			break;
		case up:
			if (bounding_box.y - vel > 0)
				next_bb.y -= vel;
			break;
		case down:
			if (bounding_box.y + bounding_box.height + vel < world_size.height)
				next_bb.y += vel;
			break;
	}
	//correct_boundries(); //make a correction if we moved out of the world boundries

	if (++direction_counter >= direction_hold) //Reset direction_counter if moved enough on same direction
		direction_counter = 0;
}

void Monster::draw()
{
	mini_gui_clear_rect(mg, bounding_box);
	
	//Update bounding box, using the default assignment operator
	bounding_box = next_bb;
	
	mini_gui_print_rect(mg,bounding_box,gfx);
	//mini_gui_print_rect(mg,bounding_box,"%s",gfx); //Other option
}

int Monster::id()
{
	return MONSTER_ID;
}

void Monster::refresh() {
	if (level < 5) {
		gfx = MONSTER0;
		vel = 1;
		next_bb.width = 1;
		next_bb.height = 1;
	}
	else if (level < 15) {
		gfx = MONSTER1;
		vel = 1;
		next_bb.width = 3;
		next_bb.height = 1;
	}
	else if (level < 25) {
		gfx = MONSTER2;
		vel = 1;
		next_bb.width = 3;
		next_bb.height = 2;
	}
	else {
		gfx = MONSTER3;
		vel = 2;
		next_bb.width = 8;
		next_bb.height = 3;
	}
	// Get world size
	struct rect world_size = mini_gui_get_screen_size(mg);
	// Fix position in case of screen overflow
	if (next_bb.x + next_bb.width >= world_size.width) {
		next_bb.x = world_size.width - next_bb.width;
	}
	if (next_bb.y + next_bb.height >= world_size.height) {
		next_bb.y = world_size.height - next_bb.height;
	}
}

void Monster::step(DrawableList& lst) {
	Drawable* curDrawable;
	Monster* curMonster;
	bool foundMyself = false;
	for (Iterator it = lst.begin(); it.valid(); it.next()) {
		curDrawable = it.get_object();
		if (this == curDrawable) { //This is me! skip
			foundMyself = true; //Remeber I passed my place in the list, to be used if I need to find myself later.
			continue;
		}
		//Check if we collide
		if (collide(*curDrawable)) { 
			if (curDrawable->id() == MONSTER_ID) {
				curMonster = dynamic_cast<Monster*>(curDrawable);
				if (level > curMonster->level) { //I'm stronger, win! :)
					level += (curMonster->level);
					lst.erase(it);
					refresh();
				}
				else { // I lost, goodbye cruel world :(
					if (foundMyself)
						die(lst); //Already reached my location on list, so search from the begining.
					else
						die(lst, it.next()); //Haven't reached my location on the list, so my Node must be ahead.
					curMonster->level += level;
					curMonster->refresh();
					return;
				}
			}
			else { //Must be an apple :P
				level++; //Apple make me stronger! (OM NOM NOM)
				refresh();
				lst.erase(it);
			}
		}
	}
}

/**
 * @brief To be used upon losing a fight against another monster.
 * @param lst A list of all drawable objects in the world
 */
void Monster::die(DrawableList& lst) {
	Iterator it = lst.begin();
	die(lst, it);
}

/**
 * @brief To be used upon losing a fight against another monster.
 * @param lst A list of all drawable objects in the world
 * @param startFrom An Iterator to start the search from
 */
void Monster::die(DrawableList& lst, Iterator startFrom) {
	for (; startFrom.valid(); startFrom.next()) {
		if (startFrom.get_object() == this)	{
			lst.erase(startFrom);
			return;
		}
	}
}

