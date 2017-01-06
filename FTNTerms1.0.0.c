/*******************************************************************************|
|  						                                |
|	FTN Terms is a spinoff of a popular board game Battleships.	        |
|	For full functionality and detailed information consult the	        |
|	manual provided with the game. This is the main source code	        |
|	containing back end, as well as part of the front end 		        |
|	code implementation from the custom library menu.h.		        |
|									        |
|	Copyright  © 2014. Viktor Šanca	<viktor.sanca@gmail.com>	        |
|    									        |
|	This program is free software: you can redistribute it and/or modify	|
|	it under the terms of the GNU General Public License as published by	|
|	the Free Software Foundation, either version 3 of the License, or	|
|       any later version.				                        |
|				                                                |
|	This program is distributed in the hope that it will be useful,  	|
|	but WITHOUT ANY WARRANTY; without even the implied warranty of		|
|	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		|
|	GNU General Public License for more details.		                |
|							                        |
|	You should have received a copy of the GNU General Public License	|
|	along with this program.  If not, see <http://www.gnu.org/licenses/> 	|
|					                                        |
|*******************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include "menu.h"

typedef struct Wins
{
	WINDOW *Main;
	WINDOW *User;
	WINDOW *AI;
}wndws;

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
	int cx;
	int cy;
	ships s1;
	ships s2;
	ships s3;
	ships s4;
	ships s5;
	wndws windows;
    	int color;
    	char message_1[50];
    	char message_2[50];
    	char message_3[50];
    	char difficulty1[50];
   	char difficulty2[50];
   	char difficulty3[50];
	char difficulty4[50];
   	char inscoord_text[50];
    	char pinfo[50];
    	char winput_message[50];
    	char score_text[50];
    	char nextcoord_message[50];
    	char resize_message[50];
    	char nextmove_message[50];
    	char miss[50];
    	char hit_message[50];
    	char already_hit[50];
    	char insert_name[50];
    	char message_lost[50];
    	char message_won[50];
    	char message_score[50];
    	char play_again[50];
    	char name[50];
    	char language[50];
    	char manual[50];
    	char game[50];
} data;

int InitializeGame(menu *Main);
int ReInit(data *user);

void CalculateFields(data *user);
void CalculateShipNumber(data *user);
void CreateGame(data *user);
void InitializeBoard(data *user);
void PrintAll(data *user, data *AI);
void PrintBoard(data *user,int y, int x);
void PrintHidden(data *AI,int y, int x);
void PrintInfo(data user);
void ResetTurn(data *AI, int number, int size);
void SetAIBoard(data *AI);
void SetAIPosition(data *AI);
void SetAICoordinates(data *AI, int size);
void SetCoordinates(data *user, int size);
void SetUserBoard(data *user);
void SetPosition(data *user);
void Initialize(data *user,menu *Main,FILE *in);


void AIMove(data *user, data *AI);
void CalculateDifficulty(data user, int nerf, int *x, int *y);
void Difficulty(data user, data AI, int *x, int *y);
void EndGame(data *user, data *AI);
int Hit(data *AI, int x, int y);
void ScoreUpdate(data *user,int sign);
void Search(data user, int *x, int *y);
void UserMove(data *user, data *AI);
void WriteHighScore(data *user);


MEVENT mev,nmev,n2mev;


int main()
{
    menu *Main=(menu*)malloc(sizeof(menu));
    strcpy(Main->language,"eng.txt");
    strcpy(Main->manual,"manualeng.txt");
    strcpy(Main->game,"gameeng.txt");
    Main->color=2;

    InitializeMain(Main);
}

int ReInit(data *user)
{
    menu *Main=(menu*)malloc(sizeof(menu));

    strcpy(Main->language,user->language);
    strcpy(Main->manual,user->manual);
    strcpy(Main->game,user->game);
    Main->color=user->color;

    InitializeMain(Main);
}

int InitializeGame(menu *Main)
{

    	ClearAll(Main);
    	refresh();

    	initscr();
    	start_color();
    	init_pair(1,COLOR_WHITE,COLOR_BLUE);
	bkgd(COLOR_PAIR(1));
    	cbreak();
	noecho();
    	keypad(stdscr,TRUE);
    	refresh();

    	curs_set(1);
    	data *user = (data*)malloc(sizeof(data)), *AI = (data*)malloc(sizeof(data));
    	FILE *in;

    	strcpy(user->language,Main->language);
    	strcpy(user->manual,Main->manual);
    	strcpy(user->game,Main->game);

	Initialize(user,Main,in);
	CreateGame(user);
	CalculateShipNumber(user);
	CalculateFields(user);
	user->total=user->total_fields;
	CalculateFields(AI);
	*AI = *user;
	InitializeBoard(user);
	InitializeBoard(AI);
	SetUserBoard(user);
	SetAIBoard(AI);


	while ((user->total_fields>0) && (AI->total_fields>0))
	{
		UserMove(user, AI);
		if ((user->total_fields>0) && (AI->total_fields>0))
			AIMove(user, AI);
		else
			EndGame(user, AI);
	}
	EndGame(user, AI);
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
    refresh();
    echo();
	__fpurge(stdin);
	WINDOW *temp;
	bool check=FALSE;

	 do
    {
        temp=user->windows.Main;
        temp=newwin(0,0,0,0);
        init_pair(1,COLOR_WHITE,COLOR_BLUE);
        wbkgd(temp,COLOR_PAIR(1));
        box(temp,0,0);
        mvwprintw(temp,2,(COLS-strlen(user->message_1))/2,"%s",user->message_1);
        mvwprintw(temp,4,(COLS-strlen(user->message_2))/2,"%s",user->message_2);
        box(temp,0,0);

        if(check==TRUE)
            mvwprintw(temp,10,(COLS-16)/2,"4<X<26    4<Y<26");

        nocbreak();
        mvwprintw(temp,6,COLS/2-4,"X:");
        __fpurge(stdin);
        wrefresh(temp);
        mvwscanw(temp,6,COLS/2,"%i", &user->size_x);
        wrefresh(temp);
        mvwprintw(temp,7,COLS/2-4,"Y:");
        __fpurge(stdin);
        mvwscanw(temp,7,COLS/2,"%i", &user->size_y);

        if((user->size_x<5)||(user->size_x>26)||(user->size_y<5)||(user->size_y>26))
        check=TRUE;

    }while((user->size_x<5)||(user->size_x>26)||(user->size_y<5)||(user->size_y>26));


    keypad(temp,TRUE);
    noecho();
    cbreak();
    __fpurge(stdin);
    int buffer;
    int counter=0;
    wclear(temp);

    curs_set(0);

    bool click=FALSE;

    do
    {
        int temp_cols,temp_lines;
        getmaxyx(temp,temp_lines,temp_cols);

        wattroff(temp,A_REVERSE);
        mvwprintw(temp,4,(temp_cols-strlen(user->message_3))/2,"%s",user->message_3);

        if (counter==0)
            wattron(temp,A_REVERSE);
        else
        {
            wattroff(temp,A_REVERSE);
            wrefresh(temp);
        }

        mvwprintw(temp,6,(temp_cols-strlen(user->message_3))/2,"%s",user->difficulty1);

        if (counter==1)
            wattron(temp,A_REVERSE);
        else
        {
            wattroff(temp,A_REVERSE);
            wrefresh(temp);
        }

        mvwprintw(temp,7,(temp_cols-strlen(user->message_3))/2,"%s",user->difficulty2);


        if (counter==2)
            wattron(temp,A_REVERSE);
        else
        {
            wattroff(temp,A_REVERSE);
            wrefresh(temp);
        }

        mvwprintw(temp,8,(temp_cols-strlen(user->message_3))/2,"%s",user->difficulty3);

        if (counter==3)
            wattron(temp,A_REVERSE);
        else
        {
            wattroff(temp,A_REVERSE);
            wrefresh(temp);
        }

        mvwprintw(temp,9,(temp_cols-strlen(user->message_3))/2,"%s",user->difficulty4);

        wattroff(temp,A_REVERSE);
        box(temp,0,0);
        wrefresh(temp);
        buffer=getch();

        if(buffer==KEY_DOWN)
            counter++;
        else if(buffer==KEY_UP)
            counter--;

        if(counter>3)
        {
            counter-=4;
            wattroff(temp,A_REVERSE);
        }
        else if(counter<0)
        {
            counter+=4;
            wattroff(temp,A_REVERSE);
        }

        mousemask(BUTTON1_CLICKED, 0);


        if((buffer==KEY_MOUSE)&&(getmouse(&mev) == OK))
        {
            if((mev.x>=(temp_cols-strlen(user->message_3))/2)&&(mev.x<=(temp_cols-strlen(user->message_3))/2+10))
            {
                if((mev.y>=6)&&(mev.y<=9))
                    click=TRUE;
            }
        }



	}while((buffer!='\n')&&(click==FALSE));

    if(click==FALSE)
    {
        user->difficulty=counter+1;
	}
	else
	{
        user->difficulty=mev.y-5;
	}

	clear();
	user->score = 0;

}

void InitializeBoard(data *user)
{
	int i, j;
	for (i = 0; i <= user->size_y; i++)
        for (j = 0; j <= user->size_x; j++)
            user->board[i][j] = 0;
}

void PrintAll(data *user, data *AI)
{
	int i, j;
	mvprintw(user->size_y+5,3*user->size_x,"%s",user->score_text);
	mvprintw(user->size_y+5,3*user->size_x+strlen(user->score_text),"%.2f",user->score);
	PrintBoard(user,2,2);
	PrintHidden(AI,2,3*user->size_x+7);
	refresh();
}

void PrintBoard(data *user,int y,int x)
{
    WINDOW *temp;

    temp=user->windows.User;
	temp=newwin(user->size_y+3,3*user->size_x+5,y,x);
	init_pair(1,COLOR_WHITE,COLOR_BLUE);
    wbkgd(temp,COLOR_PAIR(1));

    wrefresh(temp);
    refresh();
	int i, j;

	for (i = 0; i<user->size_x; i++)
		mvwprintw(temp,1,3*(i+1)+2,"%c", 'A' + i);


	for (j = 0; j<user->size_y; j++)
	{
		mvwprintw(temp,j+2,1,"%i", j + 1);
		for (i = 0; i<user->size_x; i++)
		{
			if (user->board[j][i] == 0)
			{
                init_pair(2,COLOR_CYAN,COLOR_BLUE);
                wattron(temp,COLOR_PAIR(2));
				mvwprintw(temp,j+2,3*(i+1),"  ~  ");
				wattroff(temp,COLOR_PAIR(2));
            }
			else if (user->board[j][i] == -1)
			{
                init_pair(5,COLOR_WHITE,COLOR_BLUE);
                wattron(temp,COLOR_PAIR(5));
				mvwprintw(temp,j+2,3*(i+1),"  *  ");
				wattroff(temp,COLOR_PAIR(5));

            }
			else if (user->board[j][i] == -2)
			{
                init_pair(3,COLOR_RED,COLOR_BLUE);
                wattron(temp,COLOR_PAIR(3));
				mvwprintw(temp,j+2,3*(i+1),"  x  ");
				wattroff(temp,COLOR_PAIR(3));
            }
			else
            {
                init_pair(4,user->color,COLOR_BLUE);
                wattron(temp,COLOR_PAIR(4));
				mvwprintw(temp,j+2,3*(i+1),"  %i  ", user->board[j][i]);
				wattroff(temp,COLOR_PAIR(4));
            }
		}

        wattron(temp,COLOR_PAIR(1));
	}

	box(temp,0,0);
	wrefresh(temp);

}

void PrintHidden(data *AI,int y,int x)
{
	WINDOW *temp;

    temp=AI->windows.User;
	temp=newwin(AI->size_y+3,3*AI->size_x+5,y,x);

	init_pair(1,COLOR_WHITE,COLOR_BLUE);
    wbkgd(temp,COLOR_PAIR(1));

    wrefresh(temp);

	int i, j;

	for (i = 0; i<AI->size_x; i++)
		mvwprintw(temp,1,3*(i+1)+2,"%c", 'A' + i);


	for (j = 0; j<AI->size_y; j++)
	{
		mvwprintw(temp,j+2,1,"%i", j + 1);
        for (i = 0; i<AI->size_x; i++)
		{
			if ((AI->board[j][i] != -1) && (AI->board[j][i] != -2))
			{
                init_pair(6,COLOR_CYAN,COLOR_BLUE);
                wattron(temp,COLOR_PAIR(6));
				mvwprintw(temp,j+2,3*(i+1),"  ~  ");
				wattroff(temp,COLOR_PAIR(6));
            }
			else if (AI->board[j][i] != -1)
			{
                init_pair(7,AI->color,COLOR_BLUE);
                wattron(temp,COLOR_PAIR(7));
                mvwprintw(temp,j+2,3*(i+1),"  x  ");
                wattroff(temp,COLOR_PAIR(7));
            }
			else
			{
                init_pair(8,COLOR_WHITE,COLOR_BLUE);
                wattron(temp,COLOR_PAIR(8));
				mvwprintw(temp,j+2,3*(i+1),"  *  ");
				wattroff(temp,COLOR_PAIR(8));
            }
		}

		wattron(temp,COLOR_PAIR(1));
	}

	box(temp,0,0);
	wrefresh(temp);
	refresh();

}

void PrintInfo(data user)
{
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
	bkgd(COLOR_PAIR(1));
	refresh();
	mvprintw(3,4*user.size_x+6,"%i %s", user.number,user.pinfo);
	mvprintw(4,4*user.size_x+10,"%i (%i~) %s", user.s5.number, user.s5.size,user.s5.name);
	mvprintw(5,4*user.size_x+10,"%i (%i~) %s", user.s4.number, user.s4.size,user.s4.name);
	mvprintw(6,4*user.size_x+10,"%i (%i~) %s", user.s3.number, user.s3.size,user.s3.name);
	mvprintw(7,4*user.size_x+10,"%i (%i~) %s", user.s2.number, user.s2.size,user.s2.name);
	mvprintw(8,4*user.size_x+10,"%i (%i~) %s", user.s1.number, user.s1.size,user.s1.name);
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

	int counter = 0, number = AI->number, temp_size = size, x, y, orientation, x_new, y_new, seed = rand() % 1000;
	srand(time(NULL) + seed);

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
			sleep(1);
			return;
		}


		if (orientation)
			x_new += pow(-1, direction);
		else
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
    curs_set(1);
    mousemask(BUTTON1_CLICKED, 0);


	char x_char;
	int x, y, mark = size, x_new, y_new, orient = 0, n_orient,coord_xstart=7,coord_ystart=4,buffer,cx=coord_xstart,cy=coord_ystart;
    bool check,click_check;
    move(coord_ystart,coord_xstart);

	do
	{
        check=FALSE;
		__fpurge(stdin);

        buffer=getch();

        switch(buffer)
        {
        case KEY_MOUSE:
        if(getmouse(&mev) == OK)
        {
            if(((mev.x>=coord_xstart)&&(mev.x<=3*user->size_x-3+coord_xstart))&&((mev.y>=coord_ystart)&&(mev.y<=coord_ystart-1+user->size_y)))
            {

                if(((mev.x-coord_xstart)%3)==0)
                {
                    move(mev.y,mev.x);
                    cx=mev.x;
                    cy=mev.y;

                    x=(mev.x-coord_xstart)/3;
                    y=(mev.y-coord_ystart);
                    check=TRUE;
                    refresh();
                }
            }
        }

		if (((mev.x<coord_xstart)||(mev.x>3*user->size_x-3+coord_xstart))||((mev.y<coord_ystart)||(mev.y>coord_ystart-1+user->size_y))||(check==FALSE))
        {
			mvprintw(user->size_y+9,2,"%s",user->winput_message);
			move(coord_ystart,coord_xstart);
        }
        break;

        case KEY_UP: cy--; if(cy<coord_ystart) cy+=user->size_y; move(cy,cx); break;
        case KEY_DOWN: cy++; if(cy>coord_ystart+user->size_y-1) cy=coord_ystart; move(cy,cx); break;
        case KEY_LEFT: cx-=3; if(cx<coord_xstart) cx+=coord_xstart+2*user->size_x-2; move(cy,cx); break;
        case KEY_RIGHT: cx+=3; if(cx>coord_xstart+3*user->size_x-2) cx=coord_xstart; move(cy,cx); break;
        case '\n': x=(cx-coord_xstart)/3; y=(cy-coord_ystart); check=TRUE; break;
        }

        mvprintw(user->size_y+10,2,"(%c,%i)",(cx-coord_xstart)/3+'A',cy-coord_ystart+1);
        move(cy,cx);
	} while ((check==FALSE)||(user->board[y][x]!=0));


	if (user->board[y][x] == 0)
	{
		user->board[y][x] = user->number;
		size--;
	}

	PrintInfo(*user);
	PrintBoard(user,2,2);
    refresh();


	while (size>0)
	{

		do
		{
		check=FALSE;

		__fpurge(stdin);
		mvprintw(user->size_y+9,2,"%s",user->nextcoord_message);
        move(cy,cx);
        buffer=getch();

        switch(buffer)
        {
        case KEY_MOUSE:
        if(getmouse(&mev) == OK)
        {
            if(((mev.x>=coord_xstart)&&(mev.x<=3*user->size_x-3+coord_xstart))&&((mev.y>=coord_ystart)&&(mev.y<=coord_ystart-1+user->size_y)))
            {
                move(mev.y,mev.x);

                if((mev.x-coord_xstart)%3==0)
                {
                    cx=mev.x;
                    cy=mev.y;
                    x_new=(mev.x-coord_xstart)/3;
                    y_new=(mev.y-coord_ystart);
                    check=TRUE;

                }
            }
        }

			if (((mev.x<coord_xstart)||(mev.x>3*user->size_x-3+coord_xstart))||((mev.y<coord_ystart)||(mev.y>coord_ystart+user->size_y-1))||(check==FALSE))
			{
				mvprintw(user->size_y+9,2,"%s",user->winput_message);
				move(mev.y,mev.x);
            }

        break;

        case KEY_UP: cy--; if(cy<coord_ystart) cy+=user->size_y; move(cy,cx); break;
        case KEY_DOWN: cy++; if(cy>coord_ystart+user->size_y-1) cy=coord_ystart; move(cy,cx); break;
        case KEY_LEFT: cx-=3; if(cx<coord_xstart) cx+=coord_xstart+2*user->size_x-2; move(cy,cx); break;
        case KEY_RIGHT: cx+=3; if(cx>coord_xstart+3*user->size_x-2) cx=coord_xstart; move(cy,cx); break;
        case '\n': x_new=(cx-coord_xstart)/3; y_new=(cy-coord_ystart); check=TRUE; break;

        }
            mvprintw(user->size_y+10,2,"(%c,%i)",(cx-coord_xstart)/3+'A',cy-coord_ystart+1);
            move(cy,cx);
		} while ((check==FALSE)||(user->board[y_new][x_new]!=0));


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

		if (((user->board[y_new][x_new + 1] == user->number) || (user->board[y_new][x_new - 1] == user->number) || (user->board[y_new + 1][x_new] == user->number) || (user->board[y_new - 1][x_new] == user->number)) && ((user->board[y_new][x_new] == 0) && (n_orient == orient)))
		{
			user->board[y_new][x_new] = user->number;
			size--;
			bkgd(COLOR_PAIR(1));
			PrintInfo(*user);
			PrintBoard(user,2,2);
			refresh();
			x = x_new;
			y = y_new;
		}
		else
		{
			mvprintw(user->size_y+9,2,"%s",user->winput_message);
			refresh();
			move(mev.y,mev.x);
        }
	}
	curs_set(0);
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
    int x,y,temp;
    getmaxyx(stdscr,y,x);

    while((user->size_x*7>x)||(user->size_y*2>y))
    {
        mvprintw(LINES/2,(COLS-strlen(user->resize_message))/2,"%s",user->resize_message);
        refresh();
        getmaxyx(stdscr,y,x);
        scanw("%i",temp);

        clear();
    }

    refresh();
    __fpurge(stdin);

	while (user->number>0)
	{
		SetPosition(user);
		user->number--;
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


void SetPosition(data *user)
{
	PrintInfo(*user);
	refresh();
	PrintBoard(user,2,2);

	if (user->s5.number != 0)
	{
		mvprintw(user->size_y+6,2,"%s",user->inscoord_text);
		mvprintw(user->size_y+7,2,"%s",user->s5.name);
		SetCoordinates(user, user->s5.size);
		user->s5.number--;
		clear();
	}
	else if (user->s4.number != 0)
	{
		mvprintw(user->size_y+6,2,"%s",user->inscoord_text);
		mvprintw(user->size_y+7,2,"%s",user->s4.name);
		SetCoordinates(user, user->s4.size);
		user->s4.number--;
		clear();
	}
	else if (user->s3.number != 0)
	{
		mvprintw(user->size_y+6,2,"%s",user->inscoord_text);
		mvprintw(user->size_y+7,2,"%s",user->s3.name);
		SetCoordinates(user, user->s3.size);
		user->s3.number--;
		clear();
	}
	else if (user->s2.number != 0)
	{
		mvprintw(user->size_y+6,2,"%s",user->inscoord_text);
		mvprintw(user->size_y+7,2,"%s",user->s2.name);
		SetCoordinates(user, user->s2.size);
		user->s2.number--;
		clear();
	}
	else if (user->s1.number != 0)
	{
		mvprintw(user->size_y+6,2,"%s",user->inscoord_text);
		mvprintw(user->size_y+7,2,"%s",user->s1.name);
		SetCoordinates(user, user->s1.size);
		user->s1.number--;
		clear();
	}

}


void Initialize(data *user,menu *Main,FILE *in)
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
	in=fopen(Main->game,"r");
	fgets(user->message_1,50,in);
	fgets(user->message_2,50,in);
	fgets(user->message_3,50,in);
	fgets(user->difficulty1,50,in);
	fgets(user->difficulty2,50,in);
	fgets(user->difficulty3,50,in);
	fgets(user->difficulty4,50,in);
	fgets(user->inscoord_text,50,in);
	fgets(user->pinfo,50,in);
	fgets(user->winput_message,50,in);
	fgets(user->score_text,50,in);
	fgets(user->nextcoord_message,50,in);
	fgets(user->resize_message,50,in);
	fgets(user->nextmove_message,50,in);
	fgets(user->miss,50,in);
	fgets(user->hit_message,50,in);
	fgets(user->already_hit,50,in);
	fgets(user->insert_name,50,in);
	fgets(user->message_lost,50,in);
	fgets(user->message_won,50,in);
	fgets(user->message_score,50,in);
	fgets(user->play_again,50,in);
	fgets(user->s1.name,50,in);
	fgets(user->s2.name,50,in);
	fgets(user->s3.name,50,in);
	fgets(user->s4.name,50,in);
	fgets(user->s5.name,50,in);
	fclose(in);
	user->hit = 0;
	user->lost = 0;
	user->moves = 0;
	user->score = 0.0;
	user->error = 0;
	user->color=Main->color;
	user->cx=0;
	user->cy=0;
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
			if(user.difficulty!=4)
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
	int seed = rand() % 1000;
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
	int seed = rand() % 1000;
	PrintAll(user, AI);
	srand(time(NULL) + seed);
	int x, y;

	Difficulty(*user, *AI, &x, &y);

	switch (Hit(user, x, y))
	{
	case 2: break;
	case 0: break;
	case 1: AI->hit++; user->lost++; user->total_fields--; ScoreUpdate(user,2); break;
	}

}

void UserMove(data *user, data *AI)
{
    curs_set(1);
	PrintAll(user, AI);
	char x_temp;
	int x, y, coord_xstart=12,coord_ystart=4,buffer;

	if(user->cx==0)
        user->cx=coord_xstart+3*user->size_x;
	if(user->cy==0)
        user->cy=coord_ystart;

	bool check=FALSE;
    mousemask(BUTTON1_CLICKED, 0);

	do
	{
        check=FALSE;
		mvprintw(user->size_y+9,2,"%s",user->nextmove_message);
		move(user->cy,user->cx);
		buffer=getch();

		switch(buffer)
		{
		case KEY_MOUSE:
            if(getmouse(&mev) == OK)
            {
                if(((mev.x>=coord_xstart+3*user->size_x)&&(mev.x<=6*user->size_x-3+coord_xstart))&&((mev.y>=coord_ystart)&&(mev.y<=coord_ystart-1+user->size_y)))
                {
                    if((mev.x-coord_xstart-3*user->size_x)%3==0)
                    {
                        mvprintw(user->size_y+10,2,"(%c,%i)",(mev.x-coord_xstart-3*user->size_x)/3+'A',mev.y-coord_ystart+1);
                        x=(mev.x-coord_xstart-3*user->size_x)/3;
                        y=(mev.y-coord_ystart);
                        check=TRUE;
                        move(mev.y,mev.x);
                        user->cx=mev.x;
                        user->cy=mev.y;
                        refresh();
                    }
                }
            }

		if (((mev.x<coord_xstart+3*user->size_x)||(mev.x>6*user->size_x-3+coord_xstart))||((mev.y<coord_ystart)||(mev.y>coord_ystart+user->size_y-1)))
			mvprintw(user->size_y+9,2,"%s",user->winput_message);

        move(user->cy,user->cx);
        break;

        case KEY_UP: user->cy--; if(user->cy<coord_ystart) user->cy+=user->size_y; move(user->cy,user->cx); break;
        case KEY_DOWN: user->cy++; if(user->cy>coord_ystart+user->size_y-1) user->cy=coord_ystart; move(user->cy,user->cx); break;
        case KEY_LEFT: user->cx-=3; if(user->cx<coord_xstart+3*user->size_x) user->cx+=3*user->size_x; move(user->cy,user->cx); break;
        case KEY_RIGHT: user->cx+=3; if(user->cx>coord_xstart+6*user->size_x-2) user->cx=coord_xstart+3*user->size_x; move(user->cy,user->cx); break;
        case '\n': x=(user->cx-coord_xstart-3*user->size_x)/3; y=(user->cy-coord_ystart); check=TRUE; break;

        }
        mvprintw(user->size_y+10,2,"(%c,%i)",(user->cx-coord_xstart-3*user->size_x)/3+'A',user->cy-coord_ystart+1);

	} while (check==FALSE);

	user->moves++;

	switch (Hit(AI, x, y))
	{
	case 2: mvprintw(1,2,"%s",user->miss); break;
	case 0: mvprintw(1,2,"%s",user->already_hit); break;
	case 1: user->hit++; AI->lost++; AI->total_fields--; ScoreUpdate(user,1); mvprintw(1,2,"%s",user->hit_message); break;
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


void ScoreUpdate(data *user,int sign)
{

	if(sign==1)
        user->score+=(10.0/user->total);
    else
        if(sign==2)
            user->score-=(user->difficulty-1)*0.1*user->difficulty;

}

void EndGame(data *user, data *AI)
{
	WINDOW *win;
	echo();
	PrintAll(user,AI);
	win=newwin(3*LINES/4,3*COLS/4,LINES/8,COLS/8);
	init_pair(1,COLOR_WHITE,COLOR_BLUE);
    wbkgd(win,COLOR_PAIR(1));

    if(user->total_fields==0)
        mvwprintw(win,3*LINES/8-9,(3*COLS/4-strlen(user->message_lost))/2,"%s",user->message_lost);
    else
        mvwprintw(win,3*LINES/8-9,(3*COLS/4-strlen(user->message_won))/2,"%s",user->message_won);

    mvwprintw(win,3*LINES/8-7,(3*COLS/4-strlen(user->message_score))/2,"%s",user->message_score);
    mvwprintw(win,3*LINES/8-7,(3*COLS/4-strlen(user->message_score))/2+strlen(user->message_score)+2,"%.2f",user->score);

	mvwprintw(win,3*LINES/8-5,(3*COLS/4-strlen(user->insert_name))/2,"%s",user->insert_name);
	box(win,0,0);
	mvwgetstr(win,3*LINES/8-3,(3*COLS/4-strlen(user->insert_name)/2)/2,user->name);
	wrefresh(win);

    WriteHighScore(user);

    noecho();
    char buffer;
    curs_set(0);

    do
    {
        mvwprintw(win,3*LINES/8+2,(3*COLS/4-strlen(user->play_again))/2,"%s",user->play_again);
        box(win,0,0);
        wrefresh(win);
        __fpurge(stdin);
        buffer=getch();
    } while((buffer!='Y')&&(buffer!='y')&&(buffer!='N')&&(buffer!='n'));

    clear();
    if((buffer=='Y')||(buffer=='y'))
    {
        ReInit(user);
    }
    else
        ExitProgram();
}

typedef struct Scores
{
    char user_name[50];
    int difficulty;
    float score;
}score;

void WriteHighScore(data *user)
{
    FILE *in;
    score list[50],temp;
    int i=0,j,k;
    in=fopen("hiscore.txt","r");

    while(fscanf(in,"%s",list[i].user_name)!=EOF)
    {
        fscanf(in,"%i %f",&list[i].difficulty,&list[i].score);
        i++;
    }
    fclose(in);

    strcpy(list[i].user_name,user->name);
    list[i].difficulty=user->difficulty;
    list[i].score=user->score;
    i++;

    for(j=0;j<i;j++)
        for(k=j;k<i;k++)
        {
            if(list[j].score<list[k].score)
            {
                temp=list[j];
                list[j]=list[k];
                list[k]=temp;
            }
        }

    in=fopen("hiscore.txt","w");
    fprintf(in,"\n\n");
    for(j=0;j<i;j++)
        fprintf(in,"\t%-15s %-4i %-4.2f\n",list[j].user_name,list[j].difficulty,list[j].score);
    fclose(in);
}
