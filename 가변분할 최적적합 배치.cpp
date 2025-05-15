#include<iostream> 
#define mxsz 10000 // 큰 값으로 사용 할 더미
#define max 1000 // 메모리의 크기로 테스트마다 변경

bool memory[max] = { 0, }; // 총 메모리 스페이스 0이면 free, 1이면 using으로 판단

struct Me { // 메모리 정보 구조체
	int msize; // 작업이 차지하는 메모리
	int madd[2]; // 작업이 차지하고 있는 메모리 번지 [0]은 시작 [1]은 종료번지
				// [0]은 시작주소 [1]은 [0]+(msize-1)값
};

struct prc {
	int ID; // 작업 ID
	int time; // 주기억장치를 점유 할 시간, 이후 수명이라 하겠음
	int pmn; // 메모리에 추가 된 순서(링크드 리스트로 메모리 점유중인 프로세스를 구현했는데, 수명이 다 했을 때 이 순서를 알아야 제거가 쉬움)
	struct Me mem; // 프로세스의 메모리 정보 
};

int pmcounter = 0; // pm(메모리 안의 프로세스)의 개수

// 큐 구현 시작
struct prc wq[max]; // 대기큐를 프로세스 배열로 선언
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
	if (rear == max - 1) flag = 1;
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

prc dequeue() { // 큐 맨 앞의 프로세스를 제거하는 함수
	struct prc temp;
	if (empty()) {
		temp.ID = -2;
		return temp; // 큐가 비었으면 함수종료
	}
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
// 메모리를 점유 중인 프로세스들을 링크드 리스트로 구현

struct node {
	struct prc pm; // 메모리에 있는 프로세스(이후 수명을 계속 까서 시간이 다 된 프로세스는 빠지게 할 예정)
	struct node* next;
};
struct node* head;

void inserthead(prc a) { // 헤드에 프로세스를 삽입
	struct node* temp = new node;
	temp->pm = a; temp->next = NULL;
	if (pmcounter != 0)
		temp->next = head;
	head = temp;
	pmcounter++;
	return;
}

void inserttail(prc a) { // 링크드 리스트 꼬리에 삽입
	if (pmcounter == 0) { // 링크드 리스트가 비었다면 헤드삽입 함수 실행
		inserthead(a);
		return;
	}
	struct node* temp = new node;
	struct node* temp2 = new node;
	temp = head;
	temp2->pm = a; temp2->next = NULL;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	temp = temp;
	temp->next = temp2;
	pmcounter++;
	return;
}

void delete_pm(int ID) { // 프로세스 수명이 다 되었을때 호출되어 해당 프로세스를 실행 리스트에서 제거 후 메모리를 유휴(0)로 돌림
	struct node* temp = new node;
	temp = head;
	if (head->pm.ID == ID) { // 헤드가 제거 될 노드면 실행
		for (int i = temp->pm.mem.madd[0]; i <= temp->pm.mem.madd[1]; i++) memory[i] = 0; // 사용이 끝난 메모리 번지를 0으로 셋함
		head = temp->next;
		delete(temp); // 노드 삭제
		temp = NULL;
		pmcounter--; // 삭제되었으니 pmcounter를 감소
		return;
	}
	else { // 제거 할 노드가 헤드가 아니면 실행
		while (1) {
			if (temp == NULL || temp->next == NULL) break; // temp == NULL인지 검사하는 이유는 만약 이전 반복에서 끝 노드가 삭제되었으면 현제 노드가 NULL이라 temp->next를 검사할 떄 오류가 발생하기 떄문이다.
			if (temp->next->pm.ID == ID) {
				struct node* temp2 = temp->next;
				if (temp2->next != NULL) temp->next = temp2->next;
				else temp->next = NULL;
				for (int i = temp2->pm.mem.madd[0]; i <= temp2->pm.mem.madd[1]; i++) memory[i] = 0; // 사용이 끝난 메모리 번지를 0으로 셋함
				delete(temp2);
				temp2 = NULL;
			}
			temp = temp->next;
		}
		pmcounter--; // 삭제되었으니 pmcounter를 감소
		return;
	}
}
//링크드리스트 구현 종료 

// 최적적합 파트
struct Me fragment[max]; // 프래그먼트
int fn = 0; // 프래그먼트(블록) 개수

void fragmentcheck() {
	fn = 0; // 프래그먼트 개수를 0으로 초기화한 후 카운팅 시작
	for (int i = 0; i < max; i++) {
		if (memory[i] == 0) { // 빈 메모리 주소지를 발견하면 프래그먼트 정보를 입력시작
			fn++; // 프래그먼트 개수 1 증가
			for (int j = i; j < max; j++) {
				if (memory[j] == 1 || (j == max - 1 && memory[j] == 0)) { // 활성화된(1인) 메모리 번지를 찾거나 끝에 도달 할 때 까지 못 찾으면 실행
					fragment[fn - 1].madd[0] = i; // fn-1번 프래그먼트 시작주소는 i
					fragment[fn - 1].madd[1] = j; // fn-1번 프래그먼트 끝주소는 j-1번(j 번지가 활성화가 되어있거나 끝이기 떄문에)
					fragment[fn - 1].msize = fragment[fn - 1].madd[1] - fragment[fn - 1].madd[0] + 1; // 프래그먼트 사이즈는 끝주소-시작주소+1이기 때문에 j-1-i+1 = j-i
					i = j + 1; // j까지 검사가 끝나고 입력이 끝났음으로 다음 탐색은 i+1번지가 아닌 j번지부터 탐색하면 됨
					break;
				}
			}
		}
	}
	return;
}

void bestfit(struct prc froP) {
	fragmentcheck(); // fragment 정보 재입력
	if (froP.ID == -2) return; // 큐가 비었으면(ID가 -2먄) 리턴
	int counter = 0; // 메모리 번지수를 나타낼 인덱스, 배열 인덱스가 0부터 시작하니 0으로 셋
	int flag1 = -2; // 자리를 찾았는지 나타내는 flag로 찾으면 1, 못 찾으면 -2으로 한 후 반복이 끝나고 -2이면 froP(pprc)를 다시 큐에 넣음 
	int waste[2]; // 프래그먼트에 넣었을 때의 낭비 [0]값은 이전에 검색 한 프래그먼트에 넣었을 때 값, [1]은 현재 탐색중인 프래그먼트에 넣었을 때 ㄱ밧
	int head, tail; // 각각 이번 프로세스가 차지 할 메모리 넘버의 시작과 끝
	waste[0] = mxsz - froP.mem.msize; // 임시로 큰 값으로 초기화 시킴
	while(counter < fn) { // 프래그먼트 배열 인데그사 fn-1까지기 때문에 counter < fn
		if (froP.mem.msize > fragment[counter].msize) counter++; // 만약 프래그먼트의 사이즈보다 해당 프로세스 크기가 크면 스킵
		else { // 프래그먼트 사이즈가 더 크면 주소 및 이전에 찾은 프래그먼트보다 낭비가 적은지 검사
			if (froP.mem.msize <= fragment[counter].msize) {
				waste[1] = fragment[counter].msize - froP.mem.msize; // 현재 프래그먼트에 넣었을 때의 낭비되는 값 기록
				if (waste[0] > waste[1]) { // 만약 이전에 탐색한 프래그먼트의 낭비보다 현재 프래그먼트일 때의 낭비가 적으면 실행
					flag1 = 1; // 프래그먼트를 찾았다고 flag를 1로 셋
					head = fragment[counter].madd[0]; // 프로세스가 담길 주소의 시작값
					tail = fragment[counter].madd[0] + froP.mem.msize - 1; // 프로세스가 담길 주소의 끝 값
					counter++; // 다음 프래그먼트 검사를 위한 카운터값 증가
				}
			}
		}
	}
	if (flag1 == -2) inqueue(froP); // 공간을 못 찾았으면 다시 큐에 프로세스를 삽입
	else {
		froP.mem.madd[0] = head; // 해당 프로세스가 받은 메모리주소 정보 입력
		froP.mem.madd[1] = tail; // 메모리주소 정보 입력
		inserttail(froP); // 프로세스를 링크드리스트(실행중인 프로세스 모음)에 추가
		for (int i = head; i <= tail; i++) memory[i] = 1; // 점유될 메모리 번지를 1로 셋해서 사용중임을 표시함
	}
	return;
}

void printM(int t) { // 메모리 상태 출력을 위한 함수
	printf("\n시간%d\n점유중인 프로세스\n", t);
	printf("프로세스 ID\t메모리 크기\t점유중인 메모리 주소\t남은 시간\n");
	struct node* temp = new node;
	temp = head;
	for (int i = 0; i < pmcounter; i++) {
		printf("P%d\t\t%d\t\t%d ~ %d\t\t\t%d\n", temp->pm.ID, temp->pm.mem.msize, temp->pm.mem.madd[0], temp->pm.mem.madd[1], temp->pm.time);
		temp = temp->next;
	}
	delete(temp);
	temp = NULL;
	printf("\n대기큐의 프로세스\n");
	printf("프로세스 ID\t요구 메모리크기\t요구 시간\n");
	for (int i = front; i < rear; i++) {
		printf("P%d\t\t%d\t\t%d\n", wq[i].ID, wq[i].mem.msize, wq[i].time);
	}
}

int main() {
	int inp = 0; // 프로세스 개수를 입력받을 버퍼
	int currentTime = 0; // 현재 프로그램 시간으로 1씩 증가하며 시간별 프로세스 상태들을 표시함
	struct prc inpp = {}; // 프로세스 정보 버퍼
	printf("작업 개수 입력 : ");
	scanf_s("%d", &inp);
	for (int i = 0; i < inp; i++) { // 작업 정보 입력 시작
		printf("%d번 프로세스의 메모리 크기 : ", i);
		scanf_s("%d", &inpp.mem.msize);
		printf("%d번 프로세스의 작업시간 : ", i);
		scanf_s("%d", &inpp.time);
		inpp.ID = i;
		inqueue(inpp); // 대기큐에 작업 입력
	}
	while (!empty() || !(pmcounter == 0)) { // 프로세스 대기큐와 실행중인 프로세스 개수가 0이면 종료인데 AND연산이 아닌 OR연산인 이유는 시작 pmcounter값이 0이고, 프로세스는 큐나 링크드리스트에만 존재하기에 작업이 끝나기 전엔 둘 다 비지 않아서 1이 안나오기 때문에 AND로 하면 실행이 안 된다.
		printM(currentTime); // 현재시간 프로세스 상태 출력함수
		if (pmcounter > 0) { // 실행 중인 프로세스가 1개라도 있으면 실행 중인 프로세스들의 time(수명)을 검사
			struct node* temp = new node;
			temp = head;
			int temppmcounter = pmcounter; // 삭제가 이루어지면 pm카운터가 깎이기 때문에 아후 실행되는 반복에 영향을 안 주기위해서 만듬
			for (int i = 0; i < temppmcounter; i++) {
				if (temp->pm.time <= 0) { // 만약 수명이 0이 되었다면 해당 프로세스를 링크드 리스트(실행 중)에서 삭제
					struct node* temp2 = new node;
					temp2 = temp->next;
					delete_pm(temp->pm.ID);
					temp = temp2;
				}
				else {
					temp->pm.time--; // 수명이 0이 아니라면 해당 프로세스의 수명을 1 감소시킴
					temp = temp->next;
				}
			}
		}
		bestfit(dequeue()); // 자리가 있는지 검사 시작
		currentTime++; // 현재 프로그램 시간 증가
	}
	printf("\n최적적합 시뮬레이션 끝.\n프로그램을 종료합니다.");
	return 0;
}