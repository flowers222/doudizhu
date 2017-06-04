#ifndef _SplitCard
#define _SplitCard
#include <stdio.h>
#include <stdlib.h>

/*
* Decription:
* Copyright (C), 2016, Devil.Liu
* This program is protected by copyright laws.
* Program Name: SplitCard Class in DDZ Program
* Date: 2016.7.21.13:44
* author 刘云志
* version 1.0 实现对牌型的分析和整理
*	第一次修改：2016.7.21.16:31 修改BUG
*  第二次修改：2016.7.24.23:18 修改BUG，调整方法名
*  第三次修改：2016.7.26.11:07 增加初始化数据方法
*  第四次修改：2016.7.29.11:21 火箭数组大小调整（统一格式）
*/
class SplitCard
{
public:
	SplitCard();//（与数组大小耦合）
	~SplitCard();
	static SplitCard* Instance();

	int** startSplitCard(int iCards[]);//对外接口
	int* startSplitCard(int iCards[], int type);//对外接口

private:
	void initData();//初始化数据
	void transcode();//整理牌型（与数组大小耦合）
	void judgeRepeat();//判断重牌
	void judgeOrder();//判断连牌
	void judgeMinLink(int iMinLink);//选取最小连数的连牌
	void judgeLink(int iMinLink);//判断顺子

	void judgeRocket();//判断火箭
	void judgeBomb();//判断炸弹
	void judgeSingle();//判断单牌
	void judgeDouble();//判断对牌
	void judgeThree();//判断三条
	void judgeLinkSingle();//判断单顺（与数组大小耦合）
	void judgeLinkDouble();//判断双顺
	void judgeLinkThree();//判断三顺

private:
	int* iCards;//标准编码
	int iMyCards[38];//内部编码

	int iRepeat[26];//重牌及张数
	int iSingleInRepeatIndex[38];//在重牌筛选后的单牌的索引

	int iWaitOrder[38];//等待搜索连牌的牌，最后结果为组合顺子后的单牌
	int iOrder[38];//连牌
	int iMinLinkIndex[38];//最小连数的连牌的索引
	int iSingleInLinkIndex[38];//在顺子筛选后的单牌的索引
	int iLinkIndex[38];//顺子索引

	int iRocket[4];//火箭
	int iBomb[38];//炸弹
	int iSingle[38];//单牌
	int iDouble[38];//对牌
	int iThree[38];//三条
	int iLinkSingle[38];//单顺
	int iLinkDouble[38];//双顺
	int iLinkThree[38];//三顺

	int* iResult[8];//返回结果
};

#endif