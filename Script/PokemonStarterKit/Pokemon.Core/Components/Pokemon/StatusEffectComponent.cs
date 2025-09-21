using LanguageExt;
using LanguageExt.UnsafeValueAccess;
using Pokemon.Data.Model.HardCoded;
using RPG.SourceGenerator.Attributes;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Pokemon;

[UClass]
public class UStatusEffectComponent : URPGComponent, IHealableComponent
{
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "StatusEffect")]
    public Option<FStatusHandle> StatusEffect
    {
        get;
        set
        {
            if (value.IsSome && !value.ValueUnsafe().IsValid)
                return;

            field = value;
        }
    }

    [ExcludeFromExtensions]
    public void Heal()
    {
        StatusEffect = Option<FStatusHandle>.None;
    }
}
