#include "CardsInfo.h"




CardsInfo::CardsInfo()
{
}
CardsInfo::~CardsInfo()
{
}

CardsInfo* CardsInfo::Instance()
{
	static CardsInfo instance;

	return &instance;
}

//B00-START�жϳ��������Ƿ�Ϊ��Ȩ
//����޶���:÷��,����޶�ʱ��:15-02-10
int CardsInfo::IsType0Pass(int iCs[])
{
	int iCount;
	iCount = CountCards(iCs);/*countcards�Ǽ������� 585��*/
	if (iCount == 0)/*��δ����κ�һ���ƶ������ʱ���ж�Ϊ����*/
		return 1;
	return 0;
}
//B00-END

//B01-START�жϳ��������Ƿ�Ϊ���
//����޶���:�ĺ��н�&÷��,����޶�ʱ��:15-02-10
int CardsInfo::IsType1Rocket(int iCs[])
{
	if ((iCs[2] == -1) && (iCs[0] + iCs[1] == 52 + 53))/*û�е���������С����52������53����Ϊ52,53�����ģ�����iCs[0] �� iCs[1]һ���Ǵ�С��*/
		return 1;
	return 0;
}
//B01-END

//B02-START�жϳ��������Ƿ�Ϊը��
//����޶���:�ĺ��н�&÷��,����޶�ʱ��:15-03-10
//�޶����ݼ�Ŀ��:��ֹ���ǿյ�
int CardsInfo::IsType2Bomb(int iCs[])
{
	if (4 != CountCards(iCs))
		return 0;
	if ((iCs[4] == -1) && (iCs[0] / 4 != -1 && iCs[0] / 4 == iCs[1] / 4 && iCs[0] / 4 == iCs[2] / 4 && iCs[0] / 4 == iCs[3] / 4))/*��ɫ��һ��������������һ��ѭ��*/
		return 1;
	return 0;
}
//B02-END

//B03-START�жϳ��������Ƿ�Ϊ����
//����޶���:�ĺ��н�,����޶�ʱ��:15-03-10
//�޶����ݼ�Ŀ��:if�ж���������
int CardsInfo::IsType3Single(int iCs[])
{
	if (iCs[0] != -1 && iCs[1] == -1)
		return 1;
	return 0;
}
//B03-END

//B04-START�жϳ��������Ƿ�Ϊ����
//����޶���:�ĺ��н�,����޶�ʱ��:15-02-13
int	CardsInfo::IsType4Double(int iCs[])
{
	if (IsType1Rocket(iCs))
		return 0;
	if (iCs[0] / 4 == iCs[1] / 4 && iCs[0] != -1 && iCs[2] == -1)
		return 1;
	return 0;
}
//B04-END

//B05-�жϳ��������Ƿ�Ϊ����
//����޶���:�ĺ��н�,����޶�ʱ��:15-02-13
int	CardsInfo::IsType5Three(int iCs[])
{
	if (iCs[0] != -1 && iCs[0] / 4 == iCs[1] / 4 && iCs[0] / 4 == iCs[2] / 4 && iCs[3] == -1)
		return 1;
	return 0;
}
//B05-END

//B0601-START�жϳ��������Ƿ�Ϊ����һ��
//����޶���:�ĺ��н�,����޶�ʱ��:15-02-12
int CardsInfo::IsType6ThreeOne(int iCs[])
{
	if (IsType2Bomb(iCs) || 4 != CountCards(iCs))
		return 0;
	SortByMuch(iCs);/*  �����ã�������(ͬ������)������������   618��*/
	if (iCs[0] / 4 == iCs[1] / 4 && iCs[0] / 4 == iCs[2] / 4)
		return 1;
	return 0;
}
//B0601-END

//B0602-START�жϳ��������Ƿ�Ϊ����һ��
//����޶���:�ĺ��н�,����޶�ʱ��:15-02-12
int CardsInfo::IsType6ThreeDouble(int iCs[])
{
	if (5 != CountCards(iCs))
		return 0;
	SortByMuch(iCs);/*��ͬ��������������*/
	if (iCs[0] / 4 == iCs[1] / 4 && iCs[0] / 4 == iCs[2] / 4)
		if (iCs[3] / 4 == iCs[4] / 4)
			return 1;
	return 0;
}
//B0602-END

//B07-START�жϳ��������Ƿ�Ϊ��˳
/*��˳������  ˳��*/
//����޶���:л��,����޶�ʱ��:15-02-12
int CardsInfo::IsType7LinkSingle(int iCs[])
{
	int iCount;
	int i;
	iCount = CountCards(iCs);
	if (iCount >= 5)
	{
		SortById(iCs);/*600��  ���Ʊ�����������*/
		for (i = 1; iCs[i] >= 0; i++)
		if (iCs[i - 1] / 4 + 1 != iCs[i] / 4 || iCs[i] >= 48)
		return 0;
		return 1;
		/*����д��if (iCs[i - 1] / 4 + 1 = iCs[i] / 4 && iCs[i] < 48)	return 1; */
	}
	return 0;
}
//B07-END

//B08-START�жϳ��������Ƿ�Ϊ����
//����޶���:л��&�ĺ��н�,����޶�ʱ��:15-03-10
int CardsInfo::IsType8LinkDouble(int iCs[])
{
	int iCount = CountCards(iCs);/*������*/
	int i;
	if (iCount < 6 || iCount % 2 != 0) /*����������������6��Ҳ�������ǵ���*/
		return 0;
	SortById(iCs);   //��iCs������   /*���Ʊ�����������*/
	for (i = 1; i < iCount; i++)
	{
		//�ж�i�ǵ�������˫��
		if (i % 2)   /*��˫�ж����������ˣ�*/  /*֮�����á������ڡ��ж�  ��Ϊ�ж���ж϶��ҵ�һ����û����һ���ƿɱȽ�*/
		{
			//���i�ǵ��������i���Ƶı������������һ���Ʊ���+1
			if (iCs[i] / 4 != iCs[i - 1] / 4)
				return 0;
		}
		else
		{
			//���i��˫�������i���Ƶı������������һ���Ʊ���
			if (iCs[i] / 4 != iCs[i - 1] / 4 + 1)
				return 0; 
		}
	}
	return 1;
}
//B08-END

//B09-START�жϳ��������Ƿ�Ϊ��˳
//����޶���:������,����޶�ʱ��:16-08-12
//�޶���iTempArr�����С����
int	CardsInfo::IsType9LinkThree(int iCs[])
{
	int iTempArr[12] = { 0 };   //��ʼ������iTempArr������¼ 3 - A ÿ������������
	int iCount = CountCards(iCs);
	int i, iMinNum, iMaxNum;   //iMinNum Ϊ iCs ��С����,iMaxNum Ϊ iCs ������
	if (iCount < 6 || iCount % 3 != 0)
		return 0;
	SortById(iCs);   //��iCs������
	iMinNum = iCs[0] / 4;
	iMaxNum = iMinNum + iCount / 3 - 1;
	for (i = 0; i < iCount; i++)
	{
		//�ж�iCs[i]�Ƿ�����Ч������Χ��
		if (iCs[i] > 47 || iCs[i] / 4 < iMinNum || iCs[i] / 4 > iMaxNum)
		{
			return 0;
		}
		iTempArr[iCs[i] / 4]++;
	}

	for (i = iMinNum; i <= iMaxNum; i++)
	{
		//�ж��Ƿ�ÿ����Ч������Ϊ3����
		if (iTempArr[i] != 3)
			return 0;
	}
	return 1;

}
//B09-END

//B1001-START�ж���˳����������1�ǣ�0����
//����޶���:�ĺ��н�,����޶�ʱ��:15-03-10
//�޶����ݼ�Ŀ��:��ֹ44455556�ĳ���ʱ�����˳����
int	CardsInfo::IsType10LinkThreeSingle(int iCs[])
{
	int iCount = CountCards(iCs);
	int iTempArr[18];
	int i, n, m, j, iNum, iTemp; //iNum��¼�ж��ٸ� 3+1
	if (iCount < 8 || iCount % 4 != 0)
		return 0;
	memset(iTempArr, -1, 18 * 4);   //��ʼ��iTempArr��ֵ��Ϊ-1
	iNum = iCount / 4;
	SortByMuch(iCs);   //����
	//�ж��ǲ�����ը��
	n = 1, m = 0;
	while (n)
	{
		for (i = m; i < m + 4; i++)
			iTempArr[i] = iCs[i];

		//�ж�iTempArr�ǲ���ը��������������
		if (0 == IsType2Bomb(iTempArr))
		{
			n = 0;
		}
		else
		{
			//����ǵĻ�,��ô���ը��Ӧ����һ��˳�Ӽ�һ�����ƣ����ž�Ӧ�÷���iCs�ĺ���
			iTemp = iCs[m];
			for (j = m + 1; j < iCount; j++)
			{
				iCs[j - 1] = iCs[j];
				iCs[j] = iTemp;
				iTemp = iCs[j];
			}
			m = m + 3;
		}
		memset(iTempArr, -1, 18 * 4);
	}

	//����˳��ֵ��iTempArr
	for (i = 0; i < 3 * iNum; i++)
	{
		iTempArr[i] = iCs[i];
	}

	//�ж�iTempArr�ǲ�����˳
	if (1 == IsType9LinkThree(iTempArr))
	{
		//��iTempArr����iCs   ��ֹ55544465���������ͳ���
		for (i = 0; i < 3 * iNum; i++)
		{
			iCs[i] = iTempArr[i];
		}
		return 1;
	}
	return 0;

}
//B1001-END


//B1002-START   �ж���˳���ԣ�����1�ǣ�0����
//����޶���:�ĺ��н�,����޶�ʱ��:15-02-13
int	CardsInfo::IsType10LinkThreeDouble(int	iCs[])
{
	int iCount = CountCards(iCs);
	int iTempArr[18];
	int i, n, j, k, iNum, iTemp; //iNum��¼�ж��ٸ� 3+2
	if (iCount < 10 || iCount % 5 != 0)
		return 0;
	memset(iTempArr, -1, 18 * 4);   //��ʼ��iTempArr��ֵ��Ϊ-1
	iNum = iCount / 5;
	SortByMuch(iCs);   //����
	//�ж��ǲ�����ը��
	n = 1;
	while (n)
	{
		for (i = 0; i < 4; i++)
		{
			iTempArr[i] = iCs[i];
		}

		//�ж�iTempArr�ǲ���ը��������������
		if (0 == IsType2Bomb(iTempArr))
		{
			n = 0;
		}
		else
		{
			//����ǵĻ�,��ô���ը��Ӧ��������2�����ӣ���Ӧ�÷���iCs�ĺ���
			for (k = 0; k < 4; k++)
			{
				iTemp = iCs[0];
				for (j = 1; j < iCount; j++)
				{
					iCs[j - 1] = iCs[j];
					iCs[j] = iTemp;
					iTemp = iCs[j];
				}
			}
		}
		memset(iTempArr, -1, 18 * 4);
	}

	//����˳��ֵ��iTempArr
	for (i = 0; i < 3 * iNum; i++)
	{
		iTempArr[i] = iCs[i];
	}

	//�ж�iTempArr�ǲ�����˳
	if (0 == IsType9LinkThree(iTempArr))
		return 0;

	//����iTempArr
	memset(iTempArr, -1, 18 * 4);

	j = 0;
	for (i = 3 * iNum; i < iCount; i++)
	{

		iTempArr[j] = iCs[i];
		if (j == 1)
		{
			//�ж��ǲ��Ƕ���
			if (0 == IsType4Double(iTempArr))
				return 0;
			memset(iTempArr, -1, 18 * 4);
			j = 0;
		}
		else
		{
			j++;
		}
	}

	return 1;
}
//B1002-END

//B1101-START�жϳ��������Ƿ�Ϊ�Ĵ�����
//����޶���:÷��,����޶�ʱ��:15-02-10
int CardsInfo::IsType11FourSingle(int iCs[])
{
	int iCount;
	iCount = CountCards(iCs);
	SortByMuch(iCs);  //ͬ�������Ƶ�����ǰ��
	if (iCount == 6)  //6=4+1+1
		if (iCs[0] / 4 == iCs[1] / 4 && iCs[0] / 4 == iCs[2] / 4 && iCs[0] / 4 == iCs[3] / 4)  //����
			return 1;
	return 0;
}
//B1101-END

//B1102-START�жϳ��������Ƿ�Ϊ�Ĵ�����
//����޶���:÷��,����޶�ʱ��:15-02-10
int CardsInfo::IsType11FourDouble(int iCs[])
{
	int iCount;
	iCount = CountCards(iCs);
	SortByMuch(iCs);  //ͬ�������Ƶ�����ǰ��
	if (iCount == 8)
		if (iCs[0] / 4 == iCs[1] / 4 && iCs[0] / 4 == iCs[2] / 4 && iCs[0] / 4 == iCs[3] / 4)  //����
			if (iCs[4] / 4 == iCs[5] / 4 && iCs[6] / 4 == iCs[7] / 4)  //8=4+2+2
				return 1;
	return 0;
}
//B1102-END

//I03-START�������ͺ�����
//����޶���:÷��,����޶�ʱ��:15-03-01
int CardsInfo::AnalyzeTypeCount(int iCards[])
{
	int iCount = 0;
	iCount = CountCards(iCards);
	if (IsType0Pass(iCards))
		return 0 * 100 + iCount;
	if (IsType1Rocket(iCards))
		return 1 * 100 + iCount;
	if (IsType2Bomb(iCards))
		return 2 * 100 + iCount;
	if (IsType3Single(iCards))
		return 3 * 100 + iCount;
	if (IsType4Double(iCards))
		return 4 * 100 + iCount;
	if (IsType5Three(iCards))
		return 5 * 100 + iCount;
	if (IsType6ThreeOne(iCards))
		return 6 * 100 + iCount;
	if (IsType6ThreeDouble(iCards))
		return 6 * 100 + iCount;
	if (IsType11FourSingle(iCards))
		return 11 * 100 + iCount;
	if (IsType11FourDouble(iCards))
		return 11 * 100 + iCount;
	if (IsType7LinkSingle(iCards))
		return 7 * 100 + iCount;
	if (IsType8LinkDouble(iCards))
		return 8 * 100 + iCount;
	if (IsType9LinkThree(iCards))
		return 9 * 100 + iCount;
	if (IsType10LinkThreeSingle(iCards))
		return 10 * 100 + iCount;
	if (IsType10LinkThreeDouble(iCards))
		return 10 * 100 + iCount;


	return -1;
}
//I03-END

//I04-START����������С����
//����޶���:÷��,����޶�ʱ��:15-03-01
int	CardsInfo::AnalyzeMainPoint(int iCards[])
{
	if (IsType0Pass(iCards))
		return -1;
	SortByMain(iCards);
	return iCards[0] / 4;
}
//I04-END



// ��������������
//����޶���:�����Σ�����޶�ʱ��:16-08-08
//�޶����ݣ������д���������С��������
int CardsInfo::AnalyzeMainMaxPoint(int iCards[])
{
	if (IsType0Pass(iCards))
		return -1;
	int iTypeCount = AnalyzeTypeCount(iCards);
	int iCardsCount = iTypeCount % 100;
	SortByMuch(iCards);
	//��ը������
	if (iTypeCount / 100 == 1 || iTypeCount / 100 == 2)
	{
		return iCards[0] / 4;
	}
	//������Ĵ���
	if (1100 < iTypeCount &&iTypeCount< 1200)
	{
		//����Ǵ�����
		if (IsType11FourSingle(iCards))
		{
			for (int i = 0; i < iCardsCount; i++)
			{
				if (iCards[i] / 4 == iCards[i + 1] / 4)
				{
					//�ҵ��������
					return iCards[i] / 4;
				}
			}
			return -1;
		}
		else if (IsType11FourDouble(iCards))
		{
			int iRepeat = 0;
			for (int i = 0; i < iCardsCount; i++)
			{

				if (iCards[i] / 4 == iCards[i + 1] / 4)
				{
					//�ҵ��������
					iRepeat++;
				}
				else
				{
					iRepeat = 0;
				}
				if (iRepeat >= 2)
				{
					return iCards[i] / 4;
				}
			}
			return -1;
		}
	}
	//���ƣ�
	if (iTypeCount / 100 == 3)
	{
		//����Ǵ���
		if (iCards[0] == 53)
			return iCards[0] / 4 + 1;
		return iCards[0] / 4;
	}
	//���ƣ�����,
	if (400 < iTypeCount &&iTypeCount< 600)
	{
		for (int i = 0; i < iCardsCount; i++)
		{
			if (iCards[i] / 4 == iCards[i + 1] / 4)
			{
				//�ҵ��������
				return iCards[i] / 4;
			}
		}
		return -1;
	}
	//����һ
	if (600< iTypeCount &&iTypeCount< 700)
	{
		//���������
		if (IsType6ThreeOne(iCards))
		{
			for (int i = 0; i < iCardsCount; i++)
			{
				if (iCards[i] / 4 == iCards[i + 1] / 4)
				{
					//�ҵ��������
					return iCards[i] / 4;
				}
			}
		}
		//�������˫
		else if (IsType6ThreeDouble(iCards))
		{
			int iRepeat = 0;
			for (int i = 0; i < iCardsCount; i++)
			{
				if (iRepeat >= 2)
				{
					return iCards[i] / 4;
				}
				if (iCards[i] / 4 == iCards[i + 1] / 4)
				{
					//�ҵ��������
					iRepeat++;
				}
			}
		}
		return -1;
	}
	//�����˳�ӣ���˳��˫˳����˳
	//˳�ӵĹؼ���������
	if (700 < iTypeCount &&iTypeCount< 1000)
	{
		int i = 0;
		for (i = 0; iCards[i] > -1; i++);
		return iCards[i - 1] / 4;
	}
	//�������˳����
	if (10 * 100 < iTypeCount&&iTypeCount < 11 * 100)
	{

		//�������˳����
		//����x���ƣ���iCardsCount=x+3*x;��x=iCardsCount/4;
		//������˳��3*x=3*(iTypeCount/400)=0.0075*iTypeCount;
		if (IsType10LinkThreeSingle(iCards))
		{
			//��˳һ���������
			int count = 3 * (iCardsCount / 4);//��˳������
			for (int i = 0; i < iCardsCount; i++)
			{
				if (iCards[i] / 4 == iCards[i + 1] / 4)
				{
					//�ҵ��������
					i = i + count - 1;
					return iCards[i] / 4;
				}
			}
			return -1;

		}
		//�������˳��˫
		//����x�Զ��ƣ�2*x+3*x=iCardsCount x=iCardsCount /5

		if (IsType10LinkThreeDouble(iCards))
		{
			int count = 3 * (iCardsCount / 5);//��˳������
			for (int i = 0; i < iCardsCount; i++)
			{
				if (iCards[i] / 4 == iCards[i + 1] / 4)
				{
					//�ҵ��������
					i = i + count - 1;
					return iCards[i] / 4;
				}
			}
		}
	}
	return -1;
}


//I01-START��������
//����޶���:÷��,����޶�ʱ��:15-02-11
/*��������*/
int CardsInfo::CountCards(int iCards[])
{
	int iCount = 0;
	while (iCards[iCount]>-1)
		iCount++;
	return iCount;
}
//I01-END



//MO1-START���Ʊ�����������
//����޶���:�ĺ��н�&÷��,����޶�ʱ��:15-02-09
void CardsInfo::SortById(int iCards[])
{
	int i, j;
	int iTemp;
	for (i = 0; iCards[i] >= 0; i++)
	{
		for (j = i + 1; iCards[j] >= 0; j++)
			if (iCards[i]>iCards[j])
			{
				iTemp = iCards[i];
				iCards[i] = iCards[j];
				iCards[j] = iTemp;
			}
	}
}
//MO1-END

//M02-START������(ͬ������)������������
//����޶���:�ĺ��н�,����޶�ʱ��:2015-03-01
void CardsInfo::SortByMuch(int iCards[])
{
	int i, j, k, n;
	// iTempArr[i][0]����װ����Ϊ i �ж������ƣ�
	// iTempArr[i][1],iTempArr[i][2],iTempArr[i][3],iTempArr[i][4]����װ����Ϊ i ���Ƶı��룬
	int iTempArr[13][5];
	memset(iTempArr, -1, 13 * 5 * 4);/* c/c++����  ͷ�ļ�string   ���� char a[20]--- memset(a,0,20*sizeof(char));
�������ͣ���s�е�ǰλ�ú����n���ֽ� ��typedef unsigned int size_t ���� ch �滻������ s ��
���ã�����һ���ڴ�������ĳ��������ֵ�����ǶԽϴ�Ľṹ�������������������һ����췽��*/
	SortById(iCards);    //�Ƚ�iCards�����������    /*59�� ���Ʊ�����������*/
	for (i = 0; iCards[i] > -1; i++)
	{
		if (iCards[i] < 52)
		{
			//������Ϊ iCards[i]/4 �����ж����ţ��� j ����ʾ
			j = 1;
			while (-1 != iTempArr[iCards[i] / 4][j])
				j++;
			//���������iTempArr[iCards[i]/4][j]���棬��Ϊ�ñ���ĵ���ΪiCards[i]/4
			iTempArr[iCards[i] / 4][j] = iCards[i];
			//������Ϊ iCards[i]/4 ���Ƶ����� ��Ϊ j ��
			iTempArr[iCards[i] / 4][0] = j;
		}
	}
	n = 0;   //nΪiCards���±꣬���½�iTempArr�е�������iCards��
	for (i = 4; i > 0; i--) //���ҳ�iTempArrһ���������ƣ�������д��ԭ����iCards�����У�Ȼ����Ѱ�����ŵģ�һ������
	{
		for (j = 0; j < 13; j++)
		{
			if (iTempArr[j][0] == i)    //�жϸõ��������ǲ����� i ��
			{
				for (k = 1; k <= i; k++)    // �еĻ����Ͱ��ƶ��Ž�iCards[ n ]�У�Ȼ�� n++
				{
					iCards[n] = iTempArr[j][k];
					n++;
				}
			}
		}
	}
}
//MO2-END

//M03-START��������Ҫ������������
//����޶���:�ĺ��н�,����޶�ʱ��:15-03-08 18:00
void CardsInfo::SortByMain(int iCards[])
{
	if (IsType0Pass(iCards))
		return;
	if (IsType1Rocket(iCards))
		return;
	if (IsType2Bomb(iCards))
		return;
	if (IsType3Single(iCards))
		return;
	if (IsType4Double(iCards))
		return;
	if (IsType5Three(iCards))
		return;
	if (IsType6ThreeOne(iCards))
		return;
	if (IsType6ThreeDouble(iCards))
		return;
	if (IsType11FourSingle(iCards))
		return;
	if (IsType11FourDouble(iCards))
		return;
	if (IsType7LinkSingle(iCards))
		return;
	if (IsType8LinkDouble(iCards))
		return;
	if (IsType9LinkThree(iCards))
		return;
	if (IsType10LinkThreeSingle(iCards))
		return;
	if (IsType10LinkThreeDouble(iCards))
		return;

	return;
}
//MO3-END



//M05-START����������ת��Ϊ�ַ���׷��(-1��)
//����޶���:÷��,����޶�ʱ��:15-03-01
void CardsInfo::AppendCardsToS(int iCards[], char sMsg[])
{
	int i;
	char sCard[4] = "";
	char sCardString[90] = "";
	if (iCards[0] == -1)	// PASS
		strcat_s(sCardString, "-1");
	else					// ����PASS
	{
		for (i = 0; iCards[i] >= 0; i++)
		{
			if (iCards[i] >= 10)
			{
				sCard[0] = iCards[i] / 10 + '0';
				sCard[1] = iCards[i] % 10 + '0';
				sCard[2] = ',';
				sCard[3] = '\0';
			}
			else
			{
				sCard[0] = iCards[i] % 10 + '0';
				sCard[1] = ',';
				sCard[2] = '\0';
			}
			strcat_s(sCardString, sCard);
		}
		sCardString[strlen(sCardString) - 1] = '\0';	//ȥ�������β������

	}
	strcat(sMsg, sCardString);

}
//MO5-END





//��ǰ������һ�����ʣ����
int CardsInfo::LastPlayerCardsCount(Ddz * pDdz)
{
	int count = 0;
	int m = pDdz->iOTmax - 1;

	//�ж������������ѳ�������
	if (pDdz->iOTmax == 0)
		count = 0;
	else{


		while (m >= 0)
		{
			count += CountCards(pDdz->iOnTable[m]);
			m = m - 3;
		}
	}


	if (pDdz->cLastPlay = pDdz->cLandlord)
		count = 20 - count;
	else
		count = 17 - count;
	return count;
}


int CardsInfo::GetUnReaptedCards(int iCopyCards[])
{
	//ȥ���ظ����Ʋ���С��������
	int i,j,iTemp;
	int iCount = CountCards(iCopyCards);
	for (i = 1; iCopyCards[i] >= 0; i++)
		if (iCopyCards[i] / 4 == iCopyCards[i - 1] / 4)
			iCopyCards[i - 1] = -1;
	for (i = 0; i<iCount; i++)
		for (j = i + 1; j<iCount; j++)
			if (iCopyCards[i]<iCopyCards[j])
			{
				iTemp = iCopyCards[i];
				iCopyCards[i] = iCopyCards[j];
				iCopyCards[j] = iTemp;
			}
	for (i = 0; iCopyCards[i] >= 0; i++)
		for (j = i + 1; iCopyCards[j] >= 0; j++)
			if (iCopyCards[i]>iCopyCards[j])
			{
				iTemp = iCopyCards[i];
				iCopyCards[i] = iCopyCards[j];
				iCopyCards[j] = iTemp;
			}
	iCount = CountCards(iCopyCards);
	return iCount;
}
