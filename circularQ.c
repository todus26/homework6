/**
 * circularQ.c
 *
 * School of Computer Science, 
 * Chungbuk National University.
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4             //MAX_QUEUE_SIZE를 4로 정의

typedef char element;                //element를 char로 정의
typedef struct {
	element queue[MAX_QUEUE_SIZE];   // element를 queue[MAX_QUEUE_SIZE](원소를 저장할 배열)로 선언
	int front, rear;                 // front : 자료 삭제 위치, rear : 자료 삽입 위치
}QueueType;                          // 구조체 호출 시 이름


QueueType *createQueue();                      	// Queue 생성 함수 - 반환 값은 QueueType로 
int freeQueue(QueueType *cQ);                  	// Queue 해제 함수 - 할당된 메모리 해제
int isEmpty(QueueType *cQ);                     // Queue가 비어있는지 확인하는 함수
int isFull(QueueType *cQ); 					   	// Queue가 꽉 차 있는지 확인하는 함수
void enQueue(QueueType *cQ, element item); 		// Queue에 데이터를 입력받는 함수
void deQueue(QueueType *cQ, element* item); 	// Queue에 저장된 데이터를 삭제하는 함수
void printQ(QueueType *cQ); 					// Queue에 저장된 데이터를 출력하는 함수
void debugQ(QueueType *cQ);						// Queue에 저장된 데이터를 디버깅하는 함수
element getElement(); 							// element를 입력받는 함수

int main(void)
{
	printf("----------[정새연] [2023041074]----------\n");

	QueueType *cQ = createQueue(); 				// QueueType의 *cp를 선언하고 createQueue()함수를 호출하여 반환값 대입
	element data; 				  				// element data 선언
	char command; 				  				// char command 선언

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {                     	// command에 따라 case 실행
		case 'i': case 'I': 				  	// i or I 입력 시
			data = getElement(); 			  	// data에 getElement() 함수의 반환 값 저장
			enQueue(cQ, data); 				  	// enQueue() 함수 호출
			break;
		case 'd': case 'D': 				  	// d or D 입력 시
			deQueue(cQ, &data); 			  	// deQueue() 함수 호출
			break; 
		case 'p': case 'P':				 	 	// p or P 입력 시
			printQ(cQ); 					  	// printQ() 함수 호출
			break;
		case 'b': case 'B': 				  	// b or B 입력 시
			debugQ(cQ); 					  	// debugQ() 함수 호출
			break;
		case 'q': case 'Q': 				  	// q or Q 입력 시
   	    	freeQueue(cQ); 					 	// freeQueue() 함수 호출
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); 	// command가 q or Q가 아닐 때 반복


	return 1;
}

QueueType *createQueue() 							// Queue 생성 함수
{
	QueueType *cQ; 									// QueueType의 *cQ 선언
	cQ = (QueueType *)malloc(sizeof(QueueType)); 	// cQ에 QueueType 크기의 동적 메모리 할당
	cQ->front = 0; 							 		// front를 0으로 초기화
	cQ->rear = 0;	 								// rear를 0으로 초기화
	return cQ; 										// cQ 반환
}

int freeQueue(QueueType *cQ) 						// Queue 해제 함수
{
    if(cQ == NULL) return 1; 						// cQ가 NULL이면 1 리턴
    free(cQ); 										// cQ 메모리 해제
    return 1;
}

element getElement() 								// element를 입력받는 함수
{
	element item; 									// element item 선언
	printf("Input element = "); 
	scanf(" %c", &item); 
	return item;
}


int isEmpty(QueueType *cQ)  						// Queue가 비어있는지 확인하는 함수
{
	if (cQ->front == cQ->rear)						// front 와 rear가 같다면 큐가 비어있음
		return 1;
	else
		return 0;
    return 0;
}

int isFull(QueueType *cQ) 							// Queue가 꽉 차 있는지 확인하는 함수
{	
	if ((cQ->rear + 1) % MAX_QUEUE_SIZE == cQ->front % MAX_QUEUE_SIZE) // (rear+1)% MAXSIZE 와 front % MAXSIZE 가 같으면 큐는 FULL 상태
		return 1;
	else
		return 0;
    return 0;
}

void enQueue(QueueType *cQ, element item) {
    cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; 	// rear 위치를 한 칸 뒤로 이동
    if (isFull(cQ)) {                           	// 큐가 가득 찼는지 확인
        printf("Circular Queue is full!\n");     	// 큐가 가득 찼으면 오류 메시지 출력
        return;                                  	// 함수 종료
    }
    cQ->queue[cQ->rear] = item;                 	// rear 위치에 데이터 저장
}

void deQueue(QueueType *cQ, element* item) {
    if (isEmpty(cQ)) {                           	// 큐가 비어있는지 확인
        printf("Circular Queue is already empty!\n");
        return;                                  	// 큐가 비어있으면 오류 메시지 출력 후 함수 종료
    }
    cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE; 	// front 위치를 한 칸 뒤로 이동
    *item = cQ->queue[cQ->front];                 	// front 위치의 데이터를 가져와서 item에 저장
}


void printQ(QueueType *cQ) 							// Queue에 저장된 데이터를 출력하는 함수
{
	int i, first, last; 							// i, first, last 선언

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; 	 	// front 증가
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; 		 	// rear 증가

	printf("Circular Queue : [");

	i = first;
	while(i != last){
		printf("%3c", cQ->queue[i]); 				// queue[i] 출력
		i = (i+1)%MAX_QUEUE_SIZE; 					// i 증가

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ) 									// Queue에 저장된 데이터를 디버깅하는 함수
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)  
	{
		if(i == cQ->front) { 								// front 위치 출력
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); 			// queue[i] 출력

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // 현재 front와 rear 위치 출력
}
