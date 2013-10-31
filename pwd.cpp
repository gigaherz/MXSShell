#include "ShellApi.h"

class PwdCommand : public Command
{
public:
	PwdCommand()
		: Command(String(_T("pwd")), RegisterAsCommand | RegisterAsFunction)
	{
	}

	virtual bool ExecFunction(String &rettext, StringVector params, String cmdline)
	{
		Environment::LoadCurrentDirectory(rettext);
		return true;
	}

} Pwd_Instance;