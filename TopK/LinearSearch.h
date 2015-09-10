#include "SearchContext.h"
template<typename T1,typename T2>
class LinearSearch:public SearchContext<T1,T2>
{
public:
	LinearSearch(const Node<T1, T2>* begin, const Node<T1, T2>* end)
		:SearchContext<T1,T2>(begin,end)
	{
		
	}
	void CreateContext()
	{
		std::sort(input_nodes.begin(), input_nodes.end(), [](const Node<T1,T2>& a, const Node<T1,T2>& b)
		{
			return a.priority > b.priority;
			
		});
	}
	std::vector<Node<T1, T2>> TopkSearch(Position<T1> lower, Position<T1> upper, uint32_t k)
	{
		int counter = 0;
		std::vector<Node<T1, T2>> result;
		for (const auto& i : input_nodes)
		{
			if (i.pos.x >= lower.x&&i.pos.x <= upper.x&&i.pos.y>=lower.y&&i.pos.y<=upper.y)
			{
				result.push_back(i);
				counter++;
				if (counter == k)
				{
					break;
				}
			}
		}
		return result;
	}
	~LinearSearch()
	{
		//do nothing
	}
};