#include "БъЭЗ.h"
#include "Dll7zip/Dll7zipMgr.h"

bool Compress(std::vector<wchar_t*>& source, wchar_t * key, wchar_t* der)
{
	try
	{
		return C7zManager::CompressFiles7z(source, key, der);
	}
	catch (...)
	{
		return false;
	}
}
