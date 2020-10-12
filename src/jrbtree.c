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
static void JNodePrintKey(const JNodePtr node, KeyType type);
static JNodePtr JNodeGetUncle(const JNodePtr node);
static JNodePtr JNodeGetGrandParent(const JNodePtr node);
static JNodePtr JNodeSetParentPtr(const JNodePtr node);

////////////////////////////////////////////////////////////////////////////////
/// Predefinition of JRBTree Static Functions
////////////////////////////////////////////////////////////////////////////////

static void JRBTreePrintHeight(const JNodePtr node, int height, KeyType type, char position);
static JRBTreePtr JRBTreeSetChildNode(const JRBTreePtr tree, const JNodePtr parentNode, const JNodePtr childNode);

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
		if(JRBTreeSetChildNode(tree, parentNode, newNode) == NULL)
		{
				DeleteJNode(&newNode);
				return NULL;
		}

		newNode->color = Red;

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
					_newNode->color = Red;
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

	tree->root->color = Black;
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
	JNodePtr selectedNode = JRBTreeFindNodeByKey(tree, key);
	if(selectedNode == NULL) return DeleteFail;

	DeleteResult result = DeleteSuccess;

	JNodePtr dummyNode = NewJNode();
	if(dummyNode == NULL) return DeleteFail;
	dummyNode->right = tree->root;

	JNodePtr parentNode = selectedNode->parent;
	JNodePtr currentNode = selectedNode;

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
		JNodePtr parentNode = selectedNode->parent;
		JNodePtr scNode = NULL;

		if(selectedNode->left != NULL) scNode = selectedNode->left;
		else scNode = selectedNode->right;

		if(parentNode->left == selectedNode) parentNode->left = scNode;
		else parentNode->right = scNode;

		scNode->parent = parentNode;

		// Black 노드 삭제 시 Double Red 문제 발생
		// 삭제할 노드가 Black 이고 부모 노드가 Red 이면, 자식 노드를 Black 으로 바꾼다.
		if((selectedNode->color == Black) && (parentNode->color == Red))
		{
			if(scNode->color != Black) scNode->color = Black;
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

		// 대체 노드가 Black 이고, 대체 노드의 오른쪽 노드와 삭제할 노드가 Red 인 경우
		// Black height 조건을 성립시키기 위해 대체 노드의 오른쪽 노드를 Black 으로 바꾼다.
		if((scNode->right != NULL) && (scNode->right->color == Red) && (selectedNode->color == Red))
		{
			scNode->right->color = Black;
		}

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
 * @fn JNodePtr JRBTreeFindNodeByKey(const JRBTreePtr tree, void *key)
 * @brief RB Tree에서 지정한 키를 가진 노드를 검색하는 함수
 * @param tree RB Tree 구조체 객체의 주소(입력, 읽기 전용)
 * @param key 검색할 키의 주소(입력)
 * @return 성공 시 노드의 주소, 실패 시 NULL 반환
 */
JNodePtr JRBTreeFindNodeByKey(const JRBTreePtr tree, void *key)
{
	if(tree == NULL || key == NULL) return NULL;

	JNodePtr currentNode = tree->root;
	while((currentNode != NULL) && (key != currentNode->key))
	{
		currentNode = JNodeMove(currentNode, key, tree->type);
	}

	return currentNode;
}

/**
 * @fn void JRBTreePrintAll(const JRBTreePtr tree)
 * @brief RB Tree 에 저장된 모든 노드들의 키를 출력하는 함수
 * @param tree RB Tree 구조체 객체의 주소(입력, 읽기 전용)
 * @return 반환값 없음
 */
void JRBTreePrintAll(const JRBTreePtr tree)
{
	if(tree == NULL) return;
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
 * JRBTreeInsertNode 와 JRBTreeFindNodeByKey 함수에서 호출되므로 매개변수 NULL 체크를 수행하지 않음
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
			if(strncmp((char*)(node->key), (char*)(key), _GetCompareLength((char*)(node->key), (char*)(key))) > 0) node = node->left;
			else node = node->right;
			break;
		}
		default: return NULL;
	}
	return node;
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

/**
 * @fn static JNodePtr JNodeSetParentPtr(const JNodePtr node )
 * @brief 지정한 노드의 부모 노드에게 주소를 전달하는 함수(재귀)
 * @param node 주소를 전달할 노드(입력, 읽기 전용)
 * @return 반환값 없음
 */
static JNodePtr JNodeSetParentPtr(const JNodePtr node)
{
	if(node == NULL) return NULL;
	if(node->parent == NULL) return node;

	if(node->parent->left == node) node->parent->left = node;
	else if(node->parent->right == node) node->parent->right = node;
	else return NULL;

	return JNodeSetParentPtr(node->parent);
}

/**
 * @fn static JNodePtr JNodeGetGrandParent(const JNodePtr node)
 * @brief 지정한 노드의 조부모 노드(node->parent->parent)를 반환하는 함수
 * @param node 조부모 노드를 반환할 노드(입력, 읽기 전용)
 * @return 성공 시 조부모 노드의 주소, 실패 시 NULL 반환
 */
static JNodePtr JNodeGetGrandParent(const JNodePtr node)
{
	if(node == NULL) return NULL;
	JNodePtr parentNode = node->parent;
	if(parentNode == NULL) return NULL;
	return parentNode->parent;
}

/**
 * @fn static JNodePtr JNodeGetGrandParent(const JNodePtr node)
 * @brief 지정한 노드의 부모의 형제 노드(node->parent->other_child)를 반환하는 함수
 * @param node 부모의 형제 노드를 반환할 노드(입력, 읽기 전용)
 * @return 성공 시 부모의 형제 노드의 주소, 실패 시 NULL 반환
 */
static JNodePtr JNodeGetUncle(const JNodePtr node)
{
	JNodePtr grandParentNode = JNodeGetGrandParent(node);
	if(grandParentNode == NULL) return NULL;
	JNodePtr parentNode = node->parent;
	if(parentNode == grandParentNode->right) return grandParentNode->left;
	return grandParentNode->right;
}

////////////////////////////////////////////////////////////////////////////////
/// JRBTree Static Functions
////////////////////////////////////////////////////////////////////////////////

/**
 * @fn static void JRBTreePrintHeight(const JNodePtr node, int height, KeyType type, char position)
 * @brief RB Tree 에서 지정한 높이에 있는 노드들의 키를 모두 출력하는 함수(재귀)
 * @param node 지정한 높이의 부모 노드(입력, 읽기 전용)
 * @param height 출력할 높이(입력)
 * @param type 출력할 키의 데이터 유형(입력)
 * @param position 현재 노드의 위치를 알려주는 문자, 출력에 사용(입력)
 * @return 반환값 없음
 */
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

/**
 * @fn static JRBTreePtr JRBTreeSetChildNode(const JRBTreePtr tree, const JNodePtr parentNode, const JNodePtr childNode)
 * @brief 지정한 노드의 자식 노드를 설정하는 함수
 * JRBTreeInsertNode 함수에서 호출되므로 매개변수 NULL 체크를 수행하지 않음
 * @param tree RB Tree 구조체 객체의 주소(입력, 읽기 전용, RB tree 의 데이터 저장 유형을 사용)
 * @param parentNode 자식 노드를 설정할 노드(입력, 읽기 전용)
 * @param childNode 설정할 자식 노드(입력, 읽기 전용)
 * @return 성공 시 사용한 RB Tree 의 주소, 실패 시 NULL 반환
 */
static JRBTreePtr JRBTreeSetChildNode(const JRBTreePtr tree, const JNodePtr parentNode, const JNodePtr childNode)
{
	switch(tree->type)
	{
		case IntType:
			if(*((int*)(parentNode->key)) > *((int*)(childNode->key))) parentNode->left = childNode;
			else parentNode->right = childNode;
			break;
		case CharType:
			if(*((char*)(parentNode->key)) > *((char*)(childNode->key))) parentNode->left = childNode;
			else parentNode->right = childNode;
			break;
		case StringType:
			if(strncmp((char*)(parentNode->key), (char*)(childNode->key), _GetCompareLength((char*)(parentNode->key), (char*)(childNode->key))) > 0) parentNode->left = childNode;
			else parentNode->right = childNode;
			break;
		default:
			return NULL;
	}

	childNode->parent = parentNode;
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

/**
 * @fn static char _NodeColorToChar(NodeColor color)
 * @brief NodeColor 열거형 값을 특정 색을 가리키는 문자로 변환하는 함수
 * @param color 문자로 변경할 노드의 색(입력, NodeColor 열거형 참고)
 * @return 성공 시 변환된 문자, 실패 시 널 문자 반환
 */
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

