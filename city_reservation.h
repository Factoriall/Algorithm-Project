#ifndef __CITY_RESERVATION_H__
#define __CITY_RESERVATION_H__

typedef struct {
	int x;
	int y;
}coordinate;//좌표 정보

typedef struct {
	int SID;//번호, key 값
	char name[30];//사람 이름
	char start;//출발 도시
	char end;//도착 도시
	int startTime;//출발 시간
	int endTime;//도착 시간

	int boardingCnt;//탑승 횟수
	char route[26];//루트
	double routeFlight[26];//루트 간 이동 시간
	int arrivalTime[26];//루트 간 도착 시간

	char grade;//등급
}reservation;

#endif

#pragma once
