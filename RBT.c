#include "RBT.h"


void RBT_Init(RBT_Node** pRoot) {//RBT �ʱ�ȭ
	*pRoot = NULL;
}

void Left_Rotate(RBT_Node** pRoot, RBT_Node* center) {//���� ȸ��, rightChild�� ���� �ø��� center�� �Ʒ��� ������.
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

void Right_Rotate(RBT_Node** pRoot, RBT_Node* center) {//������ ȸ��, leftChild�� ���� �ø��� center�� ������ �ڽ����� �ٲ۴�
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

void RB_Insert_Fixup(RBT_Node** pRoot, RBT_Node* newNode) {//RBT ���ǿ� �°� RBT ȸ�� �� ���� ����
	RBT_Node* y;//y: newNode�� �θ��� �θ��� �ݴ��� �ڽ� ���
	while (newNode->parent != NULL && newNode->parent->isBlack == 0) {
		if (newNode->parent == newNode->parent->parent->left) {
			y = newNode->parent->parent->right;
			if (y != NULL && y->isBlack == 0) {//�������� ��, CASE 1
				newNode->parent->isBlack = 1;
				y->isBlack = 1;
				newNode->parent->parent->isBlack = 0;
				newNode = newNode->parent->parent;
			}
			else {//�������� ��
				if (newNode == newNode->parent->right) {//CASE 2, �ٷ� CASE 3�� �Ѿ
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
			if (y != NULL && y->isBlack == 0) {//�������� ��, CASE 1 
				newNode->parent->isBlack = 1;
				y->isBlack = 1;
				newNode->parent->parent->isBlack = 0;
				newNode = newNode->parent->parent;
			}
			else {//�������� ��
				if (newNode == newNode->parent->left) {//CASE 2, �ٷ� CASE 3�� �Ѿ
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
	(*pRoot)->isBlack = 1;//������ ��Ʈ��� ���������� ����
}


void RB_Insert(RBT_Node** pRoot, KEY insertKey) {//RBT���� ��� �ֱ�
	RBT_Node* pNode = NULL;
	RBT_Node* cNode = *pRoot;
	RBT_Node* newNode = NULL;

	while (cNode != NULL) {
		if (insertKey.SID == cNode->key.SID)//���� Ű�� ������ ������ �ȵ�
			return;

		//ũ�� ���̿� ���� ����, ������ �ű�鼭 cNode, pNode ����
		pNode = cNode;
		if (insertKey.SID < cNode->key.SID)
			cNode = cNode->left;
		else
			cNode = cNode->right;
	}

	//�� ��� ����
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

RBT_Node* Tree_Search(RBT_Node* now, int key) {//key ���� ��ġ�ϴ� ��� ã��
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

RBT_Node* Minimum(RBT_Node* target) {//���� ���� ��� ã��, Successor �Լ����� �ʿ��� �Լ�
	while (target->left != NULL) {
		target = target->left;
	}
	return target;
}

RBT_Node* Successor(RBT_Node* target) {//target �������� ū ��� ã��, RB_Delete���� �ʿ��� �Լ�
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

void RB_Delete_Fixup(RBT_Node** pRoot, RBT_Node* replaceNode) {//���� �� ���ǿ� �°� RBT ȸ�� �� ���� ����
	while (replaceNode != *pRoot && replaceNode->isBlack == 1) {
		RBT_Node* sibling;//��ü ����� �θ��� �ݴ��� �ڽ� ���
		if (replaceNode == replaceNode->parent->left || replaceNode->parent->left == NULL) {
			sibling = replaceNode->parent->right;
			if (sibling != NULL && sibling->isBlack == 0) {//case 1
				sibling->isBlack = 1;
				replaceNode->parent->isBlack = 0;
				Left_Rotate(pRoot, replaceNode->parent);
				sibling = replaceNode->parent->right;
			}
			if ((sibling->left == NULL || sibling->left->isBlack == 1) && (sibling->right == NULL || sibling->right->isBlack == 1)) {//case 2: �ڽ��� �Ѵ� ���� ��
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
			if ((sibling->left == NULL || sibling->left->isBlack == 1) && (sibling->right == NULL || sibling->right->isBlack == 1)) {//case 2: �ڽ��� �Ѵ� ���� ��
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

void RB_Delete(RBT_Node** pRoot, int deleteKey) {//Tree_Search�� ���� ������ ��� ã��
	RBT_Node* target = Tree_Search(*pRoot, deleteKey);

	if (target->key.SID != deleteKey) {//���� �� ã�� ���ҽ� �׳� �ƿ�
		puts("�������� ����!");
		return;
	}

	/* ������� ����
	case 1: �ܸ� ���
	case 2: �ڽ��� �ϳ��� ���
	case 3: �ڽ��� ���� ���
	*/
	RBT_Node* deleteNode;
	if (target->left == NULL || target->right == NULL)
		deleteNode = target;
	else
		deleteNode = Successor(target);

	RBT_Node* replaceNode;
	if (deleteNode->left != NULL)//case 2 �� ���ʿ� ����
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
		if (replaceNode == NULL) {//���� replaceNode null�̸� RB_Delete_Fixup���� sibling���� ����Ű�� ���� ������ ��� ����

			nullNode->parent = deleteNode->parent;
			nullNode->isBlack = 1;
			replaceNode = nullNode;
		}
		RB_Delete_Fixup(pRoot, replaceNode);
	}
	free(nullNode);
	free(deleteNode);
}

int Get_Height(RBT_Node* now) {//key ���� ��ġ�ϴ� ��� ã��
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

/* �ݽð� ���� 90�� ȸ���ؼ� ������ tree �׸��� */
void print2DUtil(RBT_Node* root, int space) {
	// ���� ���̽�
	if (root == NULL)
		return;

	// ���� ������ �Ÿ� 
	space += 5;

	// ������ �ڽ� ���Ǳ�
	print2DUtil(root->right, space);

	// ���� ��� ����Ʈ
	// ī��Ʈ�� ���� ����
	// R�� �� ���� �Ʒ��� �ִ� ��尡 �θ��� ���̰� L�̸� �� �������� ���� �ִ� ��尡 �θ��� �ǹ�
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
	

	// ���� �ڽ� ���Ǳ�
	print2DUtil(root->left, space);
}

//�׸��� ��
void print2D(RBT_Node* root) {
	print2DUtil(root, 0);
	printf("\n=================================================================\n");
}