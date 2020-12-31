#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


typedef struct Node_* PNode;
typedef struct Node_ {
	PElem Elem;
	PNode nextNode;
} Node;
struct List_ {
	PNode Iterator;
	PNode Head;
	PNode Tail;
	delPElem delete_PElem;
	cpyPElem copy_PElem;
	cmpPElem compare_Elem;
	printPElem print_PElem;
};

//------ 'Private' functions: 
/* @brief	Frees all allocated memory of a given Node. Warning: Doesn't handle fixing 'nextNode' of the previous node.
 * @param	list	Pointer to the parent list of the node.
 * @param	node	Pointer to a Node to be freed.
 */
void NodeDestroy(PList list, PNode node)
{
	if (node != NULL && list!=NULL)
	{
		if (node->Elem != NULL)
			list->delete_PElem(node->Elem);
		free(node);
	}
}

//--------------------------------------------------------------
//------ 'Public' functions:
PList ListCreate(cpyPElem copy_PElem, delPElem delete_PElem, cmpPElem compare_Elem, printPElem print_PElem) {
	if (delete_PElem == NULL || copy_PElem == NULL || compare_Elem == NULL || print_PElem == NULL)
		return NULL;
	PList List = (PList)malloc(sizeof(struct List_));
	if (List == NULL) {
		free(List);
		return NULL;
	}
	List->Head = NULL;
	List->Tail = NULL;
	List->Iterator = NULL;
	List->delete_PElem = delete_PElem;
	List->copy_PElem = copy_PElem;
	List->compare_Elem = compare_Elem;
	List->print_PElem = print_PElem;
	return List;
}
void ListDestroy(PList List) {
	if (List == NULL)
		return;
	List->Iterator = List->Head;
	while (List->Head != NULL) {
		List->Iterator = List->Head->nextNode;
		NodeDestroy(List, List->Head);
		List->Head = List->Iterator;
	}
	free(List);
}

Result ListAdd(PList List, PElem new_Elem) {
	if (List == NULL || new_Elem == NULL)
		return FAIL;
	PNode PNode_To_Add = (PNode)malloc(sizeof(Node));
	if (PNode_To_Add == NULL)
		return FAIL;
	PNode_To_Add->Elem = List->copy_PElem(new_Elem);
	if (PNode_To_Add->Elem == NULL)
	{
		NodeDestroy(List,PNode_To_Add);
		return FAIL;
	}
	PNode_To_Add->nextNode = NULL;
	if (List->Head == NULL) { //List is empty
		List->Head = PNode_To_Add;
		List->Tail = PNode_To_Add;
		return SUCCESS;
	}
	List->Tail->nextNode = PNode_To_Add;
	List->Tail = List->Tail->nextNode;
		return SUCCESS;
}

Result ListRemove(PList List, PElem Elem) {
	if (List == NULL || Elem == NULL || List->Head == NULL || List->Head->Elem == NULL)
		return FAIL;
	if (List->compare_Elem(List->Head->Elem, Elem)) {
		PNode Point = List->Head;
		List->Head = List->Head->nextNode;
		NodeDestroy(List,Point);
		return SUCCESS;
	}
	//If the Element to remove exists, It's not the Head
	List->Iterator = List->Head;
	while (List->Iterator->nextNode != NULL && !List->compare_Elem(List->Iterator->nextNode->Elem, Elem))
		List->Iterator = List->Iterator->nextNode;
	if (List->Iterator->nextNode == NULL)
		return FAIL;
	//if reached here, found Node to remove, and the Node is List->Iterator->nextNode
	PNode Elem_To_Remove = List->Iterator->nextNode;
	if (Elem_To_Remove == List->Tail) //about to remove the last element
		List->Tail = List->Iterator; //move tail to new last element
	List->Iterator->nextNode = List->Iterator->nextNode->nextNode;
	NodeDestroy(List, Elem_To_Remove);
	return SUCCESS;
}

PElem ListGetFirst(PList List) {
	if (List == NULL || List->Head == NULL)
		return NULL;
	List->Iterator = List->Head;
	return List->Head->Elem;
}

PElem ListGetNext(PList List) {
	if (List == NULL || List->Iterator == NULL)
		return NULL;
	List->Iterator = List->Iterator->nextNode;
	if (List->Iterator == NULL)
		return NULL;
	return List->Iterator->Elem;
}

BOOL ListCompare(PList List1, PList List2) {
	if (List1 == NULL || List2 == NULL) {
		if (List1 == NULL && List2 == NULL)
			return TRUE;
		return FALSE;
	}
	List1->Iterator = List1->Head;
	List2->Iterator = List2->Head;
	while (List1->Iterator != NULL && List2->Iterator != NULL) {
		if (!(List1->compare_Elem(List1->Iterator->Elem, List2->Iterator->Elem)))
			return FALSE;
		List1->Iterator = List1->Iterator->nextNode;
		List2->Iterator = List2->Iterator->nextNode;
	}
	if (List1->Iterator == NULL && List2->Iterator == NULL) // in case one is bigger than othen, but elements are the same as long as there are
		return TRUE;
	return FALSE;
}

void ListPrint(PList List) {
	if (List == NULL)
		return;
	PNode Pointer = List->Head;
	printf("[");
	while (Pointer != NULL) {
		List->print_PElem(Pointer->Elem);
		Pointer = Pointer->nextNode;
	}
	printf("]\n");
}