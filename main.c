/*! @define	RECURS_MAX	taille max de récursivité */
#define	RECURS_MAX 3

/*! @define	ECOLE_SERVER_PORT	le port de connexion */
#define	ECOLE_SERVER_PORT 1234
/*! @define	SERVER_PORT	le port de connexion */
#define	SERVER_PORT 8080
/*! @define	PLAYER_NAME	le nom du joueur */
#define PLAYER_NAME "LefebvreVincent"
/*! @define	LOCAL_HOST 	le serveur en local */
#define LOCAL_HOST "localhost"
/*! @define	POLY_HOST	l'adresse du serveur poly
tech */
#define POLY_HOST "polydev.cia-polytech-sorbonne.fr"
#define PROF_HOST "li2051-215.members.linode.com"
/*! @define	STANDARD_X_SIZE	largeur standard d'un plateau */
#define STANDARD_X_SIZE 10
/*! @define	STANDARD_Y_SIZE	hauteur standard d'un plateau */
#define STANDARD_Y_SIZE 10
/*! @define	DIFFICULTY	La difficulté du jeu */
#define DIFFICULTY 0
/*! @define	GAME_NAME	le nom de la partie */
#define GAME_NAME "TestGameVL"
/*! @define ECOLE_HOST 		le host à l'école */
#define ECOLE_HOST "pc4064.polytech.umpc.fr"
/*
*	NOM : LEFEBVRE
*	PRENOM : VINCENT
*	GROUPE : EISE3 TP3
*	TP : 
*	FICHIER :	
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "snakeAPI.h"
#include "clientAPI.h"

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))
#define array_size(array) (int)(sizeof(array)/sizeof(*(array)))
#define last_move(rcode) ((rcode == WINNING_MOVE)||(rcode == LOOSING_MOVE ) )
typedef struct
{
  int x1;
  int y1;
  int x2;
  int y2;
} t_wall;

typedef struct
{
  char right;
  char left;
  char down;
  char up;

} t_canGo;
void
whereHe (t_canGo canGo)
{

  if (canGo.up)
    printf ("\n can go North ");
  if (canGo.down)
    printf ("\n can go South ");
  if (canGo.left)
    printf ("\n can go West ");
  if (canGo.right)
    printf ("\n can go East ");


}

int
getNbOfPath (t_canGo canGo)
{
  int retour = 0;
  if (canGo.right)
    retour++;
  if (canGo.left)
    retour++;
  if (canGo.up)
    retour++;
  if (canGo.down)
    retour++;
  return retour;
}

typedef struct
{
  int *x;
  int *y;

} t_gameBoundaries;

typedef struct s_body
{
  int *position;
  struct s_body *nextPart;
} t_body;

void
updatePosition (t_move move, int *movTab)
{

  switch (move)
    {
    case 0:
      movTab[1]--;
      break;

    case 1:
      movTab[0]++;
      break;

    case 2:
      movTab[1]++;
      break;

    case 3:
      movTab[0]--;
      break;
    }


}

void
initializePosition (t_gameBoundaries boundaries, int hePlays, int *advPos,
		    int *allPos)
{
  if (hePlays)
    {
      allPos[0] = *(boundaries.x) - 3;
      advPos[0] = 2;

      allPos[1] = ((*(boundaries.y)) / 2)+2 ;
      advPos[1] = ((*(boundaries.y)) / 2)+1;

    }
  else
    {
      advPos[0] = (*(boundaries.x) - 3);
      allPos[0] = 2;

      advPos[1] = (*(boundaries.y) / 2)+1;
	
      allPos[1] = (*(boundaries.y) / 2)+2 ;
    }
}

void
initializeCanGo (t_canGo * canGo)
{
  canGo->right = 1;
  canGo->left = 1;
  canGo->down = 1;
  canGo->up = 1;


}

int
obsAtLeft (int *pos, int *obsPos)
{
  return ((pos[1] == obsPos[1]) && (pos[0] - 1 == obsPos[0]));
}

int
obsAtRight (int *pos, int *obsPos)
{
  return ((pos[1] == obsPos[1]) && (pos[0] + 1 == obsPos[0]));
}

int
obsAtNorth (int *pos, int *obsPos)
{
  return ((pos[1] + 1 == obsPos[1]) && (pos[0] == obsPos[0]));
}

int
obsAtSouth (int *pos, int *obsPos)
{
  return ((pos[1] - 1 == obsPos[1]) && (pos[0] + 1 == obsPos[0]));
}


/*
*Cul De Sac is like this
* __  __      ___   
* |    |      | |
* |_  _| |__| 
*
*/

/*!
 * @abstract	retourne un mur
 * @param	snake		la partie du corps début du cul de sac 
 * @result	la position interdite
 */




int
updateSnake (t_body * snakeHead, int nbTurn, int *position, t_move lastMove,
	     char ***tabWalls, t_gameBoundaries boundaries)
{
  t_body *save = snakeHead;
  t_body *save2 = snakeHead;
  int *tmpPos1 = malloc (sizeof (int) * 2);
  int *tmpPos2 = malloc (sizeof (int) * 2);
  int update[4] = { 1 };
  int i;
/* sauvegarde la position de têt précédente*/
  tmpPos1[0] = snakeHead->position[0];
  tmpPos1[1] = snakeHead->position[1];
  tmpPos2[0] = snakeHead->position[0];
  tmpPos2[1] = snakeHead->position[1];

  updatePosition (lastMove, position);
  


  while (snakeHead->nextPart)
    {

      snakeHead = snakeHead->nextPart;
      /*Sauvegarde de la position du corps suivant */
      tmpPos2[0] = snakeHead->position[0];
      tmpPos2[1] = snakeHead->position[1];
      /*Assignation de la position du corps précedent */
      snakeHead->position[0] = tmpPos1[0];

      snakeHead->position[1] = tmpPos1[1];
      /*La position du corps suivant est assignée à la position du corps précédent */
      tmpPos1[0] = tmpPos2[0];
      tmpPos1[1] = tmpPos2[1];

    }
  if (nbTurn % 10 == 0)
    {
      snakeHead->nextPart = malloc (sizeof (t_body));
      snakeHead->nextPart->position = malloc (sizeof (int) * 2);
      snakeHead->nextPart->nextPart = NULL;
    }
  snakeHead = save2;
  snakeHead->position[0] = position[0];
  snakeHead->position[1] = position[1];
/*				   printf("\n\t %d",*(boundaries.x));
				   printf("\n\t %d",*(boundaries.y));
*/


  while (snakeHead)
    {
      for (i = 0; i < 4; i++)
	{
	  update[i] = 1;
	}
if(position[1]==0){
    update[2]=0;
}
if(position[1]==*boundaries.y-1){
    update[1]=0;
}	
if(position[0]==0){
    update[0]=0;
}
if(position[1]==*boundaries.y-1){
    update[3]=0;
}	

      tmpPos2[0] = snakeHead->position[0];
      tmpPos2[1] = snakeHead->position[1];

      if (!tmpPos2[1])
	{
	  update[0] = 0;
	}
      if (!tmpPos2[0])
	{
	  update[3] = 0;
	}
      if (tmpPos2[0] >= *(boundaries.x) - 1)
	{
	  update[1] = 0;
	}
      if (tmpPos2[1] >= *(boundaries.y) - 1)
	{
	  update[2] = 0;
	}

      if (update[0])
	{
	  tabWalls[tmpPos2[0]][tmpPos2[1] - 1][2] = 0;
	}
      if (update[1])
	{
	  tabWalls[tmpPos2[0] + 1][tmpPos2[1]][3] = 0;
	}
      if (update[2])
	{
	  tabWalls[tmpPos2[0]][tmpPos2[1]+1][0] = 0;

	}
      if (update[3])
	{
	  tabWalls[tmpPos2[0] - 1][tmpPos2[1]][1] = 0;
	}
      snakeHead = snakeHead->nextPart;
    }
  snakeHead = save;
  free (tmpPos2);
  free (tmpPos1);
  return 1;
}

void
checkWalls (int *wall, int *position, t_canGo * canGo)
{
  t_wall twall;
  twall.x1 = *wall;
  twall.y1 = *(wall + 1);
  twall.x2 = *(wall + 2);
  twall.y2 = *(wall + 3);
  int xlength;

  int ylength;
  int i;
  if (twall.x1 > twall.x2)
    xlength = twall.x1 - twall.x2;
  else
    xlength = twall.x2 - twall.x1;

  if (twall.y1 > twall.y2)
    ylength = twall.y1 - twall.y2;
  else
    ylength = twall.y2 - twall.y1;

  for (i = twall.x1; i <= twall.x1 + xlength; i++)
    {
      if ((position[0] + 1 == i)
	  && (position[1] == twall.y1 || position[1] == twall.y2))
	{
	  canGo->right = 0;
	}

      if ((position[0] - 1 == i)
	  && (position[1] == twall.y1 || position[1] == twall.y2))
	{
	  canGo->left = 0;
	}
    }
  for (i = twall.y1; i < twall.y1 + ylength; i++)
    {
      if ((position[1] - 1 == i)
	  && (position[0] == twall.x1 || position[0] == twall.x2))
	{
	  canGo->up = 0;
	}

      if ((position[1] + 1 == i)
	  && (position[0] == twall.x1 || position[0] == twall.x2))
	{
	  canGo->down = 0;
	}
    }
}



void
checkBoundaries (t_gameBoundaries boundaries, int *position, t_canGo * canGo)
{
  if (position[1] == 0)
    {
      canGo->up = 0;
    }
  if (position[0] == 0)
    {
      canGo->left = 0;
    }
  if (position[1] == *(boundaries.y) - 1)
    {
      canGo->down = 0;
    }

  if (position[0] == *(boundaries.x) - 1)
    {
      canGo->right = 0;
    }
}

/*!
 * @function	avoidAutoBlocking
 * @abstract	Eviter de se bloquer tout seul contre un mur
 * @param	canGo	solutions possibles
 * @param	head	la tête du snake
 */
void
avoidAutoBlocking (t_canGo * canGo, t_body * head)
{
  t_body *save = head;

  if (head->position[0] == 0)
    {
      while (head)
	{
	  if (head->position[0] == 0)
	    {
	      if (save->position[1] < head->position[1] - 1)
		{
		  if (canGo->up || canGo->right || canGo->left)
		    {
		      canGo->down = 0;
		    }
		}
	    }
	  head = head->nextPart;
	}
    }
  head = save;
  if (head->position[1] == 0)
    {

    }

}

void
checkLastMove (t_canGo * canGo, t_move lastMove)
{

  if (lastMove == NORTH)
    {
      canGo->down = 0;
    }
  if (lastMove == WEST)
    {
      canGo->right = 0;
    }
  if (lastMove == SOUTH)
    {
      canGo->up = 0;
    }
  if (lastMove == EAST)
    {
      canGo->left = 0;
    }

}

t_move
decideMove (t_canGo canGo)
{
  t_move nextMove;
  int move = 0;
  int rnd;
  char tabSolution[4] = { 0, 0, 0, 0 };
  if (canGo.left)
    {
      tabSolution[3] = 1;
      nextMove = WEST;
    }
  if (canGo.up)
    {
      tabSolution[0] = 1;
      nextMove = NORTH;
    }
  if (canGo.down)
    {
      tabSolution[2] = 1;
      nextMove = SOUTH;
    }
  if (canGo.right)
    {
      tabSolution[1] = 1;
      nextMove = EAST;
    }
  if (!tabSolution[0] && !tabSolution[1] && !tabSolution[2] && !tabSolution[3]
      && !tabSolution[4])
    {
      nextMove = NORTH;
    }
  else
    {
      while (!move)
	{
	  rnd = rand () % 5;
	  move = tabSolution[rnd];
	}
      nextMove = rnd;
    }

  return nextMove;
}

void
bodyPartAnalysis (int *headPosition, t_body * bodyPart, t_canGo * canGo)
{

  int *currentPosition = bodyPart->position;
  if (headPosition[0] + 1 == currentPosition[0]
      && headPosition[1] == currentPosition[1])
    {
      canGo->right = 0;
    }
  if (headPosition[0] - 1 == currentPosition[0]
      && headPosition[1] == currentPosition[1])
    {
      canGo->left = 0;
    }
  if (headPosition[1] + 1 == currentPosition[1]
      && headPosition[0] == currentPosition[0])
    {
      canGo->down = 0;
    }
  if (headPosition[1] - 1 == currentPosition[1]
      && headPosition[0] == currentPosition[0])
    {
      canGo->up = 0;
    }

}

void
copyWallsTab (char ***tabWalls, char ***saveWalls,
	      t_gameBoundaries boundaries)
{
  int x_size = *boundaries.x;
  int y_size = *boundaries.y;
  int i, j, k;
  for (i = 0; i < x_size; i++)
    {
      for (j = 0; j < y_size; j++)
	{
	  for (k = 0; k < 4; k++)
	    {
	      tabWalls[i][j][k] = saveWalls[i][j][k];
	    }
	}
    }

}

void
checkSnake (t_body * snake, t_canGo * canGo, int *currentHeadPosition)
{
  t_body *save = snake;
  while (snake->nextPart)
    {
      bodyPartAnalysis (currentHeadPosition, snake, canGo);
      snake = snake->nextPart;
    }
  snake = save;

}


void
initializeBodies (t_body * snake1, t_body * snake2, int *pos1, int *pos2,
		  char ***tabWalls)
{
  snake1->position = malloc (sizeof (int) * 2);

  snake2->position = malloc (sizeof (int) * 2);

  snake1->position[0] = pos1[0];
  snake1->position[1] = pos1[1] ;
  snake2->position[0] = pos2[0];
  snake2->position[1] = pos2[1] ;


  tabWalls[snake2->position[0]][snake2->position[1] + 1][0] = 0;
  tabWalls[snake2->position[0]][snake2->position[1] - 1][2] = 0;
  tabWalls[snake2->position[0] + 1][snake2->position[1]][1] = 0;
  tabWalls[snake2->position[0] - 1][snake2->position[1]][3] = 0;
  tabWalls[snake1->position[0]][snake1->position[1] + 1][0] = 0;
  tabWalls[snake1->position[0]][snake1->position[1] - 1][2] = 0;
  tabWalls[snake1->position[0] + 1][snake1->position[1]][1] = 0;
  tabWalls[snake1->position[0] - 1][snake1->position[1]][3] = 0;
  snake1->nextPart = NULL;
  snake2->nextPart = NULL;
}


void
freeASnake (t_body * snake)
{
  t_body *tmp = snake;
  t_body *next = snake;


  while (tmp->nextPart)
    {
      free (tmp->position);
      next = tmp->nextPart;
      free (tmp);
      tmp = next;
    }
  free (tmp->position);
  free (tmp);

}

void
getWalls (char ***tabWalls, int x_size, int y_size, int *walls, int nbWalls)
{
  int i, j, k;
  int tmp = 0;

  int posX1, posY1, posX2, posY2;
  for (i = 0; i < x_size; i++)
    {
      tabWalls[i][0][0] = 0;
      tabWalls[i][y_size - 1][2] = 0;
    }
  for (i = 0; i < y_size; i++)
    {
      tabWalls[0][i][3] = 0;
      tabWalls[x_size - 1][i][1] = 0;
    }

  for (i = 0; i < nbWalls * 4; i += 4)
    {
      posX1 = walls[i];
      posY1 = walls[i + 1];
      posX2 = walls[i + 2];
      posY2 = walls[i + 3];

//Cas ----
      if (posX1 == posX2)
	{
	  if (posY1 < posY2)
	    {
	      tabWalls[posX1][posY2][0] = 0;
	      tabWalls[posX1][posY1][2] = 0;
	    }
	  else
	    {
	      tabWalls[posX1][posY2][2] = 0;
	      tabWalls[posX1][posY1][0] = 0;
	    }
	}
      else
	{
	  if (posX1 > posX2)
	    {

	      tabWalls[posX1][posY1][3] = 0;
	      tabWalls[posX2][posY1][1] = 0;
	    }
	  else
	    {
	      tabWalls[posX1][posY1][1] = 0;
	      tabWalls[posX2][posY1][3] = 0;

	    }
	}
    }

/* Get rid of CDS
 */

  for (i = 0; i < x_size; i++)
    {
      for (j = 0; j < y_size; j++)
	{
	  for (k = 0; k < 4; k++)
	    {
	      if (tabWalls[i][j][k])
		{
		  tmp++;

		}

	    }
	  if (tmp < 2)
	    {
	      for (k = 0; k < 4; k++)
		{
		  tabWalls[i][j][k] = 0;
		}
	    }
	}
    }
}


int
hasPath (char *block)
{

  if (block[NORTH] || block[EAST] || block[SOUTH] || block[WEST])
    {
      return 1;
    }
  return 0;

}

int
longestPath (int x, int y, char ***wall, int iteration)
{
  char bck = 0;
  int retour[4] = { 0 };
  int i;

  if (iteration < RECURS_MAX)
    {
      if (hasPath (wall[x][y]))
	{
	  if (wall[x][y][NORTH])
	    {
	      retour[NORTH] +=
		longestPath (x, y - 1, wall, iteration + 1) + 1;
	    }
	  if (wall[x][y][EAST])
	    {
	      retour[EAST] += longestPath (x + 1, y, wall, iteration + 1) + 1;
	    }
	  if (wall[x][y][SOUTH])
	    {
	      retour[SOUTH] +=
		longestPath (x, y + 1, wall, iteration + 1) + 1;
	    }
	  if (wall[x][y][WEST])
	    {
	      retour[WEST] += longestPath (x - 1, y, wall, iteration + 1) + 1;
	    }

	}
      else
	{
	  return 0;
	}
    }
  else
    {
/*
  if (retour[NORTH] > retour[SOUTH])
    {
      if (retour[EAST] > retour[WEST])
	{
	  return (retour[EAST] >
		  retour[NORTH] ? retour[EAST] : retour[NORTH]);
	}
      else
	{
	  return (retour[WEST] >
		  retour[NORTH] ? retour[WEST] : retour[NORTH]);
	}
    }
  else
    {
      if (retour[EAST] > retour[WEST])
	{
	  return (retour[EAST] >
		  retour[SOUTH] ? retour[EAST] : retour[NORTH]);
	}
      else
	{
	  return (retour[WEST] >
		  retour[SOUTH] ? retour[WEST] : retour[NORTH]);
	}
    }*/
    }
  bck = retour[0];
//  getMax
  for (i = 1; i < 4; i++)
    {
      if (bck < retour[i])
	{
	  bck = retour[i];
	}
    }

  return bck;
}

int
numberOfPossPath (t_canGo canGo)
{
  int retour = 0;
  if (canGo.right)
    retour++;
  if (canGo.down)
    retour++;
  if (canGo.up)
    retour++;
  if (canGo.left)
    retour++;
  return retour;
}


int
getFurtherPath (char ***tabWalls, t_gameBoundaries boundaries, int *tmpPos,
		int move)
{
  int result = 0;
  int pos[2];
  pos[0] = tmpPos[0];
  pos[1] = tmpPos[1];
  int guard;
  int iter;
  switch (move)
    {
    case EAST:
      guard = *boundaries.x;
      iter = pos[0];
      while (iter < guard)
	{
	  if (tabWalls[iter][pos[1]][1])
	    {
	      result++;
	      iter++;
	    }
	  else
	    {
	      iter = guard;
	    }

	}
      break;
    case WEST:
      guard = 0;
      iter = pos[0];
      while (iter > guard)
	{
	  if (tabWalls[iter][pos[1]][3])
	    {
	      result++;
	      iter--;
	    }
	  else
	    {
	      iter = guard;
	    }

	}
      break;
    case NORTH:
      guard = 0;
      iter = pos[1];

      while (iter > guard)
	{

	  if (tabWalls[pos[0]][iter][0])
	    {
	      result++;
	      iter--;
	    }
	  else
	    {
	      iter = guard;
	    }

	}
      break;
    case SOUTH:
      guard = *boundaries.y;
      iter = pos[1];
      while (iter < guard)
	{
	  if (tabWalls[pos[0]][iter][2])
	    {
	      result++;
	      iter++;
	    }
	  else
	    {
	      iter = guard;

	    }

	}
      break;
    default:
      break;
    }
  return result;
}



char getMaxFrom (int int1, int int2, int int3, int int4)
{
    
  if (int1 > int2)
    {

      if (int3 > int4)
	{
	  if (int3 > int1)
	    {
	      return 2;
	    }
	  else
	    {
	      return 0;
	    }
	}
      else
	{
	  if (int4 > int1)
	    {
	      return 3;
	    }
	  else
	    {
	      return 0;
	    }

	}

    }
  else
    {
      if (int3 > int4)
	{
	  if (int3 > int2)
	    {
	      return 2;
	    }
	  else
	    {
	      return 1;

	    }
	}
      else
	{
	  if (int4 > int2)
	    {
	      return 3;
	    }
	  else
	    {
	      return 1;

	    }

	}
    }
}

int
nbPath1 (char ***tabWalls, int x, int y, int x_size, int y_size)
{
  int retour = 0;
  int i;
  if (x < 0)
    {
      return 0;
    }
  if (y < 0)
    {
      return 0;
    }
  if (x >= x_size)
    {
      return 0;
    }
  if (y >= y_size)
    {
      return 0;
    }
  for (i = 0; i < 4; i++)
    {
      if (tabWalls[x][y][i])
	retour+=1;
    }
  /*
   *
   */
  return retour;

}

int
multiplePath (char ***tabWalls, t_canGo canGo, t_gameBoundaries boundaries,
	      int *tmpPos)
{
  int further[4] = {-1,-1,-1,-1 };
  int max;
  if (getNbOfPath (canGo) == 1)
    {

      max = 0;

      if (canGo.right)
	{
	  max = 1;
	}
      if (canGo.left)
	{
	  max = 3;
	}
      if (canGo.up)
	{
	  max = 0;

	}
      if (canGo.down)
	{
	  max = 2;
	}

    }
  else
    {

      if (canGo.right)
	{
	  further[1] = getFurtherPath (tabWalls, boundaries, tmpPos, EAST);
	}
      if (canGo.left)
	{
	  further[3] = getFurtherPath (tabWalls, boundaries, tmpPos, WEST);
	}
      if (canGo.up)
	{

	  further[0] = getFurtherPath (tabWalls, boundaries, tmpPos, NORTH);
	}
      if (canGo.down)
	{
	  further[2] = getFurtherPath (tabWalls, boundaries, tmpPos, SOUTH);
	}

      max = getMaxFrom (further[0], further[1], further[2], further[3]);

	  if ((further[max] < 3) && getNbOfPath(canGo)>1)
	{

	  if (canGo.right)
	    {
	      further[1] =
		nbPath1 (tabWalls, tmpPos[0] + 1, tmpPos[1], *boundaries.x,
			 *boundaries.y);
	      if (further[1] >=0)
		{
		  if (tabWalls[tmpPos[0] + 1][tmpPos[1]][1])
		    {
		      further[1] +=
			nbPath1 (tabWalls, tmpPos[0] + 2, tmpPos[1],
				 *boundaries.x, *boundaries.y);
		    }

		  if (tabWalls[tmpPos[0] + 1][tmpPos[1]][2])
		    {

		      further[1] +=
			nbPath1 (tabWalls, tmpPos[0] + 1, tmpPos[1] + 1,
				 *boundaries.x, *boundaries.y);
		    }
		  if (tabWalls[tmpPos[0] + 1][tmpPos[1]][0])
		    {

		      further[1] +=
			nbPath1 (tabWalls, tmpPos[0] + 1, tmpPos[1] - 1,
				 *boundaries.x, *boundaries.y);
		    }
		}
	    }
	  /*else
	    {
	      further[1] = -1;
	    }*/
	  if (canGo.left)
	    {
	      further[3] =
		nbPath1 (tabWalls, tmpPos[0] - 1, tmpPos[1], *boundaries.x,
			 *boundaries.y);
		printf("\nNB of path firstPath further[3] : %d", further[3]);
	      if (further[3] >= 0)
		{

		  if (tabWalls[tmpPos[0] - 1][tmpPos[1]][3])
		    {
		      further[3] +=
			nbPath1 (tabWalls, tmpPos[0] - 2, tmpPos[1],
				 *boundaries.x, *boundaries.y);
		    }
		  if (tabWalls[tmpPos[0] - 1][tmpPos[1]][2])
		    {
		      further[3] +=
			nbPath1 (tabWalls, tmpPos[0] - 1, tmpPos[1] + 1,
				 *boundaries.x, *boundaries.y);
		    }

		  if (tabWalls[tmpPos[0] - 1][tmpPos[1]][0])
		    {
		      further[3] +=
			nbPath1 (tabWalls, tmpPos[0] - 1, tmpPos[1] - 1,
				 *boundaries.x, *boundaries.y);
		    }
		}
	    }/*
	  else
	    {
	      further[3] = -1;
	    }*/
	  if (canGo.up)
	    {
	      further[0] =
		nbPath1 (tabWalls, tmpPos[0], tmpPos[1] - 1, *boundaries.x,
			 *boundaries.y);
	      if (further[0] >= 0)
		{
		  if (tabWalls[tmpPos[0]][tmpPos[1] - 1][0])
		    {
		      further[0] +=
			nbPath1 (tabWalls, tmpPos[0], tmpPos[1] - 2,
				 *boundaries.x, *boundaries.y);
		    }
		  if (tabWalls[tmpPos[0]][tmpPos[1] - 1][3])
		    {
		      further[0] =
			nbPath1 (tabWalls, tmpPos[0] - 1, tmpPos[1] - 1,
				 *boundaries.x, *boundaries.y);
		    }
		  if (tabWalls[tmpPos[0]][tmpPos[1] - 1][1])
		    {
		      further[0] +=
			nbPath1 (tabWalls, tmpPos[0] + 1, tmpPos[1] - 1,
				 *boundaries.x, *boundaries.y);
		}
		}
	    }/*
	  else
	    {
	      further[0] = -1;
	    }*/
	  if (canGo.down)
	    {
	      further[2] =
		nbPath1 (tabWalls, tmpPos[0], tmpPos[1] + 1, *boundaries.x,
			 *boundaries.y);
	      if (further[2] >= 0)
		{
		  if (tabWalls[tmpPos[0]][tmpPos[1] + 1][2])
		    {
		      further[2] +=
			nbPath1 (tabWalls, tmpPos[0], tmpPos[1] + 2,
				 *boundaries.x, *boundaries.y);
		    }
		  if (tabWalls[tmpPos[0]][tmpPos[1] + 1][1])
		    {
		      further[2] =
			nbPath1 (tabWalls, tmpPos[0] + 1, tmpPos[1] + 1,
				 *boundaries.x, *boundaries.y);
		    }
		  if (tabWalls[tmpPos[0]][tmpPos[1] + 1][3])
		    {
		      further[2] +=
			nbPath1 (tabWalls, tmpPos[0] - 1, tmpPos[1] + 1,
				 *boundaries.x, *boundaries.y);
		    }
		}
	    }/*
	  else
	    {
	      further[2] = -1;
	    }*/
	  max = getMaxFrom (further[0], further[1], further[2], further[3]);
	}
printf("\nFurther 0 is  %d",further[0]);
printf("\nFurther 1 is  %d",further[1]);
printf("\nFurther 2 is  %d",further[2]);
printf("\nFurther 3 is  %d",further[3]);
    }
  return max;
}


t_move
getNextMove (t_body allSnake, t_body advSnake,
	     int *position, t_gameBoundaries boundaries, t_move lastMove,
	     char ***tabWalls)
{
  t_move nextMove;
  int *tmpPos = malloc (sizeof (int) * 2);
  t_canGo canGo;
  initializeCanGo (&canGo);
  tmpPos[0] = position[0];
  tmpPos[1] = position[1];

  checkLastMove (&canGo, lastMove);

  checkBoundaries (boundaries, tmpPos, &canGo);
  printf("\n allSnake x : %d y : %d",allSnake.position[0],allSnake.position[1]);

  if (!tabWalls[allSnake.position[0]][allSnake.position[1]][1])
    canGo.right = 0;
  if (!(tabWalls[allSnake.position[0]][allSnake.position[1]][0]))
    canGo.up = 0;
  if (!tabWalls[allSnake.position[0]][allSnake.position[1]][3])
    canGo.left = 0;
  if (!(tabWalls[allSnake.position[0]][allSnake.position[1]][2]))
    canGo.down = 0;
  nextMove = decideMove (canGo);


  nextMove = multiplePath (tabWalls, canGo, boundaries, tmpPos);


  if ((tmpPos[1] == *(boundaries.y) - 1) && (tmpPos[0] == *(boundaries.x) - 1) ){
      if (lastMove==1)
          nextMove = NORTH;
	 else
	     nextMove = WEST;}
    
  if (!(tmpPos[0]) && (tmpPos[1]==*boundaries.y-1))
    {
      if (lastMove)
	nextMove = EAST;
      else
	nextMove = SOUTH;
    }
  if (!(tmpPos[0]) && (!tmpPos[1]))
    {
      if (!lastMove)
	nextMove = EAST;
      else
	nextMove = SOUTH;
    }
  free (tmpPos);
  return nextMove;
}

void
printTab (char ***tabWalls, int y_size, int x_size)
{
  int i, j, k;
  printf("\n");
  for (i = 0; i < y_size; i++)
    {
	for (j = 0; j < x_size; j++)
	  {
	    for (k = 3; k >= 0; k--)
	      {
		if (!tabWalls[j][i][k])
		  {
		    switch (k)
		      {
		      case 0:
			printf ("n");
			//printf ("%d", tabWalls[j][i][k]);
			break;
		      case 1:
			printf ("e");
			break;
		      case 2:
			printf ("s");
			break;
		      case 3:
			printf ("w");
			break;
		      }

		  }
		else
		  {
		    printf (" ");
		  }
	      }
	    printf ("*");
	  }
      printf ("\n");
    }
}

int
main (void)
{


  int x_size = STANDARD_X_SIZE;
  int y_size = STANDARD_Y_SIZE;
  t_gameBoundaries boundaries;
  boundaries.x = &x_size;
  boundaries.y = &y_size;

  int nbWalls;
  int *walls;
  char gameName[150];
    char gameType[150] = "SUPER_PLAYER difficulty=1 SEED=0 TIMEOUT=15";
  int hePlays;
  t_move lastMove = 0;
  t_move currentMove = 0;
  t_return_code allReturnCode = 0;
  t_return_code advReturnCode = 0;
  int *allCurrentPos = malloc (sizeof (int) * 2);
  int *advCurrentPos = malloc (sizeof (int) * 2);
  int nbTurnAll = 0;
  int nbTurnAdv = 0;
  t_body *advSnake = malloc (sizeof (t_body));
  t_body *allSnake = malloc (sizeof (t_body));
  char ***tabWalls;
  char ***saveWalls;
  int i, j, k;


  connectToServer (POLY_HOST, SERVER_PORT, PLAYER_NAME);

  waitForSnakeGame (gameType, gameName, boundaries.x, boundaries.y, &nbWalls);

  /* alloue les murs */
  walls = (int *) malloc (sizeof (int) * 4 * nbWalls);

  hePlays = getSnakeArena (walls);

  allReturnCode = advReturnCode = NORMAL_MOVE;
  initializePosition (boundaries, hePlays, advCurrentPos, allCurrentPos);

  tabWalls = malloc ((x_size + 1) * sizeof (char **));
  saveWalls = malloc ((x_size + 1) * sizeof (char **));
  for (i = 0; i < x_size; i++)
    {

      tabWalls[i] = malloc ((y_size) * sizeof (char *));
      saveWalls[i] = malloc ((y_size) * sizeof (char *));
      for (j = 0; j < y_size; j++)
	{
	  tabWalls[i][j] = (char *) malloc (4);
	  saveWalls[i][j] = (char *) malloc (4);
	  for (k = 0; k < 4; k++)
	    {
	      tabWalls[i][j][k] = 1;
	      saveWalls[i][j][k] = 1;
	    }
	}
    }


  getWalls (saveWalls, x_size, y_size, walls, nbWalls);
  printArena ();

  copyWallsTab (tabWalls, saveWalls, boundaries);

  initializeBodies (allSnake, advSnake, allCurrentPos, advCurrentPos,
		    tabWalls);
  updateSnake (allSnake, nbTurnAll, allCurrentPos, currentMove, tabWalls,
	       boundaries);
  updateSnake (advSnake, nbTurnAdv, advCurrentPos, lastMove, tabWalls,
	       boundaries);


  while (!(last_move (allReturnCode)) && (!(last_move (advReturnCode))))
    {
      if (hePlays)
	{
	  advReturnCode = getMove (&lastMove);
	  if (last_move (advReturnCode))
	    break;
	  updateSnake (advSnake, nbTurnAdv, advCurrentPos, lastMove, tabWalls,
		       boundaries);
	  nbTurnAdv++;
	}
      else
	{

	  updateSnake (allSnake, nbTurnAll, allCurrentPos, currentMove,
		       tabWalls, boundaries);
	  currentMove =
	    getNextMove (*allSnake, *advSnake, allCurrentPos,
			 boundaries, currentMove, tabWalls);
	   // printTab(tabWalls,*boundaries.y,*boundaries.x);
	  printf ("\n next Move is %d", currentMove);
	  printArena ();
	  allReturnCode = sendMove (currentMove);
	  nbTurnAll++;
	  copyWallsTab (tabWalls, saveWalls, boundaries);
	}
      hePlays = !hePlays;
    }
  if (allReturnCode == WINNING_MOVE || advReturnCode == LOOSING_MOVE)
    {
      printf ("\nC'est gagné !");
    }
  closeConnection ();

  for (i = 0; i < x_size; i++)
    {
      for (j = 0; j < y_size; j++)
	{
	  free (tabWalls[i][j]);
	  free (saveWalls[i][j]);
	}
      free (tabWalls[i]);
      free (saveWalls[i]);
    }
  free (tabWalls);
  free (saveWalls);
  freeASnake (allSnake);
  free (allCurrentPos);
  free (advCurrentPos);
  free (walls);
  freeASnake (advSnake);

  return EXIT_SUCCESS;
}
