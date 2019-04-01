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
	int m_nCarId;			            //车辆id
	int m_nStartTime;		            //实际发车时间
	int m_nPlanStartTime;		        //计划发车时间
	int m_nSystemRunningTime;	        //系统调度时间
	std::vector<int> m_vecVisitedPath;	//经过的路径
};

// 图的抽象基类定义#有权无向图
//顶点(路口)与边的权重都是int类型
class GraphLink {
public:
	GraphLink(std::string &roadPath, std::string &crossPath, std::string &carPathPath, std::string &resultPath);                            //构造函数，建立一个空的邻接表
	~GraphLink();                           //析构函数，删除一个邻接表
	bool GraphEmpty() const {				//判图空否	
		return (m_nNumEdges == 0);
	}
	//返回当前顶点数
	int GetNumberOfVertices() {					
		return m_nNumVertices;
	}
	//返回当前边数	
	int GetNumberOfEdges() {					
		return m_nNumEdges;
	}
	//输出图的所有和边的信息
	friend std::ostream& operator <<(std::ostream& out, GraphLink& G);

	//-------图类必须实现的一些接口------------------
	CCross* GetCrossByIndex(const int crossIndex);   //通过下标取位置为index的的路口
	CCross* GetCrossById(const int crossId) {        //通过路口的Id获取对应路口的指针
		for (int i = 0; i < m_nNumVertices; i++) {
			if (crossId == m_ptNodeTable[i].m_nCrossID)
				return &m_ptNodeTable[i];        //找到，返回地址
		}
		return nullptr;                          //未找到，返回空
	}

	int GetWeight(int crossIndex1, int crossIndex2); //返回边(crossIndex1,crossIndex2)上的权值
	int GetRoadID(int crossIndex1, int crossIndex2); //返回边(crossIndex1, crossIndex2)上的路名
	bool InsertVertex(const CCross& cross);	   //在图中插入一个顶点cross,参数直接是cross对象
	bool InsertEdge(const CRoad& road);         //插入边，参数为道路对象
	//---寻路算法---
	int ShortestPath(int v1, int /*dist[]*/v2, std::stack<int>& path/*int path[]*/);//单源寻找v1-v2的最短路径，迪杰斯特拉算法
	std::string m_strResultPath;                //输出文件路径
	
	//--------解死锁----------
	//根据起点的startCrossId与边的名称，获取终点ID
	int GetRoadEndCrossID(const int startCrossId, const int roadId);
	//改变车辆路径
	void ChangeCarPath(CCar* car, CCross* cross);
	
	//--------装入初始化的车辆，道路，路口----------
	std::vector<CCross> m_vecCross;//路口数组
	std::vector<CRoad> m_vecRoad;  //道路数组
	std::vector<CCar> m_vecCar;    //车辆数组
	//-----必要属性---------
	std::vector<std::vector<int>>m_vecvecWeight;             //Floyd寻找到的最短路径的权值
	std::vector<std::vector<std::vector<int>>>m_vecvecPath;   //Floyd寻找到的最短路径Cross序列
	CCross* m_ptNodeTable;         //顶点表
	std::vector<Result> m_nResult;	//存储结果

	//-------判题器的实现---------
	//模拟结果
	void AnalogConsequence();
	//模拟交规
	void TrafficRules();

	//-----判题器用到的函数------
	//返回所有处于等待状态的节点Index
	std::vector<int> GetWaitingCrossIndex();
	//判断进入路口时，会不会与其他的车辆冲突
	bool Conflict(std::vector<CCar*> otherCars, CCar* car, CCross *cross);

	//最大车辆参数
	int CAR_NUM;
	//得到地图的名字,区分是哪一个地图
	std::string getMapName(std::string mapPath);
	//判断map里边有1和2那个多
	int checkWhichMap(std::string mapName);
	

private:
	int m_nMaxVertices;			//图中最大顶点数
	int m_nNumEdges;		    //当前边数
	int m_nNumVertices;			//当前顶点数
	//佛洛依德：求出所有顶点之间的最短路径
	void Floyd(/*int ** a, *//*int ** path*/);
	int GetVertexPos(const int vertex) {   //给出顶点在图中的位置
		for (int i = 0; i < m_nNumVertices; i++)
			if (m_ptNodeTable[i].m_nCrossID == vertex)
				return i;
		return -1;
	}
	//读写文件入口
	void ReadFile(const std::string path, const std::string type);
	void WriteFile(const std::string path, const std::vector<Result> &result);


};





