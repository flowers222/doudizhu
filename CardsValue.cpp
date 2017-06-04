/// \file CardsValue.cpp
/// 
/// ��ֵ����ģ��
///
/// \author ��ӱ��
/// \version 1.0
/// \date 2016-08
#include "CardsValue.h"

CardsValue::CardsValue()
{
}


CardsValue::~CardsValue()
{
}


CardsValue* CardsValue::Instance()

{
	static CardsValue instance;

	return &instance;
}
//��ʼ������
void CardsValue::InitCardsValueEngine(int* LastPlayCards, int* ThisPlayerCards, int* NextPlayCards, int *Ontable, char thisPlayer, bool isMaxNode,Ddz *Ddz)
{
	pDdz = Ddz;
	CInfo = CardsInfo::Instance();

	//��ʼ���������
	InitPlayerType(thisPlayer);
	pLastPlayCards = LastPlayCards;
	pThisPlayerCards = ThisPlayerCards;
	pNextPlayCards = NextPlayCards;
	pOnTable = Ontable;
	//��ʼ����������
	for (int i = 0; i < 12; i++)
	{
		CardsPower[i] = i;
	}

}
/*dsum������������*/
double CardsValue::CalSumValue()
{
	double dSum;
	int i;
	double thisSum = 100;			//��ֵ
	double lastSum = 100;			//��ֵ
	double nextSum = 100;			//��ֵ
	int * p=NULL;
	int t = 0;
	while(t<3)
	{
		switch (t)
		{
		case 0: p = pThisPlayerCards;
		case 1: p = pLastPlayCards;
		case 2: p = pNextPlayCards;
		}
		double  s;
		for (i = 0; p[i] >= 0; i++)
		{
			s = 100;
			thisSum = thisSum - 5;			//����Խ��Խ��ÿ��һ�������Ƽ�5��
			if (i >= 1 && p[i - 1] / 4 == p[i] / 4)
				thisSum = thisSum + 2;	//��������ͬ���2��
			if (i >= 4 && p[i - 4] / 4 <= 7
				&& p[i - 4] / 4 + 1 == p[i - 3] / 4
				&& p[i - 3] / 4 + 1 == p[i - 2] / 4
				&& p[i - 2] / 4 + 1 == p[i - 1] / 4
				&& p[i - 1] / 4 + 1 == p [i] / 4)
				s= s+ 6;	//2�����������Ƶ�˳��6��
		}
		switch (t)
		{
		case 0: thisSum = s;
		case 1: lastSum = s;
		case 2: nextSum=s;
		}
		t++;
	}
	if (thisPlayerType == lord)
		dSum = lastSum + nextSum - 2 * thisSum;
	else
		if (nextPlayerType == lord)
			dSum = thisSum + lastSum - 2 * nextSum;
		else
			dSum = thisSum + lastSum - 2 * lastSum;

	return dSum;
}

//���ƺ���
void CardsValue::AnalyseCards(int iPlaOnHand[])
{
	//2016430 ������
	//�����㷨˼·����ΪCardsInfo����ĺ���ֻ�ܷ��������С��0��ʼ��ǰ���������Ƿ������ͱ���ģ��������Ͷ����ϣ����ƣ������ж�Ϊ�Ƿ�����
	//1.���Խ�һ����С21�����飬��ʼ��ÿ��ֵΪ-1���������iplaOnHand[]���Ƹ����������һ��C\C++���ƺ�����//д�������Ѿ��賿����Ҳ���۾Ͳ���������ʾ�������ˣ��������Ұ�
	//2.Ȼ�������Լ��������鴫��CInfo->AnalyzeTypeCount,�������-1���Ƿ����ͣ���Ŀǰ����ʲô���Ͷ����ǣ�����ʱ�Ƴ�0�±���ƣ�������Ƴ������Ѻ������Ҳ��ǰ�ƶ����������鷳��
	//���Խ�Vector�������飬�������Ƴ������ܷ��㣩
	//3.Ȼ���ٴ���analyzeTypeCount���棬�ظ�2��ֱ���������֣�����CardsInfo����ע�����ܸ��ݵõ����ͺ�������
	//�ٽ���һ�����ʹ�vector����������vector�������Ƴ�������analyTyCount�������ظ�2����������Ƴ���ϣ���õ�0��
	//�ɴ�Ӧ�þ��ֳܷ�ȫ�����ͣ�����������Ϣ�洢�����׻�ȡ����ʽ�����������������ã�������������ۡ�
	//���Եĵط���ָ��


	/*
	int i;
	for (i = 0; iPlaOnHand[i] >= 0; i++)
	{
		dSum = dSum - 5;//ÿ��һ�����ƾͿ۵�5������ֵ
		if (i >= 1 && iPlaOnHand[i] / 4 <= 12
			&& iPlaOnHand[i] / 4 == iPlaOnHand[i - 1] / 4)
		{
			double CalCardsValue02(iPlaOnHand[i]);
			break;//�жϳ�Ϊ���Ӳ��ҵ���Ϊ���Ӹ�ֵ�ĺ���
		}

		if (i >= 5 && iPlaOnHand[i] / 4 <= 11
			&& iPlaOnHand[i] / 4 == iPlaOnHand[i - 1] / 4
			&& iPlaOnHand[i - 2] / 4 == iPlaOnHand[i - 3] / 4 == iPlaOnHand[i] / 4 - 1
			&& iPlaOnHand[i - 4] / 4 == iPlaOnHand[i - 5] / 4 == iPlaOnHand[i - 2] / 4 - 1)
		{
			double CalCardsValue03(iPlaOnHand[i]);//�жϳ�Ϊ���Բ��ҵ���Ϊ���Ը�ֵ�ĺ���
			break;
		}

		if (i >= 4 && iPlaOnHand[i] / 4 <= 11
			&& iPlaOnHand[i] / 4 == iPlaOnHand[i - 1] / 4 + 1
			&& iPlaOnHand[i - 1] / 4 == iPlaOnHand[i - 2] / 4 + 1
			&& iPlaOnHand[i - 2] / 4 == iPlaOnHand[i - 3] / 4 + 1
			&& iPlaOnHand[i - 3] / 4 == iPlaOnHand[i - 4] / 4 + 1)
		{
			double CalCardsValue04(iPlaOnHand[i]);//�жϳ�Ϊ���Ӳ��ҵ���Ϊ���Ӹ�ֵ�ĺ���
			break;
		}

		if (i >= 2 && iPlaOnHand[i] / 4 == iPlaOnHand[i - 1] / 4 == iPlaOnHand[i - 2] / 4)
		{
			double CaLCardsValue05(iPlaOnHand[i]);//�жϳ�Ϊ����һ���ҵ���Ϊ����һ��ֵ�ĺ���
			break;
		}

		if (i >= 3 && iPlaOnHand[i] / 4 == iPlaOnHand[i - 1] / 4 == iPlaOnHand[i - 2] / 4 == iPlaOnHand[i - 3])
		{
			double CalCardsValue06(iPlaOnHand[i]);//�жϳ�Ϊը��
			break;
		}

		if (i >= 1 && iPlaOnHand[i] / 4 == 13
			&& iPlaOnHand[i - 1] / 4 == 13)
		{
			double CalCardsValue07(iPlaOnHand[i]);//�жϳ�Ϊ����
			break;
		}

		double CalCardsValue01(iPlaOnHand[i]);//���϶����� �Ǿ��ǵ���
	}
	*/
}

//double CardsValue::CalCardsValue01(int iPlaOnHand)
//{
//	int i = iPlaOnHand / 4;
//	dSum = dSum + i;
//
//}//���㵥��
//
//double CardsValue::CalCardsValue02(int iPlaOnHand)
//{
//	int i = iPlaOnHand / 4;
//	dSum = dSum + (i + 3);
//}//�������
//
//double CardsValue::CalCardsValue03(int iPlaOnHand)
//{
//	int i = iPlaOnHand / 4;
//	dSum = dSum + (i + 7);
//}//��������
//
//double CardsValue::CalCardsValue04(int iPlaOnHand)
//{
//	int i = iPlaOnHand / 4;
//	dSum = dSum + (i + 8);
//}//����˳��
//
//double CardsValue::CalCardsValue05(int iPlaOnHand)
//{
//	int i = iPlaOnHand / 4;
//	dSum = dSum + (i + 5);
//}//��������һ
//
//double CardsValue::CalCardsValue06(int iPlaOnHand)
//{
//	int i = iPlaOnHand / 4;
//	dSum = dSum + (i + 20);
//}//����ը��
//
//double CardsValue::CalCardsValue07(int iPlaOnHand)
//{
//	int i = iPlaOnHand / 4;
//	dSum = dSum + 40;
//}//�������

void CardsValue::InitPlayerType(char thisPlayer)
{
	 if(thisPlayer == pDdz->cLandlord)
	{
		thisPlayerType = lord;
		lastPlayerType = farmer;
		nextPlayerType = farmer;
		return;
	}
	 //�ϼ��ǵ���
	 if (pDdz->cLastPlay == pDdz->cLandlord)
	{
		if (thisPlayer == pDdz->cDir)
		{
			lastPlayerType = lord;
			thisPlayerType = farmer;
			nextPlayerType = farmer;
		}
		else
		if (thisPlayer == 'n')
		{
			lastPlayerType = farmer;
			thisPlayerType = farmer;
			nextPlayerType = lord;
		}
		else
		{
			lastPlayerType = farmer;
			thisPlayerType = lord;
			nextPlayerType = farmer;
		}
	}
	 //�����ǵ���
	 else if (pDdz->cDir == pDdz->cLandlord)
	 {
		 if (thisPlayer == pDdz->cDir)
		 {
			 lastPlayerType = farmer;
			 thisPlayerType = lord;
			 nextPlayerType = farmer;
		 }
		 if (thisPlayer == 'n')
		 {
			 lastPlayerType = lord;
			 thisPlayerType = farmer;
			 nextPlayerType = farmer;
		 }
		 else
		 {
			 lastPlayerType = farmer;
			 thisPlayerType = farmer;
			 nextPlayerType = lord;
		 }

	 }
	 //�¼��ǵ���
	else
	{
		if (thisPlayer == pDdz->cDir)
		{
			lastPlayerType = farmer;
			thisPlayerType = farmer;
			nextPlayerType = lord;
		}
		if (thisPlayer == 'n')
		{
			lastPlayerType = farmer;
			thisPlayerType = lord;;
			nextPlayerType = farmer;
		}
		else
		{
			lastPlayerType = lord;
			thisPlayerType = farmer;
			nextPlayerType = farmer;
		}
	}

}



int CardsValue::CallSumCardsPower(int iPlaOnHand)
{
	return 0;
}
int CardsValue::CallMinHandCount(int iPlaOnHand[])
{
	return 0;
}