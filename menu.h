/*******************************************************************************|
|    										|
|	FTN Terms is a spinoff of a popular board game Battleships.		|
|	For full functionality and detailed information consult the		|
|	manual provided with the game. This is the library created 		|
|	for handling the main menu.						|
|										|
|	Copyright  © 2014. Viktor Šanca <viktor.sanca@gmail.com>		|
|    										|
|	This program is free software: you can redistribute it and/or modify	|
|	it under the terms of the GNU General Public License as published by	|
|	the Free Software Foundation, either version 3 of the License, or	|
|       any later version.							|
|										|
|	This program is distributed in the hope that it will be useful,		|
|	but WITHOUT ANY WARRANTY; without even the implied warranty of		|
|	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		|
|	GNU General Public License for more details.				|
|										|
|	You should have received a copy of the GNU General Public License	|
|	along with this program.  If not, see <http://www.gnu.org/licenses/> 	|
|										|
|*******************************************************************************/

#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Box
{
	WINDOW *win;
	char text[20];
	int highlighted;
	int start_x;
	int end_x;
	int start_y;
	int end_y;
	int width;
	char temp[20];
	int box;
}winbox;

typedef struct Menu
{
    int current,type,color,signal;
    FILE *in;
    char exit_message[20],language[20],manual[20],game[20];
	winbox Boxes[8];
}menu;


void ClearAll(menu *Screen);
void CreateMenu(FILE *in,menu *Main);
void CreateOptions(FILE *in, menu *Main);
void DisplayHighScore(menu *ReturnScreen);
char DisplayMessage(menu *ReturnScreen);
void DisplayUserManual(menu *ReturnScreen);
void ExitProgram();
void MenuChoice(menu *Main, menu *Options, int selection);
void MouseChoice(menu *Main,menu *Options,int y, int x);
void MouseOptionsChoice(menu *Main, menu *Options, int y, int x);
void InitializeBox(FILE *in, winbox *temp,int *y_prev);
int InitializeMain(menu *Main);
void InitializeNoBox(FILE *in, winbox *temp,int *y_prev);
void PrintBox(winbox *temp);
void PrintBoxSpecial(winbox *temp,int color);
void PrintMenu(menu *Main, menu *Options);
void PrintOptions(menu *Main,menu *ReturnMenu);
void StandardProcedure(menu *Main, menu *Options);


int InitializeMain(menu *Main)
{
    FILE *in;
    menu *Options=(menu*)malloc(sizeof(menu));

    Main->type=1;
    Options->type=2;
    Options->color=Main->color;

    scrollok(stdscr,TRUE);

    initscr();
    start_color();
    curs_set(0);
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
    bkgd(COLOR_PAIR(1));

    cbreak();
    noecho();
    keypad(stdscr,TRUE);
    refresh();

    StandardProcedure(Main,Options);

    getch();
    echo();
    endwin();

return 0;
}

void StandardProcedure(menu *Main, menu *Options)
{
    while(1)
    {
        Main->in=fopen(Main->language,"r");
        CreateMenu(Main->in,Main);
        CreateOptions(Main->in,Options);
        fclose(Main->in);
        PrintMenu(Main,Options);
        ClearAll(Main);
        refresh();
    }
}

void InitializeBox(FILE *in, winbox *temp,int *y_prev)
{
    fgets(temp->text,20,in);
    temp->width=strlen(temp->text)+1;
    temp->start_x=(COLS-temp->width)/2;
    temp->start_y=*y_prev+1;
    *y_prev+=3;
    temp->highlighted=0;
    int height=3;
    temp->win=newwin(height,temp->width,temp->start_y,temp->start_x);
    keypad(temp->win,TRUE);
    box(temp->win,0,0);
}

void InitializeNoBox(FILE *in, winbox *temp,int *y_prev)
{
    fgets(temp->text,20,in);
    temp->width=strlen(temp->text);
    temp->start_x=(COLS-temp->width)/2;
    temp->start_y=*y_prev;
    temp->box=1;
    *y_prev+=1;
    temp->highlighted=0;
    int height=1;
    temp->win=newwin(height,temp->width-1,temp->start_y,temp->start_x);
    keypad(temp->win,TRUE);
}

void CreateMenu(FILE *in,menu *Main)
{
    int y_prev=6;
	InitializeBox(in,&(Main->Boxes[0]),&y_prev);
	InitializeBox(in,&(Main->Boxes[1]),&y_prev);
	InitializeBox(in,&(Main->Boxes[2]),&y_prev);
	InitializeBox(in,&(Main->Boxes[3]),&y_prev);
	InitializeBox(in,&(Main->Boxes[4]),&y_prev);
	fgets(Main->exit_message,20,in);

    if(Main->signal==0)
        Main->Boxes[0].highlighted=1;

	refresh();
	endwin();

}

void PrintBox(winbox *temp)
{
    if (temp->box==0)
        if (temp->highlighted==1)
            wattron(temp->win,A_REVERSE);

    init_pair(1,COLOR_WHITE,COLOR_BLUE);
    wattron(temp->win,COLOR_PAIR(1));

    if (temp->box==0)
    {
        mvwprintw(temp->win,1,1,"%s",temp->text);
        box(temp->win,0,0);
    }
    else
        mvwprintw(temp->win,0,0,"%s",temp->text);

    wrefresh(temp->win);
    refresh();
    wattroff(temp->win,A_REVERSE);
    temp->highlighted=0;
}

void PrintBoxSpecial(winbox *temp,int color)
{
    if (color!=4)
        init_pair(2,color,COLOR_BLUE);
    else
        init_pair(2,color,COLOR_WHITE);

    if (temp->box==0)
        if (temp->highlighted==1)
            wattron (temp->win,A_REVERSE|COLOR_PAIR(2));

    if (temp->box==0)
    {
        mvwprintw(temp->win,1,1,"%s",temp->text);
        box(temp->win,0,0);
    }
    else
        mvwprintw(temp->win,0,0,"%s",temp->text);

    wrefresh(temp->win);
    refresh();
    wattroff (temp->win,A_REVERSE);
    temp->highlighted=0;
}

MEVENT mev;

void PrintMenu(menu *Main,menu *Options)
{
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
    bkgd(COLOR_PAIR(1));
    char buffer[90];
    FILE *in;
    in=fopen("logo.txt","r");
    while(fgets(buffer,80,in)!=NULL)
        printw("%s",buffer);
    fclose(in);

    refresh();
    int key,i,new_current=0,temp_lines=LINES,temp_cols=COLS;
    do
    {
        for(i=0;i<5;i++)
            PrintBox(&(Main->Boxes[i]));

        if(is_term_resized(temp_lines,temp_cols)==TRUE)
            break;


        mousemask(BUTTON1_CLICKED, 0);
        key=getch();
        switch(key)
        {
            case KEY_MOUSE: if(getmouse(&mev) == OK) MouseChoice(Main,Options,mev.y,mev.x); break;
            case KEY_UP: if((Main->current)==0) (Main->current)+=5; Main->Boxes[abs((Main->current-1)%5)].highlighted=1; Main->Boxes[abs(Main->current%5)].highlighted=0; --(Main->current);
            if(Main->current<=0) Main->current+=5; break;
            case KEY_DOWN: Main->Boxes[abs((Main->current+1)%5)].highlighted=1; Main->Boxes[abs(Main->current%5)].highlighted=0; ++(Main->current); Main->current=(Main->current)%5; break;
            case '\n': MenuChoice(Main,Options,abs(Main->current%5)); Main->Boxes[Main->current].highlighted=1; break;
            default: refresh();
        }
        refresh();
    }while(1);
}

void MouseChoice(menu *Main,menu *Options,int y, int x)
{
    int i,temp=-1;
    for(i=0;i<5;i++)
    {
        if ((y>=Main->Boxes[i].start_y)&&(y<=(Main->Boxes[i].start_y+Main->Boxes[i].width))&&(x>=Main->Boxes[i].start_x)&&(x<=(Main->Boxes[i].start_x+Main->Boxes[i].width)))
            temp=i;
    }
    switch(temp)
    {
        case 0: InitializeGame(Main); break;
        case 1: DisplayHighScore(Main); StandardProcedure(Main,Options); break;
        case 2: PrintOptions(Options,Main); Options->current=0; StandardProcedure(Main,Options); break;
        case 3: DisplayUserManual(Main); StandardProcedure(Main,Options); break;
        case 4: temp=DisplayMessage(Main); if((temp=='y')||(temp=='Y')) ExitProgram(); break;
    }
    refresh();
}

void MenuChoice(menu *Main, menu *Options, int selection)
{
    char temp;
    switch(selection)
    {
        case 0: InitializeGame(Main); break;
        case 1: DisplayHighScore(Main); StandardProcedure(Main,Options); break;
        case 2: PrintOptions(Options,Main); StandardProcedure(Main,Options); break;
        case 3: DisplayUserManual(Main); StandardProcedure(Main,Options); break;
        case 4: temp=DisplayMessage(Main); if((temp=='y')||(temp=='Y')) ExitProgram(); break;
    }
}

void OptionsChoice(menu *Options, menu *Main, int selection)
{
    char temp;
    switch(selection)
    {
        case 0: Main->color++; Main->color=(Main->color)%7; Options->color++; Options->color=(Options->color)%7; PrintBoxSpecial(&(Options->Boxes[selection]),Main->color); refresh(); break;
        case 1: break;
        case 2: strcpy(Main->language,"eng.txt"); strcpy(Main->manual,"manualeng.txt"); strcpy(Main->game,"gameeng.txt"); ClearAll(Main); Main->signal=1; wattron(Main->Boxes[Main->current].win,A_REVERSE); wrefresh(Main->Boxes[Main->current].win); StandardProcedure(Main,Options); break;
        case 3: strcpy(Main->language,"srb.txt"); strcpy(Main->manual,"manualsrb.txt"); strcpy(Main->game,"gamesrb.txt"); ClearAll(Main); Main->signal=1; wattron(Main->Boxes[Main->current].win,A_REVERSE); wrefresh(Main->Boxes[Main->current].win); StandardProcedure(Main,Options); break;
        case 4: ClearAll(Options); PrintMenu(Main,Options); refresh(); Main->Boxes[Main->current].highlighted=1; Main->signal=1; wattron(Main->Boxes[Main->current].win,A_REVERSE); refresh();
        Options->current=0; StandardProcedure(Main,Options); break;
    }
}



void CreateOptions(FILE *in, menu *Main)
{
    int y_prev=3;

	InitializeBox(in,&(Main->Boxes[0]),&y_prev);
    y_prev+=2;
    InitializeNoBox(in,&(Main->Boxes[1]),&y_prev);
	y_prev--;
	InitializeBox(in,&(Main->Boxes[2]),&y_prev);
	InitializeBox(in,&(Main->Boxes[3]),&y_prev);
    y_prev++;

	InitializeBox(in,&(Main->Boxes[4]),&y_prev);

    Main->Boxes[0].highlighted=1;
	refresh();
	endwin();
}

void PrintOptions(menu *Main,menu *ReturnMenu)
{
    ClearAll(ReturnMenu);
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
    bkgd(COLOR_PAIR(1));
    init_pair(2,Main->color,COLOR_BLUE);
    wattrset(Main->Boxes[0].win,COLOR_PAIR(2));
    refresh();

    int key,i,new_current=0,temp_lines=LINES,temp_cols=COLS;
    do
    {
        PrintBoxSpecial(&(Main->Boxes[0]),Main->color);
        for(i=1;i<5;i++)
            PrintBox(&(Main->Boxes[i]));

        if(is_term_resized(temp_lines,temp_cols)==TRUE)
            break;

        mousemask(BUTTON1_CLICKED, 0);
        key=getch();
        switch(key)
        {
            case KEY_MOUSE: if(getmouse(&mev) == OK) MouseOptionsChoice(Main,ReturnMenu,mev.y,mev.x); break;
            case KEY_UP: if((Main->current)==0) (Main->current)+=5;
            Main->Boxes[abs((Main->current-1)%5)].highlighted=1; Main->Boxes[abs(Main->current%5)].highlighted=0; --(Main->current);
            if(Main->current<=0) Main->current+=5;
            if(Main->current==1){ --Main->current; Main->Boxes[0].highlighted=1; }
            break;

            case KEY_DOWN: Main->Boxes[abs((Main->current+1)%5)].highlighted=1;
            Main->Boxes[abs(Main->current%5)].highlighted=0; ++(Main->current); Main->current=(Main->current)%5;
            if(Main->current==1){ ++Main->current; Main->Boxes[2].highlighted=1; }
            break;
            case '\n': OptionsChoice(Main,ReturnMenu,abs(Main->current%5)); Main->Boxes[Main->current].highlighted=1; break;
        }
        refresh();
    }while(1);
}

void MouseOptionsChoice(menu *Main, menu *Options, int y, int x)
{
    int i,temp=-1;

    for(i=0;i<5;i++)
    {
        if ((y>=Main->Boxes[i].start_y)&&(y<=(Main->Boxes[i].start_y+Main->Boxes[i].width))&&(x>=Main->Boxes[i].start_x)&&(x<=(Main->Boxes[i].start_x+Main->Boxes[i].width)))
            temp=i;
    }

    switch(temp)
    {
        case 0: Main->color++; Main->color=(Main->color)%7; Options->color++; Options->color=(Options->color)%7; refresh(); break;
        case 1: break;
        case 2: strcpy(Options->language,"eng.txt"); strcpy(Options->manual,"manualeng.txt"); strcpy(Options->game,"gameeng.txt"); ClearAll(Main); Options->signal=1;
        wattron(Options->Boxes[Options->current].win,A_REVERSE); wrefresh(Options->Boxes[Options->current].win); StandardProcedure(Options,Main); break;
        case 3: strcpy(Options->language,"srb.txt"); strcpy(Options->manual,"manualsrb.txt"); strcpy(Options->game,"gamesrb.txt"); ClearAll(Main); Options->signal=1;
        wattron(Options->Boxes[Options->current].win,A_REVERSE); wrefresh(Options->Boxes[Options->current].win); StandardProcedure(Options,Main); break;
        case 4: ClearAll(Options); refresh(); PrintMenu(Options,Main); break;
    }

}

char DisplayMessage(menu *ReturnScreen)
{
    WINDOW *temp;
    char buffer;
    temp=newwin(LINES/2,COLS/2,LINES/4,COLS/4);
    wbkgd(temp,COLOR_PAIR(1));
    mvwprintw(temp,LINES/4,(COLS/2-strlen(ReturnScreen->exit_message))/2,"%s",ReturnScreen->exit_message);
    box(temp,0,0);
    wrefresh(temp);
    refresh();
    buffer=getch();
    clear();
    refresh();
    return buffer;
}


void DisplayHighScore(menu *ReturnScreen)
{
    WINDOW *temp;
    FILE *in;
    char buffer;
    ClearAll(ReturnScreen);
    temp=newwin(LINES,COLS,0,0);
    wbkgd(temp,COLOR_PAIR(1));

    in=fopen("hiscore.txt","r");
    while((buffer=fgetc(in))!=EOF)
        waddch(temp,buffer);
    fclose(in);

    box(temp,0,0);
    wrefresh(temp);
    refresh();
    getch();
    clear();
    refresh();
}


void DisplayUserManual(menu *ReturnScreen)
{
    WINDOW *temp;
    FILE *in;
    char buffer;

    ClearAll(ReturnScreen);
    temp=newwin(LINES,COLS,0,0);
    wbkgd(temp,COLOR_PAIR(1));

    in=fopen(ReturnScreen->manual,"r");
    while((buffer=fgetc(in))!=EOF)
        waddch(temp,buffer);
    fclose(in);

    box(temp,0,0);
    wrefresh(temp);
    refresh();
    getch();
    clear();
    refresh();
}

void ClearAll(menu *Screen)
{
    SCREEN *s;
    int i;
    clear();
    refresh();

    for(i=0;i<5;i++)
    {
        wclear(Screen->Boxes[i].win);
        wrefresh(Screen->Boxes[i].win);
    }
}

void DeleteAll(menu *Screen)
{
    int i;

    for(i=0;i<8;i++)
        delwin(Screen->Boxes[i].win);
}

void ExitProgram()
{
    delwin(stdscr);
    endwin();
    exit(0);
}
