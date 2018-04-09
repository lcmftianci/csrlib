#include <stdio.h>

//#define TF_LIB_GTL_ALIGN_ATTRIBUTE
//#define TF_LIB_GTL_ALIGNED_CHAR_ARRAY

//#include "tensorflow\c\c_api.h"
#include "tensorflow/core/public/session.h"  
#include "tensorflow/core/platform/env.h"

#include <iostream>

using namespace std;
using namespace tensorflow;

int main()
{
	//printf("Hello from TensorFlow C library version %s\n", TF_Version());
	Session* session;
	Status status = NewSession(SessionOptions(), &session);
	if (!status.ok()) {
		cout << status.ToString() << "\n";
		return 1;
	}
	cout << "Session successfully created.\n";
	system("pause");
	return 0;
}