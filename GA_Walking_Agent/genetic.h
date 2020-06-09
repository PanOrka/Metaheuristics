/*
// Selection, sorted generation
*/
void selection(generation *g, double whole_size_ratio, double best_size_ratio);

/*
// Mutation probability from [0, 1000]
*/
void mutate(generation *g, stdin_data *data, int probability);

/*
// Reproduce random
*/
void reproduce(generation *g, stdin_data *data, int probability);
