#include "CppUnitTest.h"
#include <SDKDDKVer.h>

#include "FileCoverageV2.h"
#include "MergeRunnerV2.h"
#include "RuntimeOptions.h"

#ifndef NOMINMAX
#	define NOMINMAX
#	include <Windows.h>
#endif

#pragma warning(disable: 4091)
#include <DbgHelp.h>
#pragma warning(default: 4091)

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<FileCoverageV2::LineArray>(const class FileCoverageV2::LineArray& t) { return L"FileCoverageV2::LineArray"; }
		}
	}
}

namespace TestMergeRunner
{
	TEST_CLASS(TestMergeRunnerV2)
	{
	public:

		void MergeTest(const std::string dirName, const std::string& inputXML)
		{
			const auto max = FileCoverageV2::maskCount;
			const auto c   = FileCoverageV2::maskIsCode;
			const auto p   = FileCoverageV2::maskIsPartial;

			FileCoverageV2::LineArray reference = { 0, 0, c, c | 5, c | p | 2, c | max, c | 4000, 0, c | 6 };

			// Create runner
			std::stringstream stream(inputXML.c_str());
			MergeRunnerV2 runner;
			auto dict = runner.createDictionary("reportFilename", stream);
			const size_t EXPECT_DICT_SIZE = 1;
			Assert::AreEqual(EXPECT_DICT_SIZE, dict.size());

			// Check dictionary
			const auto& saved = dict[dirName]["demo"];
			Assert::AreEqual(reference.size(), saved._code.size());
			Assert::AreEqual(reference, saved._code);
			Assert::AreEqual((size_t)9, saved._nbLinesFile);
			Assert::AreEqual((size_t)6, saved._nbLinesCode);
			Assert::AreEqual((size_t)5, saved._nbLinesCovered);
		}

		TEST_METHOD(MergeWithoutDirectory)
		{
			const std::string inputXML =
				R"(<?xml version="1.0" encoding="utf-8"?>)""\n" \
				R"(<CppCoverage version="2.0">)""\n" \
				R"(		<file path="demo" md5="0123456789ABCDEFGHIJKLMNOPQRSTUV">)""\n" \
				R"(			<stats nbLinesInFile="9" nbLinesOfCode="6" nbLinesCovered="5"/>)""\n" \
				R"(			<coverage>AAAAAACABYACwP+/oI8AAAaA</coverage>)""\n" \
				R"(		</file>)""\n" \
				R"(</CppCoverage>)";

			MergeTest("", inputXML);
		}

		TEST_METHOD(MergeWithDirectory)
		{
			const std::string inputXML =
				R"(<?xml version="1.0" encoding="utf-8"?>)""\n" \
				R"(<CppCoverage version="2.0">)""\n" \
				R"(	<directory path="directoryName">)""\n" \
				R"(		<file path="demo" md5="0123456789ABCDEFGHIJKLMNOPQRSTUV">)""\n" \
				R"(			<stats nbLinesInFile="9" nbLinesOfCode="6" nbLinesCovered="5"/>)""\n" \
				R"(			<coverage>AAAAAACABYACwP+/oI8AAAaA</coverage>)""\n" \
				R"(		</file>)""\n" \
				R"(	</directory>)""\n" \
				R"(</CppCoverage>)";

			MergeTest("directoryName", inputXML);
		}
	};
}