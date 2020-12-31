#ifndef _STUDENT_H_
#define _STUDENT_H_
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef struct Student_ Student, *PStudent;

/* @brief	Creates a new instance of Student.
 * @param	name	Name of the student
 * @param	age		Student's age
 * @param	ID		Student's Identification No.
 * @param	faculty	Student's faculty
 * @return	A pointer to Student ADT (PStudent), or NULL if fails.
 */
PStudent StudentCreate(char* name, int age, int ID, char* faculty);


/* @brief	Prints a student's info as in the following example: Name: Avi, Age: 30, ID: 35452214, Faculty: EE
 * @param	stud	A pointer to the Student ADT.
 */
void printStudent(PElem stud);

/* @brief	Compares 2 instances of Students.
 * @param	student1 Pointer to the first student ADT to comapre.
 * @param	student2 Pointer to the second student ADT to comapre.
 * @return	TRUE if has the same ID.
 */
BOOL compareStudents(PElem student1, PElem student2);

/* @brief	Creates a copy of a Student instance.
 * @param	source	Pointer to the Student to be copied from.
 * @return	Pointer to Student.
 */
PElem cloneStudent(PElem source);

/* @brief	Frees all memory allocated for a given Student.
 * @param	student	Pointer to the Student to be deleted.
 */
void destroyStudent(PElem student);

#endif
