#include<stdio.h>
#include<stdlib.h>
#include "List.h"

/* Takes argument as Num pointer
 * Initializes Number data type
 * Initializes head and tail pointer to NULL
 */
void init( Num *N ) {
    N->head = NULL ;
    N->tail = NULL ;
    N->sign = 0 ;
    N->integer = 0 ;
    N->decimal = -1 ;
}

/* Takes argument as Num pointer and char data to be inserted
 * Append the node at last
 * If decimal = -1, then append node integer++
 * If decimal >= 0, means of float type, decimal++
 * If we append '.' then initialize decimal to -1
 */
void append( Num *N, char ch ) {
    if( ch == '.' ){
        if( N->decimal > -1 ) {  // Here Number already contains '.' so again it does not append '.' and returns
            printf("Invalid Number...!!");
            return ;
        }
        N->decimal = 0 ;
        return ;
    }

    node *newnode ;
    newnode = (node*)malloc(sizeof(node)) ;
    if( !newnode )
        return ;
    newnode->data = ch ;
    newnode->next = NULL ;
    newnode->prev = NULL ;

    if( ! N->head ) {            //Head is pointing to NULL means Num is empty
        N->head = newnode ;
        N->tail = newnode ;
        if( N->decimal == 0 )
            N->decimal ++ ;
        else
            N->integer ++ ;
        return ;
    }
    newnode->prev = N->tail ;
    N->tail->next = newnode ;
    N->tail = newnode ;
    if( N->decimal > -1 ) {
        N->decimal ++ ;
    }
    else {
        N->integer ++ ;
    }

    return ;
}

/* Adds node at start of Number
 * As node is adding at start so increase in integer type integer++
 * If try to append '.' to int type then it convert complete number to float ex : convert 234 to 0.234
 */
void push( Num *N, char ch) {
    if( ch == '.' ){
        if( N->decimal > -1 ) {
            printf("Invalid Number...!!");
            return ;
        }
        N->decimal = N->integer ;
        N->integer = 0 ;
        return ;
    }

    node *newnode ;
    newnode = (node*)malloc(sizeof(node)) ;
    if( !newnode )
        return ;
    newnode->data = ch ;
    newnode->next = NULL ;
    newnode->prev = NULL ;

    if( ! N->head ) {//Head is pointing to NULL means Num is empty
        N->head = newnode ;
        N->tail = newnode ;
        N->integer ++ ;
        return ;
    }

    newnode->next = N->head ;
    N->head->prev = newnode ;
    N->head = newnode ;
    N->integer ++ ;

    return ;
}

/* Deletes the starting element of Number
 * If integer part present then integer--
 * If integer part is not present then decimal--
 */
void pop( Num *N ) {
    if( N->integer == 0 && ( N->decimal == 0 || N->decimal == -1 ))
        return ;

    if( N->integer == 1 && N->decimal <= 0 ) {
        init( N );
        return ;
    }
    else if( N->integer == 0 && N->decimal == 1 ) {
        init( N );
        return ;
    }

    if( N->integer > 0 )
        N->integer-- ;
    else if( N->decimal > -1 )
        N->decimal-- ;
    node *p = N->head ;
    N->head = p->next ;
    N->head->prev = NULL ;
    free( p ) ;
    return ;
}

/* Deletes the last element of Number
 * Here we 1st look for decimal part as we deleting from last
 * If decimal part present then decimal--
 * If decimal part is not present then integer--
 */
void del( Num *N ) {
    if( N->integer == 0 && ( N->decimal == 0 || N->decimal == -1 ))
        return ;

    if( N->integer == 1 && N->decimal <= 0 ) {
        init( N );
        return ;
    }
    else if( N->integer == 0 && N->decimal == 1 ) {
        init( N );
        return ;
    }
    node *q = N->tail ;
    N->tail = q->prev ;
    N->tail->next = NULL ;
    free( q ) ;
    if( N->decimal > 0 )
        N->decimal-- ;
    else
        N->integer-- ;
    return ;
}

void distroy( Num *N ){
    if( !N )
        return ;
    if( N->decimal == -1 )
        N->decimal = 0 ;
    while( N->decimal )
        del( N);
    while( N->integer)
        pop(N);
    free(N);
    return ;
}

/* Deletes unnecessary starting zeros
 * Deletes only integer part starting zeros
 * Do not delete zeros of decimal part
 */
void pop_starting_zeros( Num *N ) {
    if( N->integer == 0 )
        return ;
    while( N->integer && N->head && N->head->data == '0' )
        pop( N );
    return ;
}

/* Deletes unnecessary last zeros
 * Deletes only decimal part zeros
 * Do not delete zeros of integer part
 */

void del_last_zeros( Num *N ) {
    if( N->decimal == -1 )
        return ;
    while( N->decimal && N->tail && N->tail->data == '0' ) {
        del( N ) ;
    }
    return ;
}


//Deletes unnecessary zeros on both sides
void delete_zeros_of_both_sides( Num *N ) {
    pop_starting_zeros( N ) ;
    del_last_zeros( N ) ;
    return ;
}

void adjust_Number( Num *N1, Num *N2 ) {
    int d1, d2 , l1, l2 , i;
    d1 = N1->decimal ;
    d2 = N2->decimal ;
    l1 = N1->integer ;
    l2 = N2->integer ;

    if( d1 > -1 || d2 > -1 ) {
        if( d1 == d2 );
        else if( d1 > d2 ) {
            if( d2 == -1 ) {
                d2 = 0 ;
                N2->decimal = 0 ;
            }
            for( i = 0 ; i < d1-d2 ; i++ )
                append( N2, '0' );
        }
        else {
            if( d1 == -1 ) {
                d1 = 0 ;
                N1->decimal = 0 ;
            }
            for( i = 0 ; i < d2-d1 ; i++ )
                append( N1, '0' );
        }
    }
    if( l1 > l2 )
        for( i = 0 ; i < l1 - l2 ; i++ )
            push( N2, '0' );

    else if( l2 > l1 )
        for( i = 0 ; i < l2 - l1 ; i++ )
            push( N1, '0' );
}

void swap_Number( Num *N1, Num *N2 ) {
    Num tmp ;
    copy_Number( N1, &tmp);
    copy_Number( N2, N1);
    copy_Number( &tmp, N2);
    return ;
}


/* Compare two numbers
 * Takes 2 Numbers as argument
 * Firstly it deletes starting zeros of both numbers
 * Returns 1 if 1st Number is large
 * Returns 0 if 2nd Number is large i.e, 1st Number is smaller
 * Returns -1 if both Number are equal ( 2 and 2.0 are both equal Numbers )
 */
 int compare( Num *N1, Num *N2 ) {
    if( !N1 || !N2 )
        return 0 ;
    adjust_Number( N1, N2);
    node *p = N1->head, *q = N2->head ;
    while( p && q ){
        if( p->data > q->data )
                return 1 ;
        if( p->data < q->data )
            return 0 ;
        p = p->next ;
        q = q->next ;
    }
    return -1 ;
}

/* Prints the Number
 * If Number is NULL prints 0
 * Prints negative sign ( - ) if Number sign value 1 otherwise do not print
 * Prints the number as per Number integer length and decimal length
 * While printing integer part gives comma after 3 digits considering from decimal point (.)
 */
void traverse( Num *N ){
    if( !N )
        return ;

    delete_zeros_of_both_sides( N );
    node *p = N->head ;
    int i = N->integer , d = N->decimal , s = N->sign ;
    int j = 0 , comma = i ;
    if( s )
        printf("-");
    if( !i )
        printf("0");
    while( p && j < i ) {
        //if( j && comma % 3 == 0 )
        //    printf(",");
        printf("%c", p->data );
        p = p->next ;
        j++ ;
        comma-- ;
    }
    if( d > 0 ){
        j = 0 ;
        printf(".");

        while( p && j < d ) {
            printf("%c", p->data );
            p = p->next ;
            j++ ;
        }
    }
    printf("\n");
    return ;
}

void save( FILE *fp, Num *N ){
    fprintf(fp,"Sol : ");
    delete_zeros_of_both_sides( N );
    node *p = N->head ;
    int i = N->integer , d = N->decimal ;
    int j = 0 , comma = i ;
    if( N->sign )
        fprintf(fp, "-");
    if( !i )
        fprintf(fp, "0");
    while( p && j < i ) {
        fprintf(fp, "%c", p->data );
        p = p->next ;
        j++ ;
        comma-- ;
    }
    if( d > 0 ){
        j = 0 ;
        fprintf(fp, ".");
        while( p && j < d ) {
            fprintf(fp, "%c", p->data );
            p = p->next ;
            j++ ;
        }
    }
    fprintf(fp, "\n");
    return ;
}

/* Returns 0 if Number is zero
 * Returns 1 if Number is not zero
 */
int zero_Number( Num *N ) {
    if( !N )
        return 0 ;
    node *p = N->head ;
    while( p ) {
        if( p->data != '0' )
            return 1 ;
        p = p->next ;
    }
    return 0 ;
}

/* Copy Number 1 to Number 2
 */
void copy_Number( Num *N1, Num *N2 ) {
    init( N2 );
    node *p = N1->head ;
    while( p ) {
        append( N2, p->data );
        p = p->next ;
    }
    N2->decimal = N1->decimal ;
    N2->integer = N1->integer ;
    N2->sign = N1->sign ;
    return ;
}
