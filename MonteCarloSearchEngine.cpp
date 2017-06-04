#include "MonteCarloSearchEngine.h"
#include <random>
#include <iostream>

//�����޸ģ��û���λ��Ϣ���´���AI��Ӯ����������
MonteCarloSearchEngine::MonteCarloSearchEngine()
{
	
	srand((unsigned)time(NULL));//��ʼ��������ӣ�����
    pRecomCards = new RecommendCards();
	pMyDdzBoard = new fDdz();
	pUpDdzBoard = new fDdz();
	pDownDdzBoard = new fDdz();
	pCinfo = new CardsInfo();
	//��ʼ����������
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
	//����ڵ�
	delete(nodesList);
}
//��ʼ����������
void MonteCarloSearchEngine::Init()
{

	//��ʼ���̶�������Ϣ
	GetOtherPlayerCards(pUpDdzBoard->iOnHand,pDownDdzBoard->iOnHand);
	pMyDdzBoard->cDir = pRealDdz->cDir;
	pMyDdzBoard->cLandlord = pRealDdz->cLandlord;
	pMyDdzBoard->cLastPlay = pRealDdz->cLastPlay;
	//��ʼ����λ��Ϣ
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

	//��ʼ��ģ���ƾ���Ϣ
	pMyDdzBoard->iLastPassCount = pRealDdz->iLastPassCount;
	pMyDdzBoard->iLastTypeCount = pRealDdz->iLastTypeCount;
	pMyDdzBoard->iLastMainPoint = pRealDdz->iLastMainPoint;
	pMyDdzBoard->iUpPlayerCards = pRealDdz->iUpPlayerCards;
	pMyDdzBoard->iDownPlayerCards = pRealDdz->iDownPlayerCards;
	//��¼AI���һ����Ч������Ϣ
	memcpy(pMyDdzBoard->iOnHand, pRealDdz->iOnHand, sizeof(pRealDdz->iOnHand));
	int i;
	for (i = 0; pRealDdz->iOnTable[i][0] != -2; i++);
	if (pRealDdz->iLastPassCount == 1)
	{
		pMyDdzBoard->iLastOnTable = pRealDdz->iOnTable[i - 2];
	}
	else
	{
		//�����һ�γ���
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
	//����AI������
	int myCardsCount = pCinfo->CountCards(pRealDdz->iOnHand);

	//��ʼ���ϼ�
	pUpDdzBoard->iLastPassCount = pRealDdz->iLastPassCount;
	pUpDdzBoard->iLastTypeCount = pRealDdz->iLastTypeCount;
	pUpDdzBoard->iUpPlayerCards = pRealDdz->iDownPlayerCards;
	pUpDdzBoard->iLastMainPoint = pRealDdz->iLastMainPoint;
	pUpDdzBoard->iDownPlayerCards = myCardsCount;
	pUpDdzBoard->iLastOnTable = iLastValidCards;
	//��ʼ���¼�

	pDownDdzBoard->iLastPassCount = pRealDdz->iLastPassCount;
	pDownDdzBoard->iLastTypeCount = pRealDdz->iLastTypeCount;
	pDownDdzBoard->iUpPlayerCards = myCardsCount;
	pDownDdzBoard->iLastMainPoint = pRealDdz->iLastMainPoint;
	pDownDdzBoard->iDownPlayerCards = pRealDdz->iUpPlayerCards;
	pDownDdzBoard->iLastOnTable = iLastValidCards;


}


//��ʼ����
void MonteCarloSearchEngine::SearchAGoodMove(Ddz *pDdz)
{

	pRealDdz = pDdz;
	//��ʼ������ֵ
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
	//��ʼ���ڵ�
	for (int i = 0; pMyDdzBoard->iPlaArr[i][0] != -1; i++)
	{
		//memcpy(iPlaArr[i], pMyDdzBoard->iPlaArr[i], sizeof(pMyDdzBoard->iPlaArr[i]));
		//iPlaArr[i+1][0] = -1;
		//����ڵ�
		memcpy(nodesList[i].iCards, pMyDdzBoard->iPlaArr[i], sizeof(pMyDdzBoard->iPlaArr[i]));
		nodesList[i].node = i;
		nodesList[i].iPlay = 0;
		nodesList[i].fUCB1 = 0;
		nodesList[i].fAverage = 0;
		nodesList[i].iWin = 0;
	}

	//ʱ��PASS
	DWORD k = GetTickCount(); //��ȡ���뼶��Ŀ
	int iStartTime = k / 1000; // ��ʼʱ������Ϊ��λ
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
		//�ж��Ƿ�ÿ���ڵ㶼�����ʹ�
		for (i = 0; i < iNodesCount; i++)
		{
			if (nodesList[i].iPlay == 0)
			{
				//TODEBUG
				bestNode = i;/*���ȷ���δ�����ʹ��Ľڵ�*/
				break;
			}
		}
		//ȡ����������нڵ㶼�����ʹ���ѡ��UCB1��ߵĽڵ�
		//���ѡ��ڵ�
		if (i == iNodesCount)
		{
			iRunTime = GetTickCount() / 1000 - iStartTime;
			if (iRunTime<=MaxRunTime/3)/*������ʱ�䲻�����������ʱ�������֮һʱ�������ѡ���*/
			bestNode = GetRandomNumFromRange(0, iNodesCount,0);
			else{
				/*������ʱ������������ʱ�������֮һʱ�����������ѡ���*/
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
	//cout << "ģ�����������" << iRunTime << "��" << endl;
	//cout << "��ģ��" << iTotalPlay << "��" << endl;
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
	//SaveFile << "�������ƣ� " ;
	//for (j = 0; pDdz->iOnHand[j] != -1; j++)
	//	SaveFile << pDdz->iOnHand[j] << ',';
	//SaveFile << endl;

	//SaveFile << "���γ��ƽڵ㣺" << endl;
	//for (i = 0; i<iNodesCount; i++)
	//{
	//	for (j = 0; nodesList[i].iCards[j] != -1; j++)
	//		SaveFile << nodesList[i].iCards[j] << ',';
	//	SaveFile << endl;
	//}
	//SaveFile << "���γ��ƣ�" ;
	//for (j = 0; ibestMove[j] != -1; j++)
	//	SaveFile << ibestMove[j] << ',';
	//SaveFile << endl;
	//SaveFile << "ģ�����������" << iRunTime << "��" << endl;
	//SaveFile << "��ģ��" << iTotalPlay << "��" << endl;
	//SaveFile << "AIWIN:" << AIWin << " ,UPWIN:" << UpWin << " DownWIN:" << DownWin << endl;
	//SaveFile << endl;
	//SaveFile.close();
	delete(nodesList);
	//ibestMove = nodesList[bestNode].iCards;
}

//���AI�ڵ�������ؿ���ģ��
//AIӮ����1�����򷵻�0
int MonteCarloSearchEngine::RunMonteCarloSimulation(int node)
{
	if (iTotalPlay>0)
		//��ʼ��ģ�����
		Init();
	//cout << " node:" << node << "in " << iNodesCount << endl;
	Update(node, 0);

	int iDepth = 1;
	//if (iTotalPlay>=1)
	//GetOtherPlayerCards(pUpDdzBoard->iOnHand, pDownDdzBoard->iOnHand);
	//��ʼ���ϼ��¼������Ϣ
	//pUpDdzBoard->iLastOnTable = nodesList[node].iCards;
	//pDownDdzBoard->iLastOnTable = nodesList[node].iCards;
	int iIsGameOver = -1;
	unsigned seed;
	seed = (unsigned)time(NULL);
	while (iIsGameOver == -1)
	{
		seed += 10000;//�ı�seed
		//�����ڵ�
		int thisNode;
		int nodesCount = MakeMove(iDepth);
		//ѡ������ڵ����
		if (nodesCount > 1)
		{
			thisNode = GetRandomNumFromRange(0, nodesCount, iDepth);
		}
		else
		{
			thisNode = 0;

		}//���¾�����Ϣ
		Update(thisNode, iDepth);
		iDepth = iDepth + 1;
		iIsGameOver = IsGameOver();
	}
	return iIsGameOver;
}

// ���㲢���±��ڵ����ģ��ֵ
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

	/*SaveFile << "��" << iTotalPlay << "��ģ��"  ;
	SaveFile << "��:" << iDepth << " ";
	SaveFile << "����:";
	for (int i = 0; thisBoard->iOnHand[i] != -1; i++)
	{
		SaveFile << thisBoard->iOnHand[i] << ",";
	}
	SaveFile << endl;
	SaveFile << "�ڵ�:";*/
	//������//
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

	//�����pass
	if (thisBoard->iPlaArr[node][0] <= -1)
	{
		thisBoard->iLastPassCount++;
		iCardsCount = 0;
		if (thisBoard->iLastPassCount >= 2)	//��������PASS
		{
			thisBoard->iLastPassCount = 0;
			thisBoard->iLastTypeCount = 0;
			thisBoard->iLastMainPoint = -1;
			memset(iLastValidCards, -1, sizeof(iLastValidCards));
			thisBoard->iLastOnTable = iLastValidCards;
		}
		//elseʲôҲ���ı�
	}
	else
	{
		//����������
		//for (iCardsCount = 0; thisBoard->iPlaArr[node][iCardsCount] != -1; iCardsCount++);
		//����thisBoard����
		//����iplaArr�������
		//TODO�����Դ���
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
	//�������нڵ��FUCB1
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
	std::vector<int> vi(count); // ����һ����СΪN��vector
	//��������������Ʒ���һ������
	for (int i = 0; i<count; ++i)
	{
		vi[i] = pRealDdz->iOnOtherHand[i];
	}
	//�������
	//srand((unsigned)time(NULL));
	//�����������������
	std::random_shuffle(vi.begin(), vi.end());

	int i = 0;
	//���ɼ�����һ����ҵ������
	for (vector<int>::iterator it = vi.begin(); it != vi.begin() + pRealDdz->iUpPlayerCards; it++)
	{

		iUpPlayerCards[i] = *it;
		i++;
	}

	iUpPlayerCards[i] = -1;
	i = 0;
	pCinfo->SortById(iUpPlayerCards);
	//������һ���Ǽ�����ҵ������
	i = 0;
	for (vector<int>::iterator it = vi.begin() + pRealDdz->iUpPlayerCards; it != vi.end(); it++)
	{
		iDownPlayerCards[i] = *it;
		i++;
	}

	iDownPlayerCards[i] = -1;
	pCinfo->SortById(iDownPlayerCards);
}
//�ж��ƾ��Ƿ����,�����ؼ�ֵ
int MonteCarloSearchEngine::IsGameOver()
{
	//�ҷ��Ƴ���
	if (pMyDdzBoard->iOnHand[0] == -1)
	{
		AIWin++;
		return 1;
	}
	else if (pUpDdzBoard->iOnHand[0] == -1)
	{
		UpWin++;
		//����ǵ���������Ӯ��AIũ���䣩
		if (pUpDdzBoard->cDir == pUpDdzBoard->cLandlord)
			return 0;
		//������ǵ�������AI�ǵ�����AI�����䣩
		else if (pMyDdzBoard->cDir == pMyDdzBoard->cLandlord)
			return 0;
		//������ǵ���,��AI���ǵ�����AIũ��Ӯ��
		else if (pMyDdzBoard->cDir != pMyDdzBoard->cLandlord)
			return 1;
	}
	else if (pDownDdzBoard->iOnHand[0] == -1)
	{
		DownWin ++;
		//����ǵ���������Ӯ
		if (pDownDdzBoard->cDir == pDownDdzBoard->cLandlord)
			return 0;
		//������ǵ�������AI�ǵ���
		else if (pMyDdzBoard->cDir == pMyDdzBoard->cLandlord)
			return 0;
		//������ǵ���,��AI���ǵ���
		else if (pMyDdzBoard->cDir != pMyDdzBoard->cLandlord)
			return 1;
	}
	return -1;
}



void MonteCarloSearchEngine::InitBoard()
{
}


// ���ұ�
int MonteCarloSearchEngine::GetRandomNumFromRange(int iStart, int iEnd,int iDepth)
{
	std::vector<int> nodes(iEnd); // ����һ����СΪN��vector
	for (int i = 0; i < iEnd; i++)
	{
		nodes[i] = iStart;
		iStart++;
	}
	std::random_shuffle(nodes.begin(), nodes.end());//������
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
