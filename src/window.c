/*                                           ### description ###                     
    File contains functions for creating, and manipulating ncurses windows and the window structs. 
*/

//---------------------------------------------- headers -------------------------------------------------
#include <stdlib.h>
#include "window.h"

//---------------------------------------------- define --------------------------------------------------
#define CUR_WIN WIN_AR->ar[WIN_AR->length]
//---------------------------------------------- enums ---------------------------------------------------


//---------------------------------------------- typedefs ------------------------------------------------


//---------------------------------------------- structs -------------------------------------------------


//---------------------------------------------- global vars ---------------------------------------------
WINDOW_ARRAY *WIN_AR;
WINDOW_S *GOAL;
WINDOW *PROMPT_WIN,*MAIN_WINDOW;
int START_X,START_Y;
//---------------------------------------------- prototypes ----------------------------------------------

static int checkXYParams(void);
static int checkGoalParams(const int x, const int y, const int height, const int width);
static void startColors(void);
static void printToPromptWindow(const char *const str);
static void printWindowArray(void);
static void makeNewWindow(const int height, const int width);
static void printStartLocation(void);
static int getIntFromInput(void);
static void printWindow(const WINDOW_S *const window);
static WINDOW_S *makeWindow(const int x, const int y,const int height, const int width);
static int moveWindowY(const int y,const int height,const DIRECTION dir);
static int moveWindowX(const int x,const int width,const DIRECTION dir);
static void makePromptWindow(void);
static void makeMainWindow(void);
static void resizeWindowArray(void);
//---------------------------------------------- code ----------------------------------------------------

static void resizeWindowArray(void) {
    WIN_AR->max_size += 5;
    WINDOW_S **temp   = realloc(WIN_AR->ar,sizeof(WINDOW_S) * WIN_AR->max_size);
    if(temp == NULL) {
        temp = realloc(WIN_AR->ar,sizeof(WINDOW_S) * WIN_AR->max_size);
    }
    WIN_AR->ar = temp;
}


static void makeMainWindow(void) {
    MAIN_WINDOW = newwin(HEIGHT + 2,WIDTH + 2,0,0);
    wborder(MAIN_WINDOW,'|','|','-', '-', '+', '+', '+', '+');
    wrefresh(MAIN_WINDOW);
}

static void makePromptWindow(void) {
    PROMPT_WIN = newwin(4,26,0,WIDTH + 4);
    wborder(PROMPT_WIN,'|','|','-', '-', '+', '+', '+', '+');
    wrefresh(PROMPT_WIN);
    
}

static void startColors(void) {
    start_color();
	init_pair(1,COLOR_RED,COLOR_BLACK);
	init_pair(2,COLOR_GREEN,COLOR_GREEN);
    init_pair(3,COLOR_WHITE,COLOR_GREEN);
}

void initScreen(void) {
	initscr();    //start ncuurses
	noecho();    //dont show user input on screen
	curs_set(0); //hide cursor
	keypad(stdscr,TRUE); //enable keypad. 
    startColors();
    refresh();
    makePromptWindow();
    makeMainWindow();
}

static int moveWindowX(const int x,const int width,const DIRECTION dir) {
    const int new_x = (dir == LEFT)? x - 1 : x + 1;
    if(new_x <= 0 || (new_x + width) > WIDTH) {
        return x;
    }
    return new_x;
}

static int moveWindowY(const int y,const int height,const DIRECTION dir) {
    const int new_y = (dir == UP)? y - 1: y + 1;
    if(new_y <= 0 || (new_y + height - 1) > HEIGHT) {
        return y;
    }
    return new_y;
}

static WINDOW_S *makeWindow(const int x, const int y,const int height, const int width) {
    WINDOW_S *temp = malloc(sizeof(WINDOW_S));
    temp->window   = newwin(height,width,y,x);
    temp->height   = height;
    temp->width    = width;
    temp->x        = x;
    temp->y        = y;
    return temp;
}

static void makeNewWindow(const int height, const int width) {
    WIN_AR->length++;
   if(WIN_AR->length >= WIN_AR->max_size) {
       resizeWindowArray();
   }
   WIN_AR->ar[WIN_AR->length] = makeWindow(1,1,height,width);
}

void makeWindowArray(void) {
    WIN_AR           = malloc(sizeof(WINDOW_ARRAY));
    WIN_AR->length   = -1;
    WIN_AR->max_size = 5;
    WIN_AR->ar       = malloc(sizeof(WINDOW_S) * 5);
}


void moveCurrentWindow(DIRECTION dir) {
    CUR_WIN->x = (dir == LEFT || dir == RIGHT)? moveWindowX(CUR_WIN->x,CUR_WIN->width,dir):CUR_WIN->x;
    CUR_WIN->y = (dir == UP || dir == DOWN)? moveWindowY(CUR_WIN->y,CUR_WIN->height,dir):CUR_WIN->y;
    wclear(CUR_WIN->window);
    wrefresh(CUR_WIN->window);
    mvwin(CUR_WIN->window,CUR_WIN->y,CUR_WIN->x);
    printWindowArray();
    printStartAndGoal();
}

static void printWindow(const WINDOW_S *const window) {
    wborder(window->window,'|','|','-', '-', '+', '+', '+', '+');
    wrefresh(window->window);
}

static void printWindowArray(void) {
    for(int i = 0; i <= WIN_AR->length; i++) {
        printWindow(WIN_AR->ar[i]);
    }
}

static int getIntFromInput(void) {
    char str[5];
    getstr(str);
    return atoi(str);
}

static void printToPromptWindow(const char *const str) {
    wclear(PROMPT_WIN);
    mvwprintw(PROMPT_WIN,1,1,"%s",str);
    wborder(PROMPT_WIN,'|','|','-', '-', '+', '+', '+', '+');
    wrefresh(PROMPT_WIN);
}

void deleteCurrentWindow(void) {
    wclear(CUR_WIN->window);
    wrefresh(CUR_WIN->window);
    delwin(CUR_WIN->window);
    free(CUR_WIN);
    WIN_AR->length--;
    printWindowArray();
}

static void printStartLocation(void) {
    wattron(MAIN_WINDOW,COLOR_PAIR(1));
    mvwprintw(MAIN_WINDOW,START_Y,START_X,"X");
    wrefresh(MAIN_WINDOW);
    wattroff(MAIN_WINDOW,COLOR_PAIR(1));
}

void printStartAndGoal(void) {
    printStartLocation();
    wattron(GOAL->window,COLOR_PAIR(2));
    mvwprintw(GOAL->window,0,0,"      ");
    mvwprintw(GOAL->window,1,0,"      ");
    mvwprintw(GOAL->window,2,0,"      ");
    wattroff(GOAL->window,COLOR_PAIR(2));
    wattron(GOAL->window,COLOR_PAIR(3));
    mvwprintw(GOAL->window,1,1,"Goal");
    wattroff(GOAL->window,COLOR_PAIR(3));
    wrefresh(GOAL->window);
}

//make sure start location is in bounds
static int checkXYParams(void) {
    if(START_X <= 0 || START_X >= WIDTH) {
        return 1;
    }
    if(START_Y <= 0 || START_Y >= HEIGHT) {
        return 1;
    }
    return 0;
}

void getStartXY(void) {
    printToPromptWindow("please enter starting x:");
    START_X = getIntFromInput();
    printToPromptWindow("please enter starting y:");
    START_Y = getIntFromInput();
    printToPromptWindow("");
    if(checkXYParams()) {
        printToPromptWindow("start is out of bounds.");
        getch();
        printToPromptWindow("");
        getStartXY();
    }
}

void createNewWindow(void) {
    printToPromptWindow("please enter height:\n");
    const int height = getIntFromInput();
    printToPromptWindow("please enter width:\n");
    const int width = getIntFromInput();
    printToPromptWindow("");
    makeNewWindow(height,width);
    printWindowArray();
}

//check to make sure goal isnt out of bounfs.
static int checkGoalParams(const int x, const int y, const int height, const int width) {
    if(x <= 0 || x + width >= WIDTH) {
        return 1;
    }
    if(y <= 0 || y + height >= HEIGHT) {
        return 1;
    }
    return 0;
}

void makeGoal(void) {
    printToPromptWindow("enter x for goal: ");
    const int x = getIntFromInput();
    printToPromptWindow("enter y for goal: ");
    const int y      = getIntFromInput();
    const int height = 3;
    const int width  = 6;
    printToPromptWindow("");
    if(!checkGoalParams(x,y,height,width)) {
        GOAL = makeWindow(x,y,height,width);
    }
    else {
        printToPromptWindow("goal is out of bounds.");
        getch();
        printToPromptWindow("");
        makeGoal();
    }
}

int getLevelNumber(void) {
    printToPromptWindow("enter number of level");
    return getIntFromInput();
}

