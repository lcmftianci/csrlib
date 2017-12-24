#ifndef _CSR_COPY_H_
#define _CSR_COPY_H_

#include "comheader.h"




//void CopyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists);
bool CopyFileToDest(std::string lpSrc, std::string lpDes, bool bIsExit = true, bool bmkDir = false);

#endif
