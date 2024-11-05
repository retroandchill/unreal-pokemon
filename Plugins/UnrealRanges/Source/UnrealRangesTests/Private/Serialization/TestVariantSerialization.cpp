#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Ranges/Serialization/DataSerialization.h"

BEGIN_DEFINE_SPEC(FTestVariantSerialization, "Unit Tests.Serialization.TestVariantSerialization",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
END_DEFINE_SPEC(FTestVariantSerialization);

void FTestVariantSerialization::Define() {
    Describe("Test Variant Serialization", [this] {
        It("Can serialize and deserialize a variant object with the same result", [this] {
            TArray<uint8> Bytes;
            std::variant<int32, FString> Variant1 = 43;
            FMemoryWriter Writer(Bytes, true);
            Writer.SetIsLoading(false);
            Writer.SetIsSaving(true);
            Writer << Variant1;

            std::variant<int32, FString> Variant2;
            FMemoryReader Reader(Bytes, true);
            Reader.SetIsLoading(true);
            Reader.SetIsSaving(false);
            Reader << Variant2;
            UE_ASSERT_TRUE(Variant1 == Variant2);

            Bytes.Empty();
            Variant1 = FString(TEXT("Test string"));
            Writer << Variant1;
            Reader << Variant2;
            UE_ASSERT_TRUE(Variant1 == Variant2);
            
            return true;
        });
    });
}