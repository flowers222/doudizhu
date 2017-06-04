#include "CallNum.h"

CallNum::CallNum()
{
	CInfo = CardsInfo::Instance();
}

CallNum::~CallNum()
{
}

CallNum* CallNum::Instance()
{
	static CallNum instance;

	return &instance;
}



//�����жϺ���
//���ߣ�����־
//ʱ�䣺2016.4.1.13:11
//��һ���޸ģ�2016.7.12.17:13 �߼��޸�
//�ڶ����޸ģ�2016.7.13.12:37 ��������޸�
//�������޸ģ�2016.7.16.15:04 �������ͷ���BUG
//���Ĵ��޸ģ�2016.8.13.13:45 ���ӻ��׷��4������
//������޸ģ�2016.8.15.15:53 ȡ�����׷��4������
int CallNum::CardForce(int iCards[])
{
	int iMyCards[18] = { 0 };//��������
	int iRepeat[27] = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 };//���Ƽ�����
	int iOrder[18] = { 0 };//����

	int iBomb[5] = { 0 };//ը��
	int iPair[9] = { 0 };//����
	int iThree[6] = { 0 };//����һ
	int iStraight[9] = { 0 };//˳��/*ֻ�ǵ�һ�ź����һ��  ������ĸ�˳�� ����һ���1*/
	int iSingle = 0;//���ţ����ƣ�
	int iEntry = 0;//������

	int iForce = 0;//����

	bool bLock = true;//������

	//�������ͣ�ת��Ϊʵ�ʵ�����������������
	for (int i = 0; i < 17; i++)
	{
		if (iCards[i] / 4 == 0)
			iMyCards[i] = 3;    //3
		if (iCards[i] / 4 == 1)
			iMyCards[i] = 4;    //4
		if (iCards[i] / 4 == 2)
			iMyCards[i] = 5;    //5
		if (iCards[i] / 4 == 3)
			iMyCards[i] = 6;    //6
		if (iCards[i] / 4 == 4)
			iMyCards[i] = 7;    //7
		if (iCards[i] / 4 == 5)
			iMyCards[i] = 8;    //8
		if (iCards[i] / 4 == 6)
			iMyCards[i] = 9;    //9
		if (iCards[i] / 4 == 7)
			iMyCards[i] = 10;    //10
		if (iCards[i] / 4 == 8)
			iMyCards[i] = 11;    //J
		if (iCards[i] / 4 == 9)
			iMyCards[i] = 12;    //Q
		if (iCards[i] / 4 == 10)
			iMyCards[i] = 13;    //K
		if (iCards[i] / 4 == 11)
		{
			iMyCards[i] = 14;    //A
			iEntry++;
		}
		if (iCards[i] / 4 == 12)
		{
			iMyCards[i] = 22;    //2
			iEntry++;
		}
		if (iCards[i] == 52)
		{
			iMyCards[i] = 66;    //С��
			iEntry++;
		}
		if (iCards[i] == 53)
		{
			iMyCards[i] = 99;    //����
			iEntry++;
		}
	}

	//���ӡ�����һ��ը���ĵ�һ��ɸѡ
	for (int i = 0, j = 0; i < 17; i++)
	{
		if (iMyCards[i] == iMyCards[i + 1])
		{
			iRepeat[j] = iMyCards[i + 1];
			iRepeat[j + 1]++; /*�ظ�����*/
		}
		else
		{
			j += 2;
			if (iMyCards[i] != iMyCards[i - 1] && iMyCards[i] != iMyCards[i + 1] && iMyCards[i] < 14)
			{
				iSingle++;  /*��������1*/
			}
		}
	}
	//�ڶ���ɸѡ
	for (int i = 1, j = 0, k = 0, l = 0; i < 26; i += 2)
	{
		//ը���ļ���
		if (iRepeat[i] == 4)
		{
			iBomb[j] = iRepeat[i - 1];
			j++;
		}
		//���ӵļ���
		if (iRepeat[i] == 2)
		{
			iPair[k] = iRepeat[i - 1];
			k++;
		}
		//����һ�ļ���
		if (iRepeat[i] == 3)
		{
			iThree[l] = iRepeat[i - 1];
			l++;
		}
	}

	//˳�ӵĵ�һ��ɸѡ
	for (int i = 0, j = 0; i < 17; i++)
	{
		if (iMyCards[i + 1] == iMyCards[i] + 1)
		{
			iOrder[j] = iMyCards[i];
			j++;
		}
		else if (iMyCards[i + 1] > iMyCards[i] + 1 || i == 16)
		{
			if (iOrder[j - 1] + 1 == iMyCards[i])
			{
				iOrder[j] = iMyCards[i];
				j += 2;
			}
		}
		else
		{
			continue;
		}
	}
	//˳�ӵĵڶ���ɸѡ
	for (int i = 0, j = 0; i < 17; i++)
	{
		if (iOrder[i] != 0)
		{
			if (bLock)
			{
				iStraight[j] = iOrder[i];
				j++;
				bLock = false;
			}
		}
		else
		{
			if (iOrder[i - 1] == 0 && iOrder[i + 1] == 0)
			{
				break;
			}
			else
			{
				if (iOrder[i - 1] - iStraight[j - 1] >= 4)
				{
					iStraight[j] = iOrder[i - 1];
					j++;
					iStraight[j] = 0;
					j++;
					bLock = true;
				}
				else
				{
					iStraight[j - 1] = 0;
					j--;
					bLock = true;
				}
			}
		}
	}

	//�����ļ���
	for (int i = 0; i < 17; i++)
	{
		//ը����4��������������м��ƣ���Ҫ��1����Ϊ��ʱ���ò����ը��ȥ��˳�ӡ�
		if (i < 4 && iBomb[i] != 0)
		{
			iForce += 4;
			if (iBomb[i] < 11 && iBomb[i] > 6)
			{
				iForce -= 1;
			}
		}
		//������2������
		if (iMyCards[i] == 99)
		{
			iForce += 2;
		}
		//С����1������
		if (iMyCards[i] == 66)
		{
			iForce += 1;
		}
		//һ��2��1������
		if (i < 9 && iPair[i] == 2)
		{
			iForce += 1;
		}
		//3��J�����ϼ�1������
		if (i < 6 && iThree[i] > 10)
		{
			iForce += 1;
			//3��2�Ӽ�2������
			if (iThree[i] == 22)
			{
				iForce += 2;
			}
		}
		//˳�Ӽ�1��2������
		if (i < 9 && iStraight[i + 1] != 0 && iStraight[i] != 0)
		{
			if (iStraight[i + 1] - iStraight[i] > 8 || iStraight[i + 1] > 12)
			{
				iForce += 2;
			}
			else
			{
				iForce += 1;
			}
		}
	}
	//�����Ŵ��ڵ��ţ����ű����״���
	if (iEntry > iSingle)
	{
		iForce += 1;
	}
	//������С�ڵ���,���Ų��ױ�����
	if (iEntry < iSingle)
	{
		int iTemp = iSingle - iEntry;
		iForce -= iTemp;
	}

	return iForce;
}




//�жϽ��Ƶ���
//���ߣ�����־
//ʱ�䣺2016.3.31.12:52
//��һ���޸ģ�2016.4.1.13:10
//�ڶ����޸ģ�2016.4.20.23:36  У���ϼҽ��Ƶ������㷽��
//�������޸ģ�2016.7.11.15:45  У���Ƶ��ж�
//���Ĵ��޸ģ�2016.7.13.18:02  У�����Ʒ���
//������޸ģ�2016.7.16.15:04  У�����ƾ���
//�������޸ģ�2016.8.7.11:22  ���Ƶ������޸�
//���ߴ��޸ģ�2016.8.13.14:48  ���Ƶ������޸�
//�ڰ˴��޸ģ�2016.8.15.15:53  У���ϼҽ��Ƶ������㷽��,�޸��Ƿ���ͷ�ҵ��жϷ���
int CallNum::CallCardAnalyze(Ddz* pDdz)
{
	int iInHand = 0;//������
	int iPip = 0;//�Ƶ�
	int iCallPoint = -1;//���ս�����
	int iHeadCallPointSecond;//�ϼҽ����ܵ���
	int iHeadCallPointFirst;//���ϼҽ��Ƶ���

	//��ȡ�ϼҽ�����
	if (pDdz->iBid[0] == -1)
	{
		iHeadCallPointFirst = -1;//���ϼҽ��Ƶ���
		iHeadCallPointSecond = -1;//�ϼҽ��Ƶ���
	}
	else if (pDdz->iBid[1] == -1)
	{
		iHeadCallPointFirst = -1;//���ϼҽ��Ƶ���
		iHeadCallPointSecond = pDdz->iBid[0];//�ϼҽ��Ƶ���
	}
	else if (pDdz->iBid[2] == -1)
	{
		iHeadCallPointFirst = pDdz->iBid[0];//���ϼҽ��Ƶ���
		iHeadCallPointSecond = pDdz->iBid[1];//�ϼҽ��Ƶ���
	}

	//�Ƶ��ж�
	for (int i = 0; i<17; i++)
	{
		if (pDdz->iOnHand[i] >= 44)
		{
			iInHand = pDdz->iOnHand[i];
			if (iInHand >= 53)
				iPip += 4;
			else if (iInHand >= 52)
				iPip += 3;
			else if (iInHand >= 48)
				iPip += 2;
			else
				iPip += 1;
		}
	}

	//���Ʒ���
	//���ƵĻ�������������6���Ƶ�
	if (iPip > 5)
	{
		int iPipNum = CardForce(pDdz->iOnHand) + iPip;

		if (iHeadCallPointFirst == -1 && iHeadCallPointSecond == -1)
		{
			if (iPipNum > 11)
			{
				iCallPoint = 3;
			}
			else if (iPipNum > 9)
			{
				iCallPoint = 2;
			}
			else if (iPipNum > 7)
			{
				iCallPoint = 1;
			}
			else
			{
				iCallPoint = 0;
			}
		}
		if (iHeadCallPointFirst == -1 && iHeadCallPointSecond != -1)
		{
			if (iPipNum > 11 && iHeadCallPointSecond != 3)
			{
				iCallPoint = 3;
			}
			else if (iPipNum > 9)
			{
				if (iHeadCallPointSecond <= 2)
				{
					iCallPoint = 3;
				}
				else
				{
					iCallPoint = 0;
				}
			}
			else if (iPipNum > 7)
			{
				if (iHeadCallPointSecond <= 1)
				{
					iCallPoint = 2;
				}
				else
				{
					iCallPoint = 0;
				}
			}
			else
			{
				if (iHeadCallPointSecond == 0)
				{
					iCallPoint = 1;
				}
				else
				{
					iCallPoint = 0;
				}
			}
		}
		if (iHeadCallPointFirst != -1 && iHeadCallPointSecond != -1)
		{
			if (iPipNum > 11 && iHeadCallPointFirst != 3 && iHeadCallPointSecond != 3)
			{
				iCallPoint = 3;
			}
			else if (iPipNum > 9)
			{
				if (iHeadCallPointSecond <= 2 && iHeadCallPointFirst <= 2)
				{
					iCallPoint = 3;
				}
				else
				{
					iCallPoint = 0;
				}
			}
			else if (iPipNum > 7)
			{
				if (iHeadCallPointSecond <= 1 && iHeadCallPointFirst <= 1)
				{
					iCallPoint = 2;
				}
				else
				{
					iCallPoint = 0;
				}
			}
			else
			{
				if (iHeadCallPointSecond == 0 && iHeadCallPointFirst == 0)
				{
					iCallPoint = 1;
				}
				else
				{
					iCallPoint = 0;
				}
			}
		}
	}
	else
		iCallPoint = 0;

	//���ؽ��Ƶ���
	return iCallPoint;
}
