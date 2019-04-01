#include "Road.h"
//初始化函数
void CRoad::Init(int roadid, int len, int limitspeed, int lanenum,
	int start, int end, bool istwoway/* = true*/) {
	//给属性一一赋值
	m_nRoadID = roadid;
	m_nRoadIndex = -1;
	m_nLength = len;
	m_nLimitSpeed = limitspeed;
	m_nLaneNum = lanenum;
	m_nStartID = start;
	m_nEndID = end;
	m_bTwoWay = istwoway;
	m_ptOutLink = nullptr;
	m_ptInLink = nullptr;
	m_fWeight = ((float)m_nLength) /(m_nLaneNum);
	//初始化m_nRoadCondition,之后的m_nRoadCondition就不再增加
	std::vector<CCar> val;
	std::vector<std::vector<CCar>> addLane(lanenum, val);
	m_nRoadCondition = addLane;
	std::vector<int> initWaitingChannel(lanenum, 0);
	//isConflict = false;
}

//构造函数
CRoad::CRoad(int roadid, int len, int limitspeed, int lanenum,
	int start, int end, bool istwoway/* = true*/) {
	Init(roadid, len, limitspeed, lanenum, start, end, istwoway);

}

//复制构造函数
CRoad::CRoad(const CRoad& road){
	Init(road.m_nRoadID, road.m_nLength, road.m_nLimitSpeed, road.m_nLaneNum,
		road.m_nStartID, road.m_nEndID, road.m_bTwoWay);
	m_ptOutLink = road.m_ptOutLink;
	m_ptInLink = road.m_ptInLink;
	m_fWeight = road.m_fWeight;
	m_nRoadCondition = road.m_nRoadCondition;
	m_nRoadIndex = road.m_nRoadIndex;
}

//赋值运算符重载
CRoad& CRoad::operator = (const CRoad& road) {
	Init(road.m_nRoadID, road.m_nLength, road.m_nLimitSpeed, road.m_nLaneNum,
		road.m_nStartID, road.m_nEndID, road.m_bTwoWay);
	m_ptOutLink = road.m_ptOutLink;
	m_ptInLink = road.m_ptInLink;
	m_fWeight = road.m_fWeight;
	m_nRoadCondition = road.m_nRoadCondition;
	m_nRoadIndex = road.m_nRoadIndex;
	return *this;
}

//标记所有车辆的状态
//车辆限速：V1;道路限速:V2
//1、将路口的车标记为等待状态
//2、路里边前方无阻挡可行使最大车速的车，行驶最大车速，并记作终止状态
//3、前方有终止状态的车阻挡（s<min(V1,V2)），则将该车行驶s，然后设置为终止状态
//4、前方有等待状态的车阻挡(s<min(V1,V2)),则该车设置为等待状态
void CRoad::DriveAllCarJustOnRoadToEndState(int channelIndex) {
	if (channelIndex < 0) {
		std::cout << "ERR：Get wrong ChannelIndex\n";
		exit(-1);
		return;
	}
	//0位置往后表示从路口开始
	for (int i = 0; i < m_nRoadCondition[channelIndex].size(); i++) {		
		if (i == 0) {
			//第一辆车在路口,设置为waiting状态
			if (m_nRoadCondition[channelIndex][i].GetRemianingLength() < m_nRoadCondition[channelIndex][i].GetCurrentSpeed()) {
				m_nRoadCondition[channelIndex][i].m_bIsWaiting = true;
			}
			else
			{
				//第一辆车不在路口
				//能够行驶最大速度
				m_nRoadCondition[channelIndex][i].Run(m_nRoadCondition[channelIndex][i].GetCurrentSpeed());
				m_nRoadCondition[channelIndex][i].m_bIsWaiting = false;
			}			
		}
		else
		{
			//第一辆车后面的车，以前面的车的距离和状态为依据
			int distanceToFrontCar = m_nRoadCondition[channelIndex][i - 1].GetDistance() - m_nRoadCondition[channelIndex][i].GetDistance();
			int currentSpeed = m_nRoadCondition[channelIndex][i].GetCurrentSpeed();
			if (distanceToFrontCar <= currentSpeed) {
				//不够行驶最大速度
				if (m_nRoadCondition[channelIndex][i - 1].m_bIsWaiting) {
					//前车是等待状态
					m_nRoadCondition[channelIndex][i].m_bIsWaiting = true;
					//把记录等待状态的数组值加1
				}
				else
				{
					//不是等待状态，行驶到前车的后面
					m_nRoadCondition[channelIndex][i].Run(distanceToFrontCar - 1);
					m_nRoadCondition[channelIndex][i].m_bIsWaiting = false;
				}
			}
			else
			{
				//能够行驶最大速度
				m_nRoadCondition[channelIndex][i].Run();
				m_nRoadCondition[channelIndex][i].m_bIsWaiting = false;
			}			
		}
	}
}
//进入车辆
bool CRoad::DriveIn(CCar car, const int drivingDistance, int currentSpeed) {
	//int maxSpeed = std::min(car.m_nMaxSpeed, m_nLimitSpeed);
	for (auto &channel : m_nRoadCondition) {
		//只有当前车不是
		if (!channel.empty()) {
			//剩余的车距可以驶入
			if (channel.back().GetDistance() > drivingDistance) {
				//可以进入当前车道
				if (car.ChangeRoad(m_nRoadID,m_nLength)) {
					//变道成功
					car.m_bIsWaiting = false;
					car.SetInitialDistance(drivingDistance);
					car.SetCurrentSpeed(currentSpeed);
					channel.push_back(car);
					return true;
				}
			}
			else
			{
				//剩余车距不能驶入，就去下一个车道
			}
		}
		else
		{
			//道路上是空的
			//可以进入当前车道
			if (car.ChangeRoad(m_nRoadID, m_nLength)) {
				//变道成功
				car.m_bIsWaiting = false;
				car.SetInitialDistance(drivingDistance);
				car.SetCurrentSpeed(currentSpeed);
				channel.push_back(car);
				return true;
			}
			else
			{
				//变道失败
				return false;
			}
		}
	}
	std::cout << "Car Start Failed！" << std::endl;
	//插入失败
	return false;
}
//驶出车辆,输入排和列,从0开始
CCar CRoad::DriveOut(const int carID) {
	//得到头车
	int channelIndex = 0;
	CCar *car = GetFirstPriorityCar(channelIndex);
	CCar outCar(-1, -1, -1, -1, -1);
	if (car == nullptr) {
		std::cout << "ERR Drive out Car Failed!\n";
	}
	else
	{
		outCar = *car;
		outCar.m_bIsWaiting = false;
		//移除头车
		m_nRoadCondition[channelIndex].erase(m_nRoadCondition[channelIndex].begin());
	}
	return outCar;
}

int CRoad::GetWaitingCarsNumber() {
	int sum = 0;
	for (auto &channel : m_nRoadCondition) {
		for (auto &car : channel) {
			if (car.m_bIsWaiting) {
				sum++;
			}
			else
			{
				break;
			}
		}
	}
	return sum;
}
//所有车都是终止状态
bool CRoad::AllCarsNotWaiting() {
	if(GetWaitingCarsNumber() > 0)
		return false;
	else
	{
		return true;
	}
}
CCar* CRoad::GetFirstPriorityCar(int &channelIndex) {
	//让引索从第一车道开始找
	channelIndex = 0;
	//存储每一条车道的状态
	std::vector<bool> allCarsFinished(m_nRoadCondition.size(), false);
	//从道路的入口长度位置向后遍历
	for (int i = m_nLength; i >= 0; i--) {
		//每次遍历到车道的末尾就换到下一行
		while (true)
		{
			if (!m_nRoadCondition[channelIndex].empty()) {
				//车道有车,检查头车是否为等待状态
				CCar* firstCar = &m_nRoadCondition[channelIndex][0];
				if (firstCar->m_bIsWaiting) {
					//头车是等待,检查是不是在当前排
					if (firstCar->GetDistance() == i) {
						//在当前排,返回头车,和引索						
						return firstCar;
					}
					else
					{
						//不在当前排,只移动车道引索
						if (channelIndex >= m_nRoadCondition.size() - 1) {
							channelIndex = 0;
							break;
						}
						else
						{
							channelIndex++;
						}
						
					}

				}
				else
				{
					//头车不是等待状态，更新车道的状态记录
					allCarsFinished[channelIndex] = true;
					//移动到下一条车道
					if (channelIndex >= m_nRoadCondition.size() - 1) {
						channelIndex = 0;
						break;
					}
					else
					{
						channelIndex++;
					}
				}
			}
			else
			{
				allCarsFinished[channelIndex] = true;
				//移动车道索引,并且转到下一排
				if (channelIndex >= m_nRoadCondition.size() - 1) {
					channelIndex = 0;
					break;
				}
				else
				{
					channelIndex++;
				}
			}
		}
		//每次遍历完所有的车道，检查所有车道是否都不为等待状态了,就直接返回nullptr
		bool isAllChannelFinished = true;
		for (auto status : allCarsFinished) {
			//如果有车道为等待了，就再看下一排的车
			if (status == false) {
				isAllChannelFinished = false;
				break;
			}
		}
		//若是所有车道都是完成状态，就返回空
		if (isAllChannelFinished) {
			return nullptr;
		}
	}
	return nullptr;
}
bool CRoad::CheckExitCondition(int & maxRunningDistance) {
	//检查前面是否有车阻拦
	for (auto& channel : m_nRoadCondition) {
		if (channel.empty()) {
			maxRunningDistance = std::min(maxRunningDistance, m_nLength);
			return true;
		}
		else
		{
			//检查当前车道有没有被沾满
			int frontCarRunningDistance = channel.back().GetDistance();
			//车道没有被占满
			if (frontCarRunningDistance != 1) {				
				//计算与前车的距离
				int distanceToFrontCar = frontCarRunningDistance - maxRunningDistance-1;
				if (distanceToFrontCar > 0) {
					//前面没有车阻拦
					return true;
				}
				else
				{
					//前面有车阻拦,且为等待状态的车
					if (channel.back().m_bIsWaiting) {
						maxRunningDistance = 0;
						return false;
					}
					else
					{
						//前面的车是终止状态的车,就跟在前面车的后面
						maxRunningDistance = frontCarRunningDistance - 1;
						return true;
					}
				}
			}
			else
			{
				//车道占满了,检查最后一辆车是不是等待状态
				if (channel.back().m_bIsWaiting) {
					maxRunningDistance = 0;
					return false;
				}
				else
				{
					//最后一辆车不是等待状态，就检查下一个车道
				}
				
			}

		}
	}
	//当整个车道都塞满了
	maxRunningDistance = 0;
	return true;
}

void CRoad::DriveBehindCarsToEnd(int channel) {
	if (!m_nRoadCondition[channel].empty() && !m_nRoadCondition[channel][0].m_bIsWaiting) {
		for (int i = 1; i < m_nRoadCondition[channel].size(); i++) {
			CCar* car = &m_nRoadCondition[channel][i];
			//查看与前车的状态			
			if (car->m_bIsWaiting) {
				//本车是等待
				car->m_bIsWaiting = false;
				int currentSpeed = car->GetCurrentSpeed();
				int distanceToFrontCar = m_nRoadCondition[channel][i-1].GetDistance() - car->GetDistance();
				if (distanceToFrontCar <= currentSpeed) {
					//与前车的距离不够走最大速度
					car->Run(distanceToFrontCar - 1);
				}
				else
				{
					//可以走最大速度
					car->Run();
				}
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		std::cout << "ERR Dirving Wrong car\n";
	}

}
//让车道里边面的所有等待的车都走到最终状态
void CRoad::DriveAllWaitingCarToEnd(int channelIndex) {
	for (int i = 0; i < m_nRoadCondition[channelIndex].size(); i++) {		
		CCar &car = m_nRoadCondition[channelIndex][i];
		if (car.m_bIsWaiting) {
			//车是等待状态
			if (i == 0) {
				//头车
				if (car.GetRemianingLength() < car.GetCurrentSpeed()) {
					//要出路口，就等待下一次调度
					return;
				}
				else
				{
					//不出路口，走到行走最大速度
					car.m_bIsWaiting = false;
					car.Run();
				}
			}
			else
			{
				//后面的车,判断车距来行走		
				car.m_bIsWaiting = false;
				int distanceToFrontCar = m_nRoadCondition[channelIndex][i - 1].GetDistance() - car.GetDistance();				
				if (distanceToFrontCar > car.GetCurrentSpeed()) {
					//够走最大速度
					car.Run();					
				}
				else
				{
					//走到前车后面跟着
					car.Run(distanceToFrontCar - 1);
				}
			}
		}
		else
		{
			//终止状态的车直接返回
			return;
		}
	}
}

