#pragma once
#include <list>
#include <unordered_map>
using namespace std;

/*
 * Cache的容量有限，因此当Cache的容量用完后，而又有新的内容需要添加进来时，
 * 就需要舍弃原有的部分内容，从而腾出空间来放新内容。
 * LRU Cache 的替换原则就是将最近最少使用的内容替换掉(Least Recently Used)
*/

/*
 * 实现 LRUCache 类：
 * LRUCache(int capacity) 以正整数作为容量capacity初始化 LRU 缓存
 * int get(int key) 
 *     如果关键字 key 存在于缓存中，则返回关键字的值，否则返回 -1 。
 * void put(int key, int value) 
 *     如果关键字 key 已经存在，则变更其数据值 value ；
 *     如果不存在，则向缓存中插入该组 key-value 。
 *     如果插入操作导致关键字数量超过 capacity ，则应该 逐出 最久未使用的关键字。
 * 函数 get 和 put 必须以 O(1) 的平均时间复杂度运行。
*/

class LRUCache 
{
private:
    int _capacity; // 最大容量
    typedef pair<int, int> pii;
    list<pii> _lruList; // lru链表，存储数据，接近链表尾表示该数据最近没有被使用
    unordered_map<int, list<pii>::iterator> _hash; // key : 存储key对应value的链表节点迭代器
public:
    LRUCache(int capacity) 
        : _capacity(capacity)
    {
    }

    int get(int key) 
    {
        if (_hash.find(key) == _hash.end())
        {
            return -1;
        }

        // 将key对应的lru节点移至链表开头，同时更新_hash中的迭代器
        int value = _hash[key]->second;
        _lruList.erase(_hash[key]);
        auto newIt = _lruList.insert(_lruList.begin(), make_pair(key, value));
        _hash[key] = newIt;

        return value;
    }

    void put(int key, int value) 
    {
        if (_hash.find(key) != _hash.end())
        {
            // 存在key，则更新value，将其移至_lru头部，并更新_hash中的迭代器
            _lruList.erase(_hash[key]);
            auto newIt = _lruList.insert(_lruList.begin(), make_pair(key, value));
            _hash[key] = newIt;
            return;
        }
        
        if (_hash.size() == _capacity)
        {
            // 新插入的数据超出容量，此时从lru链表中淘汰一个最近最少被使用的数据，并将其从hash中删除
            pii lruData = _lruList.back();
            _lruList.pop_back();
            _hash.erase(lruData.first);
        }

        // 将key-value头插lru链表和hash中
        auto it = _lruList.insert(_lruList.begin(), make_pair(key, value));
        _hash[key] = it;

    }
};