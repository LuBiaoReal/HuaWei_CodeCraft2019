#include "Cross.h"
//初始化函数
void CCross::Init(int crossid, int roadid1, int roadid2, int roadid3, int roadid4) {
	//给属性一一赋值
	m_nCrossID = crossid;
	m_nCrossIndex = -1;
	m_nRoadID_1 = roadid1;
	m_nRoadID_2 = roadid2;
	m_nRoadID_3 = roadid3;
	m_nRoadID_4 = roadid4;
	//顺时针表示路口周围的道路情况
	if(m_nRoadID_1 != -1)
		m_mapRoadOrder.insert(std::pair<int, int>(roadid1, 1));
	if (m_nRoadID_2 != -1)
		m_mapRoadOrder.insert(std::pair<int, int>(roadid2, 2));
	if (m_nRoadID_3 != -1)
		m_mapRoadOrder.insert(std::pair<int, int>(roadid3, 3));
	if (m_nRoadID_4 != -1)
		m_mapRoadOrder.insert(std::pair<int, int>(roadid4, 4));
	//边链表头指针赋值未null
	m_ptOutAdj = nullptr;
	m_ptInAdj = nullptr;
	std::vector<CCar> sameTimeCars;
	std::vector<std::vector<CCar>> garage(MAX_CAR_START_TIME, sameTimeCars);
	m_vecvecGarage = garage;
}

//构造函数
CCross::CCross(int crossid, int roadid1, int roadid2, int roadid3, int roadid4){
	//调用初始化函数初始化
	Init(crossid, roadid1, roadid2, roadid3, roadid4);
}

//复制构造函数
CCross::CCross(const CCross& cross) {
	//调用初始化函数
	Init(cross.m_nCrossID, cross.m_nRoadID_1, cross.m_nRoadID_2, cross.m_nRoadID_3, cross.m_nRoadID_4);
	this->m_ptOutAdj = cross.m_ptOutAdj;
	m_mapRoadOrder = cross.m_mapRoadOrder;
	m_ptInAdj = cross.m_ptInAdj;
	m_vecvecGarage = cross.m_vecvecGarage;
	m_nCrossIndex = cross.m_nCrossIndex;
}

//赋值运算符重载
CCross& CCross::operator = (const CCross& cross) {
	Init(cross.m_nCrossID, cross.m_nRoadID_1, cross.m_nRoadID_2, cross.m_nRoadID_3, cross.m_nRoadID_4);
	this->m_ptOutAdj = cross.m_ptOutAdj;
	m_mapRoadOrder = cross.m_mapRoadOrder;
	m_ptInAdj = cross.m_ptInAdj;
	m_vecvecGarage = cross.m_vecvecGarage;
	m_nCrossIndex = cross.m_nCrossIndex;
	return *this;
}

//返回路口中的方向信息
//根据直行>左转>右转的顺序，依次返回3-2-1,失败返回-1
TurningDirection CCross::GetDirection(int roadidstart, int roadidend) {
	if (roadidend == -1) {
		//终点直行
		return TurningDirection::Straight;
	}
	if (roadidstart == -3) {
		std::cout << "ERR 车还没有出发\n";
		exit(-1);
		return TurningDirection::Impasse;
	}
	//获取道路对应的标定值(1, 2, 3, 4)
	int value1 = m_mapRoadOrder[roadidstart];
	int value2 = m_mapRoadOrder[roadidend];
	//根据标定值判断方向
	int delta = value1 - value2;
	TurningDirection rlt = TurningDirection::Impasse;
	switch (delta)
	{
	case 2:
	case -2:
		rlt = TurningDirection::Straight;           //直行
		break;
	case -1:
	case 3:
		rlt = TurningDirection::Left;           //左转
		break;
	case 1:
	case -3:
		rlt = TurningDirection::Right;           //右转
		break;
	default:
		break;
	}
	return rlt;
}
//开车上路
bool CCross::DriveFromGarage(CCar startCar ,int startTime) {
	int firstRoadID = startCar.GetNextRoadID();
	CRoad* p = m_ptOutAdj;
	while (p != nullptr)
	{
		//找到了发车道路
		if (p->m_nRoadID == firstRoadID) {	
			//查看路的前面有没有车阻拦
			for (auto &channel : p->m_nRoadCondition) {
				if (!channel.empty()) {
					if (channel.back().GetDistance()-1 > 0) {
						//有空位就可以发车出去
						//行驶距离
						startCar.m_nRealStartTime = startTime;	//设置出发时间
						int currentSpeed = std::min(startCar.m_nMaxSpeed, p->m_nLimitSpeed);
						int drivingDistance = std::min(channel.back().GetDistance() - 1, currentSpeed);
						p->DriveIn(startCar, drivingDistance, currentSpeed);
						return true;
					}	
					//车道塞满了，去下一条车道看看
				}
				else
				{
					//车道为空
					int currentSpeed = std::min(startCar.m_nMaxSpeed, p->m_nLimitSpeed);
					int drivingDistance = std::min(p->m_nLength, currentSpeed);	//以道路长度作比较
					p->DriveIn(startCar, drivingDistance, currentSpeed);
					return true;
				}
			}
		}
		p = p->m_ptOutLink;
	}
	//std::cout << "Road blocked！" << std::endl;
	return false;
}
int CCross::GetRuningCarNumber() {
	CRoad* p = m_ptInAdj;
	int sum = 0;
	while (p != nullptr)
	{
		for (auto &channel: p->m_nRoadCondition) {
			sum += channel.size();
		}
		p = p->m_ptInLink;
	}
	return sum;
}
CRoad* CCross::GetExitRoad(int roadID) {
	CRoad* p = m_ptOutAdj;
	while (p != nullptr) {
		if (p->m_nRoadID == roadID)
			return p;
		p = p->m_ptOutLink;
	}
	return nullptr;
}
int CCross::GetWaitingNumber() {
	CRoad* p = m_ptInAdj;	
	int sumWaitingCars = 0;
	while (p != nullptr) {
		sumWaitingCars += p->GetWaitingCarsNumber();
		p = p->m_ptInLink;
	}
	return sumWaitingCars;
}
//得到此车车道的对面车道的ID
int CCross::GetFrontRoadID(int currentRoadID) {
	int frontRoadID = -2;
	//当前道路的map引索
	int currentRoadMapIndex = m_mapRoadOrder[currentRoadID];
	//得到对面道路的map引索
	int frontRoadMapIndex = currentRoadMapIndex + 2;
	if (frontRoadMapIndex > 4) {
		frontRoadMapIndex -= 4;
	}
	//寻找对面道路的ID
	std::map<int, int>::iterator iter = m_mapRoadOrder.begin();
	while (iter != m_mapRoadOrder.end()) {
		if (iter->second == frontRoadMapIndex) {
			//找到了对面的道路ID
			frontRoadID = iter->first;
			//判断对面车道是否是出口
			CRoad *outRoad = m_ptOutAdj;
			while (outRoad != nullptr)
			{
				if (outRoad->m_nRoadID == frontRoadID) {
					return frontRoadID;
				}
				outRoad = outRoad->m_ptOutLink;
			}
			return -2;
		}
		iter++;
	}
	//没找到，说明对面是死路，返回-2
	return frontRoadID;
}

