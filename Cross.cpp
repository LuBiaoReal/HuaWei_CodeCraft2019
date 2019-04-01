#include "Cross.h"
//��ʼ������
void CCross::Init(int crossid, int roadid1, int roadid2, int roadid3, int roadid4) {
	//������һһ��ֵ
	m_nCrossID = crossid;
	m_nCrossIndex = -1;
	m_nRoadID_1 = roadid1;
	m_nRoadID_2 = roadid2;
	m_nRoadID_3 = roadid3;
	m_nRoadID_4 = roadid4;
	//˳ʱ���ʾ·����Χ�ĵ�·���
	if(m_nRoadID_1 != -1)
		m_mapRoadOrder.insert(std::pair<int, int>(roadid1, 1));
	if (m_nRoadID_2 != -1)
		m_mapRoadOrder.insert(std::pair<int, int>(roadid2, 2));
	if (m_nRoadID_3 != -1)
		m_mapRoadOrder.insert(std::pair<int, int>(roadid3, 3));
	if (m_nRoadID_4 != -1)
		m_mapRoadOrder.insert(std::pair<int, int>(roadid4, 4));
	//������ͷָ�븳ֵδnull
	m_ptOutAdj = nullptr;
	m_ptInAdj = nullptr;
	std::vector<CCar> sameTimeCars;
	std::vector<std::vector<CCar>> garage(MAX_CAR_START_TIME, sameTimeCars);
	m_vecvecGarage = garage;
}

//���캯��
CCross::CCross(int crossid, int roadid1, int roadid2, int roadid3, int roadid4){
	//���ó�ʼ��������ʼ��
	Init(crossid, roadid1, roadid2, roadid3, roadid4);
}

//���ƹ��캯��
CCross::CCross(const CCross& cross) {
	//���ó�ʼ������
	Init(cross.m_nCrossID, cross.m_nRoadID_1, cross.m_nRoadID_2, cross.m_nRoadID_3, cross.m_nRoadID_4);
	this->m_ptOutAdj = cross.m_ptOutAdj;
	m_mapRoadOrder = cross.m_mapRoadOrder;
	m_ptInAdj = cross.m_ptInAdj;
	m_vecvecGarage = cross.m_vecvecGarage;
	m_nCrossIndex = cross.m_nCrossIndex;
}

//��ֵ���������
CCross& CCross::operator = (const CCross& cross) {
	Init(cross.m_nCrossID, cross.m_nRoadID_1, cross.m_nRoadID_2, cross.m_nRoadID_3, cross.m_nRoadID_4);
	this->m_ptOutAdj = cross.m_ptOutAdj;
	m_mapRoadOrder = cross.m_mapRoadOrder;
	m_ptInAdj = cross.m_ptInAdj;
	m_vecvecGarage = cross.m_vecvecGarage;
	m_nCrossIndex = cross.m_nCrossIndex;
	return *this;
}

//����·���еķ�����Ϣ
//����ֱ��>��ת>��ת��˳�����η���3-2-1,ʧ�ܷ���-1
TurningDirection CCross::GetDirection(int roadidstart, int roadidend) {
	if (roadidend == -1) {
		//�յ�ֱ��
		return TurningDirection::Straight;
	}
	if (roadidstart == -3) {
		std::cout << "ERR ����û�г���\n";
		exit(-1);
		return TurningDirection::Impasse;
	}
	//��ȡ��·��Ӧ�ı궨ֵ(1, 2, 3, 4)
	int value1 = m_mapRoadOrder[roadidstart];
	int value2 = m_mapRoadOrder[roadidend];
	//���ݱ궨ֵ�жϷ���
	int delta = value1 - value2;
	TurningDirection rlt = TurningDirection::Impasse;
	switch (delta)
	{
	case 2:
	case -2:
		rlt = TurningDirection::Straight;           //ֱ��
		break;
	case -1:
	case 3:
		rlt = TurningDirection::Left;           //��ת
		break;
	case 1:
	case -3:
		rlt = TurningDirection::Right;           //��ת
		break;
	default:
		break;
	}
	return rlt;
}
//������·
bool CCross::DriveFromGarage(CCar startCar ,int startTime) {
	int firstRoadID = startCar.GetNextRoadID();
	CRoad* p = m_ptOutAdj;
	while (p != nullptr)
	{
		//�ҵ��˷�����·
		if (p->m_nRoadID == firstRoadID) {	
			//�鿴·��ǰ����û�г�����
			for (auto &channel : p->m_nRoadCondition) {
				if (!channel.empty()) {
					if (channel.back().GetDistance()-1 > 0) {
						//�п�λ�Ϳ��Է�����ȥ
						//��ʻ����
						startCar.m_nRealStartTime = startTime;	//���ó���ʱ��
						int currentSpeed = std::min(startCar.m_nMaxSpeed, p->m_nLimitSpeed);
						int drivingDistance = std::min(channel.back().GetDistance() - 1, currentSpeed);
						p->DriveIn(startCar, drivingDistance, currentSpeed);
						return true;
					}	
					//���������ˣ�ȥ��һ����������
				}
				else
				{
					//����Ϊ��
					int currentSpeed = std::min(startCar.m_nMaxSpeed, p->m_nLimitSpeed);
					int drivingDistance = std::min(p->m_nLength, currentSpeed);	//�Ե�·�������Ƚ�
					p->DriveIn(startCar, drivingDistance, currentSpeed);
					return true;
				}
			}
		}
		p = p->m_ptOutLink;
	}
	//std::cout << "Road blocked��" << std::endl;
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
//�õ��˳������Ķ��泵����ID
int CCross::GetFrontRoadID(int currentRoadID) {
	int frontRoadID = -2;
	//��ǰ��·��map����
	int currentRoadMapIndex = m_mapRoadOrder[currentRoadID];
	//�õ������·��map����
	int frontRoadMapIndex = currentRoadMapIndex + 2;
	if (frontRoadMapIndex > 4) {
		frontRoadMapIndex -= 4;
	}
	//Ѱ�Ҷ����·��ID
	std::map<int, int>::iterator iter = m_mapRoadOrder.begin();
	while (iter != m_mapRoadOrder.end()) {
		if (iter->second == frontRoadMapIndex) {
			//�ҵ��˶���ĵ�·ID
			frontRoadID = iter->first;
			//�ж϶��泵���Ƿ��ǳ���
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
	//û�ҵ���˵����������·������-2
	return frontRoadID;
}

