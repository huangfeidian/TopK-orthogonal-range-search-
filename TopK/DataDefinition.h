#ifndef __H_DATADEFINITION_H__
#define __H_DATADEFINITION_H__
template <typename T1>
class Position
{
public:
	T1 x;
	T1 y;
	Position(T1 in_x, T1 in_y)
		:x(in_x), y(in_y)
	{

	}
	bool operator==(const Position<T1>& a)
	{
		return x == a.x&&y == a.y;
	}
	Position()
	{

	}

};
template <typename T>
bool operator==(const Position<T>& a, const Position<T>& b)
{
	return a.x == b.x&&a.y == b.y;
}
template <typename T>
bool xpos_cmp(const Position<T>& a, const Position<T>& b)
{
	return a.x < b.x;
}
template <typename T>
bool ypos_cmp(const Position<T>& a, const Position<T>& b)
{
	return a.y < b.y;
}
template <typename T1>
class PackedPos
{
public:
	T1 x;
	T1 y;
	T1 neg_x;
	T1 neg_y;
	PackedPos(T1 in_x, T1 in_y)
		:x(in_x), y(in_y)
	{
		neg_x=-1*x;
		neg_y=-1*y;
	}
	PackedPos()
	{

	}
};
template <typename T1, typename T2>
class Node
{
public:
	Position<T1> pos;
	T2 priority;
	Node(T1 in_x, T1 in_y, T2 in_priority)
		:pos(in_x, in_y), priority(in_priority)
	{

	}
	Node()
	{

	}
	bool operator==(const Node<T1, T2>& a)
	{
		return priority == a.priority&&pos == a.pos;
	}
};
template <typename T1, typename T2>
bool operator==(const Node<T1, T2>& a, const Node<T1, T2>& b)
{
	return a.pos == b.pos&&a.priority == b.priority;
}
template <typename T1, typename T2>
class PriorityCmp
{
public:
	bool operator()(const Node<T1, T2>& a, const Node<T1, T2>& b)
	{
		return a.priority > b.priority;
	}
};
template <typename T1,typename T2>
bool xpos_cmp(const Node<T1,T2>& a, const Node<T1,T2>& b)
{
	return a.pos.x < b.pos.x;
}
template <typename T1, typename T2>
bool ypos_cmp(const Node<T1, T2>& a, const Node<T1,T2>& b)
{
	return a.pos.y < b.pos.y;
}
#endif