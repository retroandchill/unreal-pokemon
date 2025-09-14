using JetBrains.Annotations;
using Pokemon.Core.Entities;
using RPG.SourceGenerator.Attributes;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Pokemon;

[UClass]
[UMetaData("HideCategories", "Shininess")]
[UsedImplicitly]
public class UShininessComponent : URPGComponent
{
    [UProperty(PropertyFlags.Transient)]
    private UIdentityComponent IdentityComponent { get; set; }

    [UProperty(PropertyFlags.Transient)]
    private UOwnershipComponent OwnershipComponent { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Shininess")]
    public bool IsShiny { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Shininess")]
    public bool IsSuperShiny
    {
        get;
        set
        {
            field = value;
            if (value)
                IsShiny = true;
        }
    }

    protected override void PreInitialize()
    {
        IdentityComponent = GetRequiredSiblingComponent<UIdentityComponent>();
        OwnershipComponent = GetRequiredSiblingComponent<UOwnershipComponent>();
    }

    [UFunction]
    [ExcludeFromExtensions]
    // ReSharper disable once UnusedParameter.Global
    public void Initialize(FPokemonInitParams initParams)
    {
        var personality = IdentityComponent.PersonalityValue;
        var ownerId = OwnershipComponent.Owner.ID;

        var shininessValue = GetShininessValue(personality, ownerId);
        IsShiny = shininessValue < PokemonStatics.ShininessChance;
        IsSuperShiny = shininessValue == 0;
    }

    protected virtual long GetShininessValue(uint personality, int ownerId)
    {
        var a = personality ^ ownerId;
        var b = a & 0xFFFF;
        var c = (a >> 16) & 0xFFFF;
        return b ^ c;
    }
}
