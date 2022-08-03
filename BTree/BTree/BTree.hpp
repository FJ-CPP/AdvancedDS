#pragma once
#include <iostream>
#include <utility>

template <class K, size_t M>
class BTreeNode
{
public:
	// M��B��ÿ���ڵ������M-1��keyֵ��M���ӽڵ�ָ��
	// Ϊ�˷�����ѣ��࿪��һ���ռ�
	BTreeNode<K, M>* _subs[M + 1];
	BTreeNode<K, M>* _parent;
	K _keys[M];
	size_t _n; // ����˶��ٸ�keyֵ
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

// M��B��
template <class K, size_t M>
class BTree
{
	typedef BTreeNode<K, M> Node;
private:
	Node* _root = nullptr;
public:
	// ���k���ڣ���second��ʾ��_key�е��±꣬first��ʾk���ڽڵ�
	// ���k�����ڣ���secondΪ-1��firstΪkӦ������Ľڵ�
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
					// ������������
					break;
				}
				else if (k > cur->_keys[i])
				{
					++i;
				}
				else
				{
					// �����ظ���ֵ
					return { cur, i };
				}
			}
			parent = cur; 
			// ���ڵ�i����ֵ��_sub[i]�����ӣ�_sub[i+1]���Һ���
			if (i == cur->_n)
			{
				cur = cur->_subs[i + 1]; // k�ǵ�ǰ�ڵ������ģ�Ӧ�����ҽڵ�����
			}
			else
			{
				cur = cur->_subs[i];
			}
		}

		return { parent, -1 }; // ������k������kӦ������Ľڵ�
	}

	// ��k���ӽڵ�ָ��sub����parent
	// subӦ����Ϊk���ҽڵ㣬��k�����±�+1
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
			sub->_parent = parent; // ���ӽڵ�͸��ڵ���������
		}
		parent->_n++;
	}

	bool Insert(K& key)
	{
		if (_root == nullptr)
		{
			// ��Ϊ��
			_root = new Node;
			_root->_keys[0] = key;
			_root->_n++;

			return true;
		}

		std::pair<Node*, int> ret = Find(key);

		if (ret.second >= 0)
		{
			// ˵��key���ڣ�����ʧ��
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
				// �����ڵ�û�������ֲ������
				return true;
			}

			// �����ڵ����ˣ���parent�ڵ�keys��subs���з���
			// ȡ��λ��key[M/2]
			// [0, M/2)��key�Ͷ�Ӧsubs����parent�ڣ�(M/2, M]�������½����ֵܽ��brother
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
				// ˵�����ѵĽڵ��Ǹ��ڵ㣬��ʱ����һ���µĸ��ڵ�
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
				// ��һ�ֽ���λ��key[mid]���ӽڵ�brother���뵽��һ��ڵ���
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






