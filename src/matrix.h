#include <cstring>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <fstream>

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
	Matrix(int row_, int column_):row(row_),column(column_){
		data_ptr = new Dtype[(*this).row * (*this).column];
		//data_ptr=(Dtype*)malloc(sizeof(Dtype)*row*column);
		for(int i = 0; i < row ; i++){
			for(int j = 0; j < column ; j++){
				data_ptr[offset(i, j)] = (Dtype)(rand()%(32767))/32767;
			}
		}
		memory_blocks_num++;
	}
	Matrix(int row_, int column_, Dtype val):row(row_),column(column_){
		data_ptr = new Dtype[(*this).row * (*this).column];
		//data_ptr=(Dtype*)malloc(sizeof(Dtype)*row*column);
		for(int i = 0; i < row ; i++){
			for(int j = 0; j < column ; j++){
				data_ptr[offset(i,j)] = val;
			}
		}
		memory_blocks_num++;
	}
	Matrix(const Matrix& source){

		(*this).row = source.row;
		(*this).column = source.column;
		int memsize_ = sizeof(Dtype)*(*this).row*(*this).column;
		(*this).data_ptr = new Dtype[(*this).row * (*this).column];
		//(*this).data_ptr = (Dtype*)malloc(memsize_);
		memcpy(data_ptr, source.data_ptr, memsize_);
		memory_blocks_num++;
	}

	Matrix(const std::string filename){
		(*this).data_ptr = NULL ;
		(*this).load(filename);
	}
	void load(const std::string filename){
		std::ifstream fin(filename.c_str());
		fin>>(*this).row>>(*this).column;
		if ((*this).data_ptr != NULL) {
			delete[] data_ptr;
			memory_blocks_num -- ;
		}
		data_ptr = new Dtype[(*this).row * (*this).column];
		memory_blocks_num ++ ;
		for(int i = 0 ; i < (*this).row ; i++ ){
			for(int j = 0 ; j < (*this).column ; j++ ){
				fin>>(*this)[i][j];
			}
		}
	}


	void save(const std::string filename) const {
		std::ofstream fout(filename.c_str());
		fout<<(*this).row << " " << (*this).column << std::endl;
		for(int i = 0 ; i < (*this).row ; i++ ){
			for(int j = 0 ; j < (*this).column ; j++ ){
				fout << (*this)[i][j] << " ";
			}
			fout << std::endl;
		}
	}

	void operator = (const Matrix& source){

		if(NULL != data_ptr)
			free(data_ptr);
		
		memory_blocks_num++;

		(*this).row = source.row;
		(*this).column = source.column;
		int memsize_ = sizeof(Dtype)*(*this).row*(*this).column;
		(*this).data_ptr = new Dtype[(*this).row * (*this).column];
		//(*this).data_ptr = (Dtype*)malloc(memsize_);
		memcpy(data_ptr, source.data_ptr, memsize_);

	}

	~Matrix(){

		if(NULL == (*this).data_ptr) return ;
		delete[] (*this).data_ptr;
		//free((*this).data_ptr);
		(*this).data_ptr = NULL;
		memory_blocks_num--;
	}

	int offset(int indexrow_,int indexcolumn_) const {
		return indexrow_ * (*this).column + indexcolumn_;
	}

	Matrix T() const {
		Matrix ret((*this).column,(*this).row);

		for (int i=0 ;i < (*this).row ; ++i ){
			for(int j = 0;j < (*this).column ; ++j ){
				ret[j][i] = (*this)[i][j];
			}
		}

		return ret;

	}

	Matrix process(Dtype func(Dtype)) const {
		Matrix ret((*this).row , (*this).column,0);
		for(int i = 0;i < (*this).row; i++){
			for(int j = 0;j < (*this).column; j++){
				ret[i][j] = func((*this)[i][j]);
			}
		}
		return ret;
	}

	Matrix operator + (const Dtype val) const {
		Matrix A((*this).row,(*this).column,val);
		return *this+A;
	}


	Matrix operator - (const Dtype val) const {
		Matrix A((*this).row,(*this).column,val);
		return *this-A;
	}


	Matrix operator * (const Dtype val) const {
		Matrix A((*this).row,(*this).column,val);
		return *this*A;
	}


	Matrix operator / (const Dtype val) const {
		Matrix A((*this).row,(*this).column,val);
		return *this/A;
	}

	Matrix operator + (const Matrix& A) const {
		Matrix ret((*this).row, (*this).column);

		if((*this).column != A.column || (*this).row != A.row){
			//log_error()
			return ret;
		}

		for (int i = 0;i < (*this).row; i ++) {
			for (int j = 0 ;j < (*this).column ; j++) {
				Dtype tdata = (*this).data_at(i,j);
				Dtype adata = A.data_at(i,j);
				Dtype& rdata = ret.data_at(i,j);
				rdata = tdata + adata;
			}
		}
		return ret;
	}

	Matrix operator - (const Matrix& A) const {
		Matrix ret((*this).row,(*this).column);

		if((*this).column != A.column || (*this).row != A.row){
			//log_error()
			return ret;
		}

		for (int i = 0;i < (*this).row; i ++) {
			for (int j = 0 ;j < (*this).column ; j++) {
				Dtype tdata = (*this).data_at(i,j);
				Dtype adata = A.data_at(i,j);
				Dtype& rdata = ret.data_at(i,j);
				rdata = tdata - adata;
			}
		}
		return ret;
	}

	Matrix operator * (const Matrix& A) const {
		Matrix ret((*this).row,(*this).column,-1);

		if((*this).column != A.column || (*this).row != A.row){
			//log_error()
			return ret;
		}

		for (int i = 0;i < (*this).row; i ++) {
			for (int j = 0 ;j < (*this).column ; j++) {
				Dtype tdata = (*this).data_at(i,j);
				Dtype adata = A.data_at(i,j);
				Dtype& rdata = ret.data_at(i,j);
				rdata = tdata * adata;
			}
		}
		return ret;
	}

	void operator += (const Matrix & A){
		for (int i = 0; i < (*this).row ; i++){
			for (int j = 0; j < (*this).column ; j++){
				(*this)[i][j] += A[i][j];
			}
		}
	}

	Matrix operator / (const Matrix& A) const {
		Matrix ret((*this).row,(*this).column);

		if((*this).column != A.column || (*this).row != A.row){
			//log_error()
			return ret;
		}

		for (int i = 0;i < (*this).row; i ++) {
			for (int j = 0 ;j < (*this).column ; j++) {
				Dtype tdata = (*this).data_at(i,j);
				Dtype adata = A.data_at(i,j);
				Dtype& rdata = ret.data_at(i,j);
				rdata = tdata / adata;
			}
		}

		return ret;

	}

	Matrix dot(const Matrix& A) const {

		Matrix ret((*this).row,A.column,0);

		if((*this).column!=A.row){
			//log_error()
			return ret;
		}
		for (int k = 0; k < (*this).column ; k++) {
			for (int i = 0;i < (*this).row; i ++) {
				for (int j = 0 ;j < A.column ; j++) {
					Dtype tdata = (*this).data_at(i,k);
					Dtype adata = A.data_at(k,j);
					Dtype& rdata = ret.data_at(i,j);
					rdata += tdata * adata;
				}
			}
		}
		return ret;
	}

	Dtype* operator [](int row_) const {
		if (valid(row_,(*this).column-1) == false ) {
			//log_error()
			return (*this).data_ptr;
		}
		return (*this).data_ptr+row_*column;
	}

	Dtype& data_at(int row_,int column_) const {
		if (valid(row_,column_-1) == -1 ) {
			//log_error()
			return data_ptr[0];
		}
		return data_ptr[offset(row_,column_)];
	}

	Matrix get_sub_in_rows(std::vector<int> indexs){
		Matrix ret(indexs.size() , (*this).column );
		for(int i = 0 ;i < indexs.size() ; ++ i ){
			memcpy(ret[i],(*this)[indexs[i]],sizeof(Dtype)*(*this).column);
		}
		return ret;
	}

	Matrix get_sub_in_columns(std::vector<int> indexs){
		return (*this).T().get_sub_in_rows(indexs).T();
	}

	Matrix append(Matrix<Dtype> appdata){
		Matrix ret((*this).row+appdata.row , (*this).column );
		if((*this).column != appdata.column){
			//log_error();
			return ret;
		}
		memcpy(ret[0],(*this)[0],sizeof(Dtype)*(*this).row*(*this).column);
		memcpy(ret[(*this).row],appdata[0],sizeof(Dtype)*appdata.row*appdata.column);
		return ret;
	}

	bool valid(int row_,int column_) const {
		if(row_ < 0) return false;
		if(row_ > (*this).row) return false;
		if(column_ < 0) return false;
		if(column_ > (*this).column) return false;
		return true;
	}

	void show() const {
		puts("-------------------------------------------");
		for(int i = 0;i < (*this).row; i++){
			for(int j = 0;j < (*this).column; j++){
				std::cout << data_ptr[offset(i,j)] << " ";
			}std::cout << std::endl;
		}
		puts("-------------------------------------------");
	}

	Dtype mean(){
		Dtype ret = 0;
		for(int i = 0;i < (*this).row; i++){
			for(int j = 0;j < (*this).column; j++){
				ret += (*this)[i][j];
			}
		}
		return ret/(row*column);
	}

	int Row()const {
		return (*this).row;
	}
	int Column()const {
		return (*this).column;
	}


private:
	int row,column;
	Dtype* data_ptr;
};
