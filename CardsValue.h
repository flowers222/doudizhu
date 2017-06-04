#include "CardsInfo.h"


class CardsValue
{
public:
	CardsValue();
	~CardsValue();
	static CardsValue* Instance();
	enum PlayerType
	{
		lord=0,//地主
		farmer=1,//农民
	};
	CardsInfo *CInfo;//牌类信息
	PlayerType lastPlayerType, thisPlayerType, nextPlayerType;//上家，本家，下家

	//局面信息变量
	int* pLastPlayCards;
	int* pThisPlayerCards ;//当前层玩家牌
	int *pNextPlayCards;
	int *pOnTable;//桌面最新牌，当首位为-1时表示前两家pass，可以随意出牌
	Ddz *pDdz;

	//UNDONE
	int CardsPower[12];//存储十二个牌型的牌力，包括弃权牌型，参见《单机斗地主引擎介绍.ppt》第五页，暂定牌力按排列顺序+1
	int CallSumCardsPower(int iPlaOnHand);//计算传入手牌总牌力
	int CallMinHandCount(int iPlaOnHand[]);//重新整合牌型，计算最小手数
	//DONE
	void InitCardsValueEngine(int* LastPlayCards, int* ThisPlayerCards, int* NextPlayCards, int *Ontable, char ThisPlayer, bool isMaxNode, Ddz *pDdz);//初始化引擎
	void InitPlayerType(char thisPlayer);//初始化玩家类型
	//UNDONE


	//-------------------------------
	//-----------------------------
	double CalSumValue();//计算总优势值

	void AnalyseCards(int iPlaOnHand[]);//拆牌，并分析牌型并传入不同的分析函数进行赋值
	double CalCardsValue01(int iPlaOnHand);//给单牌赋值
	double CalCardsValue02(int iPlaOnHand);//给对子赋值
	double CalCardsValue03(int iPlaOnHand);//给连对赋值
	double CalCardsValue04(int iPlaOnHand);//给顺子赋值
	double CalCardsValue05(int iPlaOnHand);//给三带一赋值
	double CalCardsValue06(int iPlaOnHand);//给炸弹赋值
	double CalCardsValue07(int iPlaOnHand);//给飞机赋值

	double dSum;//优势值



};

