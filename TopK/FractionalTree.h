#include "SearchContext.h"

#include <stdint.h>
#include <queue>
using std::pair;
using std::make_pair;

template <typename T1, typename T2>
class FractionalTree :public SearchContext<T1, T2>
{
private:
	using result_queue_type = std::priority_queue<Node<T1, T2>, std::vector<Node<T1, T2>>, PriorityCmp<T1, T2>>;
	class FractionalTreeNode
	{
	public:
		uint32_t size;
		T1 delimit;
		T2 max_priority;
		FractionalTreeNode* left;
		FractionalTreeNode* right;
		bool is_leaf;
		Node<T1, T2>* local_nodes;
		//if is leaf sorted by y ,else sorted by priority
		//in fact ,if the k is value that would not change 
		//we don't need to store all the node in local nodes
		//just store the topk nodes in local nodes
		Position<T1> lower_bound;
		Position<T1> upper_bound;
	};
	FractionalTreeNode* head;
	const int min_split_size;

	FractionalTreeNode* split(Node<T1, T2>* begin, Node<T1, T2>* end)
	{
		T2 max_priority = std::numeric_limits<T2>::min();
		uint32_t size = end - begin;
		FractionalTreeNode* new_node = new FractionalTreeNode();
		std::nth_element(begin, begin + size / 2, end, xpos_cmp<T1, T2>);
		T1 delimit = begin[size / 2].pos.x;
		Node<T1, T2>* new_nth = std::partition(begin + size / 2 + 1, end, [](const Node<T1, T2>& b)
		{
			return b.pos.x == delimit;
		});
		int new_size = new_nth - begin;
		if (new_size == size)
		{
			new_nth = std::partition(begin, begin + size / 2 + 1, [](const Node<T1, T2>& b)
			{
				return b.pos.x <delimit;
			});
			if (new_nth == begin)
			{
				delete new_node;
				return nullptr;
			}
		}
		new_node->delimit = (*(new_nth - 1)).pos.x;
		new_node->is_leaf = false;
		new_node->is_x = true;
		new_node->left = new_node->right = nullptr;
		new_node->local_nodes = new Node<T1, T2>[size];;
		new_node->size = size;
		T1 lower_x, upper_x, lower_y, upper_y;
		upper_x = upper_y = std::numeric_limits<T1>::min();
		lower_x = lower_y = std::numeric_limits<T1>::max();
		for (int i = 0; i < size; i++)
		{
			lower_x = lower_x >(begin + i)->pos.x ? (begin + i)->pos.x : lower_x;
			lower_y = lower_y > (begin + i)->pos.y ? (begin + i)->pos.y : lower_y;
			upper_x = upper_x > (begin + i)->pos.x ? upper_x : (begin + i)->pos.x;
			upper_y = upper_y > (begin + i)->pos.y ? upper_y : (begin + i)->pos.y;
			max_priority = max_priority > begin[i].priority ? max_priority : begin[i].priority;
			new_node->local_nodes[i] = begin[i];
		}
		new_node->lower_bound.x = lower_x;
		new_node->upper_bound.x = upper_x;
		new_node->lower_bound.y = lower_y;
		new_node->upper_bound.y = upper_y;
		new_node->max_priority = max_priority;
		std::sort(new_node->local_nodes, new_node->local_nodes + size, [](const Node<T1, T2>& a, const Node<T1, T2>& b)
		{
			return a.priority > b.priority;
		});
		new_node->left = CreateNode(begin, new_nth, );
		new_node->right = CreateNode(new_nth, end, );
		return new_node;
	}
	FractionalTreeNode* CreateLeaf(Node<T1, T2>* begin, Node<T1, T2>* end)
	{
		FractionalTreeNode* new_node;
		int size = end - begin;
		new_node = new FractionalTreeNode();
		new_node->left = new_node->right = nullptr;
		new_node->is_leaf = true;
		new_node->size = size;
		new_node->local_nodes = new Node<T1, T2>[size];
		T1 lower_x, upper_x, lower_y, upper_y;
		upper_x = upper_y = std::numeric_limits<T1>::min();
		lower_x = lower_y = std::numeric_limits<T1>::max();
		for (int i = 0; i < size; i++)
		{
			lower_x = lower_x >(begin + i)->pos.x ? (begin + i)->pos.x : lower_x;
			lower_y = lower_y > (begin + i)->pos.y ? (begin + i)->pos.y : lower_y;
			upper_x = upper_x > (begin + i)->pos.x ? upper_x : (begin + i)->pos.x;
			upper_y = upper_y > (begin + i)->pos.y ? upper_y : (begin + i)->pos.y;
			new_node->local_nodes[i] = *(begin + i);
		}
		new_node->lower_bound.x = lower_x;
		new_node->upper_bound.x = upper_x;
		new_node->lower_bound.y = lower_y;
		new_node->upper_bound.y = upper_y;
		std::sort(new_node->local_nodes, new_node->local_nodes + size, [](const Node<T1, T2>& a, const Node<T1, T2>& b)
		{
			return a.priority > b.priority;
		});
		new_node->max_priority = (new_node->local_nodes)[0].priority;
		return new_node;
	}
	FractionalTreeNode* CreateNode(Node<T1, T2>* begin, Node<T1, T2>* end)
	{
		FractionalTreeNode* new_node;

		int size = end - begin;
		if (size == 0)
		{
			return nullptr;
		}
		if (size <= min_split_size)
		{
			return CreateLeaf(begin, end)
		}
		new_node = split(begin, end);
		if (new_node == nullptr)
		{
			new_node = CreateLeaf(begin, end);
		}
		return new_node;
	}
public:
	FractionalTree(const Node<T1, T2>* begin, const Node<T1, T2>* end, int in_min_split_size)
		:SearchContext<T1, T2>(begin, end), min_split_size(in_min_split_size)
	{

	}
	void CreateContext()
	{
		if (input_nodes.size() = 0)
		{
			head = nullptr;
		}
		else
		{
			head = CreateNode(&input_nodes[0], &input_nodes[input_nodes.size() - 1] + 1);
		}
	}
	std::vector<Node<T1, T2>> TopkSearch(Position<T1> lower, Position<T2> upper, uint32_t k)
	{
		std::vector<Node<T1, T2>> result;
		if (input_nodes.size() == 0 || k == 0)
		{
			return result;
		}
		result_queue_type result_queue;
		recursive_search(lower, upper, k, head, result_queue);
		while (result_queue.size() != 0)
		{
			result.push_back(result_queue.top());
			result_queue.pop();
		}
		std::reverse(result.begin(), result.end());
		return result;
	}
	void inside_search(uint32_t k, FractionalTreeNode* cur_node, result_queue_type& result_queue)
	{
		if (result_queue.size() == k&&result_queue.top().priority > cur_node->max_priority)
		{
			return;
		}

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
					result_queue.push(*(begin + i));
				}

			}
			else
			{
				result_queue.push(*(begin + i));
			}
		}
	}
	void leaf_search(Position<T1> lower, Position<T2> upper, uint32_t k, FractionalTreeNode* cur_node, result_queue_type& result_queue)
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
					if ((cur_node->local_nodes + i)->x >= lower.x && (cur_node->local_nodes + i)->x <= upper.x &&
						(cur_node->local_nodes + i)->y >= lower.y && (cur_node->local_nodes + i)->y <= upper.y)
					{
						result_queue.pop();
						result_queue.push(*(begin + i));
					}
				}

			}
			else
			{
				if ((cur_node->local_nodes + i)->x >= lower.x && (cur_node->local_nodes + i)->x <= upper.x &&
					(cur_node->local_nodes + i)->y >= lower.y && (cur_node->local_nodes + i)->y <= upper.y)
				{
					result_queue.push(*(begin + i));
				}
			}
		}
	}
	void recursive_search(Position<T1> lower, Position<T2> upper, uint32_t k, FractionalTreeNode* cur_node, result_queue_type& result_queue)
	{
		bool range_cross, x_cross, y_cross;
		bool range_inside, x_inside, y_inside;
		x_cross = !((cur_node->lower_bound.x > upper.x) || (cur_node->upper_bound.x < lower.x));
		y_cross = !((cur_node->lower_bound.y > upper.y) || (cur_node->upper_bound.y < lower.y));
		range_cross = x_cross&&y_cross;
		if (!range_cross)
		{
			return;
		}
		x_inside = ((cur_node->lower_bound.x >= lower.x) && (cur_node->upper_bound.x <= upper.x));
		y_inside = ((cur_node->lower_bound.y >= lower.y) && (cur_node->upper_bound.y <= upper.y));
		range_inside = x_inside&&y_inside;
		if (range_inside)
		{
			inside_search(k, cur_node, result_queue);
		}
		else
		{
			if (cur_node->is_leaf)
			{
				leaf_search(lower, upper, k, cur_node, result_queue);
			}
			else
			{
				recursive_search(lower, upper, k, cur_node->left, result_queue);
				recursive_search(lower, upper, k, cur_node->right, result_queue);
			}
		}

	}
	void recursive_free(FractionalTreeNode* cur_node)
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
		delete [] cur_node->local_nodes;
		delete cur_node;
	}
	~FractionalTree()
	{
		recursive_free(head);
	}
};