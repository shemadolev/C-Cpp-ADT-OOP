#ifndef MINI_GUI_H
#define MINI_GUI_H

#ifdef __cplusplus
extern "C" {
#endif

// Dummy declaration.
struct mini_gui;

/**
 * @brief A rectangle. Top-left is (X,Y), bottom-right is (X+W,Y+H);
 */
struct rect {
	unsigned short x;
	unsigned short y;
	unsigned short width;
	unsigned short height;
};

/**
 * @brief A refresh method prototype. Is invoked by MG
 * whenever a whole screen refresh should be done.
 * @note When called, MG cursor will always be at (0,0).
 */
typedef void(*mini_gui_refresh_method_t)(struct mini_gui*);

/**
 * @brief Initiate a mini-gui object.
 * @param log_filename A filename to write log outputs
 * @param refresh_callback A refresh callback method.
 * Is invoked whenever a reshresh should be done.
 * @returns a MG object, or NULL in case of an error.
 */
struct mini_gui* mini_gui_init(const char* log_filename, mini_gui_refresh_method_t refresh_callback);

/**
 * @brief Clear all allocated memory for MG.
 */
void mini_gui_destroy(struct mini_gui* mg);

/**
 * @brief Sets the window to a fixed size.
 * @param mg The MG object
 * @param size A rectangle; the fixed window size
 */
void mini_gui_set_screen_size(struct mini_gui* mg, struct rect size);

/**
 * @brief Set the MG object to skip all screen drawing & sleeping.
 * @note The log will still be updated.
 */
void mini_gui_skip_drawing(struct mini_gui* mg);

/**
 * @brief Returns a rectangle with the screen size
 * @param mg The MG object
 */
struct rect mini_gui_get_screen_size(struct mini_gui* mg);

/**
 * @brief Moves the cursor to a specific location within the terminal
 * @param mg The MG object
 * @param x The X location in the terminal
 * @param y The Y location in the terminal
 */
void mini_gui_move(struct mini_gui* mg, unsigned short x, unsigned short y);

/**
 * @brief Print a formatted string to the terminal
 * @param mg The MG object
 * @param fmt The formatted string
 * @param ... Any additional arguments
 */
void mini_gui_print(struct mini_gui* mg, const char* fmt, ...);

/**
 * @brief Print a formatted string within area
 * @param mg The MG object
 * @param rect A rectangle, a position within screen
 * @param fmt The formatted string
 * @param ... Any additional arguments
 */
void mini_gui_print_rect(struct mini_gui* mg, struct rect rect, const char* fmt, ...);

/**
 * @brief Clears an area in the screen (rectangle)
 * @param mg The MG object
 * @param rect A rectangle, a position within screen
 */
void mini_gui_clear_rect(struct mini_gui* mg, struct rect rect);

/**
 * @brief Adds a string to the logfile
 * @param mg The MG object
 * @param fmt The formatted string
 * @param ... Any additional arguments
 */
void mini_gui_log(struct mini_gui* mg, const char* fmt, ...);

/**
 * @brief Sleeps for X milliseconds
 * @param mg The MG object
 * @param milliseconds MS to sleep
 * @note Will not sleep in case the 'skip drawing' attribute was set
 */
void mini_gui_sleep(struct mini_gui* mg, int milliseconds);

#ifdef __cplusplus
}
#endif


#endif
