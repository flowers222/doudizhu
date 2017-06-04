#include "RecommendCards.h"
//2016/8/7 16:19 �޶�iplacountֻ��AddinPLArr�ﱻ�޸ģ�Ӧ���ܽ�������ȵĳ��ƴ���
//������������ʼ��
//�Ż��˵��������жϣ������˳�ը�������
//���רΪģ�����ʹ�õĳ��ƺ���
int RecommendCards::InitPlaGenerate(fDdz* pDdz)
{
	int i, j;
	for (i = 0; i<PlaMax; i++)		//����Ƽ���������
		for (j = 0; j<21; j++)
			pDdz->iPlaArr[i][j] = -1;
	iPlaCount = 0;
	pDdz->iPlaCount = 0;
	iHandNum = MinHandNum(pDdz->iOnHand);
	isSim = false;
	iUpHandNum = MinHandNum(iUpCards);
	iDownHandNum = MinHandNum(iDownCards);
	ifNeedPass = false;//�Ƿ���Ҫ����PASS
	iLastMaxPoint = CInfo->AnalyzeMainMaxPoint(pDdz->iLastOnTable);
	return 0;
}

//����AI���ݵ�ǰ�ƾ����ɵ�ǰ�Ƽ�����
//��һ���޶�:����ģ����⼯������
//����޶���:�����Σ�����޶�ʱ��:16-08-8
//�ڶ����޸�����:���ӳ�ը����PASS�����

int RecommendCards::HelpPla(struct fDdz * pDdz, bool IsSim)
{
	int iPass[3] = { -2, -1, -1 };
	bool isPassed = false;
	InitPlaGenerate(pDdz);
	isSim = IsSim;
	//assert(pDdz->iOnHand[0]!=-1);
	int playerType = GetPlayerType(pDdz);

	//�Ƿ�ǰ������pass
	if (IfPassTwice(pDdz))
	{
		//���ǣ���Ҫ���³���
		//�������ƿɳ�
		Help0Pass(pDdz, playerType);
	}
	else
	{
		int iTypeCount = pDdz->iLastTypeCount;

		if (301 == iTypeCount)//����
		{
			if (IsSim)
				Help3SingleSim(pDdz);
			else
				Help3Single(pDdz);
		}
		else if (402 == iTypeCount)
		{
			if (IsSim)
				Help4DoubleSim(pDdz);
			else
				Help4Double(pDdz);
		}
		else if (503 == iTypeCount)
			Help5Three(pDdz);
		else if (604 == iTypeCount)
		{
			if (isSim)
				Help6ThreeOneSim(pDdz);
			else
				Help6ThreeOne(pDdz);
		}
		else if (605 == iTypeCount)
		{
			if (isSim)
				Help6ThreeDoubleSim(pDdz);
			else
				Help6ThreeDouble(pDdz);
		}
		else if (iTypeCount > 700 && iTypeCount < 800)
			Help7LinkSingle(pDdz);
		else if (iTypeCount > 800 && iTypeCount < 900)
			Help8LinkDouble(pDdz);
		else if (iTypeCount > 900 && iTypeCount < 1000)
			Help9LinkThree(pDdz);
		else if (iTypeCount > 1000 && iTypeCount < 1100)
		{
			if ((iTypeCount - 1000) % 4 == 0)
				Help10LinkThreeSingle(pDdz);
			else
				Help10LinkThreeDouble(pDdz);
		}
		else if (1106 == iTypeCount)
			Help11FourSingle(pDdz);
		else if (1108 == iTypeCount)
			Help11FourDouble(pDdz);
		//������ƿɳ�����ը��
		if (iPlaCount == 0)
		{
			if (iHandNum <= 9)
				Help2Bomb(pDdz);
		}
		//�����ҪPASS��û�нڵ㣬����
		if (ifNeedPass&&!isPassed)
		{
			if (pDdz->iPlaArr[iPlaCount][0] != -2)
			{
				AddInPlaArr(pDdz, iPass);
			}
		}
		if (iPlaCount == 0)
		{
			if (pDdz->iPlaArr[iPlaCount][0] != -2)
			{
				AddInPlaArr(pDdz, iPass);
			}
			isPassed = true;
		}

	}


	return iPlaCount;
}

RecommendCards::RecommendCards() : CInfo(CardsInfo::Instance())
{
	pSplitCards = new SplitCard();
	pMoldel = new PokerModelAnalysis();
}


RecommendCards::~RecommendCards()
{
	delete(pSplitCards);
	delete(pMoldel);
}


RecommendCards* RecommendCards::Instance()
{
	static RecommendCards instance;

	return &instance;
}

//���³��ƣ���iOnHand�в�ѯ���͵�iPlaArr
//����޶���:������,����޶�ʱ��:16-08-9 15:43
//�޸ģ��׳�ʱֻ��һ�����ͣ����׳�ʱ�����������Ϳ�����
//�޸ģ���ֹ���Ƴ�����ʱ��ӣ���֤���ƿɳ�
int RecommendCards::Help0Pass(fDdz * pDdz, int playerType)
{
	//DEBUG:���ʣ����ֻ���Ĵ�������ϣ����ƻ�ֻ������
	//���³���Ӧ��û�����ȼ�����ʤ�ʾ���
	//���Ƶ�ʱ�����ȳ�������С�Ƶ����
	////�ж��Ƿ��׳�//
	int cardsCount = CInfo->CountCards(pDdz->iOnHand);
	//����׳�
	if ((cardsCount == 17 && playerType != 1) || cardsCount == 20)
	{


		if (isSim)
			Help3SingleSim(pDdz);
		if (!isSim)
			Help3Single(pDdz);
		if (isSim)
			Help4DoubleSim(pDdz);
		if (!isSim)
			Help4Double(pDdz);

		Help5Three(pDdz);

		if (isSim)
			Help6ThreeOneSim(pDdz);
		if (!isSim)
			Help6ThreeOne(pDdz);

		if (isSim)
			Help6ThreeDoubleSim(pDdz);
		if (!isSim)
			Help6ThreeDouble(pDdz);
		Help7LinkSingle(pDdz);
		Help8LinkDouble(pDdz);
		Help9LinkThree(pDdz);
		Help10LinkThreeDouble(pDdz);
		Help10LinkThreeSingle(pDdz);
	}
	else
	{
		//����2:��N�����ͣ�ֱ���ڵ�����������

		if (isSim)
			Help4DoubleSim(pDdz);
		else
			Help4Double(pDdz);

		Help5Three(pDdz);

		if (isSim)
			Help6ThreeOneSim(pDdz);
		else
			Help6ThreeOne(pDdz);

		if (isSim)
			Help6ThreeDoubleSim(pDdz);
		else
			Help6ThreeDouble(pDdz);
		Help7LinkSingle(pDdz);//ͨ��
		Help8LinkDouble(pDdz);//ͨ������Ӧ�ü���˫˳ģ��������ʹ��������
		Help9LinkThree(pDdz);//ͨ��
		Help10LinkThreeSingle(pDdz);//ͨ��
		Help10LinkThreeDouble(pDdz);//ͨ��
		Help11FourSingle(pDdz);
		Help11FourDouble(pDdz);
		//�����¼ң�����������ϲ��ǵ���
		if (playerType == 3)
		{
			if (isSim)
				Help3SingleSim(pDdz);
			else
				Help3Single(pDdz);
		}
		//�����������
		if (playerType == 1)
		{
			if (pDdz->iUpPlayerCards != 1 && pDdz->iDownPlayerCards != 1)
			{
				//�������������ϲ��ǵ��ƣ��ȳ�����
				if (isSim)
					Help3SingleSim(pDdz);
				else
					Help3Single(pDdz);
			}
		}
		//����ǵ����ϼҳ���,����������ϲ��ǵ���,�ȳ�����
		if (playerType == 2)
		{
			if (pDdz->iDownPlayerCards > 1)
			{
				//����������ϲ��ǵ��ƣ��ȳ�����
				if (isSim)
					Help3SingleSim(pDdz);
				else
					Help3Single(pDdz);
			}
		}

		if (iPlaCount == 0)
		{
			if (playerType == 2 || playerType == 1)
			{
				Help3Single(pDdz);
			}
		}
		if (iPlaCount == 0 && cardsCount==4)
			Help2Bomb(pDdz);

		if (iPlaCount == 0&&cardsCount==2)
			Help1Rocket(pDdz);
	}
	//��ֹ���Ƴ�����ʱ��ӣ���֤���ƿɳ�
	if (iPlaCount == 0)
	{
		//Help3SingleSim(pDdz);
		int iTmpCards[50];
		int t = 0;
		for (int i = 0; pDdz->iOnHand[i] != -1; i++)
		{
			if (pDdz->iOnHand[i] < 48)
			{
				iTmpCards[t] = pDdz->iOnHand[i];
				t++;
				iTmpCards[t] = -2;
				t++;
			}
		}
		iTmpCards[t] = -1;
		AddInPlaArr(pDdz, iTmpCards);
	}

	return iPlaCount;
}
//H00-END

//H00-END

//H01-START��iOnHand�в�ѯ��ϻ�����͵�iPlaArr
//����޶���:�����Σ�����޶�ʱ��:2016-07-26
int RecommendCards::Help1Rocket(struct fDdz * pDdz)
{
	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 0);
	if (iCards[0]>-1)
		AddInPlaArr(pDdz, iCards);
	return iPlaCount;
}
//H01-END

//H02-START�Ƽ�����Ӧ��ը��
//����޶���:�����Σ�����޶�ʱ��:2016-07-26
int RecommendCards::Help2Bomb(struct fDdz * pDdz)
{
	int Cards = CInfo->CountCards(pDdz->iOnHand);
	if (!isSim&&Cards >= 17)
		return 0;
	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	int iSave[21];
	memset(iSave, -1, sizeof(iSave));
	if (GetLegalCards(pDdz, iCards, iSave))
	{
		AddInPlaArr(pDdz, iSave);
		return iPlaCount;
	}
	//����Ǹ���
	if (!IfPassTwice(pDdz))
	{
		ifNeedPass = true;
		Help1Rocket(pDdz);
	}
	return 0;

}
//H02-END


// - START��iOnHand�в�ѯ�������͵�iPlaArr
//����޶���:������,����޶�ʱ��:16-08-11
//�޶����ݼ�Ŀ��:�����ж�
//������������Ƶ����
//�Ż����Ƴ��Ʋ���
//�����޸�:������Ƴ�����2�����
int RecommendCards::Help3Single(struct fDdz * pDdz)
{
	//cout <<"Help3Single" << endl;
	//��ȡ�����������
	int iCopyOnHand[21];


	int iCount = iPlaCount;//���ƽ�
	//���ƺ���������������Ž�ȥ����Ϊ�˴���ǿ׳���Ǽ������ж����
	int theLastDan = -1;
	int iTmpCards[60];
	int t = 0;//iTmpCards�±�,���߸�����ʱ����
	int er[5] = { -1, -1, -1, -1, -1 };
	int dan2 = -1;
	int j = 0;
	int i = 0;
	int iHandCards = CInfo->CountCards(pDdz->iOnHand);
	int iTeamCards = -1;//����������
	bool ifPassTwice = IfPassTwice(pDdz);
	///���ֻʣһ���ƣ��ҺϷ���ֱ�ӳ�//
	if (pDdz->iOnHand[0] != -1 && pDdz->iOnHand[1] == -1)
	{
		iTmpCards[0] = pDdz->iOnHand[0];
		iTmpCards[1] = -2;
		iTmpCards[2] = -1;
		if (GetLegalCards(pDdz, iTmpCards))
			return iPlaCount-iCount;
	}
	/////////////////////////////////////
	//memset(iCopyOnHand, -1, sizeof(iCopyOnHand));
	bool Condition = AnalyzeCondition(pDdz);
	if (!Condition)
	{
		Help3SingleSim(pDdz);
		return iPlaCount - iCount;
	}

	memcpy(iCopyOnHand,pDdz->iOnHand,sizeof(iCopyOnHand));
	//�ų�ը��������¼ը������
	int BombPoint[4] = { -1, -1, -1, -1 };
	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	t = 0;
	for (i = 0; iCards[i] != -1; i++)
	{
		if (iCards[i] != -2)
		{
			DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
		else
		{
			BombPoint[t] = iCards[i - 1] / 4;
			t++;
		}
	}
	//�ų����
	for (i = 0; pDdz->iOnHand[i] != -1; i++);
	if (pDdz->iOnHand[i - 1] == 53 && pDdz->iOnHand[i - 2] == 52)
	{
		DeleteElementInCards(iCopyOnHand, 21, 53);
		DeleteElementInCards(iCopyOnHand, 21, 52);
	}
	/////////////////

	//�ų�2�������Ǽ�2
	t = 0;
	for (i = 0; pDdz->iOnHand[i] != -1; i++)
	{
			if (pDdz->iOnHand[i] >=48)
			{
				DeleteElementInCards(iCopyOnHand, 21, pDdz->iOnHand[i]);
			}
			//��¼2
			if (pDdz->iOnHand[i] >= 48 && pDdz->iOnHand[i] < 52)
			{
				t++;
				er[t] = pDdz->iOnHand[i];
			}
	}
	er[0] = t;


	int *iOnHand = iCopyOnHand;
	///�����ų�2������ը�Ĺ����Ϸ�����(����ͨ��)////
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 7);
	if (iCards[0] > -1)
	{
		i = CInfo->CountCards(pDdz->iOnHand);
		//�׳�������С��
		if (i >= 17 && pDdz->iLastMainPoint<0)
		{
			iCards[2] = -1;
		}
		if (GetLegalCards(pDdz, iCards))
		{
			return iPlaCount - iCount;
		}
	}

	//���û�кϷ����ƣ�Ҳ����ֻʣһ����
	//�ж��ǳ��ƻ��Ǹ���
	if (ifPassTwice)
	{
		//�������

		return iPlaCount - iCount;
	}
	else
	{

		//PASS/////////////////////////
		////�����1�����ϵķ�ը��2����2///
		if (er[0]>1 && er[0]<4)
		{
			iTmpCards[0] = er[1];
			iTmpCards[1] = -2;
			iTmpCards[2] = -1;
			if (GetLegalCards(pDdz, iTmpCards))
			{
				return iPlaCount - iCount;
			}
		}
		/////////////////////////////////


		//PASS//////////////////////////////
		//����˫˳�����Ķ�����,������Ʋ����浽iSave��
		//˫˳�������ڼ������Ƿ���˫˳�ڵ����

		//��˫˳
		iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 5);
		//���������˫˳
		int shunPoint[3] = { -1, -1, -1 };//����Ϊ������*100+������
		int ShunNum = 0;
		j = 0;
		for (t = 0; iCards[t] != -1; t++)
		{
			if (iCards[0] == -2)
				break;
			if (iCards[t] == -2)
			{
				shunPoint[ShunNum] = (j / 2) * 100 + iCards[t - 1] / 4;
				ShunNum++;

				j = 0;
			}
			else
				j++;
		}
		int nodesNum = 0;
		int danIn2shun[12];
		iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);
		//�����Ӹĳɵ�������
		t = 0;
		bool isIn = false;
		for (i = 0; iCards[i] != -1; i++)
		{
			isIn = false;
			if (iCards[0] <0)
				break;
			if (iCards[i] == -2)
			{
				int point = iCards[i - 1] / 4;
				//���������ĳ��˫˳������棬����
				for (j = 0; j < ShunNum; j++)
				{
					int maxPoint = shunPoint[j] % 100;
					int minPoint = maxPoint - shunPoint[j] / 100 + 1;
					if (point <= maxPoint&&point >= minPoint)
					{
						danIn2shun[t] = iCards[i - 1];
						t++;
						isIn = true;
						break;
					}
				}
				if (!isIn)
				{
					iTmpCards[0] = iCards[i - 1];
					iTmpCards[1] = -2;
					iTmpCards[2] = -1;
					if (GetLegalCards(pDdz, iTmpCards))
					{
						nodesNum++;
					}
				}
			}
		}

		danIn2shun[t] = -1;
		if (nodesNum> 0)
		{
			return iPlaCount - iCount;
		}
		////////////////////////////////

		///////PASS//////////////
		////���������ϵĵ�˳���Ÿ�֮////
		//Ŀǰ�õ�������Ƿ���ʱ�õ��ĵ�һ������һ�������������
		nodesNum = 0;
		int DanShunNum;
		int MaxNum;
		int DanInShun[4] = { -1 };
		//�ҵ��������������˳���浽iTmpCards��
		for (i = 5; i < 13; i++)
		{
			//��ѯ�ó��ȵ�˳��
			DanShunNum = AnalyzeSingleLink(i, -1,pDdz->iOnHand, iTmpCards);
			if (DanShunNum != i)
				break;
			else
			{
				MaxNum = DanShunNum;
			}
		}
		if (MaxNum >= 6)
		{
			AnalyzeSingleLink(MaxNum, -1, pDdz->iOnHand, iTmpCards);
			j = 0;
			//�õ���󳤶�˳��
			//��鶥���Ƿ���ը������,����ǣ���ǰһ�����
			//��鶥���Ƿ���˫˳����������ǣ���ǰһ�����
			for (i = MaxNum; i >=6; i--)
			{
				isIn = false;
				for (t = 0; BombPoint[t] != -1; t++)
				{
					if (iTmpCards[i - 1] / 4 == BombPoint[t])
					{
						isIn = true;
						break;
					}
				}
				for (t = 0; BombPoint[t] != -1; t++)
				{
					if (iTmpCards[i - 1] / 4 == danIn2shun[t]/4)
					{
						isIn = true;
						break;
					}
				}
				if (!isIn)
				{
					DanInShun[j] = iTmpCards[i - 1];
					j++;
					DanInShun[j] = -2;
					j++;
					DanInShun[j] = -1;
					if (GetLegalCards(pDdz, DanInShun))
					{
						nodesNum++;
					}
					break;
				}
			}
		}
		if (nodesNum>0)
		{
			return iPlaCount - iCount;
		}
		//�����������֮->DEBUG:��û�й����������Ƿ�����˳��
		//����˳��������ͬ���
		nodesNum = 0;
		memset(iTmpCards, -1, sizeof(iTmpCards));
		iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);
		t = 0;
		for (int i = 0; iCards[i] != -1; i++)
		{
			if (iCards[0] <0)
				break;
			if (iCards[i] == -2)
			{
				iTmpCards[t] = iCards[i - 1];
				t++;
				iTmpCards[t] = -2;
				t++;
			}
		}
		iTmpCards[t] = -1;
		if (GetLegalCards(pDdz, iTmpCards))
		{
			return iPlaCount-iCount;
		}

		/////PASS////////////
		////////////////////////////////
		///�����˳///
		memset(iTmpCards, -1, sizeof(iTmpCards));
			iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 4);
			t = 0;

			for (int i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] != -2)
				{
					isIn = false;
					for (j = 0; BombPoint[j] > -1; j++)
					{
						if (iCards[i] / 4 == BombPoint[j])
						{
							isIn = true;
							break;
						}
					}
					if (isIn)
						continue;
					//�ų�˫˳
					for (j = 0; danIn2shun[j] > -1; j++)
					{
						if (iCards[i] / 4 == danIn2shun[j] / 4)
						{
							isIn = true;
							break;
						}
					}
					if (isIn)
						continue;
					if (!isIn)
					{
						iTmpCards[t] = iCards[i];
						t++;
						iTmpCards[t] = -2;
						t++;
					}
				}
			}
			iTmpCards[t] = -1;
			if (GetLegalCards(pDdz, iTmpCards))
			{
				return iPlaCount - iCount;
			}
	    /////////////////

		////�����������ĺϷ�˫˳��////
		//�õ���˫˳�еĵ���
		//���¸�����Ҫ�жϸ������ƻ��Ǹ�������
			memset(iTmpCards,-1,sizeof(iTmpCards));
			if (danIn2shun[0] != -1)
		{
			t = 0;
			for (int i = 0; danIn2shun[i] != -1; i++)
			{
				iTmpCards[t] = danIn2shun[i];
				t++;
				iTmpCards[t] = -2;
				t++;
			}
			iTmpCards[t] = -1;
			if (GetLegalCards(pDdz, iTmpCards))
			{
				return iPlaCount - iCount;
			}
		}
		//�Ҳ����κοɳ�����
		if (iPlaCount - iCount == 0)
		{
			//////////////////////////////////////////
			//�����һ�ŵ�2��������Ʋ�����������С������2�������
			i = CInfo->CountCards(pDdz->iOnHand);
			if (er[0] == 1)
			{
				bool Condition = AnalyzeCondition(pDdz);
				if (!Condition || iHandNum <= 3)
				{
					iTmpCards[0] = er[1];
					iTmpCards[1] = -2;
					iTmpCards[2] = -1;
					if (GetLegalCards(pDdz, iTmpCards))
						return iPlaCount;
				}
			}
			/////////////////////////////////

			////�ҵ�����///
			iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 0);
			//���û�л��,�������ڻ���������ֻ���ǵ�����
			if (iCards[0] < 0)
			{
				bool condition = AnalyzeCondition(pDdz);
				for (t = 0; pDdz->iOnHand[t] != -1; t++);
				if (pDdz->iOnHand[t - 1] >= 52)
				{
					if (!condition || iHandNum <= 3)
					{
						iTmpCards[0] = pDdz->iOnHand[t - 1];
						iTmpCards[1] = -2;
						iTmpCards[2] = -1;
						if (GetLegalCards(pDdz, iTmpCards))
						{
							return iPlaCount - iCount;
						}
					}
				}
			}
			//�л�������ǳ�������С��,��PASS
			else
			{
				//��С��
				iTmpCards[0] = 52;
				iTmpCards[1] = -2;
				iTmpCards[2] = -1;
				GetLegalCards(pDdz, iTmpCards);
				//�������ը��
				Help2Bomb(pDdz);
				return iPlaCount - iCount;
			}
		}

	}
	return 0;
}
//H03-END

//��ѯ���в�ͬ�����������͵�iOnHand����
int RecommendCards::Help3SingleSim(fDdz* pDdz)
{
	int i;
	int iTmpCards[60] = { -1 };


	int count = iPlaCount;
	int point = -1;
	int t = 0;
	for (i = 0; pDdz->iOnHand[i] >= 0; i++)
	{
		if (t > 0)
		{
			if (pDdz->iOnHand[i] / 4 == point)
				continue;
		}
		iTmpCards[t] = pDdz->iOnHand[i];
		point = iTmpCards[t] / 4;
		t++;
		iTmpCards[t] = -2;
		t++;
	}
	iTmpCards[t] = -1;
	if (GetLegalCards(pDdz, iTmpCards))
	{
		return iPlaCount - count;
	}
	return 0;
}

///////PASS//////////
//H04-START�Ƽ�����Ӧ�Զ���
//����޶���:�����Σ�����޶�ʱ��:2016-07-26
int RecommendCards::Help4Double(struct fDdz * pDdz)
{
	//cout << "Help4Double" << endl;;
	//TODO:���������������<=����+���������ܺ�-2ʱ�������ӣ����򣬳�������
	//����˫˳�����Ķ�����
	//˫˳�������ڼ������Ƿ���˫˳�ڵ����
	int i, j, t;
	int iCopyOnHand[21];
	int iCount = iPlaCount;
	int * iCards;
	int iTmpCards[50];
	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));
	//�Ƿ����ֱ�ӳ�
	i = CInfo->CountCards(pDdz->iOnHand);
	if (i == 2)
	{
		iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 6);
		if (iCards[0] >= 0)
		{
			if (GetLegalCards(pDdz, iCards))
				return iPlaCount - iCount;
		}
	}
	bool  Condition = AnalyzeCondition(pDdz);
	if (!Condition)
	{
		Help4DoubleSim(pDdz);
		return iPlaCount - iCount;
	}
	//�ų�2����
	int condition = AnalyzeCondition(pDdz);
	if (pDdz->iLastMainPoint<11)
	{
		for (i = 48; i <= 53; i++)
		{
			DeleteElementInCards(iCopyOnHand, 21, i);
		}
	}
	//�ж�ը��,���ų�
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}
	//��˫˳
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 5);
	//�ų���˫˳����Ķ���
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}
	//�ų��ڵ�˳����Ķ���
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 4);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}

	//��������
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);
	if (GetLegalCards(pDdz, iCards))
	{
		return iPlaCount - iCount;
	}
	//������ƣ��˳�
	if (pDdz->iLastMainPoint < 0)
	{
		return 0;
	}

	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));
	//��������ų�2
	if (pDdz->iLastMainPoint<11)
	{
		for (i = 48; i <= 53; i++)
		{
			DeleteElementInCards(iCopyOnHand, 21, i);
		}
	}
	//�ж�ը��,���ų�
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}
	////����˳
	//iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 3);
	////�ų�����˳���������
	//if (iCards[0] > -1)
	//{
	//	for (i = 0; iCards[i] != -1; i++)
	//	{
	//		if (iCards[i] != -2)
	//			DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
	//	}
	//}
	//������
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);
	t = 0;
	if (iCards[0]>-1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] == -2)
			{
				iTmpCards[t] = iCards[i - 1];
				t++;
				iTmpCards[t] = iCards[i - 2];
				t++;
				iTmpCards[t] = -2;
				t++;
			}
		}
	}
	iTmpCards[t] = -1;
	if (iTmpCards[0] != -1)
	{
		if (GetLegalCards(pDdz, iTmpCards))
		{
			return iPlaCount - iCount;
		}
	}

	iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);
	if (GetLegalCards(pDdz, iCards))
	{
		return iPlaCount - iCount;
	}
	//�����Ȼû���ƣ�����ը��2
	int dui2[4] = { -1, -1, -1, -1 };
	t = 0;
	for (i = 0; pDdz->iOnHand[i] != -1; i++)
	{
		if (pDdz->iOnHand[i] / 4 == 12)
		{
			if (t == 3)
			{
				dui2[0] = -1;
				break;
			}
			dui2[t] = pDdz->iOnHand[i];
			t++;
		}
	}
	i = CInfo->CountCards(pDdz->iOnHand);
	//����ж�2,�����ڵ�һ�ֳ�
	if (dui2[1] != -1)
	{
		if (pDdz->cDir == pDdz->cLandlord&&i == 20)
		{
			return 0;
		}
		//����
		dui2[2] = -2;
		dui2[3] = -1;
		if (GetLegalCards(pDdz, dui2))
		{
			return iPlaCount - iCount;
		}
	}

	return iPlaCount - iCount;
}

/////PASS////
//�޹��������
int RecommendCards::Help4DoubleSim(struct fDdz * pDdz)
{
	int iTmpCards[60] = { -1 };
	int t = 0;
	int i;
	int point = -1;
	int iCount = iPlaCount;
	for (i = 0; pDdz->iOnHand[i + 1] >= 0; i++)
	{
		if (pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 1] / 4)
		{
			if (pDdz->iOnHand[i] / 4 != point)
			{
				iTmpCards[t] = pDdz->iOnHand[i];
				point = pDdz->iOnHand[i] / 4;
				t++;
				iTmpCards[t] = pDdz->iOnHand[i + 1];
				t++;
				iTmpCards[t] = -2;
				t++;

			}
		}
	}
	iTmpCards[t] = -1;
	if (GetLegalCards(pDdz, iTmpCards))
	{
		return iPlaCount - iCount;
	}
	return iPlaCount - iCount;
}
//H05-START�Ƽ�����Ӧ������
//����޶���:�����Σ�����޶�ʱ��:2016-07-26
int RecommendCards::Help5Three(struct fDdz * pDdz)
{
	int iCopyOnHand[21];
	int i;
	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));
	//�ж�ը��,���ų�
	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand,21, iCards[i]);
		}
	}
	//ȥ��2
	if (pDdz->iLastMainPoint < 11)
	{
		for (i = 48; i < 52; i++)
		{
			DeleteElementInCards(iCopyOnHand, 21, i);
		}
	}
	//ȥ����˳
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 3);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}

	iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);
	int iCount = iPlaCount;
	if (iCards[0] > -1)
	{
		if (GetLegalCards(pDdz, iCards))
		{
			return iPlaCount - iCount;
		}
	}

	//���û��
	if (pDdz->iLastMainPoint < 0)
		return 0;
	else
	{
		memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));
		//�ж�ը��,���ų�
		int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
		if (iCards[0] > -1)
		{
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] != -2)
					DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
			}
		}
		//ȥ��2
		if (pDdz->iLastMainPoint < 11)
		{
			for (i = 48; i < 52; i++)
			{
				DeleteElementInCards(iCopyOnHand, 21, i);
			}
		}
		iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 2);
		if (GetLegalCards(pDdz, iCards))
		{
			return iPlaCount - iCount;
		}
	}

	return 0;
}
//H05-END

//SplitCards����Ҫ�󣺲���ʱ���Ʒ���2��������
//H0601-START�Ƽ�����Ӧ������һ��
//����޶���:�����Σ�����޶�ʱ��:2016-08-10
//�Ż�������
int RecommendCards::Help6ThreeOne(struct fDdz * pDdz)
{
	int iTmpCards[60] = { -1 };
	int iCount = iPlaCount;
	//�ж����޺Ϸ�����,�����浽iSave1��

	int t = 0;
	int i = 0;
	int *iCards;
	t = CInfo->CountCards(pDdz->iOnHand);
	int j;
	//�������ֱ�ӳ�����ֱ�ӳ���ͨ����
	if (t == 4)
	{
		j = 0;
		iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 2);
		if (iCards[0] > -1)
		{
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] != -2)
				{
					iTmpCards[j] = iCards[i];
					j++;
				}
			}
			for (i = 0; pDdz->iOnHand[i] != -1; i++)
			{
				if (pDdz->iOnHand[i] / 4 != iTmpCards[0] / 4)
				{
					iTmpCards[j] = pDdz->iOnHand[i];
					j++;
					break;
				}
			}
			iTmpCards[j] = -2;
			j++;
			iTmpCards[j] = -1;
			if (GetLegalCards(pDdz, iTmpCards))
			{
				return iPlaCount - iCount;
			}
		}
	}
	///////////////////////
	int iCopyOnHand[21];
	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));
	//�ж�ը��,���ų�
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	//�ų�ը��
	for (i = 0; iCards[i] != -1; i++)
	{
		if (iCards[i] != -2)
		{
			DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}

	iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);
	//���û������
	if (iCards[0] < 0)
	{
		return 0;
	}
	//���������,����������iTmpCards(ͨ��)
	t = 0;
	int santiaoNum = 0;
	for (i = 0; iCards[i] != -1; i++)
	{
		if (iCards[0] == -2)
			break;
		if (iCards[i] == -2)
		{
			iTmpCards[t] = iCards[i - 3];
			t++;
			iTmpCards[t] = iCards[i - 2];
			t++;
			iTmpCards[t] = iCards[i - 1];
			t++;
			iTmpCards[t] = -2;
			t++;
			santiaoNum++;
		}
	}
	iTmpCards[t] = -1;
	if (santiaoNum)
	{
		int santiao[6];
		int danwang = -1;
		int er[4] = { -1, -1, -1, -1 };
		t = 0;
		j = 0;
		//�����ϵĵ����ͷ�ը��2����������(ͨ��)
		int iHandCards = CInfo->CountCards(pDdz->iOnHand);
		for (i = iHandCards; i > iHandCards - 7; i--)
		{
			if (52 <= pDdz->iOnHand[i] && 53 >= pDdz->iOnHand[i])
			{
				j++;
				danwang = pDdz->iOnHand[i];

			}
			if (48 <= pDdz->iOnHand[i] && 51 >= pDdz->iOnHand[i])
			{
				er[t] = pDdz->iOnHand[i];
				t++;
			}
		}
		//���2��ը��,������
		if (er[3] >= 48)
			er[0] = -1;
		if (j > 1)
			danwang = -1;
		//���ҵ���
		//ȥ��2����
		for (i = 48; i <=53; i++)
		{
			DeleteElementInCards(iCopyOnHand, 21,i);
		}
		iCards = pSplitCards->startSplitCard(iCopyOnHand, 7);
		int dan[21] = { -1, -1, -1, -1 };
		//����е�,
		if (iCards[0] > -1)
		{
				AnalyzeSplitedCards(iCards, dan);
		}
		//���û�����ǳ��ƣ��뿪
		if (iCards[0] <= -1 && pDdz->iLastMainPoint < 0)
		{
			return 0;
		}
		//���û�����Ǹ��ƣ�������������1�����������������ȫ������
		if (dan[0] <= -1 && santiaoNum > 1)
		{
			t = 0;
			for (i = 0; iTmpCards[i] != -1; i++)
			{
				if (iTmpCards[i] == -2)
				{
					dan[t] = iTmpCards[i - 1];
					t++;
				}
			}
			dan[t] = -1;
		}
		//�����Ȼû��������ȥ��ը����2�Ķ��������ȫ������
		if (dan[0] <= -1)
		{
			iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);
			if (iCards[0] >= 0)
			{
				t = 0;
				for (i = 0; iCards[i] != -1; i++)
				{
					if (iCards[i] == -2)
					{
						dan[t] = iCards[i - 1];
						t++;
					}
				}
				dan[t] = -1;
			}
		}
		//���û����Ч����
		if (dan[0] <= -1)
		{
			bool condition = AnalyzeCondition(pDdz);
			if (!condition || iHandNum <= 2)
			{
				t = 0;

				if (er[0] != -1)
				{
					dan[t] = er[0];
					t++;
				}
				if (danwang != -1)
				{
					dan[t] = danwang;
					t++;
				}

			}
			dan[t] = -1;
		}
		if (dan[0] == -1)
		{
			return 0;
		}
		//����ϲ����ƺ�����
		for (i = 0; iTmpCards[i] != -1; i++)
		{
			if (iTmpCards[i] == -2)
			{
				santiao[2] = iTmpCards[i - 1];
				santiao[1] = iTmpCards[i - 2];
				santiao[0] = iTmpCards[i - 3];
				santiao[3] = -1;
				int santiaopoint = CInfo->AnalyzeMainMaxPoint(santiao);
				for (int j = 0; dan[j] != -1; j++)
				{
					if (dan[j] / 4 != santiaopoint)
					{
						santiao[3] = dan[j];
						santiao[4] = -2;
						santiao[5] = -1;
						GetLegalCards(pDdz,santiao);
					}
				}

			}

		}
	}
	return iPlaCount - iCount;
}
//H0601-END

//�޹�������һ��
int RecommendCards::Help6ThreeOneSim(struct fDdz * pDdz)
{
	int iCount = iPlaCount;
	int iTmpCards[21] = { -1 };

	int i, iTemp, j;
	int point = -1;
	int danNum = 0;
	int dan[21];
	int santiaoPoint = 0;
	int t = 0;
	//����е�
	for (i = 0; pDdz->iOnHand[i + 2] >= 0; i++)
	{

		if (pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 1] / 4
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 2] / 4)
		{
			iTemp = i;

			for (j = 0; pDdz->iOnHand[j] >= 0; j++)
			{
				t = 0;
				//�õ��Ʋ����������е������ҽ�ը���ų�
				if (j == iTemp || j == iTemp + 1 || j == iTemp + 2 || pDdz->iOnHand[j] / 4 == pDdz->iOnHand[i] / 4)
					continue;
				//�õ���Ҳ���ܸ���һ��ͬһ�������ĵ��Ƶ�����ͬ
				if (pDdz->iOnHand[j] / 4 == point)
					continue;
				iTmpCards[t] = pDdz->iOnHand[i];
				t++;
				iTmpCards[t] = pDdz->iOnHand[i + 1];
				t++;
				iTmpCards[t] = pDdz->iOnHand[i + 2];
				t++;
				iTmpCards[t] = pDdz->iOnHand[j];
				point = pDdz->iOnHand[j] / 4;
				t++;
				iTmpCards[t] = -1;
				santiaoPoint = CInfo->AnalyzeMainMaxPoint(iTmpCards);
				iTmpCards[t] = -2;
				t++;
				iTmpCards[t] = -1;
				GetLegalCards(pDdz,iTmpCards);
			}

		}
	}

	return iPlaCount - iCount;
}


//H0602-START�Ƽ�����Ӧ������һ��
//����޶���:�����Σ�����޶�ʱ��:2016-08-10
//�޸��ж����޺Ϸ�������GetLegalCards�жϣ��������
int RecommendCards::Help6ThreeDouble(struct fDdz * pDdz)
{
	int iTmpCards[60] = { -1 };
	int iCount = iPlaCount;
	int t = 0;
	int i = 0;
	int *iCards;
	t = CInfo->CountCards(pDdz->iOnHand);
	//�������ֱ�ӳ�����ֱ�ӳ�
	if (t == 5)
	{
		iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 2);
		if (iCards[0] > -1 && iCards[0] / 4 > iLastMaxPoint)
		{
			iTmpCards[0] = iCards[0];
			iTmpCards[1] = iCards[1];
			iTmpCards[2] = iCards[2];
			iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 6);
			if (iCards[0] > -1)
			{
				iTmpCards[3] = iCards[0];
				iTmpCards[4] = iCards[1];
				iTmpCards[5] = -2;
				iTmpCards[6] = -1;

				if (GetLegalCards(pDdz, iTmpCards))
				{
					return iPlaCount - iCount;
				}
			}
		}
	}
	//////////////////////////
	int iCopyOnHand[21];
	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));
	//�ж�ը��,���ų�
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 1);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}
	i = CInfo->CountCards(pDdz->iOnHand);
	if (i==20)
	{
		for (i = 48; i < 52; i++)
		{
			DeleteElementInCards(iCopyOnHand, 21, i);
		}
	}

	/////////////////
	//�ж���������
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);
	int santiaoNum = 0;

	t = 0;
	for (i = 0; iCards[i] != -1; i++)
	{
		if (iCards[0] == -2)
			break;
		if (iCards[i] == -2)
		{
			iTmpCards[t] = iCards[i - 3];
			t++;
			iTmpCards[t] = iCards[i - 2];
			t++;
			iTmpCards[t] = iCards[i - 1];
			t++;
			iTmpCards[t] = -2;
			t++;
			santiaoNum++;
		}
	}
	iTmpCards[t] = -1;

	//һ������ ������*3+������*2<21
	//���36���ڵ�//TODO�����ڿ��Դ���ڵ���
	//���û������������
	if (santiaoNum == 0)
	{
		return 0;

	}
	if (santiaoNum)
	{
		bool condition = AnalyzeCondition(pDdz);
		if (condition||iHandNum>2)
		{
			for (i = 48; i < 52; i++)
			{
				DeleteElementInCards(iCopyOnHand, 21, i);
			}
		}
		////����˫˳����Ķ���////////
		int santiao[8];
		iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);
		int duizi[60];
		if (iCards[0] <= -1)
		{
			//���û�������ǳ��ƣ��뿪
			if (pDdz->iLastMainPoint < 0)
				return 0;
			//���������������Ķ���,��������iCard��
			int  t = 0;
			for (int i = 0; iTmpCards[i] != -1; i++)
			{
				if (iTmpCards[i] == -2)
				{
					duizi[t] = iTmpCards[i - 2];
					++t;
					duizi[t] = iTmpCards[i - 1];
					++t;
					duizi[t] = -2;
					++t;
				}
			}
			duizi[t] = -1;
			iCards = duizi;
		}
		//�����Ȼû����
		if (iCards[0] == -1)
			return 0;
		condition = AnalyzeCondition(pDdz);
		for (i = 0; iTmpCards[i] != -1; i++)
		{
			int santiaoPoint;
			if (iTmpCards[i] == -2)
			{
				santiao[0] = iTmpCards[i - 3];
				santiao[1] = iTmpCards[i - 2];
				santiao[2] = iTmpCards[i - 1];
				santiao[3] = -1;
				for (int j = 0; iCards[j] != -1; j++)
				{
					if (iCards[j] == -2)
					{
						santiaoPoint = CInfo->AnalyzeMainMaxPoint(santiao);
						if (iCards[j - 1] / 4 != santiaoPoint)
						{
							santiao[3] = iCards[j - 2];
							santiao[4] = iCards[j - 1];
							santiao[5] = -2;
							santiao[6] = -1;
							if (GetLegalCards(pDdz, santiao))
							{
								if (condition)
									return iPlaCount - iCount;
							}
						}
					}
				}
			}
		}
	}
	return iPlaCount - iCount;
}
//H0602-END

//�޹���������
int RecommendCards::Help6ThreeDoubleSim(struct fDdz * pDdz)
{
	int num = 0;
	int iCount = iPlaCount;
	int i, iTemp, j;
	int iTmpCards[500] = { -1 };
	int point = -1;
	int t = 0;
	int santiaoPoint = -1;
	for (i = 0; pDdz->iOnHand[i + 2] >= 0; i++)
	{
		if (t + 5 + 1 > 500)
			break;
		if (num + 1<kPlaMax
			&& pDdz->iOnHand[i] / 4 > iLastMaxPoint
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 1] / 4
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 2] / 4)
		{
			iTemp = i;
			for (j = 0; pDdz->iOnHand[j + 1] >= 0; j++)
			{
				t = 0;
				//�������������е����ų�
				if (j >= iTemp - 1 && j <= iTemp + 2)
					continue;
				//�ö���Ҳ���ܸ���һ��ͬһ�������Ķ��ӵ�����ͬ
				if (pDdz->iOnHand[j] / 4 == point)
					continue;
				if (pDdz->iOnHand[j]>=52)
					continue;
				if (pDdz->iOnHand[j] / 4 == pDdz->iOnHand[j + 1] / 4)
				{
					iTmpCards[t] = pDdz->iOnHand[i];
					t++;
					iTmpCards[t] = pDdz->iOnHand[i + 1];
					t++;
					iTmpCards[t] = pDdz->iOnHand[i + 2];
					t++;
					iTmpCards[t] = pDdz->iOnHand[j];
					point = pDdz->iOnHand[j] / 4;
					t++;
					iTmpCards[t] = pDdz->iOnHand[j + 1];
					t++;
					iTmpCards[t] = -1;
					santiaoPoint = CInfo->AnalyzeMainMaxPoint(iTmpCards);
					iTmpCards[t] = -2;
					t++;
					iTmpCards[t] = -1;

					if (GetLegalCards(pDdz, iTmpCards))
						num++;

				}
			}
		}
	}
	return iPlaCount - iCount;
}
//H07-START�Ƽ�����Ӧ�Ե�˳
//����޶���:�����Σ�����޶�ʱ��:2016-07-26
int RecommendCards::Help7LinkSingle(struct fDdz * pDdz)
{
	int i, j, k, iLength, iTemp,  t;
	int iCopyCards[21];
	int iCount = iPlaCount;
	CInfo->SortById(pDdz->iOnHand);
	int shunziCount = 0;
	int *iCards;

	//�ų�ը��
	//����׳���ȥ��ը��(ͨ��)
	memcpy(iCopyCards, pDdz->iOnHand, sizeof(pDdz->iOnHand));
	t = CInfo->CountCards(pDdz->iOnHand);
	iCards = pSplitCards->startSplitCard(iCopyCards, 1);
	for (i = 0; iCards[i] != -1; i++)
	{
		if (iCards[i] != -2)
		{
			DeleteElementInCards(iCopyCards, 21, iCards[i]);
		}
	}
	//ȥ������2
	for (i = 48; i <= 53; i++)
	{
		DeleteElementInCards(iCopyCards, 21, i);
	}
	//ȥ�����Ʋ���С��������
	///CInfo->GetUnReaptedCards(iCopyCards);

	///////////////////////////////////

	int iTmpShunzi[21];
	memset(iTmpShunzi, -1, sizeof(iTmpShunzi));
	int NodesCount = 0;
	int MaxNum;
	int iDanShunNum = 0;
	int iDanShunNoBombNum = 0;
	int MaxNumNoBomb = 0;
	//����ը������Ӱ���Ƿ��ܲ����˳
	//���������ڵ㣺һ����ȥ��ը��������ƣ�һ����ûȥ��ը����
	//����ǳ���
	if (pDdz->iLastMainPoint < 0)
	{

		for (iLength = 5; iLength <= 13; iLength++)
		{
			//��ѯ�ó��ȵ�˳��
			iDanShunNoBombNum = AnalyzeSingleLink(iLength, -1, iCopyCards, iTmpShunzi);
			iDanShunNum = AnalyzeSingleLink(iLength, -1, pDdz->iOnHand, iTmpShunzi);
			//��ģ��,�Ҳ�ѯ����ʧ�ܳ�ǰһ����¼���С˳��
			if (!isSim)
			{
				if (iDanShunNum == iLength)
				{
					MaxNum = iLength;
				}
				if (iDanShunNoBombNum == iLength)
				{
					MaxNumNoBomb = iLength;
				}
			}
			else
			{
				if (iDanShunNum == iLength)
				{
					GetLegalCards(pDdz,iTmpShunzi);
				}
			}
		}
		if (MaxNumNoBomb >= 5)
		{
			memset(iTmpShunzi, -1, sizeof(iTmpShunzi));
			iDanShunNoBombNum = AnalyzeSingleLink(MaxNumNoBomb, -1, iCopyCards, iTmpShunzi);
			if (iDanShunNoBombNum == MaxNumNoBomb)
			GetLegalCards(pDdz, iTmpShunzi);
		}
		i = CInfo->CountCards(pDdz->iOnHand);
		//������׳����߳�������ը��ʱһ�£����ó�
		if (MaxNumNoBomb != MaxNum&&MaxNum >= 5 && i != 20)
		{
			memset(iTmpShunzi, -1, sizeof(iTmpShunzi));
			iDanShunNum = AnalyzeSingleLink(MaxNum, -1, pDdz->iOnHand, iTmpShunzi);
			if (iDanShunNum==MaxNum)
			GetLegalCards(pDdz, iTmpShunzi);
		}
	}
	//������ƣ��������ܳ����ƽڵ�
	else
	{
		iLength = pDdz->iLastTypeCount - 700;
		int point = pDdz->iLastMainPoint;
		while(point + iLength - 1 <= 11)
		{
			iDanShunNum=AnalyzeSingleLink(iLength, point, pDdz->iOnHand, iTmpShunzi);
			point=point+1;
			if (iDanShunNum == iLength)
				GetLegalCards(pDdz, iTmpShunzi);
		}
	}
	return iPlaCount - iCount;
}
//H07-END

//H08-START�Ƽ�����Ӧ��˫˳
//����޶���:�����Σ�����޶�ʱ��:2016-08-11
//�޶�����ʱ��˫˳�����
int RecommendCards::Help8LinkDouble(struct fDdz * pDdz)
{

	int i, j, k, iLength;
	int iCopyOnHand[21];
	int n = 1;
	int iFlag = 0;
	int iCardsFlag = 0;
	int iCard = 0;
	int iTmpShuangshun[21];
	int iTmpCards[21];
	int iTypeCount;
	int iCount = iPlaCount;
	//int iSave[12 * 12];
	memset(iTmpShuangshun, -1, sizeof(iTmpShuangshun));
	CInfo->SortById(pDdz->iOnHand);
	//����ʱ
	for (i = 0; i < 21; i++)
		iCopyOnHand[i] = pDdz->iOnHand[i];
	//ȥ������2
	for (i = 0; iCopyOnHand[i] >= 0; i++)
		if (iCopyOnHand[i] / 4 >= 12)
			iCopyOnHand[i] = -1;
	//�ų�ը��
	//��������׳���ȥ��ը��(ͨ��)
	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));
	k = CInfo->CountCards(pDdz->iOnHand);
	if (k == 20&&!isSim)
	{
		int *iCards = pSplitCards->startSplitCard(iCopyOnHand, 1);
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
			{
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
			}
		}
	}
	auto GetShuangShun = [&](int *iCards, int iLength,int iLastMainPoint)->int
	{
		int shuangshunNum = 0;
		int iCopyCards[21];
		for (i = 0; iCards[i] != -1; i++)
		{
			iCopyCards[i] = iCards[i];
		}
		iCopyCards[i] = -1;
		for (i = 0; i < CInfo->CountCards(iCopyCards); i++)
		{
			if (iCopyCards[i] / 4 != iCopyCards[i + 1] / 4)
			{
				for (j = i; j < CInfo->CountCards(iCopyCards); j++)
					iCopyCards[j] = iCopyCards[j + 1];
				i--;
			}

		}//���������� ��������� ���ӱ䵥�� ���ű�û��
		for (i = 0; i < CInfo->CountCards(iCopyCards); i++)
		{
			if (iCopyCards[i] / 4 == iCopyCards[i + 1] / 4)
			{
				for (j = i; j < CInfo->CountCards(iCopyCards); j++)
					iCopyCards[j] = iCopyCards[j + 1];
				i--;
			}
		}
		//�����Ͷ��ӱ�ɵ���
		//���Ͻ�����iCopyCards[21]���������ɶ��ӵĵ������������ΪA

		for (i = 0; iCopyCards[i] >= 0; i++)
		{
			if (iCopyCards[i] / 4 == iCopyCards[i + 1] / 4 - 1)
				n++;
			else
			{
				n = 1;
				i = iFlag;
				iFlag++;
			}
			if (n == iLength)
			{
				//int lastMinPoint = CInfo->AnalyzeMainPoint(pDdz->iLastOnTable);
				if (shuangshunNum + 1 < kPlaMax
					&& iCopyCards[i + 2 - iLength] / 4 > iLastMainPoint)
				{
					for (j = i + 2 - iLength; j <= i + 1; j++)
					{
						for (k = 0; k < CInfo->CountCards(pDdz->iOnHand); k++)
						{
							if (iCopyCards[j] / 4 == pDdz->iOnHand[k] / 4 && iCardsFlag < 2)
							{
								iTmpShuangshun[iCard] = pDdz->iOnHand[k];
								iCard++;
								iCardsFlag++;
							}
						}
						iCardsFlag = 0;
					}
					iTmpShuangshun[iCard] = -2;
					iTmpShuangshun[iCard + 1] = -1;
					if (GetLegalCards(pDdz, iTmpShuangshun))
						shuangshunNum++;
					iCard = 0;
				}
				n = 1;
				i = iFlag;
				iFlag++;
			}
		}
		return shuangshunNum;
	};
	if (pDdz->iLastTypeCount > 0)
	{
		iLength = (pDdz->iLastTypeCount - 800) / 2;
		GetShuangShun(iCopyOnHand,iLength,pDdz->iLastMainPoint);
	}
	else
	{
		for (iLength = 3; iLength <=11; iLength++)
		{
			GetShuangShun(iCopyOnHand, iLength,pDdz->iLastMainPoint);
		}
	}
	return iPlaCount-iCount;
}
//H08-END

//H09-START�Ƽ�����Ӧ����˳
//����޶���:�����Σ�����޶�ʱ��:2016-08-11
//����Ӧ�Գ��Ƴ���˳�����
int RecommendCards::Help9LinkThree(struct fDdz * pDdz)
{
	int iTmpSanshun[21];
	memset(iTmpSanshun, -1, sizeof(iTmpSanshun));
	int sanshunNum = 0;
	int i, iTemp, j, k, iLength = pDdz->iLastTypeCount - 900;
	int iTempArr[12][5];
	if (CInfo->CountCards(pDdz->iOnHand) < 6)
		return 0;
	int iTypeCount = 0;
	int iCount = iPlaCount;
	if (pDdz->iLastMainPoint>= 0)
	{
		// iTempArr[i][0]����װ����Ϊ i �ж������ƣ�
		// iTempArr[i][1],iTempArr[i][2],iTempArr[i][3],iTempArr[i][4]����װ����Ϊ i ���Ƶı��룬
		memset(iTempArr, -1, 12 * 5 * 4);
		CInfo->SortById(pDdz->iOnHand);    //�Ƚ������������
		for (i = 0; pDdz->iOnHand[i] > -1; i++)
		{
			if (pDdz->iOnHand[i] < 48)
			{
				//������Ϊ iCards[i]/4 �����ж����ţ��� j ����ʾ
				j = 1;
				while (-1 != iTempArr[pDdz->iOnHand[i] / 4][j])
					j++;

				//���������iTempArr[iCards[i]/4][j]���棬��Ϊ�ñ���ĵ���ΪiCards[i]/4
				iTempArr[pDdz->iOnHand[i] / 4][j] = pDdz->iOnHand[i];

				//������Ϊ iCards[i]/4 ���Ƶ����� ��Ϊ j ��
				iTempArr[pDdz->iOnHand[i] / 4][0] = j;
			}
		}
		//int lastMinPoint = CInfo->AnalyzeMainPoint(pDdz->iLastOnTable);
		for (i = pDdz->iLastMainPoint + 1; i < 12; i++)
		{
			iTemp = iLength - 3;
			if (iTempArr[i][0] >= 3)
			{
				j = i;
				while (iTemp)
				{
					if (iTempArr[j][0] >= 3)
					{
						iTemp -= 3;
						j++;
					}
					else break;
				}
				if (sanshunNum + 1 < kPlaMax
					&& iTemp == 0)
				{
					k = 0;
					while (k < iLength)
					{
						iTmpSanshun[k] = iTempArr[j][1];
						k++;
						iTmpSanshun[k] = iTempArr[j][2];
						k++;
						iTmpSanshun[k] = iTempArr[j][3];
						k++;
						j--;
					}
					iTmpSanshun[k] = -2;
					k++;
					iTmpSanshun[k] = -1;
					GetLegalCards(pDdz,iTmpSanshun);

				}
			}
		}
	}
	//����ʱ,ֱ�ӳ����ƺ���˳
	if (pDdz->iLastMainPoint < 0)
	{
		int iCopyCards[21];
		memcpy(iCopyCards, pDdz->iOnHand, sizeof(pDdz->iOnHand));
		//�ų�ը��
		i = CInfo->CountCards(pDdz->iOnHand);
		int *iCards;
		if (i == 20&&!isSim)
		{
			iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] != -2)
				{
					DeleteElementInCards(iCopyCards, 21, iCards[i]);
				}
			}

		}
		iCards = pSplitCards->startSplitCard(iCopyCards, 3);
		//�������˳��ֱ�ӳ�
		if (iCards[0] >= 0)
		{
			j = 0;
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] == -2)
				{
					iTmpSanshun[j] = -2;
					j++;
					iTmpSanshun[j] = -1;
					GetLegalCards(pDdz, iTmpSanshun);
				}
				else{
					iTmpSanshun[j] = iCards[i];
					j++;
				}
			}
		}
	}
	return iPlaCount-iCount;
}
//H09-END

//UNDONE
//H1001-START�Ƽ�����Ӧ����˳����
//����޶���:�����Σ�����޶�ʱ��:16-07-26
int RecommendCards::Help10LinkThreeSingle(struct fDdz * pDdz)
{
	int iCount = iPlaCount;
	int iTmpSanshundan[40];
	int sanshundanNum = 0;
	int i, iTemp, j, k, m;
	int iTempArr[12][5];
	int nFlag = 0;
	int iProtect = 0;
	int iLength = (pDdz->iLastTypeCount - 1000);
	// iTempArr[i][0]����װ����Ϊ i �ж������ƣ�
	// iTempArr[i][1],iTempArr[i][2],iTempArr[i][3],iTempArr[i][4]����װ����Ϊ i ���Ƶı��룬
	memset(iTempArr, -1, 12 * 5 * 4);
	CInfo->SortById(pDdz->iOnHand);    //�Ƚ������������
	int iCopyOnHand[21] = { -1 };
	//�ų�ը��
	int bombPoint[4] = { -1, -1, -1, -1 };
	j = 0;
	int t = 0;//ը����
	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	j = 0;
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] == -2)
			{
				bombPoint[j] = iCards[i - 1] / 4;
				j++;
			}
		}
	}
	j = 0;
	for (i = 0; pDdz->iOnHand[i] != -1; i++)
	{
		if (pDdz->iOnHand[i] / 4 != bombPoint[0] && pDdz->iOnHand[i] / 4 != bombPoint[1])
		{
			iCopyOnHand[j] = pDdz->iOnHand[i];
			j++;
		}
	}
	iCopyOnHand[j] = -1;
	int *iOnHand = pDdz->iOnHand;
	bool condition = AnalyzeCondition(pDdz);
	if (iLength < 0)
	{
		iLength = 8;
		i = CInfo->CountCards(pDdz->iOnHand);
		if (condition||i==20)
		iOnHand = iCopyOnHand;
		iCards = pSplitCards->startSplitCard(iOnHand, 3);
		if (iCards[0] >= 0)
		{
			int t = 0;//�����˳��
			j = 0;
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] == -2)
				{
					if (j > t)
					{
						t = j;
					}
					j = 0;
				}
				else
					j++;//������
			}
			iLength = t + t / 3;
		}
	}

	for (i = 0; iOnHand[i] > -1; i++)
	{
		if (iOnHand[i] < 48)
		{
			//������Ϊ iCards[i]/4 �����ж����ţ��� j ����ʾ
			j = 1;
			while (-1 != iTempArr[iOnHand[i] / 4][j])
				j++;

			//���������iTempArr[iCards[i]/4][j]���棬��Ϊ�ñ���ĵ���ΪiCards[i]/4
			iTempArr[iOnHand[i] / 4][j] = iOnHand[i];

			//������Ϊ iCards[i]/4 ���Ƶ����� ��Ϊ j ��
			iTempArr[iOnHand[i] / 4][0] = j;
		}
	}

	CInfo->SortById(iOnHand);    //�Ƚ������������
	//int lastMinPoint = CInfo->AnalyzeMainPoint(pDdz->iLastOnTable);
	for (i = pDdz->iLastMainPoint + 1; i < 12; i++)
	{
		iTemp = iLength;
		if (iTempArr[i][0] >= 3)
		{
			j = i;
			while (iTemp)
			{
				if (iTempArr[j][0] >= 3)
				{
					iTemp -= 4;
					j++;
				}
				else break;
			}
			if (iTemp == 0)
			{
				k = 0;
				j--;
				while (k < iLength / 4 * 3)
				{
					iTmpSanshundan[k] = iTempArr[j][1];
					k++;
					iTmpSanshundan[k] = iTempArr[j][2];
					k++;
					iTmpSanshundan[k] = iTempArr[j][3];
					k++;
					j--;
				}

				//���Ʋ��ܴ�ը������,����ʱ���ܴ�2,
				for (j = 0; iOnHand[j] >= 0; j++)
				{
					nFlag = 1;
					for (m = 0; m < k; m++)
					{
						if (iOnHand[j] / 4 == iTmpSanshundan[m] / 4)
						{
							nFlag = 0;
							break;
						}
						//����̫���ܴ�2
						if (iOnHand[j]/4==12 && iHandNum > 5)
						{
							nFlag = 0;
							break;
						}
						//����ֱ�ӳ������ܴ���
						if (iOnHand[j] / 4 == 13 && iHandNum >3)
						{
							nFlag = 0;
							break;
						}
						if (iOnHand[j] / 4 == bombPoint[0] || iOnHand[j] / 4 == bombPoint[1])
						{
							nFlag = 0;
							break;
						}
					}
					if (sanshundanNum + 1 < kPlaMax
						&& nFlag)
					{
						iTmpSanshundan[k] = iOnHand[j];
						k++;
						if (k == iLength)
						{
							iTmpSanshundan[k] = -2;
							k++;
							iTmpSanshundan[k] = -1;
								//CInfo->SortByMuch(iTmpSanshundan);
								if (GetLegalCards(pDdz, iTmpSanshundan))
								{
									sanshundanNum++;
								}
								k = 0;
							break;
						}
					}
				}
			}
		}
	}

	return iPlaCount-iCount;

}
//H1001-END

//UNDONE
//����ű���
//H1002-START�Ƽ�����Ӧ����˳��˫
//����޶���:������ ����޶�ʱ��:16-07-26
int RecommendCards::Help10LinkThreeDouble(struct fDdz * pDdz)
{
	int iCount = iPlaCount;
	int iCardsNum = CInfo->CountCards(pDdz->iOnHand);
	if (iCardsNum >= 17)
		return 0;
	int iTmpSanshunshuang[70];
	int sanshunshuangNum = 0;
	int i, iTemp, j, k, m, iLength = (pDdz->iLastTypeCount - 1000);
	int iTempArr[12][5];
	int nFlag = 0;
	int iCopyOnHand[21] = { -1 };
	int bombPoint[4] = { -1, -1, -1, -1 };//����ը������
	int t = 0;//ը����
	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	j = 0;
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] == -2)
			{
				bombPoint[j] = iCards[i - 1] / 4;
				j++;
			}
		}
	}
	//�ų�ը������
	j = 0;
	for (i = 0; pDdz->iOnHand[i] != -1; i++)
	{
		if (pDdz->iOnHand[i] / 4 != bombPoint[0] && pDdz->iOnHand[i] / 4 != bombPoint[1] && pDdz->iOnHand[i] / 4 != bombPoint[2]
			&& pDdz->iOnHand[i] / 4 != bombPoint[3])
		{
			if (pDdz->iOnHand[i] < 52)
			{
				iCopyOnHand[j] = pDdz->iOnHand[i];
				j++;
			}
		}
	}
	iCopyOnHand[j] = -1;
	int *iOnHand = pDdz->iOnHand;
	bool condition = AnalyzeCondition(pDdz);

		auto GetSanShunDouble = [&](int Length)
		{
			for (i = pDdz->iLastMainPoint + 1; i <12; i++)
		{
			iTemp = iLength;
			if (iTempArr[i][0] >= 3)
			{
				j = i;
				while (iTemp)
				{
					if (iTempArr[j][0] >= 3)
					{

						iTemp -= 5;
						j++;
					}
					else break;
				}
				if (iTemp == 0)
				{
					k = 0;
					j--;
					while (k < iLength / 5 * 3)
					{
						iTmpSanshunshuang[k] = iTempArr[j][1];
						k++;
						iTmpSanshunshuang[k] = iTempArr[j][2];
						k++;
						iTmpSanshunshuang[k] = iTempArr[j][3];
						k++;
						j--;
					}

					for (j = 0; pDdz->iOnHand[j] >= 0; j++)
					{
						nFlag = 1;
						for (m = 0; m < k; m++)
						{
							if (iOnHand[j] / 4 == iTmpSanshunshuang[m] / 4)
							{
								nFlag = 0;
								break;
							}
							if (iOnHand[j] / 4 == 12 && iHandNum > 4)
							{
								nFlag = 0;
								break;
							}
							//���ܴ���
							if (iOnHand[j] / 4 >= 13)
							{
								nFlag = 0;
								break;
							}
							if (iOnHand[j] / 4 == bombPoint[0] || iOnHand[j] / 4 == bombPoint[1])
							{
								nFlag = 0;
								break;
							}
						}
						if (sanshunshuangNum + 1<kPlaMax
							&& nFlag)
						{
							if (iOnHand[j] / 4 != iOnHand[j + 1] / 4)
								continue;
							iTmpSanshunshuang[k] = iOnHand[j];
							k++;
							iTmpSanshunshuang[k] = iOnHand[j + 1];
							k++;
							if (k == iLength)
							{
								iTmpSanshunshuang[k] = -2;
								k++;
								iTmpSanshunshuang[k] = -1;
								GetLegalCards(pDdz, iTmpSanshunshuang);
								//break;
							}
						}
					}
				}
			}
		}
		};
	// iTempArr[i][0]����װ����Ϊ i �ж������ƣ�
	// iTempArr[i][1],iTempArr[i][2],iTempArr[i][3],iTempArr[i][4]����װ����Ϊ i ���Ƶı��룬
	memset(iTempArr, -1, 12 * 5 * 4);
	CInfo->SortById(iOnHand);    //�Ƚ������������
	if (pDdz->iLastMainPoint < 0)
	{
		iLength = 10;
		i = CInfo->CountCards(pDdz->iOnHand);
		if (condition || i == 20)
			iOnHand = iCopyOnHand;
	}
	else
	{
		iOnHand = pDdz->iOnHand;
	}
	for (i = 0; iOnHand[i] > -1; i++)
	{
		if (iOnHand[i] < 48)
		{
			//������Ϊ iCards[i]/4 �����ж����ţ��� j ����ʾ
			j = 1;
			while (-1 != iTempArr[iOnHand[i] / 4][j])
				j++;

			//���������iTempArr[iCards[i]/4][j]���棬��Ϊ�ñ���ĵ���ΪiCards[i]/4
			iTempArr[iOnHand[i] / 4][j] = iOnHand[i];

			//������Ϊ iCards[i]/4 ���Ƶ����� ��Ϊ j ��
			iTempArr[iOnHand[i] / 4][0] = j;
		}
	}
	if (pDdz->iLastMainPoint < 0)
	{
		iLength = 10;
		int i = CInfo->CountCards(pDdz->iOnHand);
		if (condition || i == 20)
			iOnHand = iCopyOnHand;
		iCards = pSplitCards->startSplitCard(iOnHand, 3);
		//�������˳��ֱ�ӳ��ų�ը�������������˳
		int maxSanshun = 6;//�����˳��
		if (iCards[0] >= 0)
		{

			j = 0;
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] == -2)
				{
					if (j > maxSanshun)
					{
						maxSanshun = j;
					}
					j = 0;
				}
				else
					j++;//������
			}

			iLength = maxSanshun + 2 * (maxSanshun / 3);
		}

		while (iLength >= 10)
		{
			GetSanShunDouble(iLength);
			maxSanshun -= 3;
			if (maxSanshun>0)
			iLength = maxSanshun + 2  * (maxSanshun / 3);
		}
	}
	else
	{
		GetSanShunDouble(iLength);
	}
	//int lastMinPoint = CInfo->AnalyzeMainPoint(pDdz->iLastOnTable);

	return iPlaCount-iCount;
}
//H1002-END

//UNDONE
//H1101-START�Ƽ�����Ӧ���Ĵ�����
//����޶���:�ĺ��нܣ�����޶�ʱ��:15-03-10 12:00
int RecommendCards::Help11FourSingle(struct fDdz * pDdz)
{
	int Cards = CInfo->CountCards(pDdz->iOnHand);
	//���׳�
	if (!isSim&&Cards >= 17 && pDdz->iLastMainPoint<0)
		return 0;
	///��δ������������ǿ��ܵó��Ľ���ǲ����Ҫ�����������
	//�������5���Ĵ�2�ڵ�
	int MaxNode = 5;
	int iCount = iPlaCount;
	int iTmpsidaierdan[21];
	memset(iTmpsidaierdan, -1, sizeof(iTmpsidaierdan));
	int sidaierdanNum = 0;
	int i, iTempI, j, k;

	if (CInfo->CountCards(pDdz->iOnHand)<6)
		return 0;
	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 7);
	CInfo->SortById(pDdz->iOnHand);
	for (i = 0; pDdz->iOnHand[i + 3] >= 0; i++)
	{
		//int lastMinPoint = CInfo->AnalyzeMainPoint(pDdz->iLastOnTable);
		if (pDdz->iOnHand[i] / 4 > pDdz->iLastMainPoint
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 1] / 4
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 2] / 4
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 3] / 4)
		{
			//���ը����2
			if (pDdz->iOnHand[i]/4 == 12)
			{
				//���ʣ�������϶࣬����
				if (iHandNum > 3)
					continue;
			}
			iTempI = i;
			int theLastJ = 0;
			//�����Ƿ����ֳ����ŵ���,����У������С������
			if (iCards[0] >= 0 && iCards[2] >= 0)
			{
				if (iCards[0] >= 48)
				{
					//���ʣ�������϶࣬����
					if (iHandNum > 3)
						continue;
				}
				iTmpsidaierdan[0] = pDdz->iOnHand[i];
				iTmpsidaierdan[1] = pDdz->iOnHand[i + 1];
				iTmpsidaierdan[2] = pDdz->iOnHand[i + 2];
				iTmpsidaierdan[3] = pDdz->iOnHand[i + 3];
				iTmpsidaierdan[4] = iCards[0];
				iTmpsidaierdan[5] = iCards[2];
				iTmpsidaierdan[6] = -2;
				iTmpsidaierdan[7] = -1;
				if (GetLegalCards(pDdz, iTmpsidaierdan))
				{
					sidaierdanNum++;
				}
			}
			else
			{
				for (j = 0; pDdz->iOnHand[j] >= 0; j++)
				{
					//�õ��Ʋ�����4���е���
					if (j == iTempI || j == iTempI + 1 || j == iTempI + 2 || j == iTempI + 3)
						continue;

					if (j != theLastJ)
					{
						if (pDdz->iOnHand[j] / 4 == pDdz->iOnHand[theLastJ] / 4)
							continue;
					}
					if (pDdz->iOnHand[j] / 4 == pDdz->iOnHand[theLastJ] / 4 || pDdz->iOnHand[j] / 4==13)
						continue;
					if (pDdz->iOnHand[j] /4== 12)
					{
						//���ʣ�������϶࣬����
						if (iHandNum > 3)
							continue;
					}
					int theLastK = 0;
					for (k = j + 1; pDdz->iOnHand[k] >= 0; k++)
					{

						//�õ��Ʋ�����4���е���,Ҳ���ܸ�ǰһ�ŵ��Ƶ�����ͬ
						if (k == iTempI || k == iTempI + 1 || k == iTempI + 2 || k == iTempI + 3)
							continue;
						if (pDdz->iOnHand[k] / 4 == pDdz->iOnHand[j] / 4)
							continue;
						//�����ܸ���һ�ŵ��Ƶ�����ͬ
						if (k != theLastK)
						{
							if (pDdz->iOnHand[k] / 4 == pDdz->iOnHand[theLastK] / 4 || pDdz->iOnHand[k] / 4==13)
								continue;
							if (pDdz->iOnHand[k]/4 == 12)
							{
								//���ʣ�������϶࣬����
								if (iHandNum > 3)
									continue;
							}

						}


							iTmpsidaierdan[0] = pDdz->iOnHand[i];
							iTmpsidaierdan[1] = pDdz->iOnHand[i + 1];
							iTmpsidaierdan[2] = pDdz->iOnHand[i + 2];
							iTmpsidaierdan[3] = pDdz->iOnHand[i + 3];
							iTmpsidaierdan[4] = pDdz->iOnHand[j];
							iTmpsidaierdan[5] = pDdz->iOnHand[k];
							iTmpsidaierdan[6] = -2;
							iTmpsidaierdan[7] = -1;
							theLastK = k;
							theLastJ = j;
							if (GetLegalCards(pDdz, iTmpsidaierdan))
							{
								sidaierdanNum++;
							}
						}
					}
				}
			}
		}
		return iPlaCount - iCount;
}
//H1101-END

//UNDONE
//H1102-START�Ƽ�����Ӧ���Ĵ�����
//����޶���:�ĺ��нܣ�����޶�ʱ��:15-03-23 12:00
int RecommendCards::Help11FourDouble(struct fDdz * pDdz)
{
	int Cards = CInfo->CountCards(pDdz->iOnHand);
	if (!isSim&&Cards >= 17)
		return 0;
	//�������5���Ĵ�2�ڵ�
	int MaxNode = 5;
	int iCount = iPlaCount;
	int iTmpsidai[21];
	memset(iTmpsidai, -1, sizeof(iTmpsidai));
	int sidaiNum = 0;
	int i, iTempI, j, k;
	if (CInfo->CountCards(pDdz->iOnHand)<8)
		return 0;
	CInfo->SortById(pDdz->iOnHand);
	for (i = 0; pDdz->iOnHand[i + 3] >= 0; i++)
	{
		if (pDdz->iOnHand[i] / 4 > pDdz->iLastMainPoint
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 1] / 4
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 2] / 4
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 3] / 4)
		{
			//���ը����2
			if (pDdz->iOnHand[i] >= 48)
			{
				//���ʣ�������϶࣬����
				if (iHandNum > 3)
					continue;
			}
			iTempI = i;
			for (j = 0; pDdz->iOnHand[j] >= 0; j++)
			{
				if (j == iTempI || j == iTempI + 1 || j == iTempI + 2 || j == iTempI + 3 || pDdz->iOnHand[j] / 4 != pDdz->iOnHand[j + 1] / 4)
					continue;
				if (pDdz->iOnHand[j] >= 48)
				{
					//���ʣ�������϶࣬����
					if (iHandNum > 3)
						continue;
				}
				for (k = j + 2; pDdz->iOnHand[k] >= 0; k++)
				{
					//�õ��Ʋ�����4���е����ҰѴ�С���ų���
					if (k == iTempI || k == iTempI + 1 || k == iTempI + 2 || k == iTempI + 3 || pDdz->iOnHand[j] >= 52 || pDdz->iOnHand[k] / 4 != pDdz->iOnHand[k + 1] / 4)
						continue;
					if (pDdz->iOnHand[k] >= 48)
					{
						//���ʣ�������϶࣬����
						if (iHandNum > 3)
							continue;
					}
					if (sidaiNum + 1<MaxNode)
					{
						iTmpsidai[0] = pDdz->iOnHand[i];
						iTmpsidai[1] = pDdz->iOnHand[i + 1];
						iTmpsidai[2] = pDdz->iOnHand[i + 2];
						iTmpsidai[3] = pDdz->iOnHand[i + 3];
						iTmpsidai[4] = pDdz->iOnHand[j];
						iTmpsidai[5] = pDdz->iOnHand[j + 1];
						iTmpsidai[6] = pDdz->iOnHand[k];
						iTmpsidai[7] = pDdz->iOnHand[k + 1];
						iTmpsidai[8] = -2;
						iTmpsidai[9] = -1;

							if (GetLegalCards(pDdz, iTmpsidai))
							{
								sidaiNum++;
							}
					}
				}
			}
		}
	}

	return iPlaCount-iCount;
}
//H1102-END

//��ȡ������ͣ�1Ϊ������2Ϊ�ϼ�ũ��3Ϊ�¼�ũ��
//����޶���:�����Σ�����޶�ʱ��:16-03-29 17:00
int RecommendCards::GetPlayerType(fDdz *pDdz)
{
	if (pDdz->cDir == pDdz->cLandlord)
		return 1;
	else if (pDdz->cLastPlay == pDdz->cLandlord)
		return 3;
	else
		return 2;

}

//���������������ʣ����
int RecommendCards::OtherPlayersCards(fDdz *pDdz, Ddz *rDdz)
{
	int i;
	//�����AI
	if (pDdz->cDir == rDdz->cDir)
	{
		pDdz->iUpPlayerCards = rDdz->iUpPlayerCards;//�������������
		pDdz->iDownPlayerCards = rDdz->iDownPlayerCards;//�������������
	}
	//�����AI�������
	else if (pDdz->cDir == rDdz->cLastPlay)
	{

		for (i = 0; rDdz->iOnHand; i++);
		pDdz->iUpPlayerCards = rDdz->iDownPlayerCards;//�������������
		pDdz->iDownPlayerCards = i;
	}
	//�����AI�������
	else
	{
		for (i = 0; rDdz->iOnHand; i++);
		pDdz->iUpPlayerCards = i;//�������������
		pDdz->iDownPlayerCards = rDdz->iUpPlayerCards;
	}
	return 0;
}

////������λ�������ʣ�ƣ��洢��iOnOtherHand[]��,������С��������,������λ���ʣ����
////iOnOtherHand[]��-1��β
////����޶��ߣ�������
////����޸�ʱ�䣺2016/4/1
//int RecommendCards::OtherPlayersCards(fDdz* pDdz)
//{
//
//	//����������û�г�����iOnhand[]��iOnTable[]���棬�������������������
//
//	int allCards[54];//����һ����װ������������������,��ʼ��Ϊ1��ʾ�����ƶ���
//	for (int i = 0; i < 54; i++)
//	{		//��ʼ����������
//		allCards[i] = 1;
//	}
//	int cards = 0;
//	//�ų�iOnHand[]�е���
//
//	for (int i = 0; pDdz->iOnHand[i] > -1; i++)
//	{
//
//		if (cards != pDdz->iOnHand[i])
//			cards = pDdz->iOnHand[i];
//		allCards[cards] = 0;
//		cards++;
//	}
//
//
//	//�ų�iOnTable[]�е���,iontable���е��ƶ�Ӧ��ֵ��Ϊ0��
//	for (int i = 0; pDdz->iOnTable[i][0] != -2; i++)
//	{
//		cards = 0;
//		CInfo->SortById(pDdz->iOnTable[i]);
//		for (int j = 0; pDdz->iOnTable[i][j] >-1; i++)
//		{
//			if (cards != pDdz->iOnTable[i][j])
//				cards = pDdz->iOnTable[i][j];
//			allCards[cards] = 0;
//			cards++;
//		}
//
//	}
//	//������Ǽǵ�iOnOtherHand[]��
//	cards = 0;
//	for (int i = 0; i<54; i++)
//	{
//		if (allCards[i] == 1)
//		{
//			pDdz->iOnOtherHand[cards] = i;
//			cards++;
//		}
//
//	}
//	cards++;
//	pDdz->iOnOtherHand[cards] = -1;
//	CInfo->SortById(pDdz->iOnOtherHand);
//	cards--;
//	return cards;
//
//}


////UNDO�����Ϊ��������̶����Ź�����
////Ϊ��������������һ����
////����޶��ߣ�������
////����޸�ʱ�䣺2016/4/19
//void RecommendCards::RandomHandCards(fDdz* pDdz)
//{
//	int count = 0;//OtherPlayersCards(pDdz);
//	int count1 = 0;//LastPlayerCardsCount(pDdz);
//	int count2 = count - count1;
//
//	std::vector<int> vi(count); // ����һ����СΪN��vector
//	for (int i = 0; i<count; ++i)
//	{
//		vi[i] = pDdz->iOnOtherHand[i];
//	}
//	srand((unsigned)time(NULL));
//	std::random_shuffle(vi.begin(), vi.end());
//
//	int i = 0;
//	//���ɼ�����һ����ҵ������
//	for (vector<int>::iterator it = vi.begin(); it != vi.begin() + count1; it++)
//	{
//
//		pDdz->iRandomOnHand[0][i] = *it;
//		i++;
//	}
//
//	pDdz->iRandomOnHand[0][i] = -1;
//	i = 0;
//	cout << "iRandomOnHand[0]:";
//	while (pDdz->iRandomOnHand[0][i] != -1)
//	{
//		cout << pDdz->iRandomOnHand[0][i] << ",";
//		i++;
//	}
//	cout << '\n';
//	CInfo->SortById(pDdz->iRandomOnHand[0]);
//	//������һ���Ǽ�����ҵ������
//	i = 0;
//	for (vector<int>::iterator it = vi.begin() + count1; it != vi.end(); it++)
//	{
//		pDdz->iRandomOnHand[1][i] = *it;
//		i++;
//	}
//
//	pDdz->iRandomOnHand[1][i] = -1;
//
//	i = 0;
//	cout << "iRandomOnHand[1]:";
//	while (pDdz->iRandomOnHand[1][i] != -1)
//	{
//		cout << pDdz->iRandomOnHand[1][i] << ",";
//		i++;
//	}
//	cout << '\n';
//	cout << "iOnHand";
//	i = 0;
//	while (pDdz->iOnHand[i] != -1)
//	{
//		cout << pDdz->iOnHand[i] << ",";
//		i++;
//	}
//	CInfo->SortById(pDdz->iRandomOnHand[1]);
//}

////M07-START������ģ���Ƴ�һ����
////����޶���:÷��,����޶�ʱ��:15-03-01
//void RecommendCards::HelpTakeOff(fDdz* pDdz, int num)
//{
//	int i, j = 0, k, same;
//	for (i = 0; pCardsOnHand[i] >= 0; i++)
//	{
//		same = 0;
//		for (k = 0; pDdz->iPlaArr[num][k] >= 0; k++)
//		{
//			if (pCardsOnHand[i] == pDdz->iPlaArr[num][k])
//			{
//				same = 1;
//			}
//		}
//		if (same != 1)
//		{
//			pDdz->iPlaOnHand[j] = pCardsOnHand[i];
//			j++;
//		}
//	}
//	pDdz->iPlaOnHand[j] = -1;
//}



// �ж�ǰ�������Ƿ�PASS����
bool RecommendCards::IfPassTwice(fDdz *pDdz)
{
	if (pDdz->iLastTypeCount <= 0)
	{//����2��PASS,����������
		return true;
	}
	return false;

}

//DEBUG:������������ж϶����ܼ򵥽���״γ���ʱ�������������
//BUG
// ���SpitCard�ķ��ظ�ʽ�������
// ��iCards���������ܴ���ϼҳ��ƵĽ�,���浽����iSave����,iSave�����ʼ��Ϊ-1
//����޶���:�����Σ�����޶�ʱ��:16-07-28 23:50
//�޶����ݣ������жϳ���ʱ��Ե����
//2016/8/8:�����ж�ը����������
//2016/8/10 :ȡ������������ƿ����ж����Ƶ���䣬�������
int RecommendCards::GetLegalCards(fDdz *pDdz, int *iCards, int *iSave)
{
	//���ơ���������˳��˫˳����˳��������űȽϴ�С������һ����˳���ơ��Ĵ�������������������˳�����������űȽϴ�С������ƴ�С�޹ء�
	//�������ը���������ը���������������ͣ�ը��֮�䰴���Ŵ�С���бȽϡ�
	int iCardsTmp[50];
	int t = 0;//����ָ��iCards�±�
	int start;//ÿ�����Ϳ�ʼ���±�
	int iNodesCount = 0;
	//������ĳɡ�xxx -2 xxxx -2 -1���ṹ
	int num;
	for (num = 0; iCards[num] != -1; num++);
	if (iCards[num - 1] != -2)
	{
		iCards[num] = -2;
		iCards[num + 1] = -1;
	}
	/////
	for (int i = 0; iCards[i] != -1; i++)
	{
		start = 0;
		if (iCards[i] == -2)
		{
			iCardsTmp[t] = -1;
			CInfo->SortById(iCardsTmp);
			int iTypeCount = CInfo->AnalyzeTypeCount(iCardsTmp);
			int iType = iTypeCount / 100;
			int iCardsCount = iTypeCount % 100;
			int myPoint = 0;
			//�������˳
			//if (iType == 9 && pDdz->iLastTypeCount / 100 == 10)
			//{
			//��Ҫ�����
			//iType = 10;//��ͬ�жϴ���
			//}
			//���������������Ҫ�Ե�������һ
			//if (iType == 5 && pDdz->iLastTypeCount / 100 == 6)
			//{
			//��Ҫ����Ǵ���
			//iType = 6;//��ͬ�ж�����
			//}


			if (pDdz->iLastMainPoint == -1 || pDdz->iLastTypeCount / 100 == iType)
			{
				if (pDdz->iLastTypeCount % 100 == iCardsCount || pDdz->iLastMainPoint == -1)
				{
					iCardsTmp[iCardsCount] = -1;
					//����Ϸ�,��ӵ�iSave
					myPoint = CInfo->AnalyzeMainMaxPoint(iCardsTmp);
					if (iLastMaxPoint<myPoint)
					{
						iCardsTmp[t] = -1;
						AppendCards(iCardsTmp, iSave);
						iNodesCount++;
					}
				}
			}
			//�����ը�����ϼҲ���ը��Ҳ���ǻ��
			if (iType == 2 && pDdz->iLastTypeCount / 100>2)
			{
				iCardsTmp[t] = -1;
				AppendCards(iCardsTmp, iSave);
				iNodesCount++;
			}
			//������
			if (iType == 1)
			{
				iCardsTmp[t] = -1;
				AppendCards(iCardsTmp, iSave);
				iNodesCount++;
			}

			t = 0;
		}
		else
		{
			iCardsTmp[t] = iCards[i];
			++t;
		}
	}
	return iNodesCount;
}

//���뵥��������Ϣ���ж��Ƿ�Ϸ�������Ϸ�������iplaarr
int RecommendCards::GetLegalCards(fDdz *pDdz, int *iCards)
{
	//���ơ���������˳��˫˳����˳��������űȽϴ�С������һ����˳���ơ��Ĵ�������������������˳�����������űȽϴ�С������ƴ�С�޹ء�
	//�������ը���������ը���������������ͣ�ը��֮�䰴���Ŵ�С���бȽϡ�
	int iCount = iPlaCount;
	int iCardsTmp[30];
	int t = 0;//����ָ��iCards�±�
	int iTypeCount;
	int iType;
	int iCardsCount;
	int myPoint;
	for (int i = 0; iCards[i] != -1; i++)
	{
		/////////��������//////////////////
		if (iCards[i] == -2)
		{
			iCardsTmp[t] = -1;
			CInfo->SortById(iCardsTmp);
			iTypeCount = CInfo->AnalyzeTypeCount(iCardsTmp);
				iType = iTypeCount / 100;
			iCardsCount = iTypeCount % 100;
			myPoint = CInfo->AnalyzeMainMaxPoint(iCardsTmp);
			iCardsTmp[t] = -2;
			t++;
			iCardsTmp[t] = -1;
			t = 0;
		////////////�ж�////////////////////
		if (pDdz->iLastTypeCount / 100 == iType)
		{
			if (pDdz->iLastTypeCount % 100 == iCardsCount)
			{
				if (iLastMaxPoint < myPoint)
				{
					AddInPlaArr(pDdz, iCardsTmp);
				}
			}
		}
		if (pDdz->iLastMainPoint == -1)
		{
			if (iTypeCount > 0)
			{
				AddInPlaArr(pDdz, iCardsTmp);
			}
		}

		//�����ը�����ϼҲ���ը��Ҳ���ǻ��
		if (iType == 2 && pDdz->iLastTypeCount / 100>2)
		{
			AddInPlaArr(pDdz, iCardsTmp);
		}
		//������
		if (iType == 1)
		{
			AddInPlaArr(pDdz, iCardsTmp);
		}
	}
	else
	{
		iCardsTmp[t] = iCards[i];
		++t;
	}
	}

	return iPlaCount - iCount;


}


//��һ�׷ֺ����͵��ƽ������ƴ洢��IPLARR��������
//TODO:�޸ľ�����Ϣ
//����޶���:�����Σ�����޶�ʱ��:16-08-1 23:50
//�޶����ݣ����ӽڵ�����
int RecommendCards::AddInPlaArr(fDdz *pDdz, int *iCards)
{
	for (int i = 0; iCards[i] != -1; i++)
	{
		if (iCards[i] != -2)
		{
			bool isIn = false;
			for (int j = 0; pDdz->iOnHand[j] != -1; j++)
			{
				if (iCards[i] == pDdz->iOnHand[j])
				{
					isIn = true;
					break;
				}
			}
			if (!isIn)
				return iPlaCount;
		}
	}
	int num = iPlaCount;
	int t = 0;//�ڶ�ά�����±�
	if (iCards[0] != -2)
	{
		for (int i = 0; iCards[i] != -1; i++)
		{

			if (iCards[i] != -2)
			{
				//����ڵ����Ѿ��������ޣ�ֱ�ӷ���
				if (num + 2 > PlaMax)
					return num;
				pDdz->iPlaArr[num][t] = iCards[i];
				t++;
			}
			else
			{
				pDdz->iPlaArr[num][t] = -1;
				t = 0;
				num++;
			}
		}
	}
	else
	{
		//����ڵ����Ѿ��������ޣ�ֱ�ӷ���
		if (num + 2 > PlaMax)
			return num;
		pDdz->iPlaArr[num][t] = -2;
		t++;
		pDdz->iPlaArr[num][t] = -1;
		num++;
	}
	iPlaCount = num;
	return num;
}




int RecommendCards::IfHelpEnemyCard(fDdz* pDdz)
{
	//�ǵ�������
	if (pDdz->cDir == pDdz->cLandlord)
		return 1;
	//���ǵ�������
	if (pDdz->iLastPassCount == 1)
	{
		if (pDdz->cLastPlay == pDdz->cLandlord)
			return 0;
		else
			return 1;
	}
	if (pDdz->iLastPassCount == 0)
	{
		if (pDdz->cLastPlay == pDdz->cLandlord)
			return 1;
		else
			return 0;
	}
	return 1;
}


// ����������������Ʋ�����iSave����
int RecommendCards::AnalyzeSplitedCards(int* iCards, int *iSave)
{
	int t = 0;//����ָ��iCards�±�
	for (int i = 0; iCards[i] != -1; i++)
	{

		if (iCards[i] != -2)
		{
			iSave[t] = iCards[i];
			++t;
		}
	}
	iSave[t] = -1;
	return t;
}


//��from����ӵ�to������棬to����ָ���Ϊ-2��������Ϊ-1
//�ӽ�������ʼд�����ݣ�����-2��-1��β
void RecommendCards::AppendCards(int* from, int* to)
{
	int i;
	//����to����-1����
	for (i = 0; to[i] != -1; i++);
	int j = i;

	for (i = 0; from[i] != -1; i++, j++)
	{
		to[j] = from[i];
	}
	//�ı��׺
	if (to[j - 1] != -2)
	{
		to[j] = -2;
		to[j + 1] = -1;
	}
}




// ����������С����
int RecommendCards::MinHandNum(int* iOnHand)
{
	int iCopyOnHand[21];
	//memcpy(iCopyOnHand, iOnHand, sizeof(iOnHand));//Ī��ʧЧ
	for (int i = 0; i < 21; i++)
	{
		iCopyOnHand[i] = iOnHand[i];
	}
	int num = 0;//����
	int t = 0;
	int j = 0;
	int bestStart = 0;
	int i = 0;
	int iTmpCards[21];
	memset(iTmpCards, -1, sizeof(iTmpCards));
	//�Ӳ��������У���Ѱ���������������,����icopyHand���ų�,����������
	auto findMaxNumCards = [&](int *iSplitedCards)
	{
		if (iSplitedCards[0] > -1)
		{
			t = 0;//���������
			j = 0;
			bestStart = 0;//���������ʼλ��
			for (i = 0; iSplitedCards[i] != -1; i++)
			{
				if (iSplitedCards[i] == -2)
				{
					if (j > t)
					{
						t = j;
						bestStart = i - t;
					}
					j = 0;
				}
				else
					j++;//����������
			}
		//	memcpy(iTmpCards, &iSplitedCards[bestStart], t*sizeof(int));
			t = 0;
			for (i = bestStart; iSplitedCards[i] != -2; i++)
			{
				iTmpCards[t] = iSplitedCards[i];
				t++;
			}
			iTmpCards[t] = -1;
			num++;
			//��iCopyOnHand���ų�
			if (iTmpCards[0] > -1)
			{
				for (i = 0; iTmpCards[i] != -1; i++)
				{
					DeleteElementInCards(iCopyOnHand,21, iTmpCards[i]);
				}
			}
		}
	};
	//�ж����������ų�,����������
	auto DeleteTypeCards = [&](int *iCards)
	{
		if (iCards[0] > -1)
		{
			t = 0;
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] == -2)
				{
					iTmpCards[t] = -1;
					for (j = 0; iTmpCards[j] != -1; j++)
					{
						DeleteElementInCards(iCopyOnHand, 21,iTmpCards[j]);
					}
					num++;
					t = 0;
					iTmpCards[0] = -1;
				}
				else
				{
					iTmpCards[t] = iCards[i];
					t++;
				}
			}
		}
	};

	//�����жϻ��
	int *iCards = pSplitCards->startSplitCard(iOnHand, 0);
	if (iCards[0] > -1)
	{
		DeleteElementInCards(iCopyOnHand,21,52);
		DeleteElementInCards(iCopyOnHand, 21, 53);
		num++;
	}
	//�ж�ը��
	iCards = pSplitCards->startSplitCard(iOnHand, 1);
	//�ж��м���ը��,���ų�(ͨ��)
	DeleteTypeCards(iCards);

	//�ҵ�������˳,���ų�
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 3);
	DeleteTypeCards(iCards);

	//�ж�˫˳,�ҵ�����˫˳,���ų�
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 5);
	DeleteTypeCards(iCards);
	//�жϵ�˳,�ҵ����е�˳,���ų�
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 4);
	DeleteTypeCards(iCards);
	//�ж�����
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);
	//�ж��м�������,����iCopyOnHand���ų�
	DeleteTypeCards(iCards);
	//�ж϶���
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);
	//�ж��м�������,���ų�
	DeleteTypeCards(iCards);
	//�жϵ���
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 7);
	DeleteTypeCards(iCards);
	//�ж���(ʣ���Ʊ�Ϊ����2�����Ʋ����)
	for (i = 0; iCopyOnHand[i] != -1; i++)
	{
		if (iCopyOnHand[i] >= 48)
			num++;
	}
	return num;
}




bool RecommendCards::GetModel(Ddz * realDdz)
{
	pRealDdz = realDdz;
	pMoldel->GetOtherPokersCards(iUpCards, iDownCards, pRealDdz);
	//����������
	int i = CInfo->CountCards(iUpCards);
	int j = CInfo->CountCards(iDownCards);
	if(pRealDdz->iUpPlayerCards != i || pRealDdz->iDownPlayerCards != j)
		return false;
	CInfo->SortById(iUpCards);
	CInfo->SortById(iDownCards);
	return true;
}

//�������Ʒ���false,�ǲ�������true
bool RecommendCards::AnalyzeCondition(fDdz* pDdz)
{
	int sort = 0;//����
	//������������
	if (pDdz->cLandlord == pDdz->cDir)
	{
		if (pDdz->iUpPlayerCards <= 6 || pDdz->iDownPlayerCards <= 6)
			sort++;
	}
	else
	{
		if (pDdz->cLastPlay == pDdz->cLandlord&&pDdz->iUpPlayerCards <= 6)
			sort++;
		if (pDdz->cLastPlay != pDdz->cLandlord&&pDdz->iDownPlayerCards <= 6)
			sort++;
	}

	//����������
	if (pDdz->cLandlord == pDdz->cDir)
	{
		if (iUpHandNum <= 3 || iDownHandNum <= 3)
			sort++;
		if (iUpHandNum < iHandNum&&iDownHandNum < iHandNum)
			sort++;
	}
	else
	{
		if (pDdz->cLastPlay == pDdz->cLandlord&&iUpHandNum <= 3)
			sort++;
		if (pDdz->cLastPlay != pDdz->cLandlord&&iDownHandNum <= 3)
			sort++;
	}
	//���Ƽ�ֵ����
	int MyValue = CardsValue(pDdz->iOnHand);
	int UpValue = CardsValue(iUpCards);
	int DownValue = CardsValue(iDownCards);
	if (pDdz->cLandlord == pDdz->cDir)
	{
		if (MyValue <UpValue&& MyValue <DownValue)
			sort++;

	}
	else
	{
		if (pDdz->cLastPlay == pDdz->cLandlord&&UpValue >MyValue&&UpValue>DownValue)
			sort++;
		if (pDdz->cLastPlay != pDdz->cLandlord&&DownValue >MyValue&&DownValue >UpValue)
			sort++;
	}
	if (sort >= 2)
		return false;
	else
	return true;
}


void RecommendCards::DeleteElementInCards(int* iCards, int iCardsCount, int iElement)
{
	int i, j;
	i = j = 0;
	for (j = 0; j < iCardsCount; j++)
	{
		if (iCards[j] == iElement)
			i++;
		else
			iCards[j - i] = iCards[j];

	}
}

//
int RecommendCards::AnalyzeSingleLink(int Length, int iLastMainPoint, int *Cards, int* iTmpShunzi)
{
	if (Length < 5||Length>12)
		return 0;
	//ȥ�����Ʋ���С��������
	int i, j, k;
	int iCards[21];
	for (i = 0; i < 21; i++)
	{
		iCards[i] = Cards[i];
	}
	CInfo->GetUnReaptedCards(iCards);
		int iBiaoji, iFlag, num;
		num = 0;
		for (i = 0; iCards[i + Length - 1] >= 0; i++)
		{
			num = 0;//������
			iBiaoji = 0;
			iFlag = 0;
			//�����������һ���Ƶ���С����
			if (iCards[i] / 4 > iLastMainPoint)
			{
				for (j = i; j < Length - 1 + i; j++)
				{
					//����������Ƶ㲻���򣬱�ǲ��˳�ѭ��
					if (iCards[j] / 4 != iCards[j + 1] / 4 - 1)
					{
						iBiaoji = 1;
						break;
					}
				}
			}
			else
				continue;
			if (iBiaoji == 0)
			{
				for (k = i; k < Length + i; k++)
				{
					if (iCards[k] < 0)
						break;
					iTmpShunzi[iFlag] = iCards[k];
					iFlag++;
					if (k == Length - 1 + i)
					{
						num = iFlag;
						iTmpShunzi[iFlag++] = -2;
						iTmpShunzi[iFlag++] = -1;
						return num;
					}
				}
			}
		}
		return num;
}


int RecommendCards::CardsValue(int* iCards)
{
	int i;
	int dSum = 100;			//��ֵ
	for (i = 0; iCards[i] >= 0; i++)
	{
		dSum = dSum - 5;			//����Խ��Խ��û��һ�������Ƽ�5��
		if (i >= 1 && iCards[i - 1] / 4 == iCards[i] / 4)
			dSum = dSum + 2;	//��������ͬ���2��
		if (i >= 4 && iCards[i - 4] / 4 <= 7
			&& iCards[i - 4] / 4 + 1 == iCards[i - 3] / 4
			&& iCards[i - 3] / 4 + 1 == iCards[i - 2] / 4
			&& iCards[i - 2] / 4 + 1 == iCards[i - 1] / 4
			&& iCards[i - 1] / 4 + 1 == iCards[i] / 4)
			dSum = dSum + 6;	//2�����������Ƶ�˳��6��
	}
	return dSum;
}
