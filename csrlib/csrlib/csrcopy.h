#ifndef _CSR_COPY_H_
#define _CSR_COPY_H_

#include <iostream>
#include <windows.h>
#include <atlstr.h>
#include <strsafe.h>


//void CopyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists);
bool CopyFileToDest(std::string lpSrc, std::string lpDes, bool bIsExit);

#endif
