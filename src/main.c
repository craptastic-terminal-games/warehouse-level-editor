/*                                           ### description ###                       
    main file for level editor for warehouse robot. 
*/
//------------------------- License ----------------------------------
//  license: GPL 3.0        Author: github/return5
//
//  Copyright (C) <2020>  <return5>
//
//  This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
//---------------------------------------------- headers -------------------------------------------------
#include <stdlib.h>
#include "window.h"
#include "write_file.h"
//---------------------------------------------- define --------------------------------------------------

//---------------------------------------------- enums ---------------------------------------------------


//---------------------------------------------- typedefs ------------------------------------------------


//---------------------------------------------- structs -------------------------------------------------


//---------------------------------------------- global vars ---------------------------------------------
int WIDTH,HEIGHT,PLAY;

//---------------------------------------------- prototypes ----------------------------------------------
static void loopFunc(void);
static void getWidthHeight(void); 
static void quitProgram(void);
static int moveWindowAround(void);
static void getUserInput(void);

//---------------------------------------------- code ----------------------------------------------------

//get the width and height of the level form the user
static void getWidthHeight(void) {
    char *c     = NULL;
    size_t buff = 0;
    printf("please enter width: ");
    getline(&c,&buff,stdin);
    WIDTH = atoi(c);
    c     = NULL;
    printf("please enter height: ");
    getline(&c,&buff,stdin);
    HEIGHT = atoi(c);
    free(c);
}

static void quitProgram(void) {
    saveWindows();
    PLAY = 0;
    endwin();
}

static int moveWindowAround(void) {
    switch(getch()) {
        case '\n':return 0;
            break;
        case KEY_UP: moveCurrentWindow(UP);
            break;
        case KEY_DOWN: moveCurrentWindow(DOWN);
            break;
        case KEY_LEFT: moveCurrentWindow(LEFT);
            break;
        case KEY_RIGHT: moveCurrentWindow(RIGHT);
            break;
        case 'd': 
            deleteCurrentWindow();
            return 0;
        default: //do nothing.
            break;
    }
    return 1;
}


static void getUserInput(void) {
    switch(getch()) {
        case 'q': quitProgram();
            break;
        case 'c': 
            createNewWindow();
            while(moveWindowAround());
            break;
        default: //do nothing
            break;
    }
}

static void loopFunc(void) {
    while(PLAY) {
        getUserInput(); 
    }
}

int main(void) {
    getWidthHeight();
    PLAY = 1;
    initScreen();
    getStartXY();
    makeGoal();
    printStartAndGoal();
    makeWindowArray();
    loopFunc();
    return EXIT_SUCCESS;
}

