#pragma once
#include<vector>
#include<queue>
#include<list>
#include"Car.h"
#include<iostream>

struct CRoad
{
public:
	//��ʼ������
	void Init(int roadid, int len, int limitspeed, int lanenum,
		int start, int end, bool istwoway = true);

	CRoad() { m_ptOutLink = m_ptInLink = nullptr; }            //Ĭ�Ϲ��캯��
	//���캯��
	CRoad(int roadid, int len, int limitspeed, int lanenum,
		int start, int end, bool istwoway = true);
	//���ƹ��캯��
	CRoad(const CRoad& road);
	//��ֵ���������
	CRoad& operator = (const CRoad& road);	
	bool operator != (const CRoad& R)const {
		if (this->m_nStartID != R.m_nStartID && this->m_nEndID != R.m_nEndID)
			return true;
		return false;
	}
	

	
	//----�õ��˵ĺ���----
	//������ͨ���ĳ�����״̬
	void DriveAllCarJustOnRoadToEndState(int channel);
	//����Ƿ����г�������Ϊ����ֹ״̬	
	bool AllCarsNotWaiting();
	//�õ���һ���ȼ��ĳ���,�����ȳ����ĳ�������
	CCar* GetFirstPriorityCar(int &channelIndex);
	//ʻ������,�������ĳ���
	CCar DriveOut(const int carID);
	//��鳵���������û�еȴ������赲���������¼����ܹ��������ʻ�ľ���
	bool CheckExitCondition(int & maxRunningDistance);
	//���복��,��ʻ��ʵ�ʾ��룬������ٶ�
	bool DriveIn(CCar car, const int drivingDistance, int currentSpeed);
	//ͳ���ж��ٸ�����waiting״̬�ĳ���,�ж�����	
	int GetWaitingCarsNumber();
	//�õ�һ�ź�������г���������ʻ������״̬
	void DriveBehindCarsToEnd(int channel);
	//�ó������������еȴ��ĳ����ߵ�����״̬
	void DriveAllWaitingCarToEnd(int channelIndex);

	//-------����--------------
	//��ǰ���ȳ���,ÿ��ϵͳ���Ƚ�������0,���G��ʻ����������Լ�1
	//int currentChannelIndex;
	//��·�ṹ��ĳ�������
	int m_nRoadID;     //��·ID
	int m_nRoadIndex;  //��·�±�
	int m_nLength;     //��·����
	int m_nLimitSpeed; //�����
	int m_nLaneNum;    //��������
	int m_nStartID;    //��ʼ��ID
	int m_nEndID;      //�յ�ID
	bool m_bTwoWay;    //�Ƿ�˫��
	float m_fWeight;      //��·��Ȩ��weight = len/speed;
	//vector�Ĵ�СΪ4���Ǳ�ʾ��4������
	//queue��ÿ���������泵���ľ�����Ϣ������������ID,�����ڵ�λ��
    //��m_nRoadCondition��0��ʼ����,0λ���ǵ�һ��������ȥ��0λ��ɾ��
	std::vector<std::vector<CCar>> m_nRoadCondition;
	CRoad* m_ptOutLink;      //������һ������ָ��
	CRoad* m_ptInLink;		 //�����һ������ָ��
};

