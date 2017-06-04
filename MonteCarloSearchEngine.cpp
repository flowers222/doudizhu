#include "MonteCarloSearchEngine.h"
#include <random>
#include <iostream>

//最新修改：用户方位信息更新错误，AI稳赢（整除错误）
MonteCarloSearchEngine::MonteCarloSearchEngine()
{
	
	srand((unsigned)time(NULL));//初始化随机种子：测试
    pRecomCards = new RecommendCards();
	pMyDdzBoard = new fDdz();
	pUpDdzBoard = new fDdz();
	pDownDdzBoard = new fDdz();
	pCinfo = new CardsInfo();
	//初始化测试数据
	AIWin = 0;
	UpWin = 0;
	DownWin = 0;
	nodeSeed = 0;
}
MonteCarloSearchEngine::~MonteCarloSearchEngine()
{
	delete(pRecomCards);
	delete(pMyDdzBoard);
	delete(pUpDdzBoard);
	delete(pDownDdzBoard);
	delete(pCinfo);
	//清除节点
	delete(nodesList);
}
//初始化搜索引擎
void MonteCarloSearchEngine::Init()
{

	//初始化固定基本信息
	GetOtherPlayerCards(pUpDdzBoard->iOnHand,pDownDdzBoard->iOnHand);
	pMyDdzBoard->cDir = pRealDdz->cDir;
	pMyDdzBoard->cLandlord = pRealDdz->cLandlord;
	pMyDdzBoard->cLastPlay = pRealDdz->cLastPlay;
	//初始化方位信息
	switch (pMyDdzBoard->cDir)
	{
	case 'A':
		pUpDdzBoard->cDir = 'C';
		pDownDdzBoard->cDir = 'B';
		break;
	case 'B':
		pUpDdzBoard->cDir = 'A';
		pDownDdzBoard->cDir = 'C';
		break;
	case 'C':
		pUpDdzBoard->cDir = 'B';
		pDownDdzBoard->cDir = 'A';
		break;
	}
	pUpDdzBoard->cLastPlay = pDownDdzBoard->cDir;
	pUpDdzBoard->cLandlord = pRealDdz->cLandlord;
	pDownDdzBoard->cLastPlay = pMyDdzBoard->cDir;
	pDownDdzBoard->cLandlord = pRealDdz->cLandlord;

	//初始化模拟牌局信息
	pMyDdzBoard->iLastPassCount = pRealDdz->iLastPassCount;
	pMyDdzBoard->iLastTypeCount = pRealDdz->iLastTypeCount;
	pMyDdzBoard->iLastMainPoint = pRealDdz->iLastMainPoint;
	pMyDdzBoard->iUpPlayerCards = pRealDdz->iUpPlayerCards;
	pMyDdzBoard->iDownPlayerCards = pRealDdz->iDownPlayerCards;
	//记录AI最后一次有效桌面信息
	memcpy(pMyDdzBoard->iOnHand, pRealDdz->iOnHand, sizeof(pRealDdz->iOnHand));
	int i;
	for (i = 0; pRealDdz->iOnTable[i][0] != -2; i++);
	if (pRealDdz->iLastPassCount == 1)
	{
		pMyDdzBoard->iLastOnTable = pRealDdz->iOnTable[i - 2];
	}
	else
	{
		//如果第一次出牌
		if (i==0)
			pMyDdzBoard->iLastOnTable = pRealDdz->iOnTable[0];
		else
		pMyDdzBoard->iLastOnTable = pRealDdz->iOnTable[i - 1];
	}
	for (i = 0; pMyDdzBoard->iLastOnTable[i] > 0; i++)
	{
		iLastValidCards[i] = pMyDdzBoard->iLastOnTable[i];
	}
	iLastValidCards[i] = -1;
	//计算AI手牌数
	int myCardsCount = pCinfo->CountCards(pRealDdz->iOnHand);

	//初始化上家
	pUpDdzBoard->iLastPassCount = pRealDdz->iLastPassCount;
	pUpDdzBoard->iLastTypeCount = pRealDdz->iLastTypeCount;
	pUpDdzBoard->iUpPlayerCards = pRealDdz->iDownPlayerCards;
	pUpDdzBoard->iLastMainPoint = pRealDdz->iLastMainPoint;
	pUpDdzBoard->iDownPlayerCards = myCardsCount;
	pUpDdzBoard->iLastOnTable = iLastValidCards;
	//初始化下家

	pDownDdzBoard->iLastPassCount = pRealDdz->iLastPassCount;
	pDownDdzBoard->iLastTypeCount = pRealDdz->iLastTypeCount;
	pDownDdzBoard->iUpPlayerCards = myCardsCount;
	pDownDdzBoard->iLastMainPoint = pRealDdz->iLastMainPoint;
	pDownDdzBoard->iDownPlayerCards = pRealDdz->iUpPlayerCards;
	pDownDdzBoard->iLastOnTable = iLastValidCards;


}


//开始搜索
void MonteCarloSearchEngine::SearchAGoodMove(Ddz *pDdz)
{

	pRealDdz = pDdz;
	//初始化各种值
	Init();

	bool model=pRecomCards->GetModel(pRealDdz);
	if (!model)
	{
		GetOtherPlayerCards(pRecomCards->iUpCards, pRecomCards->iDownCards);
	}
	iNodesCount = pRecomCards->HelpPla(pMyDdzBoard,0);
	if (iNodesCount == 1)
	{
		memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
		return;
	}

	nodesList = new MTCNode[iNodesCount];
	//初始化节点
	for (int i = 0; pMyDdzBoard->iPlaArr[i][0] != -1; i++)
	{
		//memcpy(iPlaArr[i], pMyDdzBoard->iPlaArr[i], sizeof(pMyDdzBoard->iPlaArr[i]));
		//iPlaArr[i+1][0] = -1;
		//保存节点
		memcpy(nodesList[i].iCards, pMyDdzBoard->iPlaArr[i], sizeof(pMyDdzBoard->iPlaArr[i]));
		nodesList[i].node = i;
		nodesList[i].iPlay = 0;
		nodesList[i].fUCB1 = 0;
		nodesList[i].fAverage = 0;
		nodesList[i].iWin = 0;
	}

	//时间PASS
	DWORD k = GetTickCount(); //获取毫秒级数目
	int iStartTime = k / 1000; // 开始时间以秒为单位
	//cout << iStartTime << endl;
	int iRunTime = 0;

	int BestUCb1 = 0;
	int bestNode = -1;
	iTotalPlay = 0;
	unsigned seed;
	seed = (unsigned)time(NULL);
	while (iRunTime<MaxRunTime)
	//while (iTotalPlay<1)
	{
		seed += 1;
		int i = 0;
		//判断是否每个节点都被访问过
		for (i = 0; i < iNodesCount; i++)
		{
			if (nodesList[i].iPlay == 0)
			{
				//TODEBUG
				bestNode = i;/*优先访问未被访问过的节点*/
				break;
			}
		}
		//取消：如果所有节点都被访问过，选择UCB1最高的节点
		//随机选择节点
		if (i == iNodesCount)
		{
			iRunTime = GetTickCount() / 1000 - iStartTime;
			if (iRunTime<=MaxRunTime/3)/*已运行时间不大于最大运行时间的三分之一时，随机挑选结点*/
			bestNode = GetRandomNumFromRange(0, iNodesCount,0);
			else{
				/*已运行时间大于最大运行时间的三分之一时，不再随机挑选结点*/
				bestNode = 0;
				for (i = 0; i < iNodesCount; i++)
				{
					if (nodesList[i].fUCB1 >= nodesList[bestNode].fUCB1)
					{
						bestNode = i;
					}
				}
			}
		}

		int winner = RunMonteCarloSimulation(bestNode);
		iTotalPlay++;
		UpdateUCB1(nodesList, bestNode, winner, iTotalPlay);
		iRunTime = GetTickCount() / 1000 - iStartTime;

	}
	//cout << "模拟出牌已运行" << iRunTime << "秒" << endl;
	//cout << "共模拟" << iTotalPlay << "次" << endl;
	//cout << "AIWIN:" << AIWin << " ,UPWIN:" << UpWin << " DownWIN:" << DownWin << endl;
	bestNode = 0;
	for (int j = 0; j < iNodesCount; j++)
	{
		//cout << "node " << j << "average=" << nodesList[j].fAverage <<
		//	"= " << nodesList[j].iWin << "/" << nodesList[j].iPlay <<
		//	" UCB1=" << nodesList[j].fUCB1 << endl;
		if (nodesList[j].fAverage >= nodesList[bestNode].fAverage)
			bestNode = j;
	}
	//cout << "choose:" << bestNode << endl;
	memcpy(ibestMove,nodesList[bestNode].iCards,sizeof(ibestMove));
	//ofstream SaveFile("data.txt", ios::app);
	//int i, j;
	//j = 0;
	//SaveFile << "本次手牌： " ;
	//for (j = 0; pDdz->iOnHand[j] != -1; j++)
	//	SaveFile << pDdz->iOnHand[j] << ',';
	//SaveFile << endl;

	//SaveFile << "本次出牌节点：" << endl;
	//for (i = 0; i<iNodesCount; i++)
	//{
	//	for (j = 0; nodesList[i].iCards[j] != -1; j++)
	//		SaveFile << nodesList[i].iCards[j] << ',';
	//	SaveFile << endl;
	//}
	//SaveFile << "本次出牌：" ;
	//for (j = 0; ibestMove[j] != -1; j++)
	//	SaveFile << ibestMove[j] << ',';
	//SaveFile << endl;
	//SaveFile << "模拟出牌已运行" << iRunTime << "秒" << endl;
	//SaveFile << "共模拟" << iTotalPlay << "次" << endl;
	//SaveFile << "AIWIN:" << AIWin << " ,UPWIN:" << UpWin << " DownWIN:" << DownWin << endl;
	//SaveFile << endl;
	//SaveFile.close();
	delete(nodesList);
	//ibestMove = nodesList[bestNode].iCards;
}

//针对AI节点进行蒙特卡洛模拟
//AI赢返回1，否则返回0
int MonteCarloSearchEngine::RunMonteCarloSimulation(int node)
{
	if (iTotalPlay>0)
		//初始化模拟界面
		Init();
	//cout << " node:" << node << "in " << iNodesCount << endl;
	Update(node, 0);

	int iDepth = 1;
	//if (iTotalPlay>=1)
	//GetOtherPlayerCards(pUpDdzBoard->iOnHand, pDownDdzBoard->iOnHand);
	//初始化上家下家玩家信息
	//pUpDdzBoard->iLastOnTable = nodesList[node].iCards;
	//pDownDdzBoard->iLastOnTable = nodesList[node].iCards;
	int iIsGameOver = -1;
	unsigned seed;
	seed = (unsigned)time(NULL);
	while (iIsGameOver == -1)
	{
		seed += 10000;//改变seed
		//产生节点
		int thisNode;
		int nodesCount = MakeMove(iDepth);
		//选择随机节点出牌
		if (nodesCount > 1)
		{
			thisNode = GetRandomNumFromRange(0, nodesCount, iDepth);
		}
		else
		{
			thisNode = 0;

		}//更新局面信息
		Update(thisNode, iDepth);
		iDepth = iDepth + 1;
		iIsGameOver = IsGameOver();
	}
	return iIsGameOver;
}

// 计算并更新本节点局面模拟值
void MonteCarloSearchEngine::Update(int node, int iDepth)
{


	int iCardsCount;
	fDdz *thisBoard = pMyDdzBoard;
	switch (iDepth % 3)
	{
	case 1:thisBoard = pDownDdzBoard;
		break;
	case 2:thisBoard = pUpDdzBoard;
		break;
	}
	//ofstream SaveFile("SimData.txt", ios::app);
	//int i, j;
	//j = 0;

	/*SaveFile << "第" << iTotalPlay << "次模拟"  ;
	SaveFile << "层:" << iDepth << " ";
	SaveFile << "手牌:";
	for (int i = 0; thisBoard->iOnHand[i] != -1; i++)
	{
		SaveFile << thisBoard->iOnHand[i] << ",";
	}
	SaveFile << endl;
	SaveFile << "节点:";*/
	//测试用//
	if (iDepth == 0 && iTotalPlay>0)
	{
		memcpy(thisBoard->iPlaArr[node], nodesList[node].iCards, sizeof(nodesList[node].iCards));
	}
	//for (int i = 0; thisBoard->iPlaArr[node][i] != -1; i++)
	//{
	//	SaveFile << thisBoard->iPlaArr[node][i] << ",";
	//}
	//SaveFile << endl;
	//SaveFile.close();

	//如果是pass
	if (thisBoard->iPlaArr[node][0] <= -1)
	{
		thisBoard->iLastPassCount++;
		iCardsCount = 0;
		if (thisBoard->iLastPassCount >= 2)	//连续两家PASS
		{
			thisBoard->iLastPassCount = 0;
			thisBoard->iLastTypeCount = 0;
			thisBoard->iLastMainPoint = -1;
			memset(iLastValidCards, -1, sizeof(iLastValidCards));
			thisBoard->iLastOnTable = iLastValidCards;
		}
		//else什么也不改变
	}
	else
	{
		//计算牌张数
		//for (iCardsCount = 0; thisBoard->iPlaArr[node][iCardsCount] != -1; iCardsCount++);
		//更新thisBoard数据
		//假设iplaArr是有序的
		//TODO：测试代码
		auto my_lambda_func = [](int *iOnHand, int elem)
		{
			int i = 0, j = 0;
			for (j = 0; j<21; j++)
			{

				if (iOnHand[j] == elem)
					i++;
				else
				{
					iOnHand[j - i] = iOnHand[j];
				}
			}
		};
		for (int n = 0; thisBoard->iPlaArr[node][n] != -1; n++)
		{
			my_lambda_func(thisBoard->iOnHand, thisBoard->iPlaArr[node][n]);
		}
		thisBoard->iLastPassCount = 0;
		thisBoard->iLastMainPoint = pCinfo->AnalyzeMainPoint(thisBoard->iPlaArr[node]);
		thisBoard->iLastTypeCount = pCinfo->AnalyzeTypeCount(thisBoard->iPlaArr[node]);
		memcpy(iLastValidCards, thisBoard->iPlaArr[node], sizeof(thisBoard->iPlaArr[node]));
		thisBoard->iLastOnTable = iLastValidCards;
	}
	if (thisBoard == pMyDdzBoard)
	{
		pUpDdzBoard->iLastPassCount = thisBoard->iLastPassCount;
		pUpDdzBoard->iLastOnTable = iLastValidCards;
		pUpDdzBoard->iLastTypeCount = thisBoard->iLastTypeCount;
		pUpDdzBoard->iLastMainPoint = thisBoard->iLastMainPoint;
		pDownDdzBoard->iLastPassCount = thisBoard->iLastPassCount;
		pDownDdzBoard->iLastOnTable = iLastValidCards;
		pDownDdzBoard->iLastTypeCount = thisBoard->iLastTypeCount;
		pDownDdzBoard->iLastMainPoint = thisBoard->iLastMainPoint;
	}
	if (thisBoard == pUpDdzBoard)
	{
		pMyDdzBoard->iLastPassCount = thisBoard->iLastPassCount;
		pMyDdzBoard->iLastOnTable = iLastValidCards;
		pMyDdzBoard->iLastTypeCount = thisBoard->iLastTypeCount;
		pMyDdzBoard->iLastMainPoint = thisBoard->iLastMainPoint;
		pDownDdzBoard->iLastPassCount = thisBoard->iLastPassCount;
		pDownDdzBoard->iLastOnTable = iLastValidCards;
		pDownDdzBoard->iLastTypeCount = thisBoard->iLastTypeCount;
		pDownDdzBoard->iLastMainPoint = thisBoard->iLastMainPoint;
	}

	if (thisBoard == pDownDdzBoard)
	{
		pUpDdzBoard->iLastPassCount = thisBoard->iLastPassCount;
		pUpDdzBoard->iLastOnTable = iLastValidCards;
		pUpDdzBoard->iLastTypeCount = thisBoard->iLastTypeCount;
		pUpDdzBoard->iLastMainPoint = thisBoard->iLastMainPoint;
		pMyDdzBoard->iLastPassCount = thisBoard->iLastPassCount;
		pMyDdzBoard->iLastOnTable = iLastValidCards;
		pMyDdzBoard->iLastTypeCount = thisBoard->iLastTypeCount;
		pMyDdzBoard->iLastMainPoint = thisBoard->iLastMainPoint;
	}
	iCardsCount = pCinfo->CountCards(thisBoard->iOnHand);
	if (thisBoard == pMyDdzBoard)
	{
		pUpDdzBoard->iDownPlayerCards = iCardsCount;
		pDownDdzBoard->iUpPlayerCards = iCardsCount;
	}
	if (thisBoard == pUpDdzBoard)
	{
		pDownDdzBoard->iDownPlayerCards = iCardsCount;
		pMyDdzBoard->iUpPlayerCards = iCardsCount;
	}
	if (thisBoard == pDownDdzBoard)
	{
		pUpDdzBoard->iUpPlayerCards = iCardsCount;
		pMyDdzBoard->iDownPlayerCards = iCardsCount;
	}

}




void MonteCarloSearchEngine::UpdateUCB1(MTCNode nodes[], int node, int Winner, int iTotalPlay)
{

	nodes[node].iPlay++;
	if (Winner == 1)
		nodes[node].iWin++;
	if (iTotalPlay == 0)
	{
		nodes[node].fUCB1 = 0;
		return;
	}
	nodes[node].fAverage = (float)nodes[node].iWin / (float)nodes[node].iPlay;
	//更新所有节点的FUCB1
	for (int i = 0; i < iNodesCount; i++)
	{
		if (nodes[i].iPlay == 0)
			break;
		float fUCB1 = 2 * log(iTotalPlay);
		fUCB1 = fUCB1 / nodes[i].iPlay;
		fUCB1 = sqrtf(fUCB1);
		nodes[i].fUCB1 = nodes[i].fAverage + fUCB1;
	}
}



int MonteCarloSearchEngine::MakeMove(int iDepth)
{
	int iCount = 0;
	fDdz *thisBoard = pMyDdzBoard;
	switch (iDepth % 3)
	{
	case 1:thisBoard = pDownDdzBoard;
		break;
	case 2:thisBoard = pUpDdzBoard;
		break;
	}
	//pRecomCards->InitPlaGenerate(thisBoard);
	iCount = pRecomCards->HelpPla(thisBoard,1);
	//iDepthPos[iDepth + 1] = iDepthPos[iDepth] + iCount;
	//AddDepthNodesInList(iDepth);
	return iCount;
}



//UNDONE
void MonteCarloSearchEngine::GetOtherPlayerCards(int* iUpPlayerCards, int* iDownPlayerCards)
{
	//int iOnUpHand[21] = { 1, 2, 3, 9, 11, 12, 14, 16, 19, 20, 24, 25, 32, 41, 44, 52, 53, -1 };
	//memcpy(pUpDdzBoard->iOnHand, iOnUpHand, sizeof(iOnUpHand));
	//int iOnDownHand[21] = { 6, 7, 10, 15, 21, 27, 31, 33, 34, 35, 37, 38, 47, 48, 49, 50, 51, -1 };
	//memcpy(pDownDdzBoard->iOnHand, iOnDownHand, sizeof(iOnDownHand));
	for (int i = 0; i < 21; i++)
	{
		iUpPlayerCards[i] = -1;
		iDownPlayerCards[i] = -1;
	}
	int count = pRealDdz->iUpPlayerCards + pRealDdz->iDownPlayerCards;
	std::vector<int> vi(count); // 定义一个大小为N的vector
	//将其他玩家手中牌放入一个数组
	for (int i = 0; i<count; ++i)
	{
		vi[i] = pRealDdz->iOnOtherHand[i];
	}
	//随机种子
	//srand((unsigned)time(NULL));
	//将整个数组随机排序
	std::random_shuffle(vi.begin(), vi.end());

	int i = 0;
	//生成己方上一手玩家的随机牌
	for (vector<int>::iterator it = vi.begin(); it != vi.begin() + pRealDdz->iUpPlayerCards; it++)
	{

		iUpPlayerCards[i] = *it;
		i++;
	}

	iUpPlayerCards[i] = -1;
	i = 0;
	pCinfo->SortById(iUpPlayerCards);
	//生成另一个非己方玩家的随机牌
	i = 0;
	for (vector<int>::iterator it = vi.begin() + pRealDdz->iUpPlayerCards; it != vi.end(); it++)
	{
		iDownPlayerCards[i] = *it;
		i++;
	}

	iDownPlayerCards[i] = -1;
	pCinfo->SortById(iDownPlayerCards);
}
//判断牌局是否结束,并返回极值
int MonteCarloSearchEngine::IsGameOver()
{
	//我方牌出尽
	if (pMyDdzBoard->iOnHand[0] == -1)
	{
		AIWin++;
		return 1;
	}
	else if (pUpDdzBoard->iOnHand[0] == -1)
	{
		UpWin++;
		//如果是地主，地主赢（AI农民输）
		if (pUpDdzBoard->cDir == pUpDdzBoard->cLandlord)
			return 0;
		//如果不是地主，且AI是地主（AI地主输）
		else if (pMyDdzBoard->cDir == pMyDdzBoard->cLandlord)
			return 0;
		//如果不是地主,且AI不是地主（AI农民赢）
		else if (pMyDdzBoard->cDir != pMyDdzBoard->cLandlord)
			return 1;
	}
	else if (pDownDdzBoard->iOnHand[0] == -1)
	{
		DownWin ++;
		//如果是地主，地主赢
		if (pDownDdzBoard->cDir == pDownDdzBoard->cLandlord)
			return 0;
		//如果不是地主，且AI是地主
		else if (pMyDdzBoard->cDir == pMyDdzBoard->cLandlord)
			return 0;
		//如果不是地主,且AI不是地主
		else if (pMyDdzBoard->cDir != pMyDdzBoard->cLandlord)
			return 1;
	}
	return -1;
}



void MonteCarloSearchEngine::InitBoard()
{
}


// 左开右闭
int MonteCarloSearchEngine::GetRandomNumFromRange(int iStart, int iEnd,int iDepth)
{
	std::vector<int> nodes(iEnd); // 定义一个大小为N的vector
	for (int i = 0; i < iEnd; i++)
	{
		nodes[i] = iStart;
		iStart++;
	}
	std::random_shuffle(nodes.begin(), nodes.end());//迭代器
	int i;
	if (iDepth==0)
	{
		nodeSeed++;
		if (nodeSeed >= iEnd)
			nodeSeed = 0;
			i = nodeSeed;
	}
	else
	{
		 i = nodes[iEnd - 1];
	}

	int thisNode = nodes[i];
	return thisNode;
}
