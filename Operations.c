#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "List.h"
#include "Operations.h"

/* Takes argument as Num pointer of 2 Numbers
 * Adds 2 numbers and returns Nadd Number
 */
Num *add( Num *N1, Num *N2 ) {
    if( !N1 || !N2 )
        return NULL ;

    Num *Nadd = (Num*)malloc(sizeof(Num)) ;
    init( Nadd );
    if( N1->sign == N2->sign )  // a + b or -a + (-b)
        Nadd->sign = N1->sign ;
    else if( N1->sign == 0 && N2->sign == 1 ) {  // a + (-b) i.e, equivalent to  a - b
        N2->sign = 0 ;
        Nadd = sub( N1, N2 );
        N2->sign = 1 ;
        return Nadd ;
    }
    else {   // -a + b i.e, equivalent to b - a
        N2->sign = 1 ;
        Nadd = sub( N1, N2 );
        N2->sign = 0 ;
        return Nadd ;
    }

    adjust_Number( N1, N2 );
    node *p = N1->tail , *q = N2->tail ;
    int carry = 0 ;
    char c ;

    while( p && q ) {
        c = p->data + q->data - 48 + carry ;
        if( c > 57 ) {
            c = c - 10 ;
            carry = 1 ;
        }
        else
            carry = 0 ;
        push( Nadd, c );
        p = p->prev ;
        q = q->prev ;
    }
    if( carry == 1 )
        push( Nadd, '1');

    if( N1->decimal > 0 ) {
        Nadd->decimal = N1->decimal ;
        Nadd->integer = Nadd->integer - Nadd->decimal ;
    }

    /*free(p);
    free(q);*/
    return Nadd ;
}

/* Takes arguments as Num pointer of 2 Numbers
 * Subtract 2 NUmbers
 * Returns Nsub
 */
Num *sub( Num *N1, Num *N2 ) {
    if( !N1 || !N2 )
        return NULL ;

    int flag = 0 ;
    //printf("\nJayesh : %d \n", compare( N1, N2));
    if( compare( N1, N2) == 0 && N1->sign == N2->sign ) {// If 1st Number is smaller than 2nd Number
        swap_Number( N1, N2 );
        flag = 1 ;                // Set flag to 1
    }

    Num *Nsub = (Num*)malloc(sizeof(Num)) ;
    init( Nsub );
    if( N1->sign == N2->sign ) {  // a - b or -a - ( -b ) i.e, b - a for this sign will change
        Nsub->sign = N1->sign ;
    }
    else if( N1->sign == 0 && N2->sign == 1 ) {  // a - ( -b ) i.e, a + b
        N2->sign = 0 ;
        Nsub = add( N1, N2 );
        N2->sign = 1 ;
        return Nsub ;
    }
    else {                                       // -a - b i.e, -a + ( -b )
        N2->sign = 1 ;
        Nsub = add( N1, N2 );
        N2->sign = 0 ;
        return Nsub ;
    }

    adjust_Number( N1, N2 );
    node *p = N1->tail , *q = N2->tail ;

    int borrow = 0 ;
    char c ;

    while( p && q ) {
        c = p->data - q->data + 48 - borrow ;
        if( c < 48 ) {
            c = c + 10 ;
            borrow = 1 ;
        }
        else
            borrow = 0 ;
        push( Nsub, c );
        p = p->prev ;
        q = q->prev ;
    }

    if( N1->decimal > 0 ) {
        Nsub->decimal = N1->decimal ;
        Nsub->integer = Nsub->integer - Nsub->decimal ;
    }

    if( flag ) {             // Swaps the both number again
        swap_Number( N1, N2 );
        if( Nsub->sign == 0 )
            Nsub->sign = 1 ;
        else
            Nsub->sign = 0 ;
    }

    //free(p);
    //free(q);
    return Nsub ;
}

/* Takes argument as Num pointer of 2 Numbers
 * multipy 2 Numbers
 * Returns Nmul
 * Here 1st Number is multiplied by individual by single digit of 2nd Number
 * This is added to Nmul
 */
Num *multipy( Num *N1, Num *N2 ) {
    if( !N1 || !N2 )
        return NULL ;

    Num *Nmul, *M1 ;
    Nmul = (Num*)malloc(sizeof(Num)) ;
    M1 = (Num*)malloc(sizeof(Num)) ;
    init( Nmul );

    if( zero_Number( N1 ) == 0 || zero_Number( N2 ) == 0 )
        return Nmul ;

    int d1 = N1->decimal, d2 = N2->decimal, k = 0 ;
    node *q = N2->tail ;

    while( q ) {
        M1 = mul_fun( N1, q->data, k ) ;
        Nmul = add( Nmul, M1 );
        q = q->prev ;
        k++ ;
    }

    if( d1 == -1 && d2 == -1 )
        Nmul->decimal = -1 ;
    else if( d1 == -1 ) {
        Nmul->decimal = d2 ;
        Nmul->integer = Nmul->integer - d2 ;
    }
    else if( d2 == -1 ) {
        Nmul->decimal = d1 ;
        Nmul->integer = Nmul->integer - d1 ;
    }
    else {
        Nmul->decimal = d1 + d2 ;
        Nmul->integer = Nmul->integer - Nmul->decimal ;
    }

    if( N1->sign != N2->sign )
        Nmul->sign = 1 ;

    delete_zeros_of_both_sides( Nmul );

    //free(q);
    //distroy(M1);
    return Nmul ;
}

/* Takes arguments as Num pointer char data and int K
 * Here Num is multiplied by char i.e, single digit
 * K is for appending required zeros while multipying by its place value
 * Returns Num M ( Multiplication of Num N1 and char data )
 */
Num *mul_fun( Num *N1, char data , int k ) {
    if( !N1 )
        return NULL ;
    Num *M ;
    M = (Num*)malloc(sizeof(Num)) ;
    init( M );

    node *p = N1->tail ;
    char ch ;
    int carry = 0 , i;

    for( i = 0 ; i < k ; i++ )
        push( M, '0' );

    while( p ) {
        ch = ( p->data - 48 ) * ( data - 48 ) + carry ;
        carry = ch / 10;
        ch = ch % 10 + 48;
        push( M, ch );
        p = p->prev ;
    }
    if( carry > 0 ) {
        carry = carry + 48 ;
        push( M, carry );
    }

    //free(p);
    return M ;
}

/* Takes argument as Num pointer of 2 Numbers
 * Divides 2 Numbers
 * Returns Ndiv
 * It always returns quotient in integer form
 * If any Number is float though it returns solution in complete form only i.e, in integer form
 */
Num *divide( Num *N1, Num *N2, int scale ) {
    if( scale < 0 )
        scale = 0 ;

    if( !N1 || !N2 )
        return NULL ;

    delete_zeros_of_both_sides( N1 );
    delete_zeros_of_both_sides( N2 );

    Num *Ndiv ;
    Ndiv = (Num*)malloc(sizeof(Num)) ;
    init( Ndiv ) ;

    if( N1->integer + scale < N2->integer )
        return Ndiv ;

    Num num1, num2 ;
    copy_Number( N1, &num1);
    copy_Number( N2, &num2);
    adjust_Number( &num1, &num2);

    int i ;

    if( zero_Number( N2 ) == 0 ) {
        printf("Operation Cannot be performed..!!\n");
        return NULL ;
    }
    else if( zero_Number( N1 ) == 0 )
        return Ndiv ;

    for( i = 0 ; i < scale ; i++ )
        append( &num1, '0' );

    if( num1.decimal > -1 ) {
        num1.integer = num1.integer + num1.decimal ;
        num1.decimal = -1 ;
    }
    if( num2.decimal > -1 ) {
        num2.integer = num2.integer + num2.decimal ;
        num2.decimal = -1 ;
    }

    pop_starting_zeros( &num2 ) ;

    Num *a, *mul;
    a = (Num*)malloc(sizeof(Num)) ;
    mul = (Num*)malloc(sizeof(Num)) ;
    init( a ) ;
    char ch ;

    node *p = num1.head, *q = num2.head ;
    while( q->next ) {
        append( a, p->data ) ;
        p = p->next ;
        q = q->next ;
    }

    while( p ) {
        append( a, p->data );
        for( ch = '9' ; ch >= '0' ; ch-- ) {
            init( mul );
            mul = mul_fun( &num2, ch, 0 );
            if( compare( mul, a ) == 0 || compare( mul, a ) == -1 ) {
                append( Ndiv, ch ) ;
                ch = 0 ;
            }
        }
        a = sub( a, mul );
        p = p->next ;
    }

    if( Ndiv->integer >= scale ) {
        Ndiv->integer = Ndiv->integer - scale ;
        Ndiv->decimal = scale ;
    }
    else {
        for( i = 0 ; i < scale - Ndiv->integer ; i++ )
            push( Ndiv, '0' );
        push( Ndiv, '.' );
    }

    if( N1->sign != N2->sign )
        Ndiv->sign = 1 ;

    delete_zeros_of_both_sides( Ndiv );

    /*free(p);
    free(q);
    distroy( &num1);
    distroy( &num2);
    distroy( a);
    distroy( mul);*/
    return Ndiv ;
}

/* Takes argument as Num pointer of 2 Numbers
 * Returns modula of two numbers
 */
 Num *modula( Num *N1, Num *N2 ) {
    if( !N1 || !N2 )
        return NULL ;
    return sub( N1, multipy( N2, divide( N1, N2, 0 )));
}

/*Num *modula( Num *N1, Num *N2 ) {
    if( !N1 || !N2 )
        return NULL ;

    Num *Nmod, *Ndiv, *Nmul ;
    Nmod = (Num*)malloc(sizeof(Num)) ;
    Ndiv = (Num*)malloc(sizeof(Num)) ;
    Nmul = (Num*)malloc(sizeof(Num)) ;
    init( Nmod ) ;

    delete_zeros_of_both_sides( N1 );
    delete_zeros_of_both_sides( N2 );

    Ndiv = divide( N1, N2, 0 ) ;
    Nmul = multipy( N2, Ndiv ) ;
    Nmod = sub( N1, Nmul ) ;

    delete_zeros_of_both_sides( Nmod );
    return Nmod ;
}*/

Num *length( Num *N ){
    Num *Nlen ;
    Nlen = (Num*)malloc(sizeof(Num)) ;
    init( Nlen ) ;

    if( !N ){
        append( N, '0');
        return Nlen ;
    }
    else if( N->integer == 0 && N->decimal < 1 ) {
        append( N, '1');
        return Nlen ;
    }
    node *p = N->head ;
    Num S ;
    init(&S);
    append( &S, '1');
    while( p ){
        Nlen = add( Nlen, &S);
        p = p->next ;
    }
    //free( p);
    return Nlen ;
}

/* Returns the square of given Number
 */
Num *square( Num *N ) {
    if( !N )
        return NULL ;

    Num *Nsq ;
    Nsq = (Num*)malloc(sizeof(Num)) ;
    init( Nsq ) ;
    Nsq = multipy( N, N ) ;
    return Nsq ;
}

/* Returns the cube of given Number
 */
Num *cube( Num *N ) {
    if( !N )
        return NULL ;

    Num *Ncube = (Num*)malloc(sizeof(Num)) ;
    init( Ncube ) ;
    Ncube = multipy( N, N ) ;
    Ncube = multipy( N, Ncube ) ;
    return Ncube ;
}

/* Raised the Number N1 to power N2
 * Power always should be in Integer form
 * If power is as 3.4 then it will raised power to 3
 * Gives warning for that
 */
Num *power( Num *N1, Num *N2 ) {
    if( !N1 || !N2 )
        return NULL ;

    Num *Npow = (Num*)malloc(sizeof(Num)) ;
    init( Npow );
    if( zero_Number( N1 ) == 0 ) {
        append( Npow, '0' );
        return Npow ;
    }

    delete_zeros_of_both_sides( N1 ) ;
    delete_zeros_of_both_sides( N2 ) ;

    if( N2->decimal > 0 ) {
        printf("Runtime warning, Non-zero scale in Exponent..!!\n");
        while( N2->decimal )
            del( N2 );
    }

    int flag = 0 ;
    if( N2->sign == 1 ) {
        N2->sign = 0 ;
        flag = 1 ;
    }

    append( Npow, '1' );
    if( zero_Number( N2 ) == 0 )
        return Npow ;

    Num S ;
    init( &S );
    append( &S, '1' ) ;

    while( zero_Number( N2 ) != 0 ) {
        Npow = multipy( N1, Npow );
        N2 = sub( N2, &S ) ;
    }

    if( flag ) {
        Npow = divide( &S, Npow, 30 );
    }

    //distroy( &S);
    return Npow ;
}

Num *square_root( Num *N, int rscale ) {
    if( !N )
        return NULL ;

    if( rscale < 0 )
        rscale = 0 ;
    rscale = rscale * 2;

    if( N->sign == 1 ) {
        printf("Square root of Negative Number is not be real number..!!\n");
        return NULL ;
    }

    int i ;
    Num *Nsqrt = (Num*)malloc(sizeof(Num)) ;
    init( Nsqrt );

    delete_zeros_of_both_sides( N);

    if( N->integer % 2 == 1 )
        push( N, '0');
    if( N->decimal == -1 ){
        append( N, '.');
        for( i = 0 ; i < rscale ; i++ )
            append( N, '0');
    }
    else if( N->decimal % 2 == 1 )
        append( N, '0');

    if( N->decimal < rscale )
        while( N->decimal < rscale )
            append( N, '0');
    else
        while( N->decimal > rscale )
            del( N);

    node *p = N->head ;

    char ch ;
    Num *S, *M, *A ;
    S = (Num*)malloc(sizeof(Num));
    M = (Num*)malloc(sizeof(Num));
    A = (Num*)malloc(sizeof(Num));
    init(S);
    init(M);
    init(A);

    while( p ){
        for( i = 0 ; p && i < 2 ; i++ ){
            append( S, p->data );
            p = p->next ;
        }

        A = mul_fun( Nsqrt, '2', 0);
        for( ch = '9' ; ch >= '0' ; ch-- ){
            append( A, ch);
            M = mul_fun( A, ch, 0);
            if( compare( M, S) == 0 || compare( M,S) == -1 ){
                append( Nsqrt, ch );
                ch = 0;
            }
            del(A);
        }
        S = sub( S, M);
    }

    rscale = rscale / 2;
    if( Nsqrt->integer >= rscale ){
        Nsqrt->integer = Nsqrt->integer - rscale ;
        Nsqrt->decimal = rscale ;
    }
    else{
        for( i = 0 ; i < rscale - Nsqrt->integer ; i++ )
            push( Nsqrt, '0');
        append( Nsqrt, '.');
    }

    //free( p);
    //distroy( S);
    //distroy( M);
    //distroy( A);
    return Nsqrt ;
}

/* Calculate the factorial of Number
 * Number should be positive and Integer type
 * If Number is of float type as 4.5
 * Then returns factorial of 4 and gives warning
 * Returns 0 for Factorial of Negative Number
 */
Num *fact( Num *N ) {
    if( !N )
        return NULL ;

    Num *Nfact = (Num*)malloc(sizeof(Num)) ;
    init( Nfact );
    append( Nfact, '1' );

    delete_zeros_of_both_sides( N ) ;

    if( zero_Number( N ) == 0 )
        return Nfact ;

    if( N->sign == 1 ) {
        printf("Factorial of Negative Number can not be determined..!!\n");
        init( Nfact );
        return Nfact ;
    }

    if( N->decimal > 0 ) {
        printf("Runtime warning, Number should be Integer type..!!\n");
        while( N->decimal )
            del( N );
    }

    Num S ;
    init( &S );
    append( &S, '1' ) ;

    while( zero_Number( N ) != 0 ) {
        Nfact = multipy( N, Nfact );
        N = sub( N, &S );
    }

    return Nfact ;
}

float cosine_angle(float n) {
    int  i;
    float a, cos = 1 , f =1;
    a = n * 3.141592 / 180;
    for( i = 1 ; i <= 100 ; i++ ) {
        f = -1 * f * a * a / (2 * i * (2 * i - 1));
        cos = cos + f;
    }
    return cos ;
}

float sine_angle(float n) {
    int  i;
    float sin, f;
    n = n * 3.14159 / 180;
    f = n;
    sin = n;
    for( i = 1 ; i <= 100 ; i++ ) {
        f = -1 * f * n * n / (2 * i * (2 * i + 1));
        sin = sin + f;
    }
    return sin ;
}

/* Returns the sine value for given angle
 */
Num *sine( Num *N ){
    if( !N )
        return NULL ;

    Num *angle, *degree, *Nsin ;
    angle = (Num*)malloc(sizeof(Num)) ;
    degree = (Num*)malloc(sizeof(Num)) ;
    Nsin = (Num*)malloc(sizeof(Num)) ;

    init( angle ) ;
    init( degree ) ;
    init( Nsin ) ;

    int i = 0 , j = 8 ;
    append( angle, '3' ) ;
    append( angle, '6' ) ;
    append( angle, '0' ) ;

    degree = modula( N, angle );
    append( angle, '.' ) ;
    for( i = 0 ; i < 5 ; i++ )
        append( angle, '0' ) ;

    adjust_Number( angle, degree ) ;

    float n, sin, power = 100 ;
    node *p = degree->head ;
    while( p && j ) {
        n = n + ( p->data - 48 )* power ;
        p = p->next ;
        power = power / 10 ;
        j-- ;
    }
    if( degree->sign == 1 )
        n = n * -1 ;

    sin = sine_angle( n ) ;

    if( sin < 0 ) {
        Nsin->sign = 1 ;
        sin = sin * -1 ;
    }

    sin = sin + 0.000004 ;
    int s = sin * 100000 ;
    char ch ;

    for( i = 0 ; i < 6 ; i++ ) {
        ch = s % 10 + 48 ;
        s = s / 10 ;
        push( Nsin, ch ) ;
    }

    Nsin->integer = 1 ;
    Nsin->decimal = 5 ;

    //distroy( angle);
    //distroy( degree);
    return Nsin ;
}

/* Returns the cosine value for given angle
 */
Num *cosine( Num *N ) {
    if( !N )
        return NULL ;
    Num *angle, *degree, *Ncos ;
    angle = (Num*)malloc(sizeof(Num)) ;
    degree = (Num*)malloc(sizeof(Num)) ;
    Ncos = (Num*)malloc(sizeof(Num)) ;

    init( angle ) ;
    init( degree ) ;
    init( Ncos ) ;

    int i = 0 , j = 8 ;
    append( angle, '3' ) ;
    append( angle, '6' ) ;
    append( angle, '0' ) ;

    degree = modula( N, angle );
    append( angle, '.' ) ;
    for( i = 0 ; i < 5 ; i++ )
        append( angle, '0' ) ;

    adjust_Number( angle, degree ) ;

    float n, cos, power = 100 ;
    node *p = degree->head ;
    while( p && j ) {
        n = n + ( p->data - 48 )* power ;
        p = p->next ;
        power = power / 10 ;
        j-- ;
    }
    cos = cosine_angle( n ) ;

    if( cos < 0 ) {
        Ncos->sign = 1 ;
        cos = cos * -1 ;
    }

    cos = cos + 0.000004 ;
    int s = cos * 100000 ;
    char ch ;

    for( i = 0 ; i < 6 ; i++ ) {
        ch = s % 10 + 48 ;
        s = s / 10 ;
        push( Ncos, ch ) ;
    }

    Ncos->integer = 1 ;
    Ncos->decimal = 5 ;

    //distroy( angle);
    //distroy( degree);
    return Ncos ;
}

/* Returns the tangent value for given angle
 *
Num *tangent( Num *N ){
    if( !N )
        return NULL ;
    return divide( sine(N), cosine(N), 5 );
}
*/
Num *tangent( Num *N ) {
    if( !N )
        return NULL ;
    Num *Ntan, *angle, *degree ;
    Ntan = (Num*)malloc(sizeof(Num)) ;
    angle = (Num*)malloc(sizeof(Num)) ;
    degree = (Num*)malloc(sizeof(Num)) ;

    init( Ntan ) ;
    init( angle ) ;
    init( degree ) ;

    int i = 0 , j = 8 ;
    append( angle, '3' ) ;
    append( angle, '6' ) ;
    append( angle, '0' ) ;

    degree = modula( N, angle );
    append( angle, '.' ) ;
    for( i = 0 ; i < 5 ; i++ )
        append( angle, '0' ) ;

    adjust_Number( angle, degree ) ;

    float n, tan, power = 100 ;
    node *p = degree->head ;
    while( p && j ) {
        n = n + ( p->data - 48 )* power ;
        p = p->next ;
        power = power / 10 ;
        j-- ;
    }

    if( degree->sign == 1 )
        n = n * -1 ;

    tan = sine_angle( n ) / cosine_angle(n) ;
    //printf("\nJayesh : %f %f", sine_angle(n), cosine_angle(n));

    if( tan < 0 ) {
        Ntan->sign = 1 ;
        tan = tan * -1 ;
    }

    tan = tan + 0.000004 ;
    int s = tan * 100000 ;
    char ch ;

    for( i = 0 ; s > 0.000001  ; i++ ) {
        ch = s % 10 + 48 ;
        s = s / 10 ;
        push( Ntan, ch ) ;
    }

    Ntan->integer = 1 ;
    Ntan->decimal = 5 ;

    return Ntan ;
}

/* Returns 1 if 1st Number is Larger
 * Otherwise returns 0
 */
Num *larger( Num *N1, Num *N2 ){// >
    if( !N1 || !N2 )
        return NULL ;

    Num *Nlar = (Num*)malloc(sizeof(Num)) ;
    init( Nlar ) ;

    if( zero_Number(N1) == 0 ){
        if( zero_Number(N2) == 0 || N2->sign == 0 )
            append( Nlar, '0');
        else
            append( Nlar, '1');
    }
    else if( N1->sign == 0 ){
        if( N2->sign == 1)
            append( Nlar, '1');
        else{
            if( compare( N1, N2) == 1 )
                append( Nlar, '1');
            else
                append( Nlar, '0');
        }
    }
    else{
        if( zero_Number( N2) == 0 || N2->sign == 0 )
            append( Nlar, '0');
        else{
            if( compare( N1, N2) == 0 )
                append( Nlar, '1');
            else
                append( Nlar, '0');
        }
    }
    return Nlar ;
}

/* Returns 1 if 1st Number is Smaller
 * Otherwise returns 0
 */
Num *smaller( Num *N1, Num *N2 ){
    return larger( N2, N1);
}

/* Returns 1 if both Number are equal
 * Otherwise returns 0
 */
Num *equal_Num( Num *N1, Num *N2 ){// <
    if( !N1 || !N2 )
        return NULL ;

    Num *Neq = (Num*)malloc(sizeof(Num)) ;
    init( Neq) ;
    if( compare( N1, N2 ) == -1 && N1->sign == N2->sign )
        append( Neq, '1' );
    else
        append( Neq, '0');
    return Neq ;
}

Num *not_equal( Num *N1, Num *N2 ){
    if( !N1 || !N2 )
        return NULL ;

    Num *Neq = (Num*)malloc(sizeof(Num)) ;
    init( Neq) ;
    if( compare( N1, N2 ) == -1 && N1->sign == N2->sign )
        append( Neq, '0' );
    else
        append( Neq, '1');
    return Neq ;
}

Num *greater_than_equal( Num *N1, Num *N2 ){
    Num *P1 = (Num*)malloc(sizeof( P1 )) ;
    Num *P2 = (Num*)malloc(sizeof( P2 )) ;
    P1 = larger( N1, N2 );
    P2 = equal_Num( N1, N2 );
    
    if( P1->head )
        if( P1->head->data == '1' )
            return P1 ;
    if( P2->head )
        if( P2->head->data == '1' )
            return P2 ;
    init( P1 );
    append( P1, '0');
    return P1 ;
}

Num *smaller_than_equal( Num *N1, Num *N2 ){
    Num *P1 = (Num*)malloc(sizeof( P1 )) ;
    Num *P2 = (Num*)malloc(sizeof( P2 )) ;
    P1 = smaller( N1, N2 );
    P2 = equal_Num( N1, N2 );

    if( P1->head )
        if( P1->head->data == '1' )
            return P1 ;
    if( P2->head )
        if( P2->head->data == '1' )
            return P2 ;
    init( P1 );
    append( P1, '0');
    return P1 ;
}

/* Returns 1 if both Number are equal
 * Otherwise returns 0
 */
Num *and_Num( Num *N1, Num *N2 ){
    Num *Nand = (Num*)malloc(sizeof(Num)) ;
    init( Nand) ;

    if( !N1 || !N2 )
        return Nand ;
    if( zero_Number(N1) == 0 || zero_Number(N2) == 0 )
        append( Nand, '0');
    else
        copy_Number( N2, Nand);
    return Nand ;
}

/* Returns 1 if both Number are equal
 * Otherwise returns 0
 */
Num *or_Num( Num *N1, Num *N2 ){// <
    Num *Nor = (Num*)malloc(sizeof(Num)) ;
    init( Nor) ;

    if( !N1 ){
        if( !N2 )
            append( Nor, '0');
        else if( zero_Number( N2 ) == 0 )
            append( Nor, '0');
        else
            copy_Number( N2, Nor);
    }
    else if( zero_Number(N1) == 0 ){
        if( !N2 )
            append( Nor, '0');
        else if( zero_Number( N2 ) == 0 )
            append( Nor, '0');
        else
            copy_Number( N2, Nor);
    }
    else
        copy_Number( N1, Nor);
    return Nor ;
}

Num *left_shift( Num *N1, Num *N2 ){
    if( !N1 || !N2 )
        return NULL ;

    delete_zeros_of_both_sides( N2 );
    if( N2->sign == 1 ) {
        printf("2nd Number should be positive..!!\n");
        init( N1 );
        return N1 ;
    }

    if( N2->decimal > 0 ) {
        printf("2nd Number should be integer..!!\n");
        while( N2->decimal )
            del( N2 );
    }

    Num one, two ;
    init( &one );
    init( &two );
    append( &one, '1');
    append( &two, '2');

    while( zero_Number( N2 ) != 0 ) {
        N1 = multipy( N1, &two );
        N2 = sub( N2, &one ) ;
    }

    return N1 ;
}

Num *right_shift( Num *N1, Num *N2 ){
    if( !N1 || !N2 )
        return NULL ;

    delete_zeros_of_both_sides( N2 );
    if( N2->sign == 1 ) {
        printf("2nd Number should be positive..!!\n");
        init( N1 );
        return N1 ;
    }

    if( N2->decimal > 0 ) {
        printf("2nd Number should be integer..!!\n");
        while( N2->decimal )
            del( N2 );
    }

    Num one, two ;
    init( &one );
    init( &two );
    append( &one, '1');
    append( &two, '2');

    while( zero_Number( N2 ) != 0 ) {
        N1 = divide( N1, &two, 0 );
        N2 = sub( N2, &one ) ;
    }

    return N1 ;
}