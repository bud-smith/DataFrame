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
	vector<Column<T>> *data_vec;
public:
	// 1.1 - Initialisation
	// Default constructor
	DataFrame() {
		data_vec = new vector<Column<T>>();
	};
	// Parameterised constructor
	DataFrame(vector<Column<T>> *_vectors) {
		data_vec = new vector<Column<T>>();
		data_vec = _vectors;
	};
	// Destructor
	/*~DataFrame() {
		delete data_vec;
	};*/

	// 1.2 - Load Data
	// Loads vectors data into data_vec and overwrites original data
	void load_data(vector<Column<T>> *_vectors) {
		data_vec->clear();
		data_vec = _vectors;
	}
	void load_data(Column<T> *_vector) {
		data_vec->clear();
		data_vec->push_back(_vector);
	}
	// Adds data to the data_vec, that being a column of data.
	void add_data(Column<T> *_vector) {
		data_vec->push_back(_vector);
	}
	void add_data(vector<Column<T>> *_vectors) {
		for (auto iter : _vectors) {
			data_vec->push_back(iter);
		}
	}
	void add_data(Column<T> _vector, string column_name) {}
	void add_data(vector<Column<T>> _vectors, string column_name) {}

	// 1.3 - Setting Column Names
	void set_columns(string column_names[], uint32_t size) {
		for (int32_t i = 0; i < size; i++) {
			data_vec->at(0).at(i) = column_names[i];
		}
	}

	// 1.4 - Updating Column Names
	void update_column(string old_name, string new_name) {}
	void update_columns(uint32_t column, string name) {
		data_vec->at(0).at(column) = name;
	}

	// 1.5 - Updating Columns
	void update_columnval(uint32_t column_idx, vector<T> _vector) {}
	void update_columnval(string column_name, vector<T> _vector) {}
	void delete_column(uint32_t column_idx) {}
	void delete_column(string column_name) {}

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
		return (data_vec.size() * data_vec[0]->size());
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
	void summary() {}
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
	for (int32_t i = 0; i < 5; i++) {
		for (int32_t j = 0; j < 5; j++) {
			vectors[i][j] = (T)get_random_number();
		}
	}
}

template<typename T>
void create_random_array(Column<T>& vector) {
	for (int32_t i = 0; i < 5; i++) {
		vector[i].push_back((T)get_random_number());
	}
}

// Prints a 2D vector
template<typename T>
void print_2d_vector(const vector<Column<T>>& vectors) {
	for (int32_t i = 0; i < 5; i++) {
		for (int32_t j = 0; j < 5; j++) {
			cout << vectors[i][j] << " ";
		}
		cout << endl;
	}
}

int main() {
	vector<Column<int32_t>> vectors(df_WIDTH, Column<int32_t>(df_HEIGHT));

	create_random_table(vectors);

	DataFrame<Column, int32_t> df(&vectors);

	print_2d_vector(df.get_dataframe());

	Column<int32_t> vec;
	create_random_array(vec);
	
	df.load_data(&vec);
}