#include "ParticleTest.h"

#ifdef _WIN64
#include "CppUnitTest.h"
#endif


void RunTests()
{
	// ParticleTest.cpp
	TestParticleCopyConstructor();
	TestParticleDefaultConstructor();
}



#ifdef _WIN64
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestSpace
{
	TEST_CLASS(TestClass)
	{
	public:

		TEST_METHOD(TestMethod)
		{
			RunTests();
		}

	};
}
#endif
