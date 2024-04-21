/** 
 * postfix.c
 *
 * School of Computer Science, 
 * Chungbuk National University.
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10       // stack size 
#define MAX_EXPRESSION_SIZE 20  // expression size

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  				/* ( 왼쪽 괄호 */
	rparen = 9,  				/* ) 오른쪽 괄호*/
	times = 7,   				/* * 곱셈 */
	divide = 6,  				/* / 나눗셈 */
	plus = 5,    				/* + 덧셈 */
	minus = 4,   				/* - 뺄셈 */
	operand = 1 				/* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];		/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];			/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  				/* postfixStack용 top */
int evalStackTop = -1;	   				/* evalStack용 top */

int evalResult = 0;	   					/* 계산 결과를 저장 */

void postfixpush(char x);  
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
	printf("----------[정새연] [2023041074]----------\n");

	char command; 								// 사용자가 입력한 메뉴 

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) { 						// command에 따라 case 실행
		case 'i': case 'I':   					// i or I 입력 시
			getInfix();							// getInfix() 함수 호출
			break;
		case 'p': case 'P':  					// p or P 입력 시
			toPostfix();   						// toPostfix() 함수 호출
			break;
		case 'e': case 'E': 					// e or E 입력 시
			evaluation(); 						// evaluation() 함수 호출
			break;
		case 'd': case 'D': 					// d or D 입력 시
			debug(); 							// debug() 함수 호출
			break;
		case 'r': case 'R': 					// r or R 입력 시
			reset(); 							// reset() 함수 호출
			break;	
		case 'q': case 'Q': 					// q or Q 입력 시
			break; 								// 종료
		default: 								// 그 외의 경우
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); 	// command가 q or Q가 아닐 때까지 반복

	return 1;
}

void postfixPush(char x)  						// 스택에 push하는 함수
{
    postfixStack[++postfixStackTop] = x; 		// postfixStackTop을 1 증가시키고 x를 대입
}

char postfixPop() 								// 스택에서 pop하는 함수
{
    char x;
    if(postfixStackTop == -1) 					// postfixStackTop이 -1일 때(공백스택일 때)
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--]; 	// x에 postfixStack[postfixStackTop] 대입 후 postfixStackTop을 1 감소
    }
    return x;
}

void evalPush(int x) 							// evalStack push하는 함수
{
    evalStack[++evalStackTop] = x; 				// evalStackTop을 1 증가시키고 x를 대입
}

int evalPop() 									// evalStack pop하는 함수
{
    if(evalStackTop == -1) 						// evalStackTop이 -1일 때(공백스택일 때)
        return -1;
    else
        return evalStack[evalStackTop--]; 		// evalStackTop을 1 감소시킨 후 evalStack[evalStackTop] 반환
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix() 								// infix expression(증위 표현법)을 입력받는 함수
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp); 
}

precedence getToken(char symbol) 				// 우선순위 반환 함수
{
	switch(symbol) {
	case '(' : return lparen;  					// symbol이 '('일 때 lparen 반환
	case ')' : return rparen; 					// symbol이 ')'일 때 rparen 반환
	case '+' : return plus; 					// symbol이 '+'일 때 plus 반환
	case '-' : return minus; 					// symbol이 '-'일 때 minus 반환
	case '/' : return divide;					// symbol이 '/'일 때 divide 반환
	case '*' : return times;					// symbol이 '*'일 때 times 반환
	default : return operand;					// 그 외의 경우 operand 반환
	}
}

precedence getPriority(char x) 					// 우선순위 반환 함수
{
	return getToken(x); 						// getToken(x) 반환
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c) 							// 문자하나를 전달받아, postfixExp에 추가하는 함수
{
	if (postfixExp == '\0') 					// postfixExp가 공백일 때
		strncpy(postfixExp, c, 1); 				// c를 postfixExp에 문자열 1개 복사
	else
		strncat(postfixExp, c, 1); 				// c를 postfixExp에 문자열 1개 이어붙임
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix() 								// infixExp를 postfix로 변경하는 함수
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp; 						// infixExp를 가리키는 포인터
	char x; 									/* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while (*exp != '\0')
	{
		x = getToken(*exp); 										// x에 문자 하나 임시로 저장
		if (x == operand) 											// opperand 이면 charCat
		{
			charCat(exp); 											// exp의 한글자를 postfixExp에 추가
			exp++; 													// exp가 다음 글자를 가리키게함
		}
		else if ( x== lparen) 										// 왼쪽 괄호이면 push
			postfixPush(*exp++);
		else if (x == rparen) 										// 오른쪽 괄호이면 왼쪽 괄호가 나올 때 까지 토큰들을 pop
		{
			while (getToken(postfixStack[postfixStackTop]) != lparen)
			{
				charCat(&postfixStack[postfixStackTop]); 			// postfixStack[postfixStackTop] 를 postfixExp에 추가하고 Pop
				postfixPop();
			}
			postfixPop(); 											// 왼쪽 괄호 버림
			exp++; 													// exp가 다음 글자를 가리키게함
		}
		else
		{
			while (getPriority(postfixStack[postfixStackTop]) >= x) // 스택 top의 우선순위 >=  x인동안은 exp의 한 글자를 postfixExp에 추가하고 pop
			{
				charCat(&postfixStack[postfixStackTop]); 
				postfixPop();
			}
			postfixPush(*exp++);			 						// x의 우선순위가 더 크거나 위의 반복문이 끝나면 push
		}
	}
	charCat(&postfixStack[postfixStackTop]);
	while ( postfixPop() != '\0') 									// postfixstack에 남아있는 원소들 postfixExp에 넣음
	{
		charCat(&postfixStack[postfixStackTop]);  					// postfixStack[postfixStackTop]을 postfixExp에 추가하고 pop
		
	}

}

void debug() 														// 디버깅 함수
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++) 						// postfixStack 출력
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset() 														// 초기화 함수
{
	infixExp[0] = '\0'; 											// infixExp[0]을 NULL로 초기화
	postfixExp[0] = '\0'; 											// postfixExp[0]을 NULL로 초기화

	for(int i = 0; i < MAX_STACK_SIZE; i++) 						// postfixStack을 NULL로 초기화
		postfixStack[i] = '\0'; 
    
	postfixStackTop = -1; 											// postfixStackTop을 -1로 초기화(초기값)
	evalStackTop = -1; 												// evalStackTop을 -1로 초기화(초기값)
	evalResult = 0; 												// evalResult를 0으로 초기화{초기값}
}

void evaluation() 													// 계산 함수
{
	char* ptr = postfixExp; 										// ptr은 postfixExp를 가리킴
	char x; 														// x 선언 - 한글자를 저장하는 변수
	int op1, op2;													// op1, op2 선언 - 피연산자 두개를 저장하는 변수
	while (*ptr != '\0')
	{
		x = getToken(*ptr); 										// x에 한글자 저장 
		if (x == operand)
		{
			evalPush(*ptr-'0'); 									// ptr이 가리키는 값 즉 postfixExp의 해당 글자를 숫자로 변환한 후 push
		}
		else // 피연산자가 아닌 연산자라면 op1, op2에 피연산자 두개를 pop으로 저장하고 연산
		{
			op2 = evalPop();
			op1 = evalPop();
			switch (x)
			{
			case plus: evalPush(op1 + op2); 						// plus의 경우 plus한 값을 push
				break;
			case minus: evalPush(op1 - op2); 						// plus의 경우 minus한 값을 push
				break;
			case times: evalPush(op1 * op2); 						// times의 경우 times한 값을 push
				break;
			case divide: evalPush(op1 / op2); 						// divide의 경우 divide한 값을 push
				break;
			}
		}
		ptr++; 														// 연산이 끝난 후 ptr은 postfixExp의 한칸 뒤 저장
	}
	evalResult = evalPop(); 										// 결과 저장
}

