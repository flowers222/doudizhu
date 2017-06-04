#include "SplitCard.h"

/*
* Decription:
* Copyright (C), 2016, Devil.Liu
* This program is protected by copyright laws.
* Program Name: SplitCard Class in DDZ Program
* Date: 2016.7.21.13:44
* author 刘云志
* version 1.0 实现对牌型的分析和整理
*	第一次修改：2016.7.21.16:30 修改BUG
*  第二次修改：2016.7.24.23:16 修改拆牌逻辑，调试代码通过
*  第三次修改：2016.7.26.10:21 返回数组结束符的修改
*  第四次修改：2016.7.26.11:13 增加初始化数据方法，调整数据初始化位置
*  第五次修改：2016.7.26.23:47 修改BUG
*  第六次修改：2016.7.29.11:20 返回数组结束符的修改还原
*  第七次修改：2016.8.3.00:11  修改BUG，防止无单顺时出现乱码
*  第八次修改：2016.8.6.13:10  修改BUG，防止一张王时出现火箭；防止判断顺子时数组越界，出现乱码，甚至崩溃
*  第九次修改：2016.8.7.19:10  添加判断，防止传入数组无值得情况
*  第十次修改：2016.8.10.00:48 修改BUG，单牌中排除“王”的逻辑问题
*  第十一次修改： 2016.8.11.00:49  修改BUG
*  第十二次修改： 2016.8.13.13:20  增加最后的顺子再合并功能
*/
SplitCard::SplitCard()
{
	iCards = NULL;//外部编码

	//返回的结果
	iResult[0] = iRocket;
	iResult[1] = iBomb;
	iResult[2] = iThree;
	iResult[3] = iLinkThree;
	iResult[4] = iLinkSingle;
	iResult[5] = iLinkDouble;
	iResult[6] = iDouble;
	iResult[7] = iSingle;
}

SplitCard::~SplitCard()
{
}

SplitCard* SplitCard::Instance()
{
	static SplitCard instance;

	return &instance;
}

//初始化数据
void SplitCard::initData()
{
	for (int k = 0; k < 4; k++)
	{
		iRocket[k] = 0;//火箭
	}
	for (int j = 0; j < 38; j++)
	{
		iMyCards[j] = 0;//内部编码
		iWaitOrder[j] = 0;//待连牌的牌
		iOrder[j] = 0;//已连牌的牌
		iMinLinkIndex[j] = 0;//最小连数的连牌

		iSingleInRepeatIndex[j] = -1;//重牌里的单牌的索引
		iSingleInLinkIndex[j] = -1;//顺子里的单牌的索引
		iLinkIndex[j] = -1;//顺子的索引

		iBomb[j] = 0;//炸弹
		iSingle[j] = 0;//单牌
		iDouble[j] = 0;//对牌
		iThree[j] = 0;//三条
		iLinkSingle[j] = 0;//单顺
		iLinkDouble[j] = 0;//双顺
		iLinkThree[j] = 0;//三顺
	}
	for (int i = 0; i < 26; i++)
	{
		iRepeat[i] = i % 2;//重牌
	}
}

//整理牌型，标准编码与内部编码的索引相同（合格）
void SplitCard::transcode()
{
	for (int i = 0; i < 38; i++)
	{
		if (iCards[i] == -1)
		{
			iMyCards[i] = -1;
			break;
		}
		if (iCards[i] / 4 == 0)
			iMyCards[i] = 3;    //3
		if (iCards[i] / 4 == 1)
			iMyCards[i] = 4;    //4
		if (iCards[i] / 4 == 2)
			iMyCards[i] = 5;    //5
		if (iCards[i] / 4 == 3)
			iMyCards[i] = 6;    //6
		if (iCards[i] / 4 == 4)
			iMyCards[i] = 7;    //7
		if (iCards[i] / 4 == 5)
			iMyCards[i] = 8;    //8
		if (iCards[i] / 4 == 6)
			iMyCards[i] = 9;    //9
		if (iCards[i] / 4 == 7)
			iMyCards[i] = 10;    //10
		if (iCards[i] / 4 == 8)
			iMyCards[i] = 11;    //J
		if (iCards[i] / 4 == 9)
			iMyCards[i] = 12;    //Q
		if (iCards[i] / 4 == 10)
			iMyCards[i] = 13;    //K
		if (iCards[i] / 4 == 11)
			iMyCards[i] = 14;    //A
		if (iCards[i] / 4 == 12)
			iMyCards[i] = 22;    //2
		if (iCards[i] == 52)
			iMyCards[i] = 66;    //小王
		if (iCards[i] == 53)
			iMyCards[i] = 99;    //大王
	}
}

//判断重牌（合格）
void SplitCard::judgeRepeat()
{
	for (int i = 0, j = 0, k = 0; iMyCards[i] != -1; i++)
	{
		if (iMyCards[i] == iMyCards[i + 1])
		{
			iRepeat[j] = iMyCards[i + 1];
			iRepeat[j + 1]++;
		}
		else
		{
			j += 2;
			//搜索无重复的单牌
			if (iMyCards[i] != iMyCards[i - 1] && iMyCards[i] != iMyCards[i + 1])
			{
				iSingleInRepeatIndex[k] = i;
				k++;
			}
		}
	}
}

//判断连牌（合格）
void SplitCard::judgeOrder()
{
	int iLinkEnd = iWaitOrder[0];//连牌中的最后一个牌值
	int iLinkEndIndex = 0;//一组连牌的开始索引
	int iEndIndex = -1;//结束符“-1”的索引
	bool bStartLock = true;//是否是一组连牌的标志

	//开始搜索连牌
	for (int i = 1, j = 1; iWaitOrder[i] != -1; i++, j++)
	{
		//一组连牌的开始
		if (bStartLock)
		{
			//开始的两位是否相连
			if (iWaitOrder[i] == iLinkEnd + 1)
			{
				iOrder[iLinkEndIndex] = iLinkEnd;
				iOrder[j] = iWaitOrder[i];
				iLinkEnd = iOrder[j];
			}
			//开始的两位是否相等
			else if (iLinkEnd == iWaitOrder[i])
			{
				iOrder[iLinkEndIndex] = 0;
				iOrder[j] = iWaitOrder[i];
			}
			//开始的两位是否相隔
			else if (iLinkEnd + 1 < iWaitOrder[i])
			{
				iOrder[iLinkEndIndex] = 0;
				iOrder[j] = iWaitOrder[i];
				iLinkEnd = iOrder[j];
			}
			//开始的两位的第二位为0
			else
			{
				iOrder[j] = 0;
			}
			//开锁
			if (iWaitOrder[i] != 0)
			{
				bStartLock = false;
			}
		}
		else
		{
			//与后一位相连
			if (iLinkEnd + 1 == iWaitOrder[i])
			{
				iOrder[j] = iWaitOrder[i];
				iLinkEnd = iOrder[j];
			}
			//与后一位相隔，一组连牌结束
			else if (iLinkEnd + 1 < iWaitOrder[i])
			{
				iLinkEndIndex = i;
				iLinkEnd = iWaitOrder[i];
				bStartLock = true;
			}
			//与后一位相等，或后一位为0
			else
			{
				iOrder[j] = 0;
			}
		}
		iEndIndex = j + 1;
	}
	if (iEndIndex == -1)
	{
		iEndIndex = 0;
	}
	iOrder[iEndIndex] = -2;
	iEndIndex++;
	iOrder[iEndIndex] = -1;
}

//选取最小连数的牌（合格）
void SplitCard::judgeMinLink(int iMinLink)
{
	int iLinkEnd = 0;//连牌中的最后一个牌值
	int iFiveIndex[] = { -1, -1, -1, -1, -1 };//五连牌的索引
	int iIndex = 0;//最小五连牌的保存索引
	int iLinkNum = 0;//连牌的数量
	int iIsCircle = -1;//是否结束最小五连牌搜索的标志

	//循环搜索最小五连牌
	while (iIsCircle != iIndex)
	{
		//首先在WaitOrder中搜索连牌
		judgeOrder();
		//将上次最小五连的结束索引保留
		iIsCircle = iIndex;
		//Order起始位置特殊处理
		if (iOrder[0] != 0)
		{
			iLinkEnd = iOrder[0] - 1;
		}
		//其次在Order中搜索最小五连牌
		for (int i = 0; iOrder[i] != -1; i++)
		{
			//找到满足条件的五连牌
			if (iLinkNum == iMinLink)
			{
				for (int k = 0; k < iMinLink; k++)
				{
					iMinLinkIndex[iIndex] = iFiveIndex[k];
					iWaitOrder[iFiveIndex[k]] = 0;
					iFiveIndex[k] = -1;
					iIndex++;
				}
				iMinLinkIndex[iIndex] = -2;
				iIndex++;
				iLinkNum = 0;
				iLinkEnd = 0;
				break;
			}
			//正在搜索满足条件的五连牌
			else
			{
				//相连
				if (iOrder[i] != 0 && iLinkEnd + 1 == iOrder[i])
				{
					iFiveIndex[iLinkNum] = i;
					iLinkEnd = iOrder[i];
					iLinkNum++;
				}

				//一组连牌结束，但小于五连
				else if (iLinkEnd + 1 < iOrder[i])
				{
					for (int k = iLinkNum; k >= 0; k--)
					{
						iFiveIndex[k] = -1;
					}
					iLinkNum = 0;
					iLinkEnd = iOrder[i];
					iFiveIndex[iLinkNum] = i;
					iLinkNum++;
				}
				//第一位为0，或第一二位相等
				else
				{
					//第一位为0，但第二位不为0
					if (iLinkEnd == 0)
					{
						iLinkEnd = iOrder[i + 1] - 1;
					}
				}
			}
		}
	}
	//结束符
	iMinLinkIndex[iIndex] = -1;
}

//判断顺子（合格）
void SplitCard::judgeLink(int iMinLink)
{
	int iLinkExtendIndex[12] = { 0 };//扩展的连牌，以每组五连的结束索引为起始符，-2为结束符
	int iWriteIndex = -1;
	int iReadIndex = 0;
	bool bIsExtendLink = false;
	int iLinkSingleIndex = 0;

	judgeMinLink(iMinLink);

	//搜索剩余牌值
	for (int i = 0; iWaitOrder[i] != -1; i++)
	{
		if (iWaitOrder[i] != 0)
		{
			//搜索是否可以扩展
			//扩展是否结束，如果结束就在MinLinkIndex中搜索，否则在LinkExtendIndex中继续扩展
			if (!bIsExtendLink)
			{
				for (int j = iMinLink - 1; iMinLinkIndex[j] != -1; j = j + iMinLink + 1)
				{
					if (iMinLinkIndex[j] != 0 && iMyCards[iMinLinkIndex[j]] + 1 == iWaitOrder[i])
					{
						iWriteIndex++;
						iLinkExtendIndex[iWriteIndex] = iMinLinkIndex[j];
						iWriteIndex++;
						iLinkExtendIndex[iWriteIndex] = i;
						iWaitOrder[i] = 0;
						bIsExtendLink = true;
					}
				}
			}
			else
			{
				for (int j = i; iWaitOrder[j] != -1; j++)
				{
					if (iMyCards[iLinkExtendIndex[iWriteIndex]] + 1 == iWaitOrder[j])
					{
						iWriteIndex++;
						iLinkExtendIndex[iWriteIndex] = j;
						iWaitOrder[j] = 0;
						i = j;
					}
					else if (iMyCards[iLinkExtendIndex[iWriteIndex]] + 1 < iWaitOrder[j] || iWaitOrder[j + 1] == -1)
					{
						iWriteIndex++;
						iLinkExtendIndex[iWriteIndex] = -2;
						i = j - 1;
						bIsExtendLink = false;
						break;
					}
					else
					{
						continue;
					}
				}
			}
		}
		if (iWaitOrder[i + 1] == -1)
		{
			iWriteIndex++;
			iLinkExtendIndex[iWriteIndex] = -2;
		}
	}

	//合并连牌
	for (int j = 0; iMinLinkIndex[j] != -1; iLinkSingleIndex++, j++)
	{
		//读取五连牌
		if (iMinLinkIndex[j] != -2)
		{
			iLinkIndex[iLinkSingleIndex] = iMinLinkIndex[j];
		}
		//读取扩展牌
		else
		{
			//可扩展
			if (iLinkExtendIndex[iReadIndex] != 0 && iLinkExtendIndex[iReadIndex] == iMinLinkIndex[j - 1])
			{
				iReadIndex++;
				iLinkIndex[iLinkSingleIndex] = iLinkExtendIndex[iReadIndex];
				iReadIndex++;
				iLinkSingleIndex++;
				while (iLinkExtendIndex[iReadIndex] != -2)
				{
					iLinkIndex[iLinkSingleIndex] = iLinkExtendIndex[iReadIndex];
					iReadIndex++;
					iLinkSingleIndex++;
				}
				iLinkIndex[iLinkSingleIndex] = -2;
				iReadIndex++;
			}
			//不可扩展
			else
			{
				iLinkIndex[iLinkSingleIndex] = -2;
			}
		}
	}
	//结束符
	iLinkIndex[iLinkSingleIndex] = -1;
	//判断最后的顺子是否可以再合并
	for (int k = 0; iLinkIndex[k] != -1; k++)
	{
		if (iLinkIndex[k] == -2)
		{
			if (iMyCards[iLinkIndex[k - 1]] + 1 == iMyCards[iLinkIndex[k + 1]])
			{
				for (int l = k; iLinkIndex[l] != -1; l++)
				{
					iLinkIndex[l] = iLinkIndex[l + 1];
				}
			}
		}
	}
}

//判断单顺（合格）
void SplitCard::judgeLinkSingle()
{
	int iEndIndex = -1;//结束符“-1”的索引

	//初始化WaitOrder
	for (int l = 0; l < 38; l++)
	{
		iWaitOrder[l] = iMyCards[l];
	}

	judgeLink(5);

	//在顺子筛选后的单牌
	for (int i = 0, j = 0; iWaitOrder[i] != -1; i++)
	{
		if (iWaitOrder[i] > 0)
		{
			iSingleInLinkIndex[j] = i;
			j++;
			iWaitOrder[i] = 0;
		}
	}

	//存储单顺外部编码
	for (int k = 0; iLinkIndex[k] != -1; k++)
	{
		if (iLinkIndex[k] != -2)
		{
			iLinkSingle[k] = iCards[iLinkIndex[k]];
		}
		else
		{
			iLinkSingle[k] = iLinkIndex[k];
		}
		iEndIndex = k + 1;
	}
	//防止无单顺时数组越界
	if (iEndIndex == -1)
	{
		iEndIndex++;
	}
	iLinkSingle[iEndIndex] = -1;
}

//判断火箭(合格)
void SplitCard::judgeRocket()
{
	int j = 0;
	for (int i = 0; iMyCards[i] != -1; i++)
	{
		if (iMyCards[i] == 66 || iMyCards[i] == 99)
		{
			iRocket[j] = iCards[i];
			j++;
		}
	}
	iRocket[j] = -2;
	j++;
	iRocket[j] = -1;
	//只有一张王
	if (iRocket[1] == -2)
	{
		iRocket[0] = -1;
		iRocket[1] = 0;
	}
}

//判断炸弹（合格）
void SplitCard::judgeBomb()
{
	int iBombValue = 0;
	int iCount = 0;
	int iIndex = 0;

	for (int i = 1; i < 26; i += 2)
	{
		if (iRepeat[i] == 4)
		{
			iBombValue = iRepeat[i - 1];
			for (int j = 0; iMyCards[j] != -1; j++)
			{
				if (iBombValue == iMyCards[j])
				{
					iBomb[iIndex] = iCards[j];
					iCount++;
					iIndex++;
				}
				if (iCount == 4)
				{
					iBomb[iIndex] = -2;
					iIndex++;
					iCount = 0;
					break;
				}
			}
		}
	}
	iBomb[iIndex] = -1;
}

//判断对牌（合格）
void SplitCard::judgeDouble()
{
	int iDoubleValue = 0;
	int iCount = 0;
	int iIndex = 0;

	for (int i = 1; i < 26; i += 2)
	{
		if (iRepeat[i] == 2)
		{
			iDoubleValue = iRepeat[i - 1];
			for (int j = 0; iMyCards[j] != -1; j++)
			{
				if (iDoubleValue == iMyCards[j])
				{
					iDouble[iIndex] = iCards[j];
					iCount++;
					iIndex++;
				}
				if (iCount == 2)
				{
					iDouble[iIndex] = -2;
					iIndex++;
					iCount = 0;
					break;
				}
			}
		}
	}
	iDouble[iIndex] = -1;
}

//判断双顺（合格）
void SplitCard::judgeLinkDouble()
{
	int iIndex = 0;
	int iDoubleValue = 0;
	int iLastWaitOrderIndex = -1;

	//初始化WaitOrder
	for (int k = 1; k < 26; k += 2)
	{
		if (iRepeat[k] >= 2)
		{
			iDoubleValue = iRepeat[k - 1];
			for (int l = 0; iMyCards[l] != -1; l++)
			{
				if (iDoubleValue == iMyCards[l])
				{
					iWaitOrder[l] = iMyCards[l];
					iLastWaitOrderIndex = l + 1;
					break;
				}
			}
		}
	}
	if (iLastWaitOrderIndex == -1)
	{
		iLastWaitOrderIndex = 1;
	}
	iWaitOrder[iLastWaitOrderIndex] = -1;

	judgeLink(3);

	for (int i = 0; iLinkIndex[i] != -1; i++)
	{
		if (iLinkIndex[i] != -2)
		{
			for (int j = 0; iMyCards[j] != -1; j++)
			{
				if (iMyCards[iLinkIndex[i]] == iMyCards[j])
				{
					iLinkDouble[iIndex] = iCards[j];
					iIndex++;
					iLinkDouble[iIndex] = iCards[j + 1];
					iIndex++;
					break;
				}
			}
		}
		else
		{
			iLinkDouble[iIndex] = -2;
			iIndex++;
		}
	}
	iLinkDouble[iIndex] = -1;
}

//判断三条（合格）
void SplitCard::judgeThree()
{
	int iThreeValue = 0;
	int iCount = 0;
	int iIndex = 0;

	for (int i = 1; i < 26; i += 2)
	{
		if (iRepeat[i] == 3)
		{
			iThreeValue = iRepeat[i - 1];
			for (int j = 0; iMyCards[j] != -1; j++)
			{
				if (iThreeValue == iMyCards[j])
				{
					iThree[iIndex] = iCards[j];
					iCount++;
					iIndex++;
					if (iCount == 1)
					{
						iWaitOrder[j] = iMyCards[j];
					}
				}
				if (iCount == 3)
				{
					iThree[iIndex] = -2;
					iIndex++;
					iCount = 0;
					break;
				}
			}
		}
	}
	iThree[iIndex] = -1;
}

//判断三顺
void SplitCard::judgeLinkThree()
{
	int iIndex = 0;
	int iThreeValue = 0;
	int iLastWaitOrderIndex = -1;

	//初始化WaitOrder
	for (int k = 1; k < 26; k += 2)
	{
		if (iRepeat[k] >= 3)
		{
			iThreeValue = iRepeat[k - 1];
			for (int l = 0; iMyCards[l] != -1; l++)
			{
				if (iThreeValue == iMyCards[l])
				{
					iWaitOrder[l] = iMyCards[l];
					iLastWaitOrderIndex = l + 1;
					break;
				}
			}
		}
	}
	if (iLastWaitOrderIndex == -1)
	{
		iLastWaitOrderIndex = 1;
	}
	iWaitOrder[iLastWaitOrderIndex] = -1;

	judgeLink(2);

	for (int i = 0; iLinkIndex[i] != -1; i++)
	{
		if (iLinkIndex[i] != -2)
		{
			for (int j = 0; iMyCards[j] != -1; j++)
			{
				if (iMyCards[iLinkIndex[i]] == iMyCards[j])
				{
					iLinkThree[iIndex] = iCards[j];
					iIndex++;
					iLinkThree[iIndex] = iCards[j + 1];
					iIndex++;
					iLinkThree[iIndex] = iCards[j + 2];
					iIndex++;
					break;
				}
			}
		}
		else
		{
			iLinkThree[iIndex] = -2;
			iIndex++;
		}
	}
	iLinkThree[iIndex] = -1;
}

//判断单牌（合格）
void SplitCard::judgeSingle()
{
	int iIndex = 0;

	judgeLinkSingle();

	for (int i = 0; iSingleInRepeatIndex[i] != -1; i++)
	{
		for (int j = 0; iSingleInLinkIndex[j] != -1; j++)
		{
			if (iSingleInRepeatIndex[i] == iSingleInLinkIndex[j])
			{
				//排除火箭
				if (iCards[iSingleInRepeatIndex[i]] != 52 && iCards[iSingleInRepeatIndex[i]] != 53)
				{
					iSingle[iIndex] = iCards[iSingleInLinkIndex[j]];
					iIndex++;
					iSingle[iIndex] = -2;
					iIndex++;
				}
				break;
			}
		}
	}
	iSingle[iIndex] = -1;
}

//对外接口（合格）
int** SplitCard::startSplitCard(int iInitCards[])
{
	iCards = iInitCards;
	//初始化数据
	initData();
	//防止iInitCards中无值
	if (iCards[0] == -1)
	{
		iBomb[0] = -1;
		iSingle[0] = -1;
		iDouble[0] = -1;
		iThree[0] = -1;
		iLinkSingle[0] = -1;
		iLinkDouble[0] = -1;
		iLinkThree[0] = -1;

		return iResult;
	}

	transcode();//转换编码
	judgeRepeat();//搜索重牌

	judgeRocket();//判断火箭
	judgeBomb();//判断炸弹
	judgeLinkSingle();//判断单顺
	judgeDouble();//判断对牌
	judgeLinkDouble();//判断双顺
	judgeThree();//判断三条
	judgeLinkThree();//判断三顺
	judgeSingle();//判断单牌

	return iResult;
}

//对外接口（合格）
int* SplitCard::startSplitCard(int iInitCards[], int type)
{
	iCards = iInitCards;
	//初始化数据
	initData();
	//防止iInitCards中无值
	if (iCards[0] == -1)
	{
		return iCards;
	}

	transcode();//转换编码
	judgeRepeat();//搜索重牌

	switch (type)
	{
	case 0:
		judgeRocket();//判断火箭
		return iRocket;
	case 1:
		judgeBomb();//判断炸弹
		return iBomb;
	case 2:
		judgeThree();//判断三条
		return iThree;
	case 3:
		judgeLinkThree();//判断三顺
		return iLinkThree;
	case 4:
		judgeLinkSingle();//判断单顺
		return iLinkSingle;
	case 5:
		judgeLinkDouble();//判断双顺
		return iLinkDouble;
	case 6:
		judgeDouble();//判断对牌
		return iDouble;
	case 7:
		judgeSingle();//判断单牌
		return iSingle;
	default:
		return NULL;
	}
}
