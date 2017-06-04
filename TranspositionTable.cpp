#include "TranspositionTable.h"


TranspositionTable::TranspositionTable() :CInfo(CardsInfo::Instance()), SCards(SortCards::Instance())
{
	InitializeHashTable();
}


TranspositionTable::~TranspositionTable()
{
	delete(pHT);
}


void TranspositionTable::InitializeHashTable()
{
	
	int num = 20 * 15 * 11;//�����еĳ����������=��������*��ֵ����*��������
	//������ά����
	pHT= new hashTableNode*[num];
	for (int i = 0; i < num; i++)
		pHT[i] = new hashTableNode[num];
}


int TranspositionTable::HashCheck(int* LastPlayCards, int* theOtherPlayCards, int iDepth, int** iOnHand)
{
	CalculateHashKey(LastPlayCards, theOtherPlayCards,iOnHand);
	hashTableNode *p;
	p = &pHT[hashKey1][hashKey2];
	if (p != NULL)
	{
		if (iDepth <= p->depth&&checkSum == p->playerPokersInfo)
		{
			return p->value;
		}
	}
		
	return 9999;
}

//������һ����Һ����ϸ���ҳ���һ���ƣ���������ϣֵ
void TranspositionTable::CalculateHashKey(int* LastPlayerCards, int* theOtherPlayerCards,int** iOnHand)
{
	int* iCards=NULL;
	int iCount ;
	int iMainPoint;
	int iMainType;
	for (int i = 0; i<2; i++)
	{
		
		switch (i)
		{
		case 0:{iCards=LastPlayerCards; break; }
		case 1:{iCards=theOtherPlayerCards; break; }
		}
		//���������
		iMainPoint = CInfo->AnalyzeMainPoint(iCards);
		int countPoint = CInfo->AnalyzeTypeCount(iCards);
			iCount = countPoint % 100;
			iMainType = countPoint / 100;
			switch (i)
			{
			case 0:{hashKey1 = iCount*iMainPoint*iMainType; break; }
			case 1:{hashKey2 = iCount*iMainPoint*iMainType; break; }
			}
			
	}

	checkSum=CalculateCheckSum(iOnHand);

}


__int64 TranspositionTable::CalculateCheckSum(int** iOnHand)
{
	int *iCards = new int[55];
	int iCount = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; iOnHand[i][j] != -1; j++)
		{
			iCards[iCount] = iOnHand[i][j];
			iCount++;
		}
	}
	SCards->SortById(iCards);
	////////////////////////////////////////////////////////////////////////
	/////UNDO:������ת���ɶ����ƣ��ٰѶ�����ת����ʮ���ƣ������������������Ϣ/////
	///////////////////////////////////////////////////////////////////////
	delete(iCards);
	return 0;
}


void TranspositionTable::EnterHashTable(int* LastPlayCards, int* theOtherPlayCards, int iDepth, int** iOnHand,int value,int *ChooseCards)
{
	CalculateHashKey(LastPlayCards, theOtherPlayCards, iOnHand);
	checkSum = CalculateCheckSum(iOnHand);
	hashTableNode *p;
	p = &pHT[hashKey1][hashKey2];
	p->depth = iDepth;
	p->value = value;
	p->playerPokersInfo = checkSum;
	p->pokersBlockInfo = ichooseCards;
}
