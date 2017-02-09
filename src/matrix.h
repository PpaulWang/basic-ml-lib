#include <cstring>
#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;

namespace wcq{
	int memory_blocks_num=0;	
}

using namespace wcq;

template < typename Dtype >
class Matrix{
	
public:
	Matrix(){
		data_ptr=NULL;
	}
	Matrix(int _row, int _column):row(_row),column(_column){

		data_ptr=(Dtype*)malloc(sizeof(Dtype)*row*column);
		for(int i = 0; i < row ; i++){
			for(int j = 0; j < column ; j++){
				data_ptr[offset(i, j)] = (Dtype)(rand()%(32767))/32767;
			}
		}
		memory_blocks_num++;
	}
	Matrix(int _row, int _column, Dtype val):row(_row),column(_column){
		data_ptr=(Dtype*)malloc(sizeof(Dtype)*row*column);
		for(int i = 0; i < row ; i++){
			for(int j = 0; j < column ; j++){
				data_ptr[offset(i,j)] = val;
			}
		}
		memory_blocks_num++;
	}
	Matrix(const Matrix& source){

		this->row = source.row;
		this->column = source.column;
		int __memsize = sizeof(Dtype)*this->row*this->column;
		this->data_ptr = (Dtype*)malloc(__memsize);
		memcpy(data_ptr, source.data_ptr, __memsize);
		memory_blocks_num++;
	}

	void operator = (const Matrix& source){

		if(NULL != data_ptr)
			free(data_ptr);
		
		memory_blocks_num++;

		this->row = source.row;
		this->column = source.column;
		int __memsize = sizeof(Dtype)*this->row*this->column;
		this->data_ptr = (Dtype*)malloc(__memsize);
		memcpy(data_ptr, source.data_ptr, __memsize);

	}

	~Matrix(){

		if(NULL == data_ptr) return ;
		free(data_ptr);
		//memory_blocks_num--;
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

	Matrix operator + (const Dtype val) const {
		Matrix A(this->row,this->column,val);
		return *this+A;
	}


	Matrix operator - (const Dtype val) const {
		Matrix A(this->row,this->column,val);
		return *this-A;
	}


	Matrix operator * (const Dtype val) const {
		Matrix A(this->row,this->column,val);
		return *this*A;
	}


	Matrix operator / (const Dtype val) const {
		Matrix A(this->row,this->column,val);
		return *this/A;
	}

	Matrix operator + (const Matrix& A) const {
		Matrix ret(this->row, this->column);

		if(this->column != A.column || this->row != A.row){
			//log_error()
			return ret;
		}

		for (int i = 0;i < this -> row; i ++) {
			for (int j = 0 ;j < A.column ; j++) {
				Dtype tdata = this->data_at(i,j);
				Dtype adata = A.data_at(i,j);
				Dtype& rdata = ret.data_at(i,j);
				rdata = tdata + adata;
			}
		}
		return ret;
	}

	Matrix operator - (const Matrix& A) const {
		Matrix ret(this->row,this->column);

		if(this->column != A.column || this->row != A.row){
			//log_error()
			return ret;
		}

		for (int i = 0;i < this -> row; i ++) {
			for (int j = 0 ;j < A.column ; j++) {
				Dtype tdata = this->data_at(i,j);
				Dtype adata = A.data_at(i,j);
				Dtype& rdata = ret.data_at(i,j);
				rdata = tdata - adata;
			}
		}
		return ret;
	}

	Matrix operator * (const Matrix& A) const {
		Matrix ret(this->row,this->column,-1);

		if(this->column != A.column || this->row != A.row){
			//log_error()
			return ret;
		}

		for (int i = 0;i < this -> row; i ++) {
			for (int j = 0 ;j < A.column ; j++) {
				Dtype tdata = this->data_at(i,j);
				Dtype adata = A.data_at(i,j);
				Dtype& rdata = ret.data_at(i,j);
				rdata = tdata * adata;
			}
		}
		return ret;
	}
	void operator += (const Matrix & A){
		for (int i = 0; i < this->row ; i++){
			for (int j = 0; j < this->column ; j++){
				(*this)[i][j] += A[i][j];
			}
		}
	}
	Matrix operator / (const Matrix& A) const {
		Matrix ret(this->row,this->column);

		if(this->column != A.column || this->row != A.row){
			//log_error()
			return ret;
		}

		for (int i = 0;i < this -> row; i ++) {
			for (int j = 0 ;j < A.column ; j++) {
				Dtype tdata = this->data_at(i,j);
				Dtype adata = A.data_at(i,j);
				Dtype& rdata = ret.data_at(i,j);
				rdata = tdata / adata;
			}
		}

		return ret;

	}

	Matrix dot(const Matrix& A) const {

		Matrix ret(this->row,A.column,0);

		if(this->column!=A.row){
			//log_error()
			return ret;
		}
		for (int k = 0; k < this->column ; k++) {
			for (int i = 0;i < this -> row; i ++) {
				for (int j = 0 ;j < A.column ; j++) {
					Dtype tdata = this->data_at(i,k);
					Dtype adata = A.data_at(k,j);
					Dtype& rdata = ret.data_at(i,j);
					rdata += tdata * adata;
				}
			}
		}
		return ret;
	}

	Dtype* operator [](int _row) const {
		if (valid(_row,column) == false ) {
			//log_error()
			return data_ptr;
		}
		return data_ptr+_row*column;
	}

	Dtype& data_at(int _row,int _column) const {
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
				cout<<data_ptr[offset(i,j)]<<" ";
			}cout<<endl;
		}
		puts("-------------------------------------------");
	}
	Dtype mean(){
		Dtype ret=0;
		for(int i=0;i<row;i++){
			for(int j=0;j<column;j++){
				ret+=(*this)[i][j];
			}
		}
		return ret/(row*column);
	}

	int Row()const {
		return row;
	}
	int Column()const {
		return column;
	}
	

	

private:
	int row,column;
	Dtype* data_ptr;
};
