#include<iostream> 
#define mxsz 10000
#define BlockN 3 // 500 300 200으로 분할 할 예정


struct prc {
	int ID; // 작업 ID
	int time; // 주기억장치를 점유 할 시간, 이후 수명이라 하겠음
	int size; // 작업이 필요한 메모리 사이즈
	int block; // 할당 받은 블록의 넘버
};

int memory[BlockN] = { 500, 300, 200 }; // 각 블록을 합쳐 배열로 표현
int m_state[BlockN] = { -1, -1, -1 }; // 각 블록의 상태, 사용중이면 프로세스 포인터를 입력받음

// 큐 구현 시작
struct prc wq[mxsz]; // 대기큐를 프로세스 배열로 선언
struct prc run[mxsz]; // 실행중인 프로세스 기록용 배열
int run_count = 0; // 실행중인 프로세스 개수 기록
int front = 0; // 큐의 머리
int rear = 0; // 큐의 꼬리

int empty() { // 큐가 비었으면 1리턴, 아니면 0 리턴
	int flag = -1;
	if (front == rear)
		flag = 1;
	else flag = 0;
	return flag;
}

int full() { // 큐가 꽉 찼으면 1을 리턴, 아니면 0 리턴
	int flag = -1;
	if (rear == mxsz - 1) flag = 1;
	else flag = 0;
	return flag;
}

void inqueue(prc a) { // 큐에 프로세스를 추가하는 함수
	if (full())	return; // 큐가 꽉 찼으면 추가 x
	if (empty()) { // 비었으면 머리와 꼬리를 -1이므로 0으로 세팅
		rear = 0;
		front = 0;
	}
	wq[rear] = a; // 큐의 맨 뒤에 프로세스 추가
	rear++; // 찬거나 빈게 아니면 꼬리 1 증가
	return;
}

struct prc dequeue() { // 큐 맨 앞의 프로세스를 제거하는 함수
	struct prc temp;
	if (empty()) {
		temp.ID = -2; // -2면 빼낸게 없단거로해서 최적적합 함수에서 바로 종료시킬 것
		return temp;
	} // 큐가 비었으면 함수종료
	else {
		temp = wq[front]; // 빈게 아니면 temp에 빼낼 프로세스를 넣어줌
		front++; // 빼냈으니 front를 1증가
	}
	if (empty()) { // 0로 셋해서 다음 입력이 0부터 차게 설정
		front = 0;
		rear = 0;
	}
	return temp;
}
// 큐 구현 종료

void bestfit(struct prc froP) {
	if (froP.ID == -2) return;
	int counter = 0; // 메모리 번지수를 나타낼 인덱스, 배열 인덱스가 0부터 시작하니 0으로 셋
	int waste[2]; // 기억장치 낭비로 [0]이 기록되어있는 낭비(알고리즘에서 초기 기억장치 낭비)이고 [1]이 현제 검사하고 있는 블록에 넣었을 때의 낭비
	waste[0] = mxsz - froP.size;
	int subscript = -1; // 할당받을 블록 넘버 기록용 변수로 블록넘버가 0부터 시작이니 -1로 초기화 해둠
	while (counter < BlockN) {
		if (m_state[counter] != -1) {
			counter++;
			continue; // 해당 블록이 이미 점유중이면 다음 블록 검사
		}
		else {
			if (froP.size > memory[counter]) counter++;
			else {
				waste[1] = memory[counter] - froP.size; // 기억장치 낭비 = 기억장치 크기 - 작업 크기
				if (waste[0] > waste[1]) {
					subscript = counter; // 블록에 할당 시 버려지는 공간이 이전 블록 검사 때 보다 적으면 subscript에 적재
					waste[0] = waste[1];
					counter++;
				}
			}
		}
	}
	if (subscript == -1) inqueue(froP); // 못 찾으면 다시 큐에 넣음
	else {
		m_state[subscript] = froP.ID; // 해당 블록을 점유중인 프로세스의 ID 기록
		froP.block = subscript; // 블록 넘버 기록
		run[run_count++] = froP; // 해당 프로세스를 작업중으로 이동
	}
	return;
}

void removeP(int idx) {
	for (int i = 0; i < BlockN; i++) {
		if (run[idx].ID == m_state[i]) { // 삭제될 프로세스의 ID를 어느 블록이 가지고 있는지 검사
			m_state[i] = -1; // 삭제 될 프로세스가 점유하고 있는 메모리의 상태를 -1(유휴)로 바꿈
			break; // 이후 반복문(메모리 블록 검색) 종료
		}
	}
	for (int i = idx; i < run_count; i++) {
		run[i] = run[i + 1]; // 삭제 될 프로세스가 있는 위치부터 뒤의 배열요소들을 한 칸씩 땡겨옴
	}
	run_count--; // 실행중인 프로세스 개수를 1 감소시킴
	return;

}

void printM(int t) { // 메모리 상태 출력을 위한 함수
	printf("\n시간%d\n점유중인 프로세스\n", t);
	printf("프로세스ID\t블록\t남은 시간\n");
	for (int i = 0; i < run_count; i++)
		printf("P[%d]\t\t블록%d\t\t%d\n", run[i].ID, run[i].block, run[i].time);
	printf("\n대기큐의 프로세스\n");
	printf("프로세스 ID\t필요한 메모리크기\n");
	for (int i = front; i < rear; i++) {
		printf("P%d\t\t%d\n", wq[i].ID, wq[i].size);
	}
	return;
}

int main() {
	int inp = 0;
	int currentTime = 0; // 현재 프로그램 시간
	struct prc inpp = {}; // 버퍼용 작업 변수
	printf("작업 개수 입력 : ");
	scanf_s("%d", &inp);
	for (int i = 0; i < inp; i++) { // 작업 정보 입력 시작
		printf("%d번 프로세스의 메모리 크기 : ", i);
		scanf_s("%d", &inpp.size);
		printf("%d번 프로세스의 작업시간 : ", i);
		scanf_s("%d", &inpp.time);
		inpp.ID = i;
		inqueue(inpp); // 대기큐에 작업 입력
	}

	while (!empty() || !(run_count == 0)) {
		printM(currentTime);
		for (int i = 0; i < run_count; i++) {
			if (run[i].time <= 0) {
				removeP(i); // 작업 시간이 끝나면 작업 삭제
				i = i--; // 한 줄씩 땡겼기 때문에 i를 1 감소시켜서 삭제된 다음 번째 항목이었던 것을 검사
			}
			run[i].time--; // 남은시간 1 감소
		}
		bestfit(dequeue()); // 자리가 있는지 검사 시작
		currentTime++; // 현재 프로그램 시간 증가
	}
	printf("\n모든 작업이 끝났습니다. \n최적적합 메모리 배치 알고리즘 테스트 프로그램을 종료합니다.\n");
	return 0;
}