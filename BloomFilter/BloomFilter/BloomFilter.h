#pragma once
#include <bitset>
#include <string>
/*
* 布隆过滤器用于检索某一个数据是否存在于集合中
* 
* 布隆过滤器的底层原理：
* 针对某一个数据，我们通过K个哈希函数给它在位图中映射K个位置
* 相较于普通位图一个数据映射一个位置，它能降低哈希冲突的概率，并且相较于map/set，它能大大地减少空间的使用
* 也正因为一个数据会映射多个位置，所以布隆过滤器会存在误判的问题，以及删除十分困难（因为一旦删除，可能会影响其它一个甚至多个数据）
* 
* 假设布隆过滤器底层位图的大小为M，需要过滤的样本数量为N，允许的判断失误率是P
* 那么它们满足：M = -(N×lnP)/(ln2)^2
* 哈希函数的个数K满足K = (M/N)*ln2   【ln2≈0.69】
*/

//字符串BKDR哈希算法
struct BKDR_Hash
{
	size_t operator()(const string& s)
	{
		size_t hash = 0;
		for (size_t i = 0; i < s.size(); ++i)
		{
			hash = hash * 131 + s[i];
		}
		return hash;
	}
};
//字符串AP哈希算法
struct AP_Hash
{
	size_t operator()(const string& s)
	{
		size_t hash = 0;
		int flg = 0;
		for (size_t i = 0; i < s.size(); ++i)
		{
			if ((flg & 1) == 0)
			{
				hash ^= ((hash << 7) ^ s[i] ^ (hash >> 3));
			}
			else
			{
				hash ^= (~((hash << 11) ^ s[i] ^ (hash >> 5)));
			}
		}
		return hash;
	}
};
//字符串DJB哈希算法
struct DJB_Hash
{
	size_t operator()(const string& s)
	{
		size_t hash = 5381;
		for (size_t i = 0; i < s.size(); ++i)
		{
			hash += (hash << 5) + s[i];
		}
		return hash;
	}
};
template <size_t N, class T = string, class Hash1 = BKDR_Hash, class Hash2 = AP_Hash, class Hash3 = DJB_Hash>
class BloomFliter
{
public:
	//将key插入到集合中
	void set(const T& key)
	{
		size_t index1 = h1(key) % _bs.size();
		size_t index2 = h2(key) % _bs.size();
		size_t index3 = h3(key) % _bs.size();
		_bs.set(index1);
		_bs.set(index2);
		_bs.set(index3);
	}
	//检查key是否存在
	bool test(const T& key)
	{
		size_t index1 = h1(key) % _bs.size();
		size_t index2 = h2(key) % _bs.size();
		size_t index3 = h3(key) % _bs.size();
		return _bs.test(index1) && _bs.test(index2) && _bs.test(index3);
	}
private:
	//根据K = (M/N)*ln2的公式，布隆过滤器底层位图的大小M = N*K/ln2 【这里我们给定哈希函数个数K为3】
	bitset<(size_t)(4.3 * N)> _bs;
	Hash1 h1;
	Hash2 h2;
	Hash3 h3;
};
