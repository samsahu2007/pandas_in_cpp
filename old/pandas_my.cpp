#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<algorithm>
#include <variant>
#include <type_traits>
#include "pandas_my.h"
#include<typeinfo>
#include<set>

using namespace std;
using my_df = vector<variant<vector<int>,vector<double>,vector<string>>> ;
struct string_int_bundle{
    string name;
    int index =0;
};

namespace DataFrame{

    using ColumnData = std::variant<std::vector<int>, std::vector<double>, std::vector<std::string>>;

    string check_type( string s){
        int i=0;
        if (s[0]=='-')  i=1;
        for(;i<s.size();++i){
            if(isdigit(s[i]))   continue;
            else if(s[i]=='.')  return "double";
            else return "string";
        }
        return "int";
    }

    DataStruct & initialize_df(string filename, bool with_header){

        ifstream file (filename);
        
        if(!file.is_open()) throw invalid_argument("File cannot be opened");

        DataStruct * the_data = new DataStruct;
        vector<vector<string>> arr;
        string line;
        vector<string> header;

        long long row_num = 0;
        for(int i=0; getline(file, line); ++i){
            stringstream ss(line);
            string cell;
            
            vector<string> row;
            
            while(getline(ss, cell, ',')){
                row.push_back(cell);
            }

            if(with_header && i==0){
                header = row;
            }
            else if(!with_header && i==0){
                for(int i=0; i< row.size(); ++i){
                    header.push_back(to_string(i+1));
                }
            }
            else
                arr.push_back(row);
            
            row_num++;
        }

        vector<string> d_type;
        for(int i=0; i < arr[0].size(); ++i){
            string prev_data_type ="";
            int j=0;
            for(auto v : arr){
                //if(j==0 && with_header ) continue;
                string data_type = DataFrame::check_type(v[i]);
                if(prev_data_type=="")  prev_data_type=data_type;
                else if(prev_data_type==data_type);
                else{
                    string err_msg = "Row: " + to_string(i+1) + " Column: " 
                                        + to_string(j+1) + "has corrupt value";
                    throw invalid_argument(err_msg);
                }
                j++;
            }
            d_type.push_back(prev_data_type);
        }

        vector<ColumnData> new_dataframe;
        for(int i=0; i<d_type.size();++i){
            string & df_type = d_type[i];
            if(df_type == "string")
                new_dataframe.push_back(vector<string>());
            else if(df_type == "int")
                new_dataframe.push_back(vector<int>());
            else if(df_type=="double")
                new_dataframe.push_back(vector<double>());
            else{
                new_dataframe.push_back(vector<string>());
                cout << "Why did it reach here? Each vector must be double/int/string" << endl;
            }
            
        }

        for(int i=0; i < arr[0].size(); ++i){
            string data_type =d_type[i];
                std::visit([&arr, i](auto& col){
                    using T = typename std::decay_t<decltype(col)>::value_type;
                    for( int j=0; j< arr.size(); ++j){
                        //if(j==0 && with_header ) 
                          //  continue;
                        if constexpr (std::is_same_v<T, int>)
                            col.push_back(stoi(arr[j][i]));
                        else if constexpr (is_same_v<T, double>)
                            col.push_back(stod(arr[j][i]));
                        else if constexpr (is_same_v<T, string>)
                            col.push_back(arr[j][i]);
                    }
                }, new_dataframe[i]);
        }

        // for(auto x: header)
        //     cout << x << endl;

        for(auto x: header){
            header_type a;
            a.header_name = x;
            the_data -> header_vec.push_back(a);
        }
        the_data -> df_type = d_type;
        the_data -> data = new_dataframe;
        the_data -> row_num = row_num;
        return *the_data;
    }    

    DataStruct one_hot_encoding(DataStruct & dataframe, string key){
        vector<string> header;
        for(auto x: dataframe.header_vec){
            header.push_back(x.header_name);
        }
        my_df & copy_df = dataframe.data; 
        DataStruct the_data;
        int jk = 0;
        for(auto v: copy_df){
            if(holds_alternative<vector<string>>(v) && header[jk]==key){
                set<string> s;
                const auto & vec = get<vector<string>>(v);
                for (string x: vec){
                    s.emplace(x);
                    //cout << x << endl;
                }
                vector<string_int_bundle> temp;

                int ppp=0;
                int intial_size_data = the_data.data.size();
                cout << "initial_size_data: "<< intial_size_data << endl;
                for(string x: s){
                    header_type a;
                    string_int_bundle b;
                    the_data.data.push_back(vector<int>(dataframe.row_num, 0));
                    a.header_name = x;
                    b.name = x;
                    b.index = intial_size_data + ppp;
                    the_data.header_vec.push_back(a);
                    // a.header_ref = &(the_data.data.back());
                    // if(a.header_ref == nullptr){
                    //     cout << "why null: " << a.header_name << endl;
                    // }
                    temp.push_back(b);

                    //cout << typeid(*(a.header_ref)).name() << endl;
                    ++ppp;
                    // cout << x << endl;
                }
                //cout << "Inserted " << ppp << " new vectors" << endl;

                int vec_count =0;
                for(string x: vec){
                    for(string_int_bundle p: temp){
                        if(p.name == x){
                            //if(x=="No Info")    cout << "got a match" << endl;
                            vector<int> & str_vec = get<vector<int>>(the_data.data[p.index]);
                            //cout << p.header_name << endl;
                            // if(str_vec == nullptr) {
                            //     cout << "Why null";
                            //     cout << p.header_name << endl;
                            // }
                            (str_vec)[vec_count]=1;
                                // cout << x << endl;
                        }
                    }
                    ++vec_count;
                }
                cout << "hi" << endl;

            }
            else{
                vector<header_type> h_n = dataframe.header_vec;
                header_type a;
                a.header_name = h_n[jk].header_name;
                the_data.header_vec.push_back(a);
                the_data.data.push_back(v);
            }
            jk++; 
        }

        the_data.row_num = dataframe.row_num;
        return the_data;
    }

    void display( DataStruct & dataframe ){
        vector<string> header;
        for(auto x: dataframe.header_vec){
            header.push_back(x.header_name);
        }
        for(auto x: header){
            cout << x << " " ;
        }
        cout << "\n";
        vector<variant<vector<int>,vector<double>,vector<string>>> df = dataframe.data;
        for(int j=0; j< min((long long)10, dataframe.row_num);++j){
            for(int i=0; i<df.size(); ++i){
                visit([&j](auto & col){
                    cout << col[j] <<" ";
                }, df[i]);
            }
            cout << "\n";
        }
    }

    variant<vector<int>,vector<double>,vector<string>> get_column(DataStruct & dataframe, string key){
        int i=0;
        for(auto v: dataframe.header_vec){
            if (v.header_name == key){
                return dataframe.data[i];
            }
            i++;
        }
        throw invalid_argument("Key is invalid");
    }
}

// int main(){
//     DataFrame::DataStruct & dataframe = DataFrame::initialize_df("diabetes_prediction_dataset.csv", true);
//     display(dataframe);
//     // for(auto x: dataframe.df_type)
//     //     cout << x << endl;
// }