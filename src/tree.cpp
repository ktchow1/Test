#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<stack>
#include<queue>
#include<unordered_map>

// ************************ //
// *** Tree : DFS & BFS *** //
// ************************ //
template<typename T> struct node
{
	node(const T& x) : value(x) {}
	~node()
	{
		std::cout << "~" << value << " ";
		if (lhs) { delete lhs; lhs = nullptr; }
		if (rhs) { delete rhs; rhs = nullptr; }
	}

	T value;
	node<T>* lhs = nullptr;
	node<T>* rhs = nullptr;
};

enum tranverse_order
{
	in_order_does_not_work,
	in_order,
	pre_order,
	post_order
};

template<typename T>
void DFS_recursive(const node<T>* root, tranverse_order order)
{
	if (root)
	{
		if (order == pre_order)
		{
			std::cout << root->value << " ";
			DFS_recursive(root->lhs, order);
			DFS_recursive(root->rhs, order);
		}
		else if (order == in_order)
		{
			DFS_recursive(root->lhs, order);
			std::cout << root->value << " ";
			DFS_recursive(root->rhs, order);
		}
		else
		{
			DFS_recursive(root->lhs, order);
			DFS_recursive(root->rhs, order);
			std::cout << root->value << " ";
		}
	}
}

template<typename T> void DFS_iterative(const node<T>* root, tranverse_order order)
{
	std::stack<const node<T>*> s;
	if (!root) return; 	

	// preorder executes this_node, push lhs and push rhs
	if (order == pre_order)
	{
		s.push(root);
		while(!s.empty()) // all nodes in s are non-nullptr
		{
			auto this_node = s.top(); s.pop();			
			std::cout << this_node->value << " ";		
			if (this_node->rhs) s.push(this_node->rhs); 
			if (this_node->lhs) s.push(this_node->lhs);	
		}
	}
	// This fails, as you keep on pushing lhs, rhs, never do that, unless you can mark visited node on the tree.
	else if (order == in_order_does_not_work)
	{
		s.push(root);		
		while (!s.empty())
		{
			auto this_node = s.top();
			if (this_node->lhs) 
			{
				s.push(this_node->lhs);
			}
			else
			{						
				s.pop();
				std::cout << this_node->value << " ";				
				if (this_node->rhs) s.push(this_node->rhs);
			}												
		}
	} 
	else if (order == in_order)
	{
		auto this_node = root;
		while (!s.empty() || this_node) // s contains nullptr node
		{
			// (1) s is used to store all this_node preempted by this_node->lhs, s does not store this_node->rhs, since ...
			if (this_node)
			{
				s.push(this_node);
				this_node = this_node->lhs;
			}
			// (2) s is popped, this_node is executed, which is then followed by this_node->rhs, hence no need to push this_node->rhs
			else
			{
				this_node = s.top(); s.pop();
				std::cout << this_node->value << " ";
				this_node = this_node->rhs;
			}
		}
	} 
//	else // complicated implementation ...
}

template<typename T> void BFS_iterative(const node<T>* root) // no order option
{
	std::queue<const node<T>*> q;
	if (root) q.push(root);
	else return;

	while (!q.empty())
	{
		auto this_node = q.front(); q.pop();

		std::cout << this_node->value << " ";
		if (this_node->lhs) q.push(this_node->lhs);
		if (this_node->rhs) q.push(this_node->rhs);
	}
}

template<typename T>
void find_layer_average(const node<T>* root, std::vector<std::pair<int, int>>& sum_and_count)
{
	std::queue<std::pair<const node<int>*, int>> q; // include layer index, with root as layer 0
	if (root) q.push(std::make_pair(root, 0));
	else return;

	while (!q.empty())
	{
		auto pair = q.front(); q.pop();

		while (pair.second >= sum_and_count.size()) sum_and_count.push_back(std::make_pair(0, 0));
		sum_and_count[pair.second].first  += pair.first->value;
		sum_and_count[pair.second].second += 1;

		if (pair.first->lhs) q.push(std::make_pair(pair.first->lhs, pair.second + 1));
		if (pair.first->rhs) q.push(std::make_pair(pair.first->rhs, pair.second + 1));
	}
}

inline node<int>* generate_tree()
{
	node<int>* root = new node<int>(1);
	root->lhs = new node<int>(2);
	root->rhs = new node<int>(3);
	root->lhs->lhs = new node<int>(4);
	root->lhs->rhs = new node<int>(5);
	root->rhs->rhs = new node<int>(6);
	root->lhs->lhs->lhs = new node<int>(7);
	root->lhs->lhs->rhs = new node<int>(8);
	root->rhs->rhs->lhs = new node<int>(9);
	root->rhs->rhs->rhs = new node<int>(10);
	root->lhs->lhs->rhs->lhs = new node<int>(11);
	root->rhs->rhs->lhs->lhs = new node<int>(12);
	root->rhs->rhs->lhs->rhs = new node<int>(13);
	root->rhs->rhs->rhs->lhs = new node<int>(14);
	root->rhs->rhs->rhs->rhs = new node<int>(15);
	root->rhs->rhs->rhs->lhs->lhs = new node<int>(16);
	root->rhs->rhs->rhs->rhs->lhs = new node<int>(17);
	root->rhs->rhs->rhs->rhs->rhs = new node<int>(18);
	return root;
}

void test_tree()
{
	node<int>* root = generate_tree();
	std::cout << "\nDFS "; DFS_recursive(root, pre_order);
	std::cout << "\nDFS "; DFS_iterative(root, pre_order);
	std::cout << "\nDFS "; DFS_recursive(root, in_order);
	std::cout << "\nDFS "; DFS_iterative(root, in_order);
	std::cout << "\nDFS "; DFS_recursive(root, post_order);		
//	std::cout << "\nDFS "; DFS_iterative(root, post_order);
	std::cout << "\nBFS "; BFS_iterative(root);

	std::vector<std::pair<int, int>> sum_and_count;
	find_layer_average(root, sum_and_count);

	for (int n = 0; n != sum_and_count.size(); ++n)
	{
		std::cout << "\nlayer_" << n
			<< " sum=" << sum_and_count[n].first
			<< " num=" << sum_and_count[n].second;
	}
	std::cout << "\nDEL "; delete root;	
}

