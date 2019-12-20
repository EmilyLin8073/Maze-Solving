# Maze-Solving
A C program that will find its way through a maze using the depth-first search algorithm.

The following shows an example of such an input file. 
The coordinates are given with the row listed first and the column listed second. 
A maze of NxM has rows numbered from 1 to N and columns number from 1 to M.
10 20 
1 1 
10 20 
5 1 
4 2 
3 3 
1 10 
2 9 
3 8 
4 7 
5 6 
6 5 
7 4 
8 3

The above input creates the following maze will 11 blocked positions:
      size: 10, 20
      start: 1, 1
      end: 10, 20
********************** 
*s........*..........* 
*........*...........* 
*..*....*............* 
*.*....*.............* 
**....*..............* 
*....*...............* 
*...*................* 
*..*.................* 
*....................* 
*...................e* 
**********************

The blocked positions and the edges of the above maze are filled in with *'s. 
The start position is filled in with an 's'. 
The end position in filled in with an 'e'. 
The other positions are filled in with periods.
