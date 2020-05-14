#include "RBT.h"


void RBT_Init(RBT_Node** pRoot) {//RBT 초기화
	*pRoot = NULL;
}

void Left_Rotate(RBT_Node** pRoot, RBT_Node* center) {//왼쪽 회전, rightChild를 위로 올리고 center를 아래로 내린다.
	RBT_Node* rightChild = center->right;
	center->right = rightChild->left;
	if (rightChild->left != NULL)
		rightChild->left->parent = center;

	rightChild->parent = center->parent;
	if (center->parent == NULL) {
		*pRoot = rightChild;
	}
	else if (center == center->parent->left) {
		center->parent->left = rightChild;
	}
	else {
		center->parent->right = rightChild;
	}
	rightChild->left = center;
	center->parent = rightChild;
}

void Right_Rotate(RBT_Node** pRoot, RBT_Node* center) {//오른쪽 회전, leftChild를 위로 올리고 center를 오른쪽 자식으로 바꾼다
	RBT_Node* leftChild = center->left;
	center->left = leftChild->right;
	if (leftChild->right != NULL)
		leftChild->right->parent = center;

	leftChild->parent = center->parent;
	if (center->parent == NULL) {
		*pRoot = leftChild;
	}
	else if (center == center->parent->right) {
		center->parent->right = leftChild;
	}
	else {
		center->parent->left = leftChild;
	}
	leftChild->right = center;
	center->parent = leftChild;
}

void RB_Insert_Fixup(RBT_Node** pRoot, RBT_Node* newNode) {//RBT 조건에 맞게 RBT 회전 및 색깔 조정
	RBT_Node* y;//y: newNode의 부모의 부모의 반대쪽 자식 노드
	while (newNode->parent != NULL && newNode->parent->isBlack == 0) {
		if (newNode->parent == newNode->parent->parent->left) {
			y = newNode->parent->parent->right;
			if (y != NULL && y->isBlack == 0) {//빨간색일 시, CASE 1
				newNode->parent->isBlack = 1;
				y->isBlack = 1;
				newNode->parent->parent->isBlack = 0;
				newNode = newNode->parent->parent;
			}
			else {//검은색일 시
				if (newNode == newNode->parent->right) {//CASE 2, 바로 CASE 3로 넘어감
					newNode = newNode->parent;
					Left_Rotate(pRoot, newNode);
				}
				//CASE 3
				newNode->parent->isBlack = 1;
				newNode->parent->parent->isBlack = 0;
				Right_Rotate(pRoot, newNode->parent->parent);
			}
		}
		else {
			y = newNode->parent->parent->left;
			if (y != NULL && y->isBlack == 0) {//빨간색일 시, CASE 1 
				newNode->parent->isBlack = 1;
				y->isBlack = 1;
				newNode->parent->parent->isBlack = 0;
				newNode = newNode->parent->parent;
			}
			else {//검은색일 시
				if (newNode == newNode->parent->left) {//CASE 2, 바로 CASE 3로 넘어감
					newNode = newNode->parent;
					Right_Rotate(pRoot, newNode);
				}
				//CASE 3
				newNode->parent->isBlack = 1;
				newNode->parent->parent->isBlack = 0;
				Left_Rotate(pRoot, newNode->parent->parent);
			}
		}
	}
	(*pRoot)->isBlack = 1;//마지막 루트노드 검정색으로 변경
}


void RB_Insert(RBT_Node** pRoot, KEY insertKey) {//RBT에서 노드 넣기
	RBT_Node* pNode = NULL;
	RBT_Node* cNode = *pRoot;
	RBT_Node* newNode = NULL;

	while (cNode != NULL) {
		if (insertKey.SID == cNode->key.SID)//같은 키가 나오면 삽입이 안됨
			return;

		//크기 차이에 따라 왼쪽, 오른쪽 옮기면서 cNode, pNode 설정
		pNode = cNode;
		if (insertKey.SID < cNode->key.SID)
			cNode = cNode->left;
		else
			cNode = cNode->right;
	}

	//새 노드 생성
	newNode = (RBT_Node*)malloc(sizeof(RBT_Node));
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->key = insertKey;
	newNode->parent = pNode;
	newNode->isBlack = 0;
	if (pNode == NULL) {
		*pRoot = newNode;
	}
	else {
		if (insertKey.SID < pNode->key.SID) {
			pNode->left = newNode;
		}
		else {
			pNode->right = newNode;
		}

	}

	RB_Insert_Fixup(pRoot, newNode);
}

RBT_Node* Tree_Search(RBT_Node* now, int key) {//key 값과 일치하는 노드 찾기
	if (now != NULL) {
		if (now->key.SID == key)
			return now;
		else if (now->key.SID > key) {
			if (now->left == NULL)
				return now;
			else
				return Tree_Search(now->left, key);
		}
		else {
			if (now->right == NULL)
				return now;
			else
				return Tree_Search(now->right, key);
		}
	}
	else {
		return NULL;
	}
}

RBT_Node* Minimum(RBT_Node* target) {//가장 작은 노드 찾기, Successor 함수에서 필요한 함수
	while (target->left != NULL) {
		target = target->left;
	}
	return target;
}

RBT_Node* Successor(RBT_Node* target) {//target 다음으로 큰 노드 찾기, RB_Delete에서 필요한 함수
	if (target->right != NULL)
		return Minimum(target->right);
	else {
		RBT_Node* pNode = target->parent;
		while (pNode != NULL && target == pNode->right) {
			target = pNode;
			pNode = pNode->parent;
		}

		return pNode;
	}
}

void RB_Delete_Fixup(RBT_Node** pRoot, RBT_Node* replaceNode) {//삭제 후 조건에 맞게 RBT 회전 및 색깔 조정
	while (replaceNode != *pRoot && replaceNode->isBlack == 1) {
		RBT_Node* sibling;//교체 노드의 부모의 반대쪽 자식 노드
		if (replaceNode == replaceNode->parent->left || replaceNode->parent->left == NULL) {
			sibling = replaceNode->parent->right;
			if (sibling != NULL && sibling->isBlack == 0) {//case 1
				sibling->isBlack = 1;
				replaceNode->parent->isBlack = 0;
				Left_Rotate(pRoot, replaceNode->parent);
				sibling = replaceNode->parent->right;
			}
			if ((sibling->left == NULL || sibling->left->isBlack == 1) && (sibling->right == NULL || sibling->right->isBlack == 1)) {//case 2: 자식이 둘다 검은 색
				sibling->isBlack = 0;
				replaceNode = replaceNode->parent;
			}
			else {
				if (sibling->right == NULL || sibling->right->isBlack == 1) {//case 3
					sibling->left->isBlack = 1;
					sibling->isBlack = 0;
					Right_Rotate(pRoot, sibling);
					sibling = replaceNode->parent->right;
				}
				sibling->isBlack = replaceNode->parent->isBlack;//case 4
				replaceNode->parent->isBlack = 1;
				sibling->right->isBlack = 1;
				Left_Rotate(pRoot, replaceNode->parent);
				replaceNode = *pRoot;
			}
		}
		else {
			sibling = replaceNode->parent->left;
			if (sibling != NULL && sibling->isBlack == 0) {//case 1
				sibling->isBlack = 1;
				replaceNode->parent->isBlack = 0;
				Right_Rotate(pRoot, replaceNode->parent);
				sibling = replaceNode->parent->left;
			}
			if ((sibling->left == NULL || sibling->left->isBlack == 1) && (sibling->right == NULL || sibling->right->isBlack == 1)) {//case 2: 자식이 둘다 검은 색
				sibling->isBlack = 0;
				replaceNode = replaceNode->parent;
			}
			else {
				if (sibling->left == NULL || sibling->left->isBlack == 1) {//case 3
					sibling->right->isBlack = 1;
					sibling->isBlack = 0;
					Left_Rotate(pRoot, sibling);
					sibling = replaceNode->parent->left;
				}
				sibling->isBlack = replaceNode->parent->isBlack;//case 4
				replaceNode->parent->isBlack = 1;
				sibling->left->isBlack = 1;
				Right_Rotate(pRoot, replaceNode->parent);
				replaceNode = *pRoot;
			}
		}
	}
	replaceNode->isBlack = 1;
}

void RB_Delete(RBT_Node** pRoot, int deleteKey) {//Tree_Search를 통해 삭제할 노드 찾기
	RBT_Node* target = Tree_Search(*pRoot, deleteKey);

	if (target->key.SID != deleteKey) {//같은 것 찾지 못할시 그냥 아웃
		puts("예약정보 없음!");
		return;
	}

	/* 삭제노드 상태
	case 1: 단말 노드
	case 2: 자식이 하나인 노드
	case 3: 자식이 둘인 노드
	*/
	RBT_Node* deleteNode;
	if (target->left == NULL || target->right == NULL)
		deleteNode = target;
	else
		deleteNode = Successor(target);

	RBT_Node* replaceNode;
	if (deleteNode->left != NULL)//case 2 중 왼쪽에 존재
		replaceNode = deleteNode->left;
	else//case 1,3
		replaceNode = deleteNode->right;

	if (replaceNode != NULL)//case 2,3
		replaceNode->parent = deleteNode->parent;

	if (deleteNode->parent == NULL) {
		*pRoot = replaceNode;
	}
	else if (deleteNode == deleteNode->parent->left) {
		deleteNode->parent->left = replaceNode;
	}
	else {
		deleteNode->parent->right = replaceNode;
	}
	if (deleteNode != target) {
		target->key = deleteNode->key;
	}

	RBT_Node* nullNode = (RBT_Node*)malloc(sizeof(RBT_Node));
	if (deleteNode->isBlack == 1) {
		if (replaceNode == NULL) {//만약 replaceNode null이면 RB_Delete_Fixup에서 sibling값을 가리키기 위해 임의의 노드 생성

			nullNode->parent = deleteNode->parent;
			nullNode->isBlack = 1;
			replaceNode = nullNode;
		}
		RB_Delete_Fixup(pRoot, replaceNode);
	}
	free(nullNode);
	free(deleteNode);
}

int Get_Height(RBT_Node* now) {//key 값과 일치하는 노드 찾기
	if (!now)
		return 0;
	else {
		int left = Get_Height(now->left);
		int right = Get_Height(now->right);
		return 1 + (left > right ? left : right);
	}
}

int Get_Node_Num(RBT_Node* now) {
	if (!now)
		return 0;
	else {
		int left = Get_Node_Num(now->left);
		int right = Get_Node_Num(now->right);
		return 1 + left + right;
	}
}

/* 반시계 방향 90도 회전해서 나오는 tree 그리기 */
void print2DUtil(RBT_Node* root, int space) {
	// 기저 케이스
	if (root == NULL)
		return;

	// 레벨 사이의 거리 
	space += 5;

	// 오른쪽 자식 살피기
	print2DUtil(root->right, space);

	// 현재 노드 프린트
	// 카운트로 공백 조절
	// R은 전 레벨 아래에 있는 노드가 부모라는 것이고 L이면 전 레벨에서 위에 있는 노드가 부모라는 의미
	printf("\n");
	for (int i = 5; i < space; i++)
		printf(" ");
	if (root->parent != NULL)
		if (root->isBlack) {
			//printf("(%d/%c->%c/%s/%lf %d/%d~%d) ", root->key.SID, root->key.start, root->key.end, root->key.name, root->key.flightHour, root->key.routeCnt, root->key.startTime, root->key.endTime);
			printf("(%d)", root->key.SID);
		}
		else {
			//printf("%d/%c->%c/%s/%lf %d/%d~%d ", root->key.SID, root->key.start, root->key.end, root->key.name, root->key.flightHour, root->key.routeCnt, root->key.startTime, root->key.endTime);
			printf("%d", root->key.SID);
		}
	else
		printf("(%d)", root->key.SID);
		//printf("(%d/%c->%c/%s/%lf %d/%d~%d) ", root->key.SID, root->key.start, root->key.end, root->key.name, root->key.flightHour, root->key.routeCnt, root->key.startTime, root->key.endTime);

	
	/*
	for (int i = 0; i < root->key.routeCnt; i++) {
		printf("%c->", root->key.route[i]);
	}
	printf("%c ", root->key.end);
	for (int i = 0; i < root->key.routeCnt; i++) {
		printf("%d->%d %lf/ ", root->key.srcInfo[i], root->key.destInfo[i], root->key.flightInfo[i]);
	}
	printf("\n");*/
	

	// 왼쪽 자식 살피기
	print2DUtil(root->left, space);
}

//그리기 툴
void print2D(RBT_Node* root) {
	print2DUtil(root, 0);
	printf("\n=================================================================\n");
}