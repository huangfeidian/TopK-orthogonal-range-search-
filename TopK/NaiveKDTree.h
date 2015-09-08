#include "SearchContext.h"
#include <stdint.h>
#include <queue>
using std::pair;
using std::make_pair;

template <typename T1,typename T2>
class NaiveKDTree:public SearchContext<T1,T2>
{
private:
	using result_queue_type = std::priority_queue<Node<T1, T2>, std::vector<Node<T1, T2>>, PriorityCmp<T1, T2>>;
	class KDTreeNode
	{
	public:
		uint32_t size;
		T1 delimit;
		bool is_x;
		KDTreeNode* left;
		KDTreeNode* right;
		bool is_leaf;
		Node<T1,T2>* local_nodes;
		Position<T1> lower_bound;
		Position<T1> upper_bound;
	};
	KDTreeNode* head;
	const int min_split_size;
	
	KDTreeNode* split_x(Node<T1, T2>* begin, Node<T1, T2>* end)
	{
		KDTreeNode<T1>* new_node = new KDTreeNode();
		std::nth_element(begin, begin + size / 2, end, xpos_cmp<T1, T2>);
		T1 delimit = begin[size / 2].pos.x;
		Node<T1, T2>* new_nth = std::partition(begin + size / 2 + 1, end, [](const Node<T1, T2>& b)
		{
			return b.pos.x == delimit;
		});
		int new_size = new_nth - begin;
		if (new_size == size)
		{
			new_nth = std::partition(begin,begin + size / 2 + 1,  [](const Node<T1, T2>& b)
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
		new_node->local_nodes = nullptr;
		new_node->size = 0;
		T1 lower_x, upper_x, lower_y, upper_y;
		upper_x = upper_y = std::numeric_limits<T1>::min();
		lower_x = lower_y = std::numeric_limits<T1>::max();
		for (int i = 0; i < size; i++)
		{
			lower_x = lower_x > (begin + i)->pos.x ? (begin + i)->pos.x : lower_x;
			lower_y = lower_y > (begin + i)->pos.y ? (begin + i)->pos.y : lower_y;
			upper_x = upper_x > (begin + i)->pos.x ? upper_x:(begin + i)->pos.x ;
			upper_y = upper_y > (begin + i)->pos.y ? upper_y : (begin + i)->pos.y;
		}
		new_node->lower_bound.x = lower_x;
		new_node->upper_bound.x = upper_x;
		new_node->lower_bound.y = lower_y;
		new_node->upper_bound.y = upper_y;
		new_node->left = CreateNode(begin, new_nth, false);
		new_node->right = CreateNode(new_nth, end, false);
		return new_node;
	}
	KDTreeNode* split_y(Node<T1, T2>* begin, Node<T1, T2>* end)
	{
		KDTreeNode<T1>* new_node = new KDTreeNode();
		std::nth_element(begin, begin + size / 2, end, ypos_cmp<T1, T2>);
		T1 delimit = begin[size / 2].pos.y;
		Node<T1, T2>* new_nth = std::partition(begin + size / 2 + 1, end, [](const Node<T1, T2>& b)
		{
			return b.pos.y == delimit;
		});
		int new_size = new_nth - begin;
		if (new_size == size)
		{
			new_nth = std::partition(begin, begin + size / 2 + 1, [](const Node<T1, T2>& b)
			{
				return b.pos.y <delimit;
			});
			if (new_nth == begin)
			{
				delete new_node;
				return nullptr;
			}
		}
		new_node->delimit = (*(new_nth - 1)).pos.x;
		new_node->is_leaf = false;
		new_node->is_x = false;
		new_node->left = new_node->right = nullptr;
		new_node->local_nodes = nullptr;
		new_node->size = 0;
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
		new_node->left = CreateNode(begin, new_nth, false);
		new_node->right = CreateNode(new_nth, end, false);
		return new_node;
	}
	KDTreeNode* CreateNode(Node<T1, T2>* begin, Node<T1, T2>* end, bool is_x)
	{
		KDTreeNode* new_node;
		int size = end - begin;
		if (size == 0)
		{
			return nullptr;
		}
		if (size <= min_split_size)
		{
			new_node = new KDTreeNode();
			new_node->left = new_node->right = nullptr;
			new_node->is_leaf = true;
			new_node->is_x = true;
			new_node->size = size;
			new_node->delimit = begin->pos.x;
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
			std::sort(begin, end, [](const Node<T1, T2>& a, const Node<T1, T2>& b)
			{
				return a.priority > b.priority;
			})
			return new_node;
		}
		if (is_x)
		{
			new_node = split_x(begin, end);
			if (new_node == nullptr)
			{
				new_node = split_y(begin, end);
				if (new_node == nullptr)
				{
					new_node = new KDTreeNode();
					new_node->left = new_node->right = nullptr;
					new_node->is_leaf = true;
					new_node->is_x = false;
					new_node->size = size;
					new_node->delimit = begin->pos.x;
					new_node->local_nodes = begin;
					new_node->upper_bound.x = new_node->lower_bound.x = begin->pos.x;
					new_node->upper_bound.y = new_node->lower_bound.y = begin->pos.y;
					std::sort(begin, end, [](const Node<T1, T2>& a, const Node<T1, T2>& b)
					{
						return a.priority > b.priority;
					})
				}
			}
		}
		else
		{
			new_node = split_y(begin, end);
			if (new_node == nullptr)
			{
				new_node = split_x(begin, end);
				if (new_node == nullptr)
				{
					new_node = new KDTreeNode();
					new_node->left = new_node->right = nullptr;
					new_node->is_leaf = true;
					new_node->is_x = true;
					new_node->size = size;
					new_node->delimit = begin->pos.x;
					new_node->local_nodes = begin;
					new_node->upper_bound.x = new_node->lower_bound.x = begin->pos.x;
					new_node->upper_bound.y = new_node->lower_bound.y = begin->pos.y;
					std::sort(begin, end, [](const Node<T1, T2>& a, const Node<T1, T2>& b)
					{
						return a.priority > b.priority;
					})
				}
			}
		}
		return new_node;
	}
public:
	NaiveKDTree(const Node<T1, T2>* begin, const Node<T1, T2>* end,int in_min_split_size)
		:SearchContext<T1, T2>(begin, end), min_split_size(in_min_split_size)
	{
		
	}
	void CreateContext()
	{
		if (input_nodes.size() = 0)
		{
			head=nullptr;
		}
		else
		{
			head = CreateNode(&input_nodes[0], &input_nodes[input_nodes.size() - 1] + 1, true);
		}
	}
	std::vector<Node<T1, T2>> TopkSearch(Position<T1> lower, Position<T2> upper, uint32_t k)
	{
		std::vector<Node<T1, T2>> result;
		if (input_nodes.size() == 0||k==0)
		{
			return result;
		}
		result_queue_type result_queue;
		recursive_search(lower, upper, k, head, &result_queue);
		while (result_queue.size() != 0)
		{
			result.push_back(result_queue.top());
			result_queue.pop();
		}
		std::reverse(result.begin(), result.end());
		return result;
	}
	void recursive_search(Position<T1> lower, Position<T2> upper, uint32_t k,KDTreeNode* cur_node, result_queue_type* result_queue)
	{
		bool range_cross, x_cross, y_cross;
		
		x_cross = !((cur_node->lower_bound.x > upper.x)||(cur_node->upper_bound.x < lower.x));
		y_cross = !((cur_node->lower_bound.y > upper.y) || (cur_node->upper_bound.y < lower.y));
		range_cross = x_cross&&y_cross;
		if (!range_cross)
		{
			return;
		}
		if (cur_node->is_leaf)
		{
			for (int i = 0; i < cur_node->size; i++)
			{

				if ((cur_node->local_nodes + i).priority < result_queue.top().priority)
				{
					if (result_queue.size() == k)
					{
						break;
					}
				}
				else
				{
					if ((cur_node->local_nodes + i)->x >= lower.x && (cur_node->local_nodes + i)->x <= upper.x &&
						(cur_node->local_nodes + i)->y >= lower.y && (cur_node->local_nodes + i)->y <= upper.y)
					{
						if (result_queue.size() == k)
						{
							result_queue.pop();
							
						}
						result_queue.push(*(begin + i));
					}
				}
			}
		}
		else
		{
			if (cur_node->left != nullptr)
			{
				recursive_search(lower, upper, k, cur_node->left, result_queue);
			}
			if (cur_node->right != nullptr)
			{
				recursive_search(lower, upper, k, cur_node->right, result_queue);
			}
		}
	}
	void recursive_free(KDTreeNode* cur_node)
	{
		if (cur_node == nullptr)
		{
			return;
		}
		if (cur_node->left == nullptr)
		{
			recursive_free(cur_node->left);
		}
		if (cur_node->right == nullptr)
		{
			recursive_free(cur_node->right);
		}
		delete cur_node;
	}
	~NaiveKDTree()
	{
		recursive_free(head);
	}
};