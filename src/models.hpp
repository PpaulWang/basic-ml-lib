#include <iostream>
#include <fstream>
#include <cmath>
#include "matrix.hpp"
#include <algorithm>
using namespace std;

template <typename Dtype>
class AUC{
public:
	AUC(){}
	AUC(const Matrix<Dtype>& PROBS,const Matrix<Dtype>& LABELS){
		pros = PROBS[0];
		labels = LABELS[0];
		N = PROBS.Row();

		
		std::vector<int> ranks = get_ranks();
		long long PN = 0 ;
		long long P_rank_sum = 0;
		for (int i = 0;i < N ; i++ ){
			if(0.5 < labels[i]) {
				P_rank_sum += ranks[i];
				PN++;
			}
		}
		this->auc_ = (Dtype)(P_rank_sum - PN*(PN+1)/2)/(PN*(N-PN));
	}
	std::vector<int> get_ranks() const {
		std::vector<pair<Dtype,int> > temp;
		for(int i= 0 ;i< N ;i++){
			temp.push_back(
				make_pair(pros[i],i)
			);
		}
		sort(temp.begin(),temp.end());
		std::vector<int> ret(N);
		for(int i=0;i<N;i++){
			ret[ temp[i].second ] = i;
		}
		return ret;
	}
	Dtype auc() const {
		return this->auc_;
	}
		
private:
	const Dtype* pros;
	const Dtype* labels;
	Dtype auc_;
	int N ; 

};

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

	Dtype get_auc(const Matrix<Dtype>& inputs,const Matrix<Dtype>& targets ){ 

		Matrix<Dtype> model_inputs = inputs.T();

		Matrix<Dtype> model_targets = targets.T();
		
		Matrix<Dtype> outputs = forward(model_inputs);
		float last_fpr=1,ret=0;
		for (float threshold = 0.0 ; threshold < 1.0 ; threshold += 0.001 ){
			int TP = 0 , FP = 0 , FN = 0 , TN = 0;
		
			for ( int i = 0 ; i < outputs.Column() ; i++ ){
				if (outputs[0][i] > threshold ){
					if(targets[0][i] > 0.9) TP++;
					else FP++;
				}
				else {
					if(targets[0][i] > 0.9) FN++;
					else TN++;
				}
			}
			int N = FP + TN , P = FN + TP;
			Dtype TPR = (Dtype) TP / P;
			Dtype FPR = (Dtype) FP / N;
			ret += (last_fpr - FPR) * TPR;
			last_fpr = FPR;
		}
		return ret;
	}
	Dtype get_acc(const Matrix<Dtype>& inputs,const Matrix<Dtype>& targets , const Dtype threshold = 0.5 ){

		Matrix<Dtype> model_inputs = inputs.T();

		Matrix<Dtype> model_targets = targets.T();
		
		Matrix<Dtype> outputs = forward(model_inputs);
		int TP = 0 , FP = 0 , FN = 0 , TN = 0;
		
		for ( int i = 0 ; i < outputs.Column() ; i++ ){
			if (outputs[0][i] > threshold ){
				if(targets[0][i] > 0.9) TP++;
				else FP++;
			}
			else {
				if(targets[0][i] > 0.9) FN++;
				else TN++;
			}
		}
		
		return (Dtype)( TP + TN ) / ( TP + TN + FP + FN );
	}
	Dtype get_loss(const Matrix<Dtype>& inputs,const Matrix<Dtype>& targets){

		Matrix<Dtype> model_inputs = inputs.T();

		Matrix<Dtype> model_targets = targets.T();
		

		Matrix<Dtype> outputs = forward(model_inputs);
		Matrix<Dtype> error = model_targets - outputs;

		return (error*error).mean();
	}

	Matrix<Dtype> predict(const Matrix<Dtype>& inputs){
		return forward(inputs.T()).T();
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
		fin >> (*this).input_nums >> (*this).output_nums ;
		(*this).weights = Matrix<Dtype>(filename+".matrix");
	}
	void set_learning_rate(Dtype learning_rate){
		(*this).learning_rate = learning_rate;
	}
	void save_file(std::string filename) const {
		std::ofstream fout(filename.c_str());
		fout << (*this).input_nums << " " << (*this).output_nums << std::endl;
		fout << filename+".matrix" << std::endl;
		(*this).weights.save(filename+".matrix");
	}
private:
	int input_nums;
	int output_nums;
	Dtype learning_rate;
	Matrix<Dtype> weights;
};
