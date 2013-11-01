#include "ShellApi.h"

using namespace std;

bool Running;
long ExitCode;

bool Exec::Exit(StringVector params, String cmdline)
{
	if(params.size()==2)
	{
		ExitCode=_tcstol(params[1].c_str(),NULL,0);
	}
	Running=false;
	return true;
}

bool GetText(String &dest, String line, _TCHAR delim)
{
	dest.clear();
	for(String::const_iterator sit=line.begin();sit!=line.end();sit++)
	{
		if(*sit==delim)
			return true;
		dest.append(1,*sit);
	}

	if(delim!=0)
	{
		_tcout << "Syntax Error: End of line found in unfinished quoted string." << endl;
		return false;
	}
	return true;
}

bool ParseLine(String &dest, String &src, _TCHAR delim)
{
	String line;
	String tmp;
	const _TCHAR *tline;

	dest.clear();
	for(tline=src.c_str();*tline!=NULL;tline++)
	{
		_TCHAR ch=*tline;

		if(ch==_T('\\'))
		{
			ch=*(++tline);
			switch(ch)
			{
			case _T('a'): line.append(1,_T('\a')); break;
			case _T('b'): line.append(1,_T('\b')); break;
			case _T('f'): line.append(1,_T('\f')); break;
			case _T('n'): line.append(1,_T('\n')); break;
			case _T('r'): line.append(1,_T('\r')); break;
			case _T('t'): line.append(1,_T('\t')); break;
			case _T('v'): line.append(1,_T('\v')); break;
			case _T(' '): line+=_T("\\ "); break;
			case _T('\''): line+=_T("\\'"); break;
			case _T('"'): line+=_T("\\\""); break;
			case _T('?'): line+=_T("\\?"); break;
			case _T('$'): line+=_T("\\$"); break;
			case _T('%'): line+=_T("\\%"); break;
				// TODO: parse numeric escape codes
			}
		}
		else if(ch==_T('\"'))
		{
			tline++;
			line.append(1,_T('\"'));
			GetText(tmp,tline,_T('\"'));
			line.append(tmp);
			tline+=tmp.length();
			line.append(1,_T('\"'));
		}
		else
		if(ch==_T('['))
		{
			String tmp2(++tline);
			ParseLine(tmp,tmp2,_T(']'));
			line.append(tmp);
			src.assign(tmp2);
			tline=src.c_str();
		}
		else {
			line.append(1,ch);
		}
	}

	dest.clear();
	for(tline=line.c_str();*tline!=NULL;tline++)
	{
		_TCHAR ch=*tline;

		if(ch==delim)
		{
			src.assign(tline);
			return true;
		}

		if(ch==_T('\\'))
		{
			ch=*(++tline);
			switch(ch)
			{
			//case _T('a'): line.append(1,_T('\a')); break;
			//case _T('b'): line.append(1,_T('\b')); break;
			//case _T('f'): line.append(1,_T('\f')); break;
			//case _T('n'): line.append(1,_T('\n')); break;
			//case _T('r'): line.append(1,_T('\r')); break;
			//case _T('t'): line.append(1,_T('\t')); break;
			//case _T('v'): line.append(1,_T('\v')); break;
			case _T(' '): line+=_T("\\ "); break;
			case _T('\''): line.append(1,_T('\'')); break;
			case _T('"'): line.append(1,_T('"')); break;
			case _T('?'): line.append(1,_T('?')); break;
			case _T('$'): line.append(1,_T('$')); break;
			case _T('%'): line.append(1,_T('%')); break;
				// TODO: parse numeric escape codes
			}
		}
		else 
			if(ch==_T('\"'))
		{
			tline++;
			dest.append(1,_T('\"'));
			GetText(tmp,tline,_T('\"'));
			dest.append(tmp);
			tline+=tmp.length();
			dest.append(1,_T('\"'));
		}
		else
		if(ch==_T('\''))
		{
			tline++;
			GetText(tmp,tline,_T('\''));
			dest.append(tmp);
			tline+=tmp.length();
		}
		else
		if(ch==_T('%'))
		{
			tmp.clear();
			while(*tline!=NULL) 
			{
				tline++;
				ch=*tline;
				if(((ch>=_T('a'))&&(ch<=_T('z')))||
				   ((ch>=_T('A'))&&(ch<=_T('Z')))||
				    (ch==_T('_')))
				{
					tmp.append(1,ch);
				}
				else break;
			}
			tline--; //go back 1 char
			String tmp2;
			Environment::GetVariable(tmp2,tmp,_T(""));
			dest.append(tmp2);
		}
		else
		if(ch==_T('$'))
		{
			StringVector params;
			tmp.clear();
			while(*tline!=NULL) 
			{
				ch=*(++tline);
				if (((ch>=_T('a'))&&(ch<=_T('z')))||
					((ch>=_T('A'))&&(ch<=_T('Z')))||
					((ch>=_T('0'))&&(ch<=_T('9')))||
				    (ch==_T('+'))||
				    (ch==_T('-'))||
				    (ch==_T('_')))
				{
					tmp.append(1,ch);
				}
				else if(ch==_T('('))
				{
					String tmp2(++tline);
					String text;
					ParseLine(text,tmp2,_T(')'));
					line.assign(tmp2);
					tline=line.c_str();
					if(text.length()>0)
					{
						tmp.append(1,_T(','));
						tmp.append(text);
					}
					break;
				}
				else 
				{
					ch=*(--tline);
					break;
				}
			}
			Str::SplitLine(params,tmp,_T(','));

			String tmp2;

			Str::JoinVector(tmp2,params,0,params.size()-1,_T(' '));

			Exec::ExecCommand(params,tmp2,&tmp);
			dest.append(tmp);

			if(tline==NULL) break;
		}
		else {
			dest.append(1,ch);
		}
	}

	src.clear();

	if(delim!=0)
	{
		_tcout << "Syntax Error: Unexpected End of Line." << endl;
		return false;
	}
	return true;

}

bool ExecLine(String line)
{
	String tmp(line);
	String parsed;
	String command;
	bool result =ParseLine(parsed,tmp,0);
	if(!result) return false;

	command.assign(parsed);
	StringVector params;
	String::size_type argc=Str::SplitLine(params,parsed,' ');
	if(argc<=0) return false;

	return Exec::ExecCommand(params,command); 
}

void Prompt() 
{
	String PS;
	Environment::GetVariable(PS, _T("PROMPT"), _T(": "));

	String parsed;
	bool result =ParseLine(parsed,PS,0);

	_tcout << parsed;
}

int _tmain(int argc, _TCHAR* argv[])
{
	_TCHAR Char;
	String Line;

	Running=true;
	ExitCode=0;
	
	String PS;
	Environment::GetVariable(PS, _T("PROMPT"), _T("$pwd()$_(): "));
	Environment::SetVariable(_T("PROMPT"), PS);

	_tcout << "MXSS v0.2 by gigaherz" << endl << endl;

	Prompt();

	Line.clear();
	while(!_tcin.eof())
	{
		_tcin.get(Char);
		
		if((Char==_T('\n'))||_tcin.eof())
		{
			bool result=ExecLine(Line);
			if(!Running) 
			{
				if(!result) return 1;
				return ExitCode;
			}
			
			if(_tcin.eof())
				return ExitCode;
			else
				Line.clear();

			Prompt();
		}
		else Line.append(1,Char);
	}

	return ExitCode;
}

