//#include<Windows.h>
//#include<stdio.h>
//#include<string.h>
//#include<stdlib.h>
//FILE* fp;
//void searchCatalog(char* root) {
//	char temp[100];
//	strcpy(temp, root);
//	strcat(temp, "\\\\*");
//	HANDLE hdnode;
//	WIN32_FIND_DATA wdfnode;
//	hdnode = FindFirstFile(temp, &wdfnode);
//	if (!FindNextFile(hdnode, &wdfnode)) {
//		FindClose(hdnode);
//		return;
//	}
//	while (FindNextFile(hdnode, &wdfnode)) {
//		char roottemp[100];
//		strcpy(roottemp, root);
//		strcat(roottemp, "\\\\");
//		strcat(roottemp, wdfnode.cFileName);
//		searchCatalog(roottemp);
//		printf("%s\n", wdfnode.cFileName);
//		fprintf(fp, "%s\n", wdfnode.cFileName);
//	}
//	FindClose(hdnode);
//}
//int main(int argc, TCHAR* argv[]) {
//	if (argc<2) {
//		return 0;
//	}
//	fp = fopen("C:\\filelist.txt", "w");
//	if (fp == NULL)
//		return 0;
//	searchCatalog(argv[1]);
//	fclose(fp);
//}