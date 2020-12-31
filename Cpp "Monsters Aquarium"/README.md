## Demonstrates C++ classes, inheritance, virtual classes, and terminal "GUI".

The program expects initial user commands, and then simulates different creatures in different sizes that move and fight (pseudo) randomly.
All actions will written into a log file.

Input should be in the following format:

### Part 1:
```
log <name for a log file>
`seed <integer as a seed for the random actions>
size <dimension x for the arena> <dimension y for the arena>   
steps <number of steps to run the simulation>`  
```
### Part 2:
A list of creatures:
```
item apple <x position> <y position>
item monster <x position> <y position> <number of steps before changing direction>
```
Aplles are stationary, while monsters move around and will become stronger (and bigger) every time they eat apples or defeat another monster.
