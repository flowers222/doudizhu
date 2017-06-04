#pragma once
//////////////////////////////////
//////�ƾ���Ϣ����̨///////////////
//���ܣ����ڳ�ʼ���־֣������ƾ�////
////////////////////////////////
#include "Ddz.h"
#include "CardsInfo.h"
#include "MonteCarloSearchEngine.h"
#include "CallNum.h"
#include<list>
class BoardMessageControl
{
public:
	BoardMessageControl();
	~BoardMessageControl();
	void InitTurn(struct Ddz * pDdz);				//P01
	void InputMsg(struct Ddz * pDdz);				//P02
	void AnalyzeMsg(struct Ddz * pDdz);				//P03
	void GetDou(struct Ddz * pDdz);					//P0301
	void GetInf(struct Ddz * pDdz);					//P0302
	void GetDea(struct Ddz * pDdz);					//P0303
	void GetBid(struct Ddz * pDdz);					//P0304
	void GetLef(struct Ddz * pDdz);					//P0305
	void GetPla(struct Ddz * pDdz);					//P0306
	void CalPla(struct Ddz * pDdz);					//P030601
	void UpdateMyPla(struct Ddz * pDdz);			//P030602
	void UpdateHisPla(struct Ddz * pDdz);			//P030603
	void GetGam(struct Ddz * pDdz);					//P0307
	void OutputMsg(struct Ddz * pDdz);				//P04
	void CalOthers(struct Ddz * pDdz);				//P05
	void InitRound(struct Ddz * pDdz);				//M04
	int CalBid(struct Ddz * pDdz);					//I02
	void OtherPlayerCards(Ddz *pDdz, int *iCards);
	CardsInfo* CInfo; //��InitTurn�б���ʼ��
	MonteCarloSearchEngine *engine;//��InitTurn�б���ʼ��
	char cUpPlayer;
	char cDownPlayer;
	//int otherPlayerCards[54];
};

