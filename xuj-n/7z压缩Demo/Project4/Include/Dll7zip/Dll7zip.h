#pragma once

// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� DLL7ZIP_EXPORTS
// ���ű���ġ�

#ifdef DLL7ZIP_EXPORTS
#define DLL7ZIP_CLASS __declspec(dllexport)
#define DLL7ZIP_CLASS_API __declspec(dllexport)
#else
#define DLL7ZIP_CLASS __declspec(dllimport)
#define DLL7ZIP_CLASS_API __declspec(dllimport)
#endif

#include <stdint.h>



