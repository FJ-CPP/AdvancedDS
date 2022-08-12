#pragma once
#include <bitset>
#include <string>
/*
* ��¡���������ڼ���ĳһ�������Ƿ�����ڼ�����
* 
* ��¡�������ĵײ�ԭ��
* ���ĳһ�����ݣ�����ͨ��K����ϣ����������λͼ��ӳ��K��λ��
* �������ͨλͼһ������ӳ��һ��λ�ã����ܽ��͹�ϣ��ͻ�ĸ��ʣ����������map/set�����ܴ��ؼ��ٿռ��ʹ��
* Ҳ����Ϊһ�����ݻ�ӳ����λ�ã����Բ�¡��������������е����⣬�Լ�ɾ��ʮ�����ѣ���Ϊһ��ɾ�������ܻ�Ӱ������һ������������ݣ�
* 
* ���貼¡�������ײ�λͼ�Ĵ�СΪM����Ҫ���˵���������ΪN��������ж�ʧ������P
* ��ô�������㣺M = -(N��lnP)/(ln2)^2
* ��ϣ�����ĸ���K����K = (M/N)*ln2   ��ln2��0.69��
*/

//�ַ���BKDR��ϣ�㷨
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
//�ַ���AP��ϣ�㷨
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
//�ַ���DJB��ϣ�㷨
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
	//��key���뵽������
	void set(const T& key)
	{
		size_t index1 = h1(key) % _bs.size();
		size_t index2 = h2(key) % _bs.size();
		size_t index3 = h3(key) % _bs.size();
		_bs.set(index1);
		_bs.set(index2);
		_bs.set(index3);
	}
	//���key�Ƿ����
	bool test(const T& key)
	{
		size_t index1 = h1(key) % _bs.size();
		size_t index2 = h2(key) % _bs.size();
		size_t index3 = h3(key) % _bs.size();
		return _bs.test(index1) && _bs.test(index2) && _bs.test(index3);
	}
private:
	//����K = (M/N)*ln2�Ĺ�ʽ����¡�������ײ�λͼ�Ĵ�СM = N*K/ln2 ���������Ǹ�����ϣ��������KΪ3��
	bitset<(size_t)(4.3 * N)> _bs;
	Hash1 h1;
	Hash2 h2;
	Hash3 h3;
};
