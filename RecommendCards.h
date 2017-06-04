
/*******************************************************************
*
*  �ļ�����: RecommendCards.h
*  ��Ҫ����:����������
*
*  ��������:2016-6
*  ����:������
*  ˵��:koudingkouding@gmail.com ������������
******************************************************************/
#include "CardsInfo.h"
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "SplitCard.h"
#include "PokerModelAnalysis.h"
//�ڵ�������
#define PlaMax 100
//�Ƽ�����������
//2016/8/8 �޶�iplaArr��һ������ΪPLAMAX������PLAMAX�ĳ�50
struct fDdz
{
	int iOnHand[21];		//�����ƣ�����ֵ��ʼ��Ϊ-1��
	int *iLastOnTable;	//���һ�η�Pass�������飬��-1��β��Pass��Ϊ-1
	int iUpPlayerCards;//�������������
	int iDownPlayerCards;//�������������
	char cDir;				//��ҷ�λ���
	char cLandlord;				//������ҷ�λ���
	char cLastPlay;         //��λ������ҷ�λ���
	int iPlaCount;			//���ֳ��ƿ��н�������ֵ��[0��kPlaMax-1]��
	int iPlaArr[PlaMax][21];		//���ֳ��ƿ��н⼯�������ƽ����Ʊ���������-1���,-2��β��//PASS����Ϊ-3��//passΪ-2-1;
	int iLastPassCount;		//��ǰ��������PASS����ֵ��[0,2],��ֵ2����������ȡ0��һ��PASSȡ1������PASSȡ2��
	int iLastMainPoint;		//��ǰ�������Ƶ�����ֵ��[0,15],��ֵ-1��iLastPassCount=0ʱ����ֵ����=1ʱ����ԭֵ��=2ʱֵΪ-1��
	int iLastTypeCount;		//��ǰ��������������ֵ��[0,1108],��ֵ0��iLastPassCount=0ʱ����ֵ��=1ʱ����ԭֵ��=2ʱֵΪ0��//���³���
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
	int iPlaCount;			//����ģ����������������Ķ��ֳ��ƿ��н�����
	SplitCard *pSplitCards;//���������
	PokerModelAnalysis *pMoldel;
	int OtherPlayersCards(fDdz* pDdz, Ddz *Ddz);
	int* pCardsOnHand;
	char cAI;//��¼AI��λ
	void HelpTakeOff(fDdz* pDdz, int num);	//M07
	int InitPlaGenerate(fDdz* pDdz);
	int iTmpSave[100];
	int iLastMaxPoint;//�ϼ�������Ƶ���
	// �ж�ǰ�������Ƿ�PASS����
	bool IfPassTwice(fDdz *pDdz);
	// ��iCards���������ܴ���ϼҳ��Ƶ����ʹ浽iplaArr����
	int GetLegalCards(fDdz *pDdz, int *iCards, int *iSave);
	int GetLegalCards(fDdz *pDdz, int *iCards);
	int AddInPlaArr(fDdz *pDdz, int *iCards);
	int IfHelpEnemyCard(fDdz* pDdz);
	// �������������������
	int AnalyzeSplitedCards(int* iCards, int *iSave);
	void AppendCards(int* from, int* to);
	int Help3SingleSim(fDdz* pDdz);
	// ���������������
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

