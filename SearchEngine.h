#pragma once
#include "RecommendCards.h"
#include "Ddz.h"
#include "string.h"
#include "CardsValue.h"
#include <vector>
using namespace std;

enum Entry_type{exact,lower_bound,upper_bound};
typedef  struct HTN
{
	Entry_type entryType;//���ݱ�������
	__int64 checkSum;//�����ʣ����Ϣ
	int pokersBlockInfo;//���������ѡ�е��ƿ���Ϣ
	int value;//����ֵ
	int depth;//�������
	 
}hashTableNode;

class SearchEngine
{
public:
	/////////////////////////////////////////////
	//////////////////PVS����//////////////////
	/////////////////////////////////////////////

	SearchEngine();
	~SearchEngine();
	int IsGameOver(int iDepth);
	int MakeMove(int iDepth);
	Ddz *pDdz;
	int iMaxDepth;
	int iPlaCount;//��������ģ����Ƶ��ܿ��н���
	CardsValue *pValue;

private:
	RecommendCards *pMoveGen;//����������
	CardsInfo* pCInfo;
	int Alpha_Beta_half(int iDepth, int iAlpha, int iBeta);
	int PVS_HH_TT(int iDepth, int iAlpha, int iBeta);//�����㷨
	int layerCardsNum[8];//�洢ÿ�����ʱ�����ɵ��Ƽ��ƵĿ��н�������
public:
	void IfHelpMyself(Ddz* pDdz, char cPlayer);
	void SearchAGoodMove(Ddz* pDdz,int iDepth);//����ӿ�
	int iOnTable[3][21];//�����������ģ�������ҳ����ƣ�iOnTable[0]�������ڻָ��ƾ�
	int iOnHand[3][21];//����ģ����ƺ�������е��ƣ�0�Ǳ���������ң�1�Ǳ���������ң�2�Ǳ��ң����ң���Ϸ�е��������
	char cLastPlay;//��һλģ�������ұ��
	void UpdatePosition(int iCards[], int iDepth, bool ifDelete);
	int iChoose;//����߷���iplaArr�е��±�
	/////////////////////////////////////////////
	//////////////////��ʷ��������//////////////////
	/////////////////////////////////////////////

	// ����ʷ�÷ֱ�����
	void ResetHistoryTable();
	void EnterHistroyScore(int iCards[], int iDepth);
	void HistorySortNode(int *iCards);
	// ��ȡ��ʷ������λ��
	int GetHistroyPosition(int* iCardsis);
	int historyTable[3900];//��ʷ�÷ֱ�����Ϊ��������*���Ƶ�������*��������=20*15*13,���Ƶ��������͵�ֵ������Ϊ��

	/////////////////////////////////////////////
	//////////////////�û�������//////////////////
	/////////////////////////////////////////////
public:
	void InitializeHashTable();
	//hashTableNode **pHT;//��ϣ���ͷָ��
	 static hashTableNode hashTable[20 * 15 * 13][20 * 15 * 13];
	int HashCheck(int* LastPlayCards, int* theOtherPlayCards, int iAlpha,int iBeta, int iDepth, int iCards[21]);
	void CalculateHashKey(int* LastPlayerCards, int* theOtherPlayerCards, int iCards[21]);
private:
	int hashKey1;
	int hashKey2;
	__int64 checkSum;//У��������Ϣ������ǰ����������ҵ�ʣ����Ϣ
	int ichooseCards;

public:
	__int64 CalculateCheckSum(int iCards[21]);
	void EnterHashTable(Entry_type entryType, int* LastPlayCards, int* theOtherPlayCards, int iDepth, int iOnPlayer[21], int value, int *ChooseCards);

	// ��ֵ��ӿ�
	int Evaluation(int* pLastPlayCards, int* pThisPlayerCards, int* pNextPlayCards, int *pOntable, char cThisPlayer,bool isMaxNode);
	char ThisDepthPlayer(int iDepth);
	int MINMAX_thridPersonVersion(int iDepth);
	bool IsMaxNode(char cThisPlayer);
};

