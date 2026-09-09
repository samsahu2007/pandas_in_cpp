#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include<vector>
#include<variant>
#include<string>

using namespace std;

namespace DataFrame{
    using ColumnData = std::variant<std::vector<int>, std::vector<double>, std::vector<std::string>>;

    struct header_type{
        string header_name;
        variant<vector<int>,vector<double>,vector<string>> * header_ref = nullptr;
    };

    class DataStruct{
        vector<header_type> header_vec;
        vector<string> df_type;
        vector<variant<vector<int>,vector<double>,vector<string>>> data;
        long long row_num=0;

        DataStruct(){};
        static string check_type( string s);
        static bool init_columns(vector<ColumnData> & new_dataframe, vector<string> & d_type);
        static void parse_stream(ifstream & file,vector<vector<string>> & arr ,long long & row_num, bool & with_header,
                                        vector<string> & header);
        static void init_data_types(vector<string> & d_type, vector<vector<string>> & arr);
        static void populate_data_after_checking_type(vector<vector<string>> & arr, vector<string> & d_type, 
                                                        vector<ColumnData> & new_dataframe);
        
        public:
            vector<header_type> & get_header_vec(){
                return header_vec;
            };                                 //defining these getter functions for vectors using
            vector<string> & get_df_type(){    //by reference so that no need to bother about
                return df_type;                //separate setter functions for now
            };         
            vector<variant<vector<int>,vector<double>,vector<string>>> & get_data(){
                return data;
            }; 
            long long get_row_count(){ 
                return row_num;
            }
            bool set_row_count(long long num){
                row_num = num;
                return true;
            }
            static DataStruct & initialize_df(string filename, bool with_header);
            void display();
            variant<vector<int>,vector<double>,vector<string>> get_column(const string key);
            DataStruct & one_hot_encoding(const string key);
    };
}

#endif