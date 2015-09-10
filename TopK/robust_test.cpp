#include "LinearSearch.h"
#include "./test/DataGenerate.h"
#include "NaiveKDTree.h"
#include "RangeTree.h"
#include "FractionalTree.h"

int main()
{
	

	vector<Node<double, uint32_t>>  temp_data;
	vector<pair<Position<double>, Position<double>>> temp_query;
	temp_data = generate__data<double>(100000, 100001);
	temp_query = generate_query<double>(100000, 1001);
	LinearSearch<double, uint32_t> ls(&temp_data[0], &temp_data[100000]);
	//NaiveKDTree<double, uint32_t> kd(&temp_data[0], &temp_data[100000], 32);
	//RangeTree<double, uint32_t> rt(&temp_data[0], &temp_data[100000], 32);
	//FractionalTree<double, uint32_t> ft(&temp_data[0], &temp_data[100000], 32);
	ls.CreateContext();
	//kd.CreateContext();
	//rt.CreateContext();
	//ft.CreateContext();
	
	
	
}