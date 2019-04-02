## 主要思路
* 判题器按照官方给的伪代码实现，由于在判题器上花了太多的时间，最后解死锁地方的动态规划没有完成。只能借用一辆一辆跑的方法，。。。
初赛图1最多能够跑3000辆车，用时3012.图2最多能够跑1370辆车，用时4564，两次调度时间和线上的成绩一样。在往上走就会发生死锁.所以我们的成绩也就到这里了。
* 路径规划先使用了Floyd求解出所有最短路径，用三层数组装起来，直接取用。这样可以很大程度的提高程序的运行的速度，我们初赛的程序运行时间是3954，基本可以解决超时问题，对于会调参的大佬再也不怕时间不够用了。
## 模块介绍
* Car类：存储车辆相关的参数和方法
* Road类：存储道路相关参数以及路上的车辆信息
* Cross类：存储道路节点相关的方法和参数，以及相连道路信息
* Graph类：以Cross为基础，使用十字链表建图。TrafficRules函数里边是判题器的具体实现

### 判题器伪代码
* 伪代码仅参考，以TrafficRules函数具体实现为准，最终源码对于终点判断部分的伪代码进行了一些修改
```cpp
//第一步具体伪代码,车辆限速：V1;道路限速:V2
//---driveAllCarToEnd()---
//1、将路口的车标记为等待状态
//2、路里边前方无阻挡可行使最大车速的车，行驶最大车速，并记作终止状态
//3、前方有终止状态的车阻挡（s<min(V1,V2)），则将该车行驶s，然后设置为终止状态
//4、前方有等待状态的车阻挡(s<min(V1,V2)),则该车设置为等待状态
for(crossRoad in allCross){
  //遍历每个路口连接的入口（或者出口）道路
  //每条路上升序遍历每条车道，直到所有车道遍历完
    for(road in crossRoad){
      for(channel in road){
        road.driveAllCarToEnd(channel);
    }
  }
}

//第二步的具体伪代码
lastWaitingCarNumber = 0;
while(allCarsNotWaiting == False){
	//遍历每个处于等待的道路节点
	for(crossRoad in allCross（升序）){
		//遍历每个入口道路
		for(entranceRoad in crossRoad(按照道路ID升序)){
			while(entranceRoad.allCarNotWaiting){
				//得到另外路口的头车，后面通过车辆方向和终点，用于判断优先级
				firsdPriorityCars=[];			
				for(otherEntranceRoad in crossRoad){
					firsdPriorityCars.append(otherEntranceRoad.getFirstPriorityCar());
				}
				//得到第一优先级的车辆
				car = entranceRoad.getFirstPriorityCar();
				//判断是否会冲突
				if conflict(cross,firsdPriorityCars,car)
					break;
				//判断是否到达终点
				if(car.nextRoadID.isDestination){
					Result.append(car.information);
					entranceRoad.driveOut(car);
					entranceRoad.driveAllCarToEnd(car.channel);
				}else{
					//判断出口道路是否有等待车辆阻挡
					exitRoad = crossRoad.getExitRoad(car.nextRoadID);
					if(exitRoad.couldDirveIn(car)){
						//S1:出口车道该走的距离，S2：入口车道该走的距离
						if(car.S2 == 0){
							//所有车道都塞满的时候，就走到出口的最前面,不变道
							car.runToEnd();
							entranceRoad.driveAllCarToEnd(car.channel);
						}else{
							//可以驶入，让车驶出出口，驶入入口，并更新出口车道所有车的行驶状态
							//入口前面有终止状态车的阻挡，就将车接到该车后面，直到入口车道塞满，再进入下个车道
							entranceRoad.driveOut(car);
							entranceRoad.driveAllCarToEnd(car.channel);
							exitRoad.dirveIn(car);
						}
					}else{
						//不能进入，就表示出口的车是等待状态，就直接遍历下条道路
						break;
					}
				}
			}
		}
	}
	//每次遍历完所有节点之后，检查所有节点处于等待车的数量
	currentWiatingCarNumber = allCross.getWaitingCarNumber();
	if(currentWiatingCarNumber == lastWaitingCarNumber){
		//发生死锁
		throw deadLock;
	}else{
		lastWaitingCarNumber = currentWiatingCarNumber;
	}
}

//发车伪代码
//保证路上有500辆车要出发
while(allCross.sumRunningCarNumber() < 500){
	car = Garage.getNextCar();
	road = Cross.getRoad(car.nextRoadID);
	road.driveIn(car);	//这里要保证路口不堵车
}

//终点判断伪代码
//路上和车库都没有车
if(allCross.sumRunningCarNumber == 0 && sumGarageCarNumber == 0){
	//完成循环
	break;
```


