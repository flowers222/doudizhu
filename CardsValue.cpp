/// \file CardsValue.cpp
/// 
/// 估值函数模块
///
/// \author 胡颖玢
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
//初始化引擎
void CardsValue::InitCardsValueEngine(int* LastPlayCards, int* ThisPlayerCards, int* NextPlayCards, int *Ontable, char thisPlayer, bool isMaxNode,Ddz *Ddz)
{
	pDdz = Ddz;
	CInfo = CardsInfo::Instance();

	//初始化玩家类型
	InitPlayerType(thisPlayer);
	pLastPlayCards = LastPlayCards;
	pThisPlayerCards = ThisPlayerCards;
	pNextPlayCards = NextPlayCards;
	pOnTable = Ontable;
	//初始化牌力数组
	for (int i = 0; i < 12; i++)
	{
		CardsPower[i] = i;
	}

}
/*dsum函数可能无用*/
double CardsValue::CalSumValue()
{
	double dSum;
	int i;
	double thisSum = 100;			//估值
	double lastSum = 100;			//估值
	double nextSum = 100;			//估值
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
			thisSum = thisSum - 5;			//手牌越少越好每多一张牌优势减5分
			if (i >= 1 && p[i - 1] / 4 == p[i] / 4)
				thisSum = thisSum + 2;	//相邻两牌同点加2分
			if (i >= 4 && p[i - 4] / 4 <= 7
				&& p[i - 4] / 4 + 1 == p[i - 3] / 4
				&& p[i - 3] / 4 + 1 == p[i - 2] / 4
				&& p[i - 2] / 4 + 1 == p[i - 1] / 4
				&& p[i - 1] / 4 + 1 == p [i] / 4)
				s= s+ 6;	//2以下相邻五牌单顺加6分
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

//拆牌函数
void CardsValue::AnalyseCards(int iPlaOnHand[])
{
	//2016430 范晓梦
	//建议算法思路：因为CardsInfo里面的函数只能分析数组从小标0开始的前几张正好是符合牌型本身的（张数类型都符合）的牌，否则判定为非法牌型
	//1.所以建一个大小21的数组，初始化每项值为-1，将传入的iplaOnHand[]复制给它（需调用一个C\C++复制函数）//写到这里已经凌晨了我也超累就不给你们找示例代码了，明天问我吧
	//2.然后将我们自己建的数组传入CInfo->AnalyzeTypeCount,如果返回-1，非法牌型（即目前的牌什么牌型都不是），此时移除0下标的牌（这里的移除包括把后面的牌也往前移动，嫌数组麻烦的
	//可以建Vector代替数组，里面有移除函数很方便）
	//3.然后再传入analyzeTypeCount里面，重复2，直到返回数字，根据CardsInfo里面注释里能根据得到牌型和牌数。
	//再将这一组牌型从vector（假设你用vector）里面移除，再用analyTyCount分析，重复2，如果牌型移除完毕，会得到0。
	//由此应该就能分出全部牌型，最后把牌型信息存储成容易获取的形式，方便其他函数调用，这个可以再讨论。
	//不对的地方请指正


	/*
	int i;
	for (i = 0; iPlaOnHand[i] >= 0; i++)
	{
		dSum = dSum - 5;//每多一张手牌就扣掉5点优势值
		if (i >= 1 && iPlaOnHand[i] / 4 <= 12
			&& iPlaOnHand[i] / 4 == iPlaOnHand[i - 1] / 4)
		{
			double CalCardsValue02(iPlaOnHand[i]);
			break;//判断出为对子并且调用为对子赋值的函数
		}

		if (i >= 5 && iPlaOnHand[i] / 4 <= 11
			&& iPlaOnHand[i] / 4 == iPlaOnHand[i - 1] / 4
			&& iPlaOnHand[i - 2] / 4 == iPlaOnHand[i - 3] / 4 == iPlaOnHand[i] / 4 - 1
			&& iPlaOnHand[i - 4] / 4 == iPlaOnHand[i - 5] / 4 == iPlaOnHand[i - 2] / 4 - 1)
		{
			double CalCardsValue03(iPlaOnHand[i]);//判断出为连对并且调用为连对赋值的函数
			break;
		}

		if (i >= 4 && iPlaOnHand[i] / 4 <= 11
			&& iPlaOnHand[i] / 4 == iPlaOnHand[i - 1] / 4 + 1
			&& iPlaOnHand[i - 1] / 4 == iPlaOnHand[i - 2] / 4 + 1
			&& iPlaOnHand[i - 2] / 4 == iPlaOnHand[i - 3] / 4 + 1
			&& iPlaOnHand[i - 3] / 4 == iPlaOnHand[i - 4] / 4 + 1)
		{
			double CalCardsValue04(iPlaOnHand[i]);//判断出为连子并且调用为连子赋值的函数
			break;
		}

		if (i >= 2 && iPlaOnHand[i] / 4 == iPlaOnHand[i - 1] / 4 == iPlaOnHand[i - 2] / 4)
		{
			double CaLCardsValue05(iPlaOnHand[i]);//判断出为三带一并且调用为三带一赋值的函数
			break;
		}

		if (i >= 3 && iPlaOnHand[i] / 4 == iPlaOnHand[i - 1] / 4 == iPlaOnHand[i - 2] / 4 == iPlaOnHand[i - 3])
		{
			double CalCardsValue06(iPlaOnHand[i]);//判断出为炸弹
			break;
		}

		if (i >= 1 && iPlaOnHand[i] / 4 == 13
			&& iPlaOnHand[i - 1] / 4 == 13)
		{
			double CalCardsValue07(iPlaOnHand[i]);//判断出为对王
			break;
		}

		double CalCardsValue01(iPlaOnHand[i]);//以上都不是 那就是单子
	}
	*/
}

//double CardsValue::CalCardsValue01(int iPlaOnHand)
//{
//	int i = iPlaOnHand / 4;
//	dSum = dSum + i;
//
//}//计算单牌
//
//double CardsValue::CalCardsValue02(int iPlaOnHand)
//{
//	int i = iPlaOnHand / 4;
//	dSum = dSum + (i + 3);
//}//计算对子
//
//double CardsValue::CalCardsValue03(int iPlaOnHand)
//{
//	int i = iPlaOnHand / 4;
//	dSum = dSum + (i + 7);
//}//计算连对
//
//double CardsValue::CalCardsValue04(int iPlaOnHand)
//{
//	int i = iPlaOnHand / 4;
//	dSum = dSum + (i + 8);
//}//计算顺子
//
//double CardsValue::CalCardsValue05(int iPlaOnHand)
//{
//	int i = iPlaOnHand / 4;
//	dSum = dSum + (i + 5);
//}//计算三代一
//
//double CardsValue::CalCardsValue06(int iPlaOnHand)
//{
//	int i = iPlaOnHand / 4;
//	dSum = dSum + (i + 20);
//}//计算炸弹
//
//double CardsValue::CalCardsValue07(int iPlaOnHand)
//{
//	int i = iPlaOnHand / 4;
//	dSum = dSum + 40;
//}//计算对王

void CardsValue::InitPlayerType(char thisPlayer)
{
	 if(thisPlayer == pDdz->cLandlord)
	{
		thisPlayerType = lord;
		lastPlayerType = farmer;
		nextPlayerType = farmer;
		return;
	}
	 //上家是地主
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
	 //本家是地主
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
	 //下家是地主
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