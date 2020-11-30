#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<time.h>
#include "Help.h"

char *removespace( char *str ) {
    int i = 0, j = 0 ;
    while( str[i] ) {
        if( str[i] != ' ' )
            str[j++] = str[i] ;
        i++ ;
    }
    str[j] = '\0' ;
    return str ;
}

void help() {
    printf("Use following Keys/ Options for more details : \n");
    printf("  scale               : use to set the scale of BC \n");
    printf("  rscale              : use to set the scale of sqroot of BC \n");
    printf("  -u  --use           : print user guideline \n");
    printf("  -p  --prec          : print precedence of operator in BC \n");
    printf("  -i  --info          : print information about project \n");
    printf("  -v  --version       : print version information \n");
    printf("  -s  --history       : print recent solutions history \n");
    printf("  -a  --allhistory    : print all solutions history \n");
    printf("  -d  --delhistory    : deletes recent history \n");
    printf("  -q  quit            : quit/ terminate the program \n\n");
    return ;
}

void use() {
    printf("* This is the Command line Arbitrary precision calculator *");
    printf("Following Basic Mathematical Operations can be performed on BC\n");
    printf(" 1. Addition            : + \n");
    printf(" 2. Subtraction         : - \n");
    printf(" 3. Multiplication      : * \n");
    printf(" 4. Division            : / \n");
    printf(" 5. Modula              :   \n");
    printf(" 6. Power               : ^ \n");
    printf(" 7. Factorial           : ! \n");
    printf(" 8. Square              : sq \n");
    printf(" 8. Cube                : cb \n");
    printf(" 8. Sqroot              : sqrt \n");
    printf(" 8. Length              : len \n");
    printf(" 9. Compare             : < > >= <= == != \n\n");
    printf("10. Logical Operators   : && ||\n");
    printf("Some Trigonometric functions also can be performed on BC\n");
    printf(" 1. Sine                : s \n");
    printf(" 2. Cosine              : c \n");
    printf(" 3. Tangent             : t \n\n");
    return ;
}

void preced() {
    printf("Precedence in BC :\n");
    printf(" * Parenthesis () has highest precedence \n");
    printf(" * Precedence order is as following \n");
    printf("    # Trigonometric Functions \n");
    printf("    # Factorial \n");
    printf("    # Square, Cube, Square root \n");
    printf("    # Length \n");
    printf("    # Unary operators has same precendence \n");
    printf("    # Compare \n");
    printf("    # AND OR \n");
    printf("    # Power \n");
    printf("    # Modula \n");
    printf("    # Multiplication Division has same precedence \n");
    printf("    # Addition Subtraction has same precedence \n\n");
    return ;
}

void info() {
    printf("DSA Project\n");
    printf("* Project Name :* Bash Calculator \n");
    printf("BC is Implemented using C Programming Language \n");
    printf("Name : Jayesh Bapu Gadilohar\n");
    printf("MIS : 111910048 \nDiv : 2 \n");
    printf("Batch : S4\n");
    printf("Mail id : gadiloharjb19.comp@coep.ac.in\n\n");
    return ;
}

void version() {
    printf("DSA Project\n");
    printf("Jayesh Gadilohar : 111910048\n");
    printf("BC 1.07.1\n");
    printf("Version 14.05.01\n\n");
    return ;
}

int quit( char *str ) {
    char ch[1000], us[5] = "quit", ja[3] = "-q";
    strcpy( ch, str );
    removespace( ch );
    if( strcmp( us, ch ) == 0 || strcmp( ja, ch) == 0 ){
        printf("Quiting..!!\n");
        return 1 ;
    }
    else
        return 0 ;
}

int scale( char *str ) {
    char ch[2048] ;
    strcpy( ch, str );
    removespace( ch );

    if( ch[0] != 's' || ch[1] != 'c' || ch[2] != 'a' || ch[3] != 'l' || ch[4] != 'e' || ch[5] != '=' )
        return INT_MIN ;
    int i = 6 ;
    char no[6] ;
    while( ch[i] ) {
        if( ch[i] >= '0' && ch[i] <= '9' ) {
            no[i-6] = ch[i] ;
            i++ ;
        }
        else
            return INT_MIN ;
    }
    return atoi( no ) ;
}

int rscale( char *str ) {
    char ch[1024] ;
    strcpy( ch, str );
    removespace( ch );

    if( ch[0] != 'r' || ch[1] != 's' || ch[2] != 'c' || ch[3] != 'a' || ch[4] != 'l' || ch[5] != 'e' || ch[6] != '=' )
        return INT_MIN ;
    int i = 7 ;
    char no[6] ;
    while( ch[i] ) {
        if( ch[i] >= '0' && ch[i] <= '9' ) {
            no[i-7] = ch[i] ;
            i++ ;
        }
        else
            return INT_MIN ;
    }
    return atoi( no ) ;
}

void all_history(){
    char c ;
    FILE *fp ;
    fp = fopen("All_History.txt", "r");
    if( !fp ){
        printf("File does not exist \n");
        printf("\n");
        return ;
    }
    else{
        int i = 0 ;
        while( c != EOF ){
            c = getc(fp);
            putchar(c);
            i++ ;
        }
        if( i < 6 )
            printf("No History Saved..!!\n");
        fclose( fp );
        printf("\n");
        return ;
    }
}

void history(){
    char c ;
    FILE *fp ;
    fp = fopen("History.txt", "r");
    if( !fp ){
        printf("File does not exist \n");
        printf("\n");
        return ;
    }
    else{
        int i = 0 ;
        while( c != EOF ){
            c = getc(fp);
            putchar(c);
            i++ ;
        }
        if( i < 6 )
            printf("No History Saved..!!\n");
        fclose( fp );
        printf("\n");
        return ;
    }
}

void del_history(){
    time_t t ;
	time( &t);

    FILE *fp ;
    fp = fopen("All_History.txt", "w");
    if( !fp ){
        printf("File does not exist \n");
        printf("\n");
        return ;
    }
    fprintf( fp, "%s", ctime( &t));
    fclose(fp);
    printf("Successfully History Deleted..!!\n\n");
    return ;
}
