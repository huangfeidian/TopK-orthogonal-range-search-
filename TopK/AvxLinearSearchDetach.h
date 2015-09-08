#include "SearchContext.h"
#include "AvxLinearSearch.h"
template<typename T1, typename T2>
class AvxLinearSearchDetach :public SearchContext<T1, T2>
{
public:
	T1* all_x;
	T1* all_y;
public:
	AvxLinearSearchDetach(const Node<T1, T2>* begin, const Node<T1, T2>* end)
		:SearchContext<T1,T2>(begin, end)
	{

	}
	void CreateContext()
	{
		std::sort(input_nodes.begin(), input_nodes.end(), [](const Node<T1, T2>& a, const Node<T1, T2>& b)
		{
			if (a.priority > b.priority)
			{
				return true;
			}
		});
		all_x = _mm_malloc(sizeof(T1)*input_nodes.size(), 32);
		all_y = _mm_malloc(sizeof(T1)*input_nodes.size(), 32);
		for (int i = 0; i < input_nodes.size(); i++)
		{
			all_x[i] = input_nodes[i].pos.x;
			all_y[i] = input_nodes[i].pos.y;
		}

	}
	std::vector<Node<T1, T2>> TopkSearch(Position<T1> lower, Position<T1> upper, uint32_t k)
	{
		int counter = 0;
		

		std::vector<Node<T1, T2>> result;
		std::vector<uint32_t> indexs;
		indexs = topk_search_detail<T1>(all_x, all_y, input_nodes.size(), lower, upper, k);
		for (auto i : indexs)
		{
			result.push_back(input_nodes[i]);
		}
		return result;
	}
	~AvxLinearSearchDetach()
	{
		_mm_free(reinterpret_cast<void*>(all_x));
		_mm_free(reinterpret_cast<void*>(all_y));
	}
};