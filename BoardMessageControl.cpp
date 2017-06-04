#include "BoardMessageControl.h"

BoardMessageControl::BoardMessageControl()
{

}


BoardMessageControl::~BoardMessageControl()
{
	delete(engine);
}
void BoardMessageControl::InitTurn(struct Ddz * pDdz)
{
	engine = new MonteCarloSearchEngine();
	CInfo = CardsInfo::Instance();
	pDdz->iTurnTotal = 1;				//初始化总轮数
	pDdz->iTurnNow = 1;					//初始化当前轮次
	pDdz->iRoundTotal = 1;				//初始化总局数
	pDdz->iRoundNow = 1;				//初始化当前局次
	InitRound(pDdz);					//初始化本局数据
}
//P01-END

//M04-START重置本局数据初始值
//最后修订者:范晓梦 最后修订时间:16-07-31
//修改目的:otherPlayerCards初始化
void BoardMessageControl::InitRound(struct Ddz * pDdz)
{
	int i, j;
	pDdz->iStatus = 1;					//初始化本局引擎状态
	strcpy_s(pDdz->sCommandIn, "");		//初始化本局通信输入内容
	strcpy_s(pDdz->sCommandOut, "");		//初始化本局通信输出内容
	for (i = 0; i < 21; i++)
	{		//初始化本局手牌
		pDdz->iOnHand[i] = -1;
	}
	//初始化otherPlayerCards
	for (i = 0; i < 54; i++)
	{
		pDdz->iOnOtherHand[i] = i;
	}
	pDdz->iOnOtherHand[54] = -1;
	for (i = 0; i<162; i++)				//初始化本局桌面牌
		for (j = 0; j<21; j++)
			pDdz->iOnTable[i][j] = -2;
	for (i = 0; i < 21; i++)			//初始化本局出牌
		pDdz->iToTable[i] = -1;
	strcpy_s(pDdz->sVer, "");				//初始化本局协议版本号
	strcpy_s(pDdz->sVer, kPlayerName);		//初始化本局参赛选手称呼
	pDdz->cDir = 'B';						//初始化本局玩家方位编号
	pDdz->cLandlord = 'B';				//初始化本局地主方位编号
	pDdz->cWinner = 'B';					//初始化本局胜者方位编号
	for (i = 0; i < 3; i++)				//初始化本局叫牌
		pDdz->iBid[i] = -1;
	pDdz->iLastPassCount = 2;		//当前桌面连续PASS数（值域[0,2],初值2，正常出牌取0，一家PASS取1，两家PASS取2）
	pDdz->iLastTypeCount = 0;		//当前桌面牌型张数（值域[0,1108],初值0，iLastPassCount=0时更新值，=1时保留原值，=2时值为0）
	pDdz->iLastMainPoint = -1;		//当前桌面主牌点数（值域[0,15],初值-1，iLastPassCount=0时更新值，，=1时保留原值，=2时值为-1）
	pDdz->iBmax = 0;					//初始化本局叫牌分数
	pDdz->iOTmax = 0;					//初始化本局桌面牌手数
}
//MO4-END

//P02-START输入信息
//最后修订者:梅险,最后修订时间:15-02-08
void BoardMessageControl::InputMsg(struct Ddz * pDdz)
{
	cin.getline(pDdz->sCommandIn, 80);
}
//P02-END

//P0301-START获取并处理版本信息DOU
//最后修订者:张洪民&梅险,最后修订时间:15-02-10 21:04
//修订内容及目的:修订kPlayerNmae
void BoardMessageControl::GetDou(struct Ddz * pDdz)
{
	int i;								//简单循环变量
	for (i = 0; pDdz->sCommandIn[i] != '\0'; i++)
		pDdz->sVer[i] = pDdz->sCommandIn[i];
	pDdz->sVer[i] = '\0';
	strcpy_s(pDdz->sCommandOut, "NAME ");
	strcat_s(pDdz->sCommandOut, kPlayerName);
}
//P0301-END



//P0302-START获取并处理轮局信息INF
//最后修订者:张晨&梅险,最后修订时间:15-02-10
void BoardMessageControl::GetInf(struct Ddz * pDdz)		//轮次信息处理函数(处理数字信息，对应写入对象成员变量中):输入INFO 1/4 1/9 9 2450     输出OK INFO
{
	char c;					//存当前字节信息
	int iCount = 0;			//记录数据个数
	int iTemp = 0;			//中间变量
	int iMessage[7] = { 0 };		//记录数据数组
	int i;
	for (i = 5; pDdz->sCommandIn[i] != '\0'; i++)
	{
		c = pDdz->sCommandIn[i];
		if (c >= '0' && c <= '9')											//当前字符为0-9
		{
			iTemp = iTemp * 10 + c - '0';
			iMessage[iCount] = iTemp;								//晋级选手数
		}
		if (c == ',')														//当前字符为逗号,
		{
			iCount++;
			iTemp = 0;
		}
	}
	pDdz->iTurnNow = iMessage[0];						//当前轮次
	pDdz->iTurnTotal = iMessage[1];						//总轮数
	pDdz->iRoundNow = iMessage[2];						//当前局次
	pDdz->iRoundTotal = iMessage[3];						//总局数
	pDdz->iLevelUp = iMessage[4];							//晋级数量
	pDdz->iScoreMax = iMessage[5];						//得分界限
	InitRound(pDdz);//初始化数据
	strcpy_s(pDdz->sCommandOut, "OK INFO");
}
//P0302-END

//P0303-START获取并处理牌套信息DEA
//最后修订者:范晓梦 最后修订时间:16-07-31 17:03
//修订内容及目的:增加初始化上手和下手玩家手牌数和编码,更新其他玩家手牌信息
void BoardMessageControl::GetDea(struct Ddz * pDdz)
{
	int i;			      //简单循环变量
	int iNow = 0;		  //当前处理牌序号
	int iCardId = 0;	//当前处理牌编码
	char c;			      //当前指令字符
	pDdz->cDir = pDdz->sCommandIn[5];     //获取本家AI方位编号
	if (pDdz->cDir == 'B')
	{
		cUpPlayer = 'A';
		cDownPlayer = 'C';
	}
	if (pDdz->cDir == 'A')
	{
		cUpPlayer = 'C';
		cDownPlayer = 'B';
	}
	if (pDdz->cDir == 'C')
	{
		cUpPlayer = 'B';
		cDownPlayer = 'A';
	}
	pDdz->cLastPlay = cUpPlayer;
	pDdz->iUpPlayerCards = 0;
	pDdz->iDownPlayerCards = 0;
	for (i = 0; i < 21; i++)//清理iOnhand[]
	{
		pDdz->iOnHand[i] = -1;
	}

	for (i = 6; pDdz->sCommandIn[i] != '\0'; i++)	//依次读取牌码指令
	{
		c = pDdz->sCommandIn[i];			      //c为当前指令字符
		if (c >= '0' && c <= '9')				        //当前字符为0-9
		{
			iCardId = iCardId * 10 + c - '0';
			pDdz->iOnHand[iNow] = iCardId;
		}
		if (c == ',')							            //当前字符为逗号,
		{
			iCardId = 0;
			iNow++;
		}
	}
	CInfo->SortById(pDdz->iOnHand);
	OtherPlayerCards(pDdz, pDdz->iOnHand);
	strcpy_s(pDdz->sCommandOut, "OK DEAL");  //回复信息
	CInfo->SortById(pDdz->iOnHand);  //iOnHand[]从小到大排序
}
//P0303-END

//I02-START计算己方叫牌策略接口
int BoardMessageControl::CalBid(struct Ddz * pDdz)
{
	CallNum *CNum = new CallNum();
	int iMyBid = CNum->CallCardAnalyze(pDdz);		//叫牌
	delete(CNum);
	return iMyBid;
}
//P0304-START获取并处理叫牌信息BID
//最后修订者:李思寒&梅险,最后修订时间:15-02-08
void BoardMessageControl::GetBid(struct Ddz * pDdz)
{
	if (pDdz->sCommandIn[4] == 'W')					//如果输入信息为BID WHAT
	{
		strcpy_s(pDdz->sCommandOut, "BID _0");
		pDdz->sCommandOut[4] = pDdz->cDir;
		pDdz->iBid[pDdz->cDir - 'A'] = CalBid(pDdz);//调用叫牌函数
		pDdz->sCommandOut[5] = pDdz->iBid[pDdz->cDir - 'A'] + '0';
		pDdz->sCommandOut[6] = '\0';
		return;
	}
	if (pDdz->sCommandIn[4] >= 'A'&&pDdz->sCommandIn[4] <= 'C')  //输入信息为BID **
	{
		pDdz->iBid[pDdz->sCommandIn[4] - 'A'] = pDdz->sCommandIn[5] - '0';
		strcpy_s(pDdz->sCommandOut, "OK BID");
		return;
	}
}
//P0304-END

//P0305-START获取并处理底牌信息LEF
//最后修订者:杨洋&梅险,最后修订时间:15-02-08
void BoardMessageControl::GetLef(struct Ddz * pDdz)
{
	int i, iCount = 0;
	char c;
	pDdz->cLandlord = pDdz->sCommandIn[9];    //确定地主方
	pDdz->iLef[0] = 0;
	pDdz->iLef[1] = 0;
	pDdz->iLef[2] = 0;
	for (i = 10; pDdz->sCommandIn[i] != '\0'; i++)
	{
		c = pDdz->sCommandIn[i];
		if (c >= '0' && c <= '9')
			pDdz->iLef[iCount] = pDdz->iLef[iCount] * 10 + c - '0';
		else
			iCount++;
	}
	if (pDdz->sCommandIn[9] == pDdz->cDir)
	{
		pDdz->iOnHand[17] = pDdz->iLef[0];
		pDdz->iOnHand[18] = pDdz->iLef[1];
		pDdz->iOnHand[19] = pDdz->iLef[2];
		pDdz->iOnHand[20] = -1;
		OtherPlayerCards(pDdz, pDdz->iLef);
		pDdz->iUpPlayerCards = 17;
		pDdz->iDownPlayerCards = 17;
	}
	if (pDdz->sCommandIn[9] == cUpPlayer)
	{
		pDdz->iUpPlayerCards = 20;
		pDdz->iDownPlayerCards = 17;
	}
	if (pDdz->sCommandIn[9] == cDownPlayer)
	{
		pDdz->iUpPlayerCards = 17;
		pDdz->iDownPlayerCards = 20;
	}
	if (pDdz->sCommandIn[9] == pDdz->cDir)
		strcpy_s(pDdz->sCommandOut, "OK LEFTOVER");
	CInfo->SortById(pDdz->iOnHand);					//iOnHand[]从小到大排序
}
//P0305-END

//P0305-START 申请出牌
//最后修订者:范晓梦,最后修订时间:16-07-31
//修订内容:更新iOnOtherHand
void BoardMessageControl::CalPla(struct Ddz * pDdz)
{
	//int iMax = 0;
	//SearchEngine *engine = new SearchEngine();

	///////清空itoTable/////
	memset(pDdz->iToTable,-1,sizeof(pDdz->iToTable));
	///////////////
	engine->SearchAGoodMove(pDdz);
	int i=0;
	//更新己方要出的牌
	CInfo->SortById(engine->ibestMove);
	for (i = 0; engine->ibestMove[i] >= 0; i++)
		pDdz->iToTable[i] = engine->ibestMove[i];
	pDdz->iToTable[i] = -1;//以-1作为间隔.


}
//P030602-START根据己方出牌更新数据
//最后修订者:梅险&夏侯有杰,最后修订时间:15-03-01
//修订内容及目的:修改减少手中牌
void BoardMessageControl::UpdateMyPla(struct Ddz * pDdz)
{
	int i, j, k;
	if (pDdz->iToTable[0] == -1)	//Pass
	{
		pDdz->iOnTable[pDdz->iOTmax][0] = -1;
		pDdz->iLastPassCount++;
		if (pDdz->iLastPassCount >= 2)	//连续两家PASS
		{
			pDdz->iLastPassCount = 0;
			pDdz->iLastTypeCount = 0;
			pDdz->iLastMainPoint = -1;
		}
		pDdz->iOTmax++;
	}
	else						//不是PASS
	{
		//增加桌面牌
		for (i = 0; pDdz->iToTable[i] >= 0; i++)
			pDdz->iOnTable[pDdz->iOTmax][i] = pDdz->iToTable[i];
		pDdz->iOnTable[pDdz->iOTmax][i] = -1;
		pDdz->iOTmax++;
		//减少手中牌

		for (j = 0; pDdz->iToTable[j] >= 0; j++)
		{
			for (i = 0; pDdz->iOnHand[i] >= 0; i++)
				if (pDdz->iOnHand[i] == pDdz->iToTable[j])
				{
					for (k = i; pDdz->iOnHand[k] >= 0; k++)
						pDdz->iOnHand[k] = pDdz->iOnHand[k + 1];
					break;
				}
		}
		pDdz->iLastPassCount = 0;
		pDdz->iLastTypeCount = CInfo->AnalyzeTypeCount(pDdz->iToTable);
		pDdz->iLastMainPoint = CInfo->AnalyzeMainPoint(pDdz->iToTable);
		pDdz->cLastPlay = pDdz->cDir;
	}
}
//PO30602-END

//P030603-START根据他人出牌更新数据
//最后修订者:范晓梦,最后修订时间:16-07-31
//修订内容：记录牌到iOnOtherHand
void BoardMessageControl::UpdateHisPla(struct Ddz * pDdz)
{
	int i;
	int iCardId;
	int iNow;
	char c;
	pDdz->cLastPlay = pDdz->sCommandIn[5];
	//减少手中牌
	if (pDdz->sCommandIn[6] == '-')// PLAY ?-1 即PASS
	{
		pDdz->iOnTable[pDdz->iOTmax][0] = -1;
		pDdz->iLastPassCount++;
		if (pDdz->iLastPassCount >= 2)	//连续两家PASS
		{
			pDdz->iLastPassCount = 0;
			pDdz->iLastTypeCount = 0;
			pDdz->iLastMainPoint = -1;
		}
		pDdz->iOTmax++;
	}
	else						// PLAY 出牌
	{
		for (i = 0; i<21; i++)							//清理iToTable[]
			pDdz->iToTable[i] = -1;
		iCardId = 0;
		iNow = 0;
		for (i = 6; pDdz->sCommandIn[i] != '\0'; i++)		//依次读取牌码
		{
			c = pDdz->sCommandIn[i];					//c为当前指令字符
			if (c >= '0' && c <= '9')				        //当前字符为0-9
			{
				iCardId = iCardId * 10 + c - '0';
				pDdz->iToTable[iNow] = iCardId;
			}
			if (c == ',')									//当前字符为逗号,
			{
				iCardId = 0;
				iNow++;
			}
		}
		//增加桌面牌,减少iOnOtherHand牌
		for (i = 0; pDdz->iToTable[i] >= 0; i++)
		{
			pDdz->iOnTable[pDdz->iOTmax][i] = pDdz->iToTable[i];
		}
		pDdz->iOnTable[pDdz->iOTmax][i] = -1;
		if (pDdz->cLastPlay == cUpPlayer)
			pDdz->iUpPlayerCards -= i;
		else if (pDdz->cLastPlay == cDownPlayer)
			pDdz->iDownPlayerCards -= i;

		OtherPlayerCards(pDdz, pDdz->iToTable);
		pDdz->iLastPassCount = 0;
		pDdz->iLastTypeCount = CInfo->AnalyzeTypeCount(pDdz->iToTable);
		pDdz->iLastMainPoint = CInfo->AnalyzeMainPoint(pDdz->iToTable);
		pDdz->iOTmax++;
	}
}
//PO30603-END
//P0306-START获取并处理出牌信息PLA
//最后修订者:梅险,最后修订时间:15-02-08
void BoardMessageControl::GetPla(struct Ddz * pDdz)
{
	if (pDdz->sCommandIn[5] == 'W')					//接收信息为PLAY WHAT：应调用出牌计算函数计算出牌
	{
		//cout << "calPla" << endl;
		CalPla(pDdz);					//调用出牌计算函数计算出牌
		strcpy_s(pDdz->sCommandOut, "PLAY _");
		pDdz->sCommandOut[5] = pDdz->cDir;		//输出命令的预备信息准备到sCommandOut数组
		CInfo->AppendCardsToS(pDdz->iToTable, pDdz->sCommandOut);		//要出牌数组iToTable[]中的数字转化为字符并连接到sCommandOut中
		UpdateMyPla(pDdz);		//根据己方出牌更新数据
	}
	else										//否则收到信息为其它玩家出牌
	{
		if (pDdz->sCommandIn[5] == pDdz->cDir)//数据输错
			return;
		UpdateHisPla(pDdz);		//根据他人出牌更新数据
		strcpy_s(pDdz->sCommandOut, "OK PLAY");//回复收到
	}
	//当前手数加1
	//pDdz->iOTmax++;
}
//P0306-END

//P0307-START获取并处理胜负信息GAM
//最后修订者:梅险,最后修订时间:15-02-08 00:13
void BoardMessageControl::GetGam(struct Ddz * pDdz)
{
	pDdz->cWinner = pDdz->sCommandIn[9];			//胜利者方位编号
	if (pDdz->iRoundNow == pDdz->iRoundTotal)		//如果当前局数与每轮局相等时
	{
		pDdz->iStatus = 0;							//引擎状态变量设为结束
	}
	else											//否则
	{
		pDdz->iRoundNow++;							//当前局次加1
		InitRound(pDdz);							//引擎状态变量设为重新开始下一局
	}
	strcpy_s(pDdz->sCommandOut, "OK GAMEOVER");
}
//PO307-END


//P03-START分析处理信息
//最后修订者:梅险,最后修订时间:15-02-08 00:13
void BoardMessageControl::AnalyzeMsg(struct Ddz * pDdz)
{
	char sShort[4];
	int i;
	for (i = 0; i<3; i++)
		sShort[i] = pDdz->sCommandIn[i];
	sShort[3] = '\0';
	if (strcmp(sShort, "DOU") == 0)					//版本信息
	{
		GetDou(pDdz);
		return;
	}
	if (strcmp(sShort, "INF") == 0)					//轮局信息
	{
		GetInf(pDdz);
		return;
	}
	if (strcmp(sShort, "DEA") == 0)					//牌套信息
	{
		GetDea(pDdz);
		return;
	}
	if (strcmp(sShort, "BID") == 0)					//叫牌过程
	{
		GetBid(pDdz);
		return;
	}
	if (strcmp(sShort, "LEF") == 0)					//底牌信息
	{
		GetLef(pDdz);
		return;
	}
	if (strcmp(sShort, "PLA") == 0)					//出牌过程
	{
		GetPla(pDdz);
		return;
	}
	if (strcmp(sShort, "GAM") == 0)					//胜负信息
	{
		GetGam(pDdz);
		return;
	}
	if (strcmp(sShort, "EXI") == 0)					//强制退出
	{
		exit(0);
	}
	strcpy_s(pDdz->sCommandOut, "ERROR at module AnalyzeMsg,sCommandIn without match");
	return;
}
//P03-END





//P04-START输出信息
//最后修订者:梅险,最后修订时间:15-02-08 00:13
void BoardMessageControl::OutputMsg(struct Ddz * pDdz)
{
	cout << pDdz->sCommandOut << endl;
}
//P04-END

//P05-START计算其它数据
//最后修订者:梅险,最后修订时间:15-02-08
void BoardMessageControl::CalOthers(struct Ddz * pDdz)
{
	pDdz->iVoid = 0;
}
//P05-END


//传入要删除的牌，计算两位玩家手中剩牌，并存储在iOnOtherHand中
//最后修订者：范晓梦
//最后修改时间：2016/7/31
void BoardMessageControl::OtherPlayerCards(Ddz *pDdz, int *iCards)
{
	auto deleteElem = [](int *iOnHand, int elem){
		int i = 0, j = 0;
		for (j = 0; j < 55; j++)
		{

			if (iOnHand[j] == elem)
				i++;
			else
			{
				iOnHand[j - i] = iOnHand[j];
			}
		}
	};
	int i, j, k;

	if (iCards == pDdz->iLef)
	{
		for (j = 0; j < 3; j++)
		{
			deleteElem(pDdz->iOnOtherHand, iCards[j]);
		}
	}
	else
	{
		for (j = 0; iCards[j] >= 0; j++)
		{
			deleteElem(pDdz->iOnOtherHand, iCards[j]);
		}
	}
}