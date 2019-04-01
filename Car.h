#pragma once
#include <algorithm> 
#include <vector>
#include<iostream>
class CCar
{
public:
	//��ʼ������
	void Init(int carid, int origin, int dest, int maxspeed, int gotime);
	//���캯��
	CCar(int carid, int origin, int dest, int maxspeed, int gotime);
	//���ƹ��캯��
	CCar(const CCar& car);
	//��ֵ���������
	CCar& operator = (const CCar& car);
	
	//----�õ��ĺ���---
	//��ʻ�����ڵ�λʱ���߹���һ�ξ���
	void Run() {
		m_nDistance += m_nCurSpeed;
		if (m_nDistance > m_nCurRoadLength)
			m_nDistance = m_nCurRoadLength;
	}
	void Run(int distance) {
		m_nDistance += distance;
		if (m_nDistance > m_nCurRoadLength)
			m_nDistance = m_nCurRoadLength;		
	}

	//�õ��ڵ�ǰ��·�����˶�Զ
	int GetDistance() {
		return m_nDistance;
	}

	//�õ����ĵ�ǰ�ٶ�,���ٶ�ʱ��·�ͳ�������ٶȵĽ�Сֵ
	int GetCurrentSpeed() {
		return m_nCurSpeed;
	}

	//�õ���һ��Ҫ�ߵ�·��id,Ϊ-1���ʾ������,ֱ�ӽ����յ�
	int GetNextRoadID() {
		if (m_nCurRoadIndex == m_vecShortestPath.size() - 1) {
			return -1;
		}
		else
		{
			return m_vecShortestPath[m_nCurRoadIndex + 1];
		}
	}

	//�õ���ǰ���ߵĵ�·ID
	int GetCurrentRoadID() {
		//��û�г���
		if (m_nCurRoadIndex == -1) {
			return -3;
		}
		else
		{
			return m_vecShortestPath[m_nCurRoadIndex];
		}
	}

	//�õ��ڵ�ǰ��·��ʣ����롣��ǰ��·���ȣ��ڱ����ʱ������Ϊ��·����
	int GetRemianingLength() {
		return m_nCurRoadLength - GetDistance();
	}

	//�任��·��������ͨ������ֵȷ���Ƿ����ɹ�
	bool ChangeRoad(int ChangedID, int roadLength) {
		if (m_nCurRoadIndex != m_vecShortestPath.size() - 1 && ChangedID == m_vecShortestPath[m_nCurRoadIndex + 1]) {
			m_nCurRoadIndex++;
			m_nCurRoadLength = roadLength;
			return true;
		}
		else
		{
			std::cout << ChangedID << "���ʧ��" << std::endl;
			exit(-1);
			return false;
		}
	}

	//��·�ں�������ʻ�ٶ�
	void SetCurrentSpeed(const int speed) {
		m_nCurSpeed = speed;
	}

	//��·�ں�������ʻ����
	void SetInitialDistance(const int distance) {
		m_nDistance = distance;
	}

	//--------����ֵ---------
	//��ų������е�·��
	std::vector<int> m_vecShortestPath;		
	bool m_bIsWaiting;	       //������ʻ״̬��ture���ڵȴ������������������
	int m_nStartTime;         //����ʱ��				  
	int m_nCarID;          //����ID
	int m_nOrigin;         //ʼ����
	int m_nDestination;    //Ŀ�ĵ�
	int m_nMaxSpeed;       //���ʱ��
	//�������в���
	int m_nRealStartTime;	//ʵ�ʳ���ʱ��
	int m_nCurSpeed;		//��ʻ��ʵ��ʱ��
	int m_nDistance;		//�ڵ�·����ʻ�ľ���
	//����״̬
	//bool isfinished;		//�����Ƿ���ʻ���һ��ʱ�䵥λ
	//�ܳ����ĳ���·��
	int m_nCurRoadIndex;    //��¼��ǰ��ʻ�ĵ�·ID�±�
	int m_nCurRoadLength;	//��¼��ǰ��ʻ�ĵ�·����
};

