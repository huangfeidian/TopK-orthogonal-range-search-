#include "LinearSearch.h"
#include "./test/DataGenerate.h"
#include "NaiveKDTree.h"
#include "RangeTree.h"
#include "FractionalTree.h"
#include "CascadeLinear.h"
#include "PrioritySearchTree.h"
#include "AvxLinearSearchPacked.h"
#include "AvxLinearSearchDetach.h"
#include <iostream>
#include <chrono>
int main()
{


	vector<Node<double, uint32_t>>  temp_data;
	std::chrono::duration<double, std::milli> time_spent;
	vector<pair<Position<double>, Position<double>>> temp_query;
	temp_data = generate__data<double>(100000, 10000001);
	temp_query = generate_query<double>(100000, 1000001);
	LinearSearch<double, uint32_t> ls(&temp_data[0], &temp_data[10000000]);
	AvxLinearSearchDetach<double, uint32_t> as(&temp_data[0], &temp_data[10000000]);
	//CascadeLinear<double, uint32_t> cls(&temp_data[0], &temp_data[50000],2048);
	PrioritySearchTree<double, uint32_t> ps64(&temp_data[0], &temp_data[10000000], 64);
	PrioritySearchTree<double, uint32_t> ps32(&temp_data[0], &temp_data[10000000], 32);
	PrioritySearchTree<double, uint32_t> ps16(&temp_data[0], &temp_data[10000000], 16);
	PrioritySearchTree<double, uint32_t> ps8(&temp_data[0], &temp_data[10000000], 8);
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
	as.CreateContext();
	end = std::chrono::high_resolution_clock::now();
	time_spent = end - begin;
	std::cout << "time spent in as context creation is " << time_spent.count() << " ms" << std::endl;

	begin = std::chrono::high_resolution_clock::now();
	ps64.CreateContext();
	end = std::chrono::high_resolution_clock::now();
	time_spent = end - begin;
	std::cout << "time spent in ps64 context creation is " << time_spent.count() << " ms" << std::endl;
	
	begin = std::chrono::high_resolution_clock::now();
	ps32.CreateContext();
	end = std::chrono::high_resolution_clock::now();
	time_spent = end - begin;
	std::cout << "time spent in ps32 context creation is " << time_spent.count() << " ms" << std::endl;

	begin = std::chrono::high_resolution_clock::now();
	ps16.CreateContext();
	end = std::chrono::high_resolution_clock::now();
	time_spent = end - begin;
	std::cout << "time spent in ps16 context creation is " << time_spent.count() << " ms" << std::endl;

	begin = std::chrono::high_resolution_clock::now();
	ps8.CreateContext();
	end = std::chrono::high_resolution_clock::now();
	time_spent = end - begin;
	std::cout << "time spent in ps8 context creation is " << time_spent.count() << " ms" << std::endl;
	
	//kd.CreateContext();
	//rt.CreateContext();
	//ft.CreateContext();
	std::cout << "Context completed" << std::endl;

	begin = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000; i++)
	{
		vector<Node<double, uint32_t>> result_1;
		result_1 = ls.TopkSearch(temp_query[i].first, temp_query[i].second, 20);
	}
	end = std::chrono::high_resolution_clock::now();
	time_spent = end - begin;
	std::cout << "time spent in ls search is " << time_spent.count() << " ms" << std::endl;

	begin = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000; i++)
	{
		vector<Node<double, uint32_t>> result_1;
		result_1 = as.TopkSearch(temp_query[i].first, temp_query[i].second, 20);
	}
	end = std::chrono::high_resolution_clock::now();
	time_spent = end - begin;
	std::cout << "time spent in as search is " << time_spent.count() << " ms" << std::endl;

	begin = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000; i++)
	{
		vector<Node<double, uint32_t>> result_1;
		result_1 = ps64.TopkSearch(temp_query[i].first, temp_query[i].second, 20);
	}
	end = std::chrono::high_resolution_clock::now();
	time_spent = end - begin;
	std::cout << "time spent in ps64  search is " << time_spent.count()<<" ms" << std::endl;

	begin = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000; i++)
	{
		vector<Node<double, uint32_t>> result_1;
		result_1 = ps32.TopkSearch(temp_query[i].first, temp_query[i].second, 20);
	}
	end = std::chrono::high_resolution_clock::now();
	time_spent = end - begin;
	std::cout << "time spent in ps32  search is " << time_spent.count() << " ms" << std::endl;

	begin = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000; i++)
	{
		vector<Node<double, uint32_t>> result_1;
		result_1 = ps16.TopkSearch(temp_query[i].first, temp_query[i].second, 20);
	}
	end = std::chrono::high_resolution_clock::now();
	time_spent = end - begin;
	std::cout << "time spent in ps16  search is " << time_spent.count() << " ms" << std::endl;

	begin = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000; i++)
	{
		vector<Node<double, uint32_t>> result_1;
		result_1 = ps8.TopkSearch(temp_query[i].first, temp_query[i].second, 20);
	}
	end = std::chrono::high_resolution_clock::now();
	time_spent = end - begin;
	std::cout << "time spent in ps8  search is " << time_spent.count() << " ms" << std::endl;

	



}