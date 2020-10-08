#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/jrbtree.h"

////////////////////////////////////////////////////////////////////////////////
/// Predefinition of JNode Static Functions
////////////////////////////////////////////////////////////////////////////////

static JNodePtr JNodeRightRotate(const JNodePtr node);
static JNodePtr JNodeLeftRotate(const JNodePtr node);
static void JNodeDeleteChilds(JNodePtr node);
static JNodePtr JNodeMove(JNodePtr node, void *key, KeyType type);
static void JNodePreorderTraverse(const JNodePtr node, KeyType type);
static void JNodeInorderTraverse(const JNodePtr node, KeyType type);
static void JNodePostorderTraverse(const JNodePtr node, KeyType type);
static void JNodePrintKey(const JNodePtr node, KeyType type);
static JNodePtr JNodeGetUncle(JNodePtr node);
static JNodePtr JNodeGetGrandParent(JNodePtr node);
static JNodePtr JNodeSetParentPtr(JNodePtr node);
static int JNodeGetBlackHeight(const JNodePtr node);
static int JNodeGetBlackHeightDiff(const JNodePtr node);
static int JNodeGetHeight(const JNodePtr node);

////////////////////////////////////////////////////////////////////////////////
/// Predefinition of JRBTree Static Function
////////////////////////////////////////////////////////////////////////////////

static void JRBTreePrintHeight(const JNodePtr node, int height, KeyType type, char position);
static JRBTreePtr JRBTreeRebalance(JRBTreePtr tree);

////////////////////////////////////////////////////////////////////////////////
/// Predefinition of Util Static Functions
////////////////////////////////////////////////////////////////////////////////

static KeyType _CheckKeyType(KeyType type);
static int _GetCompareLength(const char *s1, const char *s2);
static char _NodeColorToChar(NodeColor color);

///////////////////////////////////////////////////////////////////////////////
// Functions for JNode
///////////////////////////////////////////////////////////////////////////////

/**
 * @fn JNodePtr NewJNode()
 * @brief 새로운 노드 구조체 객체를 생성하는 함수
 * @return 성공 시 생성된 노드 구조체 객체의 주소, 실패 시 NULL 반환
 */
JNodePtr NewJNode()
{
	JNodePtr newNode = (JNodePtr)malloc(sizeof(JNode));

	if(newNode == NULL)
	{
		return NULL;
	}

	newNode->color = Black;
	newNode->parent = NULL;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->key = NULL;

	return newNode;
}

/**
 * @fn DeleteResult DeleteJNode(JNodePtrContainer container)
 * @brief 노드 구조체 객체를 삭제하는 함수
 * @param container 노드 구조체 객체의 주소를 저장한 이중 포인터, 컨테이너 변수(입력)
 * @return 성공 시 DeleteSuccess, 실패 시 DeleteFail 반환(DeleteResult 열거형 참고)
 */
DeleteResult DeleteJNode(JNodePtrContainer container)
{
	if(container == NULL || *container == NULL) return DeleteFail;
	free(*container);
	*container = NULL;
	return DeleteSuccess;
}

/**
 * @fn void* JNodeGetKey(const JNodePtr node)
 * @brief 노드에 저장된 키의 주소를 반환하는 함수
 * @param node 노드 구조체 객체의 주소(입력, 읽기 전용)
 * @return 성공 시 저장된 키의 주소, 실패 시 NULL 반환
 */
void* JNodeGetKey(const JNodePtr node)
{
	if(node == NULL) return NULL;
	return node->key;
}

/**
 * @fn void* JNodeSetKey(JNodePtr node, void *key)
 * @brief 노드에 의 주소를 저장하는 함수
 * @param node 노드 구조체 객체의 주소(출력)
 * @param key 저장할 키의 주소(입력)
 * @return 성공 시 저장된 키의 주소, 실패 시 NULL 반환
 */
void* JNodeSetKey(JNodePtr node, void *key)
{
	if(node == NULL || key == NULL) return NULL;
	node->key = key;
	return node->key;
}

///////////////////////////////////////////////////////////////////////////////
// Functions for JRBTree
///////////////////////////////////////////////////////////////////////////////

/**
 * @fn JRBTreePtr NewJRBTree(KeyType type)
 * @brief 새로운 RB Tree 구조체 객체를 생성하는 함수
 * @param type 저장할 키 데이터 유형(입력) 
 * @return 성공 시 생성된 RB Tree 구조체 객체의 주소, 실패 시 NULL 반환
 */
JRBTreePtr NewJRBTree(KeyType type)
{
	if(_CheckKeyType(type) == Unknown) return NULL;

	JRBTreePtr newTree = (JRBTreePtr)malloc(sizeof(JRBTree));
	if(newTree == NULL)
	{
		return NULL;
	}

	newTree->type = type;
	newTree->root = NULL;
	newTree->data = NULL;

	return newTree;
}

/**
 * @fn DeleteResult DeleteJRBTree(JRBTreePtrContainer container)
 * @brief RB Tree 구조체 객체를 삭제하는 함수
 * @param container RB Tree 구조체 객체의 주소를 저장한 이중 포인터, 컨테이너 변수(입력)
 * @return 성공 시 DeleteSuccess, 실패 시 DeleteFail 반환(DeleteResult 열거형 참고)
 */
DeleteResult DeleteJRBTree(JRBTreePtrContainer container)
{
	if(container == NULL || *container == NULL) return DeleteFail;

	JNodePtr rootNode = (*container)->root;
	if(rootNode != NULL)
	{
		JNodeDeleteChilds(rootNode);
		free(rootNode);
	}

	free(*container);
	*container = NULL;

	return DeleteSuccess;
}

/**
 * @fn void* JRBTreeGetData(const JRBTreePtr tree)
 * @brief RB Tree에 저장된 데이터의 주소를 반환하는 함수
 * @param tree RB Tree 구조체 객체의 주소(입력, 읽기 전용)
 * @return 성공 시 저장된 데이터의 주소, 실패 시 NULL 반환
 */
void* JRBTreeGetData(const JRBTreePtr tree)
{
	if(tree == NULL) return NULL;
	return tree->data;
}

/**
 * @fn void* JRBTreeSetData(const JRBTreePtr tree, void *data)
 * @brief RB Tree에 데이터의 주소를 저장하는 함수
 * @param tree RB Tree 구조체 객체의 주소(출력)
 * @param key 저장할 데이터의 주소(입력)
 * @return 성공 시 저장된 데이터의 주소, 실패 시 NULL 반환
 */
void* JRBTreeSetData(JRBTreePtr tree, void *data)
{
	if(tree == NULL || data == NULL) return NULL;
	tree->data = data;
	return tree->data;
}

/**
 * @fn JRBTreePtr JRBTreeInsertNode(JRBTreePtr tree, void *key)
 * @brief RB Tree에 새로운 노드를 추가하는 함수
 * 중복 허용하지 않음
 * @param tree RB Tree 구조체 객체의 주소(출력)
 * @param key 저장할 노드의 키 주소(입력)
 * @return 성공 시 RB Tree 구조체의 주소, 실패 시 NULL 반환
 */
JRBTreePtr JRBTreeInsertNode(JRBTreePtr tree, void *key)
{
	if((tree == NULL || key == NULL)) return NULL;

	JNodePtr parentNode = NULL;
	JNodePtr currentNode = tree->root;

	// 추가할 노드의 위치를 검색
	while(currentNode != NULL)
	{
		if(key == currentNode->key) return NULL;

		parentNode = currentNode;
		currentNode = JNodeMove(currentNode, key, tree->type);
	}

	// 지정한 키를 저장할 새로운 노드 생성
	JNodePtr newNode = NewJNode();
	if(JNodeSetKey(newNode, key) == NULL) return NULL;

	// 첫 노드가 아니면 자식 노드 추가
	if(parentNode != NULL)
	{
		switch(tree->type)
		{
			case IntType:
				if(*((int*)(parentNode->key)) > *((int*)(key))) parentNode->left = newNode;
				else parentNode->right = newNode;
				break;
			case CharType:
				if(*((char*)(parentNode->key)) > *((char*)(key))) parentNode->left = newNode;
				else parentNode->right = newNode;
				break;
			case StringType:
				if(strncmp((char*)(parentNode->key), (char*)(key), _GetCompareLength((char*)(parentNode->key), (char*)(key))) < 0) parentNode->left = newNode;
				else parentNode->right = newNode;
				break;
			default:
				DeleteJNode(&newNode);
				return NULL;
		}

		newNode->color = Red;
		newNode->parent = parentNode;

		JNodePtr grandParentNode = JNodeGetGrandParent(newNode);
		if(grandParentNode != NULL)
		{
			JNodePtr parentNode = newNode->parent;
			JNodePtr uncleNode = JNodeGetUncle(newNode);

			// case 1) 부모와 삼촌 노드가 모두 R 인 경우
			if((parentNode->color == Red) && (uncleNode != NULL) && (uncleNode->color == Red))
			{
				parentNode->color = Black;
				uncleNode->color = Black;
				if(grandParentNode != tree->root) grandParentNode->color = Red;
			}
			// case 2) 부모 노드는 R 이고 삼촌 노드가 없거나 B 인 경우
			else if((parentNode->color == Red) && ((uncleNode == NULL) || (uncleNode != NULL) && (uncleNode->color == Black)))
			{
				JNodePtr _newNode = newNode;

				// case 2-1) 추가할 자식 노드가 부모 노드의 오른쪽인 경우
				if(_newNode == parentNode->right)
				{
					// case 2-1-1)
					// 부모 노드가 조부모 노드의 오른쪽인 경우
					// 조부모 노드를 기준으로 왼쪽으로 회전한다.
					if((grandParentNode->right == parentNode))
					{
						parentNode->color = Black;
						grandParentNode->color = Red;
						grandParentNode = JNodeLeftRotate(grandParentNode);
						tree->root = JNodeSetParentPtr(grandParentNode);
					}
					// case 2-1-2)
					// 부모 노드가 조부모 노드의 왼쪽인 경우
					// 부모 노드를 기준으로 왼쪽으로 회전한다. -> case 2-2-2)
					else if((grandParentNode->left == parentNode))
					{
						_newNode = parentNode;
						parentNode = JNodeLeftRotate(parentNode);
					}
				}

				// case 2-2) 추가할 자식 노드가 부모 노드의 왼쪽인 경우
				// 자식 노드와 부모 노드를 검은색으로 변경하고, (루트 노드가 아닌)조부모 노드를 빨간색으로 변경하고 조부모 노드를 기준으로 방향에 맞게 회전한다.
				if(_newNode == parentNode->left)
				{
					// 노드 색 변경
					_newNode->color = Black;
					parentNode->color = Black;
					if(grandParentNode != tree->root) grandParentNode->color = Red;

					// case 2-2-1)
					// 부모 노드가 조부모 노드의 오른쪽 자식인 경우
					// 부모 노드를 기준으로 오른쪽 회전 후, 조부모 노드를 기준으로 왼쪽 회전
					if(grandParentNode->right == parentNode)
					{
						parentNode = JNodeRightRotate(parentNode);
						grandParentNode = JNodeLeftRotate(grandParentNode);
					}
					// case 2-2-2)
					// 부모 노드가 조부모 노드의 왼쪽 자식인 경우
					// 조부모 노드를 기준으로 오른쪽 회전
					else
					{
						grandParentNode = JNodeRightRotate(grandParentNode);
					}
					tree->root = JNodeSetParentPtr(grandParentNode);
				}
			}
		}
	}
	// 첫 노드이면 루트 노드에 추가
	else tree->root = newNode;

	JRBTreeRebalance(tree);

	JRBTreePrintAll(tree);
	printf("---------\n");

	return tree;
}

/**
 * @fn DeleteResult JRBTreeDeleteNodeByKey(JRBTreePtr tree, void *key)
 * @brief RB Tree에 지정한 키를 가진 노드를 삭제하는 함수
 * @param tree RB Tree 구조체 객체의 주소(츨력)
 * @param key 삭제할 키의 주소(입력)
 * @return 성공 시 DeleteSuccess, 실패 시 DeleteFail 반환(DeleteResult 열거형 참고)
 */
DeleteResult JRBTreeDeleteNodeByKey(JRBTreePtr tree, void *key)
{
	if(tree == NULL || key == NULL) return DeleteFail;

	DeleteResult result = DeleteSuccess;

	JNodePtr dummyNode = NewJNode();
	if(dummyNode == NULL) return DeleteFail;
	dummyNode->right = tree->root;

	JNodePtr parentNode = dummyNode;
	JNodePtr currentNode = tree->root;
	JNodePtr selectedNode = NULL;

	while((currentNode != NULL) && (key != currentNode->key))
	{
		parentNode = currentNode;
		currentNode = JNodeMove(currentNode, key, tree->type);
	}

	if(currentNode == NULL)
	{
		DeleteJNode(&dummyNode);
		return DeleteFail;
	}
	selectedNode = currentNode;

	// 자식 노드가 없는 경우(최하위 노드)
	if((selectedNode->left == NULL) && (selectedNode->right == NULL))
	{
		if(parentNode->left == currentNode) parentNode->left = NULL;
		else parentNode->right = NULL;
	}
	// 자식 노드가 하나밖에 없는 경우
	else if((selectedNode->left == NULL) || (selectedNode->right == NULL))
	{
		// child node of selected node
		JNodePtr scNode = NULL;

		if(selectedNode->left != NULL) scNode = selectedNode->left;
		else scNode = selectedNode->right;

		if(parentNode->left == selectedNode) parentNode->left = scNode;
		else parentNode->right = scNode;

		scNode->parent = parentNode;

		// Black 노드를 삭제할 시 Double Red 문제 발생
		if(selectedNode->color == Black)
		{
			if(scNode->color != Black) scNode->color = Black;
		}

		// 이중 Black 노드 문제 발생
		JNodePtr parentNode = scNode->parent;
		JNodePtr uncleNode = JNodeGetUncle(scNode);
		// 삼촌 노드가 Red 인 경우
		if((parentNode->color == Black) && (uncleNode->color == Red))
		{
			parentNode->color = Red;
			uncleNode->color = Black;
			parentNode = JNodeLeftRotate(parentNode);
		}
		// 삼촌 노드가 Black 인 경우
		else if((parentNode->color == Black) && (uncleNode->color == Black))
		{
			uncleNode->color = Red;
			if(parentNode->right != NULL)
			{
				if(parentNode->left == scNode) parentNode->right->color = Red;
				else parentNode->left->color = Red;
			}
		}
	}
	// 자식 노드가 두 개 다 있는 경우
	else if((selectedNode->left != NULL) && (selectedNode->right != NULL))
	{
		// child node of selected node
		JNodePtr scNode = selectedNode->right;
		// parent node of child node of selected node
		JNodePtr spNode = selectedNode;

		while(scNode->left != NULL)
		{
			spNode = scNode;
			scNode = scNode->left;
		}

		void *tempKey = scNode->key;

		if(spNode->left == scNode) spNode->left = scNode->right;
		else spNode->right = scNode->right;

		JNodeSetKey(selectedNode, tempKey);
		selectedNode = scNode;
	}

	// 루트 노드 삭제 시 변경된 다른 노드로 바꾼다.
	if(dummyNode->right != tree->root) tree->root = dummyNode->right;


	DeleteJNode(&selectedNode);
	DeleteJNode(&dummyNode);

	return result;
}

/**
 * @fn void JRBTreePreorderTraverse(const JRBTreePtr tree)
 * @brief RB Tree 를 전위 순회하며 노드의 키를 출력하는 함수
 * @param tree 순회할 RB Tree (입력, 읽기 전용)
 * @return 반환값 없음
 */
void JRBTreePreorderTraverse(const JRBTreePtr tree)
{
	if(tree == NULL) return;
	JNodePreorderTraverse(tree->root, tree->type);
	printf("\n");
}

/**
 * @fn void JRBTreeInorderTraverse(const JRBTreePtr tree)
 * @brief RB Tree 를 중위 순회하며 노드의 키를 출력하는 함수
 * @param tree 순회할 RB Tree (입력, 읽기 전용)
 * @return 반환값 없음
 */
void JRBTreeInorderTraverse(const JRBTreePtr tree)
{
	if(tree == NULL) return;
	JNodeInorderTraverse(tree->root, tree->type);
	printf("\n");
}

/**
 * @fn void JRBTreePostorderTraverse(const JRBTreePtr tree)
 * @brief RB Tree 를 후위 순회하며 노드의 키를 출력하는 함수
 * @param tree 순회할 RB Tree (입력, 읽기 전용)
 * @return 반환값 없음
 */
void JRBTreePostorderTraverse(const JRBTreePtr tree)
{
	if(tree == NULL) return;
	JNodePostorderTraverse(tree->root, tree->type);
	printf("\n");
}

void JRBTreePrintAll(const JRBTreePtr tree)
{
	JRBTreePrintHeight(tree->root, 1, tree->type, 'B');
}

////////////////////////////////////////////////////////////////////////////////
/// JNode Static Functions
////////////////////////////////////////////////////////////////////////////////

/**
 * @fn static JNodePtr JNodeRightRotate(JNodePtr node)
 * @brief 지정한 노드를 기준으로 오른쪽으로 회전하는 함수
 * @param node 회전하기 위한 기준 노드(입력, 읽기 전용) 
 * @return 성공 시 회전된 기준 노드, 실패 시 NULL 반환
 */
static JNodePtr JNodeRightRotate(const JNodePtr node)
{
	if(node == NULL) return NULL;

	JNodePtr grandParentNode = node->parent;
	JNodePtr parentNode = node;
	JNodePtr currentNode = parentNode->left;

	if(grandParentNode != NULL)
	{
		if(grandParentNode->right == parentNode) grandParentNode->right = currentNode;
		else if(grandParentNode->left == parentNode) grandParentNode->left = currentNode;
	}

	if(currentNode != NULL)
	{
		parentNode->left = currentNode->right;
		currentNode->right = parentNode;
		currentNode->parent = grandParentNode;
		parentNode->parent = currentNode;
		return currentNode;
	}

	return node;
}

/**
 * @fn static JNodePtr JNodeLeftRotate(const JNodePtr node)
 * @brief 지정한 노드를 기준으로 왼쪽으로 회전하는 함수
 * @param node 회전하기 위한 기준 노드(입력, 읽기 전용) 
 * @return 성공 시 회전된 기준 노드, 실패 시 NULL 반환
 */
static JNodePtr JNodeLeftRotate(const JNodePtr node)
{
	if(node == NULL) return NULL;

	JNodePtr grandParentNode = node->parent;
	JNodePtr parentNode = node;
	JNodePtr currentNode = parentNode->right;

	if(grandParentNode != NULL)
	{
		if(grandParentNode->right == parentNode) grandParentNode->right = currentNode;
		else grandParentNode->left = currentNode;
	}

	if(currentNode != NULL)
	{
		parentNode->right = currentNode->left;
		currentNode->left = parentNode;
		currentNode->parent = grandParentNode;
		parentNode->parent = currentNode;
		return currentNode;
	}
	
	return node;
}

/**
 * @fn static void JNodeDeleteChilds(JNodePtr node)
 * @brief RB Tree 에 저장된 노드들을 모두 삭제하는 함수(재귀)
 * @param node 자식 노드들을 삭제하기 위한 기준 노드(입력)
 * @return 반환값 없음
 */
static void JNodeDeleteChilds(JNodePtr node)
{
	if(node == NULL) return;
	
	if(node->left != NULL)
	{
		JNodeDeleteChilds(node->left);
		free(node->left);
		node->left = NULL;
	}

	if(node->right != NULL)
	{
		JNodeDeleteChilds(node->right);
		free(node->right);
		node->right = NULL;
	}
}

/**
 * @fn static JNodePtr JNodeMove(JNodePtr node, void *key, KeyType type)
 * @brief 지정한 노드의 키와 전달받은 키를 비교해서 저장할 노드의 위치를 찾아 그 노드의 주소를 반환하는 함수 
 * @param node 위치를 움직일 노드의 주소(입력)
 * @param key 전달받은 키(입력)
 * @param type 키의 데이터 유형(입력)
 * @return 성공 시 위치가 변경된 노드의 주소, 실패 시 NULL 반환
 */
static JNodePtr JNodeMove(JNodePtr node, void *key, KeyType type)
{
	switch(type)
	{
		case IntType:
			if(*((int*)(node->key)) > *((int*)(key))) node = node->left;
			else node = node->right;
			break;
		case CharType:
			if(*((char*)(node->key)) > *((char*)(key))) node = node->left;
			else node = node->right;
			break;
		case StringType:
		{
			if(strncmp((char*)(node->key), (char*)(key), _GetCompareLength((char*)(node->key), (char*)(key))) < 0) node = node->left;
			else node = node->right;
			break;
		}
		default: return NULL;
	}
	return node;
}

/**
 * @fn static void JNodePreorderTraverse(const JNodePtr node, KeyType type)
 * @brief 지정한 노드를 기준으로 전위 순회하며 키를 출력하는 함수(재귀)
 * @param node 순회할 기준 노드의 주소(입력)
 * @param type 출력할 키의 데이터 유형(입력)
 * @return 반환값 없음
 */
static void JNodePreorderTraverse(const JNodePtr node, KeyType type)
{
	if(node == NULL) return;
	JNodePrintKey(node, type);
	JNodePreorderTraverse(node->left, type);
	JNodePreorderTraverse(node->right, type);
}

/**
 * @fn static void JNodeInorderTraverse(const JNodePtr node, KeyType type)
 * @brief 지정한 노드를 기준으로 중위 순회하며 키를 출력하는 함수(재귀)
 * @param node 순회할 기준 노드의 주소(입력)
 * @param type 출력할 키의 데이터 유형(입력)
 * @return 반환값 없음
 */
static void JNodeInorderTraverse(const JNodePtr node, KeyType type)
{
	if(node == NULL) return;
	JNodeInorderTraverse(node->left, type);
	JNodePrintKey(node, type);
	JNodeInorderTraverse(node->right, type);
}

/**
 * @fn static void JNodePostorderTraverse(const JNodePtr node, KeyType type)
 * @brief 지정한 노드를 기준으로 후위 순회하며 키를 출력하는 함수(재귀)
 * @param node 순회할 기준 노드의 주소(입력)
 * @param type 출력할 키의 데이터 유형(입력)
 * @return 반환값 없음
 */
static void JNodePostorderTraverse(const JNodePtr node, KeyType type)
{
	if(node == NULL) return;
	JNodePostorderTraverse(node->left, type);
	JNodePostorderTraverse(node->right, type);
	JNodePrintKey(node, type);
}

/**
 * @fn static void JNodePrintKey(const JNodePtr node, KeyType type)
 * @brief 지정한 노드의 키를 출력하는 함수
 * @param node 출력할 노드의 주소(입력, 읽기 전용)
 * @param type 출력할 키의 데이터 유형(입력)
 * @return 반환값 없음
 */
static void JNodePrintKey(const JNodePtr node, KeyType type)
{
	if(node == NULL) return;
	switch(type)
	{
		case IntType:
			printf("%d(%c) ", *((int*)(node->key)), _NodeColorToChar(node->color));
			break;
		case CharType:
			printf("%c(%c) ", *((char*)(node->key)), _NodeColorToChar(node->color));	
			break;
		case StringType:
			printf("%s(%c) ", (char*)(node->key), _NodeColorToChar(node->color));
			break;
		default: return;
	}
}

static JNodePtr JNodeSetParentPtr(JNodePtr node )
{
	if(node == NULL) return NULL;
	if(node->parent == NULL) return node;

	if(node->parent->left == node) node->parent->left = node;
	else if(node->parent->right == node) node->parent->right = node;
	else return NULL;

	return JNodeSetParentPtr(node->parent);
}

static JNodePtr JNodeGetGrandParent(JNodePtr node)
{
	if(node == NULL) return NULL;
	JNodePtr parentNode = node->parent;
	if(parentNode == NULL) return NULL;
	return parentNode->parent;
}

static JNodePtr JNodeGetUncle(JNodePtr node)
{
	JNodePtr grandParentNode = JNodeGetGrandParent(node);
	if(grandParentNode == NULL) return NULL;
	JNodePtr parentNode = node->parent;
	if(parentNode == grandParentNode->right) return grandParentNode->left;
	return grandParentNode->right;
}

/**
 * @fn static int JNodeGetBlackHeight(const JNodePtr node)
 * @brief AVL Tree 에서 지정한 Black 노드의 높이를 구하는 함수(재귀)
 * 루트 노드는 1, 이하 자식 노드 레벨(높이)부터 1 씩 증가
 * @param node 높이를 구하기 위한 노드(입력, 읽기 전용) 
 * @return 성공 시 0 이상의 높이, 실패 시 0 반환
 */
static int JNodeGetBlackHeight(const JNodePtr node)
{
	if(node == NULL) return 0;

	int leftHeight = JNodeGetBlackHeight(node->left);
	int rightHeight = JNodeGetBlackHeight(node->right);

	if(leftHeight > rightHeight)
	{
		if(node->color == Black) return leftHeight + 1;
		return leftHeight;
	}
	else
	{
		if(node->color == Black) return rightHeight + 1;
		return rightHeight;
	}
}

/**
 * @fn static int JNodeGetBlackHeightDiff(const JNodePtr node)
 * @brief AVL Tree 에서 지정한 노드의 자식노드들의 높이 차이를 구하는 함수(재귀)
 * @param node 높이의 차이를 구하기 위한 기준 노드(입력, 읽기 전용) 
 * @return 성공 시 자식 노드들의 높이 차이, 실패 시 0 반환
 */
static int JNodeGetBlackHeightDiff(const JNodePtr node)
{
	if(node == NULL) return 0;
	return JNodeGetBlackHeight(node->left) - JNodeGetBlackHeight(node->right);
}

static int JNodeGetHeight(const JNodePtr node)
{
	if(node == NULL) return 0;

	int leftHeight = JNodeGetHeight(node->left);
	int rightHeight = JNodeGetHeight(node->right);

	if(leftHeight > rightHeight) return leftHeight + 1;
	else return rightHeight + 1;
}

////////////////////////////////////////////////////////////////////////////////
/// JRBTree Static Function
////////////////////////////////////////////////////////////////////////////////

static void JRBTreePrintHeight(const JNodePtr node, int height, KeyType type, char position)
{
	if(node == NULL) return;

	int tabIndex = 0;
	int heightIndex = 0;

	for( ; tabIndex < height; tabIndex++) printf("\t");
	printf("[%c] ", position);
	JNodePrintKey(node, type);
	printf("\n");

	JRBTreePrintHeight(node->left, height + 1, type, 'L');
	JRBTreePrintHeight(node->right, height + 1, type, 'R');
}

static JRBTreePtr JRBTreeRebalance(JRBTreePtr tree)
{
	if(tree == NULL) return NULL;

	int heightDiff = JNodeGetBlackHeightDiff(tree->root);

	if(heightDiff > 1) tree->root = JNodeRightRotate(tree->root);
	if(heightDiff < -1) tree->root = JNodeLeftRotate(tree->root);

	//tree->root->color = Black;

	return tree;
}

////////////////////////////////////////////////////////////////////////////////
/// Util Static Functions
////////////////////////////////////////////////////////////////////////////////

/**
 * @fn static KeyType _CheckKeyType(KeyType type)
 * @brief 지정한 키 데이터 유형이 정의된 유형인지 검사하는 함수
 * @param type 검사할 키 데이터 유형(입력)
 * @return 성공 시 전달받은 키 데이터 유형, 실패 시 Unknown 반환(KeyType 열거형 참고)
 */
static KeyType _CheckKeyType(KeyType type)
{
	switch(type)
	{
		case IntType:
		case CharType:
		case StringType:
			break;
		default:
			return Unknown;
	}
	return type;
}

/**
 * @fn static int _GetCompareLength(const char *s1, const char *s2)
 * @brief 두 문자열을 비교할 때 가장 긴 문자열의 길이를 반환하는 함수
 * @param s1 첫 번째 비교할 문자열(입력, 읽기 전용)
 * @param s2 두 번째 비교할 문자열(입력, 읽기 전용)
 * @return 항상 비교할 길이 반환
 */
static int _GetCompareLength(const char *s1, const char *s2)
{
	int s1Length = strlen(s1);
	int s2Length = strlen(s2);
	return s1Length > s2Length ? s1Length : s2Length;
}

static char _NodeColorToChar(NodeColor color)
{
	switch(color)
	{
		case Red:
			return 'R';
		case Black:
			return 'B';
		default:
			return '\0';
	}
}

