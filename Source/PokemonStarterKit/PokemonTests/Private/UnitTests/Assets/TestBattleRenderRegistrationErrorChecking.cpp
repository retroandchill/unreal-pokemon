#include "Asserts.h"
#include "AutomationTestModule.h"
#include "Graphics/BattleRender.h"
#include "Misc/AutomationTest.h"
#include "RetroLib/Exceptions/TypeException.h"
#include "RetroLib/Exceptions/VariantException.h"
#include "RetroLib/Variants/VariantObjectStruct.h"
#include "Utilities/ContextObject.h"
#include "UtilityClasses/Helpers/ExceptionTestHelper.h"

BEGIN_DEFINE_SPEC(FTestRegistrationErrorChecking, "Unit Tests.Assets.BattleRender",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

TObjectPtr<UExceptionTestHelper> Helper;
TObjectPtr<UScriptStruct> StructType;
TObjectPtr<UScriptStruct> SoftStructType;
TOptional<Retro::IVariantRegistration &> Registration;

END_DEFINE_SPEC(FTestRegistrationErrorChecking);

void FTestRegistrationErrorChecking::Define()
{
    Describe("Test Battle Render Blueprint System Error checking", [this] {
        BeforeEach([this] {
            Helper = NewObject<UExceptionTestHelper>();
            auto &Registry = Retro::FVariantObjectStructRegistry::Get();
            StructType = Retro::GetScriptStruct<FBattleRender>();
            SoftStructType = Retro::GetScriptStruct<FSoftBattleRender>();
            Registration = Registry.GetVariantStructData(*StructType);
        });

        AfterEach([this] { Registration.Reset(); });

        It("Test can't set to invalid property type", [this] {
            auto Property =
                Helper->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UExceptionTestHelper, Brush));
            auto StructProperty = CastField<FStructProperty>(Property);
            UE_ASSERT_NOT_NULL(StructProperty);

            auto Sprite = NewObject<UContextObject>();
            UE_CHECK_THROWS(
                Retro::FTypeException,
                Registration->SetStructValue(Sprite, *StructProperty, reinterpret_cast<uint8 *>(&Helper->Brush)))
            UE_CHECK_THROWS(
                Retro::FTypeException,
                Registration->MakeSoftValue(Sprite, *StructProperty, reinterpret_cast<uint8 *>(&Helper->Brush)))
            return true;
        });

        It("Test can't set to invalid object on variant type", [this] {
            auto Property =
                Helper->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UExceptionTestHelper, Render));
            auto StructProperty = CastField<FStructProperty>(Property);
            UE_ASSERT_NOT_NULL(StructProperty);

            auto Sprite = NewObject<UContextObject>();
            UE_CHECK_THROWS(
                Retro::FVariantException,
                Registration->SetStructValue(Sprite, *StructProperty, reinterpret_cast<uint8 *>(&Helper->Render)))
            return true;
        });

        It("Test can't make soft value from invalid struct", [this] {
            auto InputProperty =
                Helper->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UExceptionTestHelper, Brush));
            auto InputStructProperty = CastField<FStructProperty>(InputProperty);
            UE_ASSERT_NOT_NULL(InputStructProperty);

            auto OutputProperty =
                Helper->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UExceptionTestHelper, Render));
            auto OutputStructProperty = CastField<FStructProperty>(OutputProperty);
            UE_ASSERT_NOT_NULL(OutputStructProperty);

            UE_CHECK_THROWS(Retro::FTypeException,
                            Registration->MakeSoftValue(*InputStructProperty, reinterpret_cast<uint8 *>(&Helper->Brush),
                                                        *OutputStructProperty,
                                                        reinterpret_cast<uint8 *>(&Helper->Render)))
            return true;
        });

        It("Test can't get reference from soft struct", [this] {
            auto Property =
                Helper->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UExceptionTestHelper, Brush));
            auto StructProperty = CastField<FStructProperty>(Property);
            UE_ASSERT_NOT_NULL(StructProperty);

            UE_CHECK_THROWS(Retro::FTypeException,
                            Registration->TryGetSoftValue(UPaperSprite::StaticClass(), *StructProperty,
                                                          reinterpret_cast<uint8 *>(&Helper->Brush)))
            return true;
        });

        It("Test can't load from from invalid struct", [this] {
            auto InputProperty =
                Helper->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UExceptionTestHelper, Brush));
            auto InputStructProperty = CastField<FStructProperty>(InputProperty);
            UE_ASSERT_NOT_NULL(InputStructProperty);

            auto OutputProperty =
                Helper->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UExceptionTestHelper, Render));
            auto OutputStructProperty = CastField<FStructProperty>(OutputProperty);
            UE_ASSERT_NOT_NULL(OutputStructProperty);

            UE_CHECK_THROWS(
                Retro::FTypeException,
                Registration->LoadSynchronous(*InputStructProperty, reinterpret_cast<uint8 *>(&Helper->Brush),
                                              *OutputStructProperty, reinterpret_cast<uint8 *>(&Helper->Render)))
            return true;
        });
    });
}