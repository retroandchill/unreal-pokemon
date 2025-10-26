using System.Reflection;
using GameDataAccessTools.Core.Views;
using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Mappers;
using Pokemon.Editor.Serialization.Model.Pbs;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameDataAccessTools;
using ZLinq;

namespace Pokemon.Editor.Serialization.Pbs.Serializers;

/// <summary>
/// The UAbilityPbsSerializer class is responsible for handling the serialization
/// and deserialization of Ability-related data using the PBS (Pokemon Broadcast System) file format.
/// </summary>
/// <remarks>
/// This class is derived from UGameDataEntryPbsSerializerBase and implements the required methods
/// to serialize and deserialize data specific to abilities stored in a UAbilityRepository.
/// </remarks>
[UClass]
public sealed partial class UAbilityPbsSerializer : UGameDataEntryPbsSerializerBase
{
    /// <inheritdoc />
    protected override bool Supports_Implementation(TSubclassOf<UAssetGameDataRepository> repositoryClass)
    {
        return repositoryClass == typeof(UAbilityRepository);
    }

    /// <inheritdoc />
    protected override void Serialize(StreamWriter streamWriter, UAssetGameDataRepository repository)
    {
        if (repository is not UAbilityRepository abilityRepository)
            throw new InvalidOperationException("Repository is not of type UAbilityRepository");

        PbsCompiler.WritePbs(
            abilityRepository.Entries.AsValueEnumerable().Select(x => x.ToAbilityInfo()).ToArray(),
            streamWriter
        );
    }

    /// <inheritdoc />
    protected override void Deserialize(StreamReader streamReader, UAssetGameDataRepository repository)
    {
        if (
            repository
                .GetType()
                .GetProperty("DataEntries", BindingFlags.NonPublic | BindingFlags.Instance)!
                .GetValue(repository)
            is not IList<FAbility> dataEntries
        )
        {
            throw new InvalidOperationException("DataEntries was not found");
        }

        dataEntries.Clear();
        foreach (
            var entry in PbsCompiler
                .CompilePbsFile<AbilityInfo>(streamReader)
                .Select(x => x.Value)
                .Select(x => x.ToAbility())
        )
        {
            dataEntries.Add(entry);
        }
    }
}
