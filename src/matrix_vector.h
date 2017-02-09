#include <cstring>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>
using namespace std;


template < typename Dtype >
class Matrix{
	
public:
	Matrix(){

	}
	Matrix(int _row, int _column):row(_row),column(_column){

		data_ptr.resize(row*column);
		for(int i = 0; i < row ; i++){
			for(int j = 0; j < column ; j++){
				data_ptr[offset(i, j)] = (Dtype)(rand()%(32767))/32767;
			}
		}

	}
	Matrix(const Matrix& A){
		(*this).row=A.row;
		(*this).column=A.column;
		(*this).data_ptr=A.data_ptr;
	}

	Matrix(int _row, int _column, Dtype val):row(_row),column(_column){
		data_ptr.resize(row*column);
		for(int i = 0; i < row ; i++){
			for(int j = 0; j < column ; j++){
				data_ptr[offset(i,j)] = val;
			}
		}

	}


	int offset(int index_row,int index_column) const {
		return index_row * column + index_column;
	}

	Matrix T() const {
		Matrix ret(this->column,this->row);

		for (int i=0 ;i < this->row ; ++i ){
			for(int j = 0;j < this->column ; ++j ){
				ret[j][i] = (*this)[i][j];
			}
		}

		return ret;

	}

	Matrix process(Dtype func(Dtype)) const {
		Matrix ret(this->row,this->column,0);
		for(int i = 0;i < this->row; i++){
			for(int j = 0;j < this->column; j++){
				ret[i][j] = func((*this)[i][j]);
			}
		}
		return ret;
	}

	Matrix operator + (Dtype val) const {
		Matrix A(this->row,this->column,val);
		return *this+A;
	}


	Matrix operator - (Dtype val) const {
		Matrix A(this->row,this->column,val);
		return *this-A;
	}


	Matrix operator * (Dtype val) const {
		Matrix A(this->row,this->column,val);
		return *this*A;
	}


	Matrix operator / (Dtype val) const {
		Matrix A(this->row,this->column,val);
		return *this/A;
	}

	Matrix operator + (const Matrix A) const {
		Matrix ret(this->row, this->column);

		if(this->column != A.column || this->row != A.row){
			//log_error()
			return ret;
		}

		for (int i = 0;i < this -> row; i ++) {
			for (int j = 0 ;j < A.column ; j++) {
				const Dtype tdata = (*this)[i][j];
				const Dtype adata = A[i][j];
				ret[i][j] = tdata + adata;
			}
		}
		return ret;
	}

	Matrix operator - (const Matrix A) const {
		Matrix ret(this->row,this->column);

		if(this->column != A.column || this->row != A.row){
			//log_error()
			return ret;
		}

		for (int i = 0;i < this -> row; i ++) {
			for (int j = 0 ;j < A.column ; j++) {
				const Dtype tdata = (*this)[i][j];
				const Dtype adata = A[i][j];
				ret[i][j] = tdata - adata;
			}
		}
		return ret;
	}

	Matrix operator * (const Matrix A) const {
		Matrix ret(this->row,this->column,-1);

		if(this->column != A.column || this->row != A.row){
			//log_error()
			return ret;
		}

		for (int i = 0;i < this -> row; i ++) {
			for (int j = 0 ;j < A.column ; j++) {
				const Dtype tdata = (*this)[i][j];
				const Dtype adata = A[i][j];
				ret[i][j] = tdata * adata;
			}
		}
		return ret;
	}

	Matrix operator / (const Matrix A) const {
		Matrix ret(this->row,this->column);

		if(this->column != A.column || this->row != A.row){
			//log_error()
			return ret;
		}

		for (int i = 0;i < this -> row; i ++) {
			for (int j = 0 ;j < A.column ; j++) {
				const Dtype tdata = (*this)[i][j];
				const Dtype adata = A[i][j];
				//Dtype& rdata = 
				ret[i][j] = tdata / adata;
			}
		}

		return ret;

	}

	Matrix dot(const Matrix A) const {

		Matrix ret(this->row,A.column,0);

		if(this->column!=A.row){
			//log_error()
			return ret;
		}
		for (int k = 0; k < this->column ; k++) {
			for (int i = 0;i < this -> row; i ++) {
				for (int j = 0 ;j < A.column ; j++) {
					const Dtype tdata = (*this)[i][k];
					const Dtype adata = A[k][j];
					ret[i][j] += tdata * adata;
				}
			}
		}
		return ret;
	}

	Dtype* operator [](int _row){
		if (valid(_row,column) == false ) {
			//log_error()
			return &(data_ptr.at(0));
		}
		return &(data_ptr.at(_row*column));
	}

	const Dtype* operator [](int _row) const {
		if (valid(_row,column) == false ) {

			return &(data_ptr.at(0));
		}
		return &(data_ptr.at(_row*column));
	}

	const Dtype& data_at(int _row,int _column) const {
		if (valid(_row,_column) == -1 ) {
			//log_error()
			return data_ptr[0];
		}
		return data_ptr[offset(_row,_column)];
	}

	bool valid(int _row,int _column) const {
		if(_row < 0) return false;
		if(_row > row) return false;
		if(_column < 0) return false;
		if(_column > column) return false;
		return true;
	}

	void show() const {
		puts("-------------------------------------------");
		for(int i=0;i<row;i++){
			for(int j=0;j<column;j++){
				cout<<(*this)[i][j]<<" ";
			}cout<<endl;
		}
		puts("-------------------------------------------");
	}
	Dtype mean(){
		Dtype ret=0;
		for(int i=0;i<row;i++){
			for(int j=0;j<column;j++){
				ret += (*this)[i][j];
			}
		}
		return ret/(row*column);
	}

	int Row()const{
		return row;
	}
	int Column()const{
		return column;
	}
	

private:
	int row,column;
	vector<Dtype> data_ptr;
};
