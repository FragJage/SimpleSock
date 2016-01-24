#include<algorithm>
#include<string>
#include<vector>
#include<iomanip>
#include"termcolor.h"


#define TEST_METHOD(class_name, method_name) \
bool class_name##_##method_name(); \
static TestFunctionAdder class_name##_##method_name##Adder(#class_name, #method_name, &class_name##_##method_name);    \
bool class_name##_##method_name()

typedef bool(*UnitTestFunction)();

struct TestMethod
{
	std::string MethodName;
	UnitTestFunction TestFunction;

	TestMethod(std::string methodName, UnitTestFunction testFunction) : MethodName(methodName), TestFunction(testFunction)
	{
	}
};

struct TestClass
{
	std::string ClassName;
	std::vector<TestMethod> TestMethodList;

	TestClass(std::string className, std::vector<TestMethod> testMethodList) : ClassName(className), TestMethodList(testMethodList)
	{
	}
};

bool operator==(const TestClass& lhs, const std::string& rhs);

class UnitTest
{
	public:
		typedef bool(*TestFunction)();

		bool Run();

	private:
		friend class TestFunctionAdder;
		static std::vector<TestClass> m_TestClassList;
};

bool operator==(const TestClass& lhs, const std::string& rhs)
{
	return lhs.ClassName == rhs;
}

class TestFunctionAdder
{
	public:
		TestFunctionAdder(std::string className, std::string methodName, UnitTest::TestFunction func)
		{
			std::vector<TestClass>::iterator it;
			std::vector<TestClass>::iterator itEnd;


			itEnd = UnitTest::m_TestClassList.end();
			it = std::find(UnitTest::m_TestClassList.begin(), itEnd, className);
			if (it == itEnd)
			{
				std::vector<TestMethod> testMethodList = { { methodName, func } };
				UnitTest::m_TestClassList.emplace_back(className, testMethodList);
			}
			else
			{
				it->TestMethodList.emplace_back(methodName, func);
			}
		}
};


std::vector<TestClass> UnitTest::m_TestClassList;

bool UnitTest::Run()
{
	std::vector<TestClass>::iterator itClass;
	std::vector<TestClass>::iterator itClassEnd;
	std::vector<TestMethod>::iterator itMethod;
	std::vector<TestMethod>::iterator itMethodEnd;
	int padLenght = 0;
	int len;
  bool result = true;


	itClass = UnitTest::m_TestClassList.begin();
	itClassEnd = UnitTest::m_TestClassList.end();

	while (itClass != itClassEnd)
	{
		len = itClass->ClassName.length();
		if (len > padLenght) padLenght = len;
		while (itMethod != itMethodEnd)
		{
			len = itMethod->MethodName.length();
			if (len > padLenght) padLenght = len;
			++itMethod;
		}
		++itClass;
	}
	padLenght += 5;

	itClass = UnitTest::m_TestClassList.begin();
	itClassEnd = UnitTest::m_TestClassList.end();

	while (itClass != itClassEnd)
	{
		std::cout << termcolor::lightYellow << "- " << itClass->ClassName << " " << std::setfill('-') << std::setw(padLenght+10 - itClass->ClassName.length()) << "-" << std::endl;
		itMethod = itClass->TestMethodList.begin();
		itMethodEnd = itClass->TestMethodList.end();
		while (itMethod != itMethodEnd)
		{
			std::cout << "    " << termcolor::lightWhite << itMethod->MethodName << std::setfill(' ') << std::setw(padLenght - itMethod->MethodName.length()) << " ";
			try
			{
				if ((*(itMethod->TestFunction))())
					std::cout << termcolor::lightGreen << "OK";
				else
        {
          result = false;
          std::cout << termcolor::lightRed << "KO";
        }
			}
			catch(const std::exception & e)
			{
        result = false;
				std::cout << termcolor::lightRed << "FAILED" << std::endl;
				std::cout << termcolor::white << "EXCEPTION" << std::endl << e.what();
			}
			std::cout << std::endl;
			++itMethod;
		}
		std::cout << std::endl;
		++itClass;
	}
	std::cout << termcolor::white;
  if(!result) return -1;
  return 0;
}
