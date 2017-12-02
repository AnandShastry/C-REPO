//============================================================================
// Name        : MyFirstProject.cpp
// Author      : Anand
// Version     :
// Copyright   : Your copyright notice
// Description : Namespace
//============================================================================

#include <iostream>
#include <unistd.h>
using namespace std;

namespace
{
   int k;
}

namespace z
{
   int k;
}

int main()
{
	static int j=5;
	while(j>0)
	{
		cout << "From A" <<z::k<< endl;
		j--;
	}

	return 0;
}