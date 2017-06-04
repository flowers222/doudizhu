#include "BoardMessageControl.h"

BoardMessageControl::BoardMessageControl()
{

}


BoardMessageControl::~BoardMessageControl()
{
	delete(engine);
}
void BoardMessageControl::InitTurn(struct Ddz * pDdz)
{
	engine = new MonteCarloSearchEngine();
	CInfo = CardsInfo::Instance();
	pDdz->iTurnTotal = 1;				//��ʼ��������
	pDdz->iTurnNow = 1;					//��ʼ����ǰ�ִ�
	pDdz->iRoundTotal = 1;				//��ʼ���ܾ���
	pDdz->iRoundNow = 1;				//��ʼ����ǰ�ִ�
	InitRound(pDdz);					//��ʼ����������
}
//P01-END

//M04-START���ñ������ݳ�ʼֵ
//����޶���:������ ����޶�ʱ��:16-07-31
//�޸�Ŀ��:otherPlayerCards��ʼ��
void BoardMessageControl::InitRound(struct Ddz * pDdz)
{
	int i, j;
	pDdz->iStatus = 1;					//��ʼ����������״̬
	strcpy_s(pDdz->sCommandIn, "");		//��ʼ������ͨ����������
	strcpy_s(pDdz->sCommandOut, "");		//��ʼ������ͨ���������
	for (i = 0; i < 21; i++)
	{		//��ʼ����������
		pDdz->iOnHand[i] = -1;
	}
	//��ʼ��otherPlayerCards
	for (i = 0; i < 54; i++)
	{
		pDdz->iOnOtherHand[i] = i;
	}
	pDdz->iOnOtherHand[54] = -1;
	for (i = 0; i<162; i++)				//��ʼ������������
		for (j = 0; j<21; j++)
			pDdz->iOnTable[i][j] = -2;
	for (i = 0; i < 21; i++)			//��ʼ�����ֳ���
		pDdz->iToTable[i] = -1;
	strcpy_s(pDdz->sVer, "");				//��ʼ������Э��汾��
	strcpy_s(pDdz->sVer, kPlayerName);		//��ʼ�����ֲ���ѡ�ֳƺ�
	pDdz->cDir = 'B';						//��ʼ��������ҷ�λ���
	pDdz->cLandlord = 'B';				//��ʼ�����ֵ�����λ���
	pDdz->cWinner = 'B';					//��ʼ������ʤ�߷�λ���
	for (i = 0; i < 3; i++)				//��ʼ�����ֽ���
		pDdz->iBid[i] = -1;
	pDdz->iLastPassCount = 2;		//��ǰ��������PASS����ֵ��[0,2],��ֵ2����������ȡ0��һ��PASSȡ1������PASSȡ2��
	pDdz->iLastTypeCount = 0;		//��ǰ��������������ֵ��[0,1108],��ֵ0��iLastPassCount=0ʱ����ֵ��=1ʱ����ԭֵ��=2ʱֵΪ0��
	pDdz->iLastMainPoint = -1;		//��ǰ�������Ƶ�����ֵ��[0,15],��ֵ-1��iLastPassCount=0ʱ����ֵ����=1ʱ����ԭֵ��=2ʱֵΪ-1��
	pDdz->iBmax = 0;					//��ʼ�����ֽ��Ʒ���
	pDdz->iOTmax = 0;					//��ʼ����������������
}
//MO4-END

//P02-START������Ϣ
//����޶���:÷��,����޶�ʱ��:15-02-08
void BoardMessageControl::InputMsg(struct Ddz * pDdz)
{
	cin.getline(pDdz->sCommandIn, 80);
}
//P02-END

//P0301-START��ȡ������汾��ϢDOU
//����޶���:�ź���&÷��,����޶�ʱ��:15-02-10 21:04
//�޶����ݼ�Ŀ��:�޶�kPlayerNmae
void BoardMessageControl::GetDou(struct Ddz * pDdz)
{
	int i;								//��ѭ������
	for (i = 0; pDdz->sCommandIn[i] != '\0'; i++)
		pDdz->sVer[i] = pDdz->sCommandIn[i];
	pDdz->sVer[i] = '\0';
	strcpy_s(pDdz->sCommandOut, "NAME ");
	strcat_s(pDdz->sCommandOut, kPlayerName);
}
//P0301-END



//P0302-START��ȡ�������־���ϢINF
//����޶���:�ų�&÷��,����޶�ʱ��:15-02-10
void BoardMessageControl::GetInf(struct Ddz * pDdz)		//�ִ���Ϣ������(����������Ϣ����Ӧд������Ա������):����INFO 1/4 1/9 9 2450     ���OK INFO
{
	char c;					//�浱ǰ�ֽ���Ϣ
	int iCount = 0;			//��¼���ݸ���
	int iTemp = 0;			//�м����
	int iMessage[7] = { 0 };		//��¼��������
	int i;
	for (i = 5; pDdz->sCommandIn[i] != '\0'; i++)
	{
		c = pDdz->sCommandIn[i];
		if (c >= '0' && c <= '9')											//��ǰ�ַ�Ϊ0-9
		{
			iTemp = iTemp * 10 + c - '0';
			iMessage[iCount] = iTemp;								//����ѡ����
		}
		if (c == ',')														//��ǰ�ַ�Ϊ����,
		{
			iCount++;
			iTemp = 0;
		}
	}
	pDdz->iTurnNow = iMessage[0];						//��ǰ�ִ�
	pDdz->iTurnTotal = iMessage[1];						//������
	pDdz->iRoundNow = iMessage[2];						//��ǰ�ִ�
	pDdz->iRoundTotal = iMessage[3];						//�ܾ���
	pDdz->iLevelUp = iMessage[4];							//��������
	pDdz->iScoreMax = iMessage[5];						//�÷ֽ���
	InitRound(pDdz);//��ʼ������
	strcpy_s(pDdz->sCommandOut, "OK INFO");
}
//P0302-END

//P0303-START��ȡ������������ϢDEA
//����޶���:������ ����޶�ʱ��:16-07-31 17:03
//�޶����ݼ�Ŀ��:���ӳ�ʼ�����ֺ���������������ͱ���,�����������������Ϣ
void BoardMessageControl::GetDea(struct Ddz * pDdz)
{
	int i;			      //��ѭ������
	int iNow = 0;		  //��ǰ���������
	int iCardId = 0;	//��ǰ�����Ʊ���
	char c;			      //��ǰָ���ַ�
	pDdz->cDir = pDdz->sCommandIn[5];     //��ȡ����AI��λ���
	if (pDdz->cDir == 'B')
	{
		cUpPlayer = 'A';
		cDownPlayer = 'C';
	}
	if (pDdz->cDir == 'A')
	{
		cUpPlayer = 'C';
		cDownPlayer = 'B';
	}
	if (pDdz->cDir == 'C')
	{
		cUpPlayer = 'B';
		cDownPlayer = 'A';
	}
	pDdz->cLastPlay = cUpPlayer;
	pDdz->iUpPlayerCards = 0;
	pDdz->iDownPlayerCards = 0;
	for (i = 0; i < 21; i++)//����iOnhand[]
	{
		pDdz->iOnHand[i] = -1;
	}

	for (i = 6; pDdz->sCommandIn[i] != '\0'; i++)	//���ζ�ȡ����ָ��
	{
		c = pDdz->sCommandIn[i];			      //cΪ��ǰָ���ַ�
		if (c >= '0' && c <= '9')				        //��ǰ�ַ�Ϊ0-9
		{
			iCardId = iCardId * 10 + c - '0';
			pDdz->iOnHand[iNow] = iCardId;
		}
		if (c == ',')							            //��ǰ�ַ�Ϊ����,
		{
			iCardId = 0;
			iNow++;
		}
	}
	CInfo->SortById(pDdz->iOnHand);
	OtherPlayerCards(pDdz, pDdz->iOnHand);
	strcpy_s(pDdz->sCommandOut, "OK DEAL");  //�ظ���Ϣ
	CInfo->SortById(pDdz->iOnHand);  //iOnHand[]��С��������
}
//P0303-END

//I02-START���㼺�����Ʋ��Խӿ�
int BoardMessageControl::CalBid(struct Ddz * pDdz)
{
	CallNum *CNum = new CallNum();
	int iMyBid = CNum->CallCardAnalyze(pDdz);		//����
	delete(CNum);
	return iMyBid;
}
//P0304-START��ȡ�����������ϢBID
//����޶���:��˼��&÷��,����޶�ʱ��:15-02-08
void BoardMessageControl::GetBid(struct Ddz * pDdz)
{
	if (pDdz->sCommandIn[4] == 'W')					//���������ϢΪBID WHAT
	{
		strcpy_s(pDdz->sCommandOut, "BID _0");
		pDdz->sCommandOut[4] = pDdz->cDir;
		pDdz->iBid[pDdz->cDir - 'A'] = CalBid(pDdz);//���ý��ƺ���
		pDdz->sCommandOut[5] = pDdz->iBid[pDdz->cDir - 'A'] + '0';
		pDdz->sCommandOut[6] = '\0';
		return;
	}
	if (pDdz->sCommandIn[4] >= 'A'&&pDdz->sCommandIn[4] <= 'C')  //������ϢΪBID **
	{
		pDdz->iBid[pDdz->sCommandIn[4] - 'A'] = pDdz->sCommandIn[5] - '0';
		strcpy_s(pDdz->sCommandOut, "OK BID");
		return;
	}
}
//P0304-END

//P0305-START��ȡ�����������ϢLEF
//����޶���:����&÷��,����޶�ʱ��:15-02-08
void BoardMessageControl::GetLef(struct Ddz * pDdz)
{
	int i, iCount = 0;
	char c;
	pDdz->cLandlord = pDdz->sCommandIn[9];    //ȷ��������
	pDdz->iLef[0] = 0;
	pDdz->iLef[1] = 0;
	pDdz->iLef[2] = 0;
	for (i = 10; pDdz->sCommandIn[i] != '\0'; i++)
	{
		c = pDdz->sCommandIn[i];
		if (c >= '0' && c <= '9')
			pDdz->iLef[iCount] = pDdz->iLef[iCount] * 10 + c - '0';
		else
			iCount++;
	}
	if (pDdz->sCommandIn[9] == pDdz->cDir)
	{
		pDdz->iOnHand[17] = pDdz->iLef[0];
		pDdz->iOnHand[18] = pDdz->iLef[1];
		pDdz->iOnHand[19] = pDdz->iLef[2];
		pDdz->iOnHand[20] = -1;
		OtherPlayerCards(pDdz, pDdz->iLef);
		pDdz->iUpPlayerCards = 17;
		pDdz->iDownPlayerCards = 17;
	}
	if (pDdz->sCommandIn[9] == cUpPlayer)
	{
		pDdz->iUpPlayerCards = 20;
		pDdz->iDownPlayerCards = 17;
	}
	if (pDdz->sCommandIn[9] == cDownPlayer)
	{
		pDdz->iUpPlayerCards = 17;
		pDdz->iDownPlayerCards = 20;
	}
	if (pDdz->sCommandIn[9] == pDdz->cDir)
		strcpy_s(pDdz->sCommandOut, "OK LEFTOVER");
	CInfo->SortById(pDdz->iOnHand);					//iOnHand[]��С��������
}
//P0305-END

//P0305-START �������
//����޶���:������,����޶�ʱ��:16-07-31
//�޶�����:����iOnOtherHand
void BoardMessageControl::CalPla(struct Ddz * pDdz)
{
	//int iMax = 0;
	//SearchEngine *engine = new SearchEngine();

	///////���itoTable/////
	memset(pDdz->iToTable,-1,sizeof(pDdz->iToTable));
	///////////////
	engine->SearchAGoodMove(pDdz);
	int i=0;
	//���¼���Ҫ������
	CInfo->SortById(engine->ibestMove);
	for (i = 0; engine->ibestMove[i] >= 0; i++)
		pDdz->iToTable[i] = engine->ibestMove[i];
	pDdz->iToTable[i] = -1;//��-1��Ϊ���.


}
//P030602-START���ݼ������Ƹ�������
//����޶���:÷��&�ĺ��н�,����޶�ʱ��:15-03-01
//�޶����ݼ�Ŀ��:�޸ļ���������
void BoardMessageControl::UpdateMyPla(struct Ddz * pDdz)
{
	int i, j, k;
	if (pDdz->iToTable[0] == -1)	//Pass
	{
		pDdz->iOnTable[pDdz->iOTmax][0] = -1;
		pDdz->iLastPassCount++;
		if (pDdz->iLastPassCount >= 2)	//��������PASS
		{
			pDdz->iLastPassCount = 0;
			pDdz->iLastTypeCount = 0;
			pDdz->iLastMainPoint = -1;
		}
		pDdz->iOTmax++;
	}
	else						//����PASS
	{
		//����������
		for (i = 0; pDdz->iToTable[i] >= 0; i++)
			pDdz->iOnTable[pDdz->iOTmax][i] = pDdz->iToTable[i];
		pDdz->iOnTable[pDdz->iOTmax][i] = -1;
		pDdz->iOTmax++;
		//����������

		for (j = 0; pDdz->iToTable[j] >= 0; j++)
		{
			for (i = 0; pDdz->iOnHand[i] >= 0; i++)
				if (pDdz->iOnHand[i] == pDdz->iToTable[j])
				{
					for (k = i; pDdz->iOnHand[k] >= 0; k++)
						pDdz->iOnHand[k] = pDdz->iOnHand[k + 1];
					break;
				}
		}
		pDdz->iLastPassCount = 0;
		pDdz->iLastTypeCount = CInfo->AnalyzeTypeCount(pDdz->iToTable);
		pDdz->iLastMainPoint = CInfo->AnalyzeMainPoint(pDdz->iToTable);
		pDdz->cLastPlay = pDdz->cDir;
	}
}
//PO30602-END

//P030603-START�������˳��Ƹ�������
//����޶���:������,����޶�ʱ��:16-07-31
//�޶����ݣ���¼�Ƶ�iOnOtherHand
void BoardMessageControl::UpdateHisPla(struct Ddz * pDdz)
{
	int i;
	int iCardId;
	int iNow;
	char c;
	pDdz->cLastPlay = pDdz->sCommandIn[5];
	//����������
	if (pDdz->sCommandIn[6] == '-')// PLAY ?-1 ��PASS
	{
		pDdz->iOnTable[pDdz->iOTmax][0] = -1;
		pDdz->iLastPassCount++;
		if (pDdz->iLastPassCount >= 2)	//��������PASS
		{
			pDdz->iLastPassCount = 0;
			pDdz->iLastTypeCount = 0;
			pDdz->iLastMainPoint = -1;
		}
		pDdz->iOTmax++;
	}
	else						// PLAY ����
	{
		for (i = 0; i<21; i++)							//����iToTable[]
			pDdz->iToTable[i] = -1;
		iCardId = 0;
		iNow = 0;
		for (i = 6; pDdz->sCommandIn[i] != '\0'; i++)		//���ζ�ȡ����
		{
			c = pDdz->sCommandIn[i];					//cΪ��ǰָ���ַ�
			if (c >= '0' && c <= '9')				        //��ǰ�ַ�Ϊ0-9
			{
				iCardId = iCardId * 10 + c - '0';
				pDdz->iToTable[iNow] = iCardId;
			}
			if (c == ',')									//��ǰ�ַ�Ϊ����,
			{
				iCardId = 0;
				iNow++;
			}
		}
		//����������,����iOnOtherHand��
		for (i = 0; pDdz->iToTable[i] >= 0; i++)
		{
			pDdz->iOnTable[pDdz->iOTmax][i] = pDdz->iToTable[i];
		}
		pDdz->iOnTable[pDdz->iOTmax][i] = -1;
		if (pDdz->cLastPlay == cUpPlayer)
			pDdz->iUpPlayerCards -= i;
		else if (pDdz->cLastPlay == cDownPlayer)
			pDdz->iDownPlayerCards -= i;

		OtherPlayerCards(pDdz, pDdz->iToTable);
		pDdz->iLastPassCount = 0;
		pDdz->iLastTypeCount = CInfo->AnalyzeTypeCount(pDdz->iToTable);
		pDdz->iLastMainPoint = CInfo->AnalyzeMainPoint(pDdz->iToTable);
		pDdz->iOTmax++;
	}
}
//PO30603-END
//P0306-START��ȡ�����������ϢPLA
//����޶���:÷��,����޶�ʱ��:15-02-08
void BoardMessageControl::GetPla(struct Ddz * pDdz)
{
	if (pDdz->sCommandIn[5] == 'W')					//������ϢΪPLAY WHAT��Ӧ���ó��Ƽ��㺯���������
	{
		//cout << "calPla" << endl;
		CalPla(pDdz);					//���ó��Ƽ��㺯���������
		strcpy_s(pDdz->sCommandOut, "PLAY _");
		pDdz->sCommandOut[5] = pDdz->cDir;		//��������Ԥ����Ϣ׼����sCommandOut����
		CInfo->AppendCardsToS(pDdz->iToTable, pDdz->sCommandOut);		//Ҫ��������iToTable[]�е�����ת��Ϊ�ַ������ӵ�sCommandOut��
		UpdateMyPla(pDdz);		//���ݼ������Ƹ�������
	}
	else										//�����յ���ϢΪ������ҳ���
	{
		if (pDdz->sCommandIn[5] == pDdz->cDir)//�������
			return;
		UpdateHisPla(pDdz);		//�������˳��Ƹ�������
		strcpy_s(pDdz->sCommandOut, "OK PLAY");//�ظ��յ�
	}
	//��ǰ������1
	//pDdz->iOTmax++;
}
//P0306-END

//P0307-START��ȡ������ʤ����ϢGAM
//����޶���:÷��,����޶�ʱ��:15-02-08 00:13
void BoardMessageControl::GetGam(struct Ddz * pDdz)
{
	pDdz->cWinner = pDdz->sCommandIn[9];			//ʤ���߷�λ���
	if (pDdz->iRoundNow == pDdz->iRoundTotal)		//�����ǰ������ÿ�־����ʱ
	{
		pDdz->iStatus = 0;							//����״̬������Ϊ����
	}
	else											//����
	{
		pDdz->iRoundNow++;							//��ǰ�ִμ�1
		InitRound(pDdz);							//����״̬������Ϊ���¿�ʼ��һ��
	}
	strcpy_s(pDdz->sCommandOut, "OK GAMEOVER");
}
//PO307-END


//P03-START����������Ϣ
//����޶���:÷��,����޶�ʱ��:15-02-08 00:13
void BoardMessageControl::AnalyzeMsg(struct Ddz * pDdz)
{
	char sShort[4];
	int i;
	for (i = 0; i<3; i++)
		sShort[i] = pDdz->sCommandIn[i];
	sShort[3] = '\0';
	if (strcmp(sShort, "DOU") == 0)					//�汾��Ϣ
	{
		GetDou(pDdz);
		return;
	}
	if (strcmp(sShort, "INF") == 0)					//�־���Ϣ
	{
		GetInf(pDdz);
		return;
	}
	if (strcmp(sShort, "DEA") == 0)					//������Ϣ
	{
		GetDea(pDdz);
		return;
	}
	if (strcmp(sShort, "BID") == 0)					//���ƹ���
	{
		GetBid(pDdz);
		return;
	}
	if (strcmp(sShort, "LEF") == 0)					//������Ϣ
	{
		GetLef(pDdz);
		return;
	}
	if (strcmp(sShort, "PLA") == 0)					//���ƹ���
	{
		GetPla(pDdz);
		return;
	}
	if (strcmp(sShort, "GAM") == 0)					//ʤ����Ϣ
	{
		GetGam(pDdz);
		return;
	}
	if (strcmp(sShort, "EXI") == 0)					//ǿ���˳�
	{
		exit(0);
	}
	strcpy_s(pDdz->sCommandOut, "ERROR at module AnalyzeMsg,sCommandIn without match");
	return;
}
//P03-END





//P04-START�����Ϣ
//����޶���:÷��,����޶�ʱ��:15-02-08 00:13
void BoardMessageControl::OutputMsg(struct Ddz * pDdz)
{
	cout << pDdz->sCommandOut << endl;
}
//P04-END

//P05-START������������
//����޶���:÷��,����޶�ʱ��:15-02-08
void BoardMessageControl::CalOthers(struct Ddz * pDdz)
{
	pDdz->iVoid = 0;
}
//P05-END


//����Ҫɾ�����ƣ�������λ�������ʣ�ƣ����洢��iOnOtherHand��
//����޶��ߣ�������
//����޸�ʱ�䣺2016/7/31
void BoardMessageControl::OtherPlayerCards(Ddz *pDdz, int *iCards)
{
	auto deleteElem = [](int *iOnHand, int elem){
		int i = 0, j = 0;
		for (j = 0; j < 55; j++)
		{

			if (iOnHand[j] == elem)
				i++;
			else
			{
				iOnHand[j - i] = iOnHand[j];
			}
		}
	};
	int i, j, k;

	if (iCards == pDdz->iLef)
	{
		for (j = 0; j < 3; j++)
		{
			deleteElem(pDdz->iOnOtherHand, iCards[j]);
		}
	}
	else
	{
		for (j = 0; iCards[j] >= 0; j++)
		{
			deleteElem(pDdz->iOnOtherHand, iCards[j]);
		}
	}
}