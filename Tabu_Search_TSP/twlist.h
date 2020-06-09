#include <stdio.h>
#include <stdlib.h>

// node in list
typedef struct {
    void *left;
    void *right;
    int value1, value2, value3;
} node;

// list
typedef struct {
    node *head;
    node *tail;

    int size;
} list;

void addElement(list *p, int value1, int value2, int value3);

node * getElement(list *lista, int n);
void deleteElement(list *lista, int n);
void deleteList(list *lista);

list * createList();