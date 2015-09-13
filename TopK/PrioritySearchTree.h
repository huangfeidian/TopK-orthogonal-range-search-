#include "SearchContext.h"
#include <stdint.h>
#include <queue>
using std::pair;
using std::make_pair;

template <typename T1, typename T2>
class PrioritySearchTree :public SearchContext<T1, T2>
{
private:
	using result_queue_type = std::priority_queue<Node<T1, T2>, std::vector<Node<T1, T2>>, PriorityCmp<T1, T2>>;
	class PSTreeNode
	{
	public:
		uint32_t size;
		bool is_x;
		PSTreeNode* left;
		PSTreeNode* right;
		bool is_leaf;
		Node<T1, T2>* local_nodes;
		Position<T1> lower_bound;
		Position<T1> upper_bound;
		Position<T1> local_lower_bound;
		Position<T1> local_upper_bound;
		PSTreeNode()
		{

		}
	};
	PSTreeNode* head;
	const int min_split_size;
	PSTreeNode* CreateLeaf(Node<T1, T2>* begin, Node<T1, T2>* end)
	{
		PSTreeNode* new_node;
		uint32_t size = end - begin;
		new_node = new PSTreeNode();
		new_node->left = new_node->right = nullptr;
		new_node->is_leaf = true;
		new_node->is_x = true;
		new_node->size = size;
		new_node->local_nodes = begin;
		T1 lower_x, upper_x, lower_y, upper_y;
		upper_x = upper_y = std::numeric_limits<T1>::min();
		lower_x = lower_y = std::numeric_limits<T1>::max();
		for (int i = 0; i < size; i++)
		{
			lower_x = lower_x >(begin + i)->pos.x ? (begin + i)->pos.x : lower_x;
			lower_y = lower_y > (begin + i)->pos.y ? (begin + i)->pos.y : lower_y;
			upper_x = upper_x > (begin + i)->pos.x ? upper_x : (begin + i)->pos.x;
			upper_y = upper_y > (begin + i)->pos.y ? upper_y : (begin + i)->pos.y;
		}
		new_node->lower_bound.x = lower_x;
		new_node->upper_bound.x = upper_x;
		new_node->lower_bound.y = lower_y;
		new_node->upper_bound.y = upper_y;

		new_node->local_lower_bound.x = lower_x;
		new_node->local_upper_bound.x = upper_x;
		new_node->local_lower_bound.y = lower_y;
		new_node->local_upper_bound.y = upper_y;

		std::sort(begin, end, [](const Node<T1, T2>& a, const Node<T1, T2>& b)
		{
			return a.priority > b.priority;
		});
		return new_node;
	}
	PSTreeNode* CreateNode(Node<T1, T2>* begin, Node<T1, T2>* end, bool is_x)
	{
		PSTreeNode* new_node;
		int size = end - begin;
		if (size == 0)
		{
			return nullptr;
		}
		if (size <= 2*min_split_size)
		{
			new_node = CreateLeaf(begin, end);
			return new_node;
		}
		new_node = new PSTreeNode;
		new_node->size = min_split_size;
		new_node->local_nodes = begin;
		T1 lower_x, upper_x, lower_y, upper_y;
		upper_x = upper_y = std::numeric_limits<T1>::min();
		lower_x = lower_y = std::numeric_limits<T1>::max();
		for (int i = 0; i < size; i++)
		{
			lower_x = lower_x >(begin + i)->pos.x ? (begin + i)->pos.x : lower_x;
			lower_y = lower_y > (begin + i)->pos.y ? (begin + i)->pos.y : lower_y;
			upper_x = upper_x > (begin + i)->pos.x ? upper_x : (begin + i)->pos.x;
			upper_y = upper_y > (begin + i)->pos.y ? upper_y : (begin + i)->pos.y;
		}
		new_node->lower_bound.x = lower_x;
		new_node->upper_bound.x = upper_x;
		new_node->lower_bound.y = lower_y;
		new_node->upper_bound.y = upper_y;

		std::nth_element(begin, begin + min_split_size, end, [](const Node<T1, T2>& a, const Node<T1, T2>& b)
		{
			return a.priority > b.priority;
		});
		std::sort(begin, begin + min_split_size, [](const Node<T1, T2>& a, const Node<T1, T2>& b)
		{
			return a.priority > b.priority;
		});

		upper_x = upper_y = std::numeric_limits<T1>::min();
		lower_x = lower_y = std::numeric_limits<T1>::max();
		for (int i = 0; i < min_split_size; i++)
		{
			lower_x = lower_x >(begin + i)->pos.x ? (begin + i)->pos.x : lower_x;
			lower_y = lower_y > (begin + i)->pos.y ? (begin + i)->pos.y : lower_y;
			upper_x = upper_x > (begin + i)->pos.x ? upper_x : (begin + i)->pos.x;
			upper_y = upper_y > (begin + i)->pos.y ? upper_y : (begin + i)->pos.y;
		}
		new_node->local_lower_bound.x = lower_x;
		new_node->local_upper_bound.x = upper_x;
		new_node->local_lower_bound.y = lower_y;
		new_node->local_upper_bound.y = upper_y;

		begin += min_split_size;
		size = end - begin;
		new_node->is_leaf = false;
		new_node->is_x = is_x;
		if (is_x)
		{
			std::nth_element(begin, begin + size / 2, end, xpos_cmp<T1, T2>);
		}
		else
		{
			std::nth_element(begin, begin + size / 2, end, ypos_cmp<T1, T2>);
		}
		new_node->left = CreateNode(begin, begin + size / 2, !is_x);
		new_node->right = CreateNode(begin + size / 2, end, !is_x);
		return new_node;
	}
public:
	PrioritySearchTree(const Node<T1, T2>* begin, const Node<T1, T2>* end, int in_min_split_size)
		:SearchContext<T1, T2>(begin, end), min_split_size(in_min_split_size)
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
			head = CreateNode(&input_nodes[0], &input_nodes[input_nodes.size() - 1] + 1, true);
		}
	}
	std::vector<Node<T1, T2>> TopkSearch(Position<T1> lower, Position<T1> upper, uint32_t k)
	{
		std::vector<Node<T1, T2>> result;
		if (input_nodes.size() == 0 || k == 0)
		{
			return result;
		}
		result_queue_type result_queue;
		queue_search(lower, upper, k, result_queue);
		while (result_queue.size() != 0)
		{
			result.push_back(result_queue.top());
			result_queue.pop();
		}
		std::reverse(result.begin(), result.end());
		return result;
	}
	void inside_search(uint32_t k, PSTreeNode* cur_node, result_queue_type& result_queue)
	{
		for (int i = 0; i < cur_node->size; i++)
		{
			if (result_queue.size() == k)
			{
				if ((cur_node->local_nodes + i)->priority < result_queue.top().priority)
				{
					break;
				}
				else
				{
					result_queue.pop();
					result_queue.push(*(cur_node->local_nodes + i));
				}

			}
			else
			{
				result_queue.push(*(cur_node->local_nodes + i));
			}
		}
	}
	void cross_search(Position<T1> lower, Position<T1> upper, uint32_t k, PSTreeNode* cur_node, result_queue_type& result_queue)
	{
		for (int i = 0; i < cur_node->size; i++)
		{
			if (result_queue.size() == k)
			{
				if ((cur_node->local_nodes + i)->priority < result_queue.top().priority)
				{
					break;
				}
				else
				{
					if ((cur_node->local_nodes + i)->pos.x >= lower.x && (cur_node->local_nodes + i)->pos.x <= upper.x &&
						(cur_node->local_nodes + i)->pos.y >= lower.y && (cur_node->local_nodes + i)->pos.y <= upper.y)
					{
						result_queue.pop();
						result_queue.push(*(cur_node->local_nodes + i));
					}
				}

			}
			else
			{
				if ((cur_node->local_nodes + i)->pos.x >= lower.x && (cur_node->local_nodes + i)->pos.x <= upper.x &&
					(cur_node->local_nodes + i)->pos.y >= lower.y && (cur_node->local_nodes + i)->pos.y <= upper.y)
				{
					result_queue.push(*(cur_node->local_nodes + i));
				}
			}
		}
	}
	void local_search(Position<T1> lower, Position<T1> upper, uint32_t k, PSTreeNode* cur_node, result_queue_type& result_queue)
	{
		bool range_cross, x_cross, y_cross;

		x_cross = !((cur_node->local_lower_bound.x > upper.x) || (cur_node->local_upper_bound.x < lower.x));
		y_cross = !((cur_node->local_lower_bound.y > upper.y) || (cur_node->local_upper_bound.y < lower.y));
		range_cross = x_cross&&y_cross;
		if (!range_cross)
		{
			return;
		}
		bool range_inside, x_inside, y_inside;
		x_inside = ((cur_node->local_lower_bound.x >= lower.x) && (cur_node->local_upper_bound.x <= upper.x));
		y_inside = ((cur_node->local_lower_bound.y >= lower.y) && (cur_node->local_upper_bound.y <= upper.y));
		range_inside = x_inside&&y_inside;
		if (range_inside)
		{
			inside_search(k, cur_node, result_queue);
		}
		else
		{
			cross_search(lower, upper, k, cur_node, result_queue);
		}
		
	}
	void queue_search(Position<T1> lower, Position<T1> upper, uint32_t k,  result_queue_type& result_queue)
	{
		std::queue<PSTreeNode*> query_queue;
		query_queue.push(head);
		while (!query_queue.empty())
		{
			PSTreeNode* cur_node = query_queue.front();
			query_queue.pop();
			bool range_cross, x_cross, y_cross;

			x_cross = !((cur_node->lower_bound.x > upper.x) || (cur_node->upper_bound.x < lower.x));
			y_cross = !((cur_node->lower_bound.y > upper.y) || (cur_node->upper_bound.y < lower.y));
			range_cross = x_cross&&y_cross;
			if (!range_cross)
			{
				continue;
			}
			if (result_queue.size() == k&&result_queue.top().priority > cur_node->local_nodes[0].priority)
			{
				continue;
			}
			local_search(lower, upper, k, cur_node, result_queue);
			if (cur_node->left)
			{
				query_queue.push(cur_node->left);
			}
			if (cur_node->right)
			{
				query_queue.push(cur_node->right);
			}

		}
		
	}
	void recursive_free(PSTreeNode* cur_node)
	{
		if (cur_node == nullptr)
		{
			return;
		}
		if (cur_node->left != nullptr)
		{
			recursive_free(cur_node->left);
		}
		if (cur_node->right != nullptr)
		{
			recursive_free(cur_node->right);
		}
		delete cur_node;
	}
	~PrioritySearchTree()
	{
		recursive_free(head);
	}
};