#pragma once
#include <vector>
namespace myBitSet
{
	template <size_t N>
	class bitset
	{
	public:
		bitset()
			:_cnt(0)
		{
			_bit.resize(N / 32 + 1, 0);//将哈希表初始化为0
		}
		//将第x个bit置成1
		void set(size_t x)
		{
			size_t index = x / 32;//求出第x个bit位所在整型的下标
			size_t pos = x % 32;//求出它是该整型的第几个bit位[0,31]
			_bit[index] |= (1 << pos);//将它的第pos位置成1
			++_cnt;
			/*
			* 注：左移<<操作符是将某个数向它的高位移动
			* 对于小端存储模式，右边存储高位
			* 对于大端存储模式，左边存储高位
			*/
		}
		//将第x个bit恢复为0
		void reset(size_t x)
		{
			size_t index = x / 32;
			size_t pos = x % 32;
			if (test(x))//如果第x位是1的话，那么把总数据量-1
				--_cnt;
			_bit[index] &= ~(1 << pos);
		}
		//检查第x个bit是否已被设置（即是否为1）
		bool test(size_t x)
		{
			size_t index = x / 32;
			size_t pos = x % 32;
			return _bit[index] & (1 << pos);
		}
		size_t size()
		{
			return _cnt;
		}
	private:
		vector<int> _bit;//映射哈希表，以整型为最小单位存储，每次映射一个bit位
		size_t _cnt;//记录已经映射的数据总数
	};
}
