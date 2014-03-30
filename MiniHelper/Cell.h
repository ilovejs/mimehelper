#ifndef _CELL_H_
#define _CELL_H_

#include <assert.h>
#include "Constants.h"


//! cell class
class Cell{
private:
	int m_x;
	int m_y;
	CellState m_state;
public:
	Cell(int x, int y):m_x(x),m_y(y),m_state(Unknow){}
	int getX()const{ return m_x;}
	int getY()const{ return m_y;}
	CellState getState()const{ return m_state;}
	void setState(const CellState state){
		m_state = state;
	}
};


#endif