#pragma once
#include<vector>
#include<queue>
#include<list>
#include"Car.h"
#include<iostream>

struct CRoad
{
public:
	//初始化函数
	void Init(int roadid, int len, int limitspeed, int lanenum,
		int start, int end, bool istwoway = true);

	CRoad() { m_ptOutLink = m_ptInLink = nullptr; }            //默认构造函数
	//构造函数
	CRoad(int roadid, int len, int limitspeed, int lanenum,
		int start, int end, bool istwoway = true);
	//复制构造函数
	CRoad(const CRoad& road);
	//赋值运算符重载
	CRoad& operator = (const CRoad& road);	
	bool operator != (const CRoad& R)const {
		if (this->m_nStartID != R.m_nStartID && this->m_nEndID != R.m_nEndID)
			return true;
		return false;
	}
	

	
	//----用到了的函数----
	//给所有通道的车设置状态
	void DriveAllCarJustOnRoadToEndState(int channel);
	//检查是否所有车都设置为了终止状态	
	bool AllCarsNotWaiting();
	//得到第一优先级的车辆,和优先车辆的车道索引
	CCar* GetFirstPriorityCar(int &channelIndex);
	//驶出车辆,输入具体的车道
	CCar DriveOut(const int carID);
	//检查车道的入口有没有等待车的阻挡，并且重新计算能够在入口行驶的距离
	bool CheckExitCondition(int & maxRunningDistance);
	//进入车辆,行驶的实际距离，进入的速度
	bool DriveIn(CCar car, const int drivingDistance, int currentSpeed);
	//统计有多少个处于waiting状态的车辆,判断死锁	
	int GetWaitingCarsNumber();
	//让第一排后面的所有车辆尽量行驶到最终状态
	void DriveBehindCarsToEnd(int channel);
	//让车道里边面的所有等待的车都走到最终状态
	void DriveAllWaitingCarToEnd(int channelIndex);

	//-------属性--------------
	//当前调度车道,每个系统调度结束就至0,，欸此驶出车辆后会自减1
	//int currentChannelIndex;
	//道路结构体的常用属性
	int m_nRoadID;     //道路ID
	int m_nRoadIndex;  //道路下标
	int m_nLength;     //道路长度
	int m_nLimitSpeed; //最大车速
	int m_nLaneNum;    //车道数量
	int m_nStartID;    //起始点ID
	int m_nEndID;      //终点ID
	bool m_bTwoWay;    //是否双向
	float m_fWeight;      //道路的权：weight = len/speed;
	//vector的大小为4，是表示有4个车道
	//queue是每个车道里面车辆的具体信息，包括车辆的ID,与所在的位置
    //从m_nRoadCondition从0开始进入,0位置是第一辆车，出去从0位置删除
	std::vector<std::vector<CCar>> m_nRoadCondition;
	CRoad* m_ptOutLink;      //出度下一条边链指针
	CRoad* m_ptInLink;		 //入度下一条边链指针
};

