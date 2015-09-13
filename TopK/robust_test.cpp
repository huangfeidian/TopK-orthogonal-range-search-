#include "LinearSearch.h"
#include "AvxLinearSearchDetach.h"
#include "AvxLinearSearchPacked.h"
#include "./test/DataGenerate.h"
#include "NaiveKDTree.h"
#include "RangeTree.h"
#include "FractionalTree.h"
#include "CascadeLinear.h"
#include "PrioritySearchTree.h"
#include <iostream>
#include <assert.h>
//#include <boost/progress.hpp>
#define DATA_SIZE 100
#define QUERY_SIZE 100
int main()
{
	

	vector<Node<double, uint32_t>>  temp_data;
	vector<pair<Position<double>, Position<double>>> temp_query;
	temp_data = generate__data<double>(100000, DATA_SIZE+1);
	temp_query = generate_query<double>(100000, QUERY_SIZE+1);
	LinearSearch<double, uint32_t> ls(&temp_data[0], &temp_data[DATA_SIZE]);
	AvxLinearSearchDetach<double, uint32_t> avxd(&temp_data[0], &temp_data[DATA_SIZE]);
	AvxLinearSearchPacked<double, uint32_t> avxp(&temp_data[0], &temp_data[DATA_SIZE]);
	//CascadeLinear<double, uint32_t> cls(&temp_data[0], &temp_data[DATA_SIZE],2048);
	//PrioritySearchTree<double, uint32_t> ps(&temp_data[0], &temp_data[DATA_SIZE], 32);
	//KDTreeSearch<double, uint32_t> kd(&temp_data[0], &temp_data[DATA_SIZE], 32);
	//RangeTree<double, uint32_t> rt(&temp_data[0], &temp_data[DATA_SIZE], 32);
	//FractionalTree<double, uint32_t> ft(&temp_data[0], &temp_data[DATA_SIZE], 32);
	//boost::progress_display check_progress(100000);
	ls.CreateContext();
	avxd.CreateContext();
	avxp.CreateContext();
	//cls.CreateContext();
	//ps.CreateContext();
	//kd.CreateContext();
	//rt.CreateContext();
	//ft.CreateContext();
	std::cout << "Context completed" << std::endl;
	for (int i = 0; i < QUERY_SIZE; i++)
	{
		bool debug_need=false;
		vector<Node<double, uint32_t>> result_1, result_2, result_3, result_4,result_5,result_6,result_7,result_8;
		result_1 = ls.TopkSearch(temp_query[i].first, temp_query[i].second, 4);
		result_2 = avxd.TopkSearch(temp_query[i].first, temp_query[i].second, 4);
		result_3 =avxp.TopkSearch(temp_query[i].first, temp_query[i].second, 4);
		//result_4 = cls.TopkSearch(temp_query[i].first, temp_query[i].second, 40);
		//result_5 = ps.TopkSearch(temp_query[i].first, temp_query[i].second, 40);
		//result_6 = kd.TopkSearch(temp_query[i].first, temp_query[i].second, 40);
		//result_7 = rt.TopkSearch(temp_query[i].first, temp_query[i].second, 40);
		//result_8 = ft.TopkSearch(temp_query[i].first, temp_query[i].second, 40);

		if (result_1 != result_3)
		{
			result_3 = avxp.TopkSearch(temp_query[i].first, temp_query[i].second, 4);
			std::cout << " check fail in result3 " << std::endl;
			exit(1);
		}
		/*if (result_1 != result_4)
		{
			std::cout << " check fail in result4 " << std::endl;
			exit(1);
		}
		if (result_1 != result_5)
		{
			std::cout << " check fail in result5 " << std::endl;
			exit(1);
		}
		if (result_1 != result_6)
		{
			std::cout << " check fail in result6 " << std::endl;
			exit(1);
		}
		if (result_1 != result_7)
		{
			std::cout << " check fail in result7 " << std::endl;
			exit(1);
		}
		if (result_1 != result_8)
		{
			std::cout << " check fail in result8 " << std::endl;
			exit(1);
		}*/
		//++check_progress;
		

		

		
		
	}

	
	
	
}