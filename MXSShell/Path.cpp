#include "Main.h"

bool Path::FileExists(const String path)
{
    return ::PathFileExists(path.c_str()) != FALSE;
}

bool Path::FileHasExtension(const String path)
{
    String::size_type a, b, c;

    a = path.find(_T('/'));
    b = path.find(_T('\\'));
    c = path.find(_T('.'));

    if (c == String::npos) return false;
    if (c < b) return false;
    if (c < a) return false;

    return true;
}

bool Path::EnumerateDirectory(StringVector fileNames)
{
    WIN32_FIND_DATA ffd;
    String pwd;

    Environment::LoadCurrentDirectory(pwd);

    pwd.append(_T("\\*"));

    HANDLE hFind = FindFirstFile(pwd.c_str(), &ffd);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        _tcout << _T("ERROR: Could not enumerate directory: ") << GetLastError() << endl;
        return false;
    }

    do
    {
        fileNames.push_back(ffd.cFileName);
    } while (FindNextFile(hFind, &ffd) != 0);

    DWORD dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        _tcout << _T("ERROR: While enumerating directory: ") << dwError << endl;
    }

    FindClose(hFind);
    return true;
}
