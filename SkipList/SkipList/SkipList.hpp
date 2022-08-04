#pragma once
#include <vector>
#include <list>
#include <cstdlib>
#include <ctime>

using namespace std;

/* 
 * ����
 * �����ʾ���һ����������ÿ������ڵ㶼���������㷨�����һ������level
 * level��ʾ�ýڵ�
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
    int _maxLevel; // �����������ж��ٲ�
    int _curLevel; // ��ǰ����Ĳ���
    double _p; // �ڵ���������һ��ĸ���
    Node* _head;
public:
    SkipList(int maxLevel = 32, double p = 0.25)
        : _maxLevel(maxLevel), _curLevel(0), _p(p), _head(new Node(-1, _maxLevel))
    {
    }

    // ��ֵΪtarget�Ľڵ��Ƿ����
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

    // ��ȡ��ֵΪnum�Ľڵ��ǰ�����
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

    // ����һ����ֵΪnum�Ľڵ�
    void Add(int num) 
    {
        vector<Node*> preNode(_maxLevel, _head); // ��¼�����ÿһ���ǰ����㣬preNode[level]Ϊ_head��ʾ�ڵ�level�㲻���ڸýڵ�
        int randomLevel = RandomLevel();
        _curLevel = max(_curLevel, randomLevel); // ��������ĵ�ǰ�߶�

        Node* newNode = new Node(num, randomLevel);

        GetPreNode(num, preNode);

        // ���½����ÿһ����������
        for (int level = 0; level <= randomLevel; ++level)
        {
            newNode->_next[level] = preNode[level]->_next[level];
            preNode[level]->_next[level] = newNode;
        }
    }

    // ɾ������һ����ֵΪnum�Ľڵ�
    bool Erase(int num) 
    {
        vector<Node*> preNode(_curLevel + 1, _head);
        GetPreNode(num, preNode);

        // ��0��ǰ��������һ���ڵ�Ӧ���Ǵ�ɾ���ڵ�
        Node* deleteNode = preNode[0]->_next[0];

        if (deleteNode == nullptr || deleteNode->_val != num)
        {
            // ��ɾ���ڵ㲻����
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
        //        // ˵���ýڵ���level�㲻����
        //        --level;
        //    }
        //    else if (num > cur->_next[level]->_val)
        //    {
        //        cur = cur->_next[level];
        //    }
        //    else
        //    {
        //        // ˵���ýڵ���level����ڣ���Ϊcur->_next[level]
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
        
        // ����һ��ĸ���Ϊ_p
        while (rand() % MAX <= MAX * _p && level < _maxLevel)
        {
            ++level;
        }

        return level;
    }
};

