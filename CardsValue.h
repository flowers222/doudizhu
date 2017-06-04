#include "CardsInfo.h"


class CardsValue
{
public:
	CardsValue();
	~CardsValue();
	static CardsValue* Instance();
	enum PlayerType
	{
		lord=0,//����
		farmer=1,//ũ��
	};
	CardsInfo *CInfo;//������Ϣ
	PlayerType lastPlayerType, thisPlayerType, nextPlayerType;//�ϼң����ң��¼�

	//������Ϣ����
	int* pLastPlayCards;
	int* pThisPlayerCards ;//��ǰ�������
	int *pNextPlayCards;
	int *pOnTable;//���������ƣ�����λΪ-1ʱ��ʾǰ����pass�������������
	Ddz *pDdz;

	//UNDONE
	int CardsPower[12];//�洢ʮ�������͵�������������Ȩ���ͣ��μ��������������������.ppt������ҳ���ݶ�����������˳��+1
	int CallSumCardsPower(int iPlaOnHand);//���㴫������������
	int CallMinHandCount(int iPlaOnHand[]);//�����������ͣ�������С����
	//DONE
	void InitCardsValueEngine(int* LastPlayCards, int* ThisPlayerCards, int* NextPlayCards, int *Ontable, char ThisPlayer, bool isMaxNode, Ddz *pDdz);//��ʼ������
	void InitPlayerType(char thisPlayer);//��ʼ���������
	//UNDONE


	//-------------------------------
	//-----------------------------
	double CalSumValue();//����������ֵ

	void AnalyseCards(int iPlaOnHand[]);//���ƣ����������Ͳ����벻ͬ�ķ����������и�ֵ
	double CalCardsValue01(int iPlaOnHand);//�����Ƹ�ֵ
	double CalCardsValue02(int iPlaOnHand);//�����Ӹ�ֵ
	double CalCardsValue03(int iPlaOnHand);//�����Ը�ֵ
	double CalCardsValue04(int iPlaOnHand);//��˳�Ӹ�ֵ
	double CalCardsValue05(int iPlaOnHand);//������һ��ֵ
	double CalCardsValue06(int iPlaOnHand);//��ը����ֵ
	double CalCardsValue07(int iPlaOnHand);//���ɻ���ֵ

	double dSum;//����ֵ



};

