using UnrealSharp.Attributes;
using UnrealSharp.RPGSaving;

namespace Pokemon.Core.Model.Trainer;

/// <summary>
/// Represents the information of a party component.
/// This struct is used to save or process data related to a trainer's party.
/// </summary>
[UStruct]
public readonly partial record struct FPartyComponentInfo
{
    /// <summary>
    /// Represents the party data for a trainer, which is a collection of saved entity data handles.
    /// </summary>
    /// <remarks>
    /// This property contains the saved data for each member of the trainer's party.
    /// Each member is represented by an instance of <see cref="FRPGEntitySaveDataHandle"/>,
    /// which includes details about the entity's state and class.
    /// </remarks>
    [field: UProperty(PropertyFlags.SaveGame)]
    public required IReadOnlyList<FRPGEntitySaveDataHandle> Party { get; init; }
}
