using LanguageExt;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;

namespace Pokemon.Core.Model.Pokemon;

/// <summary>
/// Represents information about a status effect component in the context of a Pokémon.
/// This structure is used to track optional status effects linked to a Pokémon entity.
/// A status effect refers to a specific condition or effect applied to a Pokémon during gameplay,
/// such as "Poisoned" or "Paralyzed", and is represented by an optional `StatusEffect` property.
/// </summary>
[UStruct]
public readonly partial record struct FStatusEffectComponentInfo
{
    /// <summary>
    /// Represents the status effect associated with a Pokémon.
    /// This property is optional and may contain information about specific conditions or effects,
    /// such as "Poisoned" or "Paralyzed", applied to the Pokémon during gameplay.
    /// </summary>
    [field: UProperty(PropertyFlags.SaveGame)]
    public required Option<FStatusHandle> StatusEffect { get; init; }
}
