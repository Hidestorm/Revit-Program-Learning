/*!
*@file    Dll7zipMgr.h
*@brief   7zip格式文件调用接口的定义与实现
*/

#ifndef _DLL7ZIPMGR_
#define _DLL7ZIPMGR_

#include "Dll7zip.h"

#include <vector>
using namespace std;

/*!
*@class C7zManager
*@breif 7zip格式文件调用管理类
*/
class DLL7ZIP_CLASS C7zManager
{
public:
    /*brief  将若干文件压缩成一个7zip格式的压缩包
    @param[in] pFiles 需进行压缩的文件的绝对路径列表
    @param[in] pPassword 压缩密码(如无需密码该项设置为nullptr)
    @param[in] pDestFileName 生成的7zip压缩包的绝对路径
    @return 压缩是否成功
    */
    static bool CompressFiles7z(vector<wchar_t*>& pFiles,wchar_t* pPassword,wchar_t* pDestFileName);

     /*brief  解压7zip格式的压缩包
    @param[in] pExtractFile 7zip格式压缩包的绝对路径
    @param[in] pDestFolder 表示文件解压在哪个文件夹下
    @param[in] pPassword 解压密码(如无需密码该项设置为nullptr)
    @param[out] pFiles 解压后各生成文件的绝对路径
    @return 解压是否成功
    */
    static bool ExtractFiles7z(wchar_t* pExtractFile, wchar_t* pDestFolder, wchar_t* pPassword,vector<wchar_t*>& pFiles);

    /*brief  将7zip格式压缩包中的某个文件，解压到指定路径
    @param[in] pExtractFile 7zip格式压缩包的绝对路径
    @param[in] pSpecifiedFileName 压缩包中需解压的那个文件的文件名（仅文件名，非路径）
    @param[in] pPassword 解压密码(如无需密码该项设置为nullptr)
    @param[in] pDesFilePath 解压后生成文件的绝对路径
    @return 解压是否成功
    */
    static bool ExtractSpecifiedFile7z(wchar_t* pExtractFile, wchar_t* pSpecifiedFileName, wchar_t* pPassword,wchar_t* pDesFilePath);

    /*brief  获取7zip格式的压缩包中的文件列表
    @param[in] pExtractFile 7zip格式压缩包的绝对路径
    @param[in] pPassword 解压密码(如无需密码该项设置为nullptr)
    @param[out] pFiles 压缩包中的文件列表
    @return 获取文件列表是否成功
     */
    static bool LoadFilesFrom7z(wchar_t* pExtractFile,wchar_t* pPassword,vector<wchar_t*>& pFiles);
};

#endif