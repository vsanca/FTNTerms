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

/********************************/
/*STRUKTURE*/

typedef struct Ships
{
	int size;
	int number;
	char name[20];

} ships;
//0 - ~  , -1 - *, -2 - x
typedef struct Data
{
	int difficulty;				//tezina - preciznost pogadjanja AI
	float score;				//rezultat - preracunati mozda sa ukljucenom tezinom					
	int size_x;					//dimenzija x
	int size_y;					//dimenzija y
	int board[30][30];		//tabla
	int number;					//broj brodova - preracunat, vrednost se menja pri kreiranju table
	int total;					//ukupan broj brodova, sluzi za potezni deo igre
	int total_fields;			//ukupno polja sa brodovima
	int lost;					//izgubljeni brodovi
	int hit;					//broj pogodaka
	int moves;					//eventualno za neki mod igre gde pobedjuje onaj ko potopi vise brodova
	int error;
	ships s1;
	ships s2;
	ships s3;
	ships s4;
	ships s5;

} data;

/********************************/
/*ZAGLAVLJA FUNKCIJA I PROCEDURA*/

/*FUNKCIJE KREIRANJA TABLE*/

void CalculateFields(data *user);
void CalculateShipNumber(data *user);
void CreateGame(data *user);
void InitializeBoard(data *user);
void PrintAll(data user, data AI);
void PrintBoard(data user);
void PrintHidden(data AI);
void PrintInfo(data user);
void ResetTurn(data *AI, int number, int size);
void SetAIBoard(data *AI);
void SetAIPosition(data *AI);
void SetAICoordinates(data *AI, int size);
void SetCoordinates(data *user, int size);
void SetUserBoard(data *user);
void SetPosition(data *user);
void Initialize(data *user);

/*FUNKCIJE IGRE*/
void AIMove(data *user, data *AI);
void CalculateDifficulty(data user, int nerf, int *x, int *y);
void Difficulty(data user, data AI, int *x, int *y);
/*void*/int EndGame(data *user, data *AI);
int Hit(data *AI, int x, int y);
void ScoreUpdate(data *user);
void Search(data user, int *x, int *y);
void UserMove(data *user, data *AI);

/********************************/
/*MAIN*/

int main()
{
	data *AI1 = (data*)malloc(sizeof(data)), *AI2 = (data*)malloc(sizeof(data));
	int i = 100, j = 10;
	long int total_avg = 0;
	/**/while (j)/**/
	{
		int variable = 0;
		/**/while (i)/**/
		{
			Initialize(AI1);
			CreateGame(AI1);
			CalculateShipNumber(AI1);

			CalculateFields(AI1);
			CalculateFields(AI2);
			//printf("\n%i",AI1->number);
			//getch();
			*AI2 = *AI1;		//pocetni uslovi su isti
			InitializeBoard(AI1);
			InitializeBoard(AI2);
			//printf("\n%i",AI1->number);
			//getch();
			SetAIBoard(AI1);
			SetAIBoard(AI2);


			while ((AI1->total_fields>0) && (AI2->total_fields>0))
			{
				(AI1->moves)++;

				AIMove(AI1, AI2);
				if ((AI1->total_fields>0) && (AI2->total_fields>0))
					AIMove(AI2, AI1);    																//IMA GRESKU!!!

				if ((AI1->total_fields == 0) || (AI2->total_fields == 0))
				{
					variable += EndGame(AI1, AI2);
					break;
				}
			}
			//printf("\n%i. prolaz", i);
			i--;
		}
		total_avg += variable;
		printf("\nProsek:%lf", variable / 100.0);
		Sleep(1000);
		//sleep(1);
		i = 100;
		j--;
	}
	printf("\nUkupan Prosek:%lf", total_avg / 1000.0);
	/*printf("\nIz nekog razloga izadje ovde!");*/
	getchar();
	return 0;
}

/********************************/
/*FUNKCIJE I PROCEDURE*/

/*FUNKCIJE KREIRANJA TABLE*/

void CalculateShipNumber(data *user)  /*izracunava broj pojedinacnih brodova i ukupan broj brodova*/		//PROVERENA
{
	int temp = 0;
	int surface;

	surface = 0.17*user->size_x*user->size_y;		/*ovo je po originalu, treba smisliti koeficijent*/

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

void CalculateFields(data *user)			//PROVERENA
{
	user->total_fields = 0;
	user->total_fields = user->s1.number*user->s1.size + user->s2.number*user->s2.size + user->s3.number*user->s3.size + user->s4.number*user->s4.size + user->s5.number*user->s5.size;
}

void CreateGame(data *user)   /*ispisuje pocetnu poruku i prikuplja podatke na pocetku igre*/		//PROVERENA
{
	//printf("\n\tWelcome to Battleships!\n\n Insert board size (x y):\n");
	//scanf_s("%i %i",&user->size_x,&user->size_y);
	user->size_x = 10;
	user->size_y = 10;
	//printf("\n Select difficulty:\n\n \t1. Easy \n \t2. Medium \n\t3. Hard \n\t4. Insane\n");
	//scanf_s("%i",&user->difficulty);
	user->difficulty = 2;
	user->score = 0;
}

void InitializeBoard(data *user)		//PROVERENA
{
	int i, j;
	for (i = 0; i <= user->size_y; i++)
	for (j = 0; j <= user->size_x; j++)
		user->board[i][j] = 0;
}

void PrintAll(data user, data AI)		//PROVERENA
{
	CLS;
	int i, j;

	printf("\n");
	printf("\n\n\tScore: %f\n\n", user.score);
	PrintBoard(user);
	for (j = 0; j<2; j++)
	{
		printf("\n");
		for (i = 0; i<user.size_x * 5 + 2; i++)
			printf("=");
	}
	printf("\n");
	PrintBoard(AI);
	//PrintHidden(AI);
}

void PrintBoard(data user)		/*ispisuje tablu*/		//PROVERENA
{
	int i, j;

	printf("   ");

	for (i = 0; i<user.size_x; i++)
		printf("  %c  ", 'A' + i); 				//Voditi racuna o izlasku van opsega za slova!


	for (j = 0; j<user.size_y; j++)
	{
		printf("\n\n %i ", j + 1);
		for (i = 0; i<user.size_x; i++)
		{
			if (user.board[j][i] == 0)
				printf("  ~  ");
			else if (user.board[j][i] == -1)
				printf("  *  ");
			else if (user.board[j][i] == -2)
				printf("  x  ");
			else
				printf("  %i  ", user.board[j][i]);
		}
	}
}

void PrintHidden(data AI)						//PROVERENA
{
	int i, j;

	printf("   ");

	for (i = 0; i<AI.size_x; i++)
	if (i <= 25)
		printf("  %c  ", 'A' + i);				//Voditi racuna o izlasku van opsega za slova!


	for (j = 0; j<AI.size_y; j++)
	{
		printf("\n\n %i ", j + 1);
		for (i = 0; i<AI.size_x; i++)
		{
			if ((AI.board[j][i] != -1) && (AI.board[j][i] != -2))
				printf("  ~  ");
			else if (AI.board[j][i] != -1)
				printf("  x  ");
			else
				printf("  *  ");
		}
	}
}

void PrintInfo(data user)				//PROVERENA
{
	printf("\n\nYou have %i ships on disposal:", user.number);
	printf("\n\t%i %s(%i)", user.s5.number, user.s5.name, user.s5.size);
	printf("\n\t%i %s(%i)", user.s4.number, user.s4.name, user.s4.size);
	printf("\n\t%i %s(%i)", user.s3.number, user.s3.name, user.s3.size);
	printf("\n\t%i %s(%i)", user.s2.number, user.s2.name, user.s2.size);
	printf("\n\t%i %s(%i)", user.s1.number, user.s1.name, user.s1.size);
}

void ResetTurn(data *AI, int number, int size)		//PROVERENO
{
	int i, j;

	for (j = 0; j<AI->size_y; j++)
	for (i = 0; i<AI->size_x; i++)
	if (AI->board[j][i] == number)
		AI->board[j][i] = 0;
}

void SetAICoordinates(data *AI, int size)	//nasumicno rasporedjuje brodove po tabli   IMA POVREMENI BAG
{


	int counter = 0, number = AI->number, temp_size = size, x, y, orientation, x_new, y_new, seed = rand() % 1000;
	srand(time(NULL) + seed);


	do
	{
		x = rand() % AI->size_x;
		y = rand() % AI->size_y;
		orientation = rand() % 2;						//horizontal 1 vertical 0

		if (AI->board[y][x] == 0)
		{
			AI->board[y][x] = AI->number;
			size--;
		}

	} while (temp_size == size);					//trazi poziciju sve dok ne nadje slobodnu


	x_new = x;
	y_new = y;

	while (size>0)
	{

		int direction = rand() % 2, temp_x = x_new, temp_y = y_new;  //left - right +

		if (counter>AI->size_x*AI->size_y)
		{
			ResetTurn(AI, number, temp_size); //GRESKA!
			SetAICoordinates(AI, temp_size);
			return;
		}


		if (orientation)		//horizontal
			x_new += pow(-1, direction);
		else				//vertical
			y_new += pow(-1, direction);

		if ((AI->board[y_new][x_new] == 0) && (x_new >= 0) && (x_new<AI->size_x) && (y_new >= 0) && (y_new<AI->size_y) && (AI->board[temp_y][temp_x] == AI->number))
		{
			AI->board[y_new][x_new] = AI->number;
			size--;

		}
		else
			counter++;
	}
	return;
}

void SetCoordinates(data *user, int size)
{
	char x_char;
	int x, y, mark = size, x_new, y_new, orient = 0, n_orient;
	do
	{
		fflush(stdin);								//obavezno pre svakog unosa!
		printf("\nX(letter) ,Y(number)\n");
		scanf_s("%c %i", &x_char, &y);						//matrica[red][kolona]
		if ((x_char >= 'A') && (x_char <= 'Z'))
			x = x_char - 'A';									//vracanje na broj, pa na interni sistem koji krece od 0
		else
			x = x_char - 'a';

		y--;											//vracanje na interni sistem koji krece od pozicije 0		
		if (((x<0) || (x >= user->size_x)) || ((y<0) || (y >= user->size_y)))
			printf("\nWrong input, try again!\n");

	} while (((x<0) || (x >= user->size_x)) || ((y<0) || (y >= user->size_y)));


	if (user->board[y][x] == 0)
	{
		user->board[y][x] = user->number;
		size--;
	}
	CLS;
	PrintBoard(*user);

	while (size>0)
	{
		do
		{
			fflush(stdin);
			printf("\nNext coordinate: X(letter) Y(number)\n");
			scanf_s("%c %i", &x_char, &y_new);
			if ((x_char >= 'A') && (x_char <= 'Z'))
				x_new = x_char - 'A';
			else
				x_new = x_char - 'a';
			y_new--;

			if (((x_new<0) || (x_new >= user->size_x)) || ((y_new<0) || (y_new >= user->size_y)))
				printf("\nWrong input, try again!\n");

		} while (((x_new<0) || (x_new >= user->size_x)) || ((y_new<0) || (y_new >= user->size_y)));

		//stari uslov 	if(((user->board[y_new][x_new+1]==user->number)||(user->board[y_new][x_new-1]==user->number)||(user->board[y_new+1][x_new]==user->number)||(user->board[y_new-1][x_new]==user->number))&&(user->board[y_new][x_new]==0)&&(user->board[y][x]==user->number))

		//orientation 1-horizontalna  2-vertikalna
		if (orient == 0)
		{
			if (x_new != x)
				orient = 1;
			else
				orient = 2;
		}

		if (x_new != x)
			n_orient = 1;
		else
			n_orient = 2;

		if (((user->board[y_new][x_new + 1] == user->number) || (user->board[y_new][x_new - 1] == user->number) || (user->board[y_new + 1][x_new] == user->number) || (user->board[y_new - 1][x_new] == user->number)) && (user->board[y_new][x_new] == 0) && (n_orient == orient))
		{
			user->board[y_new][x_new] = user->number;
			size--;
			CLS;
			PrintBoard(*user);
			x = x_new;
			y = y_new;
		}
		else
			printf("\nIncorrect entry, please try again!\n");
	}
}

void SetAIBoard(data *AI)
{
	while (AI->number>0)
	{
		SetAIPosition(AI);
		AI->number--;
	}
}

void SetUserBoard(data *user)
{
	while (user->number>0)
	{
		SetPosition(user);
		user->number--;
	}
	printf("\n\n\t THE BATTLE BEGINS!");
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


void SetPosition(data *user)
{
	CLS;
	PrintBoard(*user);
	PrintInfo(*user);

	if (user->s5.number != 0)
	{
		printf("\nInsert Coordinates for %s:", user->s5.name);
		SetCoordinates(user, user->s5.size);
		user->s5.number--;
	}
	else if (user->s4.number != 0)
	{
		printf("\nInsert Coordinates for %s:", user->s4.name);
		SetCoordinates(user, user->s4.size);
		user->s4.number--;
	}
	else if (user->s3.number != 0)
	{
		printf("\nInsert Coordinates for %s:", user->s3.name);
		SetCoordinates(user, user->s3.size);
		user->s3.number--;
	}
	else if (user->s2.number != 0)
	{
		printf("\nInsert Coordinates for %s:", user->s2.name);
		SetCoordinates(user, user->s2.size);
		user->s2.number--;
	}
	else if (user->s1.number != 0)
	{
		printf("\nInsert Coordinates for %s:", user->s1.name);
		SetCoordinates(user, user->s1.size);
		user->s1.number--;
	}

}

void Initialize(data *user)   /*inicijalizuje parametre velicine brodova i njihovog broja na inicijalnu vrednost*/		//PROVERENO
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
	strcpy_s(user->s1.name,20, "Destroyers");
	strcpy_s(user->s2.name,20, "Submarines");
	strcpy_s(user->s3.name,20, "Cruisers");
	strcpy_s(user->s4.name,20, "Battleships");
	strcpy_s(user->s5.name,20, "Carriers");
	user->hit = 0;
	user->lost = 0;
	user->moves = 0;
	user->score = 0.0;
	user->error = 0;
}

/*FUNKCIJE IGRE*/

void CalculateDifficulty(data user, int nerf, int *x, int *y)						//poboljsati pogadjanje
{
	int x_temp, y_temp, c = 2 * nerf, variable, coordinate_variable, seed = rand() % 1000;
	srand(time(NULL) + seed);

	do
	{
		variable = rand() % c;
		if (variable == (nerf - 1))
		{
			Search(user, &x_temp, &y_temp);
			coordinate_variable = rand() % 4;
			switch (coordinate_variable)
			{
			case 0: x_temp++;   break; //x+
			case 1: x_temp--;	break; //x-
			case 2: y_temp++;	break; //y+
			case 3: y_temp--;	break; //y-
			default: x_temp++;
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

	} while ((user.board[*y][*x] == -1) || (user.board[*y][*x] == -2));											//trazi dok ne pronadjes neki na tabli
}

void Difficulty(data user, data AI, int *x, int *y)
{
	int x_temp, y_temp;
	switch (AI.difficulty)
	{
	case 1:
		do
		{
			*x = rand() % AI.size_x; *y = rand() % AI.size_y;    											//easy - just random
		} while ((user.board[*y][*x] == -1) || (user.board[*y][*x] == -2));		break;						//bez ponavljanja

	case 2: CalculateDifficulty(user, 2, &x_temp, &y_temp); *x = x_temp; *y = y_temp; break;				//medium - random + 0.25*0.25 chance to calculate	

	case 3: CalculateDifficulty(user, 1, &x_temp, &y_temp); *x = x_temp; *y = y_temp; break;				//hard - random +0.5*0.25 chance to calculate
	default:
		do
		{
			*x = rand() % AI.size_x; *y = rand() % AI.size_y; break;    									//easy - just random
		} while ((user.board[*y][*x] == -1) && (user.board[*y][*x] == -2));
	}

}

void Search(data user, int *x, int *y)			//trazi pogodjene lokacije i u odnosu na njih locira sledecu
{
	int seed = rand() % 1000;
	srand(time(NULL) + seed);
	int i, j, k, variable, c;
	//c=user.lost+1;
	//variable=rand()%c;

	for (j = 0; j<user.size_y; j++)
	for (i = 0; i<user.size_x; i++)
	{

		if ((user.board[j][i] >0)/*&&(variable==0)*/)
		{
			goto exit;
		}
		/*else
		{
			variable--;
		}*/
	}
exit:
	*x = i;
	*y = j;
}

void AIMove(data *user, data *AI)
{
	int seed = rand() % 1000;
	//PrintAll(*user,*AI);
	srand(time(NULL) + seed);
	int x, y;

	Difficulty(*user, *AI, &x, &y);		//postoji greska prilikom pristupa, na nivoima visim od 1  -- UPDATE - vise ne bi trebalo da je bude

	switch (Hit(user, x, y))
	{
	case 2: break;
	case 0: /*AIMove(user,AI);*/ break;
	case 1: AI->hit++; user->lost++; user->total_fields--; ScoreUpdate(AI); break;	//eventualno ubaciti mogucnost Salvo moda, kada se nakon pogotka opet gadja
	}

}

void UserMove(data *user, data *AI)
{
	PrintAll(*user, *AI);
	char x_temp;
	int x, y;

	do
	{
		printf("\nSink coordinates: X(letter) Y(number)\n");
		fflush(stdin);
		scanf_s("%c %i", &x_temp, &y);
		if ((x_temp >= 'A') && (x_temp <= 'Z'))
			x = x_temp - 'A';
		else
			x = x_temp - 'a';
		y--;

		if (((x<0) || (x >= user->size_x)) || ((y<0) || (y >= user->size_y)))
			printf("\nWrong input, try again!\n");
	} while (((x<0) || (x >= user->size_x)) || ((y<0) || (y >= user->size_y)));


	user->moves++;

	switch (Hit(AI, x, y))
	{
	case 2: printf("\nMISS!\n"); break;
	case 0: printf("\nAlready hit!\n"); break;
	case 1: user->hit++; AI->lost++; AI->total_fields--; ScoreUpdate(user); printf("\nHIT!\n"); break;		//eventualno ubaciti mogucnost Salvo moda, kada se nakon pogotka opet gadja
	}

}

int Hit(data *AI, int x, int y)
{
	//printf("\nx,y: %i,%i",x,y);
	if (AI->board[y][x] == 0)		//promasaj
	{
		AI->board[y][x] = -1;
		return 2;
	}
	else if (AI->board[y][x] == -1)	//vec je gadjano polje
	{
		return 0;
	}
	else if (AI->board[y][x] == -2)	//vec je pododjeno polje
	{
		return 0;
	}
	else							//pogodak
	{
		AI->board[y][x] = -2;
		return 1;
	}
}

void ScoreUpdate(data *user)
{
	user->score += 1;
}

/*void*/int EndGame(data *user, data *AI)		//zavrsava igru, proverava pobednika, ispisuje poene
{

	//printf("\nPlay again?\n");
	if (user->moves>AI->moves)
		//printf("\nSteps: %i",user->moves);
		return user->moves;
	else
		//printf("\nSteps: %i",AI->moves);
		return AI->moves;
	//getch();
}
