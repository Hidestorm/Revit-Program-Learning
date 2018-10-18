
/*
* 一个使用公共资源/Dll7zipMgr.h进行压缩的demo，但是压缩的时候会崩溃，找不到原因
*/

#include <string.h>
#include <vector>
#include <iostream>
#include "标头.h"

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