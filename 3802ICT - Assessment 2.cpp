#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <string>
#include <cstdint>
#include <algorithm>

using namespace std;

constexpr uint32_t df_WIDTH = 5;
constexpr uint32_t df_HEIGHT = 5;

template<template<typename> typename Column, typename T>
class DataFrame {
private:
    // data_vec is the storage of the data frames.
    vector<Column<T>>* data_vec;
public:
    // 1.1 - Initialisation
    // Default constructor
    DataFrame() {
        data_vec = new vector<Column<T>>();
    };
    // Parameterised constructor
    DataFrame(vector<Column<T>>* _vectors) {
        data_vec = new vector<Column<T>>();
        data_vec = _vectors;
    };
    // Destructor
    /*~DataFrame() {
        delete data_vec;
    };*/

    // 1.2 - Load Data
    // Loads vectors data into data_vec and overwrites original data
    void load_data(vector<Column<T>>* _vectors) {
        data_vec->clear();
        data_vec = _vectors;
    }
    void load_data(Column<T> _vector) {
        data_vec->clear();
        data_vec->push_back(_vector);
    }
    // Adds data to the data_vec, that being a column of data.
    void add_data(Column<T> _vector) {
        data_vec->push_back(_vector);
    }
    void add_data(vector<Column<T>> _vectors) {
        for (auto iter : _vectors) {
            data_vec->push_back(iter);
        }
    }
    void add_data(Column<T> _vector, string column_name) {
        data_vec->push_back(_vector);
        data_vec->back().column_name_setter(column_name);
    }
    void add_data(vector<Column<T>> _vectors, vector<string> column_names) {
        int i = 0;
        for (auto iter : _vectors) {
            data_vec->push_back(iter);
            data_vec->back().column_name_setter(column_names[i]);
            i++;
        }
    }

    // 1.3 - Setting Column Names
    void set_columns(vector<string> column_names, uint32_t size) {
        for (int32_t i = 0; i < size; i++) {
            data_vec->at(i).column_name_setter(column_names[i]);
        }
    }

    // 1.4 - Updating Column Names
    void update_column(string old_name, string new_name) {
        uint32_t column_idx = find_column(old_name);
        data_vec->at(column_idx).column_name_setter(new_name);
    }
    void update_columns(uint32_t column, string name) {
        data_vec->at(column).column_name_setter(name);
    }

    // 1.5 - Updating Columns
    void update_columnval(uint32_t column_idx, vector<T> _vector) {
        data_vec->at(column_idx).clear();
        for (size_t i = 0; i < _vector.size(); i++) {
            data_vec->at(column_idx).push_back(_vector[i]);
        }
    }
    void update_columnval(string column_name, vector<T> _vector) {
        int32_t column_idx = find_column(column_name);

        data_vec->at(column_idx).clear();
        for (size_t i = 0; i < _vector.size(); i++) {
            data_vec->at(column_idx).push_back(_vector[i]);
        }
    }
    void delete_column(uint32_t column_idx) {
        
    }
    void delete_column(string column_name) {
        
    }

    // 1.6 - Get Columns
    Column<T>& operator[](uint32_t column_idx) {
        return data_vec->at(column_idx);
    }
    Column<T>& operator[](string column_name) {
        int column_idx = find_column(column_name);
        return data_vec->at(column_idx);
    }

    int find_column(string column_name) {
        for (size_t i = 0; i < data_vec->size(); i++) {
            if (data_vec->at(i).column_name_getter() == column_name) {
                return i;
            }
        }
        return -1;
    }

    // 1.7 - Data Frame Metadata
    int size() {
        return (data_vec->size() * data_vec[0]->size());
    }
    int shape() {
        return (data_vec->size());
    }

    vector<Column<T>> get_dataframe() {
        return *data_vec;
    }
};

template<typename T>
class Column : public vector<T> {
private:
    vector<T> data_vec;
    string column_name;
    using vector<T>::vector;
public:
    Column() {};
    Column(vector<T> _vector) {
        data_vec = _vector;
    }

    void column_name_setter(string name) {
        column_name = name;
    }
    string column_name_getter() {
        return column_name;
    }

    int min() {
        int min = INT_MAX;
        for (int32_t i = 0; i < data_vec.size(); i++) {
            if (data_vec[i] < min) {
                min = data_vec[i];
            }
        }
        return min;
    }
    int max() {
        int max = INT_MIN;
        for (int32_t i = 0; i < data_vec.size(); i++) {
            if (data_vec[i] > max) {
                max = data_vec[i];
            }
        }
        return max;
    }
    double mean() {
        int sum = 0;
        for (size_t i = 0; i < data_vec.size(); i++) {
            sum += data_vec[i];
        }
        return sum / (data_vec.size());
    }
    int median() {
        vector<T> _vector = data_vec;
        sort(_vector.begin(), _vector.endf());
        return _vector[_vector.size() / 2];
    }
    int mode() {
        vector<T> _vector = data_vec;
        sort(_vector.begin(), _vector.endf());

        int max_count = 1, res = _vector[0], curr_count = 1;
        for (int i = 1; i < _vector.size(); i++) {
            if (_vector[i] == _vector[i - 1])
                curr_count++;
            else {
                if (curr_count > max_count) {
                    max_count = curr_count;
                    res = _vector[i - 1];
                }
                curr_count = 1;
            }
        }

        // If last element is most frequent
        if (curr_count > max_count)
        {
            max_count = curr_count;
            res = _vector[_vector.size() - 1];
        }

        return res;
    }
    void summary() {
    
    }
};

// Generates individual random numbers
int get_random_number() {
    random_device dev;
    mt19937 twister(dev());
    uniform_int_distribution<uint32_t> dist(0, 9);

    return (dist(twister));
}

// Assigns the values of each index of a 2D vector to random values
template<typename T>
void create_random_table(vector<Column<T>>& vectors) {
    for (int32_t i = 0; i < vectors.size(); i++) {
        for (int32_t j = 0; j < vectors[0].size(); j++) {
            vectors[i][j] = (T)get_random_number();
        }
    }
}

template<typename T>
void create_random_array(Column<T>& vector) {
    for (int32_t i = 0; i < 5; i++) {
        vector.push_back((T)get_random_number());
    }
}

// Prints a 2D vector
template<typename T>
void print_dataframe(const vector<Column<T>>& vectors) {
    for (int32_t i = 0; i < vectors.size(); i++) {
        for (int32_t j = 0; j < vectors[0].size(); j++) {
            cout << vectors[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    /* -------Creating DataFrame------- */
    cout << "Empty DataFrame:\n\n";
    DataFrame<Column, int32_t> df_empty; //-
    print_dataframe(df_empty.get_dataframe());

    /* -------Creating DataFrame with parameters------- */
    cout << "Creating DataFrame with parameters:\n";
    vector<Column<int32_t>> vectors(df_WIDTH, Column<int32_t>(df_HEIGHT));
    create_random_table(vectors);
    DataFrame<Column, int32_t> df(&vectors); //-
    print_dataframe(df.get_dataframe());
    cout << endl;

    /* -------Loading data of vectors to overwrite------- */
    cout << "Loading 2D vector to overwrite:\n";
    vector<Column<int32_t>> vectors1(df_WIDTH, Column<int32_t>(df_HEIGHT));
    create_random_table(vectors1);
    df.load_data(&vectors1); //-
    print_dataframe(df.get_dataframe());
    cout << endl;

    /* -------Loading data of a vector to overwrite------- */
    cout << "Loading 1D vector to overwrite:\n";
    Column<int32_t> vec;
    create_random_array(vec);
    df.load_data(vec); //-
    print_dataframe(df.get_dataframe());
    cout << endl;

    /* -------Adding data of a vector------- */
    cout << "Adding 1D vector to DataFrame:\n";
    Column<int32_t> vec1;
    create_random_array(vec1);
    df.add_data(vec1); //-
    print_dataframe(df.get_dataframe());
    cout << endl;

    /* -------Adding data of vectors------- */
    cout << "Adding 2D vector to DataFrame:\n";
    vector<Column<int32_t>> vectors2(df_WIDTH, Column<int32_t>(df_HEIGHT));
    create_random_table(vectors2);
    df.add_data(vectors2); //-
    print_dataframe(df.get_dataframe());
    cout << endl;

    /* -------Adding data of a vector with column name------- */
    cout << "Adding 1D vector to DataFrame with column name:\n";
    Column<int32_t> vec2;
    create_random_array(vec2);
    df.add_data(vec2, "col1"); //-
    print_dataframe(df.get_dataframe());
    cout << endl;

    /* -------Adding data of vectors with column names------- */
    cout << "Adding 2D vector to DataFrame with column names:\n";
    vector<Column<int32_t>> vectors3(df_WIDTH, Column<int32_t>(df_HEIGHT));
    create_random_table(vectors3);
    df.add_data(vectors3, { "col2", "col3", "col4", "col5", "col6" }); //-
    print_dataframe(df.get_dataframe());
    cout << endl;

    /* -------Updating column names by overwriting indices------- */
    cout << "Setting column names by overwriting indices of dataframe:\n";
    df.set_columns({ "col2", "col3", "col4", "col5", "col6" }, 5);
    print_dataframe(df.get_dataframe());
    cout << endl;

    /* -------Updating column names by finding column name and overwriting------- */
    cout << "Updating column names by finding column name and overwriting:\n";
    df.update_column("col2", "col2_find_string");
    print_dataframe(df.get_dataframe());
    cout << endl;

    /* -------Updating column names by index value------- */
    cout << "Updating column names by index value:\n";
    df.update_column("col3", "col3_by_index");
    print_dataframe(df.get_dataframe());
    cout << endl;

    /* -------Replaces column at column index------- */
    cout << "Replaces column at column index (idx[0]):\n";
    Column<int32_t> vec3;
    create_random_array(vec3);
    df.update_columnval(0, vec3);
    print_dataframe(df.get_dataframe());
    cout << endl;

    /* -------Replaces column at column index------- */
    cout << "Replaces column at column name (col4):\n";
    Column<int32_t> vec4;
    create_random_array(vec4);
    df.update_columnval("col4", vec4);
    print_dataframe(df.get_dataframe());
    cout << endl;


}