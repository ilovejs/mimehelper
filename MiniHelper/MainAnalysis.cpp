#include "stdafx.h"
#include "MainAnalysis.h"
#include <algorithm>
#include <assert.h>

vector<Cell*> MainAnalysis::firstCheck(int x, int y, ChangeFlag& changeFlag){
	vector<Cell*> resultClickCells;

	Cell* currentCell;
	Cell* aroundCell;
	currentCell = gameMap.GetValue(x, y);
	if(currentCell->getState() == Unknow || currentCell->getState() == Flag0){
		return resultClickCells;
	}
	//the cell show how many mine around it
	int totalMineCount = currentCell->getState();
	int totalUnKnowCount = 0;
	int alreadyFindMineCount = 0;
	int aroundX,aroundY;
	//first find unknow cell count 
	for(int i = 0; i < 8; ++i){
		aroundX = x + DIRECTION[i][0];
		aroundY = y + DIRECTION[i][1];
		if(gameMap.IsInArray(aroundX, aroundY)){
			aroundCell = gameMap.GetValue(aroundX, aroundY);
			if(aroundCell->getState() == Unknow){
				++totalUnKnowCount;
			}else if(aroundCell->getState() == IsMine){
				++alreadyFindMineCount;
			}
		}
	}
	int doneCount = 0;
	// change all flag to mine
	if((totalMineCount - alreadyFindMineCount) == totalUnKnowCount){
		for(int i = 0; i < 8; ++i){
			aroundX = x + DIRECTION[i][0];
			aroundY = y + DIRECTION[i][1];
			if(gameMap.IsInArray(aroundX, aroundY)){
				aroundCell = gameMap.GetValue(aroundX, aroundY);
				if(aroundCell->getState() == Unknow){
					changeFlag.MineChangeFlag = true;
					aroundCell->setState(IsMine);
					doneCount++;
					if(doneCount == totalMineCount){
						return resultClickCells;
					}
				}
			}
		}
	}else if(totalMineCount == alreadyFindMineCount){
		for(int i = 0; i < 8; ++i){
			aroundX = x + DIRECTION[i][0];
			aroundY = y + DIRECTION[i][1];
			if(gameMap.IsInArray(aroundX, aroundY)){
				aroundCell = gameMap.GetValue(aroundX, aroundY);
				if(aroundCell->getState() == Unknow){
					changeFlag.HaveClickResultFlag = true;
					resultClickCells.push_back(aroundCell);
					doneCount++;
					if(doneCount == totalMineCount){
						return resultClickCells;
					}
				}
			}
		}
	}else if((totalMineCount - alreadyFindMineCount) < totalUnKnowCount){
		CellCombination newCombination(totalMineCount - alreadyFindMineCount,
			make_pair(currentCell->getX(),currentCell->getY()));
		for(int i = 0; i < 8; ++i){
			aroundX = x + DIRECTION[i][0];
			aroundY = y + DIRECTION[i][1];
			if(gameMap.IsInArray(aroundX, aroundY)){
				aroundCell = gameMap.GetValue(aroundX, aroundY);
				if(aroundCell->getState() == Unknow){
					newCombination.addCell(aroundX, aroundY);
				}
			}
		}
		totalCombination.push_back(newCombination);
	}
	return resultClickCells;
}

vector<Cell*> MainAnalysis::doSubThing(vector<int>& vecInt,ChangeFlag& changeFlag){
	vector<Cell*> resultClickCells;
	int firstIndex = 0;
	int secondIndex = 0;
	bool findFirst = false;
	//find the two combination index to sub
	for(int i = 0; i < vecInt.size(); ++i){  
		if(vecInt[i] == 1){
			if(findFirst == false){
				firstIndex = i;
				findFirst = true;
			}else if(findFirst == true){
				secondIndex = i;
				break;
			}
		}
	}
	resultClickCells = doSubThing(firstIndex, secondIndex, changeFlag);

	return resultClickCells;
}


vector<Cell*> MainAnalysis::doSubThing(int firstIndex,int secondIndex,ChangeFlag& changeFlag){
	vector<Cell*> resultClickCells;
	if(totalCombination[firstIndex].isNearMe(totalCombination[secondIndex]) &&
		totalCombination[firstIndex].getSize() != totalCombination[secondIndex].getSize()){
			//init a new list
			CellCombination baseOne;
			CellCombination otherOne;
			if(totalCombination[firstIndex].getSize() < totalCombination[secondIndex].getSize()){
				int tmp = secondIndex;
				secondIndex = firstIndex;
				firstIndex = tmp;
			}
			baseOne = CellCombination(totalCombination[firstIndex]);
			otherOne = CellCombination(totalCombination[secondIndex]);
			auto itr = baseOne.getVecCells().begin();

			for(int i = 0; i < otherOne.getSize(); ++i){
				//find one remove one
				itr = find(baseOne.getVecCells().begin(), 
					baseOne.getVecCells().end(), otherOne.getVecCells()[i]);
				if(itr != baseOne.getVecCells().end()){
					//remove
					baseOne.getVecCells().erase(itr);
				}else{
					//if can not find something means otherOne is not a sub combination of baseOne
					//so just return
					return resultClickCells;
				}
			}
			if(baseOne.getSize() == 0)return resultClickCells;

			//after sub
			int subMineCount = baseOne.getTotalMineCount() - otherOne.getTotalMineCount();
			//all left is mine,x1 + x2 + x3 = 1
			//                 x1 + x2 = 0
			//               so x3 = 1, x3 is mine
			if(baseOne.getSize() == subMineCount){
				for(int i = 0; i < baseOne.getSize(); ++i){
					gameMap.GetValue(baseOne.getVecCells()[i].first,
						baseOne.getVecCells()[i].second)->setState(IsMine);
				}
				changeFlag.MineChangeFlag = true;
			}
			//all left is not mine, x1 + x2 + x3 + x4 = 2
			//                      x1 + x2 + x3 = 2
			//                    so x4 = 0, x4 is not a mine
			else if(subMineCount == 0){
				for(int i = 0; i < baseOne.getSize(); ++i){
					Cell* cell = gameMap.GetValue(baseOne.getVecCells()[i].first,
						baseOne.getVecCells()[i].second);
					resultClickCells.push_back(cell);
				}
				changeFlag.HaveClickResultFlag = true;
			}
			//new combination, x1 + x2 + x3 + x4 = 2
			//                 x1 + x2 = 1
			//              so x3 + x4 = 1
			else if(baseOne.getSize() > subMineCount){
				baseOne.setTotalMineCount(subMineCount);
				bool alreadyExist = false;
				for(int i = 0; i < totalCombination.size(); ++i){
					if(totalCombination[i].getVecCells() == baseOne.getVecCells()){
						alreadyExist = true;
						break;
					}
				}
				if(alreadyExist == false){
					totalCombination.push_back(baseOne);
					changeFlag.HaveNewCombinationFlag = true;
				}
			}
	}

	return resultClickCells;
} 

bool compareCombo(CellCombination& c1, CellCombination& c2){
	if(c1.getSize() < c2.getSize()){
		return true;
	}else{
		return false;
	}
}

vector<Cell*> MainAnalysis::analysisCombinations(ChangeFlag& changeFlag){
	vector<Cell*> resultClickCells;
	sort(totalCombination.begin(), totalCombination.end(), compareCombo);
	for(int i = totalCombination.size() - 1; i >=1; --i){
		if(totalCombination[i].getSize() > 2){
			for(int j = i - 1; j >= 0; --j){
				resultClickCells = doSubThing(i,j, changeFlag);
				if(resultClickCells.size() > 0){
					return resultClickCells;
				}
			}
		}else{
			break;
		}
	}

	return resultClickCells;



	//initial first combination like:1,1,0,0,0
	//int total = totalCombination.size();
	//vector<Cell*> resultClickCells;
	//if(total < 2) return resultClickCells;

	//
	//vector<int> vecInt(total,0);  
	//for(int i = 0; i < 2; ++i){  
	//	vecInt[i] = 1;  
	//}  
	////find one just return
	//resultClickCells = doSubThing(vecInt, changeFlag);
	//if(resultClickCells.size() > 0){
	//	return resultClickCells;
	//}

	//for(int i = 0; i < total - 1; ++i){  
	//	if(vecInt[i] == 1 && vecInt[i+1] == 0){  
	//		//1. first exchange 1 and 0 to 0 1  
	//		swap(vecInt[i], vecInt[i+1]);  

	//		//2.move all 1 before vecInt[i] to left  
	//		sort(vecInt.begin(),vecInt.begin() + i,MainAnalysis::compare);  

	//		//after step 1 and 2, a new combination is exist
	//		
	//		resultClickCells = doSubThing(vecInt, changeFlag);
	//		//find one just return
	//		if(resultClickCells.size() > 0){
	//			return resultClickCells;
	//		}

	//		//try do step 1 and 2 from front  
	//		i = -1;  
	//	}  
	//}  
	//return resultClickCells;
}  