#include "String.h"
#include <iostream>
#include "math.h"
#include <algorithm>
#include "windows.h"
#include <vector>
#include "time.h"
#include "SplitCard.h"
#include "CardsInfo.h"

using namespace std;
class PokerModelAnalysis
{
public:
	PokerModelAnalysis();
	~PokerModelAnalysis();

	SplitCard *A;
	CardsInfo *B;
	Ddz *pMyDdz;

	int Standard1;
	int Standard2;//������������
	int InitialSum;
	int iUtimate[8][40];
	int iMyUtimate[8][40];
	int iOneUtimate[21];//�ϼҵ���
	int iOneDe[21];//�ϼұ��е���
	int iTwoUtimate[21];//�¼ҵ���
	int iTwoDe[21];//�¼ұ��е���
	int Buffer[41];
	int iOnHand[21];
	int Hc[21];
	int UpWrong;
	int DownWrong;


	void Init();
	void GetOtherPokersCards(int *iUpCards, int *iDownCards, Ddz *pMyDdz);
	void GameStart();
	void Gaming();
	void CardsValue(int *pCards);
	void LordBest();
	void Equality();
	void PartnerBest();
	void SaveCards(int i, int *iResult);
	void DeleteCards(int *iResult);
	void ConfirmCards(int *iResult);
	void Analysis(int t);
	void Type3(int *iCs, int k);
	void Type6(int *iCs, int k);
	void Type7(int *iCs, int k);
	void Random(int *Buffer);
	void DeleteB(int *iCs, int a);
	void Fendipai();
	void DeleteC();
};

