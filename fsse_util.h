
#ifndef FSSE_FSSE_UTIL_H
#define FSSE_FSSE_UTIL_H

#include <windows.h>

BOOL FileExists(LPCTSTR szPath);

BOOL FolderExists(LPCTSTR szPath);

void GetStartupFolderPath(TCHAR *buf);

#endif //FSSE_FSSE_UTIL_H
