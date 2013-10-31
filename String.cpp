#include "ShellApi.h"

int Str::Replace(String &dest, String src, _TCHAR from, _TCHAR to)
{
	int nr=0;
	dest.clear();
	for(String::const_iterator it=src.begin(); it!=src.end(); it++)
	{
		if(*it==from)
		{	dest.append(1,to); nr++; }
		else
			dest.append(1,*it);
	}
	return nr;
}

void Str::JoinVector(String &dest, StringVector data, size_t first, size_t last, int delim)
{
	dest.clear();
	if(last<first) return;

	if(delim == -1)
	{
		for(size_t i=first;i<=last;i++)
		{
			dest.append(data[i]);
		}
	}
	else
	{
		for(size_t i=first;i<last;i++)
		{
			dest.append(data[i]);
			dest.append(1,delim);
		}
		dest.append(data[last]);
	}
}

String::size_type Str::SplitLine(StringVector &dest, String data, _TCHAR delim, bool keepQuotes)
{
	String elm(_T(""));
	String::size_type start=0;

	dest.clear();

	int InAString=0;

	elm.clear();
	for(String::const_iterator dit=data.begin();dit!=data.end();dit++) 
	{
		_TCHAR ch=*dit;

		if(InAString==2) 
		{
			if(ch==_T('\''))
			{
				if(keepQuotes) elm.append(1,ch);
				InAString=false;
			}
			else elm.append(1,ch);
		}
		if(InAString==1) 
		{
			if(ch==_T('"'))
			{
				if(keepQuotes) elm.append(1,ch);
				InAString=false;
			}
			else elm.append(1,ch);
		}
		else
		{
			if(ch==_T('"'))
			{
				if(keepQuotes) elm.append(1,ch);
				InAString=1;
			}
			else if(ch==_T('\''))
			{
				if(keepQuotes) elm.append(1,ch);
				InAString=2;
			}
			else if(ch==delim)
			{
				dest.push_back(elm);
				elm.clear();
			}
			else elm.append(1,ch);
		}
	}
	dest.push_back(elm);
	return dest.size();
}
