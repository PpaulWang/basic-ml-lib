#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include "matrix.hpp"
using namespace std;
const int MAX_INDEX = 2000000;
const int FEATURE_NUM = 200;

void get_feature_indexs(vector<int> & ret,int cnt[],int index,bool flag){
	if(flag){
		ofstream fout("feature_indexs.txt");
		for(int i = 0 ; i < FEATURE_NUM ; i++){
			if(cnt[i] + 10 >= index){
				fout<<i<<" ";	
				ret.push_back(i);
			}
		}
		fout<<std::endl;
	}
	else { 
		ifstream fin("feature_indexs.txt");
		int feature_index;
		while(fin>>feature_index){
			ret.push_back(feature_index);
		}
		fin.close();
	}
	return ;
}
int main(int argc,char** argv){
	string str;
	freopen(argv[1],"r",stdin);
	int cnt[MAX_INDEX];
	memset(cnt,0,sizeof(cnt));
	int index=0;
	Matrix<float> features(MAX_INDEX,FEATURE_NUM);
	Matrix<float> labels(MAX_INDEX,1);
	vector<int> final_indexs;
	
	ifstream fin(argv[1]);
	while(getline(fin,str)){
		int feature_index;
		for(int i=0;i<str.length();i++){
			if(str[i]==':')str[i]=' ';
		}
		stringstream sin(str);
		sin>>labels[index][0];
		while(sin>>feature_index){
			sin>>features[index][feature_index];
			cnt[feature_index]++;
		}
		if(index % 10000 == 0){
			cout<<index<<" : ";
			cout<<labels[index][0]<<endl;
		}
		final_indexs.push_back(index);
		++index;
	}
	vector<int> final_feature_indexs;
	bool trainflag = strstr(argv[1],".train");
	get_feature_indexs(final_feature_indexs,cnt,index,trainflag);
	if(trainflag){
		features.get_sub_in_columns(final_feature_indexs).get_sub_in_rows(final_indexs).save("inputs_train.matrix");
		labels.get_sub_in_rows(final_indexs).save("labels_train.matrix");
	}
	else {

		features.get_sub_in_columns(final_feature_indexs).get_sub_in_rows(final_indexs).save("inputs_test.matrix");
		labels.get_sub_in_rows(final_indexs).save("labels_test.matrix");
	}
}
