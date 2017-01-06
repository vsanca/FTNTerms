#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
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
	int board[255][255];		//tabla
	int number;					//broj brodova - preracunat, vrednost se menja pri kreiranju table
	int total;					//ukupan broj brodova, sluzi za potezni deo igre
	int total_fields;			//ukupno polja
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
void PrintAll(data user,data AI);
void PrintBoard(data user);
void PrintHidden(data AI);
void PrintInfo(data user);
void ResetTurn(data *AI,int number,int size);
void SetAIBoard(data *AI);
void SetAIPosition(data *AI);
void SetAICoordinates(data *AI,int size);
void SetCoordinates(data *user,int size);
void SetUserBoard(data *user);
void SetPosition(data *user);
void Initialize(data *user);

/*FUNKCIJE IGRE*/
void AIMove(data *user,data *AI);
void CalculateDifficulty(data user,int nerf,int *x,int *y);
void Difficulty(data user,data AI,int *x,int *y);
void EndGame(data *user,data *AI);
int Hit(data *AI,int x,int y);
void ScoreUpdate(data *user);
void Search(data user,int *x,int *y);
void UserMove(data *user,data *AI);

/********************************/
/*MAIN*/

int main()
{
	data user,AI;
	Initialize(&user); 
	CreateGame(&user);
	CalculateShipNumber(&user);
	CalculateFields(&user);
	CalculateFields(&AI);
	AI=user;		//pocetni uslovi su isti
	InitializeBoard(&user);
	InitializeBoard(&AI);
	SetUserBoard(&user);
	SetAIBoard(&AI);
	
	while((user.total_fields>0)&&(AI.total_fields>0))
	{
	UserMove(&user,&AI);
	if((user.total_fields>0)&&(AI.total_fields>0))
	AIMove(&user,&AI);    //IMA GRESKU!!!
	else
	EndGame(&user,&AI);
	}
	return 0;
}

/********************************/
/*FUNKCIJE I PROCEDURE*/

/*FUNKCIJE KREIRANJA TABLE*/

void CalculateShipNumber(data *user)  /*izracunava broj pojedinacnih brodova i ukupan broj brodova*/		//PROVERENA
{
	int temp=0;
	int surface;
	
	surface=0.17*user->size_x*user->size_y;		/*ovo je po originalu, treba smisliti koeficijent*/
	
	while(surface>0)
	{
		
	if((surface>=user->s5.size)&&(surface>=0))
		{
			surface-=user->s5.size;
			user->s5.number++;
			temp++;
		}
		
	if((surface>=user->s4.size)&&(surface>=0))
		{
			surface-=user->s4.size;
			user->s4.number++;
			temp++;
		}
	
	if((surface>=user->s3.size)&&(surface>=0))
		{
			surface-=user->s3.size;
			user->s3.number++;
			temp++;
		}
		
	if((surface>=user->s2.size)&&(surface>=0))
		{
			surface-=user->s2.size;
			user->s2.number++;
			temp++;
		}
	
	if((surface>=user->s1.size)&&(surface>=0))
		{
			surface-=user->s1.size;
			user->s1.number++;
			temp++;
		}
		
	if(surface<user->s1.size)
		break;	
		
	}
	
	user->number=temp;
	user->total=temp;
}

void CalculateFields(data *user)			//PROVERENA
{
	user->total_fields=0;
	user->total_fields=user->s1.number*user->s1.size+user->s2.number*user->s2.size+user->s3.number*user->s3.size+user->s4.number*user->s4.size+user->s5.number*user->s5.size;
}

void CreateGame(data *user)   /*ispisuje pocetnu poruku i prikuplja podatke na pocetku igre*/		//PROVERENA
{
	printf("\n\tWelcome to Battleships!\n\n Insert board size (x y):\n");
	scanf("%i %i",&user->size_x,&user->size_y);
	printf("\n Select difficulty:\n\n \t1. Easy \n \t2. Medium \n\t3. Hard \n\t4. Insane\n");
	scanf("%i",&user->difficulty);

	user->score=0;
}

void InitializeBoard(data *user)
{
	int i,j;
	for(i=0;i<=user->size_y;i++)
	for(j=0;j<=user->size_x;j++)
	user->board[i][j]=0;
}

void PrintAll(data user,data AI)
{
	CLS;
	int i,j;
	
	printf("\n");
	printf("\n\n\tScore: %f\n\n",user.score);
	PrintBoard(user);
	for(j=0;j<2;j++)
	{
	printf("\n");
	for(i=0;i<user.size_x*5+2;i++)
	printf("=");
	}
	printf("\n");
	PrintBoard(AI);
	//PrintHidden(AI);
}

void PrintBoard(data user)		/*ispisuje tablu*/
{
	int i,j;
	
	printf("   ");
	
	for(i=0;i<user.size_x;i++)
		printf("  %c  ",'A'+i); 				//Voditi racuna o izlasku van opsega za slova!
	
	
	for(j=0;j<user.size_y;j++)
	{
	printf("\n\n %i ",j+1);
	for(i=0;i<user.size_x;i++)
	{
	if(user.board[j][i]==0)
	printf("  ~  ");
	else if(user.board[j][i]==-1)
	printf("  *  ");
	else if(user.board[j][i]==-2)
	printf("  x  ");
	else
	printf("  %i  ",user.board[j][i]);
	}
	}
}

void PrintHidden(data AI)
{
	int i,j;
	
	printf("   ");
	
	for(i=0;i<AI.size_x;i++)
		printf("  %c  ",'A'+i); 				//Voditi racuna o izlasku van opsega za slova!
	
	
	for(j=0;j<AI.size_y;j++)
	{
	printf("\n\n %i ",j+1);
	for(i=0;i<AI.size_x;i++)
	{
	if((AI.board[j][i]!=-1)&&(AI.board[j][i]!=-2))
	printf("  ~  ");
	else if(AI.board[j][i]!=-1)
	printf("  x  ");
	else
	printf("  *  ");
	}
	}
}

void PrintInfo(data user)
{
	printf("\n\nYou have %i ships on disposal:",user.number);
	printf("\n\t%i %s(%i)",user.s5.number,user.s5.name,user.s5.size); 
	printf("\n\t%i %s(%i)",user.s4.number,user.s4.name,user.s4.size);
	printf("\n\t%i %s(%i)",user.s3.number,user.s3.name,user.s3.size);
	printf("\n\t%i %s(%i)",user.s2.number,user.s2.name,user.s2.size);
	printf("\n\t%i %s(%i)",user.s1.number,user.s1.name,user.s1.size);
}

void ResetTurn(data *AI,int number,int size)
{
	int i,j;
	
	for(j=0;j<AI->size_y;j++)
	for(i=0;i<AI->size_x;i++)
	if(AI->board[j][i]==number)
	AI->board[j][i]=0;
}

void SetAICoordinates(data *AI,int size)	//nasumicno rasporedjuje brodove po tabli   IMA POVREMENI BAG
{
	
	int counter=0,number=AI->number,temp_size=size,x,y,orientation,x_new,y_new;
	srand(time(NULL));
	
	
	do
	{
	x=rand()%AI->size_x;
	y=rand()%AI->size_y;
	orientation=rand()%2;						//horizontal 1 vertical 0
	
	if(AI->board[y][x]==0)
	{
		AI->board[y][x]=AI->number;
		size--;
	}
	
	}while(temp_size==size);					//trazi poziciju sve dok ne nadje slobodnu
	
	
	x_new=x;
	y_new=y;
		
		while(size>0)
		{
		
		int direction=rand()%2,temp_x=x_new,temp_y=y_new;  //left - right +
		
		if(counter>AI->size_x*AI->size_y)
		{
		ResetTurn(AI,number,temp_size);
		SetAICoordinates(AI,temp_size);
		return;
		}
	
	
		if(orientation)		//horizontal
		x_new+=pow(-1,direction);
		else				//vertical
		y_new+=pow(-1,direction);
		
		if ((AI->board[y_new][x_new]==0)&&(x_new>=0)&&(x_new<AI->size_x)&&(y_new>=0)&&(y_new<AI->size_y)&&(AI->board[temp_y][temp_x]==AI->number))
		{
		AI->board[y_new][x_new]=AI->number;
		size--;	
		
		}
		else
		counter++;
		}
}

void SetCoordinates(data *user,int size)
{
	char x_char;
	int x,y,mark=size;
	fflush(stdin);								//obavezno pre svakog unosa!
	printf("\nX(letter) ,Y(number)\n");
	scanf("%c %i",&x_char,&y);						//matrica[red][kolona]
	x=x_char-'A';									//vracanje na broj, pa na interni sistem koji krece od 0
	y--;											//vracanje na interni sistem koji krece od pozicije 0
	

	
	if(user->board[y][x]==0)
	{
		user->board[y][x]=user->number;
		size--;
	}
	CLS;
	PrintBoard(*user);
	
	while(size>0)
	{
		fflush(stdin);	
		printf("\nNext coordinate: X(letter) Y(number)\n");
		scanf("%c %i",&x_char,&y);
		x=x_char-'A';	
		y--;
		
		if(((user->board[y][x+1]==user->number)||(user->board[y][x-1]==user->number)||(user->board[y+1][x]==user->number)||(user->board[y-1][x]==user->number))&&user->board[y][x]==0)
		{
		user->board[y][x]=user->number;
		size--;
		CLS;
		PrintBoard(*user);
		}else 
		printf("\nIncorrect entry, please try again!\n");
	}
}

void SetAIBoard(data *AI)
{
	while(AI->number>0)
	{
	SetAIPosition(AI);
	AI->number--;
	}
}

void SetUserBoard(data *user)
{
	while(user->number>0)
	{
	SetPosition(user);
	user->number--;
	}
	printf("\n\n\t THE BATTLE BEGINS!");
}

void SetAIPosition(data *AI)
{
	
	if(AI->s5.number!=0)
	{
	SetAICoordinates(AI,AI->s5.size);
	AI->s5.number--;
	}
	else if(AI->s4.number!=0)
	{
	SetAICoordinates(AI,AI->s4.size);
	AI->s4.number--;
	}
	else if(AI->s3.number!=0)
	{
	SetAICoordinates(AI,AI->s3.size);
	AI->s3.number--;
	}
	else if(AI->s2.number!=0)
	{
	SetAICoordinates(AI,AI->s2.size);
	AI->s2.number--;
	}
	else if(AI->s1.number!=0)
	{
	SetAICoordinates(AI,AI->s1.size);
	AI->s1.number--;
	}
	
}


void SetPosition(data *user)
{
	CLS;
	PrintBoard(*user);
	PrintInfo(*user);
	
	if(user->s5.number!=0)
	{
	printf("\nInsert Coordinates for %s:",user->s5.name);
	SetCoordinates(user,user->s5.size);
	user->s5.number--;
	}
	else if(user->s4.number!=0)
	{
	printf("\nInsert Coordinates for %s:",user->s4.name);
	SetCoordinates(user,user->s4.size);
	user->s4.number--;
	}
	else if(user->s3.number!=0)
	{
	printf("\nInsert Coordinates for %s:",user->s3.name);
	SetCoordinates(user,user->s3.size);
	user->s3.number--;
	}
	else if(user->s2.number!=0)
	{
	printf("\nInsert Coordinates for %s:",user->s2.name);
	SetCoordinates(user,user->s2.size);
	user->s2.number--;
	}
	else if(user->s1.number!=0)
	{
	printf("\nInsert Coordinates for %s:",user->s1.name);
	SetCoordinates(user,user->s1.size);
	user->s1.number--;
	}
	
}

void Initialize(data *user)   /*inicijalizuje parametre velicine brodova i njihovog broja na inicijalnu vrednost*/
{
	user->s1.size=2;
	user->s2.size=3;
	user->s3.size=3;
	user->s4.size=4;
	user->s5.size=5;
	user->s1.number=0;
	user->s2.number=0;
	user->s3.number=0;
	user->s4.number=0;
	user->s5.number=0;
	strcpy(user->s1.name,"Destroyers");
	strcpy(user->s2.name,"Submarines");
	strcpy(user->s3.name,"Cruisers");
	strcpy(user->s4.name,"Battleships");
	strcpy(user->s5.name,"Carriers");
	user->hit=0;
	user->lost=0;
	user->moves=0;
	user->score=0.0;
	user->error=0;
}

/*FUNKCIJE IGRE*/

void CalculateDifficulty(data user,int nerf,int *x,int *y)
{

	int x_temp,y_temp,c=2*nerf,variable,coordinate_variable;
	srand(time(NULL));
	
	variable=rand()%(c); 
			if(variable==nerf)
			{ 
				printf("USAO!");
				getch();
				Search(user,&x_temp,&y_temp);		//GRESKA JE U FUNKCIJI!
			
				coordinate_variable=rand()%(c);
				*x=x_temp;
				*y=y_temp;
				switch(coordinate_variable)
				{
					case 1: *(x++);break;	//x+
					case 2: *(x--);break;	//x-
					case 3: *(y++);break;	//y+
					case 4: *(y--);break;	//y-
				}
				printf("IZASAO!");
				getch();
				return;
			}	
			else
			{
				*x=rand()%user.size_x; *y=rand()%user.size_y;
				return;
			}
	
}

void Difficulty(data user,data AI,int *x,int *y)
{
	int x_temp,y_temp;
	switch(AI.difficulty)
	{
		case 1: *x=rand()%AI.size_x; *y=rand()%AI.size_y; break;    //easy - just random
		case 2: CalculateDifficulty(user,2,&x_temp,&y_temp); break;				//medium - random + 0.25*0.25 chance to calculate	
		case 3: CalculateDifficulty(user,1,&x_temp,&y_temp); break;				//hard - random +0.5*0.25 chance to calculate
	}
	
	*x=x_temp;
	*y=y_temp;
	
}

void Search(data user,int *x,int *y)			//trazi pogodjene lokacije i u odnosu na njih locira sledecu IMA GRESKU!
{
	srand(time(NULL));
	
	int i,j,k,c=user.lost,variable=rand()%c;
	printf("USAO1!");
				getch();
	
	for(j=0;j<user.size_y;j++)
	for(i=0;i<user.size_x;i++)
	{
	
	if((user.board[j][i]==-2)&&(variable==0))
		{
			*x=i;*y=j;
			return;
		}
		else
		{
			variable--;
		}
	}
	
	return;
}

void AIMove(data *user,data *AI)		
{
	PrintAll(*user,*AI);
	srand(time(NULL));
	int x,y;
	
	Difficulty(*user,*AI,&x,&y);		//postoji greska prilikom pristupa, na nivoima visim od 1
	
	switch(Hit(user,x,y))
	{
		case 2: break;
		case 0: break;
		case 1: AI->hit++; user->lost++; user->total_fields--; ScoreUpdate(AI); break;	//eventualno ubaciti mogucnost Salvo moda, kada se nakon pogotka opet gadja
	}
	
}

void UserMove(data *user,data *AI)
{
	
	PrintAll(*user,*AI);
	char x_temp;
	int x,y;
	fflush(stdin);
	printf("\nSink coordinates: X(letter) Y(number)\n");
	fflush(stdin);
	scanf("%c %i",&x_temp,&y);
	x=x_temp-'A';
	y--;
	
	user->moves++;
	
	switch(Hit(AI,x,y))
	{
		case 2: printf("\nMISS!\n");break;
		case 0: printf("\nAlready hit!\n"); break;
		case 1: user->hit++; AI->lost++; AI->total_fields--; ScoreUpdate(user); printf("\nHIT!\n");break;		//eventualno ubaciti mogucnost Salvo moda, kada se nakon pogotka opet gadja
	}
	
}

int Hit(data *AI,int x,int y)
{
	if(AI->board[y][x]==0)		//promasaj
	{
		AI->board[y][x]=-1;
		return 2;
	}
	else if(AI->board[y][x]==-1)	//vec je gadjano polje
	{
		return 0;
	}
	else if(AI->board[y][x]==-2)	//vec je pododjeno polje
	{
		return 0;
	}
	else							//pogodak
	{
		AI->board[y][x]=-2;
		return 1;
	}
}

void ScoreUpdate(data *user)
{
	user->score+=1;
}

void EndGame(data *user,data *AI)		//zavrsava igru, proverava pobednika, ispisuje poene
{
	printf("Play again?");
}
