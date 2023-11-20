#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

extern int** sudoku_board;
int* worker_validation;

int** readboard(char* filename){
    FILE *fp = NULL;
 if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int** sudoku_board = (int**)malloc(sizeof(int*) * ROW_SIZE);
    if (sudoku_board == NULL) {
        perror("Error allocating memory for rows");
        exit(EXIT_FAILURE);
    }

    for (int r = 0; r < ROW_SIZE; r++) {
        sudoku_board[r] = (int*)malloc(sizeof(int) * COL_SIZE);
        if (sudoku_board[r] == NULL) {
            perror("Error allocating memory for columns");
            exit(EXIT_FAILURE);
        }

        for (int x = 0; x < COL_SIZE; x++) {
            if (fscanf(fp, "%d,", &sudoku_board[r][x]) != 1) {
                perror("Error reading from file");
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(fp);
    return sudoku_board;

	

}


void* validate(void* arg) {
    param_struct* params = (param_struct*)arg;
    int* validation = &worker_validation[params->id];

    for (int r = params->starting_row; r <= params->ending_row; r++) {
        for (int x = params->starting_col; x <= params->ending_col; x++) {
            for (int k = r + 1; k <= params->ending_row; k++) {
                if (params->is_row && sudoku_board[r][x] == sudoku_board[k][x]) {
                    *validation = 0;
                    pthread_exit(NULL);
                } else if (!params->is_row && sudoku_board[x][r] == sudoku_board[x][k]) {
                    *validation = 0;
                    pthread_exit(NULL);
                }
            }
        }
    }

    *validation = 1;
    pthread_exit(NULL);
}



int is_board_valid(){
    pthread_t* tid;
    pthread_attr_t attr;
    param_struct* parameter;
    worker_validation = malloc(NUM_OF_THREADS * sizeof(int));

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int index = 0;
   // This for loop validates by each row
   for (int r = 0; r < ROW_SIZE; r++) {
        param_struct* params = malloc(sizeof(param_struct));
        params->id = index++;
        params->starting_row = r;
        params->ending_row = r;
        params->starting_col = 0;
        params->ending_col = COL_SIZE - 1;
        params->is_row = 1;
        pthread_create(&tid[params->id], &attr, validate, (void*)params);
    }
    // This for loop validates by each column
    for (int x = 0; x < COL_SIZE; x++) {
        param_struct* params = malloc(sizeof(param_struct));
        params->id = index++;
        params->starting_row = 0;
        params->ending_row = ROW_SIZE - 1;
        params->starting_col = x;
        params->ending_col = x;
        params->is_row = 0;
        pthread_create(&tid[params->id], &attr, validate, (void*)params);
    }
    // This for loop validates by subgrid
    for (int r = 0; r < ROW_SIZE; r += 3) {
        for (int x = 0; x < COL_SIZE; x += 3) {
            param_struct* params = malloc(sizeof(param_struct));
            params->id = index++;
            params->starting_row = r;
            params->ending_row = r + 2;
            params->starting_col = x;
            params->ending_col = x + 2;
            params->is_row = 1; 
            pthread_create(&tid[params->id], &attr, validate, (void*)params);
        }
    }

     for (int r = 0; r < NUM_OF_THREADS; r++) {
        pthread_join(tid[r], NULL);
    }

    int b_valid = 1;
    for (int r = 0; r < NUM_OF_THREADS; r++) {
        if (worker_validation[r] == 0) {
            b_valid = 0;
            break;
        }
    }

    free(worker_validation);
    pthread_attr_destroy(&attr);

    return b_valid;





}