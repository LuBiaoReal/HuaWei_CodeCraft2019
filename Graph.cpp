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
	//�����ڶ����ַ������ļ��е�����
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
		//���ַ�ת����
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
	//���ֳ������ٵľ���
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
	//��ȡcar�ļ�֮ǰ���ȳ�ʼ��һ��m_vecvecCarһ��
	//����ʱ���1��ʼ����vector���±��0��ʼ�������õ�ʱ��Ҫע���1
	//�����ַ���װ��ͬһ����ʱ��ĳ���IDҲ�ź�����
	//for (int i = 0; i < 100; i++) {
	//	std::vector<CCar>vecCar;
	//	m_vecvecCar.push_back(vecCar);
	//}
	//std::cout << "........" << std::endl;
	this->m_strResultPath = resultPath;
	//��ȡ�����ļ�
	//�Բ�ͬ�ĵ�ͼ���ò�ͬ�Ĳ���
	//int mapNum = checkWhichMap(getMapName(carPath));
	//if (mapNum == 1) {
	//	//��ͼ1
	//	CAR_NUM = 2600;
	//	//std::cout << "��ͼ1" << std::endl;
	//}
	//else if (mapNum == 2)
	//{
	//	//��ͼ2
	//	CAR_NUM = 1250;
	//	//std::cout << "��ͼ2" << std::endl;
	//}
	//else
	//{
	//	//û�ҵ�ͼ������
	//	CAR_NUM = 1200;
	//	//std::cout << "��ͼ����û�ҵ�" << std::endl;
	//}
	//----�����-----
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
		//ȡ�õ�ͼ1�Ĳ���
		CAR_NUM = 2600;
	}
	std::cout << "seed:" << seed << std::endl;
	if (seed == 1) {
		//0��ʾ��ͼ2
		CAR_NUM = 1550;
	}
	else
	{
		//1��ʾͼ1
		CAR_NUM = 3150;
	}
	ReadFile(crossPath, "Cross");

	//��ʼ���ڵ��б�
	m_nMaxVertices = m_vecCross.size();               //��󶥵���Ϊ·����
	m_nNumVertices = 0;
	m_nNumEdges = 0;
	m_ptNodeTable = new CCross[m_nMaxVertices];       //�������������
	if (m_ptNodeTable == nullptr) {
		std::cerr << "Memory allocation ERR��\n";
		exit(-1);
	}
	

	//��ͼ�в��붥��
	for (int i = 0; i < m_vecCross.size(); i++) {
		InsertVertex(m_vecCross[i]);
	}
	//�ڱ����ߵ�ʱ��ֱ�ӽ����ݷŵ��ڵ���ȥ
	ReadFile(roadPath, "Road");
	//��ͼ�в����
	std::vector<CRoad>::iterator iter_road = m_vecRoad.begin();    //ָ��road�ĵ�����
	while (iter_road != m_vecRoad.end()) {
		InsertEdge(*iter_road);
		iter_road++;
	}
	ReadFile(carPath, "Car");
    //���ø�������������е�֮������·��
	Floyd();
}

//-----��ȡ�����ļ�------
void GraphLink::ReadFile(const std::string path, const std::string type) {
	std::ifstream fin;
	fin.open(path);
	if (fin.fail()) {
		std::cout << "Open "<<type<<" Filed��\n";
		exit(-1);
	}

	//---�ָ��ļ�����---
	std::string strLine;
	std::vector<int> vecLine;
	//������Ϊ������·�ڣ���·������±�
	int carCount = -1;
	int crossCount = -1;
	int roadCount = -1;
	//ȥ����һ�е�ע��
	getline(fin, strLine);
	//���ж�ȡ,�ָ�����
	while (getline(fin, strLine)) {
		strLine = strLine.substr(1, strLine.length() - 2);
		vecLine = SomeFunc::split(strLine,",");
		//---�ж��ļ�����---
		int dataSize = vecLine.size();
		if (type == "Car" && dataSize == 5) {
			//std::cout << "Car:" << vecLine[0] << std::endl;
			CCar car(vecLine[0], vecLine[1], vecLine[2], vecLine[3], vecLine[4]);
			
			//�������±긳��ֵ
			/*carCount++;
			car.m_nCarIndex = carCount;*/
			//������ջ
			m_vecCar.push_back(car);
			//m_vecvecCar[vecLine[4] - 1].push_back(car);
		}
		else if (type == "Cross" && dataSize == 5) {
			//std::cout << "Cross:" << vecLine[0] << std::endl;
			CCross cross(vecLine[0], vecLine[1], vecLine[2], vecLine[3], vecLine[4]);
			//����·�±긳��ֵ
			/*crossCount++;
			cross.m_nCrossIndex = crossCount;*/
			//·����ջ
			m_vecCross.push_back(cross);
		}
		else if (type == "Road" && dataSize == 7) {
			//roadID, roadDistance, maxSpeed, channleNumber, startCrossID, endCrossID, isTwoWay
			CRoad road(vecLine[0], vecLine[1], vecLine[2], vecLine[3], vecLine[4], vecLine[5], vecLine[6]);
			//��·�±���ջ
			/*roadCount++;
			road.m_nRoadIndex = roadCount;*/
	        //��·��ջ
			m_vecRoad.push_back(road);
		}
		else {
			fin.close();
			std::cout << "read failed!" << std::endl;
			exit(-1);
		}
	}

	//��ȡ���ļ���Ϣ֮���ļ���Ϣ����
	if (type == "Car") {
		//�Գ�����������
		std::sort(m_vecCar.begin(), m_vecCar.end(), SomeFunc::CarSortFunc);

		//�ź���֮�󣬽���װ�����泵��
		for (int i = 0; i < m_vecCar.size(); i++) {
			CCar car = m_vecCar[i];
			int startID = car.m_nOrigin;
			int startTime = car.m_nStartTime;
			CCross* ptCross = GetCrossById(startID);
			ptCross->m_vecvecGarage[startTime - 1].push_back(car);
		}
	}
		
	else if (type == "Cross") {
		//��cross����ID��������
		std::sort(m_vecCross.begin(), m_vecCross.end(), SomeFunc::CrossSortFunc);
		//������·�ڵ��±����Գ�ʼ��
		for (int i = 0; i < m_vecCross.size(); i++) {
			m_vecCross[i].m_nCrossIndex = i;
		}
	}
	else {
		//�Ե�·������ID��С��������
		std::sort(m_vecRoad.begin(), m_vecRoad.end(), SomeFunc::RoadSortFunc);
		//��ÿ����·���±����Խ��г�ʼ��
		for (int i = 0; i < m_vecRoad.size(); i++)
			m_vecRoad[i].m_nRoadIndex = i;
	}



	//�ر��ļ�
	fin.close();
}

//-------д�����ļ�----------
void GraphLink::WriteFile(const std::string path, const std::vector<Result> &result) {
	std::ofstream fout;
	fout.open(path);
	if (fout.fail()) {
		std::cout << "Open " << path << " Failed��\n";
		exit(-1);
	}

	//д����
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

//����������ɾ��һ���ڽӱ�
GraphLink::~GraphLink() {
	for (int i = 0; i < m_nMaxVertices; i++) {      //ɾ�������ӱ��еĽ��
		CRoad* p = m_ptNodeTable[i].m_ptOutAdj;          //�ҵ����Ӧ�ı�������׽��
		//CRoad* q = m_ptNodeTable[i].m_ptInAdj;          //�ҵ����Ӧ�ı�������׽��
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
	delete[] m_ptNodeTable;                         //ɾ�����������
}


//ȡλ��Ϊi�Ķ����е�ֵ
CCross* GraphLink::GetCrossByIndex(const int index) {
	//CCross reverseRoad;            //��ʱ·�ڱ������ڷ���
	//return (index >= 0 && index < m_nNumVertices) ? m_ptNodeTable[index] : reverseRoad;
	return &m_ptNodeTable[index];
}



//���ر�(v1,v2)�ϵ�Ȩֵ������v1,v2��ͼ����0�±꿪ʼ�ı��
int GraphLink::GetWeight(int v1, int v2) {
	if (v1 == v2)                                 //���v1 == v2˵�������յ���ͬ������0
		return 0;
	if (v1 != -1 && v2 != -1) {
		CRoad* p = m_ptNodeTable[v1].m_ptOutAdj;      //v1�ĵ�һ��������
		//Ѱ���ڽӶ���v2
		while (p != nullptr && /*p->m_nEndID - 1*/GetCrossById(p->m_nEndID)->m_nCrossIndex != v2) 
			p = p->m_ptOutLink;
		if (p != nullptr)
			return p->m_fWeight;                   //�ҵ��˱ߣ����ض�Ӧ��Ȩֵ
	}
	return maxValue;                               //�Ҳ����˱ߣ��������ֵ
}

//���ر�(v1, v2)�ϵ�·��
//����v1,v2��ͼ����0�±꿪ʼ�ı��
int GraphLink::GetRoadID(int v1, int v2) {
	if (v1 != -1 && v2 != -1) {
		CRoad* p = m_ptNodeTable[v1].m_ptOutAdj;      //v1�ĵ�һ��������
		//Ѱ���ڽӶ���v2
		while (p != nullptr && /*p->m_nEndID - 1*/GetCrossById(p->m_nEndID)->m_nCrossIndex != v2) 
			p = p->m_ptOutLink;
		if (p != nullptr)
			return p->m_nRoadID;                   //�ҵ��˱ߣ����رߵ�ID��
	}
	return 0;
}

//��ͼ�в���һ������reverseRoad
//������ɹ����򷵻�true�����򷵻�false
bool GraphLink::InsertVertex(const CCross& cross) {
	if (m_nNumVertices == m_nMaxVertices)             //������������ܲ���
		return false;
	m_ptNodeTable[m_nNumVertices++] = cross;          //�����ڱ�����,��1��ʼ����
	return true;
}

//��ͼ�в���ߣ���·��road,
bool GraphLink::InsertEdge(const CRoad& road) {
	/*int index1 = road.m_nStartID - 1;
	int index2 = road.m_nEndID - 1;*/
	int index1 = GetCrossById(road.m_nStartID)->m_nCrossIndex;
	int index2 = GetCrossById(road.m_nEndID)->m_nCrossIndex;

	if (index1 >= 0 && index1 < m_nNumVertices && index2 >= 0 && index2 < m_nNumVertices) {
		//CRoad*q, *p = m_ptNodeTable[index1].m_ptOutAdj;         //v1��Ӧ�ı�����ͷָ��
		//while (p != nullptr && p->m_nEndID - 1 != index2)       //Ѱ���ڽӶ���v2
		//	p = p->m_ptOutLink;
		//if (p != nullptr)                                        //�ҵ��˱ߣ�������
		//	return false;
		//���򴴽��½��
		CRoad* p = new CRoad();
		CRoad* q = new CRoad();
		p->m_nRoadID = road.m_nRoadID;               //��·IDֵ
		p->m_nStartID = road.m_nStartID;
		p->m_nEndID = road.m_nEndID;                 //�յ�IDֵ����Ӧ��·����һ·�ڣ�
		p->m_nLength = road.m_nLength;               //��·����
		p->m_nLaneNum = road.m_nLaneNum;             //������
		p->m_nLimitSpeed = road.m_nLimitSpeed;         //����
		p->m_fWeight = road.m_fWeight;
		p->m_nRoadCondition = road.m_nRoadCondition;	
			
		//����index1����
		if (m_ptNodeTable[index1].m_ptOutAdj == nullptr)
			m_ptNodeTable[index1].m_ptOutAdj = p;
		else {
			CRoad* pLast = m_ptNodeTable[index1].m_ptOutAdj;
			while (pLast->m_ptOutLink != nullptr)
				pLast = pLast->m_ptOutLink;
			pLast->m_ptOutLink = p;
		}
		//����index2�����
		if (m_ptNodeTable[index2].m_ptInAdj == nullptr)
			m_ptNodeTable[index2].m_ptInAdj = p;
		else {
			CRoad* pLast = m_ptNodeTable[index2].m_ptInAdj;
			while (pLast->m_ptInLink != nullptr)
				pLast = pLast->m_ptInLink;
			pLast->m_ptInLink = p;
		}

		m_nNumEdges++;

		//�����˫�򣬾������ӷ���ıߣ���������
		if (road.m_bTwoWay) {
			q->m_nStartID = road.m_nEndID;
			q->m_nEndID = road.m_nStartID;
			q->m_nRoadID = road.m_nRoadID;
			q->m_nLength = road.m_nLength;
			q->m_nLaneNum = road.m_nLaneNum;
			q->m_nLimitSpeed = road.m_nLimitSpeed;
			q->m_fWeight = road.m_fWeight;
			q->m_nRoadCondition = road.m_nRoadCondition;
	
			//����index2�ĳ���
			if (m_ptNodeTable[index2].m_ptOutAdj == nullptr)
				m_ptNodeTable[index2].m_ptOutAdj = q;
			else {
				CRoad* pLast = m_ptNodeTable[index2].m_ptOutAdj;
				while (pLast->m_ptOutLink != nullptr)
					pLast = pLast->m_ptOutLink;
				pLast->m_ptOutLink = q;
			}
			////����index1�����
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
//Graph��һ����Ȩ����ͼ��
//dist[j], 0��j<n, �ǵ�ǰ�󵽵ĴӶ���v������j�����·������,
//path[j], 0��j<n, ����󵽵����·����
int GraphLink::ShortestPath(int v1, int v2, std::stack<int>& stk/*int path[]*/){
	int n = GetNumberOfVertices();
	int* dist = new int[n];
	int* path = new int[n];
	bool *S = new bool[n]; 	                    //���·�����㼯
	int i, j, k;
	int w, min;
	for (i = 0; i < n; i++) {
		dist[i] = GetWeight(v1, i);              //GetWeight�����Ǹ�������ĵ�������ȡȨֵ��
		S[i] = false;
		if (i != v1 && dist[i] < maxValue)
			path[i] = v1;
		else 
			path[i] = -1;
	}
	S[v1] = true;
	dist[v1] = 0;     	                      //����v���붥�㼯��
	for (i = 0; i < n - 1; i++) {             //�����������·��	
		min = maxValue;
		int u = v1; 			                 
		for (j = 0; j < n; j++) {            //ѡ����S�о������·���Ķ���u
			if (!S[j] && dist[j] < min) {
				u = j;
				min = dist[j];
			}
		}
		S[u] = true;	               	//������u���뼯��S
		for (k = 0; k < n; k++) {       //�޸�		
			w = GetWeight(u, k);
			if (!S[k] && w < maxValue && dist[u] + w < dist[k]) { //����kδ����S   			            
				dist[k] = dist[u] + w;
				path[k] = u;     		//�޸ĵ�k�����·��
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

	return reault;                    //������̾���ֵ
}


//a[i][j]�Ƕ���i��j֮������·�����ȡ�
//path[i][j]����Ӧ·���϶���j��ǰһ����Ķ���š�
void GraphLink::Floyd(/*int ** a, *//*int ** path*/) {
	int i, j, k, n = GetNumberOfVertices();
	//��������
	int** a = new int* [n];
	int** path = new int* [n];
	for (i = 0; i < n; i++) {
		a[i] = new int[n];
		path[i] = new int[n];
	}

	for (i = 0; i < n; i++) {    	         //����a��path��ʼ��	
		for (j = 0; j < n; j++) {
			a[i][j] = GetWeight(i, j);       //���ʱ���Ȩֵ�����Ǿ��룬���Ǿ����������
			//std::cout << a[i][j] << " ";
			if (i != j && a[i][j] < maxValue) {
				path[i][j] = i;
			}
			else {
				path[i][j] = 0;
			}
		}
	}

	for (k = 0; k < n; k++) {   //���ÿһ��k, ����a(k)��path(k)			
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				if (a[i][k] + a[k][j] < a[i][j]) {
					a[i][j] = a[i][k] + a[k][j];
					path[i][j] = path[k][j];     //����·������, �ƹ� k �� j
				}
			}
		}
	}

	//��ʼ��m_vecvecWeight
	for (i = 0; i < n; i++) {
		std::vector<int>vecWei;
		for (j = 0; j < n; j++) {
			vecWei.push_back(a[i][j]);
		}
		m_vecvecWeight.push_back(vecWei);
	}

	//��ʼ��m_vecvecPath
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
			std::reverse(vecPath.begin(), vecPath.end());    //��������
			vecvecPath.push_back(vecPath);
		}
		m_vecvecPath.push_back(vecvecPath);
	}


	//�ͷ��ڴ�
	for (i = 0; i < n; i++) {
		delete[] a[i];
		delete[] path[i];
	}

	delete[]a;
	delete path;
}

std::ostream& operator <<(std::ostream& out, GraphLink& G) {
	//���ͼ�����ж���ͱߵ���Ϣ
	int i, j, n, m;
	int e1, e2;        //��
	int w;             //Ȩ��
	int id;            //id��
	n = G.GetNumberOfVertices();
	m = G.GetNumberOfEdges();
	out << "VerTex��" << n << "\t" << "Edge��" << m << std::endl;
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


//����������ڳ�ʼ��ÿ��·�����·��
void GraphLink::AnalogConsequence() {
	for (int i = 0; i < m_nMaxVertices;i++) {
		CCross* cross = &m_ptNodeTable[i];
		for (int k = 0; k < MAX_CAR_START_TIME; k++) {        //�ܹ�ʮ�η���
			for (int j = 0; j < cross->m_vecvecGarage[k].size(); j++) {
				CCar& car = cross->m_vecvecGarage[k][j];
				//int start = car.m_nOrigin - 1;        //ʼ����
				//int end = car.m_nDestination - 1;     //�յ�
				int originIndex = GetCrossById(car.m_nOrigin)->m_nCrossIndex;          //ʼ����
				int destIndex = GetCrossById(car.m_nDestination)->m_nCrossIndex;    //�յ�
			    //������Floyd��������·����·�����и�ֵ��car�����·��
				for (int f = 1; f < m_vecvecPath[originIndex][destIndex].size(); f++) {
					int startCrossID = m_vecvecPath[originIndex][destIndex][f - 1];
					int endCrossID = m_vecvecPath[originIndex][destIndex][f];
					int startCrossIndex = GetCrossById(startCrossID)->m_nCrossIndex;
					int endCrossIndex = GetCrossById(endCrossID)->m_nCrossIndex;
					//�����±���ȡ��RoadID��װ��ó��������·����
					int roadID = GetRoadID(startCrossIndex, endCrossIndex);
					car.m_vecShortestPath.push_back(roadID);
				}
			}
		}
	}
	/*
	//�õ����нڵ㳵�⻹�ж��ٳ�û�з�
	std::cout << "ÿ������ĳ��������£�\n";
	for (int i = 0; i < m_nMaxVertices; i++) {
		int carsNumberInGarage = 0;
		for (auto &sameTimeCars : m_ptNodeTable[i].Garage) {
			carsNumberInGarage += sameTimeCars.size();
			
		}
		std::cout << "	" << carsNumberInGarage;
	}*/
}


//��������startCrossId��ߵ����ƣ���ȡ�յ�ID
int GraphLink::GetRoadEndCrossID(const int startCrossId, const int roadId) {
	int startCrossIndex = GetCrossById(startCrossId)->m_nCrossIndex;
	CRoad* p = m_ptNodeTable[startCrossIndex].m_ptOutAdj;      //��һ��������
	while (p != nullptr && p->m_nRoadID != roadId)             //Ѱ���ڽӶ���
		p = p->m_ptOutLink;
	if (p != nullptr)
		return p->m_nEndID;                   //�ҵ��˱ߣ����رߵ�ID��

	return -1;
}

//�����������ǣ���ô�͸���·��
void GraphLink::ChangeCarPath(CCar* car, CCross* cross) {

	//�Ƚ�ԭ�ȵĵ�·ɾȥ����֮��Ĳ���
	/*int starIndex = car->m_nOrigin - 1;
	int endIndex = car->m_nDestination - 1;*/
	//��ȡ������ǰ���������·��ID���յ�ID
	int startCrossIndex = GetCrossById(car->m_nOrigin)->m_nCrossIndex;
	int endCrossIndex = GetCrossById(car->m_nDestination)->m_nCrossIndex;
	//��¼��ĩβ������������·�ڸ���
	int count = 0;                
	//��ȡ��ǰ��·�����			 
	int lastCrossId = -1; 
	//��ȡԭ����һ����·���յ�·��ID
	int oldNextCrossId = -1;    
	//��ȡ��ǰ�������·���Ķ�������
	std::vector<int> nodePath;

	//��ȡ���������·��ID�����������ѭ���ҵ����е�·������
	//����ֱ��ʹ��m_vecvecParh,��Ϊ������������Ѿ����Ĺ�·��
	int startCrossId = car->m_nOrigin;
	nodePath.push_back(startCrossId);
	for (int i = 0; i < car->m_vecShortestPath.size(); i++) {
		int endCrossId = GetRoadEndCrossID(startCrossId, car->m_vecShortestPath[i]);
		if (endCrossId != -1) {
			nodePath.push_back(endCrossId);
			startCrossId = endCrossId;
		}	
	}

	//�ȼ������Ҫɾ���ıߵ�����
	//����ȡlastCrossId��oldNextCrossId��ֵ
	for (int i = nodePath.size() - 1; i >= 0; i--) {
		if (nodePath[i] == cross->m_nCrossID) {
			//�����ǰ�Ľ�㲻������·�������һ�����Ļ�
			//�͸�ԭ����һ����·���յ�·��ID��ֵ
			if (i != nodePath.size() - 1)
				oldNextCrossId = nodePath[i + 1];
			//�����ǰ�Ľ�㲻������·���еĵ�һ�����Ļ�
			//�͸�ԭ��·���е���һ����㸳ֵ
			if (i != 0)
				lastCrossId = nodePath[i - 1];
			break;
		}
		count++;
	}

	//��ȡ��һ����·��ID
	int nextRoadId = car->GetNextRoadID();    
	//�ҵ�����һ����·
	if (nextRoadId != -1) {
		CRoad* ptRoad = cross->m_ptOutAdj;
		while (ptRoad != nullptr) {
			if (ptRoad->m_nEndID != oldNextCrossId && ptRoad->m_nEndID != lastCrossId) {      //�����ȡһ����·��
				//���ж��Ƿ�����߻�·�����
				int newFirtRoadId = GetRoadID(GetCrossById(cross->m_nCrossID)->m_nCrossIndex,
					GetCrossById(ptRoad->m_nEndID)->m_nCrossIndex);
				std::vector<int> newAddPath = m_vecvecPath[GetCrossById(ptRoad->m_nEndID)->m_nCrossIndex][endCrossIndex];

				if (newFirtRoadId != GetRoadID(GetCrossById(newAddPath[0])->m_nCrossIndex, GetCrossById(newAddPath[1])->m_nCrossIndex)) {
					//��car��·����ɾ������֮��ĵ�·
					for (int i = 0; i < count; i++)
						car->m_vecShortestPath.pop_back();
					//car->shortestPath.push_back(GetRoadID(cross->m_nCrossID - 1, ptRoad->m_nEndID - 1));
					//����·�������ȼ�һ����··��
					
					car->m_vecShortestPath.push_back(newFirtRoadId);
					//��ȡ����·�ߵ�Cross����
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


//ģ�⽻ͨ����
void GraphLink::TrafficRules() {
	//��ʱ��Ƭ�δ���ÿ��һ��ʱ�䣬������һ�ε�
	int systemRunningTimes = 0;
	//allCarsfinished�ж����еĳ������յ�
	bool allCarsFinished = false;
	bool isLocked = false;
	std::vector<Result> resultList;
	//��¼�ж��ٳ��Ѿ�����
	int countArriveCarsNum = 0;
	while (!allCarsFinished) {
		systemRunningTimes++;
		//std::cout << "Start " << systemRunningTimes << " times system running��" << std::endl;
		//---��һ��������ÿ���ڵ��ϵĵ�·�ĳ���״̬---
		for (int i = 0; i < m_nNumVertices; i++) {
			//�����ڵ���ڵ�·
			CRoad *road = m_ptNodeTable[i].m_ptInAdj;
			while (road != nullptr) {
				//����channel������ȳ�����ߵĳ�
				for (int i = 0; i < road->m_nRoadCondition.size(); i++) {
					road->DriveAllCarJustOnRoadToEndState(i);
				}			
				road = road->m_ptInLink;
			}
		}

		//----�ڶ���������·���Ϻ͵�·�����д��ڵȴ�״̬�ĳ���---
		int lastWaitingCarsNumber = -1;
		//��¼�����˶��ٴ�
		int unLockedCount = 0;
		//ֻ�е����еĽڵ㳵�������ڵȴ�״̬���Ž�����һ��
		std::vector<int> allWaitingCrossIndex = GetWaitingCrossIndex();
		while (!allWaitingCrossIndex.empty())
		{
			//�������д��ڵȴ���·��
			for (int crossIndex : allWaitingCrossIndex) {
				CCross *cross = &m_ptNodeTable[crossIndex];
				//����ÿ����ڵ�·
				CRoad *entranceRoad = cross->m_ptInAdj;
				while (entranceRoad != nullptr)
				{
					//ֱ����ڵ�·���г���Ϊ��ֹ״̬�����������˳�ͻ
					while (!entranceRoad->AllCarsNotWaiting())
					{
						//----�ж��Ƿ���ͻ----
						//�õ���ǰ��·�ĵ�һ���ȼ��ĳ�
						int channelIndex = 0;
						CCar* car = entranceRoad->GetFirstPriorityCar(channelIndex);
						//�õ�����ļ���·��ͷ��
						std::vector<CCar*> otherRoadsFirstCar;
						CRoad *otherRoad = cross->m_ptInAdj;
						while (otherRoad != nullptr) {							
							if (otherRoad->m_nRoadID != entranceRoad->m_nRoadID) {
								int otherRoadChanelIndex = 0;
								otherRoadsFirstCar.push_back(otherRoad->GetFirstPriorityCar(otherRoadChanelIndex));
							}								
							otherRoad = otherRoad->m_ptInLink;
						}
						
						//�жϷ����Ƿ��ͻ
						if (car == nullptr) {
							std::cout << "ERR Detected waiting car in cross��unfound in Road ��" << std::endl;
							exit(-1);
						}
						else
						{
							//��ͻ�˾���������·��ѭ��
							if (Conflict(otherRoadsFirstCar,car,cross))
								break;
						}
						//std::cout << "IDΪ��" << car->m_nCarID << "�ĳ����ھ���·��:" << cross->m_nCrossID << std::endl;
						//----����ʻ���µĵ�·----						
						int destination = car->GetNextRoadID();
						if (destination == -1) {
							//���������յ㣬�Ͱ�ǰ�������·��ΪĿ�ĵ�
							destination = cross->GetFrontRoadID(car->GetCurrentRoadID());
						}
						if (destination == -2) {
							//-1��ʾ��һ��·�����յ�
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
								//�ó��������еĳ��ٴθ�����ʻ״̬,�����¸�����
								entranceRoad->DriveAllWaitingCarToEnd(channelIndex);
							}
							else
							{
								std::cout << "ERR Car startFailed��\n";
								exit(-1);
							}
						}
						else
						{
							//�жϳ��ڵ�·���
							CRoad* exitRoad = cross->GetExitRoad(destination);
							if (exitRoad == nullptr) {
								//ǰ���ǵ����·
								std::cout << "ERR Can't found export��\n";
								exit(-1);
							}
							else
							{

								//V1��ǰ��·�ٶȣ�V2Ŀ���·���ٶȣ���ǰ��·����ʹ����S1,Ŀ���·����ʹ����S2
								
								int V1 = car->GetCurrentSpeed();
								int S1 = car->GetRemianingLength();
								int V2 = std::min(exitRoad->m_nLimitSpeed, car->m_nMaxSpeed);
								int S2 = 0;
								if (S1 < V1) {
									//��·��
									if (S1 >= V2 && S1 != 0) {
										S2 = 0;
									}
									else
									{
										S2 = V2 - S1;
									}
								}
								else {
									std::cout << "ERR Get wrong Car in Road��" << std::endl;
									exit(-1);
								}
								//�ж��Ƿ��ܹ������еĳ��Ƿ�ȴ�
								if (exitRoad->CheckExitCondition(S2)) {
									if (S2 == 0) {
										//ֻ���ߵ���ǰ·�ڵ���ǰ��
										car->Run(S1);
										car->m_bIsWaiting = false;
										//entranceRoad->driveAllCarJustOnRoadToEndState(channelIndex);
										//�ú�������г�����ʻ������״̬
										entranceRoad->DriveBehindCarsToEnd(channelIndex);
									}
									else
									{
										//�ж��Ƿ񵽴��յ�
										if (car->GetNextRoadID() == -1) {
											//-1��ʾ��һ��·�����յ�
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
												//�ó��������еĳ��ٴθ�����ʻ״̬
												entranceRoad->DriveAllWaitingCarToEnd(channelIndex);
											}
											else
											{
												std::cout << "ERR Car startFailed��\n";
												exit(-1);
											}
										}
										else
										{
											//û�����յ�
											//����ʻ��,�ó�ʻ�����ڣ�ʻ����ڣ������³��ڳ������г�����ʻ״̬
											CCar outCar = entranceRoad->DriveOut(car->m_nCarID);
											if (outCar.m_nCarID == -1) {
												std::cout << "ERR Car run out from " << exitRoad->m_nRoadID << " Failed��" << std::endl;
												exit(-1);
											}
											entranceRoad->DriveAllWaitingCarToEnd(channelIndex);
											if (!exitRoad->DriveIn(outCar, S2, V2)) {
												std::cout << "ERR Car run in to " << exitRoad->m_nRoadID << " Failed��" << std::endl;
												exit(-1);
											}
										}

									}
								}
								else
								{
									//���ȴ�������������ȥѭ����һ����ڵ�·
									break;
								}
							}
						}
					}
					//ָ����һ����ڵ�·
					entranceRoad = entranceRoad->m_ptInLink;
				}
			}
			//����allWaitingCross
			allWaitingCrossIndex = GetWaitingCrossIndex();
			//�õ����еȴ���������
			int currentWaitingCarNumber = 0;
			for (int waitingCrossIndex : allWaitingCrossIndex) {
				currentWaitingCarNumber += m_ptNodeTable[waitingCrossIndex].GetWaitingNumber();
			}
			//�ж�����
			if (currentWaitingCarNumber == lastWaitingCarsNumber) {
				//��������
				std::cout << "--------DEADLOCK:in " << systemRunningTimes << " system Runningtime Locked !---------" << std::endl;
				std::cout << "Locked CrossID��" << std::endl;
				for (int waitingCrossIndex : allWaitingCrossIndex) {
					std::cout << " " << waitingCrossIndex;
				}
				std::cout << std::endl;

				//�ж��Ƿ�ı�һ������·�ϵĳ�
				bool isChangeCar = false;
				for (int index : allWaitingCrossIndex) {
					CCross *ptCross = &m_ptNodeTable[index];
					//�ж�����֮��̬�滮·��
					CRoad* ptRoad = ptCross->m_ptInAdj;
					while (ptRoad != nullptr) {
						int channelIndex;
						CCar* ptFirstCar = ptRoad->GetFirstPriorityCar(channelIndex);

						//�˳���ǰ���������������·���е�һ��
						bool isStartCrossLocked = false;
						for (int otherWaitingCrossIndex : allWaitingCrossIndex) {
							if (otherWaitingCrossIndex != index) {
								//�жϵ�ǰ��·�����ID�ǲ��Ǳ������Ľڵ�
								CCross &otherCross = m_ptNodeTable[otherWaitingCrossIndex];
								if (otherCross.m_nCrossID == ptRoad->m_nStartID) {
									isStartCrossLocked = true;
									break;
								}
							}
						}

						if (ptFirstCar != nullptr &&  ptFirstCar->m_bIsWaiting && isStartCrossLocked) {  //��������ĵ�һ����Ϊ�ȴ�
						  //��ô�͸���������г�·��
							ChangeCarPath(ptFirstCar, ptCross);
							std::cout << ptFirstCar->m_nCarID << "at" << ptCross->m_nCrossID << "changed path\n";
						}
						ptRoad = ptRoad->m_ptInLink;
					}
					//����һ��·,���˳�ȥ����Ȼ������ȫ��
					if (isChangeCar) {
						//break;
					}
				}
				/*isLocked = true;
				break;*/
				if (++unLockedCount== UNLOCK_NUM) {
					//�������������Ĵ���
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
		//��ʹ������ߵĳ�����,�����Ȱ�����ʱ��������ȣ��ٰ���·��id������ȣ�����ճ���id�������h
		//1�����·��һ���ж�����������
		int sumRunnningCars = 0;
		for (int i = 0; i < m_nMaxVertices; i++) {
			sumRunnningCars += m_ptNodeTable[i].GetRuningCarNumber();
		}
		//2��·�ϵĳ�С��500���ͼ�����������500����

		if (sumRunnningCars < CAR_NUM) {
			//�������ÿ���ڵ�
			for (int i = 0; i < m_nMaxVertices; i++) {
				CCross *cross = &m_ptNodeTable[i];
				//������ϵͳʱ��Ϊ��󷢳�
				for (int j = 0; j < std::min(systemRunningTimes,MAX_CAR_START_TIME); j++) {
					//��¼����λ�ã����ڴ�������ɾ��
					std::vector<int> bootedCarsIndex;
					//�õ���ͬ��ʱ�������
					for (int k = 0; k < cross->m_vecvecGarage[j].size();k++) {
						//���ճ��������������
						if (cross->DriveFromGarage(cross->m_vecvecGarage[j][k], systemRunningTimes)) {
							//�����ɹ�,���г�����1��ɾ���Ľڵ�������1
							bootedCarsIndex.push_back(k);							
							sumRunnningCars++;
							//�ж�·�ϵĳ��Ƿ�����
							if (sumRunnningCars >= CAR_NUM) {
								break;
							}
						}
						else
						{
							//----������·�ڷ����������ͻ���һ��·�ڣ��¼ӣ�----
							break;
						}
					}
					//ɾ����ǰʱ���Ѿ������ĳ���
					for (int k = bootedCarsIndex.size() - 1; k >= 0; k--) {
						cross->m_vecvecGarage[j].erase(cross->m_vecvecGarage[j].begin() + bootedCarsIndex[k]);
					}
					//�ж�·�ϵĳ��Ƿ�����
					if (sumRunnningCars >= CAR_NUM) {
						break;
					}
				}
				//�ж�·�ϵĳ��Ƿ�����
				if (sumRunnningCars >= CAR_NUM) {
					break;
				}
			}
		}
		//ͨ��·�Ϻͳ��⳵���ܺͣ��ж��Ƿ����г��������յ�
		if (sumRunnningCars == 0) {
			int carsNumberInGarage = 0;
			//�õ����нڵ㳵�⻹�ж��ٳ�û�з�
			for (int i = 0; i < m_nMaxVertices; i++) {
				for (auto &sameTimeCars : m_ptNodeTable[i].m_vecvecGarage) {
					carsNumberInGarage += sameTimeCars.size();
				}
			}
			
			if (carsNumberInGarage == 0) {
				//���г���û����
				allCarsFinished = true;
				std::cout << "CAR_NUM == "<<CAR_NUM<<" , All cars finished at "<<systemRunningTimes<<" times" << std::endl;
				WriteFile(m_strResultPath, resultList);
				break;
			}
		}

	}
}
//���ڼ���Ƿ�Ϊ��ֹ״̬�����ش�����ֹ״̬��·������
std::vector<int> GraphLink::GetWaitingCrossIndex() {
	std::vector<int> waitingCross;
	for (int i = 0; i < m_nMaxVertices; i++) {
		//�ĸ���ڶ�Ϊ����·������ֹ״̬��
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

//�鿴�����������յ��ת��Ƚ�
bool GraphLink::Conflict(std::vector<CCar*> otherCars, CCar* car, CCross *cross) {
	int destination = car->GetNextRoadID();
	TurningDirection carDirection = cross->GetDirection(car->GetCurrentRoadID(), destination);
	for (CCar*otherCar : otherCars) {
		if (otherCar != nullptr) {
			int otherCarsDestionation = otherCar->GetNextRoadID();			
			//���ڵ����յ�ĳ���ȡ�ó������Ķ��泵��ΪĿ�ĵأ���ȥ���Ƚ�
			if (otherCarsDestionation == -1) {				
				otherCarsDestionation = cross->GetFrontRoadID(otherCar->GetCurrentRoadID());
			}
			//Ŀ�ĵ���ͬʱ���ȽϷ���,
			if (destination == otherCarsDestionation) {
				//��������ת��ȼ��ȵ�ǰ���ĸ㣬����false,�����Ƿ������е�
				if (cross->GetDirection(otherCar->GetCurrentRoadID(), otherCarsDestionation) < carDirection) {
					return true;
				}
			}
		}
	}
	return false;
}
