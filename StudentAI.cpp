#include "StudentAI.h"
#include <stdlib.h>

#define DEPTH 3

//The following part should be completed by students.
//The students can modify anything except the class name and exisiting functions and varibles.
StudentAI::StudentAI(int col,int row,int k,int g)
	:AI(col, row, k, g)
{
	b = vector<vector<int> >(row, vector<int>(col, 0));
	alpha = vector<int> (DEPTH+1, INT_MIN);
	beta = vector<int> (DEPTH+1, INT_MAX);
	number = 0;
}



Move StudentAI::GetMove(Move move)
{

	int c=0 ,r=0;
	for(;r<row;r++) {
		for(;c<col;c++) {
			if(b[r][c] == 0) break;
		}
	}
	if (this->g == 0) {
		if(!(move.row == -1 && move.col == -1)) 
			b[move.row][move.col] = 1;
		if(number == 0) {
			number++;
			if(b[row/2][col/2] != 0) {
				b[row/2][col/2-1] = 1;
				return Move(col/2-1, row/2);
			}
			else {
				b[row/2][col/2] = 1;
				return Move(col/2, row/2);
			}
		}
		
		chooseMove(b, r, c, 1, DEPTH);
		b[r][c] = 2;
		return Move(c, r);
	} else {
        int i;
        for(i=row-1;i>=0;i--) {
            if(b[i][move.col] == 0) break;
        }
        if(move.col != -1)
			b[i][move.col] = 1;
		if(number == 0) {
			number++;
			if(b[row-1][col/2] != 0) {
				b[row-1][col/2-1] = 1;
				return Move(col/2-1, row-1);
			}
			else {
				b[row-1][col/2] = 1;
				return Move(col/2, row-1);
			}
		}

		chooseMoveG(b, r, c, 1, DEPTH);
        cout << "player 2: " << "row is " << r << " col is " << c << endl;
		b[r][c] = 2;
		return Move(c, r);
	}
}


bool checkBoard(vector<vector<int> >& board) {
	for(int i=0;i<board.size();i++) {
		for(int j=0;j<board[0].size();j++) {
			if(board[i][j] == 0) return false;
		}
	}
	return true;
}

// get alpha&beta from parent nodes -> dfs to get score -> check score and alpha&beta -> update res -> if root node, update chess position.

int StudentAI::chooseMove(vector<vector<int> > _board, int &iPos, int &jPos, int count, int depth) {
	int res;
	int tmp;
	bool flag = checkBoard(_board);
	alpha[count] = alpha[count-1];
	beta[count] = beta[count-1];
	// maximizer
	if(count%2 == 1) {
		if(flag == true) {
			tmp = scoreFunction(_board);
			// if larger than beta
			if(tmp >= beta[count]) return tmp;
			// update alpha
			if(tmp > alpha[count]) alpha[count] = tmp;
			return tmp;
		}
		res = INT_MIN;
		for(int i=0;i<row;i++) {
			for(int j=0;j<col;j++) {
				if(_board[i][j] != 0) continue;

				_board[i][j] = 2;
				if(count == depth) {
					tmp = scoreFunction(_board);
				} else {
					tmp = chooseMove(_board, iPos, jPos, count+1, depth);
				}

				// if larger than beta
				if(tmp >= beta[count]) return tmp;
				// update alpha
				if(tmp > alpha[count]) alpha[count] = tmp;

				if(tmp > res) {
					res = tmp;
					if(count == 1) {
						iPos = i;
						jPos = j;
					}
				}
				_board[i][j] = 0;
			}
		}
	}
	// minimizer
	else if(count%2 == 0) {
		if(flag == true) {
			tmp = scoreFunction(_board);
			// if smaller than alpha
			if(tmp <= alpha[count]) return tmp;
			// update beta
			if(tmp < beta[count]) beta[count] = tmp;
			return tmp;
		}

		res = INT_MAX;

		for(int i=0;i<row;i++) {
			for(int j=0;j<col;j++) {
				if(_board[i][j] != 0) continue;

				_board[i][j] = 1;
				if(count == depth) {
					tmp = scoreFunction(_board);
				} else {
					tmp = chooseMove(_board, iPos, jPos, count+1, depth);
				}

				// if smaller than alpha
				if(tmp <= alpha[count]) return tmp;
				// update beta
				if(tmp < beta[count]) beta[count] = tmp;

				if(tmp < res) {
					res = tmp;
				}
				_board[i][j] = 0;
			}
		}
	}
	return res;
}

int StudentAI::chooseMoveG(vector<vector<int> > _board, int &iPos, int &jPos, int count, int depth) {
	int res;
	int tmp;
	int i;
	alpha[count] = alpha[count-1];
	beta[count] = beta[count-1];
	// maximizer
	if(count%2 == 1) {
		res = INT_MIN;
		// for(int i=0;i<row;i++) {
			for(int j=0;j<col;j++) {
				if(_board[0][j] != 0) continue;

                for(i=row-1;i>=0;i--) {
                    if(_board[i][j] == 0) break;
                }

				_board[i][j] = 2;
				if(count == depth) {
					tmp = scoreFunction(_board);
				} else {
					tmp = chooseMoveG(_board, iPos, jPos, count+1, depth);
				}

				// if larger than beta
				if(tmp >= beta[count]) return tmp;

				// update alpha
				if(tmp > alpha[count]) alpha[count] = tmp;

				if(tmp > res) {
					res = tmp;
					if(count == 1) {
						iPos = i;
						jPos = j;
					}
				}
				_board[i][j] = 0;
			}
		// }
	}
	// minimizer
	else if(count%2 == 0) {
		res = INT_MAX;
		// for(int i=0;i<row;i++) {
			for(int j=0;j<col;j++) {
				if(_board[0][j] != 0) continue;

                for(i=row-1;i>=0;i--) {
                    if(_board[i][j] == 0) break;
                }
                
				_board[i][j] = 1;
				if(count == depth) {
					tmp = scoreFunction(_board);
				} else {
					tmp = chooseMove(_board, iPos, jPos, count+1, depth);
				}

				// if smaller than alpha
				if(tmp <= alpha[count]) return tmp;

				// update beta
				if(tmp < beta[count]) beta[count] = tmp;

				if(tmp < res) {
					res = tmp;
				}
				_board[i][j] = 0;
			}
		// }
	}
	return res;
}



int StudentAI::scoreFunction(vector<vector<int> > b) {
	int res2 = 0;
	int res1 = 0;
	int connected = 0;
	//res2
	//row
	for(int i=0;i<row;i++) {
		connected = 0;
		for(int j=0;j<col;j++) {
			if(b[i][j] == 2) {
				if(j > 0 && b[i][j-1] != 1) connected++;
                if(j == col-1) {
                    connected--;
                    res2 += pow(20, connected)==1? 0 : pow(20, connected);
                    if(connected == k) res2 *= 200;
				    connected = 0;
                }
			} else if(b[i][j] == 1) {
				connected--;
				res2 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res2 *= 200;
				connected = 0;
			} else {
				res2 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res2 *= 200;
				connected = 0;
			}
		}
	}

	//col
	for(int j=0;j<col;j++) {
		connected = 0;
		for(int i=0;i<row;i++) {
			if(b[i][j] == 2) {
				if(i > 0 && b[i-1][j] != 1) connected++;
                if(i == row-1) {
                    connected--;
                    res2 += pow(20, connected)==1? 0 : pow(20, connected);
                    if(connected == k) res2 *= 200;
				    connected = 0;
                }
			} else if(b[i][j] == 1) {
				connected--;
				res2 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res2 *= 200;
				connected = 0;
			} else {
				res2 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res2 *= 200;
				connected = 0;
			}
		}
	}
	//tr to bl
	for(int iRow=k;iRow<row;iRow++) {
		connected = 0;
		for(int i=iRow,j=0;i>=0;i--,j++) {
			if(b[i][j] == 2) {
				if(j>0 && b[i+1][j-1] != 1) connected++;
				if(i==0) {
					connected--;
					res2 += pow(20, connected)==1? 0 : pow(20, connected);
					if(connected == k) res2 *= 200;
				    connected = 0;
				}
			} else if(b[i][j] == 1) {
				connected--;
				res2 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res2 *= 200;
				connected = 0;
			} else {
				res2 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res2 *= 200;
				connected = 0;
			}
		}
	}

	for(int jCol=1;jCol<=col-k;jCol++) {
		connected = 0;
		for(int i=row-1,j=jCol;j<col;i--,j++) {
			if(b[i][j] == 2) {
				if(i<row-1 && b[i+1][j-1] != 1) connected++;
				if(j==col-1) {
					connected--;
					res2 += pow(20, connected)==1? 0 : pow(20, connected);
					if(connected == k) res2 *= 200;
				    connected = 0;
				}
			} else if(b[i][j] == 1) {
				connected--;
				res2 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res2 *= 200;
				connected = 0;
			} else {
				res2 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res2 *= 200;
				connected = 0;
			}
		}
	}


	//tl to br
	for(int iRow=0;iRow<=row-k;iRow++) {
		connected = 0;
		for(int i=iRow,j=0;i<row;i++,j++) {
			if(b[i][j] == 2) {
				if(j>0 && b[i-1][j-1] != 1) connected++;
				if(i==row-1) {
					connected--;
					res2 += pow(20, connected)==1? 0 : pow(20, connected);
					if(connected == k) res2 *= 200;
				    connected = 0;
				}
			} else if(b[i][j] == 1) {
				connected--;
				res2 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res2 *= 200;
				connected = 0;
			} else {
				res2 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res2 *= 200;
				connected = 0;
			}
		}
	}

	for(int jCol=1;jCol<=col-k;jCol++) {
		connected = 0;
		for(int i=0,j=jCol;j<col;i++,j++) {
			if(b[i][j] == 2) {
				if(i>0 && b[i-1][j-1] != 1) connected++;
				if(j==col-1) {
					connected--;
					res2 += pow(20, connected)==1? 0 : pow(20, connected);
					if(connected == k) res2 *= 200;
				    connected = 0;
				}
			} else if(b[i][j] == 1) {
				connected--;
				res2 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res2 *= 200;
				connected = 0;
			} else {
				res2 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res2 *= 200;
				connected = 0;
			}
		}
	}




	//res1
	//row
	for(int i=0;i<row;i++) {
		connected = 0;
		for(int j=0;j<col;j++) {
			if(b[i][j] == 1) {
				if(j > 0 && b[i][j-1] != 2) connected++;
                if(j == col-1) {
                    connected--;
                    res1 += pow(20, connected)==1? 0 : pow(20, connected);
                    if(connected == k) res1 *= 200;
				    connected = 0;
                }
			} else if(b[i][j] == 2) {
				connected--;
				res1 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res1 *= 200;
				connected = 0;
			} else {
				res1 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res1 *= 200;
				connected = 0;
			}
		}
	}

	//col
	for(int j=0;j<col;j++) {
		connected = 0;
		for(int i=0;i<row;i++) {
			if(b[i][j] == 1) {
				if(i > 0 && b[i-1][j] != 2) connected++;
                if(i == row-1) {
                    connected--;
                    res1 += pow(20, connected)==1? 0 : pow(20, connected);
                    if(connected == k) res1 *= 200;
				    connected = 0;
                }
			} else if(b[i][j] == 2) {
				connected--;
				res1 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res1 *= 200;
				connected = 0;
			} else {
				res1 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res1 *= 200;
				connected = 0;
			}
		}
	}
	//tr to bl
	for(int iRow=k;iRow<row;iRow++) {
		connected = 0;
		for(int i=iRow,j=0;i>=0;i--,j++) {
			if(b[i][j] == 1) {
				if(j>0 && b[i+1][j-1] != 2) connected++;
				if(i==0) {
					connected--;
					res1 += pow(20, connected)==1? 0 : pow(20, connected);
					if(connected == k) res1 *= 200;
				    connected = 0;
				}
			} else if(b[i][j] == 2) {
				connected--;
				res1 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res1 *= 200;
				connected = 0;
			} else {
				res1 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res1 *= 200;
				connected = 0;
			}
		}
	}

	for(int jCol=1;jCol<=col-k;jCol++) {
		connected = 0;
		for(int i=row-1,j=jCol;j<col;i--,j++) {
			if(b[i][j] == 1) {
				if(i<row-1 && b[i+1][j-1] != 2) connected++;
				if(j==col-1) {
					connected--;
					res1 += pow(20, connected)==1? 0 : pow(20, connected);
					if(connected == k) res1 *= 200;
				    connected = 0;
				}
			} else if(b[i][j] == 2) {
				connected--;
				res1 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res1 *= 200;
				connected = 0;
			} else {
				res1 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res1 *= 200;
				connected = 0;
			}
		}
	}


	//tl to br
	for(int iRow=0;iRow<=row-k;iRow++) {
		connected = 0;
		for(int i=iRow,j=0;i<row;i++,j++) {
			if(b[i][j] == 1) {
				if(j>0 && b[i-1][j-1] != 2) connected++;
				if(i==row-1) {
					connected--;
					res1 += pow(20, connected)==1? 0 : pow(20, connected);
					if(connected == k) res1 *= 200;
				    connected = 0;
				}
			} else if(b[i][j] == 1) {
				connected--;
				res1 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res1 *= 200;
				connected = 0;
			} else {
				res1 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res1 *= 200;
				connected = 0;
			}
		}
	}

	for(int jCol=1;jCol<=col-k;jCol++) {
		connected = 0;
		for(int i=0,j=jCol;j<col;i++,j++) {
			if(b[i][j] == 1) {
				if(i>0 && b[i-1][j-1] != 2) connected++;
				if(j==col-1) {
					connected--;
					res1 += pow(20, connected)==1? 0 : pow(20, connected);
					if(connected == k) res1 *= 200;
				    connected = 0;
				}
			} else if(b[i][j] == 1) {
				connected--;
				res1 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res1 *= 200;
				connected = 0;
			} else {
				res1 += pow(20, connected)==1? 0 : pow(20, connected);
				if(connected == k) res1 *= 200;
				connected = 0;
			}
		}
	}

	return res2 - res1;
}


