#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include <windows.h>

#ifdef WIN32
#define CLS system("cls")
#else
#define CLS system("clear")
#endif

typedef struct Ships
{
	int size;
	int number;
	char name[20];

} ships;

typedef struct Data
{
	int difficulty;				
	float score;									
	int size_x;					
	int size_y;					
	int board[30][30];		
	int number;					
	int total;					
	int total_fields;			
	int lost;					
	int hit;					
	int moves;					
	int error;
	ships s1;
	ships s2;
	ships s3;
	ships s4;
	ships s5;

} data;


void CalculateFields(data *user);		
void CalculateShipNumber(data *user);	
void CreateGame(data *user);	
void InitializeBoard(data *user);	
void PrintInfo(data user);	
void ResetTurn(data *AI, int number, int size);		
void SetAIBoard(data *AI);		
void SetAIPosition(data *AI);	
void SetAICoordinates(data *AI, int size);	
void Initialize(data *user);	

void AIMove(data *user, data *AI);	
void CalculateDifficulty(data user, int nerf, int *x, int *y);	
void Difficulty(data user, data AI, int *x, int *y);	
int Hit(data *AI, int x, int y);	
void Search(data user, int *x, int *y);	


int main()
{
	int min=10000, max=0, avg=0, i = 10000,j = 9;
	while (j)
	{
	while (i)
	{
		data *AI1 = (data*)malloc(sizeof(data)), *AI2 = (data*)malloc(sizeof(data));

		Initialize(AI1);
		CreateGame(AI1);
		CalculateShipNumber(AI1);
		CalculateFields(AI1);
		CalculateFields(AI2);
		*AI2 = *AI1;
		InitializeBoard(AI1);
		InitializeBoard(AI2);
		SetAIBoard(AI1);
		SetAIBoard(AI2);

		int count = 0;

		while ((AI1->total_fields > 0) && (AI2->total_fields > 0))
		{
			count++;
			AIMove(AI1, AI2);
			if ((AI1->total_fields > 0) && (AI2->total_fields > 0))
				AIMove(AI2, AI1);
			else
				break;
		}

		avg += count;
		
		if (count > max)
			max = count;
		else if (count < min)
			min = count;

		free(AI1);
		free(AI2);
		i--;
	}

	printf("\nMIN: %i, MAX: %i, AVG: %f\n", min, max, avg/10000.0);
	j--;
	}
	getch();
	return 0;
}


void CalculateShipNumber(data *user)  
{
	int temp = 0;
	int surface;

	surface = 0.17*user->size_x*user->size_y;		

	while (surface>0)
	{


		if ((surface >= user->s5.size) && (surface >= 0))
		{
			surface -= user->s5.size;
			user->s5.number++;
			temp++;
		}

		if ((surface >= user->s4.size) && (surface >= 0))
		{
			surface -= user->s4.size;
			user->s4.number++;
			temp++;
		}

		if ((surface >= user->s3.size) && (surface >= 0))
		{
			surface -= user->s3.size;
			user->s3.number++;
			temp++;
		}

		if ((surface >= user->s2.size) && (surface >= 0))
		{
			surface -= user->s2.size;
			user->s2.number++;
			temp++;
		}

		if ((surface >= user->s1.size) && (surface >= 0))
		{
			surface -= user->s1.size;
			user->s1.number++;
			temp++;
		}

		if (surface<user->s1.size)
			break;

	}

	user->number = temp;
	user->total = temp;
}

void CalculateFields(data *user)			
{
	user->total_fields = 0;
	user->total_fields = user->s1.number*user->s1.size + user->s2.number*user->s2.size + user->s3.number*user->s3.size + user->s4.number*user->s4.size + user->s5.number*user->s5.size;
}

void CreateGame(data *user)   
{	
	/*************/
	user->size_x = 10;
	user->size_y = 10;
	user->difficulty = 3;
	/*************/
	user->score = 0;
}

void InitializeBoard(data *user)		
{
	int i, j;
	for (i = 0; i <= user->size_y; i++)
	for (j = 0; j <= user->size_x; j++)
		user->board[i][j] = 0;
}

void ResetTurn(data *AI, int number, int size)
{
	int i, j;

	for (j = 0; j<AI->size_y; j++)
	for (i = 0; i<AI->size_x; i++)
	if (AI->board[j][i] == number)
		AI->board[j][i] = 0;
}

void SetAICoordinates(data *AI, int size)
{

	long long /**/ int counter = 0, number = AI->number, temp_size = size, x, y, orientation, x_new, y_new, seed = rand() % 1000;
	srand(abs(time(NULL) + seed));	/**/

	do
	{
		x = rand() % AI->size_x;
		y = rand() % AI->size_y;
		orientation = rand() % 2;

		if (AI->board[y][x] == 0)
		{
			AI->board[y][x] = AI->number;
			size--;
		}

	} while (temp_size == size);


	x_new = x;
	y_new = y;

	while (size>0)
	{

		int direction = rand() % 2, temp_x = x_new, temp_y = y_new;

		if (counter>AI->size_x*AI->size_y)
		{
			ResetTurn(AI, number, temp_size);
			SetAICoordinates(AI, temp_size);
			//Sleep(1);
			return;
		}


		if (orientation)
			x_new += pow(-1, direction);
		else
			y_new += pow(-1, direction);

		/**/
		if ((x_new >= 0) && (x_new <= AI->size_x) && (y_new >= 0) && (y_new <= AI->size_y))
		{
			if ((AI->board[y_new][x_new] == 0) && (x_new >= 0) && (x_new < AI->size_x) && (y_new >= 0) && (y_new < AI->size_y) && (AI->board[temp_y][temp_x] == AI->number))
			{
				AI->board[y_new][x_new] = AI->number;
				size--;
			}
			else
				counter++;
		}
		else 
			counter++;
	}
	return;
}

void SetAIBoard(data *AI)
{
	while (AI->number>0)
	{
		SetAIPosition(AI);
		AI->number--;
	}
}

void SetAIPosition(data *AI)
{
	if (AI->s5.number != 0)
	{
		SetAICoordinates(AI, AI->s5.size);
		AI->s5.number--;
	}
	else if (AI->s4.number != 0)
	{
		SetAICoordinates(AI, AI->s4.size);
		AI->s4.number--;
	}
	else if (AI->s3.number != 0)
	{
		SetAICoordinates(AI, AI->s3.size);
		AI->s3.number--;
	}
	else if (AI->s2.number != 0)
	{
		SetAICoordinates(AI, AI->s2.size);
		AI->s2.number--;
	}
	else if (AI->s1.number != 0)
	{
		SetAICoordinates(AI, AI->s1.size);
		AI->s1.number--;
	}

}

void Initialize(data *user)   
{
	user->s1.size = 2;
	user->s2.size = 3;
	user->s3.size = 3;
	user->s4.size = 4;
	user->s5.size = 5;
	user->s1.number = 0;
	user->s2.number = 0;
	user->s3.number = 0;
	user->s4.number = 0;
	user->s5.number = 0;
	strcpy_s(user->s1.name, 20, "Destroyers");
	strcpy_s(user->s2.name, 20, "Submarines");
	strcpy_s(user->s3.name, 20, "Cruisers");
	strcpy_s(user->s4.name, 20, "Battleships");
	strcpy_s(user->s5.name, 20, "Carriers");
	user->hit = 0;
	user->lost = 0;
	user->moves = 0;
	user->score = 0.0;
	user->error = 0;
}


void CalculateDifficulty(data user, int nerf, int *x, int *y)
{
	int x_temp, y_temp, c = 2 * nerf, variable, coordinate_variable, seed = rand() % 1000;
	srand(time(NULL) + seed);

	do
	{
		variable = rand() % c;
		if (variable == (nerf - 1))
		{
			Search(user, &x_temp, &y_temp);
			if (user.difficulty != 4)
			{
				coordinate_variable = rand() % 4;
				switch (coordinate_variable)
				{
				case 0: x_temp++;   break;
				case 1: x_temp--;	break;
				case 2: y_temp++;	break;
				case 3: y_temp--;	break;
				default: x_temp++;
				}
			}

			while (x_temp<0)
				x_temp++;

			while (y_temp<0)
				y_temp++;

			while (x_temp >= user.size_x)
				x_temp--;

			while (y_temp >= user.size_y)
				y_temp--;

			*x = x_temp;
			*y = y_temp;

		}
		else
		{
			*x = rand() % user.size_x; *y = rand() % user.size_y;
		}

	} while ((user.board[*y][*x] == -1) || (user.board[*y][*x] == -2));
}

void Difficulty(data user, data AI, int *x, int *y)
{
	int x_temp, y_temp;

	switch (AI.difficulty)
	{
	case 1:
		do
		{
			*x = rand() % AI.size_x; *y = rand() % AI.size_y;
		} while ((user.board[*y][*x] == -1) || (user.board[*y][*x] == -2));		break;

	case 2: CalculateDifficulty(user, 2, &x_temp, &y_temp); *x = x_temp; *y = y_temp; break;

	case 3: CalculateDifficulty(user, 1, &x_temp, &y_temp); *x = x_temp; *y = y_temp; break;
	case 4:	CalculateDifficulty(user, 1, &x_temp, &y_temp); *x = x_temp; *y = y_temp; break;
	default:
		do
		{
			*x = rand() % AI.size_x; *y = rand() % AI.size_y; break;
		} while ((user.board[*y][*x] == -1) && (user.board[*y][*x] == -2));
	}

}

void Search(data user, int *x, int *y)
{
	long long /**/ int seed = rand() % 1000;
	srand(time(NULL) + seed);
	int i, j, k, variable, c;

	for (j = 0; j<user.size_y; j++)
	for (i = 0; i<user.size_x; i++)
	{
		if (user.board[j][i] >0)
		{
			goto exit;
		}
	}

exit:
	*x = i;
	*y = j;
}

void AIMove(data *user, data *AI)
{
	long long /**/ int seed = rand() % 1000;
	srand(time(NULL) + seed);
	int x, y;

	Difficulty(*user, *AI, &x, &y);

	switch (Hit(user, x, y))
	{
	case 2: break;
	case 0: break;
	case 1: AI->hit++; user->lost++; user->total_fields--; break;
	}

}

int Hit(data *AI, int x, int y)
{
	if (AI->board[y][x] == 0)		
	{
		AI->board[y][x] = -1;
		return 2;
	}
	else if (AI->board[y][x] == -1)	
	{
		return 0;
	}
	else if (AI->board[y][x] == -2)	
	{
		return 0;
	}
	else							
	{
		AI->board[y][x] = -2;
		return 1;
	}
}

