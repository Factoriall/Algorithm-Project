#define _CRT_SECURE_NO_WARNINGS
#include "RBT.h"
#include "city_reservation.h"
#include "priority_queue.h"
#include <string.h>
#include <math.h>
#include <time.h>

#define INF 987654321

coordinate city[26];//���� ��ǥ ����
int graph[26][26];//�׷����� ����Ǿ��ִ��� Ȯ���ϴ� ����
int timeTable[26][26][32];//�ð� ������ ����
int isUsed[10000];//�����ȣ�� ���Ǿ����� Ȯ���ϴ� ����

int DataPriorityComp(HData a, HData b) {//������ ���� �� ���� �ø��� ���� �Լ�
	return b.arr_time - a.arr_time;
}

double getDistance(int c1, int c2) {//�Ÿ� ���ϴ� �Լ�
	return sqrt((city[c1].x - city[c2].x) * (city[c1].x - city[c2].x) + (city[c1].y - city[c2].y) * (city[c1].y - city[c2].y));
}

void Dijkstra(int s, int date, int hr, int dist[26], int prev[26], double flightHour[26]) {//���ͽ�Ʈ�� �˰��� ����, �ִ� ���� �ð� ����
	PQueue pq;
	PQueueInit(&pq, DataPriorityComp);

	dist[s] = date * 100;
	int isVisited[26] = { 0, };
	travel tr;
	tr.cityNum = s;
	tr.arr_time = date * 100 + hr;
	PEnqueue(&pq, tr);

	while (!PQIsEmpty(&pq)) {
		int curr, time;

		do {
			curr = pq.heapArr[1].cityNum;
			time = pq.heapArr[1].arr_time;
			PDequeue(&pq);
		} while (!PQIsEmpty(&pq) && isVisited[curr]);

		if (isVisited[curr]) break;

		isVisited[curr] = 1;
		for (int next = 0; next < 26; next++) {
			if (graph[curr][next]) {
				int day = ((time / 100) - 1) % 31 + 1;
				int hour = time % 100;
				int take_time;
				if (hour <= timeTable[curr][next][day]) {
					take_time = timeTable[curr][next][day] + (int)ceil(getDistance(curr, next) / 500);//������ + �̵� �ð�
					take_time = (take_time / 24) * 100 + (take_time % 24);//take_time�� 4�ڸ� ���� ��ȯ
				}
				else {
					if (day == 31) day = 0;
					take_time = timeTable[curr][next][day + 1] + (int)ceil(getDistance(curr, next) / 500);//������ + �̵� �ð�
					take_time = 100 + (take_time / 24) * 100 + (take_time % 24);//take_time�� 4�ڸ� ���� ��ȯ
				}

				int next_time = (time / 100) * 100 + take_time;
				if (dist[next] > next_time) {
					dist[next] = next_time;
					prev[next] = curr;
					flightHour[next] = getDistance(curr, next) / 500;
					travel tmp;
					tmp.cityNum = next;
					tmp.arr_time = next_time;
					PEnqueue(&pq, tmp);
				}
			}
		}
	}
}

void printInfo(reservation info) {//���� ���
	printf("�����ȣ: %d\n�����: %c\n������: %c\n�̸�: %s\n��� �ð�: %d�� %d��\n���� �ð�: %d�� %d��\n",
		info.SID, info.start, info.end, info.name, ((info.startTime / 100) - 1) % 31 + 1, info.startTime % 100, ((info.endTime / 100) - 1) % 31 + 1, info.endTime % 100);
	
	printf("��Ʈ: %c", info.start);
	for (int i = info.boardingCnt - 1; i >= 0; i--) {
		printf("->%c", info.route[i]);
	}
	printf("\n");

	printf("���->���� �ð�: ");
	for (int i = info.boardingCnt - 1; i >= 0; i--) {
		int dep_time = info.arrivalTime[i] - (int)ceil(info.routeFlight[i]);
		int dep_hour = (info.arrivalTime[i] % 100) - (int)ceil(info.routeFlight[i]);
		if (dep_hour < 0)
			dep_time -= 76;
		printf("(%d�� %d��->%d�� %d��)", (dep_time / 100 - 1) % 31 + 1, dep_time % 100, (info.arrivalTime[i] / 100 - 1) % 31 + 1, info.arrivalTime[i] % 100);
	}
	printf("\n");

	printf("���� �ð�: ");
	int price = 0;
	double multiple;
	switch (info.grade) {
	case 'P':
		multiple = 2;
		break;
	case 'B':
		multiple = 1.5;
		break;
	case 'E':
		multiple = 1;
	}

	for (int i = info.boardingCnt - 1; i >= 0; i--) {
		int integer = info.routeFlight[i] / 1;
		price += info.routeFlight[i] * 10000 * multiple;
		printf("%d�ð� %d��/", integer, (int)floor((info.routeFlight[i] - integer) * 60));
	}
	printf("\n");
	printf("���: %c\n", info.grade);
	printf("����: %d��\n", price);
}

int main() {
	srand(time(NULL));

	RBT_Node* rbtroot;//�ʱ�ȭ
	RBT_Init(&rbtroot);

	for (int i = 0; i < 26; i++) {//���� ������ ���� city�� ��ǥ ����
		int x = rand() % 6001;
		int y = rand() % 6001;

		city[i].x = x;
		city[i].y = y;
	}

	//���� ��Ʈ ����
	for (int i = 0; i < 100; i++) {
		int n, m;
		do {
			n = rand() % 26;
			m = rand() % 26;
		} while (m == n || graph[n][m]);//graph[n][m]�� ���� ���� �ʰ� m,n ���� �ߺ����� ���� �� ��

		//�ֹ���
		graph[n][m] = 1;
		graph[m][n] = 1;
	}


	//Ÿ�� ���̺� ���� �����
	for (int i = 0; i < 26; i++) {
		for (int j = 0; j < 26; j++) {
			if (graph[i][j] != 0) {
				for (int k = 1; k < 32; k++)
					timeTable[i][j][k] = rand() % 24;
			}
		}
	}


	//500�� ���� ����
	for (int t = 1; t <= 500; t++) {
		//printf("%d\n", t);
		int s;
		int e;
		int date;
		do {
			s = rand() % 26;
			e = rand() % 26;
		} while (s == e || graph[s][e]);
		date = rand() % 31 + 1;//��� �ð� ���� ����
		int grade = rand() % 3 + 1;//���� ��� ����

		reservation res;

		//�����ȣ ���� ����
		do {
			res.SID = rand() % 10000;
		} while (isUsed[res.SID]);
		isUsed[res.SID] = 1;
		res.start = s + 'A';//���� ����
		res.end = e + 'A';//���� ����
		//���
		if (grade == 0)
			res.grade = 'P';
		else if (grade == 1)
			res.grade = 'B';
		else
			res.grade = 'E';
		for (int i = 0; i < 5; i++) {//���� 5���� �̸� ����
			res.name[i] = 'A' + (rand() % 26);
		}
		res.name[5] = '\0';
	
		int dist[26];//�Ÿ� ����
		int prev[26];//���� �̵� ������ ���� �迭
		double flightHour[26];
		for (int i = 0; i < 26; i++) {//���ͽ�Ʈ�� ���� ���� �ʱ�ȭ
			dist[i] = INF;
			prev[i] = -1;
		}

		Dijkstra(s, date, 0, dist, prev, flightHour);//���ͽ�Ʈ�� �˰��� ���
		
		if (dist[e] == INF) {//���ϴ� ��ġ�� ���� ���� ��� �ٽ� ���� 
			t--;
			continue;
		}

		//���ͽ�Ʈ�� ���� ���� ���� ����
		res.endTime = dist[e];
		int cnt = 0;
		for (int now = e; prev[now] != -1; now = prev[now]) {
			res.route[cnt] = now + 'A';
			res.routeFlight[cnt] = flightHour[now];
			res.arrivalTime[cnt] = dist[now];
			cnt++;
		}
		res.boardingCnt = cnt;
		res.startTime = res.arrivalTime[cnt - 1] - (int)ceil(res.routeFlight[cnt - 1]);
		if ((res.arrivalTime[cnt - 1] % 100) - (int)ceil(res.routeFlight[cnt - 1]) < 0)
			res.startTime -= 76;

		//printInfo(res);//���� ���
		//RB Tree�� ����
		RB_Insert(&rbtroot, res);
	}

	printf("Height: %d, Node #: %d\n", Get_Height(rbtroot), Get_Node_Num(rbtroot));


	//���� UI ����
	int isFinished = 0;//4������ ���������� ���� ������ ���� ����
	while(!isFinished){
		int menuNum;

		puts("\n=========================   ����� ���� �ý���   =========================");
		printf("�޴��� �����Ͻÿ�(1: ����, 2: ���� ��ȸ, 3: ���� ���, 4: ������): ");
		scanf("%d", &menuNum);


		switch (menuNum) {
		case 1://����� ����
		{
			puts("\n=============================   ����� ����    =============================");
			char name[30];
			char startCity;
			char endCity;
			int date;
			int hour;
			char gd;

			//���� �Է�
			printf("�̸�: "); scanf("%s", name);
			getchar();

			printf("�����(A-Z): "); scanf("%c", &startCity);
			getchar();
			while (startCity < 'A' || startCity > 'Z') {
				puts("�߸��� ���� �̸�, �ٽ� �Է��ϼ���.");
				printf("�����(A-Z): "); scanf("%c", &startCity);
				getchar();
			}

			printf("������(A-Z): "); scanf("%c", &endCity);
			getchar();
			while (endCity < 'A' || endCity > 'Z') {
				puts("�߸��� ���� �̸�, �ٽ� �Է��ϼ���.");
				printf("������(A-Z): "); scanf("%c", &endCity);
				getchar();
			}

			printf("��� ��¥(1-31): "); scanf("%d", &date);
			getchar();
			while (date < 1 || date > 31) {
				puts("�߸��� �Է�, �ٽ� �Է��ϼ���.");
				printf("��� ��¥(1-31): "); scanf("%d", &date);
				getchar();
			}
			printf("��� �ð�(0-23): "); scanf("%d", &hour);
			getchar();
			while (hour < 0 || hour > 23) {
				puts("�߸��� �Է�, �ٽ� �Է��ϼ���.");
				printf("��� �ð�(0-23): "); scanf("%d", &hour);
				getchar();
			}
			printf("���(����: P(2��)/B(1.5��)/E(1��)): "); scanf("%c", &gd);
			getchar();
			while (gd != 'P' && gd != 'B' && gd != 'E') {
				puts("�߸��� �Է�, �ٽ� �Է��ϼ���.");
				printf("���(����: P(2��)/B(1.5��)/E(1��)): "); scanf("%c", &gd);
				getchar();
			}

			puts("==========================================================================");

			reservation res;
			do {
				res.SID = rand() % 10000;//�����ȣ
			} while (isUsed[res.SID]);
			isUsed[res.SID] = 1;

			res.start = startCity;//���� ����
			res.end = endCity;//���� ����
			strcpy(res.name, name);
			res.grade = gd;

			int dist[26];
			int prev[26];
			double flightHour[26];
			for (int i = 0; i < 26; i++) {//�ʱ�ȭ
				dist[i] = INF;
				prev[i] = -1;
			}

			Dijkstra(startCity-'A', date, hour, dist, prev, flightHour);//���ͽ�Ʈ�� �˰��� ����

			if (dist[endCity-'A'] == INF) {//���ϴ� ��ġ�� ���� ���� ��� �ٽ� ���� 
				puts("�� �� �ִ� ��Ʈ�� �����ϴ�!");
				continue;
			}

			//���ͽ�Ʈ�� ���� ���� ���� ����
			res.endTime = dist[endCity - 'A'];
			int cnt = 0;
			for (int now = endCity - 'A'; prev[now] != -1; now = prev[now]) {
				res.route[cnt] = now + 'A';
				res.routeFlight[cnt] = flightHour[now];
				res.arrivalTime[cnt] = dist[now];
				cnt++;
			}
			res.boardingCnt = cnt;

			res.startTime = res.arrivalTime[cnt - 1] - (int)ceil(res.routeFlight[cnt - 1]);
			if ((res.arrivalTime[cnt - 1] % 100) - (int)ceil(res.routeFlight[cnt - 1]) < 0)
				res.startTime -= 76;


			printInfo(res);//���� ���

			//RB Tree�� ����
			RB_Insert(&rbtroot, res);
			puts("\n==============================   ���� �Ϸ�    ==============================");
			printf("Height: %d, Node #: %d\n", Get_Height(rbtroot), Get_Node_Num(rbtroot));
			break;
		}
		case 2://���� ��ȸ
		{
			puts("\n==============================   ���� ��ȸ    ==============================");
			int id;
			printf("�����ȣ(SID): "); scanf("%d", &id);
			getchar();

			RBT_Node* find = Tree_Search(rbtroot, id);
			reservation info = find->key;

			if (info.SID == id)
				printInfo(info);
			else
				puts("��ġ�ϴ� �����ȣ�� �����ϴ�!");
			break;
		}
		case 3://���� ����
		{
			puts("\n==============================   ���� ����    ==============================");
			int id;
			char yn;
			printf("�����ȣ(SID): "); scanf("%d", &id);
			getchar();

			RBT_Node* find = Tree_Search(rbtroot, id);
			reservation info = find->key;

			if (info.SID != id) {
				puts("��ġ�ϴ� �����ȣ�� �����ϴ�!");
				continue;
			}

			printInfo(info);
			printf("������ �����Ͻðڽ��ϱ�?(Y/N): ");
			scanf("%c", &yn);
			getchar();
			while (!(yn == 'Y' || yn == 'y' || yn == 'N' || yn == 'n')) {
				printf("�߸��� ����, �ٽ� �Է��ϼ���.(Y/N): ");
				scanf("%c", &yn);
				getchar();
			}
			if (yn == 'Y' || yn == 'y') {
				RB_Delete(&rbtroot, id);
				isUsed[id] = 0;

				puts("\n==============================   ���� �Ϸ�    ==============================");
				//print2D(rbtroot);
			}
			else
				puts("\n===============================     ���     ===============================");
			printf("Height: %d, Node #: %d\n", Get_Height(rbtroot), Get_Node_Num(rbtroot));
			break;
		}
		case 4:
			isFinished = 1;
			break;
		default:
			puts("�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է��ϼ���.");
		}
	}
	return 0;
}