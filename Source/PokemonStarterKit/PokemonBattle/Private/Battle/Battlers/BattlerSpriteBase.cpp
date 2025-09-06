// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Battlers/BattlerSpriteBase.h"
#include "Graphics/BattleRender.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "PokemonBattleModule.h"
#include "Simple2D/Assets/SimpleFlipbook.h"
#include "Simple2D/Components/SimpleFlipbookComponent.h"
#include "Simple2D/Proxies/FlipbookProxy.h"

namespace Pokemon::Battle
{
    static const FName RenderComponentName = "RenderComponent";

    template <typename T>
        requires Retro::HasType<Retro::TVariantObjectTraits<FBattleRender>::Types, T>
    struct TRenderComponentOps
    {
        using URenderComponentType = std::nullptr_t;
    };

    template <Simple2D::Flipbook T>
    struct TFlipbookTypeData;

    template <>
    struct TFlipbookTypeData<UPaperFlipbook>
    {
        using Type = UPaperFlipbookComponent;
    };

    template <>
    struct TFlipbookTypeData<USimpleFlipbook>
    {
        using Type = USimpleFlipbookComponent;
    };

    template <Simple2D::Flipbook T>
    using TFlipbookComponentType = typename TFlipbookTypeData<T>::Type;

    template <Simple2D::Flipbook T>
    struct TRenderComponentOps<T>
    {
        using URenderComponentType = TFlipbookComponentType<T>;

        static void SetRender(URenderComponentType &Component, T *Flipbook)
        {
            Component.SetFlipbook(Flipbook);

            if (auto DefaultMaterial = Flipbook->GetDefaultMaterial(); DefaultMaterial != nullptr)
            {
                Component.SetMaterial(0, DefaultMaterial);
            }
        }
    };

    template <typename T>
        requires Retro::HasType<Retro::TVariantObjectTraits<FBattleRender>::Types, T>
    void CreateRenderComponent(ABattlerSpriteBase &Owner, TObjectPtr<USceneComponent> &Existing, T *Asset)
    {
        using URenderComponentType = typename TRenderComponentOps<T>::URenderComponentType;
        if constexpr (std::same_as<URenderComponentType, nullptr_t>)
        {
            UE_LOG(LogBattle, Warning, TEXT("Render component creation for type %s not get implemented"),
                   *T::StaticClass()->GetName());
            if (Existing != nullptr)
            {
                Owner.RemoveOwnedComponent(Existing);
                Existing = nullptr;
            }
        }
        else
        {
            auto AsRenderComponent = Cast<URenderComponentType>(Existing);
            if (AsRenderComponent == nullptr)
            {
                if (Existing != nullptr)
                {
                    Owner.RemoveOwnedComponent(Existing);
                }
                auto CreatedComponent =
                    Owner.AddComponentByClass(URenderComponentType::StaticClass(), false,
                                              Owner.GetBattleSpriteTransform(T::StaticClass()), false);
                ;
                AsRenderComponent = static_cast<URenderComponentType *>(CreatedComponent);
                Existing = AsRenderComponent;
            }

            TRenderComponentOps<T>::SetRender(*AsRenderComponent, Asset);
        }
    }

} // namespace Pokemon::Battle

void ABattlerSpriteBase::SetBattleSprite_Implementation(const FBattleRender &Render)
{
    IBattlerSprite::SetBattleSprite_Implementation(Render);
    if (Render == nullptr)
    {
        if (RenderComponent != nullptr)
        {
            RemoveOwnedComponent(RenderComponent);
            RenderComponent = nullptr;
        }

        return;
    }

    Render.Visit([this](auto Asset) { Pokemon::Battle::CreateRenderComponent(*this, RenderComponent, Asset); });
}

FTransform ABattlerSpriteBase::GetBattleSpriteTransform_Implementation(UClass *AssetType)
{
    return FTransform::Identity;
}
