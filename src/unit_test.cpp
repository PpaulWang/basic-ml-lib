#include "models.hpp"
#include <cmath>
#include <vector>

template<typename Dtype>
Dtype sigmoid(const Dtype x){
	return 1/(1+exp(-x));
}
std::vector<int> get_randoms(int low,int high,int num){
	std::vector<int> ret;
	int range = high - low ;
	while(num--){
		ret.push_back((rand()%range)+low);
	}
	return ret;
}

template<typename Dtype>
Matrix<Dtype> get_batch_datas(int train_batch_num, int feature_num, Dtype threshold, 
	Matrix<Dtype> inputs, Matrix<Dtype> target_weights){
	
	Matrix<Dtype> targets = target_weights.dot(inputs.T()).process(sigmoid);

	return targets.T();
}

template<typename Dtype>
int test_model(int argv, char** argc){

	int train_time = atoi(argc[1]);
	int feature_num = 30;

	int train_num = 500000;
	int train_batch_num = atoi(argc[2]);
	int test_batch_num = 100;
	bool load_flag = (argc[3][0]=='o');
	int echo_num = 200 ;

	Matrix<Dtype> target_weights(1,feature_num);

	LogisticRegression<Dtype> LR_models(feature_num,1,0.1);

	if (load_flag == true){

		target_weights = Matrix<Dtype>(std::string("target_weights.matrix"));
		cout<<"DSf"<<endl;
		LR_models = LogisticRegression<Dtype>("test.model");
	}
	Matrix<Dtype> train_inputs = Matrix<Dtype>(train_num,feature_num);
	Matrix<Dtype> train_targets = 
	get_batch_datas<Dtype>(train_num,feature_num,feature_num*0.5,train_inputs,target_weights);
	
	
	Matrix<Dtype> test_inputs = Matrix<Dtype>(test_batch_num,feature_num);
	Matrix<Dtype> test_targets = 
	get_batch_datas<Dtype>(test_batch_num,feature_num,feature_num*0.5,test_inputs,target_weights);
	

	//train_inputs.show();
	//train_targets.show();

	for(int i=0;i<train_time;i++){

		std::vector<int> batch_indexs = get_randoms(0,train_num,train_batch_num);
		Matrix<Dtype> batch_inputs = train_inputs.get_sub_in_rows(batch_indexs);
		Matrix<Dtype> batch_targets = train_targets.get_sub_in_rows(batch_indexs);
		Dtype loss = LR_models.train(batch_inputs,batch_targets);
		if (i % echo_num  == 0 || i == train_time - 1 ){

			printf("The loss of the %03d time is %.10f ,delta loss is %.10f\n",
				i,loss,loss-LR_models.get_loss(batch_inputs,batch_targets));
			printf ("The test loss is %.10f\n",LR_models.get_loss(test_inputs,test_targets));

		}
		Matrix<Dtype> weights_error = target_weights-LR_models.Weights() ;
		if((weights_error*weights_error).mean()<1e-10){
			printf("The model is trained OK , after %d times training\n",i+1);
			break;
			
		}
	}
	LR_models.save_file(std::string("test.model"));
	target_weights.save(std::string("target_weights.matrix"));
	cout << "target weights" << endl;
	target_weights.show();
	cout << "learned weights" << endl;
	LR_models.Weights().show();
	return 0;
}


int test(){
	//process();
	//test_model();
	
	cout<<wcq::memory_blocks_num<<endl;
	return 0;
}
int main(int argv, char** argc){
	test_model<double>(argv, argc);
	cout<< "memory_blocks_num : "<<wcq::memory_blocks_num<<endl;
	return 0;
}