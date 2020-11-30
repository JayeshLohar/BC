#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "Stack.h"
#include "Operations.h"
#include "Infix.h"
#include <limits.h>

enum state { NUMBER, OP, FINISH, ERROR, SPC };

token gettoken(char *expr, int *reset) {
	static int i = 0;
	Num *a = (Num *)malloc(sizeof(Num));
	init(a);
	char currchar;
	static enum state currstate = SPC;
	enum state nextstate;
	token t;
	if(*reset == 1) {
		currstate = SPC;
		*reset = 0;
		i = 0;
	}
	while(1) {
		currchar = expr[i];
		switch(currstate) {
			case NUMBER:
				switch(currchar) {
					case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9': case '.':
						nextstate = NUMBER;
						append(a, currchar);
						i++;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(': case ')': case 'r': case 'l': case '#': case 'p': case 'q': case 'n': case 'm':
					case 's': case 'c': case 't': case '^': case 'f': case '<': case '>': case '=': case '&': case '|': case 'g': case 'e': case '!':
 						nextstate = OP;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						i++;
						return t;
						break;
					case '\0':
						nextstate = FINISH;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						return t;
						break;
					case ' ':
						nextstate = SPC;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						i++;
						return t;
						break;

					default:
						nextstate = ERROR;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						return t;
						break;
				}
				break;

			case OP:
				switch(currchar) {
					case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9': case '.':
						t.type = OPERATOR;
						t.op = expr[i - 1];
						nextstate = NUMBER;
						currstate = nextstate;
						//i++;
						return t;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(': case ')': case 'r': case 'l': case '#': case 'p': case 'q': case 'n': case 'm':
					case 's': case 'c': case 't': case '^': case 'f': case '<': case '>': case '=': case '&': case '|': case 'g': case 'e': case '!':
						nextstate = OP;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						currstate = nextstate;
						i++;
						return t;
						break;
					case '\0':
						nextstate = FINISH;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						currstate = nextstate;
						return t;
						break;
					case ' ':
						nextstate = SPC;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						currstate = nextstate;
						i++;
						return t;
						break;
					default:
						nextstate = ERROR;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						currstate = nextstate;
						return t;
						break;
				}
				break;
			case FINISH:
				t.type = END;
				return t;
				break;
			case ERROR:
				t.type = ERR;
				return t;
				break;
			case SPC:
				switch(currchar) {
					case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9': case '.':
						append(a, currchar);
						nextstate = NUMBER;
						i++;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(': case ')': case 'r': case 'l': case '#': case 'p': case 'q': case 'n': case 'm':
					case 's': case 'c': case 't': case '^': case 'f': case '<': case '>': case '=': case '&': case '|': case 'g': case 'e': case '!':
						nextstate = OP;
						i++;
						break;
					case '\0':
						nextstate = FINISH;
						break;
					case ' ':
						nextstate = SPC;
						i++;
						break;
					default:
						nextstate = ERROR;
						break;
				}
				currstate = nextstate;
				break;
		}
	}
}

int precedance(char o) {
	switch(o) {
		case ')' :
			return 0;
			break;
		case '+' :
		case '-' :
			return 1;
			break;
		case '*' :
		case '/' :
			return 2;
			break;
		case '%' :
			return 3;
			break;
        case '^' :
            return 4;
            break;
        case '&' :
        case '|' :
            return 5;
            break;
        case '>' :
        case '<' :
        case '=' :
        case 'g' :
        case 'e' :
        case '!' :
		case 'n' :
		case 'm' :
            return 6;
            break;
        case 's':
        case 'c':
        case 't':
        case 'f':
        case 'p':
        case 'q':
        case 'r':
        case 'l':
			return 7;
			break;
        case '#' :
			return 8;
			break;
		case '(' :
			return 9;
			break;
		default :
			return 10;
			break;
	}
	return 10;
}

Num *infix(char *exp, int scale, int rsc ) {
	token t;
	char curr_op, prev_op, ch;
	int cnt_ob = 0, cnt_cb = 0, reset  = 1, curr, prev;
	Num *a, *b, *z;
	a = (Num *)malloc(sizeof(Num));
	b = (Num *)malloc(sizeof(Num));
	z = (Num *)malloc(sizeof(Num));
	init(a);
	init(b);
	init(z);
	n_stack ns;
	ch_stack cs;
	n_init(&ns);
	ch_init(&cs);
	prev = ERR;
	while(1) {
        int flag = 0;
		t = gettoken(exp, &reset);
		curr = t.type;
		if(curr == prev && prev == OPERAND) {
			return NULL;
		}

		if(t.type == OPERAND){
			n_push(&ns, t.num);
		}
		else if(t.type == OPERATOR){
			curr_op = t.op;
			if(curr_op == '(')
				cnt_ob++;
			if(curr_op == ')')
				cnt_cb++;
			if(cnt_cb > cnt_ob)
				return NULL;

			if( curr_op != '(' || curr_op!= ')' )
                flag++ ;
			if( !ch_isempty(&cs) ) {
				prev_op = ch_top(&cs);

				while(precedance(prev_op) >= precedance(curr_op)) {
					prev_op = ch_pop(&cs);
					if(!n_isempty(&ns)) {
						a = n_pop(&ns);
					}
					else{
						ch_push(&cs, prev_op);
						break;
					}

					if(!n_isempty(&ns)) {
						b = n_pop(&ns);
					}
					else {
                        if( prev_op == 's'){
                            z = sine(a) ;
                            n_push(&ns, z);
                            break ;
                        }
                        else if( prev_op == 'c'){
                            z = cosine(a) ;
                            n_push(&ns, z);
                            break ;
                        }
                        else if( prev_op == 't'){
                            z = tangent(a) ;
                            n_push(&ns, z);
                            break ;
                        }
                        else if( prev_op == 'f'){
                            z = fact(a) ;
                            n_push(&ns, z);
                            break ;
                        }
                        else if( prev_op == 'p'){
                            z = square(a) ;
                            n_push(&ns, z);
                            break ;
                        }
                        else if( prev_op == 'q'){
                            z = cube(a) ;
                            n_push(&ns, z);
                            break ;
                        }
                        else if( prev_op == 'r'){
                            z = square_root(a, rsc ) ;
                            n_push(&ns, z);
                            break ;
                        }
                        else if( prev_op == 'l'){
                            z = length(a) ;
                            n_push(&ns, z);
                            break ;
                        }
                        else if( prev_op == '-'){
                            a->sign = ( a->sign + 1)%2 ;
                            n_push(&ns, a);
                            break ;
                        }
                        else{
                            ch_push(&cs, prev_op);
                            n_push(&ns, a);
                            break;
                        }
					}
					if((prev_op == '/' || prev_op == '%') && a == 0) {
                        printf("Mathematical Error..!!\n");
                        return NULL;
                    }
					switch (prev_op) {
                        case 's' :
                            z = sine(a);
                            n_push(&ns, b);
                            n_push(&ns, z);
                            break ;
                        case 'c' :
                            z = cosine(a);
                            n_push(&ns, b);
                            n_push(&ns, z);
                            break ;
                        case 't' :
                            z = tangent(a);
                            n_push(&ns, b);
                            n_push(&ns, z);
                            break ;
                        case 'f' :
                            z = fact(a);
                            n_push(&ns, b);
                            n_push(&ns, z);
                            break ;
                        case 'p' :
                            z = square( a);
                            n_push(&ns, b);
                            n_push(&ns, z);
                            break ;
                        case 'q' :
                            z = cube( a);
                            n_push(&ns, b);
                            n_push(&ns, z);
                            break ;
                        case 'r' :
                            z = square_root(a, rsc);
                            n_push(&ns, b);
                            n_push(&ns, z);
                            break ;
                        case 'l' :
                            z = length(a);
                            n_push(&ns, b);
                            n_push(&ns, z);
                            break ;
                        case '#' :
                            a->sign = ( a->sign + 1 ) % 2 ;
                            n_push(&ns, b);
                            n_push(&ns, a);
                            break ;

						case '+' :
							z = add(a, b);
							n_push(&ns, z);
							break;
						case '-' :
							z = sub(b, a);
							n_push(&ns, z);
							break;
						case '*' :
							z = multipy(b, a);
							n_push(&ns, z);
							break;
						case '/' :
							z = divide(b, a, scale);
							n_push(&ns, z);
							break;
						case '%' :
							z = modula(b, a);
							n_push(&ns, z);
							break;
                        case '^' :
                            z = power(b, a);
                            n_push(&ns, z);
                            break;
                        case '>' :
                            z = larger(b, a);
                            n_push(&ns, z);
                            break;
                        case '<' :
                            z = smaller(b, a);
                            n_push(&ns, z);
                            break;
                        case '=' :
                            z = equal_Num(b, a);
                            n_push(&ns, z);
                            break;
                        case 'g' :
							z = greater_than_equal(b, a);
							n_push(&ns, z);
							break;
                        case 'e' :
							z = smaller_than_equal(b, a);
							n_push(&ns, z);
							break;
                        case '!' :
							z = not_equal(b, a);
							n_push(&ns, z);
							break;
                        case '&' :
                            z = and_Num(b, a);
                            n_push(&ns, z);
                            break;
                        case '|' :
                            z = or_Num(b, a);
                            n_push(&ns, z);
                            break;
						case 'n' :
                            z = left_shift(b, a);
                            n_push(&ns, z);
                            break;
						case 'm' :
                            z = right_shift(b, a);
                            n_push(&ns, z);
                            break;
						case '(' :
							ch_push(&cs, prev_op);
							n_push(&ns, b);
							n_push(&ns, a);
							break;
						default :
							return NULL;
					}
					if (prev_op == '(')
						break;
					if(!ch_isempty(&cs))
						prev_op = ch_top(&cs);
					else
						break;
				}
			}
			ch_push(&cs, t.op);
			if(curr_op == ')') {
				ch = ch_pop(&cs);
				cnt_cb--;
				ch = ch_pop(&cs);
				cnt_ob--;
			}
		}
		else if(t.type == END) {
			if(cnt_ob == cnt_cb) {
				while(!ch_isempty(&cs)) {
					if(!n_isempty(&ns)) {
						a = n_pop(&ns);
					}
					else {
						printf("Less Operands..!!\n");
                        return NULL;
					}
					ch = ch_pop(&cs);
					if( ch == 's' && n_isempty(&ns) && ch_isempty(&cs) ) {
                        z = sine(a) ;
                        n_push(&ns, z);
                        break ;
					}
					else if( ch == 'c' && n_isempty(&ns) && ch_isempty(&cs) ) {
                        z = cosine(a) ;
                        n_push(&ns, z);
                        break ;
					}
					else if( ch == 't' && n_isempty(&ns) && ch_isempty(&cs) ) {
                        z = tangent(a) ;
                        n_push(&ns, z);
                        break ;
					}
					else if( ch == 'f' && n_isempty(&ns) && ch_isempty(&cs) ) {
                        z = fact(a) ;
                        n_push(&ns, z);
                        break ;
					}
					else if( ch == 'p' && n_isempty(&ns) && ch_isempty(&cs) ) {
                        z = square(a) ;
                        n_push(&ns, z);
                        break ;
					}
					else if( ch == 'q' && n_isempty(&ns) && ch_isempty(&cs) ) {
                        z = cube( a) ;
                        n_push(&ns, z);
                        break ;
					}
					else if( ch == 'r' && n_isempty(&ns) && ch_isempty(&cs) ) {
                        z = square_root(a, rsc) ;
                        n_push(&ns, z);
                        break ;
					}
					else if( ch == 'l' && n_isempty(&ns) && ch_isempty(&cs) ) {
                        z = length(a) ;
                        n_push(&ns, z);
                        break ;
					}
					else if( ch == '-' && n_isempty(&ns) && ch_isempty(&cs) ) {
                        a->sign = ( a->sign + 1 )% 2;
                        n_push(&ns, a);
                        break ;
					}

					if(!n_isempty(&ns)) {
						b = n_pop(&ns);
					}
					else {
						printf("Less Operands..!!\n");
                        return NULL;
					}
					if((ch == '/' || ch == '%') && a == 0) {
						printf("Mathematical Error..!!\n");
						return NULL;
					}
					switch(ch) {
					    case 's' :
                            z = sine(a);
                            n_push(&ns, b);
                            n_push(&ns, z);
                            break ;
                        case 'c' :
                            z = cosine(a);
                            n_push(&ns, b);
                            n_push(&ns, z);
                            break ;
                        case 't' :
                            z = tangent(a);
                            n_push(&ns, b);
                            n_push(&ns, z);
                            break ;
                        case 'f' :
                            z = fact(a);
                            n_push(&ns, b);
                            n_push(&ns, z);
                            break ;
                        case 'p' :
                            z = square( a);
                            n_push(&ns, b);
                            n_push(&ns, z);
                            break ;
                        case 'q' :
                            z = cube( a);
                            n_push(&ns, b);
                            n_push(&ns, z);
                            break ;
                        case 'r' :
                            z = square_root(a, rsc);
                            n_push(&ns, b);
                            n_push(&ns, z);
                            break ;
                        case 'l' :
                            z = length(a);
                            n_push(&ns, b);
                            n_push(&ns, z);
                            break ;
                        case '#' :
                            a->sign = ( a->sign +1 )%2;
                            n_push(&ns, b);
                            n_push(&ns, a);
                            break ;
						case '+' :
							z = add(b, a);
							n_push(&ns, z);
							break;
						case '-' :
							z = sub(b, a);
							n_push(&ns, z);
							break;
						case '*' :
							z = multipy(b, a);
							n_push(&ns, z);
							break;
						case '/' :
							z = divide(b, a, scale);
							n_push(&ns, z);
							break;
						case '%' :
							z = modula(b, a);
							n_push(&ns, z);
							break;
                        case '^' :
							z = power(b, a);
							n_push(&ns, z);
							break;
                        case '<' :
							z = smaller(b, a);
							n_push(&ns, z);
							break;
                        case '>' :
							z = larger(b, a);
							n_push(&ns, z);
							break;
                        case '=' :
							z = equal_Num(b, a);
							n_push(&ns, z);
							break;
                        case 'g' :
							z = greater_than_equal(b, a);
							n_push(&ns, z);
							break;
                        case 'e' :
							z = smaller_than_equal(b, a);
							n_push(&ns, z);
							break;
                        case '!' :
							z = not_equal(b, a);
							n_push(&ns, z);
							break;
                        case '&' :
							z = and_Num(b, a);
							n_push(&ns, z);
							break;
                        case '|' :
							z = or_Num(b, a);
							n_push(&ns, z);
							break;
						case 'n' :
							z = left_shift(b, a);
							n_push(&ns, z);
							break;
						case 'm' :
							z = right_shift(b, a);
							n_push(&ns, z);
							break;
						default :
							return NULL;
					}
				}
			}
			else {
				printf("Error in Expression..!!\n");
				return NULL;
			}
			if(!n_isempty(&ns)){
				z = n_pop(&ns);
				if(n_isempty(&ns))
					return z;
				else {
                    printf("Less Operators..!!\n");
                    return NULL;
                }
			}
			else {
				printf("Less Operands..!!\n");
				return NULL;
			}
		}
		else if(t.type == ERR) {
			return NULL;
		}
		prev = curr;
	}
}

/* Checks the expression
 * Expression is Correct 1
 * Number of opening and closing bracket are not same 2
 * Improper use of brackets 3
 * Improper use of Operator 4
 * Improper use of Decimal Point 5
 * Improper use of Number 6
 * wrong character use 7
 */
int check_string( char *str ) {
    int i = 0 , num = 0, pt = 0, flag = 0, oper = 0 ;
    int op_b = 0 , cl_b = 0 ;
    int op[10], cl[10] ;

    while( str[i] ){
        if(( str[i] >= '0' && str[i] <= '9' ) || str[i] == '.' ){
            oper = 0 ;
            if( num == 0 || flag == 1 ){
                flag = 1 ;
                num++ ;
                if( str[i] == '.' )
                    pt++ ;
                if( pt > 1 )
                    return 5 ;
            }
            else
                return 6 ;
        }
        else if( str[i]=='+' || str[i]=='-' || str[i]=='*' || str[i]=='/' || str[i]=='%' || str[i] =='^' || str[i]=='>' || str[i] =='<' || str[i] =='=' || str[i] =='!' || str[i]=='&' || str[i]=='|'){
            num = 0 ;
            pt = 0 ;
            oper++ ;
            if( str[i] == '=' ){
                i++ ;
                if( str[i] == '=' )
                    str[i] = ' ';
                else
                    return 4 ;
            }
            if( str[i] == '!' ){
                i++ ;
                if( str[i] == '=' )
                    str[i] = ' ';
                else
                    return 4 ;
            }
            if( str[i] == '&' ) {
                i++ ;
                if( str[i]=='&' )
                    str[i] = ' ';
                else
                    return 4 ;
            }
            if( str[i] == '|' ) {
                i++ ;
                if( str[i]=='|' )
                    str[i] = ' ';
                else
                    return 4 ;
            }
            if( str[i] == '>' ) {
                if( str[i+1] == '=' ){
                    str[i+1] = ' ';
                    str[i] = 'g' ;
                }
            }
			if( str[i] == '>' ) {
                if( str[i+1] == '>' ){
                    str[i+1] = ' ';
                    str[i] = 'm' ;
                }
            }
            if( str[i] == '<' ) {
                if( str[i+1] == '=' ){
                    str[i+1] = ' ';
                    str[i] = 'e' ;
                }
            }
			if( str[i] == '<' ) {
                if( str[i+1] == '<' ){
                    str[i+1] = ' ';
                    str[i] = 'n' ;
                }
            }

            if( oper == 2 && str[i] == '-' ){
                oper-- ;
                str[i] = '#';
            }
            if( oper > 1 )
                return 4 ;
        }
        else if( str[i] == '(' ){
            flag = 0 ;
            pt = 0 ;
            op[op_b] = i ;
            op_b++ ;
        }
        else if( str[i] == ')' ){
            flag = 0 ;
            pt = 0 ;
            cl[cl_b] = i ;
            cl_b++ ;
        }
        else if( str[i]=='s' || str[i]=='c' || str[i]=='t' || str[i]=='f' || str[i]=='s' || str[i]=='l' || str[i]==' ' || str[i]=='\t' ){
            if( str[i] == 's' && str[i+1] == 'q' && str[i+2] == 'r' && str[i+3] == 't' ){
                str[i] = 'r' ;
                str[i+1] = str[i+2] = str[i+3] = ' ' ;
            }
            if( str[i] == 's' && str[i+1] == 'q' ){
                str[i] = 'p' ;
                str[i+1] = ' ' ;
            }
            if( str[i] == 'c' && str[i+1] == 'b' ){
                str[i] = 'q' ;
                str[i+1] = ' ' ;
            }
            if( str[i] == 'f' && str[i+1] == 'a' && str[i+2] == 'c' && str[i+3] == 't' ){
                str[i] = 'f' ;
                str[i+1] = str[i+2] = str[i+3] = ' ' ;
            }
            if( str[i] == 'l' && str[i+1] == 'e' && str[i+2] == 'n' ){
                str[i] = 'l' ;
                str[i+1] = str[i+2] = ' ' ;
            }
            flag = 0 ;
            pt = 0 ;
        }
        else{
            return 7 ;
        }
        i++ ;
    }
    if( op_b == cl_b ){
        for( i = 0 ; i < op_b ; i++ ){
            if( op[i] > cl[i] )
                return 3 ;
        }
    }
    else
        return 2 ;
    return 1 ;
}

int readLine(char **ptr) {
	int i;
	*ptr = (char*) malloc(sizeof(char));
	if (*ptr == NULL)
		return INT_MIN;

	for (i = 0; ((*ptr)[i] = getchar()) != '\n'; i++)
		(*ptr) = (char*) realloc(*ptr, (i + 2) * sizeof(char));

	(*ptr)[i] = '\0';
	return i;
}
