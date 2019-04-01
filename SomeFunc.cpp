#include"SomeFunc.h"
//字符串分割函数
namespace SomeFunc {

	std::vector<int> split(const  std::string& s, const std::string& delim)
	{
		std::vector<int> elems;
		size_t pos = 0;
		size_t len = s.length();
		size_t delim_len = delim.length();
		if (delim_len == 0)
			return elems;
		while (pos < len)
		{
			int find_pos = s.find(delim, pos);
			if (find_pos < 0)
			{
				elems.push_back(atoi(s.substr(pos, len - pos).c_str()));
				break;
			}
			elems.push_back(atoi(s.substr(pos, find_pos - pos).c_str()));
			pos = find_pos + delim_len;
		}
		return elems;
	}
	//车辆比较函数，升序
	bool CarSortFunc(const CCar& car1, const CCar& car2) {
		return car1.m_nCarID < car2.m_nCarID;
	}
	//路口比较函数，升序
	bool CrossSortFunc(const CCross& cross1, const CCross& cross2) {
		return cross1.m_nCrossID < cross2.m_nCrossID;
	}
	//道路比较函数，升序
	bool RoadSortFunc(const CRoad& road1, const CRoad& road2) {
		return road1.m_nRoadID < road2.m_nRoadID;
	}
}
