#pragma once
#include<limits>
#include<fstream>
#include<iostream>
#include<stack>
#include<queue>
#include<vector>
#include<list>
#include"Car.h"
#include"Cross.h"
#include"Road.h"
#include"SomeFunc.h"
#include <future>
const int maxValue = 2147483647 / 3;
const int UNLOCK_NUM = 10;

struct Result
{
	int m_nCarId;			            //����id
	int m_nStartTime;		            //ʵ�ʷ���ʱ��
	int m_nPlanStartTime;		        //�ƻ�����ʱ��
	int m_nSystemRunningTime;	        //ϵͳ����ʱ��
	std::vector<int> m_vecVisitedPath;	//������·��
};

// ͼ�ĳ�����ඨ��#��Ȩ����ͼ
//����(·��)��ߵ�Ȩ�ض���int����
class GraphLink {
public:
	GraphLink(std::string &roadPath, std::string &crossPath, std::string &carPathPath, std::string &resultPath);                            //���캯��������һ���յ��ڽӱ�
	~GraphLink();                           //����������ɾ��һ���ڽӱ�
	bool GraphEmpty() const {				//��ͼ�շ�	
		return (m_nNumEdges == 0);
	}
	//���ص�ǰ������
	int GetNumberOfVertices() {					
		return m_nNumVertices;
	}
	//���ص�ǰ����	
	int GetNumberOfEdges() {					
		return m_nNumEdges;
	}
	//���ͼ�����кͱߵ���Ϣ
	friend std::ostream& operator <<(std::ostream& out, GraphLink& G);

	//-------ͼ�����ʵ�ֵ�һЩ�ӿ�------------------
	CCross* GetCrossByIndex(const int crossIndex);   //ͨ���±�ȡλ��Ϊindex�ĵ�·��
	CCross* GetCrossById(const int crossId) {        //ͨ��·�ڵ�Id��ȡ��Ӧ·�ڵ�ָ��
		for (int i = 0; i < m_nNumVertices; i++) {
			if (crossId == m_ptNodeTable[i].m_nCrossID)
				return &m_ptNodeTable[i];        //�ҵ������ص�ַ
		}
		return nullptr;                          //δ�ҵ������ؿ�
	}

	int GetWeight(int crossIndex1, int crossIndex2); //���ر�(crossIndex1,crossIndex2)�ϵ�Ȩֵ
	int GetRoadID(int crossIndex1, int crossIndex2); //���ر�(crossIndex1, crossIndex2)�ϵ�·��
	bool InsertVertex(const CCross& cross);	   //��ͼ�в���һ������cross,����ֱ����cross����
	bool InsertEdge(const CRoad& road);         //����ߣ�����Ϊ��·����
	//---Ѱ·�㷨---
	int ShortestPath(int v1, int /*dist[]*/v2, std::stack<int>& path/*int path[]*/);//��ԴѰ��v1-v2�����·�����Ͻ�˹�����㷨
	std::string m_strResultPath;                //����ļ�·��
	
	//--------������----------
	//��������startCrossId��ߵ����ƣ���ȡ�յ�ID
	int GetRoadEndCrossID(const int startCrossId, const int roadId);
	//�ı䳵��·��
	void ChangeCarPath(CCar* car, CCross* cross);
	
	//--------װ���ʼ���ĳ�������·��·��----------
	std::vector<CCross> m_vecCross;//·������
	std::vector<CRoad> m_vecRoad;  //��·����
	std::vector<CCar> m_vecCar;    //��������
	//-----��Ҫ����---------
	std::vector<std::vector<int>>m_vecvecWeight;             //FloydѰ�ҵ������·����Ȩֵ
	std::vector<std::vector<std::vector<int>>>m_vecvecPath;   //FloydѰ�ҵ������·��Cross����
	CCross* m_ptNodeTable;         //�����
	std::vector<Result> m_nResult;	//�洢���

	//-------��������ʵ��---------
	//ģ����
	void AnalogConsequence();
	//ģ�⽻��
	void TrafficRules();

	//-----�������õ��ĺ���------
	//�������д��ڵȴ�״̬�Ľڵ�Index
	std::vector<int> GetWaitingCrossIndex();
	//�жϽ���·��ʱ���᲻���������ĳ�����ͻ
	bool Conflict(std::vector<CCar*> otherCars, CCar* car, CCross *cross);

	//���������
	int CAR_NUM;
	//�õ���ͼ������,��������һ����ͼ
	std::string getMapName(std::string mapPath);
	//�ж�map�����1��2�Ǹ���
	int checkWhichMap(std::string mapName);
	

private:
	int m_nMaxVertices;			//ͼ����󶥵���
	int m_nNumEdges;		    //��ǰ����
	int m_nNumVertices;			//��ǰ������
	//�������£�������ж���֮������·��
	void Floyd(/*int ** a, *//*int ** path*/);
	int GetVertexPos(const int vertex) {   //����������ͼ�е�λ��
		for (int i = 0; i < m_nNumVertices; i++)
			if (m_ptNodeTable[i].m_nCrossID == vertex)
				return i;
		return -1;
	}
	//��д�ļ����
	void ReadFile(const std::string path, const std::string type);
	void WriteFile(const std::string path, const std::vector<Result> &result);


};





