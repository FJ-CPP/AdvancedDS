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
	KVLTreeNode<K, V>* _parent; //父节点
	KVLTreeNode<K, V>* _left;   //左子树
	KVLTreeNode<K, V>* _right;  //右子树
	pair<K, V> _kv;       //存储键值对
	int _bf;              //平衡因子
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

		//空树，直接插入
		if (_root == nullptr)
		{
			_root = newnode;
			return true;
		}

		Node* cur = _root;
		Node* parent = cur->_parent;

		//1、查找插入位置
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

		//2、插入数据
		if (kv.first > parent->_kv.first)
		{
			parent->_right = newnode;
		}
		else
		{
			parent->_left = newnode;
		}
		newnode->_parent = parent;

		//3、自下而上调节平衡因子
		cur = newnode;
		parent = cur->_parent;
		while (parent)
		{
			//插在parent的左子树则bf--，否则++
			if (cur == parent->_left)
			{
				--parent->_bf;
			}
			else
			{
				++parent->_bf;
			}
			//检测当前是否平衡
			//情况一、新结点的插入使以parent为根的树平衡了
			//它不会影响当前树的深度，那么更不会影响到上面的树，因此直接break
			if (parent->_bf == 0)
			{
				break;
			}
			//情况二、新结点的插入使以parent为根的树一边比另一边高一层
			//它会影响到当前树的深度，也可能影响上面的树，因此继续向上调整
			else if (parent->_bf == 1 || parent->_bf == -1)
			{
				cur = parent;
				parent = parent->_parent;
			}
			//情况三、新结点的插入使以parent为根的树出现了不平衡（左右子树高度差大于1）
			//需要进行旋转
			else
			{
				//情况一、cur作为左子树，在它的左边插入后导致不平衡，进行右单旋
				if (parent->_bf == -2 && cur->_bf == -1)
				{
					rotateR(parent);
				}
				//情况二、cur作为左子树，在它的右边插入后导致不平衡，进行
				else if (parent->_bf == -2 && cur->_bf == 1)
				{
					rotateLR(parent);
				}
				//情况三、在右子树的右边插入后导致不平衡，进行左旋
				else if (parent->_bf == 2 && cur->_bf == 1)
				{
					rotateL(parent);
				}
				//情况四、在右子树的左边插入后导致不平衡，进行
				else if (parent->_bf == 2 && cur->_bf == -1)
				{
					rotateRL(parent);
				}
				//旋转完成后整棵树就平衡了，直接break
				break;
			}
		}//调节平衡因子循环结束
		return true;
	}
	void inOrderTraversal()
	{
		_inOrderTraversal(_root);
	}
private:
	void rotateL(Node* root)//左旋
	{
		Node* subR = root->_right;
		Node* subRL = subR->_left;
		Node* parent = root->_parent;//提前存储root的_parent
		//首先让root左旋下来，并处理root、subR、subRL的链接关系
		root->_right = subRL;
		if (subRL != nullptr)
			subRL->_parent = root;
		subR->_left = root;
		root->_parent = subR;
		//处理subR与root的_parent的链接关系
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
		//旋转完成后，调节平衡因子
		subR->_bf = root->_bf = 0;
	}
	void rotateR(Node* root)//右旋
	{
		Node* subL = root->_left;//root的左子树
		Node* subLR = subL->_right;//subL的右子树
		Node* parent = root->_parent;//提前存储root的_parent
		//通过旋转使root顺时针旋转至subL的右，并让subL代替root的位置
		root->_left = subLR;
		subL->_right = root;
		if (subLR != nullptr)
			subLR->_parent = root;
		//经过旋转后，若subL成为了新的根，则需要更新当前AVL树的_root
		if (parent == nullptr)
		{
			_root = subL;		
		}
		else//否则，链接subL和原来root的_parent
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
		int bf = subLR->_bf;//记录双旋前subLR的平衡因子
		//先进行一次左单旋，再进行一次右单旋
		rotateL(subL);
		rotateR(root);
		//接着根据不同情况调节平衡因子
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
		int bf = subRL->_bf;//记录双旋前subRL的平衡因子
		//先进行一次右单旋，再进行一次左单旋
		rotateR(subR);
		rotateL(root);
		//接着根据不同情况调节平衡因子
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