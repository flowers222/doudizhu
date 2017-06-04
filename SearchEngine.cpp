#include "SearchEngine.h"


SearchEngine::SearchEngine() :pMoveGen(RecommendCards::Instance()), pCInfo(CardsInfo::Instance())
{

}


SearchEngine::~SearchEngine()
{
}

//搜索引擎接口
//初始化模拟信息
void SearchEngine::SearchAGoodMove(Ddz* Ddz, int iDepth)
{
	pDdz = Ddz;//这样写可以吗？

	iMaxDepth = iDepth;
	//出牌生成器初始化
	//pMoveGen->InitPlaGenerate(pDdz);
	//模拟其他玩家手牌
	//pMoveGen->RandomHandCards(pDdz);
	//赋值信息到模拟数组
	int iCount;
	//计算本家手中牌数
	iCount = pCInfo->CountCards(pDdz->iOnHand);
	//TODO：对其他玩家手牌的猜测来自随机分配，不涉及概率计算
	memcpy(iOnHand[2], pDdz->iOnHand, sizeof(pDdz->iOnHand));
	iCount = pCInfo->CountCards(pDdz->iRandomOnHand[0]);
	memcpy(iOnHand[0], pDdz->iRandomOnHand[0], sizeof(pDdz->iRandomOnHand[0]));
	iCount = pCInfo->CountCards(pDdz->iRandomOnHand[1]);
	memcpy(iOnHand[1], pDdz->iRandomOnHand[1], sizeof(pDdz->iRandomOnHand[1]));
	int t = pDdz->iOTmax;
	t--;
	if (t > 0)
	{
		iCount = pCInfo->CountCards(iOnTable[t]);
		memcpy(iOnTable[2], pDdz->iOnTable[t], iCount * 4);
	}
	t--;
	if (t > 0)
	{
		iCount = pCInfo->CountCards(iOnTable[t]);
		memcpy(iOnTable[1], pDdz->iOnTable[t], iCount * 4);
	}
	t--;
	if (t >0)
	{
		iCount = pCInfo->CountCards(iOnTable[t]);
		memcpy(iOnTable[0], pDdz->iOnTable[t], iCount * 4);
	}

	//初始化历史表和置换表
	//ResetHistoryTable();
	//InitializeHashTable();
	//PVS_HH_TT(iMaxDepth,-200000, 200000);

	//初始化本次搜索产生的模拟出牌的数量
	iPlaCount = 0;
	//估值对象
	pValue = new CardsValue();
	//搜索函数
	Alpha_Beta_half(iMaxDepth, -2000000, 2000000);
	//搜索结束后删除估值对象
	delete(pValue);
}

//判断牌局是否结束,并返回极值
int SearchEngine::IsGameOver(int iDepth)
{
	//我方牌出尽
	if (iOnHand[2][0] == -1)
		return 19990 + iDepth;
	else if (iOnHand[0][0] == -1 )
	{
		//上家是地主,地主方赢；我是地主，上家是农民，农民赢
		if (pDdz->cLastPlay == pDdz->cLandlord || pDdz->cDir == pDdz->cLandlord)
			return -19990 - iDepth;
		//下家是地主
		if (pDdz->cDir != pDdz->cLandlord&&pDdz->cLastPlay != pDdz->cLandlord)
			return 19990 + iDepth;
	}
	else if (iOnHand[1][0] == -1)
	{
		//我是地主
		if (pDdz->cDir == pDdz->cLandlord)
			return -19990 - iDepth;
		//下家是地主
		if (pDdz->cLastPlay != pDdz->cLandlord && pDdz->cDir != pDdz->cLandlord) //我和上家都不是地主，即都为农民
			return -19990 - iDepth;
		//上家是地主
		if (pDdz->cLastPlay)
			return 19990 +iDepth;

	}
	return 0;
}


//判断申请推荐出牌的是否己方,应该给出参数判断出牌玩家是否为自己
//判断出牌方后设置步法生成器所持牌为相应出牌方的牌
//最后修订者：范晓梦
//最后修改时间：2016/4/19
//TODO：函数名存在争议，应为SetMoveGenCards
void SearchEngine::IfHelpMyself(Ddz* pDdz, char cPlayer)
{
	if (cPlayer == pDdz->cDir)
	{
		pMoveGen->pCardsOnHand = pDdz->iOnHand;
		return;
	}
	if (cPlayer == pDdz->cLastPlay)
	{
		pMoveGen->pCardsOnHand = pDdz->iRandomOnHand[0];
		return;
	}

	pMoveGen->pCardsOnHand = pDdz->iRandomOnHand[1];//如果不是己方，也不是上一方，则是下一方。
}

//生成当前层全部可能出牌,并保存至pDdz->iplaArr内
//修改2016713 范晓梦
//增加：返回模拟出牌数量
int SearchEngine::MakeMove(int iDepth)
{
	char cPlayer=ThisDepthPlayer(iDepth);

	IfHelpMyself(pDdz, cPlayer);
	//iPlaCount=pMoveGen->HelpPla(pDdz);//推荐出牌保存在pDdz->iplaArr里面,获得目前已模拟生成的解数量
	layerCardsNum[iDepth] = pDdz->iPlaCount;//保存当前层可行解数量
	cLastPlay = cPlayer;
	return 0;
}


//只有Alpha剪枝的搜索算法
//假设估值算法中，农民为极大节点，地主为极小节点
int SearchEngine::Alpha_Beta_half(int iDepth, int iAlpha, int iBeta)
{
	//因为是三人博弈，只有在农1-地主-农2出牌情况出现时，才有机会使用alpha剪枝
	//至少要搜索6层才能剪枝一次
	int iValue;
	/*上家下家和本家手中牌在数组iOnHand中的下标*/
	int depthLastPlayer;//上位出牌玩家下标
	int depthThisPlayer;
	int depthNextPlayer;
	//int *pResult = NULL;
	int iResult = -99999;
	int iMax = -1;//选择出牌的下标
	char cPlayer = NULL;
	if (iDepth!=0)
	 cPlayer = ThisDepthPlayer(iDepth);//当前层出牌人方位编号

	bool isMaxNode=false;

	/*根据当前层出牌人算出当上家下家手中牌在数组iOnHand中的下标*/
	if (cPlayer == pDdz->cDir)
	{
		depthLastPlayer = 0;
		depthThisPlayer = 2;
		depthNextPlayer = 1;
	}
	else if (cPlayer == pDdz->cLastPlay)
	{
		depthLastPlayer = 1;//为本家下位
		depthThisPlayer = 0;
		depthNextPlayer = 2;
	}
	else
	{
		depthLastPlayer = 2;//存储在搜索类iOnHand里面的局面信息的下标
		depthThisPlayer = 1;
		depthNextPlayer = 0;
	}


	//是否游戏结束
	if (iDepth > 0)
	{
		iValue = IsGameOver(iDepth);
		if (iValue != 0)
			return iValue;//已分胜负，返回极值
	}
	//如果是叶子节点
	if (iDepth <= 0)
	{
		//判断一些传入估值函数的参数
		int *pOnTable=NULL;
		if (pDdz->iLastPassCount == 2)
			pOnTable = NULL;
		if (pDdz->iLastPassCount == 1)
			pOnTable = iOnTable[1];
		if (pDdz->iLastPassCount == 0)
			pOnTable = iOnTable[2];

		iValue = Evaluation(iOnHand[depthLastPlayer], iOnHand[depthThisPlayer], iOnHand[depthNextPlayer], pOnTable, cPlayer, isMaxNode);//传入的参数高达5个，我需要优化这个接口才行
		return iValue;//估值必须根据对传入的出牌者敏感
	}

    /*生成当前可能节点，并存储在全局变量pDdz->iplaArr中，并保存节点的初始位置和结束位置*/
	isMaxNode = IsMaxNode(cPlayer);
	MakeMove(iDepth);
	int start = iPlaCount - layerCardsNum[iDepth]; //生成当前局面全部节点,并保存起始位置
	int end = iPlaCount;//模拟出牌存储在pDdz->iplaArr里的起始和结尾位置

	/*正式的ALPHA-BETA算法*/
	//iAlpha初始值为负无穷小，iBeta初始值为正无穷大

		//需要分析出下一个玩家是否是地主
		//如果是，则是会发生剪枝的情况

			/*对于每一个节点*/
	for (int i = start; i < end; i++)
	{
		//iAlpha代表MAX节点当前着法最佳，iBeta代表MIN节点当前着法最佳
		int *iCards = &pDdz->iPlaArr[i][0];
		UpdatePosition(iCards, iDepth, 1);//更新局面，把即将出牌的牌块从局面中除去
		iValue = Alpha_Beta_half(iDepth - 1, iAlpha, iBeta);
		UpdatePosition(iCards, iDepth, 0);//恢复局面，把即将出牌的牌块从局面中除去
		//对于极大节点
		if (isMaxNode)
		{

			if (iValue  > iAlpha)
			{
				//取极大值
				iAlpha = iValue;
				if (iDepth == iMaxDepth)
				{
					//pResult = pDdz->iPlaArr[i];//保留最佳走法
					iMax = i;//记录最佳走法的位置
					iChoose = iMax;
				}
				//只有当前层是农民节点（MAX节点），并且，农民层下家是地主时才会发生alpha剪枝
				if (ThisDepthPlayer(iDepth - 1) == pDdz->cLandlord)
				{
					if (iAlpha >= iBeta)
						return iBeta;
				}
			}

		}
		//对于极小节点
		else
		{
			if (iValue  <iBeta)
				//取极小值
				iBeta = iValue;
			if (iDepth == iMaxDepth)
			{
				//pResult = pDdz->iPlaArr[i];//保留最佳走法
				iMax = i;//记录最佳走法的位置
				iChoose = iMax;
			}
		}

	}
	//循环结束
	if (isMaxNode)
		return iAlpha;
	else
		return iBeta;


}
// PVS+置换表+历史启发算法
int SearchEngine::PVS_HH_TT(int iDepth, int iAlpha, int iBeta)
{
	int iScore,s;
	int depthLastPlayer;//本层上位出牌玩家下标
	int depthThisPlayer;
	int depthNextPlayer;
	int *pResult=NULL;
	int iResult=-99999;
	int iMax = -1;//选择出牌的下标
	int a, b, t;
	char cPlayer = ThisDepthPlayer(iDepth);
	bool isMaxNode;
	//待修改，估值函数不会因为出牌者的身份而更改估值局面，所以，只需要判断当前出牌玩家为农民还是地主
	 if (cPlayer == pDdz->cDir)
	{
		isMaxNode = 1;
		depthLastPlayer = 0;
		depthThisPlayer = 2;
		depthNextPlayer = 1;
	}
	 else if (cPlayer == pDdz->cLastPlay)
	{
		isMaxNode = false;
		depthLastPlayer = 1;//为本家下位
		depthThisPlayer = 0;
		depthNextPlayer = 2;
	}
	 else
	{
		isMaxNode = 1;
	depthLastPlayer = 2;//存储在搜索类里面的局面信息的编号
	depthThisPlayer = 1;
	depthNextPlayer = 0;
	}

	if (iDepth > 0)
	{
		s = IsGameOver(iDepth);
		if (s != 0)
			return s;//已分胜负，返回极值
	}


	//UNDO：叶子节点的处理还不够好
		if (iDepth <= 0)
		{
			//判断一些传入估值函数的参数
			int *pOnTable=NULL;
			if (pDdz->iLastPassCount == 2)
				pOnTable = NULL;
			if (pDdz->iLastPassCount == 1)
				pOnTable = iOnTable[1];
			if (pDdz->iLastPassCount == 0)
				pOnTable = iOnTable[2];

			iScore = Evaluation(iOnHand[depthLastPlayer], iOnHand[depthThisPlayer], iOnHand[depthNextPlayer], pOnTable, cPlayer, isMaxNode);
			EnterHashTable(exact, iOnHand[depthLastPlayer], iOnHand[depthNextPlayer], iDepth, iOnHand[depthThisPlayer], iScore, pDdz->iPlaArr[iMax]);
			return iScore;//估值必须根据对传入的出牌者敏感
		}

		iScore = HashCheck(iOnHand[depthLastPlayer], iOnHand[depthNextPlayer], iAlpha, iBeta, iDepth, iOnHand[depthThisPlayer]);
	if (iScore != 99999)
		return iScore;
	MakeMove(iDepth);//生成当前局面全部节点
	int start, end;//模拟出牌的起始和结尾位置
	//每个局面的全部节点都是从最大层开始存储
	if (iDepth == iMaxDepth)
	{
		start = 0;
		end = layerCardsNum[5];
	}
	else
	{
		//ToDebug：ERROR
		start = layerCardsNum[iDepth + 1];
		end = layerCardsNum[iDepth];
	}
	HistorySortNode(pDdz->iPlaArr[start]);

	///-----------------版本1-----------------//
	//for (int i=start; i<end; i++)
	//{
	//	UpdatePosition(pDdz->iPlaArr[i], 1);//更新局面，把即将出牌的牌块从局面中除去
	//	if (i = start)//第一个节点全窗口搜索
	//	{
	//		score = -PVS_HH_TT(iDepth - 1, -iBeta, -iAlpha);
	//		if (iDepth == iMaxDepth)//靠近根节点保留最佳走法
	//			pResult = pDdz->iPlaArr[i];
	//	}
	//	else//非第一个节点空窗探测搜索
	//	{

	//		score = -PVS_HH_TT(iDepth-1,-iAlpha-1,-iAlpha);
	//		if (score > iAlpha&&score < iBeta)//FAIL-HIGH 重新全窗口搜索
	//		{
	//			score = -PVS_HH_TT(iDepth-1,-iBeta,-score);
	//			if (iDepth == iMaxDepth)//UNCLEAR
	//			{
	//				//保存最优节点
	//				iMax = i;
	//				int t;
	//				for (t = 0; pDdz->iPlaArr[iMax][t] >= 0; t++)
	//					pDdz->iToTable[t] = pDdz->iPlaArr[iMax][t];
	//				pDdz->iToTable[t] = -1;
	//			}
	//		}
	//	}
	//	if (score > iAlpha)
	//	{
	//		iAlpha = score;
	//		if (iDepth == iMaxDepth )//UNCLEAR
	//		{
	//			//保存最优节点
	//			iMax = i;
	//			int t;
	//			for ( t = 0; pDdz->iPlaArr[iMax][t] >= 0; t++)
	//				pDdz->iToTable[t] = pDdz->iPlaArr[iMax][t];
	//			pDdz->iToTable[t] = -1;
	//		}
	//		if (iAlpha >= iBeta)
	//		{
	//			pResult = pDdz->iPlaArr[i];//保存发生剪枝的最优节点
	//			break;
	//		}//发生beta剪枝
	//	}
	//	if (iResult < score)//命中
	//	{
	//		iResult = score;
	//		pResult = pDdz->iPlaArr[i];
	//	}
	//
	//	UpdatePosition(pDdz->iPlaArr[i], 0);

	//}
	//if (pResult != NULL)
	//{
	//	EnterHistroyScore(pResult, iDepth);//更新历史得分
	//}
	//

	//return iAlpha;

	//---------------版本2----------------//
	a = iAlpha;
	b = iBeta;
	int value_isexact=0;
	for (int i = start; i < end; i++)
	{
		UpdatePosition(pDdz->iPlaArr[i],iDepth, 1);//更新局面，把即将出牌的牌块从局面中除去
		t = -PVS_HH_TT(iDepth - 1, -b, -a);
		if (t> iAlpha&&t < iBeta&&i>start)//对第一个后的节点，如果上面搜索FAIL-HIGH，重新全窗口搜索
		{
			a = -PVS_HH_TT(iDepth - 1, -iBeta, -t);//重新搜索
			value_isexact = 1;
			if (iDepth == iMaxDepth)
			{
				pResult = pDdz->iPlaArr[i];//保留最佳走法
				iMax = i;//记录最佳走法的位置
			}
		}
		UpdatePosition(pDdz->iPlaArr[i],iDepth, 0);//恢复局面
		if (a < t)
		{//第一次搜索命中
			a = t;
			value_isexact = 1;
			if (iDepth == iMaxDepth)
			{
				pResult = pDdz->iPlaArr[i]; //保留最佳节点
			}
		}
		if (a >=iBeta)
		{
			//将下边界存入哈希表
			EnterHashTable(Entry_type::lower_bound, iOnHand[depthLastPlayer], iOnHand[depthNextPlayer], iDepth, iOnHand[depthThisPlayer], a, pDdz->iPlaArr[iMax]);
			//将当前走法汇入历史记录表
			EnterHistroyScore(pDdz->iPlaArr[i], iDepth);
			return a;//发生beta剪枝
		}
		b = a + 1;//设置新的空窗
	}
	if (iMax != -1)
	{
		EnterHistroyScore(pResult, iDepth);//更新历史得分,将最佳走法汇入历史记录表
	}
	if (value_isexact)
	{
		EnterHashTable(Entry_type::exact, iOnHand[depthLastPlayer], iOnHand[depthNextPlayer], iDepth, iOnHand[depthThisPlayer], a, pDdz->iPlaArr[iMax]);
	}
	else
	{
		EnterHashTable(Entry_type::upper_bound, iOnHand[depthLastPlayer], iOnHand[depthNextPlayer], iDepth, iOnHand[depthThisPlayer], a, pDdz->iPlaArr[iMax]);
	}
	return a;//返回最佳值
}



//DEBUG:更新全局信息的时候没有考虑对方可能PASS的情况
//UNDO:没有更新上位出牌玩家情况
void SearchEngine::UpdatePosition(int iCards[], int iDepth, bool ifTakeOut)
{
	int m;//需要改变的玩家方位
	//根据深度判断需要改变的玩家方位
	char cPlayer=ThisDepthPlayer(iDepth);
	if (cPlayer == pDdz->cDir)

		m = 2;
	else
		if (cPlayer == pDdz->cLastPlay)
			m = 0;
		else
			m = 1;
	int t = 0;
	if (ifTakeOut)//是否模拟出牌
	{

		//修改玩家模拟出牌信息
		//int count = SCards->CountCards(iOnTable[1]);
		memcpy(iOnTable[0], iOnTable[1], sizeof(iOnTable[1]));
		memcpy(iOnTable[1], iOnTable[2], sizeof(iOnTable[2]));
		memcpy(iOnTable[2], iCards, sizeof(iCards));

		//修改模拟出牌玩家手牌信息
		//DEBUG:此处依赖玩家出的手牌已被排序好
		for (int i = 0; iCards[t] > -1 &&i < 21; i++)
			{
				if (iOnHand[m][i] == iCards[t])
				{
					//移除该牌
					for (int j = i; j<21; j++)
					{
						iOnHand[m][j] = iOnHand[m][j + 1];
					}
					t++;
				}

			}
		//修改全局变量信息
			if (iCards[0] == -3)	//Pass
			{
				pDdz->iLastPassCount++;
				if (pDdz->iLastPassCount >= 2)	//连续两家PASS
				{
					pDdz->iLastPassCount = 0;
					pDdz->iLastTypeCount = 0;
					pDdz->iLastMainPoint = -1;
				}
				else
				{
					pDdz->iLastPassCount = 1;
					pDdz->iLastTypeCount = pCInfo->AnalyzeTypeCount(iOnTable[1]);
					pDdz->iLastMainPoint = pCInfo->AnalyzeMainPoint(iOnTable[1]);
				}

			}
			else
			{
				pDdz->iLastPassCount = 0;
				pDdz->iLastTypeCount = pCInfo->AnalyzeTypeCount(iCards);
				pDdz->iLastMainPoint = pCInfo->AnalyzeMainPoint(iCards);
			}
			pDdz->iOTmax++;
			//修改出牌信息为本次玩家出牌
			//switch (m)
			//{
			//default:
			//	break;
			//}
		}
	//否则恢复
	else
	{

		//覆盖恢复
		memcpy(iOnTable[2], iOnTable[1], sizeof(iOnTable[2]));
		memcpy(iOnTable[1], iOnTable[0], sizeof(iOnTable[1]));
		//修改全局变量信息


			if (iOnTable[1][0] == -1&&iOnTable[2][0]==-3)	//出牌前经历连续两家PASS
			{
				pDdz->iLastPassCount = 2;
				pDdz->iLastTypeCount = 0;
				pDdz->iLastMainPoint = -1;
			}


		else
		{
			//出牌前上家PASS
			if (iOnTable[2][0] = -3)
			{
				pDdz->iLastPassCount=1;
				pDdz->iLastTypeCount = pCInfo->AnalyzeTypeCount(iOnTable[1]);
				pDdz->iLastMainPoint = pCInfo->AnalyzeMainPoint(iOnTable[1]);
			}
			else
			{
				pDdz->iLastPassCount = 0;
				pDdz->iLastTypeCount = pCInfo->AnalyzeTypeCount(iOnTable[2]);
				pDdz->iLastMainPoint = pCInfo->AnalyzeMainPoint(iOnTable[2]);
			}

		}
			pDdz->iOTmax--;
		for (t = 0; iOnHand[m][t] != -1; t++);
		for (int i= 0; iCards[i] != -1; i++)
		{
			iOnHand[m][t] = iCards[i];
			t++;
		}
		pCInfo->SortById(iOnHand[m]);//先排序，再加结尾。
		iOnHand[m][t] = -1;
	}




}

// 将历史得分表清零
void SearchEngine::ResetHistoryTable()
{
	memset(historyTable, 0, 3300 * sizeof(int));
}


//加入历史得分表
void SearchEngine::EnterHistroyScore(int iCards[], int iDepth)
{

	int i = GetHistroyPosition(iCards);
	historyTable[i] += iDepth << 2;
}


void SearchEngine::HistorySortNode(int *iCards)
{
	//根据一种排序算法将iCards[]排序//
	//BubbleSort
	int i;
		int Temp[21];
		int *p = iCards;//指向表头
		int count = 0;
		int history1;
		int history2;
		for (i = 0; p[0] > 0; p++)
		{
			count++;//计算有多少个节点
		}
		for (i = 0; i < 21; i++)
		{		//初始化
			Temp[i] = -1;
		}
		for (i = 0; i<=count; i++)
		{
			p = iCards;
			for (int j = 0; j<=count - i; j++)
			{

				history1 = GetHistroyPosition(p);
				p = p+1;
				history2 = GetHistroyPosition(p);
					if (historyTable[history2]>historyTable[history1])
				{
					int *p2 = p;
					p = p - 1;
					int count = pCInfo->CountCards(p);
					memcpy(Temp, p, count*4);
					count = pCInfo->CountCards(p2);
					memcpy(p, p2, count * 4);
					count = pCInfo->CountCards(Temp);
					memcpy(p2, Temp, count * 4);
				}
			}
		}

}


// 获取历史启发表位置
int SearchEngine::GetHistroyPosition(int* iCards)
{
	int count;
	int value = pCInfo->AnalyzeMainPoint(iCards);//牌套最小点数
	int type = pCInfo->AnalyzeTypeCount(iCards);
	if (type >0)
	{
		count = type % 100;
		type = type / 100 + 1;

	}
	else
	{

		count = pCInfo->CountCards(iCards);
		if (count != 0 && type == 0)
			type = 0 + 1;

		else
			type = 12 + 1;
	}
	return count*(type)*(value);
}



//////////////////
//置换表相关函数///
/////////////////
hashTableNode SearchEngine::hashTable[20 * 15 * 13][20 * 15 * 13];
void SearchEngine::InitializeHashTable()
{

	int num = 20 * 15 * 13;//可能有的出牌情况总数=牌张总数*牌值总数*牌型总数
	//建立二维数组
	//pHT = new hashTableNode*[num];
	for (int i = 0; i < num; i++)
	{
		//pHT[i] = new hashTableNode[num];

		for (int j = 0; j < num; j++)
		{
		hashTable[i][j].depth = -1;
		hashTable[i][j].pokersBlockInfo = -1;
		hashTable[i][j].value = -1;
		}


}


}

//参数说明：前两位玩家出牌牌块，深度，和准备出牌玩家手中的牌
int SearchEngine::HashCheck( int* LastPlayCards, int* theOtherPlayCards,int iAlpha,int iBeta, int iDepth, int iCards[21])
{
	CalculateHashKey(LastPlayCards, theOtherPlayCards, iCards);
	hashTableNode *p;
	p = &hashTable[hashKey1][hashKey2];
	if (p != NULL)
	{

		if (iDepth <= p->depth&&checkSum == p->checkSum)
		{
			switch (p->entryType)
			{
			case Entry_type::exact:
				return (p->value);
			case Entry_type::lower_bound://下边界
				if (p->value >= iBeta)
					return (p->value);
				else
					break;
			case Entry_type::upper_bound:
				if (p->value <= iAlpha)
					return (p->value);
				else
					break;
			}
		}
	}
	return 99999;//没有命中返回无效标识
}

//传入上一个玩家和上上个玩家出的一副牌，分析出哈希值
void SearchEngine::CalculateHashKey(int* LastPlayerCards, int* theOtherPlayerCards, int iOnPlayer[21])
{
	int* iCards=NULL;
	int iCount;
	int iMainPoint;
	int iMainType;
	for (int i = 0; i<2; i++)
	{

		switch (i)
		{
		case 0:{iCards = LastPlayerCards; break; }
		case 1:{iCards = theOtherPlayerCards; break; }
		}
		//算出牌张数
		if (iCards==NULL)
			_DEBUG_ERROR("iCards未赋值！！");
		iMainPoint = pCInfo->AnalyzeMainPoint(iCards);

		int countType = pCInfo->AnalyzeTypeCount(iCards);
		if (countType >0)
		{
			iCount = countType % 100;
			iMainType = countType / 100+1;

		}
		else
		{

			iCount = pCInfo->CountCards(iCards);
			if (iCount!=0&&countType==0)
			   iMainType = 0+1;//单牌

			else
				iMainType = 12+1;//非法牌型
		}
		switch (i)
		{
		case 0:{hashKey1 = iCount*iMainPoint*iMainType; break; }
		case 1:{hashKey2 = iCount*iMainPoint*iMainType; break; }
		}

	}

	checkSum = CalculateCheckSum(iOnPlayer);

}

//将玩家手中的牌转化成一个确认信息
__int64 SearchEngine::CalculateCheckSum(int iCards[21])
{

	////////////////////////////////////////////////////////////////////////
	/////把牌面转换成二进制，再把二进制转化成十进制/////
	///////////////////////////////////////////////////////////////////////

	//转为二进制
	int allCards[54];//建立一个能装下所有牌面编码的数组,初始化为0表示所有牌都没有
	for (int i = 0; i < 54; i++)
	{		//初始化本局手牌
		allCards[i] = 0;
	}
	int cards = 0;
	//排除icards[]中的牌

	for (int i = 0; iCards[i] > -1; i++)
	{

		if (cards != iCards[i])
			cards = iCards[i];
		allCards[cards] = 1;
		cards++;
	}

	//转为十进制
	cards = 0;
	for (cards = 0; cards < 54; cards++)
	{
		checkSum += (allCards[cards]) * (1 << (54 - cards));
	}

	return checkSum;
}

//传入前两个玩家的出牌，深度，本玩家手中剩牌，和选择的牌
//加入哈希表
void SearchEngine::EnterHashTable(Entry_type entryType, int* LastPlayCards, int* theOtherPlayCards, int iDepth, int iOnPlayer[21], int value, int *ChooseCards)
{
	CalculateHashKey(LastPlayCards, theOtherPlayCards, iOnPlayer);
	hashTableNode *p;
	p = &hashTable[hashKey1][hashKey2];
	p->entryType = entryType;
	p->depth = iDepth;
	p->value = value;
	p->checkSum = checkSum;
	ichooseCards = pCInfo->AnalyzeTypeCount(ChooseCards);
	p->pokersBlockInfo = ichooseCards;
}


// 估值类接口
int SearchEngine::Evaluation(int* pLastPlayCards, int* pThisPlayerCards, int* pNextPlayCards, int *pOntable, char cThisPlayer,bool isMaxNode)
{

	pValue->InitCardsValueEngine(pLastPlayCards, pThisPlayerCards, pNextPlayCards, pOntable, cThisPlayer , isMaxNode,pDdz);
	int score =(int) pValue->CalSumValue();
	return score;

}


char SearchEngine::ThisDepthPlayer(int iDepth)
{
	char cPlayer;
	if (iDepth!=0)
	{
	//根据深度判断需要模拟出牌的是哪个玩家。
		switch ((iMaxDepth - iDepth) % 3)
		{
		case 0:
			cPlayer = pDdz->cDir;
			break;
		case 1:
			cPlayer = 'n';//为下一手玩家，不必知道具体方位编号是什么。
			break;
		case 2:
			cPlayer = pDdz->cLastPlay;
			break;
		}
	}
		return cPlayer;

}




//练习MinMax算法，假设农民层取正值，地主层取负值
int SearchEngine::MINMAX_thridPersonVersion(int iDepth)
{
	//写给自己看的，当不清楚的时候可以回溯到最基本的算法来让头脑清醒


	//int bestValue,//当前层最好估值
	//	value;
	//if (iDepth <= 0)
	//{
	//	//传入当前局面，这里省略参数所以注释掉
	//	//代码
	//	//return Evaluation();
	//}
	//else
	//{
	//	//判断游戏是否结束
	//	int s = IsGameOver(iDepth);
	//	if (s != 0)
	//		return s;//已分胜负，返回极值
	//}

	////判断是否是极大层，即农民层
	//if (农民层)
	//{
	//	bestValue=-INFINITY；
	//	//获取子节点分数
	//	foreach(可能出牌方法m)
	//	{
	//		UpdatePosition();//产生该出牌出牌后对应的局面，即当前层的子节点
	//		value = MINMAX_thridPersonVersion(iDepth-1);//得到该局面的值
	//		UpdatePosition();//恢复局面为没出牌前的状态
	//		bestValue = Max(value, bestValue);//当前层最佳值取最大值


	//	}
	//	//地主层类似上面，除了bestValue初始值为INFINITY
	//	//bestValue=Min(value,bestValue)
	//}
	//return bestValue;
	return 0;
}


bool SearchEngine::IsMaxNode(char cThisPlayer)
{
	if (cThisPlayer != pDdz->cLandlord)
		return false;
	else
		return true;
}


