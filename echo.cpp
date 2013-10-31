#include "ShellApi.h"

class EchoCommand : public Command
{
public:
	EchoCommand()
		: Command(String(_T("echo")), RegisterAsCommand)
	{
	}
	
	virtual bool ExecCommand(StringVector params, String cmdline)
	{
		String text;
		Str::JoinVector(text,params,1,params.size()-1,_T(' '));
		_tcout << text << endl;
		return true;
	}

} Echo_Instance;