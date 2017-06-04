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
	Entry_type entryType;//数据表项类型
	__int64 checkSum;//本玩家剩牌信息
	int pokersBlockInfo;//本玩家搜索选中的牌块信息
	int value;//返回值
	int depth;//搜索深度
	 
}hashTableNode;

class SearchEngine
{
public:
	/////////////////////////////////////////////
	//////////////////PVS所需//////////////////
	/////////////////////////////////////////////

	SearchEngine();
	~SearchEngine();
	int IsGameOver(int iDepth);
	int MakeMove(int iDepth);
	Ddz *pDdz;
	int iMaxDepth;
	int iPlaCount;//本次搜索模拟出牌的总可行解数
	CardsValue *pValue;

private:
	RecommendCards *pMoveGen;//出牌生成器
	CardsInfo* pCInfo;
	int Alpha_Beta_half(int iDepth, int iAlpha, int iBeta);
	int PVS_HH_TT(int iDepth, int iAlpha, int iBeta);//搜索算法
	int layerCardsNum[8];//存储每层调用时已生成的推荐牌的可行解数量。
public:
	void IfHelpMyself(Ddz* pDdz, char cPlayer);
	void SearchAGoodMove(Ddz* pDdz,int iDepth);//引擎接口
	int iOnTable[3][21];//保存最近两次模拟出牌玩家出的牌，iOnTable[0]缓存用于恢复牌局
	int iOnHand[3][21];//保存模拟出牌后玩家手中的牌，0是本家上手玩家，1是本家下手玩家，2是本家（本家：游戏中的人类玩家
	char cLastPlay;//上一位模拟出牌玩家编号
	void UpdatePosition(int iCards[], int iDepth, bool ifDelete);
	int iChoose;//最佳走法在iplaArr中的下标
	/////////////////////////////////////////////
	//////////////////历史启发所需//////////////////
	/////////////////////////////////////////////

	// 将历史得分表清零
	void ResetHistoryTable();
	void EnterHistroyScore(int iCards[], int iDepth);
	void HistorySortNode(int *iCards);
	// 获取历史启发表位置
	int GetHistroyPosition(int* iCardsis);
	int historyTable[3900];//历史得分表，长度为牌张总数*主牌点数总数*牌型总数=20*15*13,单牌点数和牌型的值都不能为零

	/////////////////////////////////////////////
	//////////////////置换表所需//////////////////
	/////////////////////////////////////////////
public:
	void InitializeHashTable();
	//hashTableNode **pHT;//哈希表表头指针
	 static hashTableNode hashTable[20 * 15 * 13][20 * 15 * 13];
	int HashCheck(int* LastPlayCards, int* theOtherPlayCards, int iAlpha,int iBeta, int iDepth, int iCards[21]);
	void CalculateHashKey(int* LastPlayerCards, int* theOtherPlayerCards, int iCards[21]);
private:
	int hashKey1;
	int hashKey2;
	__int64 checkSum;//校验局面的信息：即当前申请搜索玩家的剩牌信息
	int ichooseCards;

public:
	__int64 CalculateCheckSum(int iCards[21]);
	void EnterHashTable(Entry_type entryType, int* LastPlayCards, int* theOtherPlayCards, int iDepth, int iOnPlayer[21], int value, int *ChooseCards);

	// 估值类接口
	int Evaluation(int* pLastPlayCards, int* pThisPlayerCards, int* pNextPlayCards, int *pOntable, char cThisPlayer,bool isMaxNode);
	char ThisDepthPlayer(int iDepth);
	int MINMAX_thridPersonVersion(int iDepth);
	bool IsMaxNode(char cThisPlayer);
};

