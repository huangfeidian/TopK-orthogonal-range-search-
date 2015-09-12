#include "DataDefinition.h"
#include "SearchContext.h"
#include <queue>
template <typename T1,typename T2>
class CascadeLinear :public SearchContext<T1, T2>
{
	using result_queue_type = std::priority_queue<Node<T1, T2>, std::vector<Node<T1, T2>>, PriorityCmp<T1, T2>>;
	class CascadeLinearNode
	{
	public:
		Node<T1, T2>* x_sorted_nodes;
		Node<T1, T2>* y_sorted_nodes;
		Node<T1, T2>* priority_sorted_nodes;
		uint32_t* x_cascade_index;
		uint32_t* y_cascade_index;
		uint32_t size;
		bool is_head;
		bool is_leaf;
		CascadeLinearNode* next;
	};
	CascadeLinearNode* CreateLeaf(Node<T1, T2>* begin, Node<T1, T2>* end, uint32_t block_size)
	{
		CascadeLinearNode* result_node;
		uint32_t size;
		result_node = new CascadeLinearNode;
		result_node->x_sorted_nodes = result_node->y_sorted_nodes = nullptr;
		result_node->priority_sorted_nodes = nullptr;
		result_node->x_cascade_index = result_node->y_cascade_index = nullptr;
		result_node->is_head = false;
		if (end - begin <= block_size)
		{
			size = end - begin;
		}
		else
		{
			size = block_size;
			std::nth_element(begin, begin + size, end, PriorityCmp<T1, T2>());
		}
		result_node->size = size;
		result_node->x_sorted_nodes = new Node<T1, T2>[size];
		result_node->y_sorted_nodes = new Node<T1, T2>[size];
		std::copy(begin, begin + size, result_node->x_sorted_nodes);
		std::copy(begin, begin + size, result_node->y_sorted_nodes);
		std::sort(result_node->x_sorted_nodes, result_node->x_sorted_nodes + size, xpos_cmp<T1, T2>);
		std::sort(result_node->y_sorted_nodes, result_node->y_sorted_nodes + size, ypos_cmp<T1, T2>);
		if (end - begin <= block_size)
		{
			result_node->next = nullptr;
			result_node->is_leaf = true;
		}
		else
		{
			result_node->is_leaf = false;
			result_node->next = CreateLeaf(begin + size, end, block_size * 2);
			result_node->x_cascade_index = new uint32_t[result_node->size];
			result_node->y_cascade_index = new uint32_t[result_node->size];
			uint32_t x_index, y_index;
			x_index = y_index = 0;
			uint32_t son_size = result_node->next->size;
			for (int i = 0; i < size; i++)
			{
				while (x_index != son_size&&result_node->next->x_sorted_nodes[x_index].pos.x < result_node->x_sorted_nodes[i].pos.x)
				{
					x_index++;
				}
				result_node->x_cascade_index[i] = x_index;
				while (y_index != son_size&&result_node->next->y_sorted_nodes[y_index].pos.y < result_node->y_sorted_nodes[i].pos.y)
				{
					y_index++;
				}
				result_node->y_cascade_index[i] = y_index;
			}
		}
		return result_node;
	}
	CascadeLinearNode* CreateHead(Node<T1, T2>* begin, Node<T1, T2>* end, uint32_t block_size)
	{
		CascadeLinearNode* result_node;
		uint32_t size;
		result_node = new CascadeLinearNode;
		result_node->x_sorted_nodes = result_node->y_sorted_nodes = nullptr;
		result_node->x_cascade_index = result_node->y_cascade_index = nullptr;
		result_node->is_head = true;
		if (end - begin <= block_size)
		{
			size = end - begin;
		}
		else
		{
			size = block_size;
			std::nth_element(begin, begin + size, end, PriorityCmp<T1, T2>());
		}
		result_node->size = size;
		result_node->priority_sorted_nodes = new Node<T1, T2>[size];
		std::copy(begin, begin + size, result_node->priority_sorted_nodes);
		std::sort(result_node->priority_sorted_nodes, result_node->priority_sorted_nodes + size, PriorityCmp<T1, T2>());
		if (end - begin <= block_size)
		{
			result_node->next = nullptr;
		}
		else
		{
			result_node->next = CreateLeaf(begin + size, end, block_size * 2);
		}
		return result_node;
	}
	pair<pair<uint32_t, uint32_t>, pair<uint32_t, uint32_t>> cascade_range(Position<T1> lower, Position<T1> upper, CascadeLinearNode* cur_node,
		pair<uint32_t, uint32_t> x_range, pair<uint32_t, uint32_t> y_range)
	{
		uint32_t new_x_begin, new_x_end;
		uint32_t new_y_begin, new_y_end;
		CascadeLinearNode* next = cur_node->next;
		uint32_t lx_begin, lx_end;
		uint32_t ux_begin, ux_end;
		uint32_t ly_begin, ly_end;
		uint32_t uy_begin, uy_end;
		lx_begin = x_range.first == 0 ? 0 : cur_node->x_cascade_index[x_range.first - 1];
		lx_end = x_range.first == cur_node->size ? next->size : cur_node->x_cascade_index[x_range.first];
		ux_begin = x_range.second == 0 ? 0 : cur_node->x_cascade_index[x_range.second - 1];
		ux_end = x_range.second == cur_node->size ? next->size : cur_node->x_cascade_index[x_range.second];

		ly_begin = y_range.first == 0 ? 0 : cur_node->y_cascade_index[y_range.first - 1];
		ly_end = y_range.first == cur_node->size ? next->size : cur_node->y_cascade_index[y_range.first];
		uy_begin = y_range.second == 0 ? 0 : cur_node->y_cascade_index[y_range.second - 1];
		uy_end = y_range.second == cur_node->size ? next->size : cur_node->y_cascade_index[y_range.second];
		new_x_begin = std::distance(next->x_sorted_nodes, std::lower_bound(next->x_sorted_nodes + lx_begin, next->x_sorted_nodes + lx_end, 
			lower.x, [](const Node<T1, T2>& a, T1 b)
		{
			return a.pos.x < b;
		}));
		new_x_end = std::distance(next->x_sorted_nodes, std::upper_bound(next->x_sorted_nodes + ux_begin, next->x_sorted_nodes +ux_end
			, upper.x, [](T1 b, const Node<T1, T2>& a)
		{
			return b<a.pos.x;
		}));
		new_y_begin = std::distance(next->y_sorted_nodes, std::lower_bound(next->y_sorted_nodes+ly_begin, next->y_sorted_nodes + ly_end,
			lower.y, [](const Node<T1, T2>& a, T1 b)
		{
			return a.pos.y < b;
		}));
		new_y_end = std::distance(next->y_sorted_nodes, std::upper_bound(next->y_sorted_nodes + uy_begin, next->y_sorted_nodes + uy_end
			, upper.y, [](T1 b, const Node<T1, T2>& a)
		{
			return b<a.pos.y;
		}));
		
		return make_pair(make_pair(new_x_begin, new_x_end), make_pair(new_y_begin, new_y_end));
	}
	void recursive_search(Position<T1> lower, Position<T1> upper, CascadeLinearNode* cur_node, pair<uint32_t, uint32_t> x_range,
		pair<uint32_t, uint32_t> y_range, int k, result_queue_type& result_queue)
	{

		uint32_t x_range_size, y_range_size;
		x_range_size = x_range.second - x_range.first;
		y_range_size = y_range.second - y_range.first;
		if (x_range_size != 0 && y_range_size != 0)
		{
			if (x_range_size < y_range_size)
			{
				for (int i = x_range.first; i < x_range.second; i++)
				{
					if (cur_node->x_sorted_nodes[i].pos.y >= lower.y&&cur_node->x_sorted_nodes[i].pos.y <= upper.y)
					{
						if (result_queue.size() == k)
						{
							if (cur_node->x_sorted_nodes[i].priority > result_queue.top().priority)
							{
								result_queue.pop();
								result_queue.push(cur_node->x_sorted_nodes[i]);
							}
						}
						else
						{
							result_queue.push(cur_node->x_sorted_nodes[i]);
						}
					}
				}
			}
			else
			{
				for (int i = y_range.first; i < y_range.second; i++)
				{
					if (cur_node->y_sorted_nodes[i].pos.x >= lower.x&&cur_node->y_sorted_nodes[i].pos.x <= upper.x)
					{
						if (result_queue.size() == k)
						{
							if (cur_node->y_sorted_nodes[i].priority > result_queue.top().priority)
							{
								result_queue.pop();
								result_queue.push(cur_node->y_sorted_nodes[i]);
							}
						}
						else
						{
							result_queue.push(cur_node->y_sorted_nodes[i]);
						}
					}
				}
			}
		}
		
		if (cur_node->next == nullptr)
		{
			return;
		}
		else
		{
			auto new_bound = cascade_range(lower, upper, cur_node, x_range, y_range);
			recursive_search(lower, upper, cur_node->next, new_bound.first, new_bound.second, k, result_queue);
		}
	}
	void head_search(Position<T1> lower, Position<T1> upper, int k, result_queue_type& result_queue)
	{
		for (int i = 0; i < head->size; i++)
		{
			T1 current_x, current_y;
			current_x = head->priority_sorted_nodes[i].pos.x;
			current_y = head->priority_sorted_nodes[i].pos.y;
			if (current_x >= lower.x&&current_x <= upper.x&&current_y <= upper.y&&current_y >= lower.y)
			{
				result_queue.push(head->priority_sorted_nodes[i]);
				if (result_queue.size() == k)
				{
					return;
				}
			}
		}
		if (result_queue.size() != k&&head->next)
		{
			uint32_t lowerx_index, lowery_index, uppery_index, upperx_index;
			CascadeLinearNode* next = head->next;
			lowerx_index = std::distance(next->x_sorted_nodes,std::lower_bound(next->x_sorted_nodes, next->x_sorted_nodes + next->size, lower.x, [](const Node<T1, T2>& a, T1 b)
			{
				return a.pos.x < b;
			}));
			lowery_index = std::distance(next->y_sorted_nodes,std::lower_bound(next->y_sorted_nodes, next->y_sorted_nodes + next->size, lower.y, [](const Node<T1, T2>& a, T1 b)
			{
				return a.pos.y < b;
			}));
			upperx_index = std::distance(next->x_sorted_nodes, std::upper_bound(next->x_sorted_nodes, next->x_sorted_nodes + next->size, upper.x, [](T1 b, const Node<T1, T2>& a)
			{
				return b<a.pos.x;
			}));
			uppery_index = std::distance(next->y_sorted_nodes,std::upper_bound(next->y_sorted_nodes, next->y_sorted_nodes + next->size, upper.y, [](T1 b, const Node<T1, T2>& a)
			{
				return b<a.pos.y;
			}));
			recursive_search(lower, upper, next, make_pair(lowerx_index, upperx_index), make_pair(lowery_index, uppery_index), k, result_queue);
		}
	}
	void recursive_free(CascadeLinearNode* cur_node)
	{
		if (cur_node == nullptr)
		{
			return;
		}
		if (cur_node->is_head)
		{
			delete [] cur_node->priority_sorted_nodes;
		}
		else
		{
			delete [] cur_node->x_sorted_nodes;
			delete [] cur_node->y_sorted_nodes;
			if (!cur_node->is_leaf)
			{
				delete [] cur_node->x_cascade_index;
				delete [] cur_node->y_cascade_index;
			}
		}
		recursive_free(cur_node->next);

	}
	private:
		CascadeLinearNode* head;
public:
	const uint32_t head_block_size;
	
	CascadeLinear(const Node<T1,T2>* begin,const Node<T1,T2>* end,uint32_t in_block_size)
		:SearchContext<T1, T2>(begin, end), head_block_size(in_block_size)
	{

	}
	
	void CreateContext()
	{
		if (input_nodes.size() == 0)
		{
			head = nullptr;
		}
		else
		{
			head = CreateHead(&input_nodes[0], &input_nodes[0] + input_nodes.size(), head_block_size);
		}
	}
	std::vector<Node<T1, T2>> TopkSearch(Position<T1> lower, Position<T1> upper, uint32_t k)
	{
		std::vector<Node<T1, T2>> result;
		result_queue_type result_queue;
		if (k == 0 || input_nodes.size() == 0)
		{
			return result;
		}
		head_search(lower, upper, k, result_queue);
		while (result_queue.size() != 0)
		{
			result.push_back(result_queue.top());
			result_queue.pop();
		}
		std::reverse(result.begin(), result.end());
		return result;
	}
	
	~CascadeLinear()
	{
		recursive_free(head);
	}
};