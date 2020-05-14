#ifndef __CITY_RESERVATION_H__
#define __CITY_RESERVATION_H__

typedef struct {
	int x;
	int y;
}coordinate;//��ǥ ����

typedef struct {
	int SID;//��ȣ, key ��
	char name[30];//��� �̸�
	char start;//��� ����
	char end;//���� ����
	int startTime;//��� �ð�
	int endTime;//���� �ð�

	int boardingCnt;//ž�� Ƚ��
	char route[26];//��Ʈ
	double routeFlight[26];//��Ʈ �� �̵� �ð�
	int arrivalTime[26];//��Ʈ �� ���� �ð�

	char grade;//���
}reservation;

#endif

#pragma once
