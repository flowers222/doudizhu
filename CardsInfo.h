#ifndef _CardsInfo
#define _CardsInfo

#include "Ddz.h"
//////////////////////////////////
//////������Ϣ����///////////////
//���ܣ����ڳ�ʼ���־֣������ƾ�////
////////////////////////////////

class CardsInfo
{
public:
	CardsInfo();
	~CardsInfo();
	static CardsInfo* Instance();

	///�ж�����////
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

	///�������ͼ�����//
	int	AnalyzeTypeCount(int iCards[]);				//I03
	int	AnalyzeMainPoint(int iCards[]);				//I04

	///////���������㡢����/////
	int CountCards(int iCards[]);					//I01
	void SortById(int iCards[]);					//M01
	void SortByMuch(int iCards[]);					//M02
	void SortByMain(int iCards[]);					//M03

	//����//
	void AppendCardsToS(int iCards[], char sMsg[]);	//M05

	// ��������������
	int AnalyzeMainMaxPoint(int iCards[]);
	int LastPlayerCardsCount(Ddz * pDdz);
	//ȥ���ظ����Ʋ���С��������
	int GetUnReaptedCards(int iCards[]);
};


#endif _CardsInfo