#pragma once
#include<string>
#include"Car.h"
#include"Cross.h"
#include"Road.h"
#include<vector>
namespace SomeFunc {
	std::vector<int> split(const  std::string& s, const std::string& delim);   //�ַ����ָ���	
	//��car���бȽϺ���
	bool CarSortFunc(const CCar& car1, const CCar& car2);
	bool CrossSortFunc(const CCross& cross1, const CCross& cross2);
	bool RoadSortFunc(const CRoad& road1, const CRoad& road2);
}
