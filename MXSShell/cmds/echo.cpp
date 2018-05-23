#include "ShellApi.h"

class EchoCommand : public Command
{
public:
    EchoCommand()
        : Command(String(_T("echo")), RegisterAsCommand)
    {
    }

    bool ExecCommand(StringVector params, String cmdline) override
    {
        String text;
        Str::JoinVector(text, params, 1, params.size() - 1, _T(' '));
        _tcout << text << endl;
        return true;
    }

} Echo_Instance;