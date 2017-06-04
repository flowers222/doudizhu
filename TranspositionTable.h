#pragma once
#include"CardsInfo.h"
//置换表类

typedef  struct HTN
{
	__int64 playerPokersInfo;//其他玩家剩牌信息
	int pokersBlockInfo;//本玩家搜索选中的牌块信息
	int value;//返回值
	int depth;//搜索深度

}hashTableNode;

class TranspositionTable
{
public:
	TranspositionTable();
	~TranspositionTable();
	
	void InitializeHashTable();
	hashTableNode ** pHT;//哈希表表头指针
	int HashCheck(int* LastPlayCards, int* theOtherPlayCards, int iDepth, int** iOnHand);
	void CalculateHashKey(int* LastPlayerCards, int* theOtherPlayerCards, int** iOnHand);
private:
	int hashKey1;
	int hashKey2;
	__int64 checkSum;//校验局面的信息
	int ichooseCards;
	CardsInfo* CInfo;
	SortCards*SCards;
public:
	__int64 CalculateCheckSum(int** iCards);
	void EnterHashTable(int* LastPlayCards, int* theOtherPlayCards, int iDepth, int** iOnHand, int value, int *ChooseCards);
};

