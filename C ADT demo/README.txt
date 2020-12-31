A simple program that implements ADTs (Abstract Data Types) in C language.
Use 'make' to compile the demo.

Following commands are supported as user input:
1) Add_Student <ID> <NAME> <AGE>
    Creates a temporary ADT of type Student according to the parameters, and adds it to the student list.
2) Remove_Student <ID>
    If ID==All, deletes all the entries from the student list.
    Else, deletes the entry that matches the ID passed.
3) Start_Cluster <DIM>
    Creates an ADT of type Cluster with dimension DIM. Assumes that there will be only one cluster in the program.
4) Clear_Cluster
    Deletes the cluster.
5) Add_Point <DIM> <COORD_1> <COORD_2>… <COORD_DIM>
    Creates a temporary ADT of type Point with dimension DIM. Adds all of the coordinates, and adds the point to the cluster. Finally, deletes the temporary point.
6) Print
    Prints the student list and the cluster.
