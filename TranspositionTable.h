#pragma once
#include"CardsInfo.h"
//�û�����

typedef  struct HTN
{
	__int64 playerPokersInfo;//�������ʣ����Ϣ
	int pokersBlockInfo;//���������ѡ�е��ƿ���Ϣ
	int value;//����ֵ
	int depth;//�������

}hashTableNode;

class TranspositionTable
{
public:
	TranspositionTable();
	~TranspositionTable();
	
	void InitializeHashTable();
	hashTableNode ** pHT;//��ϣ���ͷָ��
	int HashCheck(int* LastPlayCards, int* theOtherPlayCards, int iDepth, int** iOnHand);
	void CalculateHashKey(int* LastPlayerCards, int* theOtherPlayerCards, int** iOnHand);
private:
	int hashKey1;
	int hashKey2;
	__int64 checkSum;//У��������Ϣ
	int ichooseCards;
	CardsInfo* CInfo;
	SortCards*SCards;
public:
	__int64 CalculateCheckSum(int** iCards);
	void EnterHashTable(int* LastPlayCards, int* theOtherPlayCards, int iDepth, int** iOnHand, int value, int *ChooseCards);
};

