#define _CRT_SECURE_NO_WARNINGS
#include "RBT.h"
#include "city_reservation.h"
#include "priority_queue.h"
#include <string.h>
#include <math.h>
#include <time.h>

#define INF 987654321

coordinate city[26];//도시 좌표 정보
int graph[26][26];//그래프가 연결되어있는지 확인하는 정보
int timeTable[26][26][32];//시간 스케쥴 정보
int isUsed[10000];//예약번호가 사용되었는지 확인하는 정보

int DataPriorityComp(HData a, HData b) {//힙에서 작은 값 위로 올리기 위한 함수
	return b.arr_time - a.arr_time;
}

double getDistance(int c1, int c2) {//거리 구하는 함수
	return sqrt((city[c1].x - city[c2].x) * (city[c1].x - city[c2].x) + (city[c1].y - city[c2].y) * (city[c1].y - city[c2].y));
}

void Dijkstra(int s, int date, int hr, int dist[26], int prev[26], double flightHour[26]) {//다익스트라 알고리즘 구현, 최단 도착 시간 기준
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
					take_time = timeTable[curr][next][day] + (int)ceil(getDistance(curr, next) / 500);//스케쥴 + 이동 시간
					take_time = (take_time / 24) * 100 + (take_time % 24);//take_time을 4자리 수로 변환
				}
				else {
					if (day == 31) day = 0;
					take_time = timeTable[curr][next][day + 1] + (int)ceil(getDistance(curr, next) / 500);//스케쥴 + 이동 시간
					take_time = 100 + (take_time / 24) * 100 + (take_time % 24);//take_time을 4자리 수로 변환
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

void printInfo(reservation info) {//정보 출력
	printf("예약번호: %d\n출발지: %c\n도착지: %c\n이름: %s\n출발 시간: %d일 %d시\n도착 시간: %d일 %d시\n",
		info.SID, info.start, info.end, info.name, ((info.startTime / 100) - 1) % 31 + 1, info.startTime % 100, ((info.endTime / 100) - 1) % 31 + 1, info.endTime % 100);
	
	printf("루트: %c", info.start);
	for (int i = info.boardingCnt - 1; i >= 0; i--) {
		printf("->%c", info.route[i]);
	}
	printf("\n");

	printf("출발->도착 시간: ");
	for (int i = info.boardingCnt - 1; i >= 0; i--) {
		int dep_time = info.arrivalTime[i] - (int)ceil(info.routeFlight[i]);
		int dep_hour = (info.arrivalTime[i] % 100) - (int)ceil(info.routeFlight[i]);
		if (dep_hour < 0)
			dep_time -= 76;
		printf("(%d일 %d시->%d일 %d시)", (dep_time / 100 - 1) % 31 + 1, dep_time % 100, (info.arrivalTime[i] / 100 - 1) % 31 + 1, info.arrivalTime[i] % 100);
	}
	printf("\n");

	printf("비행 시간: ");
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
		printf("%d시간 %d분/", integer, (int)floor((info.routeFlight[i] - integer) * 60));
	}
	printf("\n");
	printf("등급: %c\n", info.grade);
	printf("가격: %d원\n", price);
}

int main() {
	srand(time(NULL));

	RBT_Node* rbtroot;//초기화
	RBT_Init(&rbtroot);

	for (int i = 0; i < 26; i++) {//난수 생성을 통해 city의 좌표 생성
		int x = rand() % 6001;
		int y = rand() % 6001;

		city[i].x = x;
		city[i].y = y;
	}

	//도시 루트 연결
	for (int i = 0; i < 100; i++) {
		int n, m;
		do {
			n = rand() % 26;
			m = rand() % 26;
		} while (m == n || graph[n][m]);//graph[n][m]에 값이 들어가지 않고 m,n 값이 중복되지 않을 시 끝

		//쌍방향
		graph[n][m] = 1;
		graph[m][n] = 1;
	}


	//타임 테이블 랜덤 만들기
	for (int i = 0; i < 26; i++) {
		for (int j = 0; j < 26; j++) {
			if (graph[i][j] != 0) {
				for (int k = 1; k < 32; k++)
					timeTable[i][j][k] = rand() % 24;
			}
		}
	}


	//500개 랜덤 투입
	for (int t = 1; t <= 500; t++) {
		//printf("%d\n", t);
		int s;
		int e;
		int date;
		do {
			s = rand() % 26;
			e = rand() % 26;
		} while (s == e || graph[s][e]);
		date = rand() % 31 + 1;//출발 시간 랜덤 투입
		int grade = rand() % 3 + 1;//랜덤 등급 투입

		reservation res;

		//예약번호 랜덤 투입
		do {
			res.SID = rand() % 10000;
		} while (isUsed[res.SID]);
		isUsed[res.SID] = 1;
		res.start = s + 'A';//시작 도시
		res.end = e + 'A';//도착 도시
		//등급
		if (grade == 0)
			res.grade = 'P';
		else if (grade == 1)
			res.grade = 'B';
		else
			res.grade = 'E';
		for (int i = 0; i < 5; i++) {//랜덤 5글자 이름 삽입
			res.name[i] = 'A' + (rand() % 26);
		}
		res.name[5] = '\0';
	
		int dist[26];//거리 정보
		int prev[26];//이전 이동 저장을 위한 배열
		double flightHour[26];
		for (int i = 0; i < 26; i++) {//다익스트라 구현 위한 초기화
			dist[i] = INF;
			prev[i] = -1;
		}

		Dijkstra(s, date, 0, dist, prev, flightHour);//다익스트라 알고리즘 사용
		
		if (dist[e] == INF) {//원하는 위치에 도착 못한 경우 다시 시작 
			t--;
			continue;
		}

		//다익스트라 통해 구한 정보 저장
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

		//printInfo(res);//정보 출력
		//RB Tree에 저장
		RB_Insert(&rbtroot, res);
	}

	printf("Height: %d, Node #: %d\n", Get_Height(rbtroot), Get_Node_Num(rbtroot));


	//실제 UI 구현
	int isFinished = 0;//4번으로 나갈때까지 루프 돌리기 위한 변수
	while(!isFinished){
		int menuNum;

		puts("\n=========================   비행기 예약 시스템   =========================");
		printf("메뉴를 선택하시오(1: 예약, 2: 정보 조회, 3: 예약 취소, 4: 나가기): ");
		scanf("%d", &menuNum);


		switch (menuNum) {
		case 1://비행기 예약
		{
			puts("\n=============================   비행기 예약    =============================");
			char name[30];
			char startCity;
			char endCity;
			int date;
			int hour;
			char gd;

			//정보 입력
			printf("이름: "); scanf("%s", name);
			getchar();

			printf("출발지(A-Z): "); scanf("%c", &startCity);
			getchar();
			while (startCity < 'A' || startCity > 'Z') {
				puts("잘못된 도시 이름, 다시 입력하세요.");
				printf("출발지(A-Z): "); scanf("%c", &startCity);
				getchar();
			}

			printf("도착지(A-Z): "); scanf("%c", &endCity);
			getchar();
			while (endCity < 'A' || endCity > 'Z') {
				puts("잘못된 도시 이름, 다시 입력하세요.");
				printf("도착지(A-Z): "); scanf("%c", &endCity);
				getchar();
			}

			printf("출발 날짜(1-31): "); scanf("%d", &date);
			getchar();
			while (date < 1 || date > 31) {
				puts("잘못된 입력, 다시 입력하세요.");
				printf("출발 날짜(1-31): "); scanf("%d", &date);
				getchar();
			}
			printf("출발 시간(0-23): "); scanf("%d", &hour);
			getchar();
			while (hour < 0 || hour > 23) {
				puts("잘못된 입력, 다시 입력하세요.");
				printf("출발 시간(0-23): "); scanf("%d", &hour);
				getchar();
			}
			printf("등급(가격: P(2배)/B(1.5배)/E(1배)): "); scanf("%c", &gd);
			getchar();
			while (gd != 'P' && gd != 'B' && gd != 'E') {
				puts("잘못된 입력, 다시 입력하세요.");
				printf("등급(가격: P(2배)/B(1.5배)/E(1배)): "); scanf("%c", &gd);
				getchar();
			}

			puts("==========================================================================");

			reservation res;
			do {
				res.SID = rand() % 10000;//예약번호
			} while (isUsed[res.SID]);
			isUsed[res.SID] = 1;

			res.start = startCity;//시작 도시
			res.end = endCity;//도착 도시
			strcpy(res.name, name);
			res.grade = gd;

			int dist[26];
			int prev[26];
			double flightHour[26];
			for (int i = 0; i < 26; i++) {//초기화
				dist[i] = INF;
				prev[i] = -1;
			}

			Dijkstra(startCity-'A', date, hour, dist, prev, flightHour);//다익스트라 알고리즘 구현

			if (dist[endCity-'A'] == INF) {//원하는 위치에 도착 못한 경우 다시 시작 
				puts("갈 수 있는 루트가 없습니다!");
				continue;
			}

			//다익스트라 통해 얻은 정보 저장
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


			printInfo(res);//정보 출력

			//RB Tree에 저장
			RB_Insert(&rbtroot, res);
			puts("\n==============================   예약 완료    ==============================");
			printf("Height: %d, Node #: %d\n", Get_Height(rbtroot), Get_Node_Num(rbtroot));
			break;
		}
		case 2://정보 조회
		{
			puts("\n==============================   정보 조회    ==============================");
			int id;
			printf("예약번호(SID): "); scanf("%d", &id);
			getchar();

			RBT_Node* find = Tree_Search(rbtroot, id);
			reservation info = find->key;

			if (info.SID == id)
				printInfo(info);
			else
				puts("일치하는 예약번호가 없습니다!");
			break;
		}
		case 3://정보 삭제
		{
			puts("\n==============================   정보 삭제    ==============================");
			int id;
			char yn;
			printf("예약번호(SID): "); scanf("%d", &id);
			getchar();

			RBT_Node* find = Tree_Search(rbtroot, id);
			reservation info = find->key;

			if (info.SID != id) {
				puts("일치하는 예약번호가 없습니다!");
				continue;
			}

			printInfo(info);
			printf("정말로 삭제하시겠습니까?(Y/N): ");
			scanf("%c", &yn);
			getchar();
			while (!(yn == 'Y' || yn == 'y' || yn == 'N' || yn == 'n')) {
				printf("잘못된 정보, 다시 입력하세요.(Y/N): ");
				scanf("%c", &yn);
				getchar();
			}
			if (yn == 'Y' || yn == 'y') {
				RB_Delete(&rbtroot, id);
				isUsed[id] = 0;

				puts("\n==============================   삭제 완료    ==============================");
				//print2D(rbtroot);
			}
			else
				puts("\n===============================     취소     ===============================");
			printf("Height: %d, Node #: %d\n", Get_Height(rbtroot), Get_Node_Num(rbtroot));
			break;
		}
		case 4:
			isFinished = 1;
			break;
		default:
			puts("잘못 입력하셨습니다. 다시 입력하세요.");
		}
	}
	return 0;
}