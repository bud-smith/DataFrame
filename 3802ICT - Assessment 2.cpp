#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <string>
#include <cstdint>
#include <algorithm>
#include <map>

using namespace std;

constexpr uint32_t df_WIDTH = 5;
constexpr uint32_t df_HEIGHT = 5;

auto scan = [](auto x) { cin >> x; };
auto print = [](auto x) { cout << x << " "; };
auto println = [](auto x) { cout << x << "\n"; };

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
        data_vec->erase(data_vec->begin() + column_idx);
    }
    void delete_column(string column_name) {
        int32_t column_idx = find_column(column_name);
        data_vec->erase(data_vec->begin() + column_idx);
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

    void print_dataframe() {
        for (int32_t i = 0; i < shape(); i++) {
            for (int32_t j = 0; j < data_vec->at(0).size(); j++) {
                if (j != data_vec->at(0).size() - 1) {
                    print(data_vec->at(i)[j]);
                }
                else {
                    println(data_vec->at(i)[j]);
                }
            }
        }
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

    double min() {
        double min = INT_MAX;
        for (int i = 0; i < this->size(); i++) {
            if (this->at(i) < min) {
                min = this->at(i);
            }
        }
        return min;
    }
    double max() {
        double max = INT_MIN;
        for (double i = 0; i < this->size(); i++) {
            if (this->at(i) > max) {
                max = this->at(i);
            }
        }
        return max;
    }
    double mean() {
        double sum = 0;
        for (double i = 0; i < this->size(); i++) {
            sum += this->at(i);
        }
        return sum / (this->size());
    }
    double median() {
        vector<T> _vector;
        copy(this->begin(), this->end(), back_inserter(_vector));
        sort(_vector.begin(), _vector.end());
        return _vector[_vector.size() / 2];
    }
    double mode() {
        vector<T> _vector;
        copy(this->begin(), this->end(), back_inserter(_vector));
        sort(_vector.begin(), _vector.end());

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
    map<string, double> summary() {
        map<string, double> sum;

        sum.insert(pair<string, double>("min", min()));
        sum.insert(pair<string, double>("max", max()));
        sum.insert(pair<string, double>("mean", mean()));
        sum.insert(pair<string, double>("median", median()));
        sum.insert(pair<string, double>("mode", mode()));

        return sum;
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

int main() {
    /* -------Creating DataFrame------- */
    println("Empty DataFrame:");
    DataFrame<Column, int32_t> df_empty; //-
    df_empty.print_dataframe();
    println("");

    /* -------Creating DataFrame with parameters------- */
    println("Creating DataFrame with parameters:");
    vector<Column<int32_t>> vectors(df_WIDTH, Column<int32_t>(df_HEIGHT));
    create_random_table(vectors);
    DataFrame<Column, int32_t> df(&vectors); //-
    df.print_dataframe();
    println("");;

    /* -------Loading data of vectors to overwrite------- */
    println("Loading 2D vector to overwrite:");
    vector<Column<int32_t>> vectors1(df_WIDTH, Column<int32_t>(df_HEIGHT));
    create_random_table(vectors1);
    df.load_data(&vectors1); //-
    df.print_dataframe();
    print("\n");

    /* -------Loading data of a vector to overwrite------- */
    println("Loading 1D vector to overwrite:");
    Column<int32_t> vec;
    create_random_array(vec);
    df.load_data(vec); //-
    df.print_dataframe();
    println("");

    /* -------Adding data of a vector------- */
    println("Adding 1D vector to DataFrame:");
    Column<int32_t> vec1;
    create_random_array(vec1);
    df.add_data(vec1); //-
    df.print_dataframe();
    println("");

    /* -------Adding data of vectors------- */
    println("Adding 2D vector to DataFrame:");
    vector<Column<int32_t>> vectors2(df_WIDTH, Column<int32_t>(df_HEIGHT));
    create_random_table(vectors2);
    df.add_data(vectors2); //-
    df.print_dataframe();
    println("");

    /* -------Adding data of a vector with column name------- */
    println("Adding 1D vector to DataFrame with column name:");
    Column<int32_t> vec2;
    create_random_array(vec2);
    df.add_data(vec2, "col1"); //-
    df.print_dataframe();
    println("");

    /* -------Adding data of vectors with column names------- */
    println("Adding 2D vector to DataFrame with column names:");
    vector<Column<int32_t>> vectors3(df_WIDTH, Column<int32_t>(df_HEIGHT));
    create_random_table(vectors3);
    df.add_data(vectors3, { "col2", "col3", "col4", "col5", "col6" }); //-
    df.print_dataframe();
    println("");

    /* -------Updating column names by overwriting indices------- */
    println("Setting column names by overwriting indices of dataframe:");
    df.set_columns({ "col2", "col3", "col4", "col5", "col6" }, 5);
    df.print_dataframe();
    println("");

    /* -------Updating column names by finding column name and overwriting------- */
    println("Updating column names by finding column name and overwriting:");
    df.update_column("col2", "col2_find_string");
    df.print_dataframe();
    println("");

    /* -------Updating column names by index value------- */
    println("Updating column names by index value:");
    df.update_column("col3", "col3_by_index");
    df.print_dataframe();
    println("");

    /* -------Replaces column at column index------- */
    println("Replaces column at column index (idx[0]):");
    Column<int32_t> vec3;
    create_random_array(vec3);
    df.update_columnval(0, vec3);
    df.print_dataframe();
    println("");

    /* -------Replaces column at column index------- */
    println("Replaces column at column name (col4):");
    Column<int32_t> vec4;
    create_random_array(vec4);
    df.update_columnval("col4", vec4);
    df.print_dataframe();
    println("");

    /* -------Deletes column at column index------- */
    cout << "Deletes column at column index (idx[0]):\n";
    df.delete_column(0);
    df.print_dataframe();
    println("");

    /* -------Deletes column at column name------- */
    println("Deletes column at column name (col4):");
    df.delete_column("col4");
    df.print_dataframe();
    println("");

    /* -------Accessing index by column name------- */
    println("Accessing index by column name (col4):");
    for (auto iter : df["col4"]) {
        print(iter);
    }
    println("");

    /* -------Accessing index by value------- */
    println("Accessing index by value (idx[0]):");
    for (auto iter : df[0]) {
        print(iter);
    }
    println("");

    /* -------Statistical measures------- */
    println("Statistical measures:");
    print("min:");
    println(df[0].min());
    print("max:");
    println(df[0].max());
    print("mean:");
    println(df[0].mean());
    print("median:");
    println(df[0].median());
    print("mode:");
    println(df[0].mode());
    println("");
    println("Summary:");
    map<string, double> sum = df[0].summary();
    print(sum.find("min")->first);
    println(sum.find("min")->second);
    print(sum.find("max")->first);
    println(sum.find("max")->second);
    print(sum.find("mean")->first);
    println(sum.find("mean")->second);
    print(sum.find("median")->first);
    println(sum.find("median")->second);
    print(sum.find("mode")->first);
    println(sum.find("mode")->second);
    
}