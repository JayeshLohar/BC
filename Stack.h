#include "List.h"

/* Structure of node for Number
 */
typedef struct n_node{
    Num *num ;
    struct n_node *next ;
}n_node ;

/* Structure of stack for Number
 */
typedef n_node *n_stack ;

void n_init( n_stack *S );
void n_push( n_stack *S, Num *N );
Num *n_pop( n_stack *S );
int n_isempty (n_stack *s);

/* Structure of node for character operator
 */
typedef struct ch_node{
    char ch ;
    struct ch_node *next ;
}ch_node ;

/* Structure of stack for character operator
 */
typedef ch_node *ch_stack ;

void ch_init( ch_stack *S );
void ch_push( ch_stack *S, char C );
char ch_pop( ch_stack *S );
char ch_top(ch_stack *s);
int ch_isempty (ch_stack *s);
