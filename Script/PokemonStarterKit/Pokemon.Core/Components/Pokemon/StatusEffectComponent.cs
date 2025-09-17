using LanguageExt;
using LanguageExt.UnsafeValueAccess;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Pokemon;

[UClass]
public class UStatusEffectComponent : URPGComponent
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
}