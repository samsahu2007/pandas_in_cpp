#ifndef PANDAS_MY_H
#define PANDAS_MY_H
#include<vector>
#include<variant>
#include<string>

using namespace std;

namespace DataFrame{
    struct header_type{
        string header_name;
        variant<vector<int>,vector<double>,vector<string>> * header_ref = nullptr;
    };
    struct DataStruct{
        vector<header_type> header_vec;
        vector<string> df_type;
        vector<variant<vector<int>,vector<double>,vector<string>>> data;
        long long row_num=0;
    };
    
    DataStruct & initialize_df(string filename, bool with_header);
    void display(DataStruct & dataframe);
    variant<vector<int>,vector<double>,vector<string>> get_column(DataStruct & dataframe, string key);
    DataStruct one_hot_encoding(DataStruct & dataframe, string key);
}

#endif