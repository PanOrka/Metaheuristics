/*
// Traversing given genom
*/
void traverse(genom *g, stdin_data *data);

/*
// Check if agent is in exit or near exit
*/
bool check_in_exit(int agent_row, int agent_col, stdin_data *data);

/*
// Make random moves
*/
bool random_moves(genom *g, stdin_data *data, int max_genom_size);

/*
// Make move
*/
void move(char step, int *agent_row, int *agent_col);
