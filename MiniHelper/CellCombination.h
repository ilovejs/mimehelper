#ifndef _CELLCOMBINATION_H
#define _CELLCOMBINATION_H

#include <vector>

using namespace std;


class CellCombination{
public:
	CellCombination(int mineCount,pair<int,int> pCell):totalMineCount(mineCount),
	parentCell(pCell){

	}
	CellCombination(){
	}
	//friend bool operator==( CellCombination& c1,  CellCombination& c2){
	//	if(c1.getVecCells() == c2.getVecCells()){
	//		return true;
	//	}else{
	//		return false;
	//	}
	//}
	//CellCombination(const CellCombination& c){
	//	vecCells = vector<pair<int,int> >(c.getVecCells());
	//	
	//}
	void addCell(int x, int y){
		pair<int,int> newOne = make_pair(x, y);
		vecCells.push_back(newOne);
	}
	int getSize()const{
		return vecCells.size();
	}
	int getTotalMineCount()const{
		return totalMineCount;
	}
	void setTotalMineCount(int mineCount){
		totalMineCount = mineCount;
	}
	pair<int,int> getParentCell()const{
		return parentCell;
	}
	bool isNearMe(CellCombination& other){
		if(abs(parentCell.first - other.getParentCell().first) > 2 ||
			abs(parentCell.second - other.getParentCell().second > 2)){
			return false;
		}else{
			return true;
		}
	}

	vector<pair<int,int> >& getVecCells(){
		return vecCells;
	}
private:
	vector<pair<int,int> > vecCells;
	int totalMineCount;
	pair<int,int> parentCell;
};


#endif