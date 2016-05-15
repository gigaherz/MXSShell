#include "ShellApi.h"

class ErrorCommand : public Command
{
public:
    ErrorCommand()
        : Command(String(_T("error")), RegisterAsCommand | RegisterAsFunction)
    {
    }

    virtual bool ExecCommand(StringVector params, String cmdline) override
    {
        if (params.size() > 2)
            return false;

        if (params.size() == 1)
        {
            int error = Environment::GetError();
            _tcout << error << endl;
        }
        else
        {
            Environment::SetError(_tcstol(params.at(1).c_str(), nullptr, 0));
        }

        return true;
    }

    virtual bool ExecFunction(String &rettext, StringVector params, String cmdline) override
    {
        wchar_t buffer[20];
        int error = Environment::GetError();

        swprintf_s(buffer, _T("%d"), error);

        rettext.clear();
        rettext.append(buffer);

        return true;
    }

} Error_Instance;
