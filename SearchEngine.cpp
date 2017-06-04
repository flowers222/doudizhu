#include "SearchEngine.h"


SearchEngine::SearchEngine() :pMoveGen(RecommendCards::Instance()), pCInfo(CardsInfo::Instance())
{

}


SearchEngine::~SearchEngine()
{
}

//��������ӿ�
//��ʼ��ģ����Ϣ
void SearchEngine::SearchAGoodMove(Ddz* Ddz, int iDepth)
{
	pDdz = Ddz;//����д������

	iMaxDepth = iDepth;
	//������������ʼ��
	//pMoveGen->InitPlaGenerate(pDdz);
	//ģ�������������
	//pMoveGen->RandomHandCards(pDdz);
	//��ֵ��Ϣ��ģ������
	int iCount;
	//���㱾����������
	iCount = pCInfo->CountCards(pDdz->iOnHand);
	//TODO��������������ƵĲ²�����������䣬���漰���ʼ���
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

	//��ʼ����ʷ����û���
	//ResetHistoryTable();
	//InitializeHashTable();
	//PVS_HH_TT(iMaxDepth,-200000, 200000);

	//��ʼ����������������ģ����Ƶ�����
	iPlaCount = 0;
	//��ֵ����
	pValue = new CardsValue();
	//��������
	Alpha_Beta_half(iMaxDepth, -2000000, 2000000);
	//����������ɾ����ֵ����
	delete(pValue);
}

//�ж��ƾ��Ƿ����,�����ؼ�ֵ
int SearchEngine::IsGameOver(int iDepth)
{
	//�ҷ��Ƴ���
	if (iOnHand[2][0] == -1)
		return 19990 + iDepth;
	else if (iOnHand[0][0] == -1 )
	{
		//�ϼ��ǵ���,������Ӯ�����ǵ������ϼ���ũ��ũ��Ӯ
		if (pDdz->cLastPlay == pDdz->cLandlord || pDdz->cDir == pDdz->cLandlord)
			return -19990 - iDepth;
		//�¼��ǵ���
		if (pDdz->cDir != pDdz->cLandlord&&pDdz->cLastPlay != pDdz->cLandlord)
			return 19990 + iDepth;
	}
	else if (iOnHand[1][0] == -1)
	{
		//���ǵ���
		if (pDdz->cDir == pDdz->cLandlord)
			return -19990 - iDepth;
		//�¼��ǵ���
		if (pDdz->cLastPlay != pDdz->cLandlord && pDdz->cDir != pDdz->cLandlord) //�Һ��ϼҶ����ǵ���������Ϊũ��
			return -19990 - iDepth;
		//�ϼ��ǵ���
		if (pDdz->cLastPlay)
			return 19990 +iDepth;

	}
	return 0;
}


//�ж������Ƽ����Ƶ��Ƿ񼺷�,Ӧ�ø��������жϳ�������Ƿ�Ϊ�Լ�
//�жϳ��Ʒ������ò���������������Ϊ��Ӧ���Ʒ�����
//����޶��ߣ�������
//����޸�ʱ�䣺2016/4/19
//TODO���������������飬ӦΪSetMoveGenCards
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

	pMoveGen->pCardsOnHand = pDdz->iRandomOnHand[1];//������Ǽ�����Ҳ������һ����������һ����
}

//���ɵ�ǰ��ȫ�����ܳ���,��������pDdz->iplaArr��
//�޸�2016713 ������
//���ӣ�����ģ���������
int SearchEngine::MakeMove(int iDepth)
{
	char cPlayer=ThisDepthPlayer(iDepth);

	IfHelpMyself(pDdz, cPlayer);
	//iPlaCount=pMoveGen->HelpPla(pDdz);//�Ƽ����Ʊ�����pDdz->iplaArr����,���Ŀǰ��ģ�����ɵĽ�����
	layerCardsNum[iDepth] = pDdz->iPlaCount;//���浱ǰ����н�����
	cLastPlay = cPlayer;
	return 0;
}


//ֻ��Alpha��֦�������㷨
//�����ֵ�㷨�У�ũ��Ϊ����ڵ㣬����Ϊ��С�ڵ�
int SearchEngine::Alpha_Beta_half(int iDepth, int iAlpha, int iBeta)
{
	//��Ϊ�����˲��ģ�ֻ����ũ1-����-ũ2�����������ʱ�����л���ʹ��alpha��֦
	//����Ҫ����6����ܼ�֦һ��
	int iValue;
	/*�ϼ��¼Һͱ���������������iOnHand�е��±�*/
	int depthLastPlayer;//��λ��������±�
	int depthThisPlayer;
	int depthNextPlayer;
	//int *pResult = NULL;
	int iResult = -99999;
	int iMax = -1;//ѡ����Ƶ��±�
	char cPlayer = NULL;
	if (iDepth!=0)
	 cPlayer = ThisDepthPlayer(iDepth);//��ǰ������˷�λ���

	bool isMaxNode=false;

	/*���ݵ�ǰ�������������ϼ��¼�������������iOnHand�е��±�*/
	if (cPlayer == pDdz->cDir)
	{
		depthLastPlayer = 0;
		depthThisPlayer = 2;
		depthNextPlayer = 1;
	}
	else if (cPlayer == pDdz->cLastPlay)
	{
		depthLastPlayer = 1;//Ϊ������λ
		depthThisPlayer = 0;
		depthNextPlayer = 2;
	}
	else
	{
		depthLastPlayer = 2;//�洢��������iOnHand����ľ�����Ϣ���±�
		depthThisPlayer = 1;
		depthNextPlayer = 0;
	}


	//�Ƿ���Ϸ����
	if (iDepth > 0)
	{
		iValue = IsGameOver(iDepth);
		if (iValue != 0)
			return iValue;//�ѷ�ʤ�������ؼ�ֵ
	}
	//�����Ҷ�ӽڵ�
	if (iDepth <= 0)
	{
		//�ж�һЩ�����ֵ�����Ĳ���
		int *pOnTable=NULL;
		if (pDdz->iLastPassCount == 2)
			pOnTable = NULL;
		if (pDdz->iLastPassCount == 1)
			pOnTable = iOnTable[1];
		if (pDdz->iLastPassCount == 0)
			pOnTable = iOnTable[2];

		iValue = Evaluation(iOnHand[depthLastPlayer], iOnHand[depthThisPlayer], iOnHand[depthNextPlayer], pOnTable, cPlayer, isMaxNode);//����Ĳ����ߴ�5��������Ҫ�Ż�����ӿڲ���
		return iValue;//��ֵ������ݶԴ���ĳ���������
	}

    /*���ɵ�ǰ���ܽڵ㣬���洢��ȫ�ֱ���pDdz->iplaArr�У�������ڵ�ĳ�ʼλ�úͽ���λ��*/
	isMaxNode = IsMaxNode(cPlayer);
	MakeMove(iDepth);
	int start = iPlaCount - layerCardsNum[iDepth]; //���ɵ�ǰ����ȫ���ڵ�,��������ʼλ��
	int end = iPlaCount;//ģ����ƴ洢��pDdz->iplaArr�����ʼ�ͽ�βλ��

	/*��ʽ��ALPHA-BETA�㷨*/
	//iAlpha��ʼֵΪ������С��iBeta��ʼֵΪ�������

		//��Ҫ��������һ������Ƿ��ǵ���
		//����ǣ����ǻᷢ����֦�����

			/*����ÿһ���ڵ�*/
	for (int i = start; i < end; i++)
	{
		//iAlpha����MAX�ڵ㵱ǰ�ŷ���ѣ�iBeta����MIN�ڵ㵱ǰ�ŷ����
		int *iCards = &pDdz->iPlaArr[i][0];
		UpdatePosition(iCards, iDepth, 1);//���¾��棬�Ѽ������Ƶ��ƿ�Ӿ����г�ȥ
		iValue = Alpha_Beta_half(iDepth - 1, iAlpha, iBeta);
		UpdatePosition(iCards, iDepth, 0);//�ָ����棬�Ѽ������Ƶ��ƿ�Ӿ����г�ȥ
		//���ڼ���ڵ�
		if (isMaxNode)
		{

			if (iValue  > iAlpha)
			{
				//ȡ����ֵ
				iAlpha = iValue;
				if (iDepth == iMaxDepth)
				{
					//pResult = pDdz->iPlaArr[i];//��������߷�
					iMax = i;//��¼����߷���λ��
					iChoose = iMax;
				}
				//ֻ�е�ǰ����ũ��ڵ㣨MAX�ڵ㣩�����ң�ũ����¼��ǵ���ʱ�Żᷢ��alpha��֦
				if (ThisDepthPlayer(iDepth - 1) == pDdz->cLandlord)
				{
					if (iAlpha >= iBeta)
						return iBeta;
				}
			}

		}
		//���ڼ�С�ڵ�
		else
		{
			if (iValue  <iBeta)
				//ȡ��Сֵ
				iBeta = iValue;
			if (iDepth == iMaxDepth)
			{
				//pResult = pDdz->iPlaArr[i];//��������߷�
				iMax = i;//��¼����߷���λ��
				iChoose = iMax;
			}
		}

	}
	//ѭ������
	if (isMaxNode)
		return iAlpha;
	else
		return iBeta;


}
// PVS+�û���+��ʷ�����㷨
int SearchEngine::PVS_HH_TT(int iDepth, int iAlpha, int iBeta)
{
	int iScore,s;
	int depthLastPlayer;//������λ��������±�
	int depthThisPlayer;
	int depthNextPlayer;
	int *pResult=NULL;
	int iResult=-99999;
	int iMax = -1;//ѡ����Ƶ��±�
	int a, b, t;
	char cPlayer = ThisDepthPlayer(iDepth);
	bool isMaxNode;
	//���޸ģ���ֵ����������Ϊ�����ߵ���ݶ����Ĺ�ֵ���棬���ԣ�ֻ��Ҫ�жϵ�ǰ�������Ϊũ���ǵ���
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
		depthLastPlayer = 1;//Ϊ������λ
		depthThisPlayer = 0;
		depthNextPlayer = 2;
	}
	 else
	{
		isMaxNode = 1;
	depthLastPlayer = 2;//�洢������������ľ�����Ϣ�ı��
	depthThisPlayer = 1;
	depthNextPlayer = 0;
	}

	if (iDepth > 0)
	{
		s = IsGameOver(iDepth);
		if (s != 0)
			return s;//�ѷ�ʤ�������ؼ�ֵ
	}


	//UNDO��Ҷ�ӽڵ�Ĵ���������
		if (iDepth <= 0)
		{
			//�ж�һЩ�����ֵ�����Ĳ���
			int *pOnTable=NULL;
			if (pDdz->iLastPassCount == 2)
				pOnTable = NULL;
			if (pDdz->iLastPassCount == 1)
				pOnTable = iOnTable[1];
			if (pDdz->iLastPassCount == 0)
				pOnTable = iOnTable[2];

			iScore = Evaluation(iOnHand[depthLastPlayer], iOnHand[depthThisPlayer], iOnHand[depthNextPlayer], pOnTable, cPlayer, isMaxNode);
			EnterHashTable(exact, iOnHand[depthLastPlayer], iOnHand[depthNextPlayer], iDepth, iOnHand[depthThisPlayer], iScore, pDdz->iPlaArr[iMax]);
			return iScore;//��ֵ������ݶԴ���ĳ���������
		}

		iScore = HashCheck(iOnHand[depthLastPlayer], iOnHand[depthNextPlayer], iAlpha, iBeta, iDepth, iOnHand[depthThisPlayer]);
	if (iScore != 99999)
		return iScore;
	MakeMove(iDepth);//���ɵ�ǰ����ȫ���ڵ�
	int start, end;//ģ����Ƶ���ʼ�ͽ�βλ��
	//ÿ�������ȫ���ڵ㶼�Ǵ����㿪ʼ�洢
	if (iDepth == iMaxDepth)
	{
		start = 0;
		end = layerCardsNum[5];
	}
	else
	{
		//ToDebug��ERROR
		start = layerCardsNum[iDepth + 1];
		end = layerCardsNum[iDepth];
	}
	HistorySortNode(pDdz->iPlaArr[start]);

	///-----------------�汾1-----------------//
	//for (int i=start; i<end; i++)
	//{
	//	UpdatePosition(pDdz->iPlaArr[i], 1);//���¾��棬�Ѽ������Ƶ��ƿ�Ӿ����г�ȥ
	//	if (i = start)//��һ���ڵ�ȫ��������
	//	{
	//		score = -PVS_HH_TT(iDepth - 1, -iBeta, -iAlpha);
	//		if (iDepth == iMaxDepth)//�������ڵ㱣������߷�
	//			pResult = pDdz->iPlaArr[i];
	//	}
	//	else//�ǵ�һ���ڵ�մ�̽������
	//	{

	//		score = -PVS_HH_TT(iDepth-1,-iAlpha-1,-iAlpha);
	//		if (score > iAlpha&&score < iBeta)//FAIL-HIGH ����ȫ��������
	//		{
	//			score = -PVS_HH_TT(iDepth-1,-iBeta,-score);
	//			if (iDepth == iMaxDepth)//UNCLEAR
	//			{
	//				//�������Žڵ�
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
	//			//�������Žڵ�
	//			iMax = i;
	//			int t;
	//			for ( t = 0; pDdz->iPlaArr[iMax][t] >= 0; t++)
	//				pDdz->iToTable[t] = pDdz->iPlaArr[iMax][t];
	//			pDdz->iToTable[t] = -1;
	//		}
	//		if (iAlpha >= iBeta)
	//		{
	//			pResult = pDdz->iPlaArr[i];//���淢����֦�����Žڵ�
	//			break;
	//		}//����beta��֦
	//	}
	//	if (iResult < score)//����
	//	{
	//		iResult = score;
	//		pResult = pDdz->iPlaArr[i];
	//	}
	//
	//	UpdatePosition(pDdz->iPlaArr[i], 0);

	//}
	//if (pResult != NULL)
	//{
	//	EnterHistroyScore(pResult, iDepth);//������ʷ�÷�
	//}
	//

	//return iAlpha;

	//---------------�汾2----------------//
	a = iAlpha;
	b = iBeta;
	int value_isexact=0;
	for (int i = start; i < end; i++)
	{
		UpdatePosition(pDdz->iPlaArr[i],iDepth, 1);//���¾��棬�Ѽ������Ƶ��ƿ�Ӿ����г�ȥ
		t = -PVS_HH_TT(iDepth - 1, -b, -a);
		if (t> iAlpha&&t < iBeta&&i>start)//�Ե�һ����Ľڵ㣬�����������FAIL-HIGH������ȫ��������
		{
			a = -PVS_HH_TT(iDepth - 1, -iBeta, -t);//��������
			value_isexact = 1;
			if (iDepth == iMaxDepth)
			{
				pResult = pDdz->iPlaArr[i];//��������߷�
				iMax = i;//��¼����߷���λ��
			}
		}
		UpdatePosition(pDdz->iPlaArr[i],iDepth, 0);//�ָ�����
		if (a < t)
		{//��һ����������
			a = t;
			value_isexact = 1;
			if (iDepth == iMaxDepth)
			{
				pResult = pDdz->iPlaArr[i]; //������ѽڵ�
			}
		}
		if (a >=iBeta)
		{
			//���±߽�����ϣ��
			EnterHashTable(Entry_type::lower_bound, iOnHand[depthLastPlayer], iOnHand[depthNextPlayer], iDepth, iOnHand[depthThisPlayer], a, pDdz->iPlaArr[iMax]);
			//����ǰ�߷�������ʷ��¼��
			EnterHistroyScore(pDdz->iPlaArr[i], iDepth);
			return a;//����beta��֦
		}
		b = a + 1;//�����µĿմ�
	}
	if (iMax != -1)
	{
		EnterHistroyScore(pResult, iDepth);//������ʷ�÷�,������߷�������ʷ��¼��
	}
	if (value_isexact)
	{
		EnterHashTable(Entry_type::exact, iOnHand[depthLastPlayer], iOnHand[depthNextPlayer], iDepth, iOnHand[depthThisPlayer], a, pDdz->iPlaArr[iMax]);
	}
	else
	{
		EnterHashTable(Entry_type::upper_bound, iOnHand[depthLastPlayer], iOnHand[depthNextPlayer], iDepth, iOnHand[depthThisPlayer], a, pDdz->iPlaArr[iMax]);
	}
	return a;//�������ֵ
}



//DEBUG:����ȫ����Ϣ��ʱ��û�п��ǶԷ�����PASS�����
//UNDO:û�и�����λ����������
void SearchEngine::UpdatePosition(int iCards[], int iDepth, bool ifTakeOut)
{
	int m;//��Ҫ�ı����ҷ�λ
	//��������ж���Ҫ�ı����ҷ�λ
	char cPlayer=ThisDepthPlayer(iDepth);
	if (cPlayer == pDdz->cDir)

		m = 2;
	else
		if (cPlayer == pDdz->cLastPlay)
			m = 0;
		else
			m = 1;
	int t = 0;
	if (ifTakeOut)//�Ƿ�ģ�����
	{

		//�޸����ģ�������Ϣ
		//int count = SCards->CountCards(iOnTable[1]);
		memcpy(iOnTable[0], iOnTable[1], sizeof(iOnTable[1]));
		memcpy(iOnTable[1], iOnTable[2], sizeof(iOnTable[2]));
		memcpy(iOnTable[2], iCards, sizeof(iCards));

		//�޸�ģ��������������Ϣ
		//DEBUG:�˴�������ҳ��������ѱ������
		for (int i = 0; iCards[t] > -1 &&i < 21; i++)
			{
				if (iOnHand[m][i] == iCards[t])
				{
					//�Ƴ�����
					for (int j = i; j<21; j++)
					{
						iOnHand[m][j] = iOnHand[m][j + 1];
					}
					t++;
				}

			}
		//�޸�ȫ�ֱ�����Ϣ
			if (iCards[0] == -3)	//Pass
			{
				pDdz->iLastPassCount++;
				if (pDdz->iLastPassCount >= 2)	//��������PASS
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
			//�޸ĳ�����ϢΪ������ҳ���
			//switch (m)
			//{
			//default:
			//	break;
			//}
		}
	//����ָ�
	else
	{

		//���ǻָ�
		memcpy(iOnTable[2], iOnTable[1], sizeof(iOnTable[2]));
		memcpy(iOnTable[1], iOnTable[0], sizeof(iOnTable[1]));
		//�޸�ȫ�ֱ�����Ϣ


			if (iOnTable[1][0] == -1&&iOnTable[2][0]==-3)	//����ǰ������������PASS
			{
				pDdz->iLastPassCount = 2;
				pDdz->iLastTypeCount = 0;
				pDdz->iLastMainPoint = -1;
			}


		else
		{
			//����ǰ�ϼ�PASS
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
		pCInfo->SortById(iOnHand[m]);//�������ټӽ�β��
		iOnHand[m][t] = -1;
	}




}

// ����ʷ�÷ֱ�����
void SearchEngine::ResetHistoryTable()
{
	memset(historyTable, 0, 3300 * sizeof(int));
}


//������ʷ�÷ֱ�
void SearchEngine::EnterHistroyScore(int iCards[], int iDepth)
{

	int i = GetHistroyPosition(iCards);
	historyTable[i] += iDepth << 2;
}


void SearchEngine::HistorySortNode(int *iCards)
{
	//����һ�������㷨��iCards[]����//
	//BubbleSort
	int i;
		int Temp[21];
		int *p = iCards;//ָ���ͷ
		int count = 0;
		int history1;
		int history2;
		for (i = 0; p[0] > 0; p++)
		{
			count++;//�����ж��ٸ��ڵ�
		}
		for (i = 0; i < 21; i++)
		{		//��ʼ��
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


// ��ȡ��ʷ������λ��
int SearchEngine::GetHistroyPosition(int* iCards)
{
	int count;
	int value = pCInfo->AnalyzeMainPoint(iCards);//������С����
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
//�û�����غ���///
/////////////////
hashTableNode SearchEngine::hashTable[20 * 15 * 13][20 * 15 * 13];
void SearchEngine::InitializeHashTable()
{

	int num = 20 * 15 * 13;//�����еĳ����������=��������*��ֵ����*��������
	//������ά����
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

//����˵����ǰ��λ��ҳ����ƿ飬��ȣ���׼������������е���
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
			case Entry_type::lower_bound://�±߽�
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
	return 99999;//û�����з�����Ч��ʶ
}

//������һ����Һ����ϸ���ҳ���һ���ƣ���������ϣֵ
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
		//���������
		if (iCards==NULL)
			_DEBUG_ERROR("iCardsδ��ֵ����");
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
			   iMainType = 0+1;//����

			else
				iMainType = 12+1;//�Ƿ�����
		}
		switch (i)
		{
		case 0:{hashKey1 = iCount*iMainPoint*iMainType; break; }
		case 1:{hashKey2 = iCount*iMainPoint*iMainType; break; }
		}

	}

	checkSum = CalculateCheckSum(iOnPlayer);

}

//��������е���ת����һ��ȷ����Ϣ
__int64 SearchEngine::CalculateCheckSum(int iCards[21])
{

	////////////////////////////////////////////////////////////////////////
	/////������ת���ɶ����ƣ��ٰѶ�����ת����ʮ����/////
	///////////////////////////////////////////////////////////////////////

	//תΪ������
	int allCards[54];//����һ����װ������������������,��ʼ��Ϊ0��ʾ�����ƶ�û��
	for (int i = 0; i < 54; i++)
	{		//��ʼ����������
		allCards[i] = 0;
	}
	int cards = 0;
	//�ų�icards[]�е���

	for (int i = 0; iCards[i] > -1; i++)
	{

		if (cards != iCards[i])
			cards = iCards[i];
		allCards[cards] = 1;
		cards++;
	}

	//תΪʮ����
	cards = 0;
	for (cards = 0; cards < 54; cards++)
	{
		checkSum += (allCards[cards]) * (1 << (54 - cards));
	}

	return checkSum;
}

//����ǰ������ҵĳ��ƣ���ȣ����������ʣ�ƣ���ѡ�����
//�����ϣ��
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


// ��ֵ��ӿ�
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
	//��������ж���Ҫģ����Ƶ����ĸ���ҡ�
		switch ((iMaxDepth - iDepth) % 3)
		{
		case 0:
			cPlayer = pDdz->cDir;
			break;
		case 1:
			cPlayer = 'n';//Ϊ��һ����ң�����֪�����巽λ�����ʲô��
			break;
		case 2:
			cPlayer = pDdz->cLastPlay;
			break;
		}
	}
		return cPlayer;

}




//��ϰMinMax�㷨������ũ���ȡ��ֵ��������ȡ��ֵ
int SearchEngine::MINMAX_thridPersonVersion(int iDepth)
{
	//д���Լ����ģ����������ʱ����Ի��ݵ���������㷨����ͷ������


	//int bestValue,//��ǰ����ù�ֵ
	//	value;
	//if (iDepth <= 0)
	//{
	//	//���뵱ǰ���棬����ʡ�Բ�������ע�͵�
	//	//����
	//	//return Evaluation();
	//}
	//else
	//{
	//	//�ж���Ϸ�Ƿ����
	//	int s = IsGameOver(iDepth);
	//	if (s != 0)
	//		return s;//�ѷ�ʤ�������ؼ�ֵ
	//}

	////�ж��Ƿ��Ǽ���㣬��ũ���
	//if (ũ���)
	//{
	//	bestValue=-INFINITY��
	//	//��ȡ�ӽڵ����
	//	foreach(���ܳ��Ʒ���m)
	//	{
	//		UpdatePosition();//�����ó��Ƴ��ƺ��Ӧ�ľ��棬����ǰ����ӽڵ�
	//		value = MINMAX_thridPersonVersion(iDepth-1);//�õ��þ����ֵ
	//		UpdatePosition();//�ָ�����Ϊû����ǰ��״̬
	//		bestValue = Max(value, bestValue);//��ǰ�����ֵȡ���ֵ


	//	}
	//	//�������������棬����bestValue��ʼֵΪINFINITY
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


