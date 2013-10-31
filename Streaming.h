#pragma once

class TextReader
{
public:
	virtual bool ReadLine(String &dest);
	virtual bool ReadToEnd(String &dest);
	virtual bool ReadDelimited(String &dest, String delimiters, bool allowEOF = false)=0;
	virtual bool ReadAvailableBytes(String &dest)=0;
	virtual int GetAvailableBytes()=0;
};

class Win32PipeReader : public TextReader
{
	HANDLE hReadEnd;

public:
	Win32PipeReader(HANDLE hFrom)
	{
		hReadEnd = hFrom;
	}

	virtual bool ReadDelimited(String &dest, String delimiters, bool allowEOF = false);
	virtual bool ReadAvailableBytes(String &dest);
	virtual int GetAvailableBytes();
};
