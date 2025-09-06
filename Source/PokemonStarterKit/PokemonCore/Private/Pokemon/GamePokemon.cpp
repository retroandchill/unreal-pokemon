// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/GamePokemon.h"
#include "Algo/ForEach.h"
#include "Bag/Item.h"
#include "Battle/Status.h"
#include "DataManager.h"
#include "Lookup/InjectionUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/Abilities/AbilityBlock.h"
#include "Pokemon/Moves/Move.h"
#include "Pokemon/Moves/MoveBlock.h"
#include "Pokemon/PokemonDTO.h"
#include "Pokemon/Stats/DefaultStatBlock.h"
#include "Pokemon/TrainerMemo/ObtainedBlock.h"
#include "PokemonDataSettings.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Ranges/Algorithm/To.h"
#include "Species/GenderRatio.h"
#include "Species/Nature.h"
#include "Species/SpeciesData.h"
#include "Utilities/PersonalityValueUtils.h"

void UGamePokemon::Initialize(const FPokemonDTO &DTO, const TScriptInterface<ITrainer> &Trainer)
{
    Species = DTO.Species;
    PersonalityValue = UPersonalityValueUtils::GeneratePersonalityValue(DTO);
    Nickname = DTO.Nickname;
    Gender = DTO.Gender;
    Shiny = DTO.Shiny;
    StatBlock = UnrealInjector::NewInjectedDependency<IStatBlock>(this, this, DTO);
    StatBlock->CalculateStats(GetSpecies().BaseStats);
    CurrentHP = GetMaxHP();
    MoveBlock = UnrealInjector::NewInjectedDependency<IMoveBlock>(this, this, DTO);
    AbilityBlock = UnrealInjector::NewInjectedDependency<IAbilityBlock>(this, this, DTO);
    HoldItem = DTO.Item;
    StatusEffect = DTO.StatusEffect;

    if (DTO.PokeBall.IsSet())
    {
        PokeBall = *DTO.PokeBall;
    }
    else
    {
        PokeBall = GetDefault<UPokemonDataSettings>()->DefaultPokeBall;
    }

    if (Trainer != nullptr)
    {
        if (DTO.OwnerInfo.IsSet())
        {
            OwnerInfo = *DTO.OwnerInfo;
        }
        else
        {
            OwnerInfo = FOwnerInfo(*Trainer);
        }
        CurrentHandler = Trainer;
        Rename(nullptr, CurrentHandler.GetObject());
    }
    else
    {
        if (DTO.OwnerInfo.IsSet())
        {
            OwnerInfo = *DTO.OwnerInfo;
        }
        else
        {
            OwnerInfo = FOwnerInfo(this);
        }
        CurrentHandler = nullptr;
    }

    ObtainedBlock = UnrealInjector::NewInjectedDependency<IObtainedBlock>(this, DTO);
}

FPokemonDTO UGamePokemon::ToDTO() const
{
    TMap<FName, int32> IVs;
    TMap<FName, int32> EVs;
    StatBlock->ForEachStat([&IVs, &EVs](FName ID, const IStatEntry &Stat) {
        IVs.Emplace(ID, Stat.GetIV());
        EVs.Emplace(ID, Stat.GetEV());
    });
    return {.Species = Species,
            .Level = StatBlock->GetLevel(),
            .PersonalityValue = PersonalityValue,
            .Nickname = Nickname,
            .Gender = Gender,
            .Shiny = Shiny,
            .PokeBall = PokeBall,
            .CurrentHP = CurrentHP,
            .Exp = StatBlock->GetExp(),
            .IVs = std::move(IVs),
            .EVs = std::move(EVs),
            .Nature = StatBlock->GetNature().ID,
            .Ability = AbilityBlock->GetAbilityID(),
            .Item = HoldItem,
            // clang-format off
            .Moves = MoveBlock->GetMoves() |
                     Retro::Ranges::Views::Transform(&IMove::ToDTO) |
                     Retro::Ranges::To<TArray>(),
            // clang-format on
            .MoveMemory = MoveBlock->GetMoveMemory(),
            .StatusEffect = StatusEffect,
            .ObtainMethod = ObtainedBlock->GetObtainMethod(),
            .LevelMet = ObtainedBlock->GetLevelMet(),
            .TimeReceived = ObtainedBlock->GetTimeReceived().GetValueOptional(),
            .MetLocation = ObtainedBlock->GetObtainText(),
            .TimeHatched = ObtainedBlock->GetTimeHatched().GetValueOptional(),
            .HatchedMap = ObtainedBlock->GetHatchedMap(),
            .OwnerInfo = OwnerInfo};
}

FText UGamePokemon::GetNickname() const
{
    return Nickname.IsSet() ? Nickname.GetValue() : GetSpecies().RealName;
}

EPokemonGender UGamePokemon::GetGender() const
{
    using enum EPokemonGender;

    if (Gender.IsSet())
        return Gender.GetValue();

    auto &GenderRatio = GetSpecies().GetGenderRatio();
    if (GenderRatio.IsGenderless)
        return Genderless;

    return (PersonalityValue & UPersonalityValueUtils::LOWER_8_BITS) < GenderRatio.FemaleChance ? Female : Male;
}

const TArray<FName> &UGamePokemon::GetTypes() const
{
    return GetSpecies().Types;
}

FName UGamePokemon::GetPokeBall() const
{
    return PokeBall;
}

bool UGamePokemon::IsShiny() const
{
    if (Shiny.IsSet())
    {
        return Shiny.GetValue();
    }

    uint32 A = PersonalityValue ^ static_cast<uint32>(OwnerInfo.ID);
    uint32 B = A & 0xFFFF;
    uint32 C = (A >> 16) & 0xFFFF;
    uint32 D = B ^ C;
    return D < static_cast<uint32>(GetDefault<UPokemonDataSettings>()->ShinyPokemonChance);
}

bool UGamePokemon::IsAble() const
{
    return CurrentHP > 0;
}

int32 UGamePokemon::GetCurrentHP() const
{
    return CurrentHP;
}

void UGamePokemon::SetCurrentHP(int32 Value)
{
    CurrentHP = FMath::Clamp(Value, 0, GetMaxHP());
}

int32 UGamePokemon::GetMaxHP() const
{
    return GetStatBlock()->GetStat(GetDefault<UPokemonDataSettings>()->HPStat)->GetStatValue();
}

int32 UGamePokemon::RestoreHP(int32 Amount)
{
    int32 Before = CurrentHP;
    SetCurrentHP(CurrentHP + Amount);
    return CurrentHP - Before;
}

void UGamePokemon::FullyHeal()
{
    SetCurrentHP(GetMaxHP());
    Algo::ForEach(MoveBlock->GetMoves(),
                  [](const TScriptInterface<IMove> &Move) { Move->RecoverPP(Move->GetTotalPP()); });
    StatusEffect.Reset();
}

bool UGamePokemon::IsFainted() const
{
    return CurrentHP <= 0;
}

const FSpeciesData &UGamePokemon::GetSpecies() const
{
    const auto &DataManager = FDataManager::GetInstance();
    auto &SpeciesTable = DataManager.GetDataTable<FSpeciesData>();

    auto SpeciesData = SpeciesTable.GetData(Species);
    check(SpeciesData != nullptr)
    return *SpeciesData;
}

uint32 UGamePokemon::GetPersonalityValue() const
{
    return PersonalityValue;
}

TScriptInterface<IStatBlock> UGamePokemon::GetStatBlock() const
{
    return StatBlock;
}

TScriptInterface<IMoveBlock> UGamePokemon::GetMoveBlock() const
{
    return MoveBlock;
}

TScriptInterface<IAbilityBlock> UGamePokemon::GetAbility() const
{
    return AbilityBlock;
}

TOptional<const FItem &> UGamePokemon::GetHoldItem() const
{
    if (!HoldItem.IsSet())
    {
        return nullptr;
    }

    auto ItemData = FDataManager::GetInstance().GetDataTable<FItem>().GetData(HoldItem.GetValue());
    check(ItemData != nullptr)
    return ItemData;
}

void UGamePokemon::SetHoldItem(FItemHandle Item)
{
    HoldItem.Emplace(Item);
}

void UGamePokemon::RemoveHoldItem()
{
    HoldItem.Reset();
}

TOptional<const FStatus &> UGamePokemon::GetStatusEffect() const
{
    static auto &StatusTable = FDataManager::GetInstance().GetDataTable<FStatus>();
    // clang-format off
    return StatusEffect |
           Retro::Optionals::Transform([](const FName &ID) {
               return StatusTable.GetData(ID);
           });
    // clang-format on
}

bool UGamePokemon::HasStatusEffect(const FStatusHandle &Handle) const
{
    return StatusEffect == Handle.RowID;
}

bool UGamePokemon::SetStatusEffect(const FStatusHandle &StatusID, bool bOverwriteExisting)
{
    if (StatusEffect.IsSet() && !bOverwriteExisting)
    {
        return false;
    }

    check(FDataManager::GetInstance().GetDataTable<FStatus>().IsRowNameValid(StatusID))
    StatusEffect.Emplace(StatusID);
    return true;
}

void UGamePokemon::RemoveStatusEffect()
{
    StatusEffect.Reset();
}

const FOwnerInfo &UGamePokemon::GetOwnerInfo() const
{
    return OwnerInfo;
}

TScriptInterface<IObtainedBlock> UGamePokemon::GetObtainedInformation() const
{
    return ObtainedBlock;
}

const TScriptInterface<ITrainer> &UGamePokemon::GetCurrentHandler() const
{
    return CurrentHandler;
}

void UGamePokemon::SetCurrentHandler(const TScriptInterface<ITrainer> &NewHandler)
{
    CurrentHandler = NewHandler;
    Rename(nullptr, CurrentHandler.GetObject());
}

UGamePokemon *UGamePokemon::Create(UObject *WorldContext, const FPokemonDTO &Data)
{
    return Create(WorldContext, Data, nullptr);
}

UGamePokemon *UGamePokemon::Create(UObject *WorldContext, const FPokemonDTO &Data,
                                   const TScriptInterface<ITrainer> &Trainer)
{
    auto Ret = NewObject<UGamePokemon>(WorldContext);
    Ret->Initialize(Data, Trainer);
    return Ret;
}