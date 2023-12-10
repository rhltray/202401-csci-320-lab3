#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"



extern int** sudoku_board; 
int* validation; 




int** read_board_from_file(char* filename){
    sudoku_board = (int**)malloc(sizeof(int*)*ROW_SIZE); 
    FILE *fp = NULL; 
    for(int row = 0; row < ROW_SIZE; row++)
	    sudoku_board[row] = (int*)malloc(sizeof(int)*COL_SIZE); 
    
    fp = fopen(filename,"r"); 
    for(int x = 0; x < ROW_SIZE; x++) 
        for(int y = 0; y < COL_SIZE; y++) 
            fscanf(fp, "%d%*c", &sudoku_board[x][y]);

    fclose(fp); 
    return sudoku_board; 
}
