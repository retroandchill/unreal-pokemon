#include "TileSetTagRequirements.h"

bool FTileSetTagRequirements::RequirementsMet(const FGameplayTagContainer& Container) const
{
	bool HasRequired = Container.HasAll(RequireTags);
	bool HasIgnored = Container.HasAny(IgnoreTags);

	return HasRequired && !HasIgnored;
}

bool FTileSetTagRequirements::IsEmpty() const
{
	return (RequireTags.Num() == 0 && IgnoreTags.Num() == 0);
}

FString FTileSetTagRequirements::ToString() const
{
	FString Str;

	if (RequireTags.Num() > 0)
	{
		Str += FString::Printf(TEXT("require: %s "), *RequireTags.ToStringSimple());
	}
	if (IgnoreTags.Num() > 0)
	{
		Str += FString::Printf(TEXT("ignore: %s "), *IgnoreTags.ToStringSimple());
	}

	return Str;
}
