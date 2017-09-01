#include <exception>
//#include "UnitTest/UnitTest.h"
#include "TestTCPSock.h"
#include "TestUDPSock.h"


using namespace std;

int main()
{
    UnitTest unitTest;
    int ret = 0;


    try
    {
        unitTest.addTestClass(new TestTCPSock());
        unitTest.addTestClass(new TestUDPSock());
    }
    catch(const exception &e)
    {
        unitTest.displayError(e.what());
        ret = -1;
    }

    if(ret!=-1)
        if(!unitTest.run()) ret = 1;

	return ret;
}
