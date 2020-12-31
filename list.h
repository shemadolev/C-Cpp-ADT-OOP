#ifndef _LIST_H_
#define _LIST_H_
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct List_* PList;
// Pointer to any Element for abstruction
typedef void* PElem;

//User Functions for "delete", "compare", "print" and "copy" of elements
typedef void (*delPElem)(PElem);
typedef PElem (*cpyPElem)(PElem);
typedef BOOL (*cmpPElem)(PElem, PElem);
typedef void (*printPElem)(PElem);

/* @brief Creat new List and returns pointer
 * @param Function for "delete", "compare", "print" and "copy" of elements
 * @return NULL if faild
 */
PList ListCreate(cpyPElem copy_PElem, delPElem delete_PElem, cmpPElem compare_PElem, printPElem print_PElem);

/* @brief Delete a list and all of its elements
 * @param List to be destroyed
 */
void ListDestroy(PList List);

/* @brief Adds a copy of new_PElem to the list
 * @param An element to copy and a list to add to 
 * @return SUCCESS if copied, else FAIL
 */
Result ListAdd(PList List, PElem new_Elem);

/* @brief Removes Elem from list
 * @param PElem to remove and List to be removed from
 * @return SUCCESS if similae Elem removed, else FAIL
 */
Result ListRemove(PList List, PElem Elem);

/* @brief Returns first Elem from list
 * @param List to return first Elem from
 * @return Pointer to the first Elem in list
 */
PElem ListGetFirst(PList List);

/* @brief Moves Iterator one step forward and returns new Elem the Iterator points to
 * @param List to check from
 * @return Pointer to next Elem, NULL if the Iterator reached the end of the list
 */
PElem ListGetNext(PList List);

/* @brief Compares 2 lists
 * @param Lists to compare
 * @return TRUE if identical, else FALSE
 */
BOOL ListCompare(PList List1, PList List2);

/* @brief Prints given list
 * @param List to print
 */
void ListPrint(PList List);
#endif
