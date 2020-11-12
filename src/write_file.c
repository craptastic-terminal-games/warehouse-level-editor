/*                                           ### description ###                       
        File contains functions to handle writing the window data to a file.
*/

//---------------------------------------------- headers -------------------------------------------------
#include <stdio.h>
#include "write_file.h"
#include "window.h"
//---------------------------------------------- define --------------------------------------------------

//---------------------------------------------- enums ---------------------------------------------------


//---------------------------------------------- typedefs ------------------------------------------------


//---------------------------------------------- structs -------------------------------------------------


//---------------------------------------------- global vars ---------------------------------------------
static FILE *file;

//---------------------------------------------- prototypes ----------------------------------------------
static void loopWindows(void);
static void printWindowToFile(const WINDOW_S *const window);
//---------------------------------------------- code ----------------------------------------------------

static void printWindowToFile(const WINDOW_S *const window) {
    fprintf(file,"%d,%d,%d,%d\n",window->y,window->x,window->width,window->height);
}

static void loopWindows(void) {
    for(int i = 0; i <= WIN_AR->length;i++) {
        printWindowToFile(WIN_AR->ar[i]);
    }
}

void saveWindows(void) {
    const int num = getLevelNumber();
    char file_name[13];
    snprintf(file_name,13,"level_%d.dat",num);
    file = fopen(file_name,"w");
    fprintf(file,"%d,%d\n",HEIGHT,WIDTH);
    fprintf(file,"%d,%d\n",START_Y,START_X);
    fprintf(file,"%d,%d,6,3\n",GOAL->y,GOAL->x);
    loopWindows();
    fclose(file);
}

