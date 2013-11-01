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
            /*const _TCHAR *tline=text.c_str();
            varname.clear();
            while(*tline!=NULL)
            {
            _TCHAR ch=*(tline++);
            if(((ch>=_T('a'))&&(ch<=_T('z')))||
            ((ch>=_T('A'))&&(ch<=_T('Z')))||
            (ch==_T('_')))
            {
            varname.append(1,ch);
            }
            else break;
            }
            tline--;
            while(*tline!=NULL)
            {
            _TCHAR ch=*(tline++);
            if((ch==_T('=')))
            {
            break;
            }
            else if(ch!=_T(' '))
            {
            cout << "Syntax error: Invalid character (expected '=')" << endl;
            return false;
            }
            }
            while(*tline!=NULL)
            {
            _TCHAR ch=*(tline++);
            value.append(1,ch);
            }*/
            Environment::SetVariable(varname, value);

        }
        return true;
    }

} Set_Instance;