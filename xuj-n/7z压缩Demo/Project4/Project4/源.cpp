
/*
* һ��ʹ�ù�����Դ/Dll7zipMgr.h����ѹ����demo������ѹ����ʱ���������Ҳ���ԭ��
*/

#include <string.h>
#include <vector>
#include <iostream>
#include "��ͷ.h"

void main()
{
	wchar_t t1[] = L"F:/CloudMusic/EGOIST.mp3";
	wchar_t t2[] = L"F:/CloudMusic/loadstar.mp3";

	wchar_t des[] = L"F:/CloudMusic/test.7z";
	wchar_t key[] = L"123";
	std::cout << t1;
	std::cout << t2;

	std::vector<wchar_t*> vec;
	vec.push_back(t1);
	vec.push_back(t2);

	//system("pause");

	Compress(vec, key, des);
	return ;

}