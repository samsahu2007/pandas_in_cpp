#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<algorithm>
#include <variant>
#include <type_traits>
//#include "pandas_my.h"
#include<typeinfo>
#include<set>
#include "DataStruct_class.h"


using namespace std;
using my_df = vector<variant<vector<int>,vector<double>,vector<string>>> ;
struct string_int_bundle{
    string name;
    int index =0;
};

namespace DataFrame{
    //using ColumnData = std::variant<std::vector<int>, std::vector<double>, std::vector<std::string>>;

    string DataStruct::check_type(string & s){
        int str_size = s.size();
        size_t l = 0, r = str_size;
        while(l<r && s[l]==' ') l++;
        while(r>l && s[r-1]==' ') r--; 

        if(l>0 || r<str_size-1){
            string new_s = s.substr(l, r-l);
            s = new_s;
        }

        if(s.size() == 0)
            return "NaN";
        int i=0, seen_digit = 0;
        if (s[0]=='-' || s[0] == '+'){
            if(s.size()==1)
                return "string";
            else 
                i = 1;
        }
        int dot_count = 0;
        for(;i<s.size();++i){
            if(isdigit(s[i])){
                seen_digit=1;
                continue;
            }
            else if(s[i]=='.'){
                if(seen_digit!=1) return "string";
                ++ dot_count;
            }
            else{
                return "string";
            }
        }
        if(seen_digit==1 && dot_count == 0)
            return "int";
        else if(seen_digit == 1 && dot_count == 1)
            return "double";
        else 
            return "string";
    }

    bool DataStruct::init_columns(vector<ColumnData> & new_dataframe, vector<string> & d_type){
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
                return false;
            }
        }

        return true;
    }

    void DataStruct::parse_stream(ifstream & file,vector<vector<string>> & arr ,long long & row_num, bool & with_header,
                                        vector<string> & header){
        string line;
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
    }

    void DataStruct::init_data_types(vector<string> & d_type, vector<vector<string>> & arr){
        for(int i=0; i < arr[0].size(); ++i){
            string prev_data_type ="";
            int j=0;
            for(auto v : arr){
                //if(j==0 && with_header ) continue;
                string data_type = DataFrame::DataStruct:: check_type(v[i]);
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
    }

    void DataStruct::populate_data_after_checking_type(vector<vector<string>> & arr, vector<string> & d_type, 
                                                        vector<ColumnData> & new_dataframe){
        for(int i=0; i < arr[0].size(); ++i){
            //string data_type =d_type[i];
            if(holds_alternative<vector<int>>(new_dataframe[i])){
                for( int j=0; j< arr.size(); ++j){
                    vector<int> & col = get<vector<int>>(new_dataframe[i]);
                    col.push_back(stoi(arr[j][i]));
                }
            }
            else if(holds_alternative<vector<double>>(new_dataframe[i])){
                for( int j=0; j< arr.size(); ++j){
                    vector<double> & col = get<vector<double>>(new_dataframe[i]);
                    col.push_back(stod(arr[j][i]));
                }
            }
            else if(holds_alternative<vector<string>>(new_dataframe[i])){
                for( int j=0; j< arr.size(); ++j){
                    vector<string> & col = get<vector<string>>(new_dataframe[i]);
                    col.push_back(arr[j][i]);
                }
            }
        }
    }

    DataStruct & DataStruct::initialize_df(const string filename, bool with_header){
        ifstream file (filename);
        
        if(!file.is_open()) throw invalid_argument("File cannot be opened");

        DataStruct * the_data = new DataStruct;
        vector<vector<string>> arr;
        string line;
        vector<string> header;

        long long & row_num = the_data->row_num;
        parse_stream(file, arr, row_num, with_header, header);

        vector<string> & d_type = the_data -> df_type;
        init_data_types(d_type, arr);

        vector<ColumnData> & new_dataframe = the_data -> data;
        init_columns(new_dataframe, d_type);

        populate_data_after_checking_type(arr, d_type, new_dataframe);

        for(auto x: header){
            header_type a;
            a.header_name = x;
            the_data -> header_vec.push_back(a);
        }
        return *the_data;
    }    

    void DataStruct:: display(){
        vector<string> header;
        for(auto x: header_vec){
            header.push_back(x.header_name);
        }
        for(auto x: header){
            cout << x << " " ;
        }
        cout << "\n";
        vector<variant<vector<int>,vector<double>,vector<string>>> df = data;
        for(int j=0; j< min((long long)10, row_num);++j){
            for(int i=0; i<df.size(); ++i){
                visit([&j](auto & col){
                    cout << col[j] <<" ";
                }, df[i]);
            }
            cout << "\n";
        }
    }

    variant<vector<int>,vector<double>,vector<string>>DataStruct::get_column(const string key){
        int i=0;
        for(auto v: header_vec){
            if (v.header_name == key){
                return data[i];
            }
            i++;
        }
        throw invalid_argument("Key is invalid");
    }

    DataStruct & DataStruct::one_hot_encoding(const string key){
        vector<string> header;
        for(auto x: header_vec){
            header.push_back(x.header_name);
        }
        my_df & copy_df = data; 
        DataStruct * the_data = new DataStruct;
        int jk = 0;
        for(auto v: copy_df){
            if(holds_alternative<vector<string>>(v) && header[jk]==key){
                set<string> s;
                const auto & vec = get<vector<string>>(v);
                for (string x: vec){
                    s.emplace(x);
                }
                vector<string_int_bundle> temp;

                int ppp=0;
                int intial_size_data = (*the_data).data.size();
                //cout << "initial_size_data: "<< intial_size_data << endl;
                for(string x: s){
                    header_type a;
                    string_int_bundle b;
                    (*the_data).data.push_back(vector<int>(row_num, 0));
                    a.header_name = x;
                    b.name = x;
                    b.index = intial_size_data + ppp;
                    (*the_data).header_vec.push_back(a);

                    temp.push_back(b);

                    ++ppp;
                    // cout << x << endl;
                }
                //cout << "Inserted " << ppp << " new vectors" << endl;

                int vec_count =0;
                for(string x: vec){
                    for(string_int_bundle p: temp){
                        if(p.name == x){
                            vector<int> & str_vec = get<vector<int>>((*the_data).data[p.index]);
                            (str_vec)[vec_count]=1;
                                // cout << x << endl;
                        }
                    }
                    ++vec_count;
                }
            }
            else{
                vector<header_type> h_n = header_vec;
                header_type a;
                a.header_name = h_n[jk].header_name;
                (*the_data).header_vec.push_back(a);
                (*the_data).data.push_back(v);
            }
            jk++; 
        }

        (*the_data).row_num = row_num;
        return *the_data;
    }
}