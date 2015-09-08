#include <vector>
#include <algorithm>
#include "DataDefinition.h"

template<typename T1,typename T2>
class SearchContext
{
public:
	std::vector<Node<T1,T2>> input_nodes;
	SearchContext(const Node<T1, T2>* begin, const Node<T1, T2>* end)
	{
		input_nodes.insert(input_nodes.begin(), begin, end);
	}
	virtual void CreateContext() = 0;
	virtual void ~SearchContext() ;
	virtual std::vector<Node<T1, T2>> TopkSearch(Position<T1> lower, Position<T2> upper, uint32_t k) = 0;
};