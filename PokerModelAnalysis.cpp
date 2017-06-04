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
		Buffer[i] = pMyDdz->iOnOtherHand[i];//��������װ��һ������������
	}
}//��ʼ��������Ϣ
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
	//	if (InitialSum > Standard2)//����ֵ���ڱ�׼ֵ2���Ǻ���
	//	{
	//		LordBest();
	//	}
	//	if (InitialSum >= Standard1 && InitialSum <= Standard2)//����ֵ���ڱ�׼ֵ1�ͱ�׼ֵ2֮�����һ�����
	//	{
	//		Equality();
	//	}
	//	else
	//	{
	//		PartnerBest();
	//	}
	//}
	//else//����
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
	}//���Ǹ����յĳ����ƴ浽iCs����ȥ���ж�
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
		if (B->IsType3Single(iCs) == 1)//�ǵ��� ����
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
		//if (B->IsType7LinkSingle(iCs) == 1)//�ǵ�˳ ����
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
	one = pMyDdz->iUpPlayerCards - j;//��¼���˱��е��ƻ��ж�����Ҫ�ָ��ϼ�
	j = 0;
	while (iTwoDe[j] != -1)
	{
		j++;
	}
	if (j > pMyDdz->iDownPlayerCards)
		j = 0;
	two = pMyDdz->iDownPlayerCards - j;//��¼���˱��е��ƻ��ж�����Ҫ�ָ��¼�
	i = one + two;
	std::vector<int> vi(i);
	//srand(unsigned(time(NULL)));
	for (int n = 0; n<i; n++)
	{
		vi[n] = iBuffer[n];
	}
	random_shuffle(vi.begin(), vi.end());
	j = 0;
	vector<int>::iterator it;/*����˳��*/
	for (it = vi.begin(); it != vi.begin() + one; it++)
	{
		iOneUtimate[j] = *it;
		j++;
	}//���ϼ��������
	for (k = 0; iOneDe[k] != -1; k++)
	{
		iOneUtimate[j] = iOneDe[k];
		j++;
	}//���ϼұ��е����ӽ�ȥ}//���ϼұ��е����ӽ�ȥ
	j = 0;
	for (it = vi.begin() + one; it != vi.end(); it++)
	{
		iTwoUtimate[j] = *it;
		j++;
	}//���¼��������
	k = 0;
	while (iTwoDe[k] != -1)
	{

		iTwoUtimate[j] = iTwoDe[k];
		j++;
		k++;
	}
	//���¼ұ��е����ӽ�ȥ
}

void PokerModelAnalysis::Type3(int *iCs, int k)
{
	int a = iCs[0] / 4;
	for (int i = 0; Buffer[i] != -1; i++)/*Buffer�洢����NPC�Լ����е�17�����������*/
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
				i--;//����ȡ����������ɾ����
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
				i--;//����ȡ����������ɾ����
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

	if (iUtimate[0][0] != -1)//��û�ж���
	{
		InitialSum = InitialSum + 10;//��������ֵ
	}
	for (int i = 0; iUtimate[1][i] != -1; i++)//��û����ը�� ��ը����������5��
	{
		if (iUtimate[1][i] == -2)
		{
			i++;
			continue;
		}
		else
		{
			InitialSum = InitialSum + 9;
			i = i + 4;//��������ֵ
		}
	}
	for (int i = 0; iUtimate[2][i] != -1; i = i + 4)//��û������ ������������4��
	{
		InitialSum = InitialSum + 8;//��������ֵ
	}
	for (int i = 0; iUtimate[3][i] != -1; i++)
	{
		int j;
		for (j = 0; iUtimate[3][j] != -2; j = j + 3)
		{
		}
		InitialSum = InitialSum + 7;//����jֵ����ֵ�����������ֵ
		i = i + j;//��i������һ����˳������
	}
	for (int i = 0; iUtimate[4][i] != -1; i++)//��û��˳�� ��˳�Ӿͽ���ѭ��
	{
		int j;
		for (j = 0; iUtimate[4][j] != -2; j++)//��˳�� ��Jֵ�����˳�ӵĳ��ȣ�����5����
		{
		}
		InitialSum = InitialSum + 6;//����jֵ����ֵ�����������ֵ
		i = i + j;//ʹi������һ��˳�ӵ�����
	}
	for (int i = 0; iUtimate[5][i] != -1; i++)
	{
		int j;
		for (j = 0; iUtimate[5][j] != -2; j = j + 2)
		{
		}
		InitialSum = InitialSum + 5;//����jֵ����ֵ�����������ֵ
		i = i + j;//ʹi������һ�����Ե�����
	}
	for (int i = 0; iUtimate[6][i] != -1; i = i + 3)//��û�ж��� �ж��Ӿ�������3��
	{
		InitialSum = InitialSum + 4;//��������ֵ
	}
	for (int i = 0; iUtimate[7][i] != -1; i = i + 2)//��û�е��� �е��ƾ�������2��
	{
		InitialSum = InitialSum + 3;//��������ֵ
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
}//������������õ���

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
}//�����ѷ�����õ���

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
}//�����ҷ��������

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
	if (pMyDdz->cLandlord + 1 == pMyDdz->cDir)//�������ҷ��ϼ�
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