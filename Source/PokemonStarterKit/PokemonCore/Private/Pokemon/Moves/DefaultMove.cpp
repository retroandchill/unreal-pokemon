// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/Moves/DefaultMove.h"
#include "DataManager.h"
#include "Moves/MoveData.h"
#include "Moves/Target.h"

TScriptInterface<IMove> UDefaultMove::Initialize(const FMoveDTO &MoveID)
{
    ID = MoveID.Move;
    auto TotalPP = GetMoveData().TotalPP;
    CurrentPP = FMath::Clamp(MoveID.CurrentPP.Get(TotalPP), 0, TotalPP);
    return this;
}

FMoveDTO UDefaultMove::ToDTO() const
{
    return {.Move = ID, .CurrentPP = CurrentPP};
}

const FMoveData &UDefaultMove::GetMoveData() const
{
    auto Move = FDataManager::GetInstance().GetDataTable<FMoveData>().GetData(ID);
    check(Move != nullptr)
    return *Move;
}

FText UDefaultMove::GetDisplayName() const
{
    return GetMoveData().RealName;
}

FText UDefaultMove::GetMoveDescription() const
{
    return GetMoveData().Description;
}

FName UDefaultMove::GetType() const
{
    return GetMoveData().Type;
}

EMoveDamageCategory UDefaultMove::GetDamageCategory() const
{
    return GetMoveData().Category;
}

int32 UDefaultMove::GetBasePower() const
{
    return GetMoveData().Power;
}

bool UDefaultMove::IsVariablePower() const
{
    return GetBasePower() == FMoveData::VariablePower;
}

int32 UDefaultMove::GetAccuracy() const
{
    return GetMoveData().Accuracy;
}

bool UDefaultMove::AlwaysHits() const
{
    return GetAccuracy() == FMoveData::AlwaysHitsAccuracy;
}

int32 UDefaultMove::GetCurrentPP() const
{
    return CurrentPP;
}

int32 UDefaultMove::GetTotalPP() const
{
    // TODO: Add support for PP ups
    return GetMoveData().TotalPP;
}

FName UDefaultMove::GetFunctionCode() const
{
    return GetMoveData().FunctionCode;
}

const FMoveTarget &UDefaultMove::GetTargetType() const
{
    static const auto &TargetTable = FDataManager::GetInstance().GetDataTable<FMoveTarget>();
    auto Target = TargetTable.GetData(GetMoveData().Target);
    check(Target != nullptr)
    return *Target;
}

int32 UDefaultMove::GetAdditionalEffectChance() const
{
    return GetMoveData().EffectChance;
}

const TArray<FName> &UDefaultMove::GetTags() const
{
    return GetMoveData().Tags;
}

void UDefaultMove::RecoverPP(int32 Amount)
{
    CurrentPP = FMath::Clamp(CurrentPP + Amount, 0, GetTotalPP());
}

void UDefaultMove::DecrementPP(int32 Amount)
{
    CurrentPP = FMath::Clamp(CurrentPP - Amount, 0, GetTotalPP());
}
