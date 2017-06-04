#ifndef _CardsInfo
#define _CardsInfo

#include "Ddz.h"
//////////////////////////////////
//////手牌信息分析///////////////
//功能：用于初始化轮局，更新牌局////
////////////////////////////////

class CardsInfo
{
public:
	CardsInfo();
	~CardsInfo();
	static CardsInfo* Instance();

	///判断牌型////
	int IsType0Pass(int iCs[]);						//B00
	int IsType1Rocket(int iCs[]);					//B01
	int IsType2Bomb(int iCs[]);						//B02
	int	IsType3Single(int iCs[]);					//B03
	int	IsType4Double(int iCs[]);					//B04
	int	IsType5Three(int iCs[]);					//B05
	int IsType6ThreeOne(int iCs[]);					//B0601
	int IsType6ThreeDouble(int iCs[]);				//B0602
	int IsType7LinkSingle(int iCs[]);				//B07
	int IsType8LinkDouble(int iCs[]);				//B08
	int	IsType9LinkThree(int iCs[]);				//B09
	int	IsType10LinkThreeSingle(int iCs[]);			//B1001
	int	IsType10LinkThreeDouble(int	iCs[]);			//B1002
	int IsType11FourSingle(int iCs[]);				//B1101
	int IsType11FourDouble(int iCs[]);				//B1102

	///分析牌型及点数//
	int	AnalyzeTypeCount(int iCards[]);				//I03
	int	AnalyzeMainPoint(int iCards[]);				//I04

	///////手牌数计算、排序/////
	int CountCards(int iCards[]);					//I01
	void SortById(int iCards[]);					//M01
	void SortByMuch(int iCards[]);					//M02
	void SortByMain(int iCards[]);					//M03

	//其它//
	void AppendCardsToS(int iCards[], char sMsg[]);	//M05

	// 计算主牌最大点数
	int AnalyzeMainMaxPoint(int iCards[]);
	int LastPlayerCardsCount(Ddz * pDdz);
	//去除重复的牌并从小到大排序
	int GetUnReaptedCards(int iCards[]);
};


#endif _CardsInfo