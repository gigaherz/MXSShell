#include "ShellApi.h"
#include "Streaming.h"

#include <map>

const String defexts(_T(".CMD;.BAT;.EXE"));
const String defpath(_T("."));

// They need to be pointers because C++ does not have a defined static initialization order.
map<String, Command*>* commandIndex = NULL;
map<String, Command*>* functionIndex = NULL;

bool Exec::RegisterCommand(String name, Command* instance, int regAs)
{
	// Check if registering will be possible without colliding with other registered commands
	if(!!(regAs & RegisterAsCommand))
	{
		if(!commandIndex)
		{
			commandIndex = new map<String, Command*>();
		}
		if(commandIndex->find(name) != commandIndex->end())
			return false;
	}
	if(!!(regAs & RegisterAsFunction))
	{
		if(!functionIndex)
		{
			functionIndex = new map<String, Command*>();
		}
		if(functionIndex->find(name) != functionIndex->end())
			return false;
	}

	// Register the command
	if(!!(regAs & RegisterAsCommand))
	{
		(*commandIndex)[name] = instance;
	}

	if(!!(regAs & RegisterAsFunction))
	{
		(*functionIndex)[name] = instance;
	}

	return true;
}

bool Exec::FindProgram(String &dest, String name)
{
	String path;
	String exts;
	StringVector ppath;
	StringVector pext;

	int lpatherror=Environment::GetVariable(path,_T("PATH"),defpath);
	int lexterror=Environment::GetVariable(exts,_T("PATHEXT"),defexts);

	Str::SplitLine(ppath,path,_T(';'));
	Str::SplitLine(pext,exts,_T(';'));

	if(Path::FileHasExtension(name)||(pext.size()==0)) //do not attempt to add an extension
		for(StringVector::iterator pit=ppath.begin();pit!=ppath.end();pit++)
		{
			String cpath(*pit);
			if((cpath.at(cpath.size()-1)!=_T('/'))&&(cpath.at(cpath.size()-1)!=_T('\\')))
				//				cpath.append(1,_T('\\'));
				cpath.append(1,_T('/'));
			cpath.append(name);

			if(PathFileExists(cpath.c_str()))
			{
				dest.assign(cpath);
				return true;
			}
		}
	else
	{

		for(StringVector::iterator pit=ppath.begin();pit!=ppath.end();pit++)
		{
			for(StringVector::iterator eit=pext.begin();eit!=pext.end();eit++)
			{
				String cpath(*pit);
				String cext(*eit);

				if((cpath.at(cpath.size()-1)!=_T('/'))&&(cpath.at(cpath.size()-1)!=_T('\\')))
					//					cpath.append(1,_T('\\'));
					cpath.append(1,_T('/'));

				cpath.append(name);
				cpath.append(cext);

				if(PathFileExists(cpath.c_str()))
				{
					dest.assign(cpath);
					return true;
				}
			}
		}
	}

	return false;
}

bool Exec::ExecCommand(StringVector params, String cmdline, String* _rettext)
{
	String prog;
	String cline(cmdline);

	bool asFunction = _rettext != NULL;

	auto itc = commandIndex->find(params[0]);
	auto itf = functionIndex->find(params[0]);

	if(asFunction)
	{
		_rettext->clear();

		if(itf != functionIndex->end())
		{
			return itf->second->ExecFunction(*_rettext, params, cmdline);

		}
		else if(itc != commandIndex->end())
		{
			// do not allow executing a function externally if the same name is registered as a command
			_tcout << _T("Cannot execute '") << params[0] << _T("' as a function.") << endl;
			return false;
		}
	}
	else
	{
		if(itc != commandIndex->end())
		{
			return itc->second->ExecCommand(params, cmdline);
		}
		else if(itf != functionIndex->end())
		{
			// do not allow executing a command externally if the same name is registered as a function		if(!FindProgram(prog,params[0]))
			_tcout << _T("Cannot execute '") << params[0] << _T("' as a command.") << endl;
			return false;
		}
	}

	if(params[0].compare(_T("exec"))==0)
	{
		Str::JoinVector(cline,params,1,params.size()-1,_T(' '));

		if((params[1].find(_T('/'))==String::npos)&&(params[1].find(_T('\\'))==String::npos))
		{
			if(!FindProgram(prog,params[1])) return false;
		}
		else
			prog=params[1];
	}
	else if((params[0].find(_T('/'))==String::npos)&&(params[0].find(_T('\\'))==String::npos))
	{
		if(!FindProgram(prog,params[0]))
		{
			_tcout << _T("Cannot find: ") << params[0] << endl;
			return false;
		}
	}
	else
		prog=params[0];


#ifdef WIN32
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// Function only
	HANDLE hWrite,hRead;

	if(asFunction)
	{
		SECURITY_ATTRIBUTES sa;
		sa.nLength=sizeof(sa);
		sa.bInheritHandle=TRUE;
		sa.lpSecurityDescriptor=NULL;

		if(!CreatePipe(&hRead,&hWrite,&sa,0))
		{
			cout << "CreatePipe failed (" << GetLastError() << ")." << endl;
			return false;
		}
	}

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	if(asFunction)
	{
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdInput=GetStdHandle(STD_INPUT_HANDLE);
		si.hStdOutput=hWrite;
		si.hStdError=GetStdHandle(STD_ERROR_HANDLE);
	}
	ZeroMemory( &pi, sizeof(pi) );

	_TCHAR *cls= new _TCHAR[cmdline.length()+1];
	_tcscpy_s(cls,cmdline.length()+1,cmdline.c_str());

	// Start the child process. 
	if( !CreateProcess(
		prog.c_str(),		// No module name (use command line). 
		cls,	// Command line. 
		NULL,		// Process handle not inheritable. 
		NULL,		// Thread handle not inheritable. 
		asFunction?TRUE:FALSE,
		0,			// No creation flags. 
		NULL,		// Use parent's environment block. 
		NULL,		// Use parent's starting directory. 
		&si,		// Pointer to STARTUPINFO structure.
		&pi )		// Pointer to PROCESS_INFORMATION structure.
		) 
	{
		cout << "CreateProcess failed (" << GetLastError() << ")." << endl;
		delete[] cls;
		return false;
	}

	if(asFunction)
	{
		Win32PipeReader stream(hRead);
		for(;;) {
			/*static char dbuff[1024];
			static _TCHAR wbuff[1024];
			DWORD read;
			if(!ReadFile(hRead,dbuff,1023,&read,NULL))
			{
				cout << "Pipe Read failed (" << GetLastError() << ")." << endl;
				delete[] cls;
				return false;
			}
			dbuff[read]=0;
			if(read==0) break;

			MultiByteToWideChar(CP_THREAD_ACP,0,dbuff,read,wbuff,1024);

			rettext.append(wbuff);*/

			stream.ReadAvailableBytes(*_rettext);

			DWORD exitCode;
			if(!GetExitCodeProcess(pi.hProcess,&exitCode))
			{
				delete[] cls;
				return false;
			}
			if(exitCode!=STILL_ACTIVE)
			{
				stream.ReadAvailableBytes(*_rettext);
				break;
			}
		}
	}
	else
	{
		// Wait until child process exits.
		WaitForSingleObject( pi.hProcess, INFINITE );
	}

	// Close process and thread handles. 
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	if(asFunction)
	{
		CloseHandle( hRead );
		CloseHandle( hWrite );
	}

	delete[] cls;

	return true;
#else
	//TODO: implement it for non-windows platforms
	return false; //not implemented
#endif
}
