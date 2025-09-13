using RPG.SourceGenerator.Attributes;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Entities.Components;

[UClass]
public class UStatComponent : URPGComponent
{
    [UProperty(PropertyFlags.Transient)]
    private UIdentityComponent IdentityComponent { get; set; }

    protected override void PreInitialize()
    {
        IdentityComponent = GetRequiredSiblingComponent<UIdentityComponent>();
    }

    [UFunction]
    [ExcludeFromExtensions]
    public void Initialize(FPokemonInitParams initParams) { }
}
