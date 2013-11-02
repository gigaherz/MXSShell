#include "ShellApi.h"

class SetCommand : public Command
{
public:
    SetCommand()
        : Command(String(_T("set")), RegisterAsCommand)
    {
    }

    virtual bool ExecCommand(StringVector params, String cmdline)
    {
        String text;
        String varname;
        String value;
        Str::JoinVector(text, params, 1, params.size() - 1, _T(' '));

        if (text.length() == 0)
        {
            _TCHAR *env = (_TCHAR*) GetEnvironmentStrings();
            _TCHAR *ptr = env;

            while (*ptr != NULL)
            {
                value.clear();
                while (*ptr != NULL)
                {
                    value.append(1, *(ptr++));
                }
                if (value.at(0) != _T('='))
                {
                    _tcout << value.c_str() << endl;
                }
                ptr++;
            }

            FreeEnvironmentStrings(env);
        }
        else
        {
            varname = params[1];
            Str::JoinVector(value, params, 2, params.size() - 1, _T(' '));
            Environment::SetVariable(varname, value);
        }
        return true;
    }

} Set_Instance;