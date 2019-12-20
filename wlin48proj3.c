#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

int debugMode = FALSE;

typedef struct mazeStruct
{
   char **arr;                 //Dynamic array to store the entire maze
   int xSize, ySize;           //To store the maze size
   int xStart, yStart;         //To store the starting position
   int xEnd, yEnd;             //To store the ending position
   int xPosition, yPosition;   //To store the block
} Maze;


typedef struct nodeStruct
{
   int xCoordinate;        //To store X coordinate
   int yCoordinate;        //To store y coordinate
   struct nodeStruct *next;//The node to next list
} Node;

typedef Node* NodePointer;

Maze initializeMaze(FILE *source);
NodePointer initializeStack(Maze m1);
void top(NodePointer pHead, int *xCoordinate, int *yCoordinate);
void push(NodePointer *pHead, int xCoordinate, int yCoordinate);
void pop(NodePointer *pHead);
int isEmpty(NodePointer head);
void printMaze(Maze m1);
void showLinkedList(NodePointer pHead);
void clearStack(NodePointer *pHead);
void freeMaze(Maze m1);


//In this function, we are going to initialize the maze
Maze initializeMaze(FILE *source)
{
   Maze m1;  //The maze object

   /* read in the size, starting and ending positions in the maze */
   //Read in the size for the maze
   while(!feof(source))
   {
      fscanf (source, "%d %d", &m1.xSize, &m1.ySize);

      if(m1.xSize < 1 || m1.ySize < 1)
      {
         fprintf(source,"=> Invalid: Maze sizes must be greater than 0 \n");
         continue;
      }
      else
      {
         //Allocate the 2-d dynamic array
         m1.arr = (char**)malloc(sizeof(char*)*(m1.xSize + 2));

         for(int i = 0; i < m1.xSize + 2; i++)
         {
            m1.arr[i] = (char*)malloc(sizeof(char)*(m1.ySize + 2));
         }

         //Initialize the maze to empty
         for (int i = 0; i < m1.xSize+2; i++)
            for (int j = 0; j < m1.ySize+2; j++)
               m1.arr[i][j] = '.';

         //Mark the borders of the maze with *'s
         for (int i=0; i < m1.xSize+2; i++)
         {
            m1.arr[i][0] = '*';
            m1.arr[i][m1.ySize+1] = '*';
         }
         for (int i=0; i < m1.ySize+2; i++)
         {
            m1.arr[0][i] = '*';
            m1.arr[m1.xSize+1][i] = '*';
         }
         break;
      }
   }

   //Read in the starting point
   while(!feof(source))
   {
      fscanf (source, "%d %d", &m1.xStart, &m1.yStart);

      if(m1.xStart < 1 || m1.yStart < 1)
      {
         fprintf(source,"=> Invalid: value for the starting point should be greater than 1\n");
         continue;
      }
      else if(m1.xStart > m1.xSize || m1.yStart > m1.ySize)
      {
         if(m1.xStart > m1.xSize)
            fprintf(source,"=> Invalid: row %d is out of the range from 1 to %d", m1.xStart, m1.xSize);

         if(m1.yStart > m1.ySize)
            fprintf(source,"=> Invalid: column %d is out of the range from 1 to %d", m1.yStart, m1.ySize);
         continue;
      }
      else
         break;
   }

   //To store the ending point
   while(!feof(source))
   {
      fscanf (source, "%d %d", &m1.xEnd, &m1.yEnd);

      if(m1.xEnd < 1 || m1.yEnd < 1)
      {
         fprintf(source,"=> Invalid: value for the ending point should be greater than 1\n");
         continue;
      }
      else if(m1.xEnd > m1.xSize || m1.yEnd > m1.ySize)
      {
         if(m1.xEnd > m1.xSize)
            fprintf(source,"=> Invalid: row %d is out of the range from 1 to %d", m1.xEnd, m1.xSize);

         if(m1.yEnd > m1.ySize)
            fprintf(source,"=> Invalid: column %d is out of the range from 1 to %d", m1.yEnd, m1.ySize);
         continue;
      }
      else
         break;
   }

   //Mark the blocked positions in the maze with *'s
   while (!feof(source))
   {
      fscanf (source, "%d %d", &m1.xPosition, &m1.yPosition);

      if(m1.xPosition == m1.xStart && m1.yPosition == m1.yStart)
      {
         fprintf(source,"\nInvalid value for the block point, the block point should not block the starting point\n");
         continue;
      }
      else if(m1.xPosition == m1.xEnd && m1.yPosition == m1.yEnd)
      {
         fprintf(source,"\nInvalid value for the block point, the block point should not block the ending point\n");
         continue;
      }
      else if(m1.xPosition > m1.xSize || m1.yPosition > m1.ySize)
      {
         fprintf(source,"\nInvalid value for the block point, the block point should be within the maze size\n");
         continue;
      }
      else
         m1.arr[m1.xPosition][m1.yPosition] = '*';
   }

   //Print them out to verify the input
   printf ("\nsize: %d, %d\n", m1.xSize, m1.ySize);
   printf ("start: %d, %d\n", m1.xStart, m1.yStart);
   printf ("end: %d, %d\n", m1.xEnd, m1.yEnd);

   //Mark the starting and ending positions in the maze
   m1.arr[m1.xStart][m1.yStart] = 's';
   m1.arr[m1.xEnd][m1.yEnd] = 'e';

   return m1;
}


//In this function, we are going to initialize the stack
NodePointer initializeStack(Maze m1)
{
   NodePointer pHead = NULL;     // A node pointr to store the head
   int solutionFound = FALSE;    //A boolean to decide whether the solution is found or not

   //A dynamic allocate temp variable
   char *temp = (char*)malloc(sizeof(char)*(m1.xSize + 2)*(m1.ySize + 2));

   memcpy(temp, m1.arr, (m1.xSize + 2)*(m1.ySize + 2));  //Copy the memory

   //Push the staring point to the stack
   push(&pHead, m1.xStart, m1.yStart);
   //Mark starting point as V
   m1.arr[m1.xStart][m1.yStart] = 'V';


   //If the stack is not empty or the solution is not found, the loop will be executed
   while((!isEmpty(pHead)) && (!solutionFound))
   {
      //When the debug mode is on, print out the statement
      if(debugMode == TRUE)
      {
         printf("List stack:");
         showLinkedList(pHead);
         printf("\n");
      }
      //Assign x and y to starting point
      int x = -1, y = -1;
      //Assign x and to top
      top(pHead, &x, &y);

      //All below are the dfs
      if(m1.arr[x][y+1] == '.' || m1.arr[x][y+1] == 'e')
      {
         m1.arr[x][y+1] = 'V';
         push(&pHead, x, y+1);
         y++;
      }
      else if(m1.arr[x+1][y] == '.' || m1.arr[x+1][y] == 'e')
      {
         m1.arr[x+1][y] = 'V';
         push(&pHead, x+1, y);
         x++;
      }
      else if(m1.arr[x][y-1] == '.'|| m1.arr[x][y-1] == 'e')
      {
         m1.arr[x][y-1] = 'V';
         push(&pHead, x, y-1);
         y--;
      }
      else if(m1.arr[x-1][y] == '.' || m1.arr[x][y] == 'e' )
      {
         m1.arr[x-1][y] = 'V';
         push(&pHead,x-1, y);
         x--;
      }
      else
      {
         //If we cannot find any unvisited or unblocked neighbor, we pop out the coordinate
         pop(&pHead);

         //When the debug mode is on, print out the statement
         if( debugMode == TRUE )
         {
            printf("List stack:");
            showLinkedList( pHead );
            printf("\n");
         }
      }

      //If the starting point and ending point are euqal, then the solution is found
      if( m1.xEnd == x && m1.yEnd == y )
      {
         solutionFound = TRUE;
         break;
      }
   }

   memcpy(temp, m1.arr, (m1.xSize + 2)*(m1.ySize + 2));
   return pHead; //Return pHead
}


//In this function, we are going to assignment the x and y coordinate to top every time we push
void top(NodePointer pHead, int *xCoordinate, int *yCoordinate)
{
   if(pHead!=NULL )
   {
      *xCoordinate = pHead->xCoordinate;   //The head x coordinate is the coordinate from maze
      *yCoordinate = pHead->yCoordinate;   //The head y coordinate is the coordinate from maze
   }
   else
   {
      printf("Stack is Empty\n");
      exit(-1);
   }
}


//In this function, we are going to push the opening symbol onto the stack
void push(NodePointer *pHead, int xCoordinate, int yCoordinate)
{
   NodePointer pTemp =(NodePointer)malloc(sizeof(Node));

   pTemp->xCoordinate = xCoordinate;
   pTemp->yCoordinate = yCoordinate;
   pTemp->next = *pHead;
   *pHead = pTemp;
}


//In this function, we are poping the top element in the linked list
void pop(NodePointer *pHead)
{
   if(*pHead != NULL)
   {
      NodePointer temp = *pHead;

      *pHead = (*pHead)->next;
      free(temp);
   }
}


//In this function, we are going to check if the stack is empty or not
int isEmpty(NodePointer head)
{
   return (head == NULL);
}


//In this function, we are going to print out the maze
void printMaze(Maze m1)
{
   //Print out the maze
   for(int i = 0; i < m1.xSize+2; i++)
   {
      for(int j = 0; j < m1.ySize+2; j++)
         printf ("%c", m1.arr[i][j]);
      printf("\n");
   }
}


//In this function, we are printing out the linked list
void showLinkedList(NodePointer pHead)
{
   if(pHead != NULL)
   {
      showLinkedList( pHead->next);
      printf("(%d, %d)\n", pHead->xCoordinate, pHead->yCoordinate);
   }
}


//In this function, we are going to pop everything out of the stack
void clearStack(NodePointer *pHead)
{
   while(*pHead != NULL)
   {
      pop(pHead);
   }
}


//In this function, we are going to free up the memory of the arr in the maze struct
void freeMaze(Maze m1)
{
   free(m1.arr);
}


int main(int argc, char **argv)
{
   FILE *source;

   /* verify the proper number of command line arguments were given */
   if(argc > 3)
   {
      for(int i = 0; i < argc; i++)
      {
         if(argv[i][0] == '-' && argv[i][1] == 'd')
         {
            debugMode = TRUE;
         }
      }
      printf("Usage: %s <input file name>\n", argv[0]);
      exit(-1);
   }

   /* Try to open the input file. */
   if ((source = fopen( argv[1], "r" )) == NULL )
   {
      printf ( "Can't open input file %s", argv[1] );
      exit(-1);
   }


   Maze m1 = initializeMaze(source);              //Assign m1 to the initializeMaze
   printMaze(m1);                                 //Print out the maze
   NodePointer stack = initializeStack(m1);       //Assign stack to initializedStack

   //If the stack is empty, then there's no solution
   if(isEmpty(stack))
      printf("\nThere is no way to reach the end\n");

   else  // Else, we change the visite path into V
   {
      //Print out the path coordinate
      printf("\nThe path are:\n");
      showLinkedList(stack);
      printf("\n");
   }

   //Free array
   clearStack(&stack);
   freeMaze(m1);
}








