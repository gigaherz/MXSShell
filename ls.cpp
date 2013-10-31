#include "ShellApi.h"
#include <Windows.h>
#include <strsafe.h>

class LsCommand : public Command
{
public:
    LsCommand()
        : Command(String(_T("ls")), RegisterAsCommand | RegisterAsFunction)
    {
    }

    // if used as a command, ls lists the contents of the directory
    virtual bool ExecCommand(StringVector params, String cmdline)
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
            _tcout << ffd.cFileName << endl;
        } while (FindNextFile(hFind, &ffd) != 0);

        DWORD dwError = GetLastError();
        if (dwError != ERROR_NO_MORE_FILES)
        {
            _tcout << _T("ERROR: While enumerating directory: ") << dwError << endl;
        }

        FindClose(hFind);
        return true;
    }

    // if used as a function, ls returns the list as a string
    virtual bool ExecFunction(String &rettext, StringVector params, String cmdline)
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

        // List all the files in the directory with some info about them.
        bool first = true;
        do
        {
            if (!first)
                rettext.append(_T(" "));
            rettext.append(_T("\""));
            rettext.append(ffd.cFileName);
            rettext.append(_T("\""));
            first = false;
        } while (FindNextFile(hFind, &ffd) != 0);

        DWORD dwError = GetLastError();
        if (dwError != ERROR_NO_MORE_FILES)
        {
            _tcout << _T("ERROR: While enumerating directory: ") << dwError << endl;
        }

        FindClose(hFind);
        return true;
    }

} Ls_Instance;