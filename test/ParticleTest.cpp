#include "CppUnitTest.h"
#include "Particle.cpp"
#include "stdlib.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{

	TEST_CLASS(ParticleTest)
	{
	public:

		TEST_METHOD(ParticleDefaultConstructor)
		{
			void* zero = calloc(1, sizeof(Particle));
			Particle a = Particle();
			Assert::AreEqual(memcmp(zero, &a, sizeof(Particle)), 0);
		};


		TEST_METHOD(ParticleCopyConstructor)
		{
			void* zero = calloc(1, sizeof(Particle));
			Particle a = Particle();
			a.pos = { 1.0, 2.0, 3.0 };
			a.vel = { 4.0, 5.0, 6.0 };
			a.acc = { 7.0, 8.0, 9.0 };
			a.density = 10.0;
			a.pressure = 11.0;
			Particle b = Particle(a);
			Assert::AreEqual(memcmp(&b, &a, sizeof(Particle)), 0);
			a.pressure = 12.0;
			Assert::AreNotEqual(memcmp(&b, &a, sizeof(Particle)), 0);
		};

	};
}
