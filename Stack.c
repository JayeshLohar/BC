#include<stdio.h>
#include<stdlib.h>
#include "Stack.h"

void n_init( n_stack *S ) {
    *S = NULL ;
    return ;
}

void n_push( n_stack *S, Num *N ) {
    n_node *nn = (n_node*)malloc(sizeof(n_node)) ;
    nn->num = N ;
    nn->next = *S ;
    *S = nn ;
    return ;
}
Num *n_pop( n_stack *S ) {
    Num *N ;
    n_stack temp = *S ;
    N = temp->num ;
    temp = temp->next ;
    *S = temp ;
    //free(temp) ;
    return N ;
}

int n_isempty (n_stack *s) {
	return *s == NULL;
}

void ch_init( ch_stack *S ) {
    *S = NULL ;
    return ;
}

void ch_push( ch_stack *S, char C ) {
    ch_node *nn = (ch_node*)malloc(sizeof(ch_node)) ;
    nn->ch = C ;
    nn->next = *S ;
    *S = nn ;
    return ;
}
char ch_pop( ch_stack *S ) {
    char C ;
    ch_stack temp = *S ;
    C = temp->ch ;
    temp = temp->next ;
    *S = temp ;
    //free(temp) ;
    return C ;
}

char ch_top(ch_stack *s) {
	char ch;
	ch = ch_pop(s);
	ch_push(s, ch);
	return ch;
}

int ch_isempty (ch_stack *s) {
	return *s == NULL;
}
