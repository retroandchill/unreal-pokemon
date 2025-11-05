
#include "Asserts.h"
#include "Bag/Item.h"
#include "DataRetrieval/DataStructHandle.h"
#include "Misc/AutomationTest.h"
#include "Species/SpeciesData.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDataStructHandle, "Unit Tests.Data.TestDataStructHandle",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDataStructHandle::RunTest(const FString &Parameters) {
    Pokemon::Data::FStructWrapper WrappedStruct(FSpeciesHandle::StaticStruct());

    UE_ASSERT_TRUE(WrappedStruct.GetStruct() == FSpeciesHandle::StaticStruct());
    WrappedStruct.SetRowID(TEXT("BULBASAUR"));
    UE_CHECK_EQUAL(TEXT("BULBASAUR"), WrappedStruct.GetRowID().ToString());
    auto AsHandle = WrappedStruct.Get<FSpeciesHandle>();
    UE_CHECK_EQUAL(TEXT("BULBASAUR"), AsHandle->RowID.ToString());

    WrappedStruct = FItemHandle::StaticStruct();
    UE_ASSERT_TRUE(WrappedStruct.GetStruct() == FItemHandle::StaticStruct());

    return true;
}