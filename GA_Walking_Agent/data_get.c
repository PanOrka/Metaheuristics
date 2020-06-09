#include <stdio.h>
#include <stdlib.h>

#include "data_get.h"

stdin_data * take_stdin() {
    stdin_data *data = malloc(sizeof(stdin_data));
    scanf("%d %d %d", &(data->max_time), &(data->row), &(data->col));
    data->labirynth = malloc(sizeof(char *)*(data->row));
    // indexing [row][col]
    for (int i=0; i<data->row; i++) {
        *(data->labirynth + i) = malloc(sizeof(char)*(data->col + 1)); // +1 for '\0'
        for (int k=0; k<data->col; k++) {
            scanf(" %c", (data->labirynth[i] + k));
            if (data->labirynth[i][k] == '5') {
                data->agent_row = i;
                data->agent_col = k;
            }
        }
        data->labirynth[i][data->col] = '\0';
    }

    return data;
}
