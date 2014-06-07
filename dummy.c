#include <stdio.h>

void lpCursorOn(void){
    printf("lpCursorOn()\n");
}

void lpCursorOff(void){
    printf("lpCursorOff()\n");
}

void lpPosxy(int x, int y){
    printf("lpPosxy(%i, %i)\n", x, y);
}
