#include <stdbool.h>

/*
// Genom structure
*/
typedef struct {
    char *path;
    int size;
    bool did_exit;
} genom;

/*
// Generation structure
*/
typedef struct {
    genom *gen;
    int max_size, actual_size, max_genom_size;
} generation;
