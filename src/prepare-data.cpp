#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include "matrix.hpp"
using namespace std;
const int MAX_INDEX = 100000;
const int FEATURE_NUM = 200;

int main(){
	freopen("features.txt","r",stdin);
	int cnt[MAX_INDEX];
	memset(cnt,0,sizeof(cnt));
	int index=0;
	Matrix<float> features(MAX_INDEX,FEATURE_NUM);
	Matrix<float> labels(MAX_INDEX,1);
	vector<int> final_indexs;
	while(scanf("%f",&labels[index][0]) != EOF){
		while(getchar()!='\n'){
			int feature_index;
			float feature_value;
			scanf("%d:%f",&feature_index,&feature_value);
			cnt[feature_index]++;
			features[index][feature_index] = feature_value ;
		}
		cout<<labels[index][0]<<endl;
		final_indexs.push_back(index);
		++index;

	}
	vector<int> final_feature_indexs;
	for(int i = 0 ; i < MAX_INDEX ; i++){
		if(cnt[i] == index){
			final_feature_indexs.push_back(i);
		}
	}

	//features.show();
	features.get_sub_in_columns(final_feature_indexs).get_sub_in_rows(final_indexs).save("inputs.matrix");
	labels.get_sub_in_rows(final_indexs).save("labels.matrix");
}