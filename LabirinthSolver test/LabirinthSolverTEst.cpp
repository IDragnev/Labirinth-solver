#include "CppUnitTest.h"
#include "LabirinthSolver.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using IDragnev::LabirinthSolver;

namespace LabirinthSolvertest
{		
	TEST_CLASS(LabirinthSolverTest)
	{
	private:
		using Lab = std::vector<std::string>;
		using Solver = LabirinthSolver;
		using Container = Solver::Result;

	public:	
		TEST_METHOD(noPath)
		{
			auto lab = Lab{ "*s*",
                            "**e" };

			auto result = Solver{}(std::begin(lab), std::end(lab));

			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(trivialPath)
		{
			auto lab = Lab{ "s=",
                            "*e" };

			auto result = Solver{}(std::begin(lab), std::end(lab));

			Assert::IsTrue(containsExactly(result, { "ES" }));
		}

		TEST_METHOD(splitWay)
		{
			auto lab = Lab{ "*s*",
                            "*=*",
			                "===",
			                "=*=",
                            "=e=" };

			auto result = Solver{}(std::begin(lab), std::end(lab));

			Assert::IsTrue(containsExactly(result, { "SSESSW", "SSWSSE" }));
		}

		TEST_METHOD(complex)
		{
			auto lab = Lab{ "s==",
							"*==",
							"*e=" };

			auto result = Solver{}(std::begin(lab), std::end(lab));

			Assert::IsTrue(containsExactly(result, { "ESS", "ESESW", "EESSW", "EESWS" }));
		}

	private:
		static bool containsExactly(const Container& result, const Container& paths)
		{
			auto isInResult = [&result](const auto& p) { return isMemberOf(result, p); };

			return result.size() == paths.size() &&
				   std::all_of(std::begin(paths), std::end(paths), isInResult);
		}

		static bool isMemberOf(const Container& container, const std::string& p)
		{
			auto it = std::find_if(std::begin(container), std::end(container), [&p](const auto& rhs) { return p == rhs; });
			return it != std::end(container);
		}
	};
}