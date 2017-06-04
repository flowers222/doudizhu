
/*******************************************************************
*
*  文件名称: RecommendCards.h
*  简要描述:出牌生成器
*
*  创建日期:2016-6
*  作者:范晓梦
*  说明:koudingkouding@gmail.com 有疑问请邮箱
******************************************************************/
#include "CardsInfo.h"
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "SplitCard.h"
#include "PokerModelAnalysis.h"
//节点数上限
#define PlaMax 100
//推荐出牌生成器
//2016/8/8 修订iplaArr的一层数组为PLAMAX，并将PLAMAX改成50
struct fDdz
{
	int iOnHand[21];		//手中牌（所有值初始化为-1）
	int *iLastOnTable;	//最近一次非Pass出牌数组，以-1结尾，Pass记为-1
	int iUpPlayerCards;//上手玩家手牌数
	int iDownPlayerCards;//下手玩家手牌数
	char cDir;				//玩家方位编号
	char cLandlord;				//地主玩家方位编号
	char cLastPlay;         //上位出牌玩家方位编号
	int iPlaCount;			//多种出牌可行解数量（值域[0，kPlaMax-1]）
	int iPlaArr[PlaMax][21];		//多种出牌可行解集（各出牌解由牌编号升序组成-1间隔,-2收尾）//PASS首项为-3；//pass为-2-1;
	int iLastPassCount;		//当前桌面连续PASS数（值域[0,2],初值2，正常出牌取0，一家PASS取1，两家PASS取2）
	int iLastMainPoint;		//当前桌面主牌点数（值域[0,15],初值-1，iLastPassCount=0时更新值，，=1时保留原值，=2时值为-1）
	int iLastTypeCount;		//当前桌面牌型张数（值域[0,1108],初值0，iLastPassCount=0时更新值，=1时保留原值，=2时值为0）//最新出牌
};
class RecommendCards
{
public:
	RecommendCards();
	~RecommendCards();
	static RecommendCards* Instance();
	int HelpPla(struct fDdz * pDdz, bool IsSim);
	int Help0Pass(fDdz * pDdz, int playerType);
	int Help1Rocket(struct fDdz * pDdz);			//H01
	int Help2Bomb(struct fDdz * pDdz);				//H02
	int Help3Single(struct fDdz * pDdz);			//H03
	int Help4Double(struct fDdz * pDdz);			//H04
	int Help4DoubleSim(struct fDdz * pDdz);
	int Help5Three(struct fDdz * pDdz);				//H05
	int Help6ThreeOne(struct fDdz * pDdz);			//H0601
	int Help6ThreeOneSim(struct fDdz * pDdz);
	int Help6ThreeDouble(struct fDdz * pDdz);		//H0602
	int Help6ThreeDoubleSim(struct fDdz * pDdz);
	int Help7LinkSingle(struct fDdz * pDdz);		//H07
	int Help8LinkDouble(struct fDdz * pDdz);		//H08
	int Help9LinkThree(struct fDdz * pDdz);			//H09
	int Help10LinkThreeSingle(struct fDdz * pDdz);	//H1001
	int Help10LinkThreeDouble(struct fDdz * pDdz);	//H1002
	int Help11FourSingle(struct fDdz * pDdz);		//H1101
	int Help11FourDouble(fDdz * pDdz);		//H1102
	int GetPlayerType(fDdz *pDdz);
	CardsInfo* CInfo;
	int iPlaCount;			//本次模拟出牌生成器产生的多种出牌可行解数量
	SplitCard *pSplitCards;//拆牌类变量
	PokerModelAnalysis *pMoldel;
	int OtherPlayersCards(fDdz* pDdz, Ddz *Ddz);
	int* pCardsOnHand;
	char cAI;//记录AI方位
	void HelpTakeOff(fDdz* pDdz, int num);	//M07
	int InitPlaGenerate(fDdz* pDdz);
	int iTmpSave[100];
	int iLastMaxPoint;//上家最大主牌点数
	// 判断前面两家是否PASS两次
	bool IfPassTwice(fDdz *pDdz);
	// 从iCards里面分离出能打过上家出牌的牌型存到iplaArr里面
	int GetLegalCards(fDdz *pDdz, int *iCards, int *iSave);
	int GetLegalCards(fDdz *pDdz, int *iCards);
	int AddInPlaArr(fDdz *pDdz, int *iCards);
	int IfHelpEnemyCard(fDdz* pDdz);
	// 解析拆牌类产生的数组
	int AnalyzeSplitedCards(int* iCards, int *iSave);
	void AppendCards(int* from, int* to);
	int Help3SingleSim(fDdz* pDdz);
	// 计算手牌最大手数
	int MinHandNum(int* iOnHand);
	int iHandNum;
	int iUpHandNum;
	int iDownHandNum;
	bool ifNeedPass;
	Ddz *pRealDdz;
	bool isSim;
	int iUpCards[21];
	int iDownCards[21];
	bool GetModel(Ddz * prealDdz);
	bool AnalyzeCondition(fDdz* pDdz);
	void DeleteElementInCards(int* iCards, int iCardsCount, int iPoint);
	int AnalyzeSingleLink(int Length, int iLastMainPoint,int *iCards, int* iTmpShunzi);
	int CardsValue(int* iCards);
};

