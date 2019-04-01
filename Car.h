#pragma once
#include <algorithm> 
#include <vector>
#include<iostream>
class CCar
{
public:
	//初始化函数
	void Init(int carid, int origin, int dest, int maxspeed, int gotime);
	//构造函数
	CCar(int carid, int origin, int dest, int maxspeed, int gotime);
	//复制构造函数
	CCar(const CCar& car);
	//赋值运算符重载
	CCar& operator = (const CCar& car);
	
	//----用到的函数---
	//驾驶车，在单位时间走过的一段距离
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

	//得到在当前的路上走了多远
	int GetDistance() {
		return m_nDistance;
	}

	//得到车的当前速度,该速度时道路和车辆最大速度的较小值
	int GetCurrentSpeed() {
		return m_nCurSpeed;
	}

	//得到下一条要走道路的id,为-1则表示走完了,直接进入终点
	int GetNextRoadID() {
		if (m_nCurRoadIndex == m_vecShortestPath.size() - 1) {
			return -1;
		}
		else
		{
			return m_vecShortestPath[m_nCurRoadIndex + 1];
		}
	}

	//得到当前行走的道路ID
	int GetCurrentRoadID() {
		//还没有出发
		if (m_nCurRoadIndex == -1) {
			return -3;
		}
		else
		{
			return m_vecShortestPath[m_nCurRoadIndex];
		}
	}

	//得到在当前道路的剩余距离。当前道路长度：在变道的时候设置为道路长度
	int GetRemianingLength() {
		return m_nCurRoadLength - GetDistance();
	}

	//变换道路的引索，通过返回值确定是否变道成功
	bool ChangeRoad(int ChangedID, int roadLength) {
		if (m_nCurRoadIndex != m_vecShortestPath.size() - 1 && ChangedID == m_vecShortestPath[m_nCurRoadIndex + 1]) {
			m_nCurRoadIndex++;
			m_nCurRoadLength = roadLength;
			return true;
		}
		else
		{
			std::cout << ChangedID << "变道失败" << std::endl;
			exit(-1);
			return false;
		}
	}

	//进路口后，设置行驶速度
	void SetCurrentSpeed(const int speed) {
		m_nCurSpeed = speed;
	}

	//进路口后，设置行驶距离
	void SetInitialDistance(const int distance) {
		m_nDistance = distance;
	}

	//--------属性值---------
	//存放车辆所有的路径
	std::vector<int> m_vecShortestPath;		
	bool m_bIsWaiting;	       //车辆行驶状态，ture就在等待，否则就是正常运行
	int m_nStartTime;         //出发时间				  
	int m_nCarID;          //汽车ID
	int m_nOrigin;         //始发地
	int m_nDestination;    //目的地
	int m_nMaxSpeed;       //最高时速
	//车辆运行参数
	int m_nRealStartTime;	//实际出发时间
	int m_nCurSpeed;		//行驶的实际时间
	int m_nDistance;		//在道路上行驶的距离
	//车辆状态
	//bool isfinished;		//车辆是否行驶完成一个时间单位
	//跑出来的初步路径
	int m_nCurRoadIndex;    //记录当前行驶的道路ID下标
	int m_nCurRoadLength;	//记录当前行驶的道路长度
};

