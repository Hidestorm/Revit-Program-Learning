#pragma once

// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 DLL7ZIP_EXPORTS
// 符号编译的。

#ifdef DLL7ZIP_EXPORTS
#define DLL7ZIP_CLASS __declspec(dllexport)
#define DLL7ZIP_CLASS_API __declspec(dllexport)
#else
#define DLL7ZIP_CLASS __declspec(dllimport)
#define DLL7ZIP_CLASS_API __declspec(dllimport)
#endif

#include <stdint.h>



