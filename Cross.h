#pragma once
#include<map>
#include<list>
#include"Road.h"

const int MAX_CAR_START_TIME = 50;
//·�ڵ�������
enum TurningDirection
{
	Straight, Left, Right, Impasse
};
struct CCross
{
public:
	//��ʼ������
	void Init(int crossid, int roadid1, int roadid2, int roadid3, int roadid4);
	//���캯��
	CCross(int crossid = -1, int roadid1 = -1, int roadid2 = -1, int roadid3 = -1, int roadid4 = -1);
	//���ƹ��캯��
	CCross(const CCross& cross);
	//��ֵ���������
	CCross& operator = (const CCross& cross);
	
	//-------�õ��˵ĺ���-----
	
	//����·���еķ�����Ϣ
	//����ֱ��>��ת>��ת��˳�����η���3-2-1,ʧ�ܷ���-1
	//roadidstart����ʼ��·��id,roadidend���յ�ĵ�·id
	TurningDirection GetDirection(int roadidstart, int roadidend);
	
	//�õ�����ֶ������ȴ��ĳ���
	int GetWaitingNumber();
	//�õ����ڵ�·
	CRoad* GetExitRoad(int roadID);
	//�õ�����·���ܵĳ�������
	int GetRuningCarNumber();
	//����ʻ������,����false��ʾ��ǰ����
	bool DriveFromGarage(CCar startCar, int startTime);
	//�õ��˳������Ķ��泵����ID
	int GetFrontRoadID(int currentRoadID);
	//-----------------------
	//·�ڽṹ��ĳ�������
	int m_nCrossID;       //·�ڵ�ID��
	int m_nCrossIndex;    //·�ڵ��±�
	int m_nRoadID_1;      //�γ�·�ڵĵ�·1
	int m_nRoadID_2;      //�γ�·�ڵĵ�·2
	int m_nRoadID_3;      //�γ�·�ڵĵ�·3
	int m_nRoadID_4;      //�γ�·�ڵĵ�·4
   //ÿ��·�ڵ�·��˳��,��һ��intΪ��·�ţ��ڶ���Ϊ��·�����·�ڶ�Ӧ�ķ������кţ�1�� 2�� 3�� 4��
	std::map<int, int>m_mapRoadOrder; 
	CRoad* m_ptOutAdj;       //����ͷָ��
	CRoad* m_ptInAdj;		//���ͷָ��

	std::vector<std::vector<CCar>> m_vecvecGarage;	//����
};

