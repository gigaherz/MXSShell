#include "ShellApi.h"

class ConcatCommand : public Command
{
public:
	ConcatCommand()
		: Command(String(_T("+")), RegisterAsCommand | RegisterAsFunction)
	{
	}

	virtual bool ExecFunction(String &rettext, StringVector params, String cmdline)
	{
		Str::JoinVector(rettext, params, 1, params.size()-1, -1);
		return true;
	}

} Concat_Instance;