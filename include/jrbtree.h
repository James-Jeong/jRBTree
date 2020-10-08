#ifndef __JRBTREE_H__
#define __JRBTREE_H__

///////////////////////////////////////////////////////////////////////////////
/// Enums
///////////////////////////////////////////////////////////////////////////////

// 메모리 헤제 결과 열거형
typedef enum DeleteResult
{
	// 실패
	DeleteFail = -1,
	// 성공
	DeleteSuccess = 1
} DeleteResult;

// 검색 결과 열거형
typedef enum FindResult
{
	// 실패
	FindFail = -1,
	// 성공
	FindSuccess = 1
} FindResult;

// 키 유형 열거형
typedef enum KeyType
{
	// 알 수 없는 유형
	Unknown = -1,
	// 정수
	IntType = 1,
	// 문자
	CharType,
	// 문자열
	StringType
} KeyType;

typedef enum NodeColor
{
	// Red
	Red = 1,
	// Black
	Black
} NodeColor;

///////////////////////////////////////////////////////////////////////////////
/// Macro
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Definitions
///////////////////////////////////////////////////////////////////////////////

// Linked List 에서 key 를 관리하기 위한 노드 구조체
typedef struct _jnode_t {
	// 노드 색깔
	NodeColor color;
	// Value
	void *key;
	// 부모 노드 주소
	struct _jnode_t *parent;
	// 이전 노드 주소
	struct _jnode_t *left;
	// 다음 노드 주소
	struct _jnode_t *right;
} JNode, *JNodePtr, **JNodePtrContainer;

// RB Tree 구조체
typedef struct _jrbtree_t {
	// 키 데이터 유형
	KeyType type;
	// 루트 노드
	JNodePtr root;
	// 사용자 데이터
	void *data;
} JRBTree, *JRBTreePtr, **JRBTreePtrContainer;

///////////////////////////////////////////////////////////////////////////////
// Functions for JNode
///////////////////////////////////////////////////////////////////////////////

JNodePtr NewJNode();
DeleteResult DeleteJNode(JNodePtrContainer container);
void* JNodeGetKey(const JNodePtr node);
void* JNodeSetKey(JNodePtr node, void *key);

///////////////////////////////////////////////////////////////////////////////
// Functions for JRBTree
///////////////////////////////////////////////////////////////////////////////

JRBTreePtr NewJRBTree(KeyType type);
DeleteResult DeleteJRBTree(JRBTreePtrContainer container);

void* JRBTreeGetData(const JRBTreePtr tree);
void* JRBTreeSetData(JRBTreePtr tree, void *data);

JRBTreePtr JRBTreeInsertNode(JRBTreePtr tree, void *data);
DeleteResult JRBTreeDeleteNodeByKey(JRBTreePtr tree, void *key);

void JRBTreePreorderTraverse(const JRBTreePtr tree);
void JRBTreeInorderTraverse(const JRBTreePtr tree);
void JRBTreePostorderTraverse(const JRBTreePtr tree);

void JRBTreePrintAll(const JRBTreePtr tree);

#endif

