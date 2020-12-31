
#ifdef _WIN32
// Windows
#define _CRT_SECURE_NO_WARNINGS 1 
#include <windows.h>
#else
// Linux
#define _BSD_SOURCE
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "mini_gui.h"

#define MOVE_BEGIN "\r"
#define MOVE_UP "\033[A"
#define MOVE_LEFT "\033[D"
#define MOVE_RIGHT "\033[C"
#define MOVE_DOWN "\033[B"

struct mini_gui {
	// Terminal properties
	unsigned short rows;
	unsigned short cols;
	// Points to current location
	unsigned short cursor_row;
	unsigned short cursor_col;
	// Points to a refresh method
	mini_gui_refresh_method_t refresh_callback;
	// File descriptor for writing logs
	FILE* log_file;
	// More attributes
	int fixed_size;
	int disable_drawing;
};

/**
 * @brief Private method of MG. Checks whether a whole screen
 * refresh id required. If so, invokes the refresh callback.
 */
static void check_rehresh(struct mini_gui* mg) {
	// Skip in case of fixed size window after refresh
	if (mg->fixed_size == 2) {
		return;
	}
	// Skip in case of disable drawing
	if (mg->disable_drawing) {
		return;
	}
	// Get window #rows & #cols
	unsigned short rows, cols;
#ifdef _WIN32
	// Windows
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
	// Linux
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	rows = w.ws_row;
	cols = w.ws_col;
#endif
	// Skip in case the size of the window has not changed
	if ((rows == mg->rows) &&
		(cols == mg->cols))
	{
		return;
	}
	// Change window size only if not set as fixed
	if (mg->fixed_size==0) {
		mg->rows = rows;
		mg->cols = cols;
	} else {
		// Make sure only a single refresh will take place
		mg->fixed_size = 2;
	}
	// Performing refresh
	mg->cursor_row = 0;
	mg->cursor_col = 0;
	// Clear screen. Leave cursor at top-left corner.
	for (int y = 0; y < mg->rows; ++y) {
		fprintf(stdout, "\n");
		for (int x = 0; x < mg->cols; ++x) {
			fprintf(stdout, " ");
		}
	}
	fprintf(stdout, MOVE_BEGIN);
	for (int y = 0; y < mg->rows; ++y) {
		fprintf(stdout, MOVE_UP);
	}
	fflush(stdout);
	// Call refresh callback
	if (mg->refresh_callback) {
		mg->refresh_callback(mg);
		mini_gui_move(mg, 0, 0);
	}
}

/**
 * @brief Private method of NG. Adjust a rectangle to fit in screen.
 */
static void adjust_rect(struct mini_gui* mg, struct rect* rect) {
	// Validate rectangle size
	if (rect->x >= mg->cols) rect->x = mg->cols - 1;
	if (rect->y >= mg->rows) rect->y = mg->rows - 1;
	if (rect->x + rect->width >= mg->cols) {
		rect->width = mg->cols - rect->x - 1;
	}
	if (rect->y + rect->height >= mg->rows) {
		rect->height = mg->rows - rect->y - 1;
	}
}

/**
 * @brief Initiate a mini-gui object.
 * @param log_filename A filename to write log outputs
 * @param refresh_callback A refresh callback method.
 * Is invoked whenever a reshresh should be done.
 * @returns a MG object, or NULL in case of an error.
 */
struct mini_gui* mini_gui_init(const char* log_filename, mini_gui_refresh_method_t refresh_callback) {
	// Allocate memory
	struct mini_gui* mg = (struct mini_gui*)malloc(sizeof(struct mini_gui));
	if (!mg) {
		return NULL;
	}
	// Default values
	mg->rows = 0;
	mg->cols = 0;
	mg->cursor_row = 0;
	mg->cursor_col = 0;
	mg->refresh_callback = refresh_callback;
	mg->fixed_size = 0;
	mg->disable_drawing = false;
	// Try to open log file for writring
	mg->log_file = fopen(log_filename, "w");
	if (mg->log_file == NULL) {
		free(mg);
		return NULL;
	}
	// Disable terminal echo
#ifdef _WIN32
	// Windows
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(hStdin, &mode);
	SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
#else
	// Linux
	struct termios t;
	tcgetattr(0, &t);
	t.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &t);
#endif
	return mg;
}

/**
 * @brief Clear all allocated memory for MG.
 * @param mg The MG object
 */
void mini_gui_destroy(struct mini_gui* mg) {
	// Clear screen. Leave cursor at top-left corner.
	mini_gui_move(mg, 0, 0);
	for (int y = 0; y < mg->rows; ++y) {
		fprintf(stdout, "\n");
		for (int x = 0; x < mg->cols; ++x) {
			fprintf(stdout, " ");
		}
	}
	fprintf(stdout, MOVE_BEGIN);
	for (int y = 0; y < mg->rows; ++y) {
		fprintf(stdout, MOVE_UP);
	}
	fflush(stdout);
	// Destroy objects
	fclose(mg->log_file);
	free(mg);
}

/**
 * @brief Sets the window to a fixed size
 * @param mg The MG object
 * @param size A rectangle; the fixed window size
 */
void mini_gui_set_screen_size(struct mini_gui* mg, struct rect size) {
	if (!mg) return;
	mg->fixed_size = 1;
	mg->rows = size.height;
	mg->cols = size.width;
}

/**
 * @brief Set the MG object to skip all screen drawing & sleeping.
 * @note The log will still be updated.
 */
void mini_gui_skip_drawing(struct mini_gui* mg) {
	if (!mg) return;
	mg->disable_drawing = true;
}

/**
 * @brief Returns a rectangle with the screen size
 * @param mg The MG object
 */
struct rect mini_gui_get_screen_size(struct mini_gui* mg) {
	if (!mg) {
		return (struct rect) { 0, 0, 0, 0 };
	}
	// Refresh screen 
	check_rehresh(mg);
	unsigned short c = mg->cols, r = mg->rows;
	return (struct rect) {
		0, 0, c, r
	};
}

/**
 * @brief Moves the cursor to a specific location within the terminal
 * @param mg The MG object
 * @param x The X location in the terminal
 * @param y The Y location in the terminal
 */
void mini_gui_move(struct mini_gui* mg, unsigned short x, unsigned short y) {
	if (!mg) return;
	// Do we skip drawing?
	if (mg->disable_drawing) return;
	// Refresh screen 
	check_rehresh(mg);
	// Assure the X and Y fit the screen
	if (x >= mg->cols) x = mg->cols-1;
	if (y >= mg->rows) y = mg->rows-1;
	// Move cursor
	while (mg->cursor_row < y) {
		++mg->cursor_row;
		fprintf(stdout, MOVE_DOWN);
	}
	while (mg->cursor_row > y) {
		--mg->cursor_row;
		fprintf(stdout, MOVE_UP);
	}
	while (mg->cursor_col < x) {
		++mg->cursor_col;
		fprintf(stdout, MOVE_RIGHT);
	}
	while (mg->cursor_col > x) {
		--mg->cursor_col;
		fprintf(stdout, MOVE_LEFT);
	}
	fflush(stdout);
}

/**
 * @brief Print a formatted string to the terminal
 * @param mg The MG object
 * @param fmt The formatted string
 * @param ... Any additional arguments
 */
void mini_gui_print(struct mini_gui* mg, const char* fmt, ...) {
	if (!mg) return;
	// Refresh screen 
	check_rehresh(mg);
	// Calculate print size
	va_list args;
	va_start(args, fmt);
	int size = vsnprintf(NULL, 0, fmt, args);
	// Put string into buffer
	va_start(args, fmt);
	char* buffer = (char*)malloc(sizeof(char)*size + 1);
	vsnprintf(buffer, size + 1, fmt, args);
	// Store current cursors
	unsigned short start_cursor_x = mg->cursor_col;
	unsigned short start_cursor_y = mg->cursor_row;
	int pos = 0;
	// Skip printing in case of overflow or disable print
	if (mg->disable_drawing) goto end_print;
	if ((mg->cursor_col >= mg->cols) &&
	    (mg->cursor_row >= mg->rows - 1)) goto end_print;
    // Print while no overflowing screen
	while (pos < size) {
		++mg->cursor_col;
		fputc(buffer[pos], stdout);
		++pos;
		// Return
		if (mg->cursor_col >= mg->cols) {
			if (mg->cursor_row >= mg->rows) {
				goto end_print;
			}
			mg->cursor_col = 0;
			++mg->cursor_row;
			fprintf(stdout, MOVE_DOWN);
			fprintf(stdout, MOVE_BEGIN);
		}
	}

	end_print:
	fflush(stdout);
	// Update logfile
	mini_gui_log(mg, "mini_gui_print(%d, %d, \"%s\")\n",
		start_cursor_x, start_cursor_y, buffer);
	// Flush output
	free(buffer);
}

/**
 * @brief Print a formatted string within area
 * @param mg The MG object
 * @param rect A rectangle, a position within screen
 * @param fmt The formatted string
 * @param ... Any additional arguments
 */
void mini_gui_print_rect(struct mini_gui* mg, struct rect rect, const char* fmt, ...) {
	if (!mg) return;
	int pos = 0;
	// Refresh screen 
	check_rehresh(mg);
	// Adjust rectangle to fit in screen
	adjust_rect(mg, &rect);
	// Calculate print size
	va_list args;
	va_start(args, fmt);
	int size = vsnprintf(NULL, 0, fmt, args);
	// Print to buffer
	va_start(args, fmt);
	char* buffer = (char*)malloc(sizeof(char)*size + 1);
	vsnprintf(buffer, size + 1, fmt, args);
	// Do we skip drawing?
	if (mg->disable_drawing) goto end_rect_print;
	// Split to horizontal lines
	// Print each to stdout in location
	for (int h = 0; h < rect.height; ++h) {
		mini_gui_move(mg, rect.x, rect.y + h);
		for (int x = 0; x < rect.width; ++x) {
			if (pos > size) {
				goto end_rect_print;
			}
			fputc(buffer[pos], stdout);
			++pos;
			++mg->cursor_col;
		}
	}
end_rect_print:
	// Update logfile
	mini_gui_log(mg, "mini_gui_print_rect({%d,%d,%d,%d},\"%s\")\n",
		rect.x, rect.y, rect.width, rect.height, buffer);
	// Flush output
	fflush(stdout);
	free(buffer);
	mini_gui_move(mg, mg->cursor_col, mg->cursor_row);
}

/**
 * @brief Clears an area in the screen (rectangle)
 * @param mg The MG object
 * @param rect A rectangle, a position within screen
 */
void mini_gui_clear_rect(struct mini_gui* mg, struct rect rect) {
	if (!mg) return;
	// Refresh screen 
	check_rehresh(mg);
	// Adjust rectangle to fit in screen
	adjust_rect(mg, &rect);
	// Do we skip drawing?
	if (!mg->disable_drawing) {
		// Clear area
		for (int h = 0; h<rect.height; ++h) {
			mini_gui_move(mg, rect.x, rect.y + h);
			for (int x = 0; x<rect.width; ++x) {
				fputc(' ', stdout);
				++mg->cursor_col;
			}
		}
	}
	// Update logfile
	mini_gui_log(mg, "mini_gui_clear_rect({%d,%d,%d,%d})\n",
		rect.x, rect.y, rect.width, rect.height);
	// Flush output
	fflush(stdout);
	mini_gui_move(mg, mg->cursor_col, mg->cursor_row);
}

/**
 * @brief Adds a string to the logfile
 * @param mg The MG object
 * @param fmt The formatted string
 * @param ... Any additional arguments
 */
void mini_gui_log(struct mini_gui* mg, const char* fmt, ...) {
	if (!mg) return;
	// Calculate print size
	va_list args;
	va_start(args, fmt);
	int size = vsnprintf(NULL, 0, fmt, args);
	// Print to buffer
	va_start(args, fmt);
	char* buffer = (char*)malloc(sizeof(char)*size + 1);
	vsnprintf(buffer, size + 1, fmt, args);
	// Update logfile
	fprintf(mg->log_file, "%s", buffer);
	fflush(mg->log_file);
	free(buffer);
}

/**
 * @brief Sleeps for X milliseconds
 * @param mg The MG object
 * @param milliseconds MS to sleep
 * @note Will not sleep in case the 'skip drawing' attribute was set
 */
void mini_gui_sleep(struct mini_gui* mg, int milliseconds) {
	// Do we skip drawing?
	if (mg->disable_drawing) return;
#ifdef _WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1e3);
#endif
}
