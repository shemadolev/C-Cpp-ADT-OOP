#ifndef DRAWABLE_LIST_H
#define DRAWABLE_LIST_H

#include "drawable.h"

// A node for the linked-list implementation
struct Node {
	// Pointer to previous node
	struct Node* prev;
	// Pointer to next node
	struct Node* next;
	// Pointer to the drawable object
	Drawable* item;
	// How many iterators point to this node?
	int iterator_counter;
	// Is this node valid?
	bool valid;
	
	/**
	 * @brief Sets default values for node
	 */
	Node() : 
		prev(nullptr),
		next(nullptr),
		item(nullptr),
		iterator_counter(0),
		valid(false) {};
};

// An iterator over the list
//Kadosh:
class Iterator {
	struct Node* ptr;
	friend class DrawableList;

	/**
	 * @brief Decreases iterator counter in the node pointed by this
	 * @note In case this is the last iterator that
	 * points to a node, AND the node is invalid free the memory of the node
	 * (including everything!)
	 */
	void decrease_counter();

	/**
	 * @brief Increases the iterator counter
	 * of the node pointed by ptr
	 */
	void increase_counter();

	/**
	 * @brief Make an iterator from a list node
	 */
	Iterator(Node& n);

	/**
	 * @brief Make an iterator from a pointer to list node
	 */
	Iterator(Node* n);

public:

	/**
	 * @brief When cloning iterators, one must update
	 * the iterator counter of the node
	 */
	Iterator(const Iterator& other);

	/**
	 * @brief Updates iterator counter in the node pointed by this
	 * @note In case this is the last iterator that
	 * points to a node, AND the node is invalid - free the memory of the node
	 * (including everything!)
	 */
	~Iterator();

	/*
	 * @brief Returns the item pointerd by this
	 */
	Drawable* get_object();

	/*
	 * @brief Make the node pointed by this to be invalid.
	 * @note In case this is the last iterator that
	 * points to a node, free the memory of the node
	 * (including everything!)
	 */
	void invalidate();

	/**
	 * @brief Upon assigning, we must invalidate the iterator.
	 * @note We also must update the node interator counter
	 */
	Iterator& set(const Iterator& other);

	/**
	 * @brief Changes the state of this to point at the next valid node
	 * @returns this
	 * @note In case there is no next valid node, sets this as invalid
	 */
	Iterator& next();

	/**
	 * @brief Changes the state of this to point at the previous valid node
	 * @returns this
	 * @note In case there is no previous valid node, sets this as invalid
	 */
	Iterator& prev();

	/**
	 * @breif Returns true iff this is valid
	 */
	bool valid() const;
};

// A list of Drawable objects
//Shema:
class DrawableList {

	// Head and tail of the list
	struct Node* head;
	struct Node* tail;

	// Size of list
	int size;

public:

	/*
	 * @brief Creates an empty list
	 */
	DrawableList();

	/*
	 * @brief Free all the objects of this.
	 * @note Must free the memory for the drawable objects as well
	 */
	~DrawableList();

	/*
	 * @brief Push to list front (before head).
	 * @note Do not clone the object!
	 */
	void push_front(Drawable& item);

	/*
	 * @brief Push to list back (after tail).
	 * @note Do not clone the object!
	 */
	void push_back(Drawable& item);

	/**
	 * @brief Removes an object pointed by the iterator
	 * @note Must invalidate the iterator.
	 */
	void erase(Iterator& it);

	/**
	 * @brief Returns the size of this
	 */
	int get_size() const;

	/**
	 * @brief Returns an iterator to the beginning of the list
	 */
	Iterator begin();

	/**
	 * @brief Returns an iterator to the end of the list
	 */
	Iterator end();
};

#endif 