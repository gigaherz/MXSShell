#include "ShellApi.h"

bool PathFileHasExtension(const String &path)
{
	String::size_type a,b,c;

	a=path.find(_T('/'));
	b=path.find(_T('\\'));
	c=path.find(_T('.'));

	if(c==String::npos) return false;
	if(c<b) return false;
	if(c<a) return false;

	return true;
}
