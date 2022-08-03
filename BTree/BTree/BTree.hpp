#pragma once
#include <iostream>
#include <utility>

template <class K, size_t M>
class BTreeNode
{
public:
	// M阶B树每个节点最多存放M-1个key值，M个子节点指针
	// 为了方便分裂，多开辟一个空间
	BTreeNode<K, M>* _subs[M + 1];
	BTreeNode<K, M>* _parent;
	K _keys[M];
	size_t _n; // 存放了多少个key值
public:
	BTreeNode()
		: _n(0), _parent(nullptr)
	{
		for (int i = 0; i < M; ++i)
		{
			_keys[i] = K();
		}
		for (int i = 0; i <= M; ++i)
		{
			_subs[i] = nullptr;
		}
	}
};

// M阶B树
template <class K, size_t M>
class BTree
{
	typedef BTreeNode<K, M> Node;
private:
	Node* _root = nullptr;
public:
	// 如果k存在，则second表示在_key中的下标，first表示k所在节点
	// 如果k不存在，则second为-1，first为k应当插入的节点
	std::pair<Node*, int> Find(const K& k)
	{
		Node* cur = _root;
		Node* parent = nullptr;

		while (cur)
		{
			size_t i = 0;
			while (i < cur->_n)
			{
				if (k < cur->_keys[i])
				{
					// 到左子树中找
					break;
				}
				else if (k > cur->_keys[i])
				{
					++i;
				}
				else
				{
					// 发现重复键值
					return { cur, i };
				}
			}
			parent = cur; 
			// 对于第i个键值，_sub[i]是左孩子，_sub[i+1]是右孩子
			if (i == cur->_n)
			{
				cur = cur->_subs[i + 1]; // k是当前节点中最大的，应当到右节点中找
			}
			else
			{
				cur = cur->_subs[i];
			}
		}

		return { parent, -1 }; // 不存在k，返回k应当插入的节点
	}

	// 将k和子节点指针sub插入parent
	// sub应当作为k的右节点，即k所在下标+1
	void _Insert(Node* parent, const K& k, Node* sub)
	{
		int end = parent->_n;
		while (end > 0)
		{
			if (parent->_keys[end - 1] > k)
			{
				parent->_keys[end] = parent->_keys[end - 1];
				parent->_subs[end + 1] = parent->_subs[end];
				--end;
			}
			else
			{
				break;
			}
		}
		parent->_keys[end] = k;
		parent->_subs[end + 1] = sub;

		if (sub != nullptr)
		{
			sub->_parent = parent; // 将子节点和父节点连接起来
		}
		parent->_n++;
	}

	bool Insert(K& key)
	{
		if (_root == nullptr)
		{
			// 树为空
			_root = new Node;
			_root->_keys[0] = key;
			_root->_n++;

			return true;
		}

		std::pair<Node*, int> ret = Find(key);

		if (ret.second >= 0)
		{
			// 说明key存在，插入失败
			return false;
		}

		K k = key;
		Node* parent = ret.first;
		Node* child = nullptr;

		while (parent)
		{
			_Insert(parent, k, child);

			if (parent->_n < M)
			{
				// 插入后节点没满，本轮插入结束
				return true;
			}

			// 插入后节点满了，则将parent内的keys和subs进行分裂
			// 取中位数key[M/2]
			// [0, M/2)的key和对应subs留在parent内，(M/2, M]分裂至新建的兄弟结点brother
			int mid = M / 2;
			size_t i = mid + 1;
			size_t j = 0;
			Node* brother = new Node;

			while (i < M)
			{
				brother->_keys[j] = parent->_keys[i];
				brother->_subs[j] = parent->_subs[i];
				++i;
				++j;
			}

			brother->_n = j;
			brother->_subs[j] = parent->_subs[i];
			parent->_n -= brother->_n + 1;

			if (parent->_parent == nullptr)
			{
				// 说明分裂的节点是根节点，此时创建一个新的根节点
				_root = new Node;
				_root->_keys[0] = parent->_keys[mid];
				_root->_n = 1;
				_root->_subs[0] = parent;
				_root->_subs[1] = brother;
				parent->_parent = _root;
				brother->_parent = _root;
				break;
			}
			else
			{
				// 下一轮将中位数key[mid]和子节点brother插入到上一层节点中
				k = parent->_keys[mid];
				parent = parent->_parent;
				child = brother;
			}

		}
	}

	void _Inorder(Node* node)
	{
		if (node == nullptr)
		{
			return;
		}

		size_t i = 0;
		for (; i < node->_n; ++i)
		{
			_Inorder(node->_subs[i]);
			std::cout << node->_keys[i] << " ";
		}
		_Inorder(node->_subs[i]);
	}

	void Inorder()
	{
		_Inorder(_root);
	}
};






