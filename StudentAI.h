#ifndef STUDENTAI_H
#define STUDENTAI_H
#include "AI.h"
#include <math.h>
#include <limits.h>

#pragma once


//The following part should be completed by students.
//Students can modify anything except the class name and exisiting functions and varibles.
class StudentAI :public AI
{
public:
	StudentAI(int col, int row, int k, int g);
	int number;
	vector<vector<int> > b;
	vector<int> alpha;
	vector<int> beta;
	int scoreFunction(vector<vector<int> > b);
	int chooseMove(vector<vector<int> > _board, int &iPos, int &jPos, int count, int depth);
	int chooseMoveG(vector<vector<int> > _board, int &iPos, int &jPos, int count, int depth);
	virtual Move GetMove(Move board);
};

#endif //STUDENTAI_H