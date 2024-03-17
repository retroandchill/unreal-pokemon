// "Unreal Pokémon" created by Retro & Chill.
#include "CommonDefines.h"
#include "Memory/GCPointer.h"
#include "Misc/AutomationTest.h"
#include "Charset/CharsetFactory.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestCharsetFactory, "Project.GridBased2D.TestCharsetFactory",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestCharsetFactory::RunTest(const FString& Parameters) {

	TGCPointer Factory(NewObject<UCharsetFactory>());

	AUTOMATION_ASSERT(TestTrue(TEXT("Show in menu"), Factory->ShouldShowInNewMenu()));

	TGCPointer NewCharset(Factory->FactoryCreateNew(UCharset::StaticClass(), Factory.Get(), FName(), RF_NoFlags, Factory.Get(), nullptr));
	AUTOMATION_ASSERT(TestTrue(TEXT("Show in menu"), NewCharset != nullptr));
	
	return true;
}
