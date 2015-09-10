
#include <random>
#include <algorithm>
#include <stdint.h>
#include "../DataDefinition.h"
#include <vector>
using namespace std;
template <typename T1>
vector<Node<T1, uint32_t>> generate__data(T1 bound,uint32_t data_size)
{
	random_device rd;
	vector<Node<T1, uint32_t>> result;
	mt19937 gen(rd());
	Node<T1, uint32_t> temp_node; 
	uniform_real_distribution<T1> dis(0, bound);
	for (uint32_t i = 0; i < data_size; i++)
	{
		temp_node.priority = i;
		temp_node.pos.x = dis(gen);
		temp_node.pos.y = dis(gen);
		result.push_back(temp_node);
	}
	return result;

}
template <typename T1>
vector<pair<Position<T1>, Position<T1>>> generate_query(T1 bound, uint32_t query_size)
{
	random_device rd;
	vector<pair<Position<T1>, Position<T1>>> result;
	Position<T1> a, b;
	T1 temp_min, temp_max;
	mt19937 gen(rd());
	
	uniform_real_distribution<T1> dis(-1, bound+1);
	for (uint32_t i = 0; i < query_size; i++)
	{
		a.x = dis(gen);
		a.y = dis(gen);
		b.x = dis(gen);
		b.y = dis(gen);
		temp_min = std::min(a.x, b.x);
		temp_max = std::max(a.x, b.x);
		a.x = temp_min;
		b.x = temp_max;
		temp_min = std::min(a.y, b.y);
		temp_max = std::max(a.y, b.y);
		a.y = temp_min;
		b.y = temp_max;
		result.push_back(make_pair(a, b));
	}
	return result;
}
