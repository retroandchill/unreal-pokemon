using JetBrains.Annotations;
using Pokemon.Core.Components.Bag;
using Pokemon.Data.Model.PBS;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Entities;

[UStruct]
public readonly partial record struct FPocketIdentifier(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)]
    FGameplayTag Tag, 
    [property: UProperty(PropertyFlags.BlueprintReadOnly)]
    FText Name);

[UStruct]
public readonly partial record struct FItemSlot(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)]
    FItemHandle Item, 
    [property: UProperty(PropertyFlags.BlueprintReadOnly)]
    int Quantity);

[UStruct]
public readonly partial record struct FBagInitParams(IReadOnlyList<FPocketSetting> Pockets, int MaxItemsPerSlot);

[UClass(ClassFlags.Abstract)]
public partial class UPokemonBag : URPGEntity
{
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UPocketsComponent.Initialize))]
    public UPocketsComponent PocketsComponent { get; private set; }
    
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UPocketMemoryComponent.Initialize))]
    public UPocketMemoryComponent PocketMemoryComponent { get; private set; }
    
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public UItemRegistrationComponent ItemRegistrationComponent { get; private set; }

    public static UPokemonBag Create(UObject outer)
    {
        var settings = GetDefault<UPokemonCoreSettings>();
        return Create(outer, new FBagInitParams(settings.Pockets, settings.MaxPerSlot));
    }
    
    [Factory]
    [PublicAPI]
    public static partial UPokemonBag Create(UObject outer, FBagInitParams parameters);

    
}