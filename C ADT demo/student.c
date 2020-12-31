#include "student.h"
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

struct Student_
{
	char*	name;
	int		age;
	int		ID;
	char*	faculty;
};

/* @brief	Creates a new instance of Student.
 * @param	name	Name of the student
 * @param	age		Student's age
 * @param	ID		Student's Identification No.
 * @param	faculty	Student's faculty
 * @return	A pointer to Student ADT (PStudent), or NULL if fails.
 */
PStudent StudentCreate(char* name, int age, int ID, char* faculty)
{
	if (name == NULL || faculty == NULL)
		return NULL;
	PStudent stud = (PStudent)malloc(sizeof(Student));
	if (stud == NULL) //Check if malloc failed
		return NULL;
	stud->age = age;
	stud->ID = ID;
	stud->name		= (char*)malloc(sizeof(char)*strlen(name)+1);
	stud->faculty	= (char*)malloc(sizeof(char)*strlen(faculty)+1);
	if (stud->name == NULL || stud->faculty == NULL)
	{
		destroyStudent(stud);
		return NULL;
	}
	strcpy(stud->name, name);
	strcpy(stud->faculty, faculty);
	return stud;
}

/* @brief	Prints a student's info as in the following example: Name: Avi, Age: 30, ID: 35452214, Faculty: EE
 * @param	stud	A pointer to the Student ADT.
 */
void printStudent(PElem stud)
{
	PStudent student = (PStudent)stud;
	printf("Name: %s, Age: %d, ID: %d, Faculty: %s\n", student->name, student->age, student->ID, student->faculty);
}

/* @brief	Compares 2 instances of Students.
 * @param	student1 Pointer to the first student ADT to comapre.
 * @param	student2 Pointer to the second student ADT to comapre.
 * @return	TRUE if has the same ID.
 */
BOOL compareStudents(PElem student1, PElem student2)
{
	if (student1 == NULL || student2 == NULL)
		return FALSE;
	return (((PStudent)student1)->ID == ((PStudent)student2)->ID);
}

/* @brief	Creates a copy of a Student instance.
 * @param	source	Pointer to the Student to be copied from.
 * @return	Pointer to Student.
 */
PElem cloneStudent(PElem source)
{
	if (source == NULL)
		return NULL;
	PStudent studSource = (PStudent)source;
	return StudentCreate(studSource->name, studSource->age, studSource->ID, studSource->faculty);
}

/* @brief	Frees all memory allocated for a given Student.
 * @param	student	Pointer to the Student to be deleted.
 */
void destroyStudent(PElem student)
{
	PStudent stud = (PStudent)student;
	if (student != NULL)
	{
		free(stud->name);
		free(stud->faculty);
		free(stud);
	}
}

