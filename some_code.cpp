#include<iostream>
#include<vector>
#include<numeric>
#include<fstream>
#include<sstream>
#include "pandas_my.h"

using namespace std;
using df = vector<variant<vector<int>,vector<double>,vector<string>>> ;

template<typename T>
vector<double> deviation(const vector<T> & x){
    double avg = accumulate(x.begin(), x.end(), 0.0)/x.size();
    vector<double> x_bar;
    for (auto elem: x){
        x_bar.push_back(elem - avg);
    }
    return x_bar;
}

template<typename T1, typename T2> 
double dot_product(const vector <T1> &x, const vector<T2>& y){
    if(x.size()!=y.size()){
        throw invalid_argument("Size of both vectors must be equal");
    }
    else{
        double dot=0;
        for(long long i = 0; i<x.size(); ++i){
            dot+=(x[i]* y[i]);
        }
        return dot;
    }
}

template<typename T1, typename T2>
double covariance(const vector<T1> & x, const vector<T2> & y){
    vector<double> deviation_x = deviation<T1>(x);
    vector<double> deviation_y = deviation<T2>(y);

    double dot_prod = dot_product<double, double>(deviation_x, deviation_y);

    return dot_prod/(x.size()-1); 
}


int main(){
    // ifstream file ("diabetes_prediction_dataset.csv");
     
    // if(!file.is_open()) throw invalid_argument("File cannot be opened");

    DataFrame::DataStruct & my_data = DataFrame::initialize_df("diabetes_1.csv", true);
    DataFrame::display(my_data);

    //delete &my_data;  
    cout << my_data.row_num << endl;
    cout << "\n\n\n\n";
    DataFrame::DataStruct one_hot_encoded = DataFrame::one_hot_encoding(my_data, "smoking_history");
    DataFrame::display(one_hot_encoded);
    
    cout << covariance<double, int>(get<vector<double>>(DataFrame::get_column(my_data, "age")), get<vector<int>>(DataFrame::get_column(my_data, "blood_glucose_level"))) << endl;
    
    delete &my_data;
}