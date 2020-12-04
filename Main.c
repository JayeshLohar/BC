#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>
#include<ctype.h>
#include<string.h>
#include<time.h>
#include "List.h"
#include "Operations.h"
#include "Infix.h"
#include "Help.h"

int main( int argc, char **argv ) {
    if (argc == 2) {
		if ( !strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
            		help() ;
			return 0;
		}
	    	if ( !strcmp(argv[1], "--use") || !strcmp(argv[1], "-u")) {
			use() ;
			return 0;
		}
		if ( !strcmp(argv[1], "--version") || !strcmp(argv[1], "-v")) {
			version() ;
			return 0;
		}
		if ( !strcmp(argv[1], "--prec") || !strcmp(argv[1], "-p")) {
			preced() ;
			return 0;
		}
		if ( !strcmp(argv[1], "--info") || !strcmp(argv[1], "-i")) {
			info() ;
			return 0;
		}
		if ( !strcmp(argv[1], "--history") || !strcmp(argv[1], "-s")) {
			history() ;
			return 0;
		}
		if ( !strcmp(argv[1], "--allhistory") || !strcmp(argv[1], "-a")) {
			all_history() ;
			return 0;
		}
		if ( !strcmp(argv[1], "--delhistory") || !strcmp(argv[1], "-d")) {
			del_history() ;
			return 0;
		}
		else {
			printf("Invalid Option. Try : ./jay --help\n");
			return 0;
		}
	}

	printf("\nBC 1.07.1\n");
	printf("Copyright 2020. Free Software Foundation, Inc.\n");
	printf("This is free software with ABSOLUTELY NO WARRANTY.\n");
	printf("Jayesh Gadilohar : 111910048\n\n");
	Num *r;
	char *line , expr[1024] ;
	time_t t ;
	time( &t);

	FILE *fhall, *frec ;
	fhall = fopen("All_History.txt", "a");
	if( !fhall ) {
		printf("File does not exist...!!");
		return 0 ;
	}

	fprintf( fhall, "%s", ctime( &t));
	int sc = 0, rsc = 10 , sflag, rflag, qflag, check ;
	int jay = 0 ;

	while( readLine( &line ) ) {
        strcpy( expr, line);
        sflag = rflag = qflag = 0 ;

        qflag = quit( line );

        if( qflag == 1 ){
            fprintf(fhall,"\n");
            break ;
        }
        else if( scale( line ) != INT_MIN ){
            sc = scale( line );
            sflag = 1 ;
        }
        else if( rscale( line ) != INT_MIN ){
            rsc = rscale( line );
            rflag = 1 ;
        }

        if( sflag == 1 || rflag == 1 || qflag == 1 )
            fprintf(fhall, "\n%s\n", line);

        if( sflag == 0 && rflag == 0 && qflag == 0 ) {
            check = check_string( line ) ;
            jay++ ;
            if( jay == 1 ){
                frec = fopen("History.txt", "w");
                if( !frec ) {
                    printf("File does not exist...!!");
                    return 0 ;
                }
                fprintf( frec, "%s", ctime( &t));
            }
            else{
                frec = fopen("History.txt", "a");
                if( !frec ) {
                    printf("File does not exist...!!");
                    return 0 ;
                }
            }

            fprintf( fhall, "\nEq  : %s\n", expr);
            fprintf( frec, "\nEq  : %s\n", expr);
            if( check == 1 ) {
                r = infix(line, sc, rsc );
                if(r != NULL){
                    traverse(r);
                    save( fhall, r);
                    save( frec, r);
                    fclose(frec);
                }
                else{
                    printf("Error in Expression..!!\n");
                    fprintf(fhall, "Error in Expression..!!\n");
                    fprintf(frec, "Error in Expression..!!\n");
                    fclose(frec);
                }
            }
            else if( check == 2 ){
                printf("Number of opening and closing bracket are not same..!! \n");
                fprintf(fhall, "Number of opening and closing bracket are not same..!! \n");
                fprintf(frec, "Number of opening and closing bracket are not same..!! \n");
                fclose(frec);
            }
            else if( check == 3 ){
                printf("Improper use of brackets..!! \n");
                fprintf(fhall, "Improper use of brackets..!! \n");
                fprintf(frec, "Improper use of brackets..!! \n");
                fclose(frec);
            }
            else if( check == 4 ){
                printf("Improper use of Operator..!! \n");
                fprintf(fhall, "Improper use of Operator..!! \n");
                fprintf(frec, "Improper use of Operator..!! \n");
                fclose(frec);
            }
            else if( check == 5 ){
                printf("Improper use of Decimal Point..!! \n");
                fprintf(fhall,"Improper use of Decimal Point..!! \n");
                fprintf(frec,"Improper use of Decimal Point..!! \n");
                fclose(frec);
            }
            else if( check == 6 ){
                printf("Improper use of Number..!! \n");
                fprintf(fhall,"Improper use of Number..!! \n");
                fprintf(frec,"Improper use of Number..!! \n");
                fclose(frec);
            }
            else if( check == 7 ){
                printf("wrong character use..!! \n");
                fprintf(fhall,"wrong character use..!! \n");
                fprintf(frec,"wrong character use..!! \n");
                fclose(frec);
            }
            else{
                printf("Error in Expression..!!\n");
                fprintf(fhall,"Error in Expression..!!\n");
                fprintf(frec,"Error in Expression..!!\n");
                fclose(frec);
            }
        }
	}
	fprintf(fhall,"\n");
	//fprintf(fhall,"Quiting..!!\n\n");
	fclose(fhall);
	return 0;
}
