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

void RBT_Init(RBT_Node** pRoot);//RBT �ʱ�ȭ

void Left_Rotate(RBT_Node** pRoot, RBT_Node* x);//���� ȸ��
void Right_Rotate(RBT_Node** pRoot, RBT_Node* x);//������ ȸ��

void RB_Insert_Fixup(RBT_Node** pRoot, RBT_Node* z);//RBT ���ǿ� �°� RBT ȸ�� �� ���� ����
void RB_Insert(RBT_Node** pRoot, KEY x);//RBT���� ��� �ֱ�

RBT_Node* Tree_Search(RBT_Node* now, int key);//key ���� ��ġ�ϴ� ��� ã��
RBT_Node* Minimum(RBT_Node* target);//���� ���� ��� ã��, Successor �Լ����� �ʿ��� �Լ�
RBT_Node* Successor(RBT_Node* target);//target �������� ū ��� ã��, RB_Delete���� �ʿ��� �Լ�

void RB_Delete_Fixup(RBT_Node** pRoot, RBT_Node* x);//���� �� ���ǿ� �°� RBT ȸ�� �� ���� ����
void RB_Delete(RBT_Node** pRoot, int key);//Tree_Search�� ���� ������ ��� ã��

int Get_Height(RBT_Node* now);
int Get_Node_Num(RBT_Node* now);

/* �ݽð� ���� 90�� ȸ���ؼ� ������ tree �׸��� */
void print2DUtil(RBT_Node* root, int space);
void print2D(RBT_Node* root);


#endif // ! __RBT_H__