#include "ShellApi.h"

class ExitCommand : public Command
{
public:
    ExitCommand()
        : Command(String(_T("exit")), RegisterAsCommand)
    {
    }

    bool ExecCommand(StringVector params, String cmdline) override
    {
        return Exec::Exit(params, cmdline);
    }

} Exit_Instance;