#include "LinearSearch.h"
#include "./test/DataGenerate.h"
#include "NaiveKDTree.h"
#include "RangeTree.h"
#include "FractionalTree.h"
#include "CascadeLinear.h"
#include "PrioritySearchTree.h"
#include <iostream>
#include <chrono>
int main()
{


	vector<Node<double, uint32_t>>  temp_data;
	std::chrono::duration<double, std::milli> time_spent;
	vector<pair<Position<double>, Position<double>>> temp_query;
	temp_data = generate__data<double>(100000, 100001);
	temp_query = generate_query<double>(100000, 100001);
	LinearSearch<double, uint32_t> ls(&temp_data[0], &temp_data[100000]);
	//CascadeLinear<double, uint32_t> cls(&temp_data[0], &temp_data[50000],2048);
	PrioritySearchTree<double, uint32_t> ps(&temp_data[0], &temp_data[100000], 32);
	/*NaiveKDTree<double, uint32_t> kd(&temp_data[0], &temp_data[50000], 32);
	RangeTree<double, uint32_t> rt(&temp_data[0], &temp_data[50000], 32);
	FractionalTree<double, uint32_t> ft(&temp_data[0], &temp_data[50000], 32);*/
	//cls.CreateContext();
	auto begin = std::chrono::high_resolution_clock::now();
	ls.CreateContext();
	auto end = std::chrono::high_resolution_clock::now();
	time_spent = end - begin;
	std::cout << "time spent in ls context creation is " << time_spent.count() << " ms" << std::endl;

	begin = std::chrono::high_resolution_clock::now();
	ps.CreateContext();
	end = std::chrono::high_resolution_clock::now();
	time_spent = end - begin;
	std::cout << "time spent in ps context creation is " << time_spent.count() << " ms" << std::endl;
	
	//kd.CreateContext();
	//rt.CreateContext();
	//ft.CreateContext();
	std::cout << "Context completed" << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100000; i++)
	{
		vector<Node<double, uint32_t>> result_1;
		result_1 = ls.TopkSearch(temp_query[i].first, temp_query[i].second, 20);
	}
	end = std::chrono::high_resolution_clock::now();
	time_spent = end - begin;
	std::cout << "time spent in ls searchis " << time_spent.count() << " ms" << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100000; i++)
	{
		vector<Node<double, uint32_t>> result_1;
		result_1 = ps.TopkSearch(temp_query[i].first, temp_query[i].second, 20);
	}
	end = std::chrono::high_resolution_clock::now();
	time_spent = end - begin;
	std::cout << "time spent in ps  search is " << time_spent.count()<<" ms" << std::endl;




}