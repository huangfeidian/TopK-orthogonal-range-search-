#include "SearchContext.h"
#include "AvxLinearSearch.h"
template<typename T1, typename T2>
class AvxLinearSearchPacked :public SearchContext<T1, T2>
{
public:
	PackedPos<T1>* all_pos;
public:
	AvxLinearSearchPacked(const Node<T1, T2>* begin, const Node<T1, T2>* end)
		:SearchContext<T1,T2>(begin, end)
	{

	}
	void CreateContext()
	{
		std::sort(input_nodes.begin(), input_nodes.end(), [](const Node<T1, T2>& a, const Node<T1, T2>& b)
		{
			return a.priority > b.priority;
		});
		all_pos= reinterpret_cast<PackedPos<T1>*>(_mm_malloc(sizeof(T1)*input_nodes.size()*4, 32));
		for (int i = 0; i < input_nodes.size(); i++)
		{
			all_pos[i] = PackedPos<T1>(input_nodes[i].pos.x, input_nodes[i].pos.y);
		}
	}
	std::vector<Node<T1, T2>> TopkSearch(Position<T1> lower, Position<T1> upper, uint32_t k)
	{
		int counter = 0;


		std::vector<Node<T1, T2>> result;
		std::vector<uint32_t> indexs;
		indexs = topk_search_detail<T1>(all_pos, input_nodes.size(), lower, upper, k);
		for (auto i : indexs)
		{
			result.push_back(input_nodes[i]);
		}
		return result;
	}
	~AvxLinearSearchPacked()
	{
		_mm_free(reinterpret_cast<void*>(all_pos));
	}
};