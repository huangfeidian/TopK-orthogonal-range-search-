#include "LinearSearch.h"
#include "./test/DataGenerate.h"
#include "NaiveKDTree.h"
#include "RangeTree.h"
#include "FractionalTree.h"
#include "CascadeLinear.h"
#include "PrioritySearchTree.h"
#include <iostream>
#include <assert.h>
int main()
{
	

	vector<Node<double, uint32_t>>  temp_data;
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
	ls.CreateContext();
	ps.CreateContext();
	//kd.CreateContext();
	//rt.CreateContext();
	//ft.CreateContext();
	std::cout << "Context completed" << std::endl;
	for (int i = 0; i < 100000; i++)
	{
		bool debug_need=false;
		vector<Node<double, uint32_t>> result_1, result_2, result_3, result_4,result_5,result_6;
		result_1 = ls.TopkSearch(temp_query[i].first, temp_query[i].second, 20);
		//result_2 = kd.TopkSearch(temp_query[i].first, temp_query[i].second, 20);
		//result_3 = rt.TopkSearch(temp_query[i].first, temp_query[i].second, 20);
		//result_4 = ft.TopkSearch(temp_query[i].first, temp_query[i].second, 20);
		//result_5 = cls.TopkSearch(temp_query[i].first, temp_query[i].second, 20);
		result_6 = ps.TopkSearch(temp_query[i].first, temp_query[i].second, 20);
		//assert(result_6.size() != result_1.size());
		if (result_6.size() != result_1.size())
		{
			debug_need = true;
		}
		//assert(result_1.size() == result_6.size());
		//assert(result_2.size() == result_3.size());
		//assert(result_3.size() == result_4.size());
		//assert(result_4.size() == result_5.size());
		int size = result_1.size();
		if (!debug_need)
		{
			for (int j = 0; j < size; j++)
			{
				if (!(result_1[j] == result_6[j]))
				{
					debug_need = true;
				}

				/*assert(result_2[i] == result_3[i]);
				assert(result_3[i] == result_4[i]);
				assert(result_4[i] == result_5[i]);*/
			}
		}
		
		if (debug_need)
		{
			result_6 = ps.TopkSearch(temp_query[i].first, temp_query[i].second, 20);
		}
		
	}

	
	
	
}