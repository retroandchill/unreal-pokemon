using Pokemon.Data.Model.HardCoded;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Pokemon;

/// <summary>
/// Represents a component that manages the status effects of a Pokémon within the game.
/// This component allows for tracking and manipulation of a Pokémon's status effect,
/// as well as providing functionality to heal and remove status conditions.
/// </summary>
[UClass]
public partial class UStatusEffectComponent : URPGComponent, IHealableComponent
{
    /// <summary>
    /// Represents the current status effect applied to an entity, such as a Pokémon.
    /// This property allows for retrieving and assigning the status effect.
    /// </summary>
    /// <remarks>
    /// The status effect is represented using an <see cref="TOptional{T}"/> of type <c>FStatusHandle</c>.
    /// A value may be set when the status effect is active, or cleared by setting it to None.
    /// If the assigned status effect is invalid, the property will not update.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "StatusEffect", BlueprintAccessors = true)]
    public TOptional<FStatusHandle> StatusEffect
    {
        get => StatusEffect_BackingField;
        set
        {
            if (value.HasValue && !value.Value.IsValid)
                return;

            StatusEffect_BackingField = value;
        }
    }

    /// <inheritdoc />
    [ExcludeFromExtensions]
    public void Heal()
    {
        StatusEffect = TOptional<FStatusHandle>.None;
    }
}
