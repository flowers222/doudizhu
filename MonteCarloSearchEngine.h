/*******************************************************************
*
*  �ļ�����: MonteCarloSearchEngine.h
*  ��Ҫ����:ʹ�����ؿ����������㷨����
*
*  ��������:2016-6
*  ����:������
*  ˵��:koudingkouding@gmail.com ������������
******************************************************************/

#pragma once
#include <windows.h>
#include <list>
#include <map>
#include "RecommendCards.h"
#include<math.h>
//#include <spdlog.h>
#define MaxRunTime 10//����ִ�����ؿ����㷨�����ʱ��
class MonteCarloSearchEngine
{
public:
	MonteCarloSearchEngine();
	~MonteCarloSearchEngine();
	void Init();
	void SearchAGoodMove(Ddz *pDdz);
	int RunMonteCarloSimulation(int node);
	Ddz *pRealDdz;
	int iAllPlayTimes;//�ܹ�ִ�����ؿ���ģ��Ĵ���
	int iMaxDepth;
	fDdz *pMyDdzBoard;
	fDdz *pUpDdzBoard;
	fDdz *pDownDdzBoard;
	int iTotalPlay;
	int iMyUpPlayerCards[21];
	int iMyDownPlayerCards[21];
	int ibestMove[21];
	RecommendCards *pRecomCards;
	//���ؿ������ڵ�
	struct MTCNode
	{
		int node;//�ڵ��±�
		int iWin;//����Ӫģ��Ӯ����
		int iPlay;//���ڵ㱻���ʴ���
		float fUCB1;//UCB1ֵ����ʼֵΪ-1
		float fAverage;
		int iCards[21];//�ýڵ����
	};
	int iNodesCount;
	int iLastValidCards[21];//���һ����Ч����
	MTCNode *nodesList;
	//int iPlaArr[20][21];
	CardsInfo *pCinfo;
	int iDepthPos[4];//ÿ��ڵ���iPLArr��ʼλ���±�
	//
	// ���㲢���±��ڵ�UCB1ֵ
	void UpdateUCB1(MTCNode nodes[], int node, int Winner, int iTotalPlay);
	void Update(int node, int iDepth);

	// �����ڵ㣬�����ؽڵ���
	int MakeMove(int iDepth);

	// ���ò����ɵĽڵ���ӵ��б���
	//int AddDepthNodesInList(int iDepth);
	int IsGameOver();

	void GetOtherPlayerCards(int* iUpPlayerCards, int* iDownPlayerCards);
	void InitBoard();

	// ���ұ�
	int GetRandomNumFromRange(int iStart, int iEnd,int iDepth);

	int nodeSeed;
	/////�����õ�����
	int AIWin;
	int UpWin;
	int DownWin;

};

