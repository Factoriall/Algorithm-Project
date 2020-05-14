#ifndef  __RBT_H__
#define __RBT_H__
#include <stdio.h>
#include <stdlib.h>
#include "city_reservation.h"

typedef reservation KEY;

typedef struct _rTree {
	KEY key;
	int isBlack;
	struct _rTree* left;
	struct _rTree* right;
	struct _rTree* parent;
}RBT_Node;

void RBT_Init(RBT_Node** pRoot);//RBT 초기화

void Left_Rotate(RBT_Node** pRoot, RBT_Node* x);//왼쪽 회전
void Right_Rotate(RBT_Node** pRoot, RBT_Node* x);//오른쪽 회전

void RB_Insert_Fixup(RBT_Node** pRoot, RBT_Node* z);//RBT 조건에 맞게 RBT 회전 및 색깔 조정
void RB_Insert(RBT_Node** pRoot, KEY x);//RBT에서 노드 넣기

RBT_Node* Tree_Search(RBT_Node* now, int key);//key 값과 일치하는 노드 찾기
RBT_Node* Minimum(RBT_Node* target);//가장 작은 노드 찾기, Successor 함수에서 필요한 함수
RBT_Node* Successor(RBT_Node* target);//target 다음으로 큰 노드 찾기, RB_Delete에서 필요한 함수

void RB_Delete_Fixup(RBT_Node** pRoot, RBT_Node* x);//삭제 후 조건에 맞게 RBT 회전 및 색깔 조정
void RB_Delete(RBT_Node** pRoot, int key);//Tree_Search를 통해 삭제할 노드 찾기

int Get_Height(RBT_Node* now);
int Get_Node_Num(RBT_Node* now);

/* 반시계 방향 90도 회전해서 나오는 tree 그리기 */
void print2DUtil(RBT_Node* root, int space);
void print2D(RBT_Node* root);


#endif // ! __RBT_H__