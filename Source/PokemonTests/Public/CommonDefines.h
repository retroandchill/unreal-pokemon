#pragma once

#define TEST_ASSERT(Expression) if (!(Expression)) { FinishTest(EFunctionalTestResult::Failed, TEXT("Test failed!")); return; }
