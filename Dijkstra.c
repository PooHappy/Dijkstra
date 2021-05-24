#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 9
#define INF 1000000

int count = 0;	// ��� �� �迭�� ���� ���� ��������

typedef struct GraphType {
	int n;
	int weight[MAX_VERTICES][MAX_VERTICES];
} GraphType;	//�׷��� ����

typedef struct Data {
	int start_key;	//'A'�� ���� ���� ���� ǥ�� �� ��
	int end_key;	//'A"�� ���� �� ���� ǥ�� �� ��
	int priv;		//����ġ ���� �� ��
}Data;			

int Distance[MAX_VERTICES];	//���� ����
int Found[MAX_VERTICES];	//���� �ߴ� �� 
int User[MAX_VERTICES][MAX_VERTICES];	//�� ��� ��� �̿� �ߴ� �� ������ 2���� �迭

void bubble_sort(Data *dt) {			//���� ����
	int temp = 0;
	for (int i = count - 1; i > 0; i--) {	//count-1���� 0���� �ݺ�
		for (int j = 0; j < i; j++) {		//0���� count-1�̸����� �ݺ�
			if (dt[j].priv < dt[j + 1].priv) {	//����ġ�� �� ������
				temp = dt[j + 1].priv;
				dt[j + 1].priv = dt[j].priv;
				dt[j].priv = temp;
				temp = dt[j + 1].start_key;
				dt[j + 1].start_key = dt[j].start_key;
				dt[j].start_key = temp;
				temp = dt[j + 1].end_key;
				dt[j + 1].end_key = dt[j].end_key;
				dt[j].end_key = temp;			//Data�� �� �� swap
			}
		}
	}
}

int choose(int Distance[], int n, int Found[]) {	//� ������ �� �� 
	int i, min, minpos;
	min = 100;
	minpos = 0;
	for (i = 0; i < n; i++) {
		if (Distance[i] < min && !Found[i]) {
			min = Distance[i];
			minpos = i;
		}
	}
	return minpos;
}	
typedef struct table {		//���� ���� ����
	int array[9];	// �ڱ� �ڽ��� -1
	char G[9];		//ABCDEFGHI
} table;
void shortest_path(GraphType* g, int start, table* tb) {
	for (int i = 0; i < g->n; i++) {	
		Distance[i] = g->weight[start][i];	//����ġ�� Distance�� ����
		Found[i] = FALSE;		//������ ���İ�����
		tb->array[start] = -1;	//�����ϴ� ���� �ڱ� �ڽ��� -1
	}
	Found[start] = TRUE;		//���� ����
	Distance[start] = 0;		//�ڱ� �ڽ��� ���̰� 0
	for (int i = 0; i < g->n - 1; i++) {
		int u = choose(Distance, g->n, Found);	//�̵� �� ���� ����
		Found[u] = TRUE;		//������ ����� ǥ��
		for (int w = 0; w < g->n; w++) {
			if (!Found[w]) {	//���� �� ã�� ���ο� ���ؼ�
				if (Distance[u] + g->weight[u][w] < Distance[w])	//���� �Ͽ� ã�� �뼱�� �� ª����
				{
					Distance[w] = Distance[u] + g->weight[u][w];	// ������Ʈ ���ش�. 
					tb->array[w] = u;	//tb�� ������Ʈ
				}
			}
		}
	}
	for (int i = 0; i < 9; i++) {
		int a = tb->array[i];
		if (a == -1) {	//User[a][i]���� [a]�� -1�� ���� ���� 
			a = i;		//ó���ϴµ� a�� i�� �����Ͽ� User[][]���� ���� �ʰ� ����
		}
		if (a != i) {
			User[a][i]++;	//User[a][i] ����
		}
	}		
}
void init_table(table *tb, int start) {	//table�� array[i]�� start -> �ڱ� �ڽ�
	for (int i = 0; i < 9; i++) {		//G�� ABCDEFGHI������ ����
		tb->array[i] = start;
		tb->G[i] = 'A' + i;
	}
}
void sort(Data *dt) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; i > j; j++) {
			User[j][i] += User[i][j];	//B-C�� C-B�� ���� �����̹Ƿ� 
			User[i][j] = 0;				//B-C�� ������
		}
	}
	for (int i = 0; i < 9; i++) {		//dt�迭�� User[i][j]�� 0�� �ƴϸ� �־���.
		for (int j = 0; j < 9; j++) {	//dt�迭 ���� ������ ���� count���� ����������
			if (User[i][j] != 0) {
				dt[count].start_key = i;
				dt[count].end_key = j;
				dt[count].priv = User[i][j];
				count++;
			}
		}
	}
	bubble_sort(dt);		//���� ���� 
}
int main(void) {
	int start = 0;
	Data dt[100] = { 0 };
	GraphType g = { 9,
		{
		{0, 9, 3, 8, 6, INF, 3, INF, INF},
		{9, 0, 2, 4, INF,INF, INF, INF, INF},
		{3, 2, 0, 7, 2, INF, INF, INF, INF},
		{8, 4, 7, 0, 6, 7, INF, 2, INF},
		{6, INF, 2, 6, 0, INF, INF, INF, INF},
		{INF, INF, INF, 7, INF, 0, 3, INF, 4},
		{3, INF, INF, INF, INF, 3, 0, INF, 6},
		{INF, INF, INF, 2, INF, INF, INF, 0, 2},
		{INF, INF, INF, INF, INF, 4, 6, 2, 0}
		}
	};	//�׷��� ����
	table tb = {0};	//table ����ü �ʱ�ȭ

	start = 0;	//������ 0 => A
	init_table(&tb, start);
	shortest_path(&g, 0, &tb);
	/*
	start = 1;	//������ 1 => B
	init_table(&tb, start);
	shortest_path(&g, 1, &tb);
	
	start = 2;
	init_table(&tb, start);
	shortest_path(&g, 2, &tb);
	
	start = 3;
	init_table(&tb, start);
	shortest_path(&g, 3, &tb);

	start = 4;
	init_table(&tb, start);
	shortest_path(&g, 4, &tb);

	start = 5;
	init_table(&tb, start);
	shortest_path(&g, 5, &tb);

	start = 6;
	init_table(&tb, start);
	shortest_path(&g, 6, &tb);

	start = 7;
	init_table(&tb, start);
	shortest_path(&g, 7, &tb);

	start = 8;
	init_table(&tb, start);
	shortest_path(&g, 8, &tb);
	*/
	sort(&dt);	//�ȿ��� ����
	for (int i = 0; i < count; i++) {
		if (i == (count - 1)) {
			printf(" (%c-%c, %d)", ('A' + dt[i].start_key), ('A' + dt[i].end_key), dt[i].priv);	//�������̸� -> ���� ���
			break;
		}
		printf(" (%c-%c, %d) ->", ('A' + dt[i].start_key), ('A' + dt[i].end_key), dt[i].priv);	//(A-B, 9) -> ������ ���
	}

	return 0;
}