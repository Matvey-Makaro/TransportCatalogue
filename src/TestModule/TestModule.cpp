#include "test_runner.h"
#include "TestModule.h"
#include "SvgTests.h"
#include "ComplexTests.h"

TestModule::TestModule() {}

void TestModule::Run()
{
    TestRunner tr;
    RunDocumentTests(tr);
    RunComplexTests(tr);
}
