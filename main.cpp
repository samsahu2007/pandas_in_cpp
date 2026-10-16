#include<iostream>
#include<vector>
#include<numeric>
#include<fstream>
#include<sstream>
#include "DataStruct_class.h"

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

    DataFrame::DataStruct my_data = DataFrame::DataStruct("diabetes_1.csv", true);
    my_data.display();

    cout << my_data.get_row_count() << endl;
    cout << "\n\n\n\n";
    DataFrame::DataStruct one_hot_encoded = my_data.one_hot_encoding("smoking_history");
    one_hot_encoded.display();
    
    cout << covariance<double, int>(get<vector<double>>(my_data.get_column("age")), get<vector<int>>(my_data.get_column("blood_glucose_level"))) << endl;
    
}