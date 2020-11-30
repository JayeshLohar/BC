#define OPERAND	100
#define OPERATOR 200
#define END	300
#define ERR 400

typedef struct token{
	int type;
	Num *num;
	char op;
}token;

token gettoken(char *expr, int *reset);
int precedance(char o);
Num *infix(char *exp, int scale, int rscale );
void modify(char *line);
int readline(char *line, int len);

/*char *removespace( char *str );
int help( char *str );
int info( char *str );
int scale( char *str );*/
int check_string( char *str );
int readLine(char **ptr);
