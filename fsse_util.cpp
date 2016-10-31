
#include <shlobj.h>
#include "fsse_util.h"

BOOL FileExists(LPCTSTR szPath) {
    DWORD dwAttrib = GetFileAttributes(szPath);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
            !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

BOOL FolderExists(LPCTSTR szPath) {
    DWORD dwAttrib = GetFileAttributes(szPath);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
            (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void GetStartupFolderPath(TCHAR* buf)
{
    HRESULT hr = SHGetFolderPath(NULL, CSIDL_STARTUP, NULL, 0, buf);
}