#include<iostream> 
#define mxsz 10000 // ū ������ ��� �� ����
#define max 1000 // �޸��� ũ��� �׽�Ʈ���� ����

bool memory[max] = { 0, }; // �� �޸� �����̽� 0�̸� free, 1�̸� using���� �Ǵ�

struct Me { // �޸� ���� ����ü
	int msize; // �۾��� �����ϴ� �޸�
	int madd[2]; // �۾��� �����ϰ� �ִ� �޸� ���� [0]�� ���� [1]�� �������
				// [0]�� �����ּ� [1]�� [0]+(msize-1)��
};

struct prc {
	int ID; // �۾� ID
	int time; // �ֱ����ġ�� ���� �� �ð�, ���� �����̶� �ϰ���
	int pmn; // �޸𸮿� �߰� �� ����(��ũ�� ����Ʈ�� �޸� �������� ���μ����� �����ߴµ�, ������ �� ���� �� �� ������ �˾ƾ� ���Ű� ����)
	struct Me mem; // ���μ����� �޸� ���� 
};

int pmcounter = 0; // pm(�޸� ���� ���μ���)�� ����

// ť ���� ����
struct prc wq[max]; // ���ť�� ���μ��� �迭�� ����
int front = 0; // ť�� �Ӹ�
int rear = 0; // ť�� ����

int empty() { // ť�� ������� 1����, �ƴϸ� 0 ����
	int flag = -1;
	if (front == rear)
		flag = 1;
	else flag = 0;
	return flag;
}

int full() { // ť�� �� á���� 1�� ����, �ƴϸ� 0 ����
	int flag = -1;
	if (rear == max - 1) flag = 1;
	else flag = 0;
	return flag;
}

void inqueue(prc a) { // ť�� ���μ����� �߰��ϴ� �Լ�
	if (full())	return; // ť�� �� á���� �߰� x
	if (empty()) { // ������� �Ӹ��� ������ -1�̹Ƿ� 0���� ����
		rear = 0;
		front = 0;
	}
	wq[rear] = a; // ť�� �� �ڿ� ���μ��� �߰�
	rear++; // ���ų� ��� �ƴϸ� ���� 1 ����
	return;
}

prc dequeue() { // ť �� ���� ���μ����� �����ϴ� �Լ�
	struct prc temp;
	if (empty()) {
		temp.ID = -2;
		return temp; // ť�� ������� �Լ�����
	}
	else {
		temp = wq[front]; // ��� �ƴϸ� temp�� ���� ���μ����� �־���
		front++; // �������� front�� 1����
	}
	if (empty()) { // 0�� ���ؼ� ���� �Է��� 0���� ���� ����
		front = 0;
		rear = 0;
	}
	return temp;
}
// ť ���� ����
// �޸𸮸� ���� ���� ���μ������� ��ũ�� ����Ʈ�� ����

struct node {
	struct prc pm; // �޸𸮿� �ִ� ���μ���(���� ������ ��� � �ð��� �� �� ���μ����� ������ �� ����)
	struct node* next;
};
struct node* head;

void inserthead(prc a) { // ��忡 ���μ����� ����
	struct node* temp = new node;
	temp->pm = a; temp->next = NULL;
	if (pmcounter != 0)
		temp->next = head;
	head = temp;
	pmcounter++;
	return;
}

void inserttail(prc a) { // ��ũ�� ����Ʈ ������ ����
	if (pmcounter == 0) { // ��ũ�� ����Ʈ�� ����ٸ� ������ �Լ� ����
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

void delete_pm(int ID) { // ���μ��� ������ �� �Ǿ����� ȣ��Ǿ� �ش� ���μ����� ���� ����Ʈ���� ���� �� �޸𸮸� ����(0)�� ����
	struct node* temp = new node;
	temp = head;
	if (head->pm.ID == ID) { // ��尡 ���� �� ���� ����
		for (int i = temp->pm.mem.madd[0]; i <= temp->pm.mem.madd[1]; i++) memory[i] = 0; // ����� ���� �޸� ������ 0���� ����
		head = temp->next;
		delete(temp); // ��� ����
		temp = NULL;
		pmcounter--; // �����Ǿ����� pmcounter�� ����
		return;
	}
	else { // ���� �� ��尡 ��尡 �ƴϸ� ����
		while (1) {
			if (temp == NULL || temp->next == NULL) break; // temp == NULL���� �˻��ϴ� ������ ���� ���� �ݺ����� �� ��尡 �����Ǿ����� ���� ��尡 NULL�̶� temp->next�� �˻��� �� ������ �߻��ϱ� �����̴�.
			if (temp->next->pm.ID == ID) {
				struct node* temp2 = temp->next;
				if (temp2->next != NULL) temp->next = temp2->next;
				else temp->next = NULL;
				for (int i = temp2->pm.mem.madd[0]; i <= temp2->pm.mem.madd[1]; i++) memory[i] = 0; // ����� ���� �޸� ������ 0���� ����
				delete(temp2);
				temp2 = NULL;
			}
			temp = temp->next;
		}
		pmcounter--; // �����Ǿ����� pmcounter�� ����
		return;
	}
}
//��ũ�帮��Ʈ ���� ���� 

// �������� ��Ʈ
struct Me fragment[max]; // �����׸�Ʈ
int fn = 0; // �����׸�Ʈ(���) ����

void fragmentcheck() {
	fn = 0; // �����׸�Ʈ ������ 0���� �ʱ�ȭ�� �� ī���� ����
	for (int i = 0; i < max; i++) {
		if (memory[i] == 0) { // �� �޸� �ּ����� �߰��ϸ� �����׸�Ʈ ������ �Է½���
			fn++; // �����׸�Ʈ ���� 1 ����
			for (int j = i; j < max; j++) {
				if (memory[j] == 1 || (j == max - 1 && memory[j] == 0)) { // Ȱ��ȭ��(1��) �޸� ������ ã�ų� ���� ���� �� �� ���� �� ã���� ����
					fragment[fn - 1].madd[0] = i; // fn-1�� �����׸�Ʈ �����ּҴ� i
					fragment[fn - 1].madd[1] = j; // fn-1�� �����׸�Ʈ ���ּҴ� j-1��(j ������ Ȱ��ȭ�� �Ǿ��ְų� ���̱� ������)
					fragment[fn - 1].msize = fragment[fn - 1].madd[1] - fragment[fn - 1].madd[0] + 1; // �����׸�Ʈ ������� ���ּ�-�����ּ�+1�̱� ������ j-1-i+1 = j-i
					i = j + 1; // j���� �˻簡 ������ �Է��� ���������� ���� Ž���� i+1������ �ƴ� j�������� Ž���ϸ� ��
					break;
				}
			}
		}
	}
	return;
}

void bestfit(struct prc froP) {
	fragmentcheck(); // fragment ���� ���Է�
	if (froP.ID == -2) return; // ť�� �������(ID�� -2��) ����
	int counter = 0; // �޸� �������� ��Ÿ�� �ε���, �迭 �ε����� 0���� �����ϴ� 0���� ��
	int flag1 = -2; // �ڸ��� ã�Ҵ��� ��Ÿ���� flag�� ã���� 1, �� ã���� -2���� �� �� �ݺ��� ������ -2�̸� froP(pprc)�� �ٽ� ť�� ���� 
	int waste[2]; // �����׸�Ʈ�� �־��� ���� ���� [0]���� ������ �˻� �� �����׸�Ʈ�� �־��� �� ��, [1]�� ���� Ž������ �����׸�Ʈ�� �־��� �� ����
	int head, tail; // ���� �̹� ���μ����� ���� �� �޸� �ѹ��� ���۰� ��
	waste[0] = mxsz - froP.mem.msize; // �ӽ÷� ū ������ �ʱ�ȭ ��Ŵ
	while(counter < fn) { // �����׸�Ʈ �迭 �ε��׻� fn-1������ ������ counter < fn
		if (froP.mem.msize > fragment[counter].msize) counter++; // ���� �����׸�Ʈ�� ������� �ش� ���μ��� ũ�Ⱑ ũ�� ��ŵ
		else { // �����׸�Ʈ ����� �� ũ�� �ּ� �� ������ ã�� �����׸�Ʈ���� ���� ������ �˻�
			if (froP.mem.msize <= fragment[counter].msize) {
				waste[1] = fragment[counter].msize - froP.mem.msize; // ���� �����׸�Ʈ�� �־��� ���� ����Ǵ� �� ���
				if (waste[0] > waste[1]) { // ���� ������ Ž���� �����׸�Ʈ�� ���񺸴� ���� �����׸�Ʈ�� ���� ���� ������ ����
					flag1 = 1; // �����׸�Ʈ�� ã�Ҵٰ� flag�� 1�� ��
					head = fragment[counter].madd[0]; // ���μ����� ��� �ּ��� ���۰�
					tail = fragment[counter].madd[0] + froP.mem.msize - 1; // ���μ����� ��� �ּ��� �� ��
					counter++; // ���� �����׸�Ʈ �˻縦 ���� ī���Ͱ� ����
				}
			}
		}
	}
	if (flag1 == -2) inqueue(froP); // ������ �� ã������ �ٽ� ť�� ���μ����� ����
	else {
		froP.mem.madd[0] = head; // �ش� ���μ����� ���� �޸��ּ� ���� �Է�
		froP.mem.madd[1] = tail; // �޸��ּ� ���� �Է�
		inserttail(froP); // ���μ����� ��ũ�帮��Ʈ(�������� ���μ��� ����)�� �߰�
		for (int i = head; i <= tail; i++) memory[i] = 1; // ������ �޸� ������ 1�� ���ؼ� ��������� ǥ����
	}
	return;
}

void printM(int t) { // �޸� ���� ����� ���� �Լ�
	printf("\n�ð�%d\n�������� ���μ���\n", t);
	printf("���μ��� ID\t�޸� ũ��\t�������� �޸� �ּ�\t���� �ð�\n");
	struct node* temp = new node;
	temp = head;
	for (int i = 0; i < pmcounter; i++) {
		printf("P%d\t\t%d\t\t%d ~ %d\t\t\t%d\n", temp->pm.ID, temp->pm.mem.msize, temp->pm.mem.madd[0], temp->pm.mem.madd[1], temp->pm.time);
		temp = temp->next;
	}
	delete(temp);
	temp = NULL;
	printf("\n���ť�� ���μ���\n");
	printf("���μ��� ID\t�䱸 �޸�ũ��\t�䱸 �ð�\n");
	for (int i = front; i < rear; i++) {
		printf("P%d\t\t%d\t\t%d\n", wq[i].ID, wq[i].mem.msize, wq[i].time);
	}
}

int main() {
	int inp = 0; // ���μ��� ������ �Է¹��� ����
	int currentTime = 0; // ���� ���α׷� �ð����� 1�� �����ϸ� �ð��� ���μ��� ���µ��� ǥ����
	struct prc inpp = {}; // ���μ��� ���� ����
	printf("�۾� ���� �Է� : ");
	scanf_s("%d", &inp);
	for (int i = 0; i < inp; i++) { // �۾� ���� �Է� ����
		printf("%d�� ���μ����� �޸� ũ�� : ", i);
		scanf_s("%d", &inpp.mem.msize);
		printf("%d�� ���μ����� �۾��ð� : ", i);
		scanf_s("%d", &inpp.time);
		inpp.ID = i;
		inqueue(inpp); // ���ť�� �۾� �Է�
	}
	while (!empty() || !(pmcounter == 0)) { // ���μ��� ���ť�� �������� ���μ��� ������ 0�̸� �����ε� AND������ �ƴ� OR������ ������ ���� pmcounter���� 0�̰�, ���μ����� ť�� ��ũ�帮��Ʈ���� �����ϱ⿡ �۾��� ������ ���� �� �� ���� �ʾƼ� 1�� �ȳ����� ������ AND�� �ϸ� ������ �� �ȴ�.
		printM(currentTime); // ����ð� ���μ��� ���� ����Լ�
		if (pmcounter > 0) { // ���� ���� ���μ����� 1���� ������ ���� ���� ���μ������� time(����)�� �˻�
			struct node* temp = new node;
			temp = head;
			int temppmcounter = pmcounter; // ������ �̷������ pmī���Ͱ� ���̱� ������ ���� ����Ǵ� �ݺ��� ������ �� �ֱ����ؼ� ����
			for (int i = 0; i < temppmcounter; i++) {
				if (temp->pm.time <= 0) { // ���� ������ 0�� �Ǿ��ٸ� �ش� ���μ����� ��ũ�� ����Ʈ(���� ��)���� ����
					struct node* temp2 = new node;
					temp2 = temp->next;
					delete_pm(temp->pm.ID);
					temp = temp2;
				}
				else {
					temp->pm.time--; // ������ 0�� �ƴ϶�� �ش� ���μ����� ������ 1 ���ҽ�Ŵ
					temp = temp->next;
				}
			}
		}
		bestfit(dequeue()); // �ڸ��� �ִ��� �˻� ����
		currentTime++; // ���� ���α׷� �ð� ����
	}
	printf("\n�������� �ùķ��̼� ��.\n���α׷��� �����մϴ�.");
	return 0;
}