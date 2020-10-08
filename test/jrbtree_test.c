#include "../include/ttlib.h"
#include "../include/jrbtree.h"

////////////////////////////////////////////////////////////////////////////////
/// Definitions of Test
////////////////////////////////////////////////////////////////////////////////

DECLARE_TEST();

// ---------- Common Test ----------

////////////////////////////////////////////////////////////////////////////////
/// Node Test
////////////////////////////////////////////////////////////////////////////////

TEST(Node, CreateAndDeleteNode, {
	JNodePtr node = NewJNode();
	EXPECT_NOT_NULL(node);
	EXPECT_NUM_EQUAL(DeleteJNode(&node), DeleteSuccess);

	EXPECT_NUM_EQUAL(DeleteJNode(NULL), DeleteFail);
})

////////////////////////////////////////////////////////////////////////////////
/// RB Tree Test
////////////////////////////////////////////////////////////////////////////////

TEST(RBTree, CreateAndDeleteRBTree, {
	JRBTreePtr tree = NewJRBTree(IntType);
	EXPECT_NOT_NULL(tree);
	EXPECT_NUM_EQUAL(DeleteJRBTree(&tree), DeleteSuccess);

	EXPECT_NULL(NewJRBTree(123));
	EXPECT_NUM_EQUAL(DeleteJRBTree(NULL), DeleteFail);
})

// ---------- RB Tree int Test ----------

////////////////////////////////////////////////////////////////////////////////
/// Node Test (int)
////////////////////////////////////////////////////////////////////////////////

TEST(Node_INT, SetKey, {
	JNodePtr node = NewJNode();

	int expected = 5;
	EXPECT_NOT_NULL(JNodeSetKey(node, &expected));
	EXPECT_PTR_EQUAL(node->key, &expected);
	EXPECT_NUM_EQUAL(*((int*)(node->key)), expected);

	EXPECT_NULL(JNodeSetKey(NULL, &expected));
	EXPECT_NULL(JNodeSetKey(node, NULL));
	EXPECT_NULL(JNodeSetKey(NULL, NULL));

	DeleteJNode(&node);
})

TEST(Node_INT, GetKey, {
	JNodePtr node = NewJNode();

	int expected = 5;
	JNodeSetKey(node, &expected);
	EXPECT_NOT_NULL(JNodeGetKey(node));
	EXPECT_NUM_EQUAL(*((int*)JNodeGetKey(node)), expected);

	EXPECT_NULL(JNodeGetKey(NULL));

	DeleteJNode(&node);
})

////////////////////////////////////////////////////////////////////////////////
/// RBTree Test (int)
////////////////////////////////////////////////////////////////////////////////

TEST(RBTree_INT, AddNode, {
	JRBTreePtr tree = NewJRBTree(IntType);
	int expected1 = 10;
	int expected2 = 12;
	int expected3 = 2;
	int expected4 = 14;
	int expected5 = 5;
	int expected6 = 16;
	int expected7 = 7;
	int expected8 = 8;
	int expected9 = 19;
	int expected10 = 4;
	int expected11 = 3;

	// 정상 동작 확인
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected1));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected2));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected3));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected4));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected5));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected6));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected7));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected8));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected9));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected10));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected11));
	JRBTreePrintAll(tree);

//	EXPECT_NUM_EQUAL(*((int*)(tree->root->key)), expected2);
	
	// 중복 허용 테스트
	EXPECT_NULL(JRBTreeInsertNode(tree, &expected1));

	// NULL 이 입력값이면, NULL 을 반환
	EXPECT_NULL(JRBTreeInsertNode(NULL, &expected1));
	EXPECT_NULL(JRBTreeInsertNode(tree, NULL));
	EXPECT_NULL(JRBTreeInsertNode(NULL, NULL));

	DeleteJRBTree(&tree);
})

TEST(RBTree_INT, SetData, {
	JRBTreePtr tree = NewJRBTree(IntType);
	int expected = 5;

	EXPECT_NOT_NULL(JRBTreeSetData(tree, &expected));
	EXPECT_NUM_EQUAL(*((int*)(tree->data)), expected);

	EXPECT_NULL(JRBTreeSetData(NULL, &expected));
	EXPECT_NULL(JRBTreeSetData(tree, NULL));
	EXPECT_NULL(JRBTreeSetData(NULL, NULL));

	DeleteJRBTree(&tree);
})

TEST(RBTree_INT, GetData, {
	JRBTreePtr tree = NewJRBTree(IntType);
	int expected = 5;

	JRBTreeSetData(tree, &expected);
	EXPECT_NOT_NULL(JRBTreeGetData(tree));
	EXPECT_NUM_EQUAL(*((int*)JRBTreeGetData(tree)), 5);

	EXPECT_NULL(JRBTreeGetData(NULL));

	DeleteJRBTree(&tree);
})

TEST(RBTree_INT, DeleteNodeByKey, {
	JRBTreePtr tree = NewJRBTree(IntType);
	int expected1 = 10;
	int expected2 = 12;
	int expected3 = 2;
	int expected4 = 14;
	int expected5 = 5;
	int expected6 = 16;
	int expected7 = 7;
	int expected8 = 8;
	int expected9 = 19;
	int expected10 = 4;
	int expected11 = 3;

	// 정상 동작 확인
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected1));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected2));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected3));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected4));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected5));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected6));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected7));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected8));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected9));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected10));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected11));
	JRBTreePrintAll(tree);

	printf("- Delete Start -\n");

	printf("@ Before / Delete key : %d\n", expected1);
	JRBTreePrintAll(tree);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(tree, &expected1), DeleteSuccess);
	printf("@ After\n");
	JRBTreePrintAll(tree);

	printf("@ Before / Delete key : %d\n", expected3);
	JRBTreePrintAll(tree);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(tree, &expected3), DeleteSuccess);
	printf("@ After\n");
	JRBTreePrintAll(tree);

	printf("@ Before / Delete key : %d\n", expected4);
	JRBTreePrintAll(tree);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(tree, &expected4), DeleteSuccess);
	printf("@ After\n");
	JRBTreePrintAll(tree);

	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(tree, &expected1), DeleteFail);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(NULL, &expected1), DeleteFail);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(tree, NULL), DeleteFail);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(NULL, NULL), DeleteFail);

	DeleteJRBTree(&tree);
})

// ---------- RB Tree char Test ----------

////////////////////////////////////////////////////////////////////////////////
/// Node Test (char)
////////////////////////////////////////////////////////////////////////////////

TEST(Node_CHAR, SetKey, {
	JNodePtr node = NewJNode();

	char expected = 'a';
	EXPECT_NOT_NULL(JNodeSetKey(node, &expected));
	EXPECT_PTR_EQUAL(node->key, &expected);
	EXPECT_NUM_EQUAL(*((char*)(node->key)), expected);

	EXPECT_NULL(JNodeSetKey(NULL, &expected));
	EXPECT_NULL(JNodeSetKey(node, NULL));
	EXPECT_NULL(JNodeSetKey(NULL, NULL));

	DeleteJNode(&node);
})

TEST(Node_CHAR, GetKey, {
	JNodePtr node = NewJNode();

	char expected = 'a';
	JNodeSetKey(node, &expected);
	EXPECT_NOT_NULL(JNodeGetKey(node));
	EXPECT_PTR_EQUAL(JNodeGetKey(node), &expected);
	EXPECT_NUM_EQUAL(*((char*)JNodeGetKey(node)), expected);

	EXPECT_NULL(JNodeGetKey(NULL));

	DeleteJNode(&node);
})

////////////////////////////////////////////////////////////////////////////////
/// RBTree Test (char)
////////////////////////////////////////////////////////////////////////////////

TEST(RBTree_CHAR, AddNode, {
	JRBTreePtr tree = NewJRBTree(CharType);
	char expected1 = 'a';
	char expected2 = '1';
	char expected3 = 'c';
	char expected4 = '5';
	char expected5 = 'e';
	char expected6 = '-';
	char expected7 = 'z';
	char expected8 = 'c';
	char expected9 = '@';
	char expected10 = 'b';
	char expected11 = 'w';

	// 정상 동작 확인
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected1));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected2));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected3));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected4));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected5));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected6));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected7));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected8));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected9));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected10));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected11));
	JRBTreePrintAll(tree);

//	EXPECT_NUM_EQUAL(*((char*)(tree->root->key)), expected3);
	
	// 중복 허용 테스트
	EXPECT_NULL(JRBTreeInsertNode(tree, &expected1));

	// NULL 이 입력값이면, NULL 을 반환
	EXPECT_NULL(JRBTreeInsertNode(NULL, &expected1));
	EXPECT_NULL(JRBTreeInsertNode(tree, NULL));
	EXPECT_NULL(JRBTreeInsertNode(NULL, NULL));

	DeleteJRBTree(&tree);
})

TEST(RBTree_CHAR, SetData, {
	JRBTreePtr tree = NewJRBTree(CharType);
	char expected = 'a';

	EXPECT_NOT_NULL(JRBTreeSetData(tree, &expected));
	EXPECT_NUM_EQUAL(*((char*)(tree->data)), expected);

	EXPECT_NULL(JRBTreeSetData(NULL, &expected));
	EXPECT_NULL(JRBTreeSetData(tree, NULL));
	EXPECT_NULL(JRBTreeSetData(NULL, NULL));

	DeleteJRBTree(&tree);
})

TEST(RBTree_CHAR, GetData, {
	JRBTreePtr tree = NewJRBTree(CharType);
	char expected = 'a';

	JRBTreeSetData(tree, &expected);
	EXPECT_NOT_NULL(JRBTreeGetData(tree));
	EXPECT_NUM_EQUAL(*((char*)JRBTreeGetData(tree)), expected);

	EXPECT_NULL(JRBTreeGetData(NULL));

	DeleteJRBTree(&tree);
})

TEST(RBTree_CHAR, DeleteNodeByKey, {
	JRBTreePtr tree = NewJRBTree(CharType);
	char expected1 = 'a';
	char expected2 = '1';
	char expected3 = 'c';
	char expected4 = '5';
	char expected5 = 'e';
	char expected6 = '-';
	char expected7 = 'z';
	char expected8 = 'c';
	char expected9 = '@';
	char expected10 = 'b';
	char expected11 = 'w';

	// 정상 동작 확인
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected1));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected2));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected3));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected4));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected5));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected6));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected7));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected8));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected9));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected10));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, &expected11));
	JRBTreePrintAll(tree);

	printf("- Delete Start -\n");

	printf("@ Before / Delete key : %c\n", expected1);
	JRBTreePrintAll(tree);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(tree, &expected1), DeleteSuccess);
	printf("@ After\n");
	JRBTreePrintAll(tree);

	printf("@ Before / Delete key : %c\n", expected3);
	JRBTreePrintAll(tree);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(tree, &expected3), DeleteSuccess);
	printf("@ After\n");
	JRBTreePrintAll(tree);

	printf("@ Before / Delete key : %c\n", expected4);
	JRBTreePrintAll(tree);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(tree, &expected4), DeleteSuccess);
	printf("@ After\n");
	JRBTreePrintAll(tree);

	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(tree, &expected1), DeleteFail);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(NULL, &expected1), DeleteFail);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(tree, NULL), DeleteFail);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(NULL, NULL), DeleteFail);

	DeleteJRBTree(&tree);
})

// ---------- RB Tree string Test ----------

////////////////////////////////////////////////////////////////////////////////
/// Node Test (string)
////////////////////////////////////////////////////////////////////////////////

TEST(Node_STRING, SetKey, {
	JNodePtr node = NewJNode();

	char *expected = "abc";
	EXPECT_NOT_NULL(JNodeSetKey(node, expected));
	EXPECT_PTR_EQUAL(node->key, expected);
	EXPECT_STR_EQUAL((char*)(node->key), expected);

	EXPECT_NULL(JNodeSetKey(NULL, expected));
	EXPECT_NULL(JNodeSetKey(node, NULL));
	EXPECT_NULL(JNodeSetKey(NULL, NULL));

	DeleteJNode(&node);
})

TEST(Node_STRING, GetKey, {
	JNodePtr node = NewJNode();

	char *expected = "abc";
	JNodeSetKey(node, expected);
	EXPECT_NOT_NULL(JNodeGetKey(node));
	EXPECT_STR_EQUAL((char*)JNodeGetKey(node), expected);

	EXPECT_NULL(JNodeGetKey(NULL));

	DeleteJNode(&node);
})

////////////////////////////////////////////////////////////////////////////////
/// RBTree Test (string)
////////////////////////////////////////////////////////////////////////////////

TEST(RBTree_STRING, AddNode, {
	JRBTreePtr tree = NewJRBTree(StringType);
	char *expected1 = "abc";
	char *expected2 = "def";
	char *expected3 = "zone";
	char *expected4 = "dog";
	char *expected5 = "human";
	char *expected6 = "#define";
	char *expected7 = "[UXCUTOR]";
	char *expected8 = "010-1234-5678";
	char *expected9 = "age : 10";
	char *expected10 = "apple";
	char *expected11 = "orange";

	// 정상 동작 확인
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected1));
	JRBTreePrintAll(tree);
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected2));
	JRBTreePrintAll(tree);
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected3));
	JRBTreePrintAll(tree);
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected4));
	JRBTreePrintAll(tree);
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected5));
	JRBTreePrintAll(tree);
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected6));
	JRBTreePrintAll(tree);
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected7));
	JRBTreePrintAll(tree);
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected8));
	JRBTreePrintAll(tree);
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected9));
	JRBTreePrintAll(tree);
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected10));
	JRBTreePrintAll(tree);
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected11));
	JRBTreePrintAll(tree);

//	EXPECT_STR_EQUAL((char*)(tree->root->key), expected1);
	
	// 중복 허용 테스트
	EXPECT_NULL(JRBTreeInsertNode(tree, expected1));

	// NULL 이 입력값이면, NULL 을 반환
	EXPECT_NULL(JRBTreeInsertNode(NULL, expected1));
	EXPECT_NULL(JRBTreeInsertNode(tree, NULL));
	EXPECT_NULL(JRBTreeInsertNode(NULL, NULL));

	DeleteJRBTree(&tree);
})

TEST(RBTree_STRING, SetData, {
	JRBTreePtr tree = NewJRBTree(StringType);
	char* expected = "abc";

	EXPECT_NOT_NULL(JRBTreeSetData(tree, expected));
	EXPECT_PTR_EQUAL(tree->data, expected);
	EXPECT_STR_EQUAL((char*)(tree->data), expected);

	EXPECT_NULL(JRBTreeSetData(NULL, expected));
	EXPECT_NULL(JRBTreeSetData(tree, NULL));
	EXPECT_NULL(JRBTreeSetData(NULL, NULL));

	DeleteJRBTree(&tree);
})

TEST(RBTree_STRING, GetData, {
	JRBTreePtr tree = NewJRBTree(StringType);
	char* expected = "abc";

	JRBTreeSetData(tree, expected);
	EXPECT_NOT_NULL(JRBTreeGetData(tree));
	EXPECT_PTR_EQUAL(JRBTreeGetData(tree), expected);
	EXPECT_STR_EQUAL((char*)JRBTreeGetData(tree), expected);

	EXPECT_NULL(JRBTreeGetData(NULL));

	DeleteJRBTree(&tree);
})

TEST(RBTree_STRING, DeleteNodeByKey, {
	JRBTreePtr tree = NewJRBTree(StringType);
	char *expected1 = "abc";
	char *expected2 = "def";
	char *expected3 = "zone";
	char *expected4 = "dog";
	char *expected5 = "human";
	char *expected6 = "#define";
	char *expected7 = "[UXCUTOR]";
	char *expected8 = "010-1234-5678";
	char *expected9 = "age : 10";
	char *expected10 = "apple";
	char *expected11 = "orange";

	// 정상 동작 확인
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected1));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected2));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected3));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected4));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected5));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected6));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected7));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected8));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected9));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected10));
	EXPECT_NOT_NULL(JRBTreeInsertNode(tree, expected11));
	JRBTreePrintAll(tree);

	printf("- Delete Start -\n");

	printf("@ Before / Delete key : %s\n", expected1);
	JRBTreePrintAll(tree);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(tree, expected1), DeleteSuccess);
	printf("@ After\n");
	JRBTreePrintAll(tree);

	printf("@ Before / Delete key : %s\n", expected3);
	JRBTreePrintAll(tree);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(tree, expected3), DeleteSuccess);
	printf("@ After\n");
	JRBTreePrintAll(tree);

	printf("@ Before / Delete key : %s\n", expected4);
	JRBTreePrintAll(tree);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(tree, expected4), DeleteSuccess);
	printf("@ After\n");
	JRBTreePrintAll(tree);

	printf("@ Before / Delete key : %s\n", expected7);
	JRBTreePrintAll(tree);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(tree, expected7), DeleteSuccess);
	printf("@ After\n");
	JRBTreePrintAll(tree);

	printf("@ Before / Delete key : %s\n", expected2);
	JRBTreePrintAll(tree);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(tree, expected2), DeleteSuccess);
	printf("@ After\n");
	JRBTreePrintAll(tree);

	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(tree, expected1), DeleteFail);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(NULL, expected1), DeleteFail);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(tree, NULL), DeleteFail);
	EXPECT_NUM_EQUAL(JRBTreeDeleteNodeByKey(NULL, NULL), DeleteFail);

	DeleteJRBTree(&tree);
})

////////////////////////////////////////////////////////////////////////////////
/// Main Function
////////////////////////////////////////////////////////////////////////////////

int main()
{
    CREATE_TESTSUIT();

    REGISTER_TESTS(
		// @ Common Test -----------------------------------------
		Test_Node_CreateAndDeleteNode,
		Test_RBTree_CreateAndDeleteRBTree,

		// @ INT Test -------------------------------------------
		Test_Node_INT_SetKey,
		Test_Node_INT_GetKey,
		Test_RBTree_INT_AddNode,
		Test_RBTree_INT_SetData,
		Test_RBTree_INT_GetData,
		Test_RBTree_INT_DeleteNodeByKey,

		// @ CHAR Test -------------------------------------------
		Test_Node_CHAR_SetKey,
		Test_Node_CHAR_GetKey,
		Test_RBTree_CHAR_AddNode,
		Test_RBTree_CHAR_SetData,
		Test_RBTree_CHAR_GetData,
		Test_RBTree_CHAR_DeleteNodeByKey,

		// @ STRING Test -------------------------------------------
		Test_Node_STRING_SetKey,
		Test_Node_STRING_GetKey,
		Test_RBTree_STRING_AddNode,
		Test_RBTree_STRING_SetData,
		Test_RBTree_STRING_GetData,
		Test_RBTree_STRING_DeleteNodeByKey
    );

    RUN_ALL_TESTS();

    CLEAN_UP_TESTSUIT();

	return 1;
}

