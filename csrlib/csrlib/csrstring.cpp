#include "csrstring.h"


csrstring::csrstring(void)
{
	m_str = new char[1];
	m_str[0] = '\0';
}

csrstring::csrstring(char* cstr) :m_str(cstr)
{
	int nLen = strlen(cstr);
	m_str = new char[nLen + 1];
	strcpy(m_str, cstr);
	m_str[nLen] = '\0';
}

csrstring::csrstring(const csrstring& cstr)
{

}

