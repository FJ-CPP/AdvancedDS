#pragma once
template <class K, class V>
struct BSTreeNode
{
	BSTreeNode* _left;
	BSTreeNode* _right;
	K _key;
	V _val;
	BSTreeNode(K key, V val)
		:_left(nullptr)
		, _right(nullptr)
		, _key(key)
		, _val(val)
	{}
};

template <class K, class V>
class BSTree
{
	typedef BSTreeNode<K, V> Node;
public:
	BSTree()
		:_root(nullptr)
	{}
	bool insert(const K& key, const V& val)
	{
		if (_root == nullptr)
		{
			_root = new Node(key, val);
			return true;
		}
		Node* cur = _root;
		Node* parent = cur;
		while (cur)
		{
			if (key > cur->_key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (key < cur->_key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				return false;//不可插入相同的值
			}
		}
		if (key > parent->_key)
		{
			parent->_right = new Node(key, val);
		}
		else
		{
			parent->_left = new Node(key, val);
		}
		return true;
	}
	bool erase(const K& key)
	{
		Node* cur = _root;
		Node* parent = cur;
		while (cur)//找要删除的节点
		{
			if (key > cur->_key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (key < cur->_key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else//删除节点已找到，分类讨论
			{
				//1、删除节点的左为空或者左右都为空（叶子结点）
				if (cur->_left == nullptr)
				{
					//删除节点是_root
					if (cur == _root)
					{
						_root = _root->_right;
					}
					//判断cur是parent的左子树还是右子树，方便连接
					if (parent->_left == cur)
					{
						parent->_left = cur->_right;
					}
					else
					{
						parent->_right = cur->_right;
					}
					delete cur;
				}
				//2、删除节点的右为空
				else if (cur->_right == nullptr)
				{
					//删除节点是_root
					if (cur == _root)
					{
						_root = _root->_left;
					}
					//判断cur是parent的左子树还是右子树，方便连接
					if (parent->_left == cur)
					{
						parent->_left = cur->_left;
					}
					else
					{
						parent->_right = cur->_left;
					}
					delete cur;
				}
				//3、删除节点的左右都不为空
				else
				{
					//找到左子树的最大节点或者右子树的最小节点与它进行key值交换
					//这里选择交换左子树的最大节点
					Node* leftMax = cur->_left;
					parent = leftMax;
					while (leftMax->_right)//左子树的最大节点肯定在_right分支
					{
						parent = leftMax;
						leftMax = leftMax->_right;
					}
					swap(leftMax->_key, cur->_key);
					//key值交换完成后，删除leftMax	
					if (cur->_left == leftMax)//leftMax就是cur的左节点就要通过cur->_left删除
					{
						cur->_left = leftMax->_left;
					}
					else//否则可以通过parent删除
					{
						parent->_right = leftMax->_left;
					}
					delete leftMax;
				}
				return true;
			}
		}
		//删除节点未找到
		return false;
	}
	Node* find(const K& key)
	{
		Node* cur = _root;
		while (cur)
		{
			if (key > cur->_key)
			{
				cur = cur->_right;
			}
			else if (key < cur->_key)
			{
				cur = cur->_left;
			}
			else
			{
				return cur;
			}
		}
		return nullptr;
	}
	void inorder()
	{
		if (_root == nullptr)//二叉搜索树为空
		{
			cout << "NULL" << endl;
			return;
		}
		_inorder(_root);
		cout << endl;
	}
private:
	Node* _root;
	void _inorder(const Node* root)
	{
		if (root == nullptr)
		{
			return;
		}
		_inorder(root->_left);
		cout << root->_key << ":" << root->_val << endl;
		_inorder(root->_right);
	}
};