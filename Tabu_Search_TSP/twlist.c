#include "twlist.h"

void addElement(list *p, int value1, int value2, int value3) {
    if ((*p).head == NULL) { // wtedy cala lista jest pusta, zaczynamy od head (lewa)  H oooo T
        node *n = malloc(sizeof(node));
        node n1 = {NULL, NULL, value1, value2, value3};
        *n = n1;
        (*p).head = n;
    } else if ((*p).tail == NULL) { // wtedy jest head a nie ma taila
        node *n = malloc(sizeof(node));
        node n1 = {(*p).head, (*p).head, value1, value2, value3}; // zapetlamy do circular tail
        *n = n1;
        (*p).tail = n;
        (*((*p).head)).right = n; // zapetlamy do circular head
        (*((*p).head)).left = n;
    } else { // teraz juz mamy head i tail
        node *n = malloc(sizeof(node));
        node n1 = {(*p).tail, (*p).head, value1, value2, value3};
        *n = n1;
        (*((*p).tail)).right = n; // dodajemy referencje do nowego taila w starym tailu
        (*((*p).head)).left = n; // zapetlamy do circular
        (*p).tail = n; // zastapiamy nowym tailem
    }

    (*p).size++;
}

node * getElement(list *lista, int n) {
    int size = (*lista).size;
    if (size > 1) {
        node *el1;
        n%=size;
        if (n < 0) {
            n = size + n;
        }

        if (n >= size/2) {
            el1 = (*lista).tail;
            for (int i=size-1; i>n; i--) {
                el1 = (*el1).left;
            }
            return el1;
        } else {
            el1 = (*lista).head;
            for (int i=0; i<n; i++) {
                el1 = (*el1).right;
            }
            return el1;
        }
    } else if (size == 1) {
        return (*lista).head;
    } else {
        fprintf(stderr, "Error: List is empty\n");
        exit(1);
    }
}

void deleteElement(list *lista, int n) {
    if ((*lista).size > 1) {
        node *el1;

        n %= (*lista).size;
        if (n < 0) {
            n = (*lista).size + n;
        }

        if (n >= (*lista).size/2) { // RIGHT SIDE
            el1 = (*lista).head;
            for (int i=(*lista).size-1; i>n; i--) { // teraz dostaniemy element n+1
                el1 = (*el1).left;
            }

            node *el_n = (*el1).left; // tutaj mamy element n-ty
            node *el_k = (*el_n).left; // tutaj mamy element n-1
            (*el1).left = el_k;
            (*el_k).right = el1;
            free(el_n);

            if (n == (*lista).size-1) {
                (*lista).tail = el_k; // jak usuniemy tail to ten z lewej od starego taila staje sie tailem
            }

            (*lista).size--;
        } else { // LEFT SIDE
            el1 = (*lista).tail;
            for (int i=0; i<n; i++) { // teraz mamy n-1
                el1 = (*el1).right;
            }

            node *el_n = (*el1).right; // tutaj mamy n
            node *el_k = (*el_n).right; // tutaj mamy n+1
            (*el1).right = el_k;
            (*el_k).left = el1;
            free(el_n);

            if (n == 0) {
                (*lista).head = el_k; // jak usuniemy head to n+1 staje sie headem bo n-1 jest tailem
            }

            (*lista).size--;
        }
    } else if ((*lista).size == 1) {
        free((*lista).head);
        (*lista).head = NULL;
        (*lista).tail = NULL;
        (*lista).size--;
    } else {
        fprintf(stderr, "Error: List is empty\n");
    }
}

void deleteList(list *lista) {
    if ((*lista).size <= 1) {
        free((*lista).head);
        (*lista).head = NULL;
        (*lista).tail = NULL;
        (*lista).size = 0;
    } else {
        node *el1 = (*lista).head;
        while (el1 != (*lista).tail) {
            el1 = (*el1).right;
            free((*el1).left);
        }
        free(el1);
        (*lista).head = NULL;
        (*lista).tail = NULL;
        (*lista).size = 0;
    }
}

list * createList() {
    list *lista = malloc(sizeof(list));
    list l1 = {NULL, NULL, 0};
    *lista = l1;

    return lista;
}