#include "models.hpp"
#include <cmath>

template<typename Dtype>
Dtype sigmoid(const Dtype x){
	return 1/(1+exp(-x));
}


template<typename Dtype>
Matrix<Dtype> get_batch_datas(int train_batch_num,int feature_num,Dtype threshold,Matrix<Dtype> inputs,Matrix<Dtype> target_weights){
	
	Matrix<Dtype> targets = target_weights.dot(inputs.T()).process(sigmoid);

	return targets.T();
}

template<typename Dtype>
int test_model(){

	int train_time = 2000;
	int feature_num = 3;


	int train_batch_num = 500;
	int test_batch_num = 100;
	
	int echo_num = 20;

	Matrix<Dtype> target_weights(1,feature_num);

	LogisticRegression<Dtype> LR_models(feature_num,1,0.1);
	Matrix<Dtype> train_inputs = Matrix<Dtype>(train_batch_num,feature_num);
	Matrix<Dtype> train_targets = get_batch_datas<Dtype>(train_batch_num,feature_num,feature_num*0.5,train_inputs,target_weights);
	
	
	Matrix<Dtype> test_inputs = Matrix<Dtype>(test_batch_num,feature_num);
	Matrix<Dtype> test_targets = get_batch_datas<Dtype>(test_batch_num,feature_num,feature_num*0.5,test_inputs,target_weights);
		
	//train_inputs.show();
	//train_targets.show();

	for(int i=0;i<train_time;i++){

		Dtype loss = LR_models.train(train_inputs,train_targets);

		if (i % echo_num == 0){
			printf("The loss of the %03d time is %.3f ,delta loss is %.3f\n",i,loss,loss-LR_models.get_loss(train_inputs,train_targets));
			printf ("The test loss is %.3f\n",LR_models.get_loss(test_inputs,test_targets));
		}

	}
	cout<<"target weights"<<endl;
	target_weights.show();
	cout<<"learned weights"<<endl;
	LR_models.show();
	return 0;
}


int test(){
	//process();
	//test_model();
	
	cout<<wcq::memory_blocks_num<<endl;
	return 0;
}
int main(){
	test_model<double>();
	cout<<wcq::memory_blocks_num<<endl;
	return 0;
}