
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "drawable_list.h"
#include "drawable.h"
#include "apple.h"
#include "monster.h"

/**
 * @brief Holds all properties of the world
 */
struct world {
	int steps;
	int seed;
	struct rect size;
	DrawableList* items;
	char* log_filename;
};

struct world world;

/**
 * @brief Print error message with line number, and fail
 */
void fail(const char* reason, int line_number) {
	fprintf(stderr, "%s in line %d.\n", reason, line_number);
	exit(1);
}

/**
 * @brief Parses input from stdin and builds the world
 * @returns a 'struct world' object with all the data
 * @note Parsing errors should print message and exit.
 */
struct world parse_input() {
	struct world output;
	output.items = new DrawableList();
	output.log_filename = NULL;
	output.size = (struct rect){0,0,0,0};

	char str[100];
	int arg1, arg2, arg3;
	int line_num = 1;

	// Continue until reaching EOF
	while (scanf("%s", str) > 0) {
		if (!strcmp(str, "steps")) {
			if (scanf("%d", &arg1) != 1) {
				fail("Invalid number of arguments", line_num);
			}
			output.steps = arg1;
		} else if (!strcmp(str, "seed")) {
			if (scanf("%d", &arg1) != 1);
			output.seed = arg1;
		} else if (!strcmp(str, "size")) {
			if (scanf("%d %d", &arg1, &arg2) != 2) {
				fail("Invalid number of arguments", line_num);
			}
			output.size.width = arg1;
			output.size.height = arg2;
		} else if (!strcmp(str, "item")) {
			if (scanf("%s", str) != 1) {
				fail("Invalid number of arguments", line_num);
			}
			// Act according to item type
			if (!strcmp(str, "apple")) {
				if (scanf("%d %d", &arg1, &arg2) != 2) {
					fail("Invalid number of arguments", line_num);
				}
				// NOTE: arg1 - x, arg2 - y
				Apple* item = new Apple(arg1, arg2);
				output.items->push_back(*item);
			} else if (!strcmp(str, "monster")) {
				if (scanf("%d %d %d", &arg1, &arg2, &arg3) != 3) {
					fail("Invalid number of arguments", line_num);
				}
				// NOTE: arg1 - x, arg2 - y, arg3 - direction_hold
				Monster* item = new Monster(arg1, arg2, arg3);
				output.items->push_back(*item);
			} else {
				fail("Cannot recognize item", line_num);
			}
		} else if (!strcmp(str, "log")) {
			if (scanf("%s", str) != 1) {
				fail("Invalid number of arguments", line_num);
			}
			output.log_filename = strdup(str);
		} else {
			fail("Cannot recognize input", line_num);
		}
		++line_num;
	}

	return output;
}

/*
 * @brief The screen refresh method.
 * @note Is invoked by the MG object whenever a screen
 * refresh is performed.
 */
void refresh_screen(struct mini_gui* mg) {
	// Get the screen size
	struct rect r = mini_gui_get_screen_size(mg);
	// Top border
	mini_gui_move(mg, 0, 0);
	for (int i = 0; i < r.width; i++) {
		mini_gui_print(mg, "%s", "_");
	}
	// Bottom border
	mini_gui_move(mg, 0, r.height - 1);
	for (int i = 0; i < r.width; i++) {
		mini_gui_print(mg, "%s", "_");
	}
	// Left border
	for (int i = 1; i < r.height; i++) {
		mini_gui_move(mg, 0, i);
		mini_gui_print(mg, "%s", "|");
	}
	// Right border
	for (int i = 1; i < r.height; i++) {
		mini_gui_move(mg, r.width - 1, i);
		mini_gui_print(mg, "%s", "|");
	}
	// Refresh all world objects
	for (Iterator it = world.items->begin(); it.valid(); it.next()) {
		it.get_object()->refresh();
	}
}

/**
 * @brief The world's main loop
 * @param mg The MG provider
 */
void main_loop(struct mini_gui* mg) {
	int step_counter = 0;
	int pseudo_random = 1+world.seed;
	// Do world main loop
	while (world.steps > 0) {
		mini_gui_log(mg, "# Step %d\n", step_counter++);
		// For all objects
		for (Iterator it = world.items->begin(); it.valid(); it.next()) {
			// Choose direction (pseudo-random)
			pseudo_random = (97*pseudo_random+1)%137;
			int direction = pseudo_random % 4;
			// Move in the direction
			it.get_object()->move((direction_t)direction);
			// Do step
			it.get_object()->step(*world.items);
			// Draw
			it.get_object()->draw();
		}
		// Update steps
		--world.steps;
		// Move the cursor to top screen
		mini_gui_move(mg, 0, 0);
		// Sleep for 50 ms
		// (You can change that number to make simulation run faster / slower)
		mini_gui_sleep(mg, 50);
	}
}

/**
 * @brief Application entry point
 */
int main(int argc, char* argv[]) {

	// Parse input
	world = parse_input();

	// Initiate the MG object
	struct mini_gui* mg = mini_gui_init(world.log_filename, refresh_screen);
	if (!mg) {
		printf("Cannot initialize MG\n");
		exit(1);
	}

	// For running without graphics:
	// mini_gui_skip_drawing(mg);

	// Set attirubets
	Drawable::set_mg_provider(mg);
	if (world.size.width != 0) {
		mini_gui_set_screen_size(mg, world.size);
	}

	main_loop(mg);

	// Free all data
	mini_gui_log(mg, "# Cleanup\n");
	delete world.items;
	free(world.log_filename);
	mini_gui_destroy(mg);
}