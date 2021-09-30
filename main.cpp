#define _DEBUG
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif
// test for memory leaks 
#include <iostream>
using namespace std; 

int main()
{

    #ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
//test for memory leaks 

    cout << "This is a test to set up the GitHub for the project." << endl; 
    
    return 0;
}
