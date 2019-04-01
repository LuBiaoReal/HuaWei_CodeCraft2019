#include "Car.h"
//��ʼ������
void CCar::Init(int carid, int origin, int dest, int maxspeed, int gotime) {
	//�����Ը�ֵ
	m_nCarID = carid;
	m_nOrigin = origin;
	m_nDestination = dest;
	m_nMaxSpeed = maxspeed;
	m_nStartTime = gotime;
	m_nRealStartTime = m_nStartTime;
	m_nCurSpeed = 0;
	m_nDistance = 0;
	m_bIsWaiting = false;		
	//isfinished = false ;	
	m_nCurRoadIndex = -1;
	m_nCurRoadLength = 0;
}
//���캯��
CCar::CCar(int carid, int origin, int dest, int maxspeed, int gotime) {
	Init(carid, origin, dest, maxspeed, gotime);
}

//���ƹ��캯��
CCar::CCar(const CCar& car) {
	Init(car.m_nCarID, car.m_nOrigin, car.m_nDestination, car.m_nMaxSpeed, car.m_nStartTime);
	this->m_nCurSpeed = car.m_nCurSpeed;
	m_nRealStartTime = car.m_nRealStartTime;
	m_nCurSpeed = car.m_nCurSpeed;
	m_bIsWaiting = car.m_bIsWaiting;
	m_nDistance = car.m_nDistance;
	m_nCurRoadIndex = car.m_nCurRoadIndex;
	m_vecShortestPath = car.m_vecShortestPath;
	m_nCurRoadLength = car.m_nCurRoadLength;
}

//��ֵ���������
CCar& CCar::operator = (const CCar& car) {
	Init(car.m_nCarID, car.m_nOrigin, car.m_nDestination, car.m_nMaxSpeed, car.m_nStartTime);
	this->m_nCurSpeed = car.m_nCurSpeed;
	m_nRealStartTime = car.m_nRealStartTime;
	m_nCurSpeed = car.m_nCurSpeed;
	m_bIsWaiting = car.m_bIsWaiting;
	m_nDistance = car.m_nDistance;
	m_nCurRoadIndex = car.m_nCurRoadIndex;
	m_vecShortestPath = car.m_vecShortestPath;
	m_nCurRoadLength = car.m_nCurRoadLength;
	return *this;
}


