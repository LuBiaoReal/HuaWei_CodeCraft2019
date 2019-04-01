#include "Road.h"
//��ʼ������
void CRoad::Init(int roadid, int len, int limitspeed, int lanenum,
	int start, int end, bool istwoway/* = true*/) {
	//������һһ��ֵ
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
	//��ʼ��m_nRoadCondition,֮���m_nRoadCondition�Ͳ�������
	std::vector<CCar> val;
	std::vector<std::vector<CCar>> addLane(lanenum, val);
	m_nRoadCondition = addLane;
	std::vector<int> initWaitingChannel(lanenum, 0);
	//isConflict = false;
}

//���캯��
CRoad::CRoad(int roadid, int len, int limitspeed, int lanenum,
	int start, int end, bool istwoway/* = true*/) {
	Init(roadid, len, limitspeed, lanenum, start, end, istwoway);

}

//���ƹ��캯��
CRoad::CRoad(const CRoad& road){
	Init(road.m_nRoadID, road.m_nLength, road.m_nLimitSpeed, road.m_nLaneNum,
		road.m_nStartID, road.m_nEndID, road.m_bTwoWay);
	m_ptOutLink = road.m_ptOutLink;
	m_ptInLink = road.m_ptInLink;
	m_fWeight = road.m_fWeight;
	m_nRoadCondition = road.m_nRoadCondition;
	m_nRoadIndex = road.m_nRoadIndex;
}

//��ֵ���������
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

//������г�����״̬
//�������٣�V1;��·����:V2
//1����·�ڵĳ����Ϊ�ȴ�״̬
//2��·���ǰ�����赲����ʹ����ٵĳ�����ʻ����٣���������ֹ״̬
//3��ǰ������ֹ״̬�ĳ��赲��s<min(V1,V2)�����򽫸ó���ʻs��Ȼ������Ϊ��ֹ״̬
//4��ǰ���еȴ�״̬�ĳ��赲(s<min(V1,V2)),��ó�����Ϊ�ȴ�״̬
void CRoad::DriveAllCarJustOnRoadToEndState(int channelIndex) {
	if (channelIndex < 0) {
		std::cout << "ERR��Get wrong ChannelIndex\n";
		exit(-1);
		return;
	}
	//0λ�������ʾ��·�ڿ�ʼ
	for (int i = 0; i < m_nRoadCondition[channelIndex].size(); i++) {		
		if (i == 0) {
			//��һ������·��,����Ϊwaiting״̬
			if (m_nRoadCondition[channelIndex][i].GetRemianingLength() < m_nRoadCondition[channelIndex][i].GetCurrentSpeed()) {
				m_nRoadCondition[channelIndex][i].m_bIsWaiting = true;
			}
			else
			{
				//��һ��������·��
				//�ܹ���ʻ����ٶ�
				m_nRoadCondition[channelIndex][i].Run(m_nRoadCondition[channelIndex][i].GetCurrentSpeed());
				m_nRoadCondition[channelIndex][i].m_bIsWaiting = false;
			}			
		}
		else
		{
			//��һ��������ĳ�����ǰ��ĳ��ľ����״̬Ϊ����
			int distanceToFrontCar = m_nRoadCondition[channelIndex][i - 1].GetDistance() - m_nRoadCondition[channelIndex][i].GetDistance();
			int currentSpeed = m_nRoadCondition[channelIndex][i].GetCurrentSpeed();
			if (distanceToFrontCar <= currentSpeed) {
				//������ʻ����ٶ�
				if (m_nRoadCondition[channelIndex][i - 1].m_bIsWaiting) {
					//ǰ���ǵȴ�״̬
					m_nRoadCondition[channelIndex][i].m_bIsWaiting = true;
					//�Ѽ�¼�ȴ�״̬������ֵ��1
				}
				else
				{
					//���ǵȴ�״̬����ʻ��ǰ���ĺ���
					m_nRoadCondition[channelIndex][i].Run(distanceToFrontCar - 1);
					m_nRoadCondition[channelIndex][i].m_bIsWaiting = false;
				}
			}
			else
			{
				//�ܹ���ʻ����ٶ�
				m_nRoadCondition[channelIndex][i].Run();
				m_nRoadCondition[channelIndex][i].m_bIsWaiting = false;
			}			
		}
	}
}
//���복��
bool CRoad::DriveIn(CCar car, const int drivingDistance, int currentSpeed) {
	//int maxSpeed = std::min(car.m_nMaxSpeed, m_nLimitSpeed);
	for (auto &channel : m_nRoadCondition) {
		//ֻ�е�ǰ������
		if (!channel.empty()) {
			//ʣ��ĳ������ʻ��
			if (channel.back().GetDistance() > drivingDistance) {
				//���Խ��뵱ǰ����
				if (car.ChangeRoad(m_nRoadID,m_nLength)) {
					//����ɹ�
					car.m_bIsWaiting = false;
					car.SetInitialDistance(drivingDistance);
					car.SetCurrentSpeed(currentSpeed);
					channel.push_back(car);
					return true;
				}
			}
			else
			{
				//ʣ�೵�಻��ʻ�룬��ȥ��һ������
			}
		}
		else
		{
			//��·���ǿյ�
			//���Խ��뵱ǰ����
			if (car.ChangeRoad(m_nRoadID, m_nLength)) {
				//����ɹ�
				car.m_bIsWaiting = false;
				car.SetInitialDistance(drivingDistance);
				car.SetCurrentSpeed(currentSpeed);
				channel.push_back(car);
				return true;
			}
			else
			{
				//���ʧ��
				return false;
			}
		}
	}
	std::cout << "Car Start Failed��" << std::endl;
	//����ʧ��
	return false;
}
//ʻ������,�����ź���,��0��ʼ
CCar CRoad::DriveOut(const int carID) {
	//�õ�ͷ��
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
		//�Ƴ�ͷ��
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
//���г�������ֹ״̬
bool CRoad::AllCarsNotWaiting() {
	if(GetWaitingCarsNumber() > 0)
		return false;
	else
	{
		return true;
	}
}
CCar* CRoad::GetFirstPriorityCar(int &channelIndex) {
	//�������ӵ�һ������ʼ��
	channelIndex = 0;
	//�洢ÿһ��������״̬
	std::vector<bool> allCarsFinished(m_nRoadCondition.size(), false);
	//�ӵ�·����ڳ���λ��������
	for (int i = m_nLength; i >= 0; i--) {
		//ÿ�α�����������ĩβ�ͻ�����һ��
		while (true)
		{
			if (!m_nRoadCondition[channelIndex].empty()) {
				//�����г�,���ͷ���Ƿ�Ϊ�ȴ�״̬
				CCar* firstCar = &m_nRoadCondition[channelIndex][0];
				if (firstCar->m_bIsWaiting) {
					//ͷ���ǵȴ�,����ǲ����ڵ�ǰ��
					if (firstCar->GetDistance() == i) {
						//�ڵ�ǰ��,����ͷ��,������						
						return firstCar;
					}
					else
					{
						//���ڵ�ǰ��,ֻ�ƶ���������
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
					//ͷ�����ǵȴ�״̬�����³�����״̬��¼
					allCarsFinished[channelIndex] = true;
					//�ƶ�����һ������
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
				//�ƶ���������,����ת����һ��
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
		//ÿ�α��������еĳ�����������г����Ƿ񶼲�Ϊ�ȴ�״̬��,��ֱ�ӷ���nullptr
		bool isAllChannelFinished = true;
		for (auto status : allCarsFinished) {
			//����г���Ϊ�ȴ��ˣ����ٿ���һ�ŵĳ�
			if (status == false) {
				isAllChannelFinished = false;
				break;
			}
		}
		//�������г����������״̬���ͷ��ؿ�
		if (isAllChannelFinished) {
			return nullptr;
		}
	}
	return nullptr;
}
bool CRoad::CheckExitCondition(int & maxRunningDistance) {
	//���ǰ���Ƿ��г�����
	for (auto& channel : m_nRoadCondition) {
		if (channel.empty()) {
			maxRunningDistance = std::min(maxRunningDistance, m_nLength);
			return true;
		}
		else
		{
			//��鵱ǰ������û�б�մ��
			int frontCarRunningDistance = channel.back().GetDistance();
			//����û�б�ռ��
			if (frontCarRunningDistance != 1) {				
				//������ǰ���ľ���
				int distanceToFrontCar = frontCarRunningDistance - maxRunningDistance-1;
				if (distanceToFrontCar > 0) {
					//ǰ��û�г�����
					return true;
				}
				else
				{
					//ǰ���г�����,��Ϊ�ȴ�״̬�ĳ�
					if (channel.back().m_bIsWaiting) {
						maxRunningDistance = 0;
						return false;
					}
					else
					{
						//ǰ��ĳ�����ֹ״̬�ĳ�,�͸���ǰ�泵�ĺ���
						maxRunningDistance = frontCarRunningDistance - 1;
						return true;
					}
				}
			}
			else
			{
				//����ռ����,������һ�����ǲ��ǵȴ�״̬
				if (channel.back().m_bIsWaiting) {
					maxRunningDistance = 0;
					return false;
				}
				else
				{
					//���һ�������ǵȴ�״̬���ͼ����һ������
				}
				
			}

		}
	}
	//������������������
	maxRunningDistance = 0;
	return true;
}

void CRoad::DriveBehindCarsToEnd(int channel) {
	if (!m_nRoadCondition[channel].empty() && !m_nRoadCondition[channel][0].m_bIsWaiting) {
		for (int i = 1; i < m_nRoadCondition[channel].size(); i++) {
			CCar* car = &m_nRoadCondition[channel][i];
			//�鿴��ǰ����״̬			
			if (car->m_bIsWaiting) {
				//�����ǵȴ�
				car->m_bIsWaiting = false;
				int currentSpeed = car->GetCurrentSpeed();
				int distanceToFrontCar = m_nRoadCondition[channel][i-1].GetDistance() - car->GetDistance();
				if (distanceToFrontCar <= currentSpeed) {
					//��ǰ���ľ��벻��������ٶ�
					car->Run(distanceToFrontCar - 1);
				}
				else
				{
					//����������ٶ�
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
//�ó������������еȴ��ĳ����ߵ�����״̬
void CRoad::DriveAllWaitingCarToEnd(int channelIndex) {
	for (int i = 0; i < m_nRoadCondition[channelIndex].size(); i++) {		
		CCar &car = m_nRoadCondition[channelIndex][i];
		if (car.m_bIsWaiting) {
			//���ǵȴ�״̬
			if (i == 0) {
				//ͷ��
				if (car.GetRemianingLength() < car.GetCurrentSpeed()) {
					//Ҫ��·�ڣ��͵ȴ���һ�ε���
					return;
				}
				else
				{
					//����·�ڣ��ߵ���������ٶ�
					car.m_bIsWaiting = false;
					car.Run();
				}
			}
			else
			{
				//����ĳ�,�жϳ���������		
				car.m_bIsWaiting = false;
				int distanceToFrontCar = m_nRoadCondition[channelIndex][i - 1].GetDistance() - car.GetDistance();				
				if (distanceToFrontCar > car.GetCurrentSpeed()) {
					//��������ٶ�
					car.Run();					
				}
				else
				{
					//�ߵ�ǰ���������
					car.Run(distanceToFrontCar - 1);
				}
			}
		}
		else
		{
			//��ֹ״̬�ĳ�ֱ�ӷ���
			return;
		}
	}
}

