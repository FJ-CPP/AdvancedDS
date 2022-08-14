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
			_bit.resize(N / 32 + 1, 0);//����ϣ���ʼ��Ϊ0
		}
		//����x��bit�ó�1
		void set(size_t x)
		{
			size_t index = x / 32;//�����x��bitλ�������͵��±�
			size_t pos = x % 32;//������Ǹ����͵ĵڼ���bitλ[0,31]
			_bit[index] |= (1 << pos);//�����ĵ�posλ�ó�1
			++_cnt;
			/*
			* ע������<<�������ǽ�ĳ���������ĸ�λ�ƶ�
			* ����С�˴洢ģʽ���ұߴ洢��λ
			* ���ڴ�˴洢ģʽ����ߴ洢��λ
			*/
		}
		//����x��bit�ָ�Ϊ0
		void reset(size_t x)
		{
			size_t index = x / 32;
			size_t pos = x % 32;
			if (test(x))//�����xλ��1�Ļ�����ô����������-1
				--_cnt;
			_bit[index] &= ~(1 << pos);
		}
		//����x��bit�Ƿ��ѱ����ã����Ƿ�Ϊ1��
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
		vector<int> _bit;//ӳ���ϣ��������Ϊ��С��λ�洢��ÿ��ӳ��һ��bitλ
		size_t _cnt;//��¼�Ѿ�ӳ�����������
	};
}
