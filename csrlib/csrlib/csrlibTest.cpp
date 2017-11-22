#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "csrorder.h"
#include "interview.h"
#include "csrlist.h"


using namespace std;


int main()
{
	cout << "begin" << endl;
	float arrF[] = { 1.0, 1.4, 1.7, 2.7, 6.4, 5.5 };
	SortBubble(arrF, sizeof(arrF) / sizeof(float), false);
	DisplayData(arrF, sizeof(arrF) / sizeof(float));

	cout << AddNum(3, 120) << endl;

	cout << MaxNumByte(3, 6) << endl;
	cout << MaxNumByte(9, 4) << endl;
	cout << MaxNumByte(-13, 15) << endl;
	(cout << MaxNumByte(-10, -189)) << endl;

	printf("hello world\n");

	float a = 113.3f;
	float b = 115.8f;
	swap(a, b);
	cout << a << "  " << b << endl;

	int i;
	int array[10] = {1,2,3,4,5,6,7,8,9,10};
	ACCESS_BUFORE(array[5], 4, 6);
	for (i = 0; i < 10; ++i)
	{
		printf("%d\n", array[i]);
	}

	addressBin();

	cout << thranslateScale("10", 10, 2).c_str() << endl;

	cout << thranslateScale("0000000011111010", 2, 10).c_str() << endl;

	//list²âÊÔ´úÂë
	linuxlist tlist;
	listCreate(&tlist);

	for (int i = 0; i < 10; ++i)
		listHeadInsert(&tlist, i);

	for (int i = 35; i > 20; --i)
		listHeadInsert(&tlist, i);

	listDisplay(&tlist);

	listReverse(&tlist);

	listDisplay(&tlist);

	listBubbleSort(&tlist);

	listDisplay(&tlist);

	system("pause");
	return 0;
}