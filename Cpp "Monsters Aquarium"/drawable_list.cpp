#include "drawable_list.h"
#include "drawable.h"

/**
* @brief Make an iterator from a pointer to list node
*/
Iterator::Iterator(Node* n) : ptr(n)
{
	if (n != nullptr)
		n->iterator_counter++;
}

Iterator::Iterator(Node& n) : ptr(&n) {}

Iterator::Iterator(const Iterator& other) { //copy const'or
	ptr = other.ptr;
	increase_counter();
}

void Iterator::decrease_counter() {
	if (ptr == nullptr)//the node was already deleted
		return;
	if (ptr->iterator_counter == 1 && !valid()) { //removing last iterator from node AND it's not valid
		delete ptr->item; //free the memory of the Drawable object
		delete ptr; //free the memory of the Node
		return;
	}
	//not last iterator
	ptr->iterator_counter--;
}

void Iterator::increase_counter() {
	if (ptr == nullptr)//the node was already deleted
		return;
	//node exists
	ptr->iterator_counter++;
}

Iterator::~Iterator() {
	decrease_counter(); //the note is taken care of in decrease_counter()
}


Drawable* Iterator::get_object() {
	if (ptr == nullptr || ptr->item == nullptr)//the node or item was already deleted
		return nullptr;
	return ptr->item;
}

void Iterator::invalidate() {
	if (ptr == nullptr)//the node was already deleted
		return;
	ptr->valid = false;
}

Iterator& Iterator::set(const Iterator& other) {
	if (this != &other) { //Check the object is not set to itself
		decrease_counter();
		ptr = other.ptr; //Might be nullptr, but it's ok
		increase_counter();
	}
	return *this;
}

Iterator& Iterator::next() {
	if (ptr == nullptr || ptr->next == nullptr) {//Iterator has no Node, or points to the last Node in the list
		decrease_counter();
		ptr = nullptr;
		return *this;
	}
	//if here, not the last Node
	Node* ndFindValid = ptr->next;
	while (!ndFindValid->valid) { //The Iterator points to a real Node
		if (ndFindValid->next == nullptr) { //no valid Node found
			decrease_counter();
			ptr = nullptr;
			return *this;
		}
		ndFindValid = ndFindValid->next;
	}
	decrease_counter();
	ptr = ndFindValid;
	increase_counter();
	return *this;
}

Iterator& Iterator::prev() {
	if (ptr == nullptr || ptr->prev == nullptr) {//Iterator has no Node, or points to the last Node in the list
		decrease_counter();
		ptr = nullptr;
		return *this;
	}
	//if here, not the last Node
	Node* ndFindValid = ptr->prev;
	while (!ndFindValid->valid) { //The Iterator points to a real Node
		if (ndFindValid->prev == nullptr) { //no valid Node found
			decrease_counter();
			ptr = nullptr;
			return *this;
		}
		ndFindValid = ndFindValid->prev;
	}
	decrease_counter();
	ptr = ndFindValid;
	increase_counter();
	return *this;
}

bool Iterator::valid() const {
	if (ptr != nullptr)
		return ptr->valid;
	return false;
}

DrawableList::DrawableList() :
	head(nullptr),
	tail(nullptr),
	size(0) {}

DrawableList::~DrawableList()
{
	for (Iterator it = begin(); it.valid();it.next()){
		erase(it);
	}
}

void DrawableList::push_front(Drawable& item)
{
	// validation: Can an item be sent as NULL? this is NOT a pointer... UPDATE: Appearently, NULL can't be passed byRef.
	Node* newNode = new Node();
	newNode->item = &item;
	newNode->next = head; //Might be nullptr, but doesn't matter.
	newNode->prev = nullptr;
	newNode->valid = true;
	if (head != nullptr)
		head->prev = newNode;
	head = newNode;
	if (tail == nullptr) //Meaning the list was empty
		tail = newNode;
	size++;
}

void DrawableList::push_back(Drawable& item)
{
	Node* newNode = new Node();
	newNode->item = &item;
	newNode->next = nullptr; 
	newNode->prev = tail; //Might be nullptr, but doesn't matter.
	newNode->valid = true;
	if (tail != nullptr)
		tail->next = newNode;
	tail = newNode;
	if (head == nullptr)  //Meaning the list was empty
		head = newNode;
	size++;
}

void DrawableList::erase(Iterator& it)
{
	if (!it.valid()) //Meaning this node was already erased from the list
		return;

	Node* ndErase = it.ptr;
	Node* ndPrev = ndErase->prev;
	Node* ndNext = ndErase->next;
	if (ndPrev != nullptr)
		ndPrev->next = ndNext; //Might be nullptr
	if (ndNext != nullptr)
		ndNext->prev = ndPrev; //Might be nullptr

	//Update head / tail if necessary
	if (head == ndErase)
		head = ndNext; //Might be nullptr
	if (tail == ndErase)
		tail = ndPrev; //Might be nullptr

	it.invalidate();
	size--;
}

int DrawableList::get_size() const
{
	return size;
}

Iterator DrawableList::begin()
{
	Iterator itHead = Iterator(head);
	if (!itHead.valid()) //Shouldn't happen, but - handle a first non-valid node.
		return itHead.next();
	return itHead;
}

Iterator DrawableList::end()
{
	Iterator itTail = Iterator(tail);
	if (!itTail.valid()) //Shouldn't happen, but - handle a last non-valid node. 
		return itTail.prev();
	return itTail;
}
