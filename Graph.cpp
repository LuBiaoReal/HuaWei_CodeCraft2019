#include"Graph.h"
#include<ctime>
#include<sstream>
#include <cstdlib>
std::string GraphLink::getMapName(std::string mapPath) {
	std::string::size_type pos;
	std::vector<std::string> resultList;
	std::string pattern = "/";
	mapPath += pattern;
	int size = mapPath.size();
	for (int i = 0; i < size; i++) {
		pos = mapPath.find(pattern, i);
		if ((int)pos < size) {
			std::string s = mapPath.substr(i, (int)pos - i);
			resultList.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	//到数第二个字符就是文件夹的名字
	std::string mapName = "";
	if (resultList.size() > 2) {
		mapName = resultList[resultList.size() - 2];
	}
	
	return mapName;
}
int GraphLink::checkWhichMap(std::string mapName) {
	int count1 = 0;
	int count2 = 0;
	int size = mapName.size();
	for (int i = 0; i < size; i++) {
		//把字符转数字
		int num = 0;
		num = atoi(mapName.substr(i,1).c_str());
		//std::stringstream ss(mapName[i]);
		//ss >> num;
		//std::cout << num << std::endl;
		if (num == 1) {
			count1++;
		}
		if (num == 2) {
			count2++;
		}
		//int num = atoi()
	}
	//数字出现最少的就是
	if (count1 > count2) {
		return 1;
	}
	else if(count1 < count2)
	{
		return 2;
	}
	else
	{
		return 0;
	}
}
GraphLink::GraphLink(std::string &roadPath, std::string &crossPath, std::string &carPath, std::string &resultPath) {
	//读取car文件之前，先初始化一下m_vecvecCar一下
	//出发时间从1开始，而vector的下标从0开始，所以用的时候要注意减1
	//用这种方法装，同一出发时间的车辆ID也排好了序
	//for (int i = 0; i < 100; i++) {
	//	std::vector<CCar>vecCar;
	//	m_vecvecCar.push_back(vecCar);
	//}
	//std::cout << "........" << std::endl;
	this->m_strResultPath = resultPath;
	//读取基础文件
	//对不同的地图设置不同的参数
	//int mapNum = checkWhichMap(getMapName(carPath));
	//if (mapNum == 1) {
	//	//地图1
	//	CAR_NUM = 2600;
	//	//std::cout << "地图1" << std::endl;
	//}
	//else if (mapNum == 2)
	//{
	//	//地图2
	//	CAR_NUM = 1250;
	//	//std::cout << "地图2" << std::endl;
	//}
	//else
	//{
	//	//没找到图的名字
	//	CAR_NUM = 1200;
	//	//std::cout << "地图名字没找到" << std::endl;
	//}
	//----随机数-----
	srand((unsigned)time(NULL));
	int seed = rand() % 100;
	if (seed > 50) {
		seed = 0;
	}
	else
	{
		seed = 1;
	}
	if (seed == 0) {
		//取得地图1的参数
		CAR_NUM = 2600;
	}
	std::cout << "seed:" << seed << std::endl;
	if (seed == 1) {
		//0表示地图2
		CAR_NUM = 1550;
	}
	else
	{
		//1表示图1
		CAR_NUM = 3150;
	}
	ReadFile(crossPath, "Cross");

	//初始化节点列表
	m_nMaxVertices = m_vecCross.size();               //最大顶点数为路口数
	m_nNumVertices = 0;
	m_nNumEdges = 0;
	m_ptNodeTable = new CCross[m_nMaxVertices];       //建立顶点表数组
	if (m_ptNodeTable == nullptr) {
		std::cerr << "Memory allocation ERR！\n";
		exit(-1);
	}
	

	//在图中插入顶点
	for (int i = 0; i < m_vecCross.size(); i++) {
		InsertVertex(m_vecCross[i]);
	}
	//在遍历边的时候直接将数据放到节点中去
	ReadFile(roadPath, "Road");
	//在图中插入边
	std::vector<CRoad>::iterator iter_road = m_vecRoad.begin();    //指向road的迭代器
	while (iter_road != m_vecRoad.end()) {
		InsertEdge(*iter_road);
		iter_road++;
	}
	ReadFile(carPath, "Car");
    //调用弗洛伊德求出所有点之间的最短路径
	Floyd();
}

//-----读取数据文件------
void GraphLink::ReadFile(const std::string path, const std::string type) {
	std::ifstream fin;
	fin.open(path);
	if (fin.fail()) {
		std::cout << "Open "<<type<<" Filed！\n";
		exit(-1);
	}

	//---分割文件内容---
	std::string strLine;
	std::vector<int> vecLine;
	//计数，为车辆，路口，道路等添加下标
	int carCount = -1;
	int crossCount = -1;
	int roadCount = -1;
	//去掉第一行的注释
	getline(fin, strLine);
	//按行读取,分割数据
	while (getline(fin, strLine)) {
		strLine = strLine.substr(1, strLine.length() - 2);
		vecLine = SomeFunc::split(strLine,",");
		//---判断文件类型---
		int dataSize = vecLine.size();
		if (type == "Car" && dataSize == 5) {
			//std::cout << "Car:" << vecLine[0] << std::endl;
			CCar car(vecLine[0], vecLine[1], vecLine[2], vecLine[3], vecLine[4]);
			
			//给车辆下标赋初值
			/*carCount++;
			car.m_nCarIndex = carCount;*/
			//车辆入栈
			m_vecCar.push_back(car);
			//m_vecvecCar[vecLine[4] - 1].push_back(car);
		}
		else if (type == "Cross" && dataSize == 5) {
			//std::cout << "Cross:" << vecLine[0] << std::endl;
			CCross cross(vecLine[0], vecLine[1], vecLine[2], vecLine[3], vecLine[4]);
			//给道路下标赋初值
			/*crossCount++;
			cross.m_nCrossIndex = crossCount;*/
			//路口入栈
			m_vecCross.push_back(cross);
		}
		else if (type == "Road" && dataSize == 7) {
			//roadID, roadDistance, maxSpeed, channleNumber, startCrossID, endCrossID, isTwoWay
			CRoad road(vecLine[0], vecLine[1], vecLine[2], vecLine[3], vecLine[4], vecLine[5], vecLine[6]);
			//道路下标入栈
			/*roadCount++;
			road.m_nRoadIndex = roadCount;*/
	        //道路入栈
			m_vecRoad.push_back(road);
		}
		else {
			fin.close();
			std::cout << "read failed!" << std::endl;
			exit(-1);
		}
	}

	//读取完文件信息之后将文件信息排序
	if (type == "Car") {
		//对车辆进行排序
		std::sort(m_vecCar.begin(), m_vecCar.end(), SomeFunc::CarSortFunc);

		//排好序之后，将车装入神奇车库
		for (int i = 0; i < m_vecCar.size(); i++) {
			CCar car = m_vecCar[i];
			int startID = car.m_nOrigin;
			int startTime = car.m_nStartTime;
			CCross* ptCross = GetCrossById(startID);
			ptCross->m_vecvecGarage[startTime - 1].push_back(car);
		}
	}
		
	else if (type == "Cross") {
		//对cross根据ID进行排序
		std::sort(m_vecCross.begin(), m_vecCross.end(), SomeFunc::CrossSortFunc);
		//将给个路口的下标属性初始化
		for (int i = 0; i < m_vecCross.size(); i++) {
			m_vecCross[i].m_nCrossIndex = i;
		}
	}
	else {
		//对道路根据其ID大小进行排序
		std::sort(m_vecRoad.begin(), m_vecRoad.end(), SomeFunc::RoadSortFunc);
		//将每个道路的下标属性进行初始化
		for (int i = 0; i < m_vecRoad.size(); i++)
			m_vecRoad[i].m_nRoadIndex = i;
	}



	//关闭文件
	fin.close();
}

//-------写入结果文件----------
void GraphLink::WriteFile(const std::string path, const std::vector<Result> &result) {
	std::ofstream fout;
	fout.open(path);
	if (fout.fail()) {
		std::cout << "Open " << path << " Failed！\n";
		exit(-1);
	}

	//写入结果
	//fout << "#(carId,StartTime,RoadId...)\n";
	for (int i = 0; i < result.size(); i++) {
		fout << "(" << result[i].m_nCarId << ","<< result[i].m_nStartTime;
		
		for (int j = 0; j < result[i].m_vecVisitedPath.size(); j++) {
			fout << "," << result[i].m_vecVisitedPath[j];
		}
		fout << ")\n";
	}
	fout.close();
}

//析构函数，删除一个邻接表
GraphLink::~GraphLink() {
	for (int i = 0; i < m_nMaxVertices; i++) {      //删除各链接表中的结点
		CRoad* p = m_ptNodeTable[i].m_ptOutAdj;          //找到其对应的边链表的首结点
		//CRoad* q = m_ptNodeTable[i].m_ptInAdj;          //找到其对应的边链表的首结点
		while (p != nullptr) {
			m_ptNodeTable[i].m_ptOutAdj = p->m_ptOutLink;
			delete p;
			p = m_ptNodeTable[i].m_ptOutAdj;
		}

		/*while (q != nullptr) {
			m_ptNodeTable[i].m_ptOutAdj = q->m_ptOutLink;
			delete q;
			q = m_ptNodeTable[i].m_ptOutAdj;
		}*/
	}
	delete[] m_ptNodeTable;                         //删除顶点表数组
}


//取位置为i的顶点中的值
CCross* GraphLink::GetCrossByIndex(const int index) {
	//CCross reverseRoad;            //临时路口变量用于返回
	//return (index >= 0 && index < m_nNumVertices) ? m_ptNodeTable[index] : reverseRoad;
	return &m_ptNodeTable[index];
}



//返回边(v1,v2)上的权值，其中v1,v2是图中以0下标开始的标号
int GraphLink::GetWeight(int v1, int v2) {
	if (v1 == v2)                                 //如果v1 == v2说明起点和终点相同，返回0
		return 0;
	if (v1 != -1 && v2 != -1) {
		CRoad* p = m_ptNodeTable[v1].m_ptOutAdj;      //v1的第一条关联边
		//寻找邻接顶点v2
		while (p != nullptr && /*p->m_nEndID - 1*/GetCrossById(p->m_nEndID)->m_nCrossIndex != v2) 
			p = p->m_ptOutLink;
		if (p != nullptr)
			return p->m_fWeight;                   //找到此边，返回对应的权值
	}
	return maxValue;                               //找不到此边，返回最大值
}

//返回边(v1, v2)上的路名
//其中v1,v2是图中以0下标开始的标号
int GraphLink::GetRoadID(int v1, int v2) {
	if (v1 != -1 && v2 != -1) {
		CRoad* p = m_ptNodeTable[v1].m_ptOutAdj;      //v1的第一条关联边
		//寻找邻接顶点v2
		while (p != nullptr && /*p->m_nEndID - 1*/GetCrossById(p->m_nEndID)->m_nCrossIndex != v2) 
			p = p->m_ptOutLink;
		if (p != nullptr)
			return p->m_nRoadID;                   //找到此边，返回边的ID号
	}
	return 0;
}

//在图中插入一个顶点reverseRoad
//若插入成功，则返回true，否则返回false
bool GraphLink::InsertVertex(const CCross& cross) {
	if (m_nNumVertices == m_nMaxVertices)             //顶点表满，不能插入
		return false;
	m_ptNodeTable[m_nNumVertices++] = cross;          //插入在表的最后,从1开始计数
	return true;
}

//在图中插入边（道路）road,
bool GraphLink::InsertEdge(const CRoad& road) {
	/*int index1 = road.m_nStartID - 1;
	int index2 = road.m_nEndID - 1;*/
	int index1 = GetCrossById(road.m_nStartID)->m_nCrossIndex;
	int index2 = GetCrossById(road.m_nEndID)->m_nCrossIndex;

	if (index1 >= 0 && index1 < m_nNumVertices && index2 >= 0 && index2 < m_nNumVertices) {
		//CRoad*q, *p = m_ptNodeTable[index1].m_ptOutAdj;         //v1对应的边链表头指针
		//while (p != nullptr && p->m_nEndID - 1 != index2)       //寻找邻接顶点v2
		//	p = p->m_ptOutLink;
		//if (p != nullptr)                                        //找到此边，不插入
		//	return false;
		//否则创建新结点
		CRoad* p = new CRoad();
		CRoad* q = new CRoad();
		p->m_nRoadID = road.m_nRoadID;               //道路ID值
		p->m_nStartID = road.m_nStartID;
		p->m_nEndID = road.m_nEndID;                 //终点ID值（对应道路的下一路口）
		p->m_nLength = road.m_nLength;               //道路长度
		p->m_nLaneNum = road.m_nLaneNum;             //车道数
		p->m_nLimitSpeed = road.m_nLimitSpeed;         //限速
		p->m_fWeight = road.m_fWeight;
		p->m_nRoadCondition = road.m_nRoadCondition;	
			
		//插入index1出度
		if (m_ptNodeTable[index1].m_ptOutAdj == nullptr)
			m_ptNodeTable[index1].m_ptOutAdj = p;
		else {
			CRoad* pLast = m_ptNodeTable[index1].m_ptOutAdj;
			while (pLast->m_ptOutLink != nullptr)
				pLast = pLast->m_ptOutLink;
			pLast->m_ptOutLink = p;
		}
		//插入index2的入度
		if (m_ptNodeTable[index2].m_ptInAdj == nullptr)
			m_ptNodeTable[index2].m_ptInAdj = p;
		else {
			CRoad* pLast = m_ptNodeTable[index2].m_ptInAdj;
			while (pLast->m_ptInLink != nullptr)
				pLast = pLast->m_ptInLink;
			pLast->m_ptInLink = p;
		}

		m_nNumEdges++;

		//如果是双向，就再增加反向的边，否则不增加
		if (road.m_bTwoWay) {
			q->m_nStartID = road.m_nEndID;
			q->m_nEndID = road.m_nStartID;
			q->m_nRoadID = road.m_nRoadID;
			q->m_nLength = road.m_nLength;
			q->m_nLaneNum = road.m_nLaneNum;
			q->m_nLimitSpeed = road.m_nLimitSpeed;
			q->m_fWeight = road.m_fWeight;
			q->m_nRoadCondition = road.m_nRoadCondition;
	
			//插入index2的出度
			if (m_ptNodeTable[index2].m_ptOutAdj == nullptr)
				m_ptNodeTable[index2].m_ptOutAdj = q;
			else {
				CRoad* pLast = m_ptNodeTable[index2].m_ptOutAdj;
				while (pLast->m_ptOutLink != nullptr)
					pLast = pLast->m_ptOutLink;
				pLast->m_ptOutLink = q;
			}
			////插入index1的入度
			if (m_ptNodeTable[index1].m_ptInAdj == nullptr)
				m_ptNodeTable[index1].m_ptInAdj = q;
			else {
				CRoad* pLast = m_ptNodeTable[index1].m_ptInAdj;
				while (pLast->m_ptInLink != nullptr)
					pLast = pLast->m_ptInLink;
				pLast->m_ptInLink = q;
			}
			m_nNumEdges++;
		}
		return true;
	}
	return false;
}


//Dijkstra
//Graph是一个带权有向图。
//dist[j], 0≤j<n, 是当前求到的从顶点v到顶点j的最短路径长度,
//path[j], 0≤j<n, 存放求到的最短路径。
int GraphLink::ShortestPath(int v1, int v2, std::stack<int>& stk/*int path[]*/){
	int n = GetNumberOfVertices();
	int* dist = new int[n];
	int* path = new int[n];
	bool *S = new bool[n]; 	                    //最短路径顶点集
	int i, j, k;
	int w, min;
	for (i = 0; i < n; i++) {
		dist[i] = GetWeight(v1, i);              //GetWeight函数是根据两点的点名来获取权值的
		S[i] = false;
		if (i != v1 && dist[i] < maxValue)
			path[i] = v1;
		else 
			path[i] = -1;
	}
	S[v1] = true;
	dist[v1] = 0;     	                      //顶点v加入顶点集合
	for (i = 0; i < n - 1; i++) {             //求解各顶点最短路径	
		min = maxValue;
		int u = v1; 			                 
		for (j = 0; j < n; j++) {            //选不在S中具有最短路径的顶点u
			if (!S[j] && dist[j] < min) {
				u = j;
				min = dist[j];
			}
		}
		S[u] = true;	               	//将顶点u加入集合S
		for (k = 0; k < n; k++) {       //修改		
			w = GetWeight(u, k);
			if (!S[k] && w < maxValue && dist[u] + w < dist[k]) { //顶点k未加入S   			            
				dist[k] = dist[u] + w;
				path[k] = u;     		//修改到k的最短路径
			}
		}
	}

	int reault = dist[v2];
	stk.push(v2);
	int index = path[v2];
	while (index != v1) {
		stk.push(index);
		index = path[index];
	}
	stk.push(v1);

	delete[]dist;
	delete[]S;
	delete[]path;

	return reault;                    //返回最短距离值
}


//a[i][j]是顶点i和j之间的最短路径长度。
//path[i][j]是相应路径上顶点j的前一顶点的顶点号。
void GraphLink::Floyd(/*int ** a, *//*int ** path*/) {
	int i, j, k, n = GetNumberOfVertices();
	//辅助数组
	int** a = new int* [n];
	int** path = new int* [n];
	for (i = 0; i < n; i++) {
		a[i] = new int[n];
		path[i] = new int[n];
	}

	for (i = 0; i < n; i++) {    	         //矩阵a与path初始化	
		for (j = 0; j < n; j++) {
			a[i][j] = GetWeight(i, j);       //这个时候的权值不再是距离，而是距离除以限速
			//std::cout << a[i][j] << " ";
			if (i != j && a[i][j] < maxValue) {
				path[i][j] = i;
			}
			else {
				path[i][j] = 0;
			}
		}
	}

	for (k = 0; k < n; k++) {   //针对每一个k, 产生a(k)及path(k)			
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				if (a[i][k] + a[k][j] < a[i][j]) {
					a[i][j] = a[i][k] + a[k][j];
					path[i][j] = path[k][j];     //缩短路径长度, 绕过 k 到 j
				}
			}
		}
	}

	//初始化m_vecvecWeight
	for (i = 0; i < n; i++) {
		std::vector<int>vecWei;
		for (j = 0; j < n; j++) {
			vecWei.push_back(a[i][j]);
		}
		m_vecvecWeight.push_back(vecWei);
	}

	//初始化m_vecvecPath
	for (i = 0; i < n; i++) {
		std::vector<std::vector<int>>vecvecPath;
		for (j = 0; j < n; j++) {
			std::vector<int>vecPath;
			//vecPath.push_back(j + 1);
			vecPath.push_back(GetCrossByIndex(j)->m_nCrossID);
			int index = path[i][j];
			while (index != i) {
				//vecPath.push_back(index + 1);
				vecPath.push_back(GetCrossByIndex(index)->m_nCrossID);
				index = path[i][index];
			}
			//vecPath.push_back(i + 1);
			vecPath.push_back(GetCrossByIndex(i)->m_nCrossID);
			std::reverse(vecPath.begin(), vecPath.end());    //逆序排列
			vecvecPath.push_back(vecPath);
		}
		m_vecvecPath.push_back(vecvecPath);
	}


	//释放内存
	for (i = 0; i < n; i++) {
		delete[] a[i];
		delete[] path[i];
	}

	delete[]a;
	delete path;
}

std::ostream& operator <<(std::ostream& out, GraphLink& G) {
	//输出图的所有顶点和边的信息
	int i, j, n, m;
	int e1, e2;        //边
	int w;             //权重
	int id;            //id号
	n = G.GetNumberOfVertices();
	m = G.GetNumberOfEdges();
	out << "VerTex：" << n << "\t" << "Edge：" << m << std::endl;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			w = G.GetWeight(i, j);
			id = G.GetRoadID(i, j);
			if (w > 0 && w < maxValue) {
				e1 = G.GetCrossByIndex(i)->m_nCrossID;
				e2 = G.GetCrossByIndex(j)->m_nCrossID;
				out << "(" << id << "," << e1 << "," << e2 << "," << w << ")\n";
			}
			w = G.GetWeight(j, i);
			id = G.GetRoadID(j, i);
			if (w > 0 && w < maxValue) {
				e1 = G.GetCrossByIndex(j)->m_nCrossID;
				e2 = G.GetCrossByIndex(i)->m_nCrossID;

				out << "(" << id << "," << e1 << "," << e2 << "," << w << ")\n";
			}
		}
	}
	return out;
}


//这个函数用于初始化每条路的最短路径
void GraphLink::AnalogConsequence() {
	for (int i = 0; i < m_nMaxVertices;i++) {
		CCross* cross = &m_ptNodeTable[i];
		for (int k = 0; k < MAX_CAR_START_TIME; k++) {        //总共十次发车
			for (int j = 0; j < cross->m_vecvecGarage[k].size(); j++) {
				CCar& car = cross->m_vecvecGarage[k][j];
				//int start = car.m_nOrigin - 1;        //始发点
				//int end = car.m_nDestination - 1;     //终点
				int originIndex = GetCrossById(car.m_nOrigin)->m_nCrossIndex;          //始发点
				int destIndex = GetCrossById(car.m_nDestination)->m_nCrossIndex;    //终点
			    //将利用Floyd求出的最短路径的路径序列赋值给car的最短路径
				for (int f = 1; f < m_vecvecPath[originIndex][destIndex].size(); f++) {
					int startCrossID = m_vecvecPath[originIndex][destIndex][f - 1];
					int endCrossID = m_vecvecPath[originIndex][destIndex][f];
					int startCrossIndex = GetCrossById(startCrossID)->m_nCrossIndex;
					int endCrossIndex = GetCrossById(endCrossID)->m_nCrossIndex;
					//根据下标求取出RoadID，装入该车辆的最短路径中
					int roadID = GetRoadID(startCrossIndex, endCrossIndex);
					car.m_vecShortestPath.push_back(roadID);
				}
			}
		}
	}
	/*
	//得到所有节点车库还有多少车没有发
	std::cout << "每个车库的车数量如下：\n";
	for (int i = 0; i < m_nMaxVertices; i++) {
		int carsNumberInGarage = 0;
		for (auto &sameTimeCars : m_ptNodeTable[i].Garage) {
			carsNumberInGarage += sameTimeCars.size();
			
		}
		std::cout << "	" << carsNumberInGarage;
	}*/
}


//根据起点的startCrossId与边的名称，获取终点ID
int GraphLink::GetRoadEndCrossID(const int startCrossId, const int roadId) {
	int startCrossIndex = GetCrossById(startCrossId)->m_nCrossIndex;
	CRoad* p = m_ptNodeTable[startCrossIndex].m_ptOutAdj;      //第一条关联边
	while (p != nullptr && p->m_nRoadID != roadId)             //寻找邻接顶点
		p = p->m_ptOutLink;
	if (p != nullptr)
		return p->m_nEndID;                   //找到此边，返回边的ID号

	return -1;
}

//当发生死锁是，那么就更改路径
void GraphLink::ChangeCarPath(CCar* car, CCross* cross) {

	//先将原先的道路删去锁死之后的部分
	/*int starIndex = car->m_nOrigin - 1;
	int endIndex = car->m_nDestination - 1;*/
	//获取车辆当前车辆的起点路口ID及终点ID
	int startCrossIndex = GetCrossById(car->m_nOrigin)->m_nCrossIndex;
	int endCrossIndex = GetCrossById(car->m_nDestination)->m_nCrossIndex;
	//记录从末尾到死锁经历的路口个数
	int count = 0;                
	//获取当前道路的起点			 
	int lastCrossId = -1; 
	//获取原本下一条道路的终点路口ID
	int oldNextCrossId = -1;    
	//获取当前车的最短路径的顶点序列
	std::vector<int> nodePath;

	//获取车辆的起点路口ID，利用下面的循环找到所有的路口序列
	//不能直接使用m_vecvecParh,因为这个车辆可能已经更改过路径
	int startCrossId = car->m_nOrigin;
	nodePath.push_back(startCrossId);
	for (int i = 0; i < car->m_vecShortestPath.size(); i++) {
		int endCrossId = GetRoadEndCrossID(startCrossId, car->m_vecShortestPath[i]);
		if (endCrossId != -1) {
			nodePath.push_back(endCrossId);
			startCrossId = endCrossId;
		}	
	}

	//先计算出需要删除的边的数量
	//并获取lastCrossId与oldNextCrossId的值
	for (int i = nodePath.size() - 1; i >= 0; i--) {
		if (nodePath[i] == cross->m_nCrossID) {
			//如果当前的结点不是整个路径中最后一个结点的话
			//就给原本下一条道路的终点路口ID赋值
			if (i != nodePath.size() - 1)
				oldNextCrossId = nodePath[i + 1];
			//如果当前的结点不是整个路径中的第一个结点的话
			//就给原本路径中的上一个结点赋值
			if (i != 0)
				lastCrossId = nodePath[i - 1];
			break;
		}
		count++;
	}

	//获取下一条道路的ID
	int nextRoadId = car->GetNextRoadID();    
	//找到了下一条道路
	if (nextRoadId != -1) {
		CRoad* ptRoad = cross->m_ptOutAdj;
		while (ptRoad != nullptr) {
			if (ptRoad->m_nEndID != oldNextCrossId && ptRoad->m_nEndID != lastCrossId) {      //随机获取一个新路口
				//先判断是否会有走回路的情况
				int newFirtRoadId = GetRoadID(GetCrossById(cross->m_nCrossID)->m_nCrossIndex,
					GetCrossById(ptRoad->m_nEndID)->m_nCrossIndex);
				std::vector<int> newAddPath = m_vecvecPath[GetCrossById(ptRoad->m_nEndID)->m_nCrossIndex][endCrossIndex];

				if (newFirtRoadId != GetRoadID(GetCrossById(newAddPath[0])->m_nCrossIndex, GetCrossById(newAddPath[1])->m_nCrossIndex)) {
					//从car的路径中删除堵死之后的道路
					for (int i = 0; i < count; i++)
						car->m_vecShortestPath.pop_back();
					//car->shortestPath.push_back(GetRoadID(cross->m_nCrossID - 1, ptRoad->m_nEndID - 1));
					//给新路线中首先加一条道路路径
					
					car->m_vecShortestPath.push_back(newFirtRoadId);
					//获取新增路线的Cross序列
					//std::vector<int> newAddPath = m_vecvecPath[ptRoad->m_nEndID - 1][endCrossIndex];
					for (int i = 0; i < newAddPath.size() - 1; i++) {
						/*int v1 = newAddPath[i] - 1;
						int v2 = newAddPath[i + 1] - 1;*/
						int v1 = GetCrossById(newAddPath[i])->m_nCrossIndex;
						int v2 = GetCrossById(newAddPath[i+1])->m_nCrossIndex;
						car->m_vecShortestPath.push_back(GetRoadID(v1, v2));
					}
					break;
				}
			}
			ptRoad = ptRoad->m_ptOutLink;
		}
	}
}


//模拟交通规则
void GraphLink::TrafficRules() {
	//按时间片段处理，每过一段时间，车就走一段道
	int systemRunningTimes = 0;
	//allCarsfinished判断所有的车到达终点
	bool allCarsFinished = false;
	bool isLocked = false;
	std::vector<Result> resultList;
	//记录有多少车已经到达
	int countArriveCarsNum = 0;
	while (!allCarsFinished) {
		systemRunningTimes++;
		//std::cout << "Start " << systemRunningTimes << " times system running！" << std::endl;
		//---第一步，遍历每个节点上的道路的车辆状态---
		for (int i = 0; i < m_nNumVertices; i++) {
			//遍历节点出口道路
			CRoad *road = m_ptNodeTable[i].m_ptInAdj;
			while (road != nullptr) {
				//按照channel升序调度车道里边的车
				for (int i = 0; i < road->m_nRoadCondition.size(); i++) {
					road->DriveAllCarJustOnRoadToEndState(i);
				}			
				road = road->m_ptInLink;
			}
		}

		//----第二步：处理路口上和道路上所有处于等待状态的车辆---
		int lastWaitingCarsNumber = -1;
		//记录被锁了多少次
		int unLockedCount = 0;
		//只有当所有的节点车都不处于等待状态，才进行下一步
		std::vector<int> allWaitingCrossIndex = GetWaitingCrossIndex();
		while (!allWaitingCrossIndex.empty())
		{
			//遍历所有处于等待的路口
			for (int crossIndex : allWaitingCrossIndex) {
				CCross *cross = &m_ptNodeTable[crossIndex];
				//遍历每个入口道路
				CRoad *entranceRoad = cross->m_ptInAdj;
				while (entranceRoad != nullptr)
				{
					//直到入口道路所有车都为终止状态，或者遇到了冲突
					while (!entranceRoad->AllCarsNotWaiting())
					{
						//----判断是否会冲突----
						//得到当前道路的第一优先级的车
						int channelIndex = 0;
						CCar* car = entranceRoad->GetFirstPriorityCar(channelIndex);
						//得到另外的几条路的头车
						std::vector<CCar*> otherRoadsFirstCar;
						CRoad *otherRoad = cross->m_ptInAdj;
						while (otherRoad != nullptr) {							
							if (otherRoad->m_nRoadID != entranceRoad->m_nRoadID) {
								int otherRoadChanelIndex = 0;
								otherRoadsFirstCar.push_back(otherRoad->GetFirstPriorityCar(otherRoadChanelIndex));
							}								
							otherRoad = otherRoad->m_ptInLink;
						}
						
						//判断方向是否冲突
						if (car == nullptr) {
							std::cout << "ERR Detected waiting car in cross，unfound in Road ！" << std::endl;
							exit(-1);
						}
						else
						{
							//冲突了就跳出这条路的循环
							if (Conflict(otherRoadsFirstCar,car,cross))
								break;
						}
						//std::cout << "ID为：" << car->m_nCarID << "的车正在经过路口:" << cross->m_nCrossID << std::endl;
						//----驱车驶入新的道路----						
						int destination = car->GetNextRoadID();
						if (destination == -1) {
							//即将到达终点，就把前面的那条路作为目的地
							destination = cross->GetFrontRoadID(car->GetCurrentRoadID());
						}
						if (destination == -2) {
							//-1表示下一个路就是终点
							//std::cout << "Car ID:" << car->m_nCarID << " has ran " << systemRunningTimes << " times finished!" << std::endl;
							countArriveCarsNum++;
							CCar finishedCar = entranceRoad->DriveOut(car->m_nCarID);
							Result result;
							result.m_nCarId = finishedCar.m_nCarID;
							result.m_nStartTime = finishedCar.m_nRealStartTime;
							result.m_nSystemRunningTime = systemRunningTimes;
							result.m_nPlanStartTime = finishedCar.m_nStartTime;
							result.m_vecVisitedPath = finishedCar.m_vecShortestPath;
							resultList.push_back(result);
							if (finishedCar.m_nCarID != -1) {
								//让车道的所有的车再次更新行驶状态,遍历下个车道
								entranceRoad->DriveAllWaitingCarToEnd(channelIndex);
							}
							else
							{
								std::cout << "ERR Car startFailed！\n";
								exit(-1);
							}
						}
						else
						{
							//判断出口道路情况
							CRoad* exitRoad = cross->GetExitRoad(destination);
							if (exitRoad == nullptr) {
								//前面是单向道路
								std::cout << "ERR Can't found export！\n";
								exit(-1);
							}
							else
							{

								//V1当前道路速度，V2目标道路的速度，当前道路可行使距离S1,目标道路可行使距离S2
								
								int V1 = car->GetCurrentSpeed();
								int S1 = car->GetRemianingLength();
								int V2 = std::min(exitRoad->m_nLimitSpeed, car->m_nMaxSpeed);
								int S2 = 0;
								if (S1 < V1) {
									//在路口
									if (S1 >= V2 && S1 != 0) {
										S2 = 0;
									}
									else
									{
										S2 = V2 - S1;
									}
								}
								else {
									std::cout << "ERR Get wrong Car in Road！" << std::endl;
									exit(-1);
								}
								//判断是否能够出口中的车是否等待
								if (exitRoad->CheckExitCondition(S2)) {
									if (S2 == 0) {
										//只能走到当前路口的最前方
										car->Run(S1);
										car->m_bIsWaiting = false;
										//entranceRoad->driveAllCarJustOnRoadToEndState(channelIndex);
										//让后面的所有车辆行驶到最终状态
										entranceRoad->DriveBehindCarsToEnd(channelIndex);
									}
									else
									{
										//判断是否到达终点
										if (car->GetNextRoadID() == -1) {
											//-1表示下一个路就是终点
											//std::cout << "Car ID:" << car->m_nCarID << " has ran " << systemRunningTimes << " times finished!" << std::endl;
											countArriveCarsNum++;
											CCar finishedCar = entranceRoad->DriveOut(car->m_nCarID);
											Result result;
											result.m_nCarId = finishedCar.m_nCarID;
											result.m_nStartTime = finishedCar.m_nRealStartTime;
											result.m_nSystemRunningTime = systemRunningTimes;
											result.m_nPlanStartTime = finishedCar.m_nStartTime;
											result.m_vecVisitedPath = finishedCar.m_vecShortestPath;
											resultList.push_back(result);
											if (finishedCar.m_nCarID != -1) {
												//让车道的所有的车再次更新行驶状态
												entranceRoad->DriveAllWaitingCarToEnd(channelIndex);
											}
											else
											{
												std::cout << "ERR Car startFailed！\n";
												exit(-1);
											}
										}
										else
										{
											//没到达终点
											//可以驶入,让车驶出出口，驶入入口，并更新出口车道所有车的行驶状态
											CCar outCar = entranceRoad->DriveOut(car->m_nCarID);
											if (outCar.m_nCarID == -1) {
												std::cout << "ERR Car run out from " << exitRoad->m_nRoadID << " Failed！" << std::endl;
												exit(-1);
											}
											entranceRoad->DriveAllWaitingCarToEnd(channelIndex);
											if (!exitRoad->DriveIn(outCar, S2, V2)) {
												std::cout << "ERR Car run in to " << exitRoad->m_nRoadID << " Failed！" << std::endl;
												exit(-1);
											}
										}

									}
								}
								else
								{
									//被等待车辆阻拦，就去循环下一条入口道路
									break;
								}
							}
						}
					}
					//指向下一个入口道路
					entranceRoad = entranceRoad->m_ptInLink;
				}
			}
			//更新allWaitingCross
			allWaitingCrossIndex = GetWaitingCrossIndex();
			//得到所有等待车的数量
			int currentWaitingCarNumber = 0;
			for (int waitingCrossIndex : allWaitingCrossIndex) {
				currentWaitingCarNumber += m_ptNodeTable[waitingCrossIndex].GetWaitingNumber();
			}
			//判断死锁
			if (currentWaitingCarNumber == lastWaitingCarsNumber) {
				//发生死锁
				std::cout << "--------DEADLOCK:in " << systemRunningTimes << " system Runningtime Locked !---------" << std::endl;
				std::cout << "Locked CrossID：" << std::endl;
				for (int waitingCrossIndex : allWaitingCrossIndex) {
					std::cout << " " << waitingCrossIndex;
				}
				std::cout << std::endl;

				//判断是否改变一辆死锁路上的车
				bool isChangeCar = false;
				for (int index : allWaitingCrossIndex) {
					CCross *ptCross = &m_ptNodeTable[index];
					//判断死锁之后动态规划路径
					CRoad* ptRoad = ptCross->m_ptInAdj;
					while (ptRoad != nullptr) {
						int channelIndex;
						CCar* ptFirstCar = ptRoad->GetFirstPriorityCar(channelIndex);

						//此车当前车道的起点是死锁路口中的一个
						bool isStartCrossLocked = false;
						for (int otherWaitingCrossIndex : allWaitingCrossIndex) {
							if (otherWaitingCrossIndex != index) {
								//判断当前道路的起点ID是不是被死锁的节点
								CCross &otherCross = m_ptNodeTable[otherWaitingCrossIndex];
								if (otherCross.m_nCrossID == ptRoad->m_nStartID) {
									isStartCrossLocked = true;
									break;
								}
							}
						}

						if (ptFirstCar != nullptr &&  ptFirstCar->m_bIsWaiting && isStartCrossLocked) {  //如果车道的第一辆车为等待
						  //那么就更改这个车行车路径
							ChangeCarPath(ptFirstCar, ptCross);
							std::cout << ptFirstCar->m_nCarID << "at" << ptCross->m_nCrossID << "changed path\n";
						}
						ptRoad = ptRoad->m_ptInLink;
					}
					//改了一条路,就退出去，不然就所有全改
					if (isChangeCar) {
						//break;
					}
				}
				/*isLocked = true;
				break;*/
				if (++unLockedCount== UNLOCK_NUM) {
					//到了最大解死锁的次数
					std::cout << "ERR UnLock Falied!" << std::endl;
					//break;
					exit(-1);
				}
			}
			else
			{
				lastWaitingCarsNumber = currentWaitingCarNumber;
			}
		}
		/*if (isLocked)
			break;*/
		//驱使车库里边的车出来,理论先按出发时间升序调度，再按照路口id升序调度，最后按照车辆id升序调度h
		//1、检查路上一共有多少辆车在跑
		int sumRunnningCars = 0;
		for (int i = 0; i < m_nMaxVertices; i++) {
			sumRunnningCars += m_ptNodeTable[i].GetRuningCarNumber();
		}
		//2、路上的车小于500辆就继续发，发到500辆车

		if (sumRunnningCars < CAR_NUM) {
			//升序遍历每个节点
			for (int i = 0; i < m_nMaxVertices; i++) {
				CCross *cross = &m_ptNodeTable[i];
				//按照以系统时间为最大发车
				for (int j = 0; j < std::min(systemRunningTimes,MAX_CAR_START_TIME); j++) {
					//记录发车位置，便于从数组中删除
					std::vector<int> bootedCarsIndex;
					//得到相同的时间的数组
					for (int k = 0; k < cross->m_vecvecGarage[j].size();k++) {
						//按照车辆号码升序调度
						if (cross->DriveFromGarage(cross->m_vecvecGarage[j][k], systemRunningTimes)) {
							//发车成功,运行车数加1，删除的节点引索加1
							bootedCarsIndex.push_back(k);							
							sumRunnningCars++;
							//判断路上的车是否到数了
							if (sumRunnningCars >= CAR_NUM) {
								break;
							}
						}
						else
						{
							//----如果这个路口发不出来车就换下一个路口（新加）----
							break;
						}
					}
					//删除当前时间已经出发的车辆
					for (int k = bootedCarsIndex.size() - 1; k >= 0; k--) {
						cross->m_vecvecGarage[j].erase(cross->m_vecvecGarage[j].begin() + bootedCarsIndex[k]);
					}
					//判断路上的车是否到数了
					if (sumRunnningCars >= CAR_NUM) {
						break;
					}
				}
				//判断路上的车是否到数了
				if (sumRunnningCars >= CAR_NUM) {
					break;
				}
			}
		}
		//通过路上和车库车数总和，判断是否所有车都到了终点
		if (sumRunnningCars == 0) {
			int carsNumberInGarage = 0;
			//得到所有节点车库还有多少车没有发
			for (int i = 0; i < m_nMaxVertices; i++) {
				for (auto &sameTimeCars : m_ptNodeTable[i].m_vecvecGarage) {
					carsNumberInGarage += sameTimeCars.size();
				}
			}
			
			if (carsNumberInGarage == 0) {
				//所有车库没车了
				allCarsFinished = true;
				std::cout << "CAR_NUM == "<<CAR_NUM<<" , All cars finished at "<<systemRunningTimes<<" times" << std::endl;
				WriteFile(m_strResultPath, resultList);
				break;
			}
		}

	}
}
//用于检查是否都为终止状态，返回处于终止状态的路口引索
std::vector<int> GraphLink::GetWaitingCrossIndex() {
	std::vector<int> waitingCross;
	for (int i = 0; i < m_nMaxVertices; i++) {
		//四个入口都为空则路口是终止状态的
		CRoad* p = m_ptNodeTable[i].m_ptInAdj;
		while (p != nullptr) {
			if (p->GetWaitingCarsNumber() > 0) {
				waitingCross.push_back(i);
				break;
			}				
			p = p->m_ptInLink;
		}
	}
	return waitingCross;
}

//查看与其他车的终点和转向比较
bool GraphLink::Conflict(std::vector<CCar*> otherCars, CCar* car, CCross *cross) {
	int destination = car->GetNextRoadID();
	TurningDirection carDirection = cross->GetDirection(car->GetCurrentRoadID(), destination);
	for (CCar*otherCar : otherCars) {
		if (otherCar != nullptr) {
			int otherCarsDestionation = otherCar->GetNextRoadID();			
			//对于到达终点的车，取该车车道的对面车道为目的地，再去作比较
			if (otherCarsDestionation == -1) {				
				otherCarsDestionation = cross->GetFrontRoadID(otherCar->GetCurrentRoadID());
			}
			//目的地相同时，比较方向,
			if (destination == otherCarsDestionation) {
				//当其他的转向等级比当前车的搞，返回false,方向是反着排列的
				if (cross->GetDirection(otherCar->GetCurrentRoadID(), otherCarsDestionation) < carDirection) {
					return true;
				}
			}
		}
	}
	return false;
}
