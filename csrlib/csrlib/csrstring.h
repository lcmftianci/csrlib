#ifndef _CSR_STRING_H_
#define _CSR_STRING_H_

#include "comheader.h"

class csrstring {
public:
	csrstring(void);
	csrstring(char* cstr);
	csrstring(const csrstring& cstr);

	//override operator =
	const csrstring& operator = (const csrstring & cstr);
	//override operator +
	csrstring& operator + (const csrstring & cstr);
	//override operator ==
	bool operator == (const csrstring& cstr) const;
	//override operator >
	bool operator > (const csrstring& cstr) const;
	//override operator <
	bool operator < (const csrstring& cstr) const;

	//function
	csrstring& format(const char* szformat, ...);
	const char* c_str();

private:
	char* m_str;
};



#endif // _CSR_STRING_H_
