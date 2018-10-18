/*!
*@file    Dll7zipMgr.h
*@brief   7zip��ʽ�ļ����ýӿڵĶ�����ʵ��
*/

#ifndef _DLL7ZIPMGR_
#define _DLL7ZIPMGR_

#include "Dll7zip.h"

#include <vector>
using namespace std;

/*!
*@class C7zManager
*@breif 7zip��ʽ�ļ����ù�����
*/
class DLL7ZIP_CLASS C7zManager
{
public:
    /*brief  �������ļ�ѹ����һ��7zip��ʽ��ѹ����
    @param[in] pFiles �����ѹ�����ļ��ľ���·���б�
    @param[in] pPassword ѹ������(�����������������Ϊnullptr)
    @param[in] pDestFileName ���ɵ�7zipѹ�����ľ���·��
    @return ѹ���Ƿ�ɹ�
    */
    static bool CompressFiles7z(vector<wchar_t*>& pFiles,wchar_t* pPassword,wchar_t* pDestFileName);

     /*brief  ��ѹ7zip��ʽ��ѹ����
    @param[in] pExtractFile 7zip��ʽѹ�����ľ���·��
    @param[in] pDestFolder ��ʾ�ļ���ѹ���ĸ��ļ�����
    @param[in] pPassword ��ѹ����(�����������������Ϊnullptr)
    @param[out] pFiles ��ѹ��������ļ��ľ���·��
    @return ��ѹ�Ƿ�ɹ�
    */
    static bool ExtractFiles7z(wchar_t* pExtractFile, wchar_t* pDestFolder, wchar_t* pPassword,vector<wchar_t*>& pFiles);

    /*brief  ��7zip��ʽѹ�����е�ĳ���ļ�����ѹ��ָ��·��
    @param[in] pExtractFile 7zip��ʽѹ�����ľ���·��
    @param[in] pSpecifiedFileName ѹ���������ѹ���Ǹ��ļ����ļ��������ļ�������·����
    @param[in] pPassword ��ѹ����(�����������������Ϊnullptr)
    @param[in] pDesFilePath ��ѹ�������ļ��ľ���·��
    @return ��ѹ�Ƿ�ɹ�
    */
    static bool ExtractSpecifiedFile7z(wchar_t* pExtractFile, wchar_t* pSpecifiedFileName, wchar_t* pPassword,wchar_t* pDesFilePath);

    /*brief  ��ȡ7zip��ʽ��ѹ�����е��ļ��б�
    @param[in] pExtractFile 7zip��ʽѹ�����ľ���·��
    @param[in] pPassword ��ѹ����(�����������������Ϊnullptr)
    @param[out] pFiles ѹ�����е��ļ��б�
    @return ��ȡ�ļ��б��Ƿ�ɹ�
     */
    static bool LoadFilesFrom7z(wchar_t* pExtractFile,wchar_t* pPassword,vector<wchar_t*>& pFiles);
};

#endif