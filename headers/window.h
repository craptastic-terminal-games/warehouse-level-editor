/*                                           ### description ###                       

*/

//---------------------------------------------- include guards ------------------------------------------
#ifndef WINDOW_D
//---------------------------------------------- headers -------------------------------------------------
#include <ncurses.h>
//---------------------------------------------- define --------------------------------------------------
#define WINDOW_D

//---------------------------------------------- enums ---------------------------------------------------
enum DIRECTION {UP,DOWN,LEFT,RIGHT};  //directions to move window

//---------------------------------------------- typedefs ------------------------------------------------
typedef struct WINDOW_S     WINDOW_S;
typedef struct WINDOW_ARRAY WINDOW_ARRAY;
typedef enum   DIRECTION    DIRECTION;
//---------------------------------------------- structs -------------------------------------------------
struct WINDOW_S {
    int height,width,x,y;
    WINDOW *window;
};

struct WINDOW_ARRAY {
    WINDOW_S **ar;
    int length,max_size;
};

//---------------------------------------------- global vars ---------------------------------------------
extern WINDOW_ARRAY *WIN_AR; //array to hold all the created boxes
extern WINDOW_S *GOAL;      //holds the goal location    
extern int WIDTH,HEIGHT,START_X,START_Y; //width and height of map. x and y locations for robot.

//---------------------------------------------- prototypes ----------------------------------------------

void initScreen(void);
void makeWindowArray(void);
void moveCurrentWindow(DIRECTION dir);
void deleteCurrentWindow(void);
void getStartXY(void);
void createNewWindow(void);
void makeGoal(void);
void printStartAndGoal(void);
int getLevelNumber(void);
#endif

