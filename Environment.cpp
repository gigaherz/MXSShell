#include "ShellApi.h"

bool Environment::GetVariable(String &dest, String envvar, String defval)
{
    static _TCHAR lstr[32768];
    SetLastError(0);
    size_t lenv = GetEnvironmentVariable(envvar.c_str(), lstr, 32767);
    int    lastError = GetLastError();

    if ((lenv == 0) && (lastError != 0))
    {
        dest.assign(defval);
        return false;
    }

    dest.assign(lstr);

    return true;
}

bool Environment::SetVariable(String varname, String value)
{
    return (SetEnvironmentVariable(varname.c_str(), value.c_str()) != FALSE);
}

bool Environment::LoadCurrentDirectory(String &dest)
{
    static _TCHAR lstr[32768];
    SetLastError(0);
    size_t lenv = GetCurrentDirectory(32767, lstr);
    int    lastError = GetLastError();

    if ((lenv == 0) && (lastError != 0))
    {
        dest.assign(_T("C:\\"));
        return false;
    }

    dest.assign(lstr);

    return true;
}

bool Environment::ChangeCurrrentDirectory(String NDir)
{
    String CDir;

    LoadCurrentDirectory(CDir);

    static _TCHAR t[32768];

    PathCombine(t, CDir.c_str(), NDir.c_str());

    SetLastError(0);
    SetCurrentDirectory(t);

    return (GetLastError() != 0);
}
