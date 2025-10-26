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
/// A serializer class designed to handle serialization and deserialization of species data
/// in PBS (Pokémon Battle Simulator) file format. This class extends the
/// UGameDataEntryPbsSerializerBase and provides specific implementations for working with
/// species data within a designated repository.
/// This class is responsible for serializing species data into PBS-compliant text format and
/// rebuilding species data structures from PBS files back into game data repositories.
/// </summary>
[UClass]
public sealed partial class USpeciesPbsSerializer : UGameDataEntryPbsSerializerBase
{
    /// <inheritdoc />
    protected override bool Supports_Implementation(TSubclassOf<UAssetGameDataRepository> repositoryClass)
    {
        return repositoryClass == typeof(USpeciesRepository);
    }

    /// <inheritdoc />
    protected override void Serialize(StreamWriter streamWriter, UAssetGameDataRepository repository)
    {
        if (repository is not USpeciesRepository speciesRepository)
            throw new InvalidOperationException("Repository is not of type USpeciesRepository");

        PbsCompiler.WritePbs(
            speciesRepository.Entries.AsValueEnumerable().Select(x => x.ToSpeciesInfo()).ToArray(),
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
            is not IList<FSpecies> dataEntries
        )
        {
            throw new InvalidOperationException("DataEntries was not found");
        }

        dataEntries.Clear();
        foreach (
            var entry in PbsCompiler
                .CompilePbsFile<SpeciesInfo>(streamReader)
                .Select(x => x.Value)
                .Select(x => x.ToSpecies())
        )
        {
            dataEntries.Add(entry);
        }
    }
}
