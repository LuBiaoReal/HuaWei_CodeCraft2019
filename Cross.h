#pragma once
#include<map>
#include<list>
#include"Road.h"

const int MAX_CAR_START_TIME = 50;
//路口的类声明
enum TurningDirection
{
	Straight, Left, Right, Impasse
};
struct CCross
{
public:
	//初始化函数
	void Init(int crossid, int roadid1, int roadid2, int roadid3, int roadid4);
	//构造函数
	CCross(int crossid = -1, int roadid1 = -1, int roadid2 = -1, int roadid3 = -1, int roadid4 = -1);
	//复制构造函数
	CCross(const CCross& cross);
	//赋值运算符重载
	CCross& operator = (const CCross& cross);
	
	//-------用到了的函数-----
	
	//返回路口中的方向信息
	//根据直行>左转>右转的顺序，依次返回3-2-1,失败返回-1
	//roadidstart是起始道路的id,roadidend是终点的道路id
	TurningDirection GetDirection(int roadidstart, int roadidend);
	
	//得到入口又多少辆等待的车辆
	int GetWaitingNumber();
	//得到出口道路
	CRoad* GetExitRoad(int roadID);
	//得到所有路上跑的车的数量
	int GetRuningCarNumber();
	//将车驶出车库,返回false表示当前堵塞
	bool DriveFromGarage(CCar startCar, int startTime);
	//得到此车车道的对面车道的ID
	int GetFrontRoadID(int currentRoadID);
	//-----------------------
	//路口结构体的常用属性
	int m_nCrossID;       //路口的ID号
	int m_nCrossIndex;    //路口的下标
	int m_nRoadID_1;      //形成路口的道路1
	int m_nRoadID_2;      //形成路口的道路2
	int m_nRoadID_3;      //形成路口的道路3
	int m_nRoadID_4;      //形成路口的道路4
   //每个路口道路的顺序,第一个int为道路号，第二个为道路在这个路口对应的方向序列号（1， 2， 3， 4）
	std::map<int, int>m_mapRoadOrder; 
	CRoad* m_ptOutAdj;       //出口头指针
	CRoad* m_ptInAdj;		//入口头指针

	std::vector<std::vector<CCar>> m_vecvecGarage;	//车库
};

