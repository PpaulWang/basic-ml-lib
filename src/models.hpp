#include <iostream>
#include <fstream>
#include <cmath>
#include "matrix.h"

using namespace std;


template <typename Dtype>
class LogisticRegression{
public:
	LogisticRegression(){}
	LogisticRegression(std::string filename){
		(*this).load_file(filename);
	}
	LogisticRegression(int _input_nums, int _output_nums,Dtype _learning_rate)
	:input_nums(_input_nums),output_nums(_output_nums),learning_rate(_learning_rate){
		weights = Matrix<Dtype>(output_nums, input_nums ,0);
	}
	static Dtype sigmoid(Dtype x){
		return 1/(1+exp(-x));
	}
	Matrix<Dtype> forward(const Matrix<Dtype>& inputs){
		return this->weights.dot(inputs).process(sigmoid);
		/*
		Matrix<Dtype> outputs = this->weights.dot(inputs).process(sigmoid);
		return outputs.process(sigmoid);
		
		Matrix<Dtype> ret = outputs.process(sigmoid);
		return ret;
		*/
	}
	Matrix<Dtype> backword(const Matrix<Dtype>& Error,const Matrix<Dtype>& inputs){
		/*
		return Error.dot(inputs.T());
		*/
		Matrix<Dtype> model_inputs = inputs.T();
		return Error.dot(model_inputs);
		
	}
	Dtype train(const Matrix<Dtype>& inputs,const Matrix<Dtype>& targets){

		Matrix<Dtype> model_inputs = inputs.T();

		Matrix<Dtype> model_targets = targets.T();

		Matrix<Dtype> outputs = forward(model_inputs);
		Matrix<Dtype> error = model_targets-outputs;
		//error.show();
		//(error*error).show();
		Matrix<Dtype> delat_w = backword(error,model_inputs);
		weights += (delat_w*(learning_rate/inputs.Row()));
/*
		cout<<"weights"<<endl;
		weights.show();
		
		cout<<"delat_w"<<endl;
		delat_w.show();
*/
		return (error*error).mean();
	}

	Dtype get_loss(const Matrix<Dtype>& inputs,const Matrix<Dtype>& targets){

		Matrix<Dtype> model_inputs = inputs.T();

		Matrix<Dtype> model_targets = targets.T();
		

		Matrix<Dtype> outputs = forward(model_inputs);
		Matrix<Dtype> error = model_targets - outputs;

		return (error*error).mean();
	}

	Matrix<Dtype> predict(const Matrix<Dtype>& inputs){
		return forward(inputs.T());
		/*
		Matrix<Dtype> model_inputs = inputs.T();
		return forward(model_inputs);
		*/
	}
	void show()const{
		weights.show();
	}
	const Matrix<Dtype>& Weights()const {
		return (*this).weights;
	}
	void load_file(std::string filename){
		std::ifstream fin(filename.c_str());
		fin >> (*this).input_nums >> (*this).output_nums >> (*this).learning_rate;
		(*this).weights = Matrix<Dtype>(filename+".matrix");
	}
	void save_file(std::string filename) const {
		std::ofstream fout(filename.c_str());
		fout << (*this).input_nums << " " << (*this).output_nums << " " << (*this).learning_rate <<std::endl;
		fout << filename+".matrix" << std::endl;
		(*this).weights.save(filename+".matrix");
	}
private:
	int input_nums;
	int output_nums;
	Dtype learning_rate;
	Matrix<Dtype> weights;
};