#include<iostream> 
#define mxsz 10000
#define BlockN 3 // 500 300 200���� ���� �� ����


struct prc {
	int ID; // �۾� ID
	int time; // �ֱ����ġ�� ���� �� �ð�, ���� �����̶� �ϰ���
	int size; // �۾��� �ʿ��� �޸� ������
	int block; // �Ҵ� ���� ����� �ѹ�
};

int memory[BlockN] = { 500, 300, 200 }; // �� ����� ���� �迭�� ǥ��
int m_state[BlockN] = { -1, -1, -1 }; // �� ����� ����, ������̸� ���μ��� �����͸� �Է¹���

// ť ���� ����
struct prc wq[mxsz]; // ���ť�� ���μ��� �迭�� ����
struct prc run[mxsz]; // �������� ���μ��� ��Ͽ� �迭
int run_count = 0; // �������� ���μ��� ���� ���
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
	if (rear == mxsz - 1) flag = 1;
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

struct prc dequeue() { // ť �� ���� ���μ����� �����ϴ� �Լ�
	struct prc temp;
	if (empty()) {
		temp.ID = -2; // -2�� ������ ���ܰŷ��ؼ� �������� �Լ����� �ٷ� �����ų ��
		return temp;
	} // ť�� ������� �Լ�����
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

void bestfit(struct prc froP) {
	if (froP.ID == -2) return;
	int counter = 0; // �޸� �������� ��Ÿ�� �ε���, �迭 �ε����� 0���� �����ϴ� 0���� ��
	int waste[2]; // �����ġ ����� [0]�� ��ϵǾ��ִ� ����(�˰��򿡼� �ʱ� �����ġ ����)�̰� [1]�� ���� �˻��ϰ� �ִ� ��Ͽ� �־��� ���� ����
	waste[0] = mxsz - froP.size;
	int subscript = -1; // �Ҵ���� ��� �ѹ� ��Ͽ� ������ ��ϳѹ��� 0���� �����̴� -1�� �ʱ�ȭ �ص�
	while (counter < BlockN) {
		if (m_state[counter] != -1) {
			counter++;
			continue; // �ش� ����� �̹� �������̸� ���� ��� �˻�
		}
		else {
			if (froP.size > memory[counter]) counter++;
			else {
				waste[1] = memory[counter] - froP.size; // �����ġ ���� = �����ġ ũ�� - �۾� ũ��
				if (waste[0] > waste[1]) {
					subscript = counter; // ��Ͽ� �Ҵ� �� �������� ������ ���� ��� �˻� �� ���� ������ subscript�� ����
					waste[0] = waste[1];
					counter++;
				}
			}
		}
	}
	if (subscript == -1) inqueue(froP); // �� ã���� �ٽ� ť�� ����
	else {
		m_state[subscript] = froP.ID; // �ش� ����� �������� ���μ����� ID ���
		froP.block = subscript; // ��� �ѹ� ���
		run[run_count++] = froP; // �ش� ���μ����� �۾������� �̵�
	}
	return;
}

void removeP(int idx) {
	for (int i = 0; i < BlockN; i++) {
		if (run[idx].ID == m_state[i]) { // ������ ���μ����� ID�� ��� ����� ������ �ִ��� �˻�
			m_state[i] = -1; // ���� �� ���μ����� �����ϰ� �ִ� �޸��� ���¸� -1(����)�� �ٲ�
			break; // ���� �ݺ���(�޸� ��� �˻�) ����
		}
	}
	for (int i = idx; i < run_count; i++) {
		run[i] = run[i + 1]; // ���� �� ���μ����� �ִ� ��ġ���� ���� �迭��ҵ��� �� ĭ�� ���ܿ�
	}
	run_count--; // �������� ���μ��� ������ 1 ���ҽ�Ŵ
	return;

}

void printM(int t) { // �޸� ���� ����� ���� �Լ�
	printf("\n�ð�%d\n�������� ���μ���\n", t);
	printf("���μ���ID\t���\t���� �ð�\n");
	for (int i = 0; i < run_count; i++)
		printf("P[%d]\t\t���%d\t\t%d\n", run[i].ID, run[i].block, run[i].time);
	printf("\n���ť�� ���μ���\n");
	printf("���μ��� ID\t�ʿ��� �޸�ũ��\n");
	for (int i = front; i < rear; i++) {
		printf("P%d\t\t%d\n", wq[i].ID, wq[i].size);
	}
	return;
}

int main() {
	int inp = 0;
	int currentTime = 0; // ���� ���α׷� �ð�
	struct prc inpp = {}; // ���ۿ� �۾� ����
	printf("�۾� ���� �Է� : ");
	scanf_s("%d", &inp);
	for (int i = 0; i < inp; i++) { // �۾� ���� �Է� ����
		printf("%d�� ���μ����� �޸� ũ�� : ", i);
		scanf_s("%d", &inpp.size);
		printf("%d�� ���μ����� �۾��ð� : ", i);
		scanf_s("%d", &inpp.time);
		inpp.ID = i;
		inqueue(inpp); // ���ť�� �۾� �Է�
	}

	while (!empty() || !(run_count == 0)) {
		printM(currentTime);
		for (int i = 0; i < run_count; i++) {
			if (run[i].time <= 0) {
				removeP(i); // �۾� �ð��� ������ �۾� ����
				i = i--; // �� �پ� ����� ������ i�� 1 ���ҽ��Ѽ� ������ ���� ��° �׸��̾��� ���� �˻�
			}
			run[i].time--; // �����ð� 1 ����
		}
		bestfit(dequeue()); // �ڸ��� �ִ��� �˻� ����
		currentTime++; // ���� ���α׷� �ð� ����
	}
	printf("\n��� �۾��� �������ϴ�. \n�������� �޸� ��ġ �˰��� �׽�Ʈ ���α׷��� �����մϴ�.\n");
	return 0;
}