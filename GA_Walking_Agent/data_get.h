/*
// Data structure from stdin
*/
typedef struct {
    // indexing [row][col]
    char **labirynth;
    int max_time, row, col;
    int agent_row, agent_col;
} stdin_data;

/*
// Taking data from stdin
*/
stdin_data * take_stdin();