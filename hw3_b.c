#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 9
#define INF 1000000

int count = 0;	// 출력 할 배열의 길이 저장 전역변수

typedef struct GraphType {
	int n;
	int weight[MAX_VERTICES][MAX_VERTICES];
} GraphType;	//그래프 선언

typedef struct Data {
	int start_key;	//'A'에 더할 시작 정점 표현 할 수
	int end_key;	//'A"에 더할 끝 정점 표현 할 수
	int priv;		//가중치 저장 할 수
}Data;			

int Distance[MAX_VERTICES];	//길이 저장
int Found[MAX_VERTICES];	//저장 했는 지 
int User[MAX_VERTICES][MAX_VERTICES];	//각 경로 몇번 이용 했는 지 저장할 2차원 배열

void bubble_sort(Data *dt) {			//버블 정렬
	int temp = 0;
	for (int i = count - 1; i > 0; i--) {	//count-1부터 0까지 반복
		for (int j = 0; j < i; j++) {		//0부터 count-1미만까지 반복
			if (dt[j].priv < dt[j + 1].priv) {	//가중치가 더 작으면
				temp = dt[j + 1].priv;
				dt[j + 1].priv = dt[j].priv;
				dt[j].priv = temp;
				temp = dt[j + 1].start_key;
				dt[j + 1].start_key = dt[j].start_key;
				dt[j].start_key = temp;
				temp = dt[j + 1].end_key;
				dt[j + 1].end_key = dt[j].end_key;
				dt[j].end_key = temp;			//Data의 각 값 swap
			}
		}
	}
}

int choose(int Distance[], int n, int Found[]) {	//어떤 정점을 고를 지 
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
typedef struct table {		//이전 정점 저장
	int array[9];	// 자기 자신은 -1
	char G[9];		//ABCDEFGHI
} table;
void shortest_path(GraphType* g, int start, table* tb) {
	for (int i = 0; i < g->n; i++) {	
		Distance[i] = g->weight[start][i];	//가중치를 Distance에 저장
		Found[i] = FALSE;		//정점을 거쳐갔는지
		tb->array[start] = -1;	//시작하는 정점 자기 자신은 -1
	}
	Found[start] = TRUE;		//시작 정점
	Distance[start] = 0;		//자기 자신은 길이가 0
	for (int i = 0; i < g->n - 1; i++) {
		int u = choose(Distance, g->n, Found);	//이동 할 정점 선택
		Found[u] = TRUE;		//정점을 골랐다 표시
		for (int w = 0; w < g->n; w++) {
			if (!Found[w]) {	//아직 안 찾은 도로에 한해서
				if (Distance[u] + g->weight[u][w] < Distance[w])	//경유 하여 찾은 노선이 더 짧으면
				{
					Distance[w] = Distance[u] + g->weight[u][w];	// 업데이트 해준다. 
					tb->array[w] = u;	//tb에 업데이트
				}
			}
		}
	}
	for (int i = 0; i < 9; i++) {
		int a = tb->array[i];
		if (a == -1) {	//User[a][i]에서 [a]가 -1이 들어가면 오류 
			a = i;		//처리하는데 a와 i를 같게하여 User[][]에는 들어가지 않게 설정
		}
		if (a != i) {
			User[a][i]++;	//User[a][i] 증가
		}
	}		
}
void init_table(table *tb, int start) {	//table의 array[i]는 start -> 자기 자신
	for (int i = 0; i < 9; i++) {		//G는 ABCDEFGHI순으로 설정
		tb->array[i] = start;
		tb->G[i] = 'A' + i;
	}
}
void sort(Data *dt) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; i > j; j++) {
			User[j][i] += User[i][j];	//B-C와 C-B는 같은 간선이므로 
			User[i][j] = 0;				//B-C로 합쳐줌
		}
	}
	for (int i = 0; i < 9; i++) {		//dt배열에 User[i][j]가 0이 아니면 넣어줌.
		for (int j = 0; j < 9; j++) {	//dt배열 길이 측정을 위해 count값을 증가시켜줌
			if (User[i][j] != 0) {
				dt[count].start_key = i;
				dt[count].end_key = j;
				dt[count].priv = User[i][j];
				count++;
			}
		}
	}
	bubble_sort(dt);		//버블 정렬 
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
	};	//그래프 설정
	table tb = {0};	//table 구조체 초기화

	start = 0;	//시작은 0 => A
	init_table(&tb, start);
	shortest_path(&g, 0, &tb);
	/*
	start = 1;	//시작은 1 => B
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
	sort(&dt);	//안에서 연산
	for (int i = 0; i < count; i++) {
		if (i == (count - 1)) {
			printf(" (%c-%c, %d)", ('A' + dt[i].start_key), ('A' + dt[i].end_key), dt[i].priv);	//마지막이면 -> 빼고 출력
			break;
		}
		printf(" (%c-%c, %d) ->", ('A' + dt[i].start_key), ('A' + dt[i].end_key), dt[i].priv);	//(A-B, 9) -> 식으로 출력
	}

	return 0;
}