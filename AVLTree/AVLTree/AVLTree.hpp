#pragma once
template <class K, class V>
struct KVLTreeNode
{
	KVLTreeNode(const pair<K, V>& kv)
		:_parent(nullptr)
		, _left(nullptr)
		, _right(nullptr)
		, _bf(0)
		, _kv(kv)
	{}
	KVLTreeNode<K, V>* _parent; //���ڵ�
	KVLTreeNode<K, V>* _left;   //������
	KVLTreeNode<K, V>* _right;  //������
	pair<K, V> _kv;       //�洢��ֵ��
	int _bf;              //ƽ������
};

template <class K, class V>
class AVLTree
{
	typedef KVLTreeNode<K, V> Node;
public:
	bool balanced()
	{
		int height = 0;
		return _balanced(_root, height);
	}
	bool insert(const pair<K, V>& kv)
	{
		Node* newnode = new Node(kv);

		//������ֱ�Ӳ���
		if (_root == nullptr)
		{
			_root = newnode;
			return true;
		}

		Node* cur = _root;
		Node* parent = cur->_parent;

		//1�����Ҳ���λ��
		while (cur)
		{
			if (kv.first > cur->_kv.first)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (kv.first < cur->_kv.first)
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				return false;
			}
		}

		//2����������
		if (kv.first > parent->_kv.first)
		{
			parent->_right = newnode;
		}
		else
		{
			parent->_left = newnode;
		}
		newnode->_parent = parent;

		//3�����¶��ϵ���ƽ������
		cur = newnode;
		parent = cur->_parent;
		while (parent)
		{
			//����parent����������bf--������++
			if (cur == parent->_left)
			{
				--parent->_bf;
			}
			else
			{
				++parent->_bf;
			}
			//��⵱ǰ�Ƿ�ƽ��
			//���һ���½��Ĳ���ʹ��parentΪ������ƽ����
			//������Ӱ�쵱ǰ������ȣ���ô������Ӱ�쵽������������ֱ��break
			if (parent->_bf == 0)
			{
				break;
			}
			//��������½��Ĳ���ʹ��parentΪ������һ�߱���һ�߸�һ��
			//����Ӱ�쵽��ǰ������ȣ�Ҳ����Ӱ�������������˼������ϵ���
			else if (parent->_bf == 1 || parent->_bf == -1)
			{
				cur = parent;
				parent = parent->_parent;
			}
			//��������½��Ĳ���ʹ��parentΪ�����������˲�ƽ�⣨���������߶Ȳ����1��
			//��Ҫ������ת
			else
			{
				//���һ��cur��Ϊ����������������߲�����²�ƽ�⣬�����ҵ���
				if (parent->_bf == -2 && cur->_bf == -1)
				{
					rotateR(parent);
				}
				//�������cur��Ϊ���������������ұ߲�����²�ƽ�⣬����
				else if (parent->_bf == -2 && cur->_bf == 1)
				{
					rotateLR(parent);
				}
				//������������������ұ߲�����²�ƽ�⣬��������
				else if (parent->_bf == 2 && cur->_bf == 1)
				{
					rotateL(parent);
				}
				//����ġ�������������߲�����²�ƽ�⣬����
				else if (parent->_bf == 2 && cur->_bf == -1)
				{
					rotateRL(parent);
				}
				//��ת��ɺ���������ƽ���ˣ�ֱ��break
				break;
			}
		}//����ƽ������ѭ������
		return true;
	}
	void inOrderTraversal()
	{
		_inOrderTraversal(_root);
	}
private:
	void rotateL(Node* root)//����
	{
		Node* subR = root->_right;
		Node* subRL = subR->_left;
		Node* parent = root->_parent;//��ǰ�洢root��_parent
		//������root����������������root��subR��subRL�����ӹ�ϵ
		root->_right = subRL;
		if (subRL != nullptr)
			subRL->_parent = root;
		subR->_left = root;
		root->_parent = subR;
		//����subR��root��_parent�����ӹ�ϵ
		if (parent == nullptr)
		{
			_root = subR;
		}
		else
		{
			if (parent->_left == root)
				parent->_left = subR;
			else
				parent->_right = subR;
		}
		subR->_parent = parent;
		//��ת��ɺ󣬵���ƽ������
		subR->_bf = root->_bf = 0;
	}
	void rotateR(Node* root)//����
	{
		Node* subL = root->_left;//root��������
		Node* subLR = subL->_right;//subL��������
		Node* parent = root->_parent;//��ǰ�洢root��_parent
		//ͨ����תʹroot˳ʱ����ת��subL���ң�����subL����root��λ��
		root->_left = subLR;
		subL->_right = root;
		if (subLR != nullptr)
			subLR->_parent = root;
		//������ת����subL��Ϊ���µĸ�������Ҫ���µ�ǰAVL����_root
		if (parent == nullptr)
		{
			_root = subL;		
		}
		else//��������subL��ԭ��root��_parent
		{
			if (root == parent->_left)
				parent->_left = subL;
			else
				parent->_right = subL;
		}
		subL->_parent = parent;
		root->_parent = subL;
		subL->_bf = root->_bf = 0;
	}
	void rotateLR(Node* root)
	{
		Node* subL = root->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;//��¼˫��ǰsubLR��ƽ������
		//�Ƚ���һ���������ٽ���һ���ҵ���
		rotateL(subL);
		rotateR(root);
		//���Ÿ��ݲ�ͬ�������ƽ������
		if (bf == 0)
		{
			root->_bf = subL->_bf = subLR->_bf = 0;
		}
		else if (bf == -1)
		{
			root->_bf = 1;
			subL->_bf = subLR->_bf = 0;
		}
		else if (bf == 1)
		{
			root->_bf = subLR->_bf = 0;
			subL->_bf = -1;
		}
	}
	void rotateRL(Node* root)
	{
		Node* subR = root->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;//��¼˫��ǰsubRL��ƽ������
		//�Ƚ���һ���ҵ������ٽ���һ������
		rotateR(subR);
		rotateL(root);
		//���Ÿ��ݲ�ͬ�������ƽ������
		if (bf == 0)
		{
			root->_bf = subR->_bf = subRL->_bf = 0;
		}
		else if (bf == -1)
		{
			root->_bf = subRL->_bf = 0;
			subR->_bf = 1;
		}
		else if (bf == 1)
		{
			root->_bf = -1;
			subR->_bf = subRL->_bf = 0;
		}
	}
	bool _balanced(Node* root, int& height)
	{
		if (root == nullptr)
			return true;
		int leftHeight = 0, rightHeight = 0;
		bool flag = _balanced(root->_left, leftHeight) && _balanced(root->_right, rightHeight);
		height = max(leftHeight, rightHeight) + 1;
		return abs(leftHeight - rightHeight) <= 1 && flag;
	}
	void _inOrderTraversal(Node* root)
	{
		if (root == nullptr)
			return;
		_inOrderTraversal(root->_left);
		cout << root->_kv.first << ":" << root->_kv.second << endl;
		_inOrderTraversal(root->_right);
	}
	Node* _root = nullptr;
};