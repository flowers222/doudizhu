#include "PokerModelAnalysis.h"


void PokerModelAnalysis::Init()
{
	UpWrong = 0;
	DownWrong = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			iUtimate[i][j] = -1;
		}
	}
	for (int i = 0; i < 21; i++)
	{
		iOneUtimate[i] = -1;
	}
	for (int i = 0; i < 21; i++)
	{
		iTwoUtimate[i] = -1;
	}
	for (int i = 0; i < 38; i++)
	{
		Buffer[i] = pMyDdz->iOnOtherHand[i];//把其他牌装入一个缓冲数组存放
	}
}//初始化牌型信息
PokerModelAnalysis::PokerModelAnalysis()
{
	A = new SplitCard();
	B = new CardsInfo();
}
PokerModelAnalysis::~PokerModelAnalysis()
{
}
void PokerModelAnalysis::GetOtherPokersCards(int *iUpCards, int *iDownCards, Ddz *pDdz)
{
	pMyDdz = pDdz;
	Init();
	if (pMyDdz->iOTmax <= 2)
	{
		GameStart();
		Gaming();
	}
	else
	{
		Gaming();
	}
	for (int i = 0; i < 21; i++)
	{
		iUpCards[i] = iOneUtimate[i];
	}
	for (int i = 0; i < 21; i++)
	{
		iDownCards[i] = iTwoUtimate[i];
	}
}

void PokerModelAnalysis::GameStart()
{
	//CardsValue(Hc);
	//int size = sizeof(iUtimate);
	//int i = 0;
	//memcpy(iMyUtimate, iUtimate, size);
	//if (i = 0)
	//{
	//	if (InitialSum > Standard2)//牌力值大于标准值2就是好牌
	//	{
	//		LordBest();
	//	}
	//	if (InitialSum >= Standard1 && InitialSum <= Standard2)//牌力值介于标准值1和标准值2之间就是一般的牌
	//	{
	//		Equality();
	//	}
	//	else
	//	{
	//		PartnerBest();
	//	}
	//}
	//else//烂牌
	//{
	//	Equality();
	//}
	for (int i = 0; i < 21; i++)
	{
		iOneDe[i] = -1;
	}
	for (int i = 0; i < 21; i++)
	{
		iTwoDe[i] = -1;
	}
	if (pMyDdz->cDir != pMyDdz->cLandlord)
	{
		Fendipai();
	}
}

void PokerModelAnalysis::Gaming()
{
	DeleteC();
	Analysis(2);
	Analysis(1);
	Random(Buffer);
}

void PokerModelAnalysis::Analysis(int t)
{
	int iCs[21];
	for (int i = 0; i < 21; i++)
	{
		iCs[i] = -1;
	}//把那个狗日的出的牌存到iCs里面去做判断
	if (t == 2)
	{
		for (int i = 0; i < 21; i++)
		{
			iCs[i] = pMyDdz->iOnTable[pMyDdz->iOTmax - 2][i];
		}
		DeleteB(iCs, 2);
		if (B->IsType0Pass(iCs) == 1)
		{
			return;
		}
		if (B->IsType3Single(iCs) == 1)//是单牌 分析
		{
			DeleteB(iCs, 1);
			Type3(iCs, 2);
			return;
		}
		if (B->IsType4Double(iCs) == 1)
		{
			DeleteB(iCs, 1);
			Type3(iCs, 2);
			return;
		}
		if (B->IsType6ThreeOne(iCs) == 1)
		{
			DeleteB(iCs, 1);
			Type6(iCs, 2);
			return;
		}
		//if (B->IsType7LinkSingle(iCs) == 1)//是单顺 分析
		//{
		//	Type7(iCs, 2);
		//	return;
		//}
	}
	if (t == 1)
	{
		for (int i = 0; i < 21; i++)
		{
			iCs[i] = pMyDdz->iOnTable[pMyDdz->iOTmax - 1][i];
		}
		DeleteB(iCs, 1);
		if (B->IsType0Pass(iCs) == 1)
		{
			return;
		}
		if (B->IsType3Single(iCs) == 1)
		{
			DeleteB(iCs, 2);
			Type3(iCs, 1);
			return;
		}
		if (B->IsType4Double(iCs) == 1)
		{
			DeleteB(iCs, 2);
			Type3(iCs, 1);
			return;
		}
		if (B->IsType6ThreeOne(iCs) == 1)
		{
			DeleteB(iCs, 2);
			Type6(iCs, 1);
			return;
		}
		//if (B->IsType7LinkSingle(iCs) == 1)
		//{
		//	Type7(iCs, 1);
		//	return;
		//}
	}
}

void PokerModelAnalysis::Random(int *iBuffer)
{
	int i = 0;
	int j = 0;
	int one = 0;
	int two = 0;
	int k = 0;
	while (iOneDe[j] != -1)
	{
		j++;
	}
	if (j > pMyDdz->iUpPlayerCards)
		j = 0;
	one = pMyDdz->iUpPlayerCards - j;//记录除了必有的牌还有多少牌要分给上家
	j = 0;
	while (iTwoDe[j] != -1)
	{
		j++;
	}
	if (j > pMyDdz->iDownPlayerCards)
		j = 0;
	two = pMyDdz->iDownPlayerCards - j;//记录除了必有的牌还有多少牌要分给下家
	i = one + two;
	std::vector<int> vi(i);
	//srand(unsigned(time(NULL)));
	for (int n = 0; n<i; n++)
	{
		vi[n] = iBuffer[n];
	}
	random_shuffle(vi.begin(), vi.end());
	j = 0;
	vector<int>::iterator it;/*打乱顺序*/
	for (it = vi.begin(); it != vi.begin() + one; it++)
	{
		iOneUtimate[j] = *it;
		j++;
	}//给上家随机分配
	for (k = 0; iOneDe[k] != -1; k++)
	{
		iOneUtimate[j] = iOneDe[k];
		j++;
	}//吧上家必有的牌扔进去}//吧上家必有的牌扔进去
	j = 0;
	for (it = vi.begin() + one; it != vi.end(); it++)
	{
		iTwoUtimate[j] = *it;
		j++;
	}//给下家随机分配
	k = 0;
	while (iTwoDe[k] != -1)
	{

		iTwoUtimate[j] = iTwoDe[k];
		j++;
		k++;
	}
	//把下家必有的牌扔进去
}

void PokerModelAnalysis::Type3(int *iCs, int k)
{
	int a = iCs[0] / 4;
	for (int i = 0; Buffer[i] != -1; i++)/*Buffer存储除了NPC自己手中的17张牌以外的牌*/
	{
		if (a == Buffer[i] / 4)
		{
			if (k == 2)
			{
				int j = 0;
				while (iOneDe[j] != -1)
				{
					j++;
				}
				iOneDe[j] = Buffer[i];
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];
				}
				i--;//把提取出来的数据删除掉
			}
			if (k == 1)
			{
				int j = 0;
				while (iTwoDe[j] != -1)
				{
					j++;
				}
				iTwoDe[j] = Buffer[i];
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];
				}
				i--;//把提取出来的数据删除掉
			}
		}
	}
	Random(Buffer);
}

void PokerModelAnalysis::Type6(int *iCs, int k)
{
	int a = 0;
	int b = 0;
	a = iCs[0] / 4;
	for (int i = 0; i < 4; i++)
	{
		if (iCs[i] / 4 != a)
		{
			b = iCs[i] / 4;
		}
	}
	if (k = 2)
	{
		for (int i = 0; Buffer[i] != -1; i++)
		{
			if (Buffer[i] / 4 == a)
			{
				int j = 0;
				while (iOneDe[j] != -1)
				{
					j++;
				}
				iOneDe[j] = Buffer[i];
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];
				}
				i--;
			}
			if (Buffer[i] / 4 == b)
			{
				int j = 0;
				while (iOneDe[j] != -1)
				{
					j++;
				}
				iOneDe[j] = Buffer[i];
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];
				}
				i--;
			}
		}
	}
	if (k = 1)
	{
		for (int i = 0; Buffer[i] != -1; i++)
		{
			if (Buffer[i] / 4 == a)
			{
				int j = 0;
				while (iTwoDe[j] != -1)
				{
					j++;
				}
				iTwoDe[j] = Buffer[i];
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];
				}
				i--;
			}
			if (Buffer[i] / 4 == b)
			{
				int j = 0;
				while (iTwoDe[j] != -1)
				{
					j++;
				}
				iTwoDe[j] = Buffer[i];
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];
				}
				i--;
			}
		}
	}
}

void PokerModelAnalysis::Type7(int *iCs, int k)
{
	int a = 0;
	int b = 0;
	a = iCs[0] / 4 - 1;
	for (int i = 0; iCs[i] != -1; i++)
	{
		b = iCs[i] / 4 + 1;
	}
	if (k == 2)
	{
		int x = 0;
		while (iOneDe[x] != -1)
		{
			x++;
		}
		for (int i = 0; Buffer[i] != -1; i++)
		{
			if (Buffer[i] / 4 == a)
			{
				iOneDe[x] = Buffer[i];
				x++;
			}
		}
		if (b != 12)
		{
			for (int i = 0; Buffer[i] != -1; i++)
			{
				if (Buffer[i] / 4 == b)
				{
					iOneDe[x] = Buffer[i];
					x++;
				}
			}
		}
	}
	if (k == 1)
	{
		int x = 0;
		while (iTwoDe[x] != -1)
		{
			x++;
		}
		for (int i = 0; Buffer[i] != -1; i++)
		{
			if (Buffer[i] / 4 == a)
			{
				iTwoDe[x] = Buffer[i];
				x++;
			}
		}
		if (b != 12)
		{
			for (int i = 0; Buffer[i] != -1; i++)
			{
				if (Buffer[i] / 4 == b)
				{
					iTwoDe[x] = Buffer[i];
					x++;
				}
			}
		}
	}
}
void PokerModelAnalysis::CardsValue(int *pCards)
{
	InitialSum = 0;
	ConfirmCards(pCards);

	if (iUtimate[0][0] != -1)//有没有对王
	{
		InitialSum = InitialSum + 10;//增加牌力值
	}
	for (int i = 0; iUtimate[1][i] != -1; i++)//有没有有炸弹 有炸弹就往后推5格
	{
		if (iUtimate[1][i] == -2)
		{
			i++;
			continue;
		}
		else
		{
			InitialSum = InitialSum + 9;
			i = i + 4;//增加牌力值
		}
	}
	for (int i = 0; iUtimate[2][i] != -1; i = i + 4)//有没有三条 有三条往后推4格
	{
		InitialSum = InitialSum + 8;//增加牌力值
	}
	for (int i = 0; iUtimate[3][i] != -1; i++)
	{
		int j;
		for (j = 0; iUtimate[3][j] != -2; j = j + 3)
		{
		}
		InitialSum = InitialSum + 7;//根据j值和首值编码计算牌力值
		i = i + j;//是i跳到下一个三顺的区域
	}
	for (int i = 0; iUtimate[4][i] != -1; i++)//有没有顺子 有顺子就进入循环
	{
		int j;
		for (j = 0; iUtimate[4][j] != -2; j++)//有顺子 用J值来检测顺子的长度（至少5个）
		{
		}
		InitialSum = InitialSum + 6;//根据j值和首值编码计算牌力值
		i = i + j;//使i跳到下一个顺子的区域
	}
	for (int i = 0; iUtimate[5][i] != -1; i++)
	{
		int j;
		for (j = 0; iUtimate[5][j] != -2; j = j + 2)
		{
		}
		InitialSum = InitialSum + 5;//根据j值和首值编码计算牌力值
		i = i + j;//使i跳到下一个连对的区域
	}
	for (int i = 0; iUtimate[6][i] != -1; i = i + 3)//有没有对子 有对子就往后推3格
	{
		InitialSum = InitialSum + 4;//增加牌力值
	}
	for (int i = 0; iUtimate[7][i] != -1; i = i + 2)//有没有单牌 有单牌就往后推2格
	{
		InitialSum = InitialSum + 3;//增加牌力值
	}
}

void PokerModelAnalysis::LordBest()
{
	//ConfirmCards(pMyDdz->iOnOtherHand);
	//Undone
	int k = 0;
	if (k = 0)
	{
		for (int i = 0; i < 37; i++)
		{
			if (i < 17)
			{
				iOneUtimate[i] = pMyDdz->iOnOtherHand[i];
			}
			else
			{
				iTwoUtimate[k] = pMyDdz->iOnOtherHand[i];
			}
		}
	}
	else
	{
		for (int i = 0; i < 37; i++)
		{
			if (i < 20)
			{
				iTwoUtimate[i] = pMyDdz->iOnOtherHand[i];
			}
			else
			{
				iOneUtimate[k] = pMyDdz->iOnOtherHand[i];
				k++;
			}
		}
	}
}//给地主分配最好的牌

void PokerModelAnalysis::PartnerBest()
{
	int k = 0;
	if (k = 0)
	{
		for (int i = 0; i < 37; i++)
		{
			if (i < 20)
			{
				iTwoUtimate[i] = pMyDdz->iOnOtherHand[i];
			}
			else
			{
				iOneUtimate[k] = pMyDdz->iOnOtherHand[i];
				k++;
			}
		}
	}
	else
	{
		for (int i = 0; i < 37; i++)
		{
			if (i < 17)
			{
				iOneUtimate[i] = pMyDdz->iOnOtherHand[i];
			}
			else
			{
				iTwoUtimate[k] = pMyDdz->iOnOtherHand[i];
				k++;
			}
		}
	}
}//给队友分配最好的牌

void PokerModelAnalysis::Equality()
{
	int k = 0;
	int j = 0;
	for (int i = 0; i < 37; i++)
	{
		if (i % 2 == 0 && k < 17)
		{
			iOneUtimate[k] = pMyDdz->iOnOtherHand[i];
			k++;
		}
		else
		{
			iTwoUtimate[j] = pMyDdz->iOnOtherHand[i];
			j++;
		}
	}
}//给两家分配差不多的牌

void PokerModelAnalysis::SaveCards(int j, int *pCards)
{
	int *abc = A->startSplitCard(pCards, j);
	int i;
	for (i = 0; abc[i] != -1; i++)
	{
		iUtimate[j][i] = abc[i];
	}
	if (abc[0] != -1)
	{
		DeleteCards(abc);
	}
}

void PokerModelAnalysis::DeleteCards(int *iResult)
{
	int k = 0;
	for (int i = 0; iResult[i] != -1; i++)
	{
		if (iResult[i] != -2)
		{
			for (k = 0; k < 21; k++)
			{
				if (iResult[i] == Hc[k])
				{
					for (; k < 20; k++)
					{
						Hc[k] = Hc[k + 1];
					}
					break;
				}
			}
		}
	}
}

void PokerModelAnalysis::ConfirmCards(int *pCards)
{
	SaveCards(0, pCards);
	SaveCards(1, pCards);
	SaveCards(2, pCards);
	SaveCards(3, pCards);
	SaveCards(4, pCards);
	SaveCards(5, pCards);
	SaveCards(6, pCards);
	SaveCards(7, pCards);
}

void PokerModelAnalysis::DeleteB(int *iCs, int a)
{
	if (a == 2)
	{
		for (int i = 0; iTwoDe[i] != -1; i++)
		{
			for (int j = 0; iCs[j] != -1; j++)
			{
				if (iTwoDe[i] == iCs[j])
				{
					for (int k = i; iTwoDe[k] != -1; k++)
					{
						iTwoDe[k] = iTwoDe[k + 1];
					}
					i--;
					j = 0;
				}
			}
		}
		for (int i = 0; iOneDe[i] != -1; i++)
		{
			for (int j = 0; iCs[j] != -1; j++)
			{
				if (iOneDe[i] == iCs[j])
				{
					for (int k = i; iOneDe[k] != -1; k++)
					{
						iOneDe[k] = iOneDe[k + 1];
					}
					UpWrong++;
					i--;
					j = 0;
				}
			}
		}
	}
	if (a == 1)
	{
		for (int i = 0; iTwoDe[i] != -1; i++)
		{
			for (int j = 0; iCs[j] != -1; j++)
			{
				if (iTwoDe[i] == iCs[j])
				{
					for (int k = i; iTwoDe[k] != -1; k++)
					{
						iTwoDe[k] = iTwoDe[k + 1];
					}
					DownWrong++;
					i--;
					j = 0;
				}
			}
		}
		for (int i = 0; iOneDe[i] != -1; i++)
		{
			for (int j = 0; iCs[j] != -1; j++)
			{
				if (iOneDe[i] == iCs[j])
				{
					for (int k = i; iOneDe[k] != -1; k++)
					{
						iOneDe[k] = iOneDe[k + 1];
					}
					i--;
					j = 0;
				}
			}
		}
	}
}

void PokerModelAnalysis::Fendipai()
{
	if (pMyDdz->cLandlord + 1 == pMyDdz->cDir)//地主是我方上家
	{
		for (int i = 0; i < 3; i++)
		{
			iOneDe[i] = pMyDdz->iLef[i];
		}
		for (int i = 0; Buffer[i] != -1; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (Buffer[i] == pMyDdz->iLef[j])
				{
					for (int k = i; Buffer[k] != -1; k++)
					{
						Buffer[k] = Buffer[k + 1];
					}
					i--;
				}
			}
		}
	}
	if (pMyDdz->cLandlord - 1 == pMyDdz->cDir)
	{
		for (int i = 0; i < 3; i++)
		{
			iTwoDe[i] = pMyDdz->iLef[i];
		}
		for (int i = 0; Buffer[i] != -1; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (Buffer[i] == pMyDdz->iLef[j])
				{
					for (int k = i; Buffer[k] != -1; k++)
					{
						Buffer[k] = Buffer[k + 1];
					}
					i--;
				}
			}
		}
	}
}

void PokerModelAnalysis::DeleteC()
{
	for (int i = 0; iOneDe[i] != -1; i++)
	{
		for (int j = 0; Buffer[j] != -1; j++)
		{
			if (Buffer[j] == iOneDe[i])
			{
				for (int k = j; Buffer[k] != -1; k++)
				{
					Buffer[k] = Buffer[k + 1];
				}
				i--;
				j = 0;
			}
		}
	}
	for (int i = 0; iTwoDe[i] != -1; i++)
	{
		for (int j = 0; Buffer[j] != -1; j++)
		{
			if (Buffer[j] == iTwoDe[i])
			{
				for (int k = j; Buffer[k] != -1; k++)
				{
					Buffer[k] = Buffer[k + 1];
				}
				i--;
				j = 0;
			}
		}
	}
}