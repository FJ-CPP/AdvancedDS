#pragma once
#include <vector>
#include <list>
#include <cstdlib>
#include <ctime>

using namespace std;

/* 
 * 跳表：
 * 跳表本质就是一个链表，但是每个链表节点都会由特殊算法随机出一个层数level
 * level表示该节点
*/


struct SkipListNode
{
    int _val;
    vector< SkipListNode*> _next;
    SkipListNode(int val, int level)
        : _val(val), _next(level + 1, nullptr)
    {
    }
};

class SkipList 
{
    typedef SkipListNode Node;
private:
    int _maxLevel; // 跳表最多可以有多少层
    int _curLevel; // 当前跳表的层数
    double _p; // 节点升到更高一层的概率
    Node* _head;
public:
    SkipList(int maxLevel = 32, double p = 0.25)
        : _maxLevel(maxLevel), _curLevel(0), _p(p), _head(new Node(-1, _maxLevel))
    {
    }

    // 键值为target的节点是否存在
    bool Search(int target) 
    {
        int level = _curLevel;
        Node* cur = _head;

        while (level >= 0)
        {
            if (cur->_next[level] == nullptr || target < cur->_next[level]->_val)
            {
                --level;
            }
            else if (target > cur->_next[level]->_val)
            {
                cur = cur->_next[level];
            }
            else
            {
                return true;
            }
        }

        return false;
    }

    // 获取键值为num的节点的前驱结点
    void GetPreNode(int num, vector<Node*>& preNode)
    {
        int level = _curLevel;
        Node* cur = _head;

        while (level >= 0)
        {
            if (cur->_next[level] == nullptr || num <= cur->_next[level]->_val)
            {
                preNode[level] = cur;
                --level;
            }
            else
            {
                cur = cur->_next[level];
            }
        }
    }

    // 插入一个键值为num的节点
    void Add(int num) 
    {
        vector<Node*> preNode(_maxLevel, _head); // 记录结点在每一层的前驱结点，preNode[level]为_head表示在第level层不存在该节点
        int randomLevel = RandomLevel();
        _curLevel = max(_curLevel, randomLevel); // 更新跳表的当前高度

        Node* newNode = new Node(num, randomLevel);

        GetPreNode(num, preNode);

        // 将新结点在每一层链接起来
        for (int level = 0; level <= randomLevel; ++level)
        {
            newNode->_next[level] = preNode[level]->_next[level];
            preNode[level]->_next[level] = newNode;
        }
    }

    // 删除任意一个键值为num的节点
    bool Erase(int num) 
    {
        vector<Node*> preNode(_curLevel + 1, _head);
        GetPreNode(num, preNode);

        // 第0层前驱结点的下一个节点应当是待删除节点
        Node* deleteNode = preNode[0]->_next[0];

        if (deleteNode == nullptr || deleteNode->_val != num)
        {
            // 待删除节点不存在
            return false;
        }

        for (int level = 0; level < deleteNode->_next.size(); ++level)
        {
            if (preNode[level]->_next[level] && preNode[level]->_next[level]->_val == num)
            {
                preNode[level]->_next[level] = deleteNode->_next[level];
            }
        }

        delete deleteNode;

        while (_head->_next[_curLevel] == nullptr)
        {
            --_curLevel;
        }

        return true;
        //int level = _curLevel;
        //Node* cur = _head;
        //Node* deleteNode = nullptr;

        //while (level >= 0)
        //{
        //    if (cur->_next[level] == nullptr || num < cur->_next[level]->_val)
        //    {
        //        // 说明该节点在level层不存在
        //        --level;
        //    }
        //    else if (num > cur->_next[level]->_val)
        //    {
        //        cur = cur->_next[level];
        //    }
        //    else
        //    {
        //        // 说明该节点在level层存在，且为cur->_next[level]
        //        deleteNode = cur->_next[level];
        //        cur->_next[level] = deleteNode->_next[level];
        //        --level;
        //    }
        //}

        //if (deleteNode == nullptr)
        //{
        //    return false;
        //}

        //delete deleteNode;
        //return true;
    }

    int RandomLevel()
    {
        const static int MAX = 0x3f3f;
        int level = 0;
        
        // 升高一层的概率为_p
        while (rand() % MAX <= MAX * _p && level < _maxLevel)
        {
            ++level;
        }

        return level;
    }
};

