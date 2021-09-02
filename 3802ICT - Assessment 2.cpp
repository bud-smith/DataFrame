#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <string>
#include <cstdint>

using namespace std;

template<typename T>
class DataFrame {
private:
	// data_vec is the storage of the data frames.
	vector<vector<T>> data_vec;
public:
	// 1.1 - Initialisation
	// Default constructor
	DataFrame() {};
	// Parameterised constructor
	DataFrame(vector<vector<T>> vectors) {
		data_vec = vectors;
	};

	// 1.2 - Load Data
	// Loads vectors data into data_vec and overwrites original data
	void load_data(vector<vector<T>> _vectors) {
		data_vec = _vectors;
	}
	void load_data(vector<T> _vector) {
		data_vec = _vector;
	}
	// Adds data to the data_vec, that being a column of data.
	void add_data(vector<T> _vector) {
		data_vec.push_back(_vector);
	}
	void add_data(vector<vector<T>> _vectors) {}
	void add_data(vector<T> _vector, string column_name) {}
	void add_data(vector<vector<T>> _vectors, string column_name) {}

	// 1.3 - Setting Column Names
	void set_columns(string column_names[], uint32_t size) {
		for (size_t i = 0; i < size; i++) {
			data_vec[0][i] = column_names[i];
		}
	}

	// 1.4 - Updating Column Names
	void update_column(string old_name, string new_name) {}
	void update_columns(uint32_t column, string name) {}

	// 1.5 - Updating Columns
	void update_columnval(uint32_t column_idx, vector<T> _vector) {}
	void update_columnval(string column_name, vector<T> _vector) {}
	void delete_column(uint32_t column_idx) {}
	void delete_column(string column_name) {}
	
	// 1.6 - Get Columns
	vector<T> &operator[](uint32_t column_idx) {
		return data_vec[column_idx];
	}

	// 1.7 - Data Frame Metadata
	int size() {
		return (data_vec.size() * data_vec[0].size());
	}
	int shape() {
		return (data_vec[0].size());
	}

	// 1.8 - Statistical Measures
	void min() {}
	void max() {}
	void mean() {}
	void median() {}
	void mode() {}
	void summary() {}

	vector<vector<T>> get_dataframe() {
		return data_vec;
	}
};

template<typename T>
class Column {
private:
	vector<T> column_data;
public:
	void min() {}
	void max() {}
	void mean() {}
	void median() {}
	void mode() {}
	void summary() {}
};

// Generates individual random numbers
string get_random_number() {
	random_device dev;
	mt19937 twister(dev());
	uniform_int_distribution<int> dist(0, 9);

	return to_string(dist(twister));
}

// Assigns the values of each index of the 2D vector to random values
template<typename T>
void create_random_table(vector<vector<T>> &vectors) {
	for (size_t i = 0; i < 5; i++) {
		vectors[0][i] = "-";
	}
	for (size_t i = 1; i < 5; i++) {
		for (size_t j = 0; j < 5; j++) {
			vectors[i][j] = get_random_number();
		}
	}
}

// Prints the 2D vector
template<typename T>
void print_2d_vector(const vector<vector<T>> &vectors) {
	for (size_t i = 0; i < 5; i++) {
		for (size_t j = 0; j < 5; j++) {
			cout << vectors[i][j] << " ";
		}
		cout << endl;
	}
}

int main() {
	vector<vector<string>> vectors(5, std::vector<string>(5));

	create_random_table(vectors);

	DataFrame<string> df(vectors);

	print_2d_vector(vectors);

	string column_names[] = { "col1", "col2", "col3" };
	df.set_columns(column_names, size(column_names));

	print_2d_vector(df.get_dataframe());

	cout << df[0].min() << endl;

}