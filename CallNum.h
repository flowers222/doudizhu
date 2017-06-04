#ifndef _CallNum
#define _CallNum
#include <stdio.h>
#include <stdlib.h>
#include "CardsInfo.h"

class CallNum
{
public:
	CallNum();
	~CallNum();
	static CallNum* Instance();

	int CalBid(struct Ddz * pDdz);					//I02


	CardsInfo* CInfo;
	int CardForce(int iCards[]);
	int CallCardAnalyze(Ddz* pDdz);
};

#endif _CallNum