#include <stdio.h>
#include <string.h>
#include <iostream>
#define kPlayerName "���۶�����"
#define kPlaMax 500
#pragma once


using namespace std;
//�޸ļ�¼��
//�޸��ˣ�������
//���ڣ�2016/7/13
//ȡ��iPlaCount��ģ������б�ʹ�øı���趨,iPlaArr��PASS����Ϊ-3
struct Ddz
{
	int  iStatus;			//����״̬-1����,0����,1��ʼ
	char sCommandIn[80];	//ͨ����������
	char sCommandOut[80];	//ͨ���������
	int iOnHand[21];		//�����ƣ�����ֵ��ʼ��Ϊ-1��
	int iOnTable[162][21];	//�ѳ������飨����ֵ��ʼ��Ϊ-2��ÿ����һ���ƣ���-1��β��Pass��Ϊ-1
	int iUpPlayerCards; //�ϼ���������
	int iDownPlayerCards;//�¼���������
	//int iRandomOnHand[2][21];
	int iToTable[21];		//Ҫ������
	int iOnOtherHand[55];   //������������е���
//	int iOTherHandSim[2][21]; //ģ������������е��� ,0�Ǽ�����һ����ң�1�Ǽ�����һ����ҡ�
	char sVer[80];			//Э��汾��
	char sName[80];			//����ѡ�ֳƺ�
	char cDir;				//��ҷ�λ���
	char cLandlord;			//������ҷ�λ���
	char cLastPlay;         //��ģ�����ʱ��λ������ҷ�λ���
	char cWinner;			//ʤ���߷�λ���
	int iBid[3];			//���ƹ���
	int iBmax;				//��ǰ��������ֵ��{0,1,2,3}
	int iOTmax;				//��ǰ��������
	int iRoundNow;			//��ǰ�ִ�
	int iRoundTotal;		//���ܾ���
	int iTurnNow;			//��ǰ�ִ�
	int iTurnTotal;			//������
	int iLevelUp;			//����ѡ����
	int iScoreMax;			//ת���÷ֽ���
	int iVoid;				//��Ͼ���м������
	int iLef[3];			//���ֵ���
	//int iPlaCount;			//���ֳ��ƿ��н�������ֵ��[0��kPlaMax-1]��
	int iLastPassCount;		//��ǰ��������PASS����ֵ��[0,2],��ֵ2����������ȡ0��һ��PASSȡ1������PASSȡ2��
	int iLastTypeCount;		//��ǰ��������������ֵ��[0,1108],��ֵ0��iLastPassCount=0ʱ����ֵ��=1ʱ����ԭֵ��=2ʱֵΪ0��//���³���
	int iLastMainPoint;		//��ǰ�������Ƶ�����ֵ��[0,15],��ֵ-1��iLastPassCount=0ʱ����ֵ����=1ʱ����ԭֵ��=2ʱֵΪ-1��
	//int iPlaArr[kPlaMax][21];		//���ֳ��ƿ��н⼯�������ƽ����Ʊ���������-1���,-2��β��//PASS����Ϊ-3��
	//int iPlaOnHand[21];		//ģ����ƺ����Ʊ���
};

