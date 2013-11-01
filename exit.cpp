#include "ShellApi.h"

class ExitCommand : public Command
{
public:
    ExitCommand()
        : Command(String(_T("exit")), RegisterAsCommand)
    {
    }

    virtual bool ExecCommand(StringVector params, String cmdline)
    {
        return Exec::Exit(params, cmdline);
    }

} Exit_Instance;