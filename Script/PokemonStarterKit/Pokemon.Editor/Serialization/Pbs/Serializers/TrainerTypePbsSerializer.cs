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
/// The UTrainerTypePbsSerializer class is responsible for serializing and deserializing
/// trainer type data to and from PBS (Plain Base System) files. It is a specialized
/// implementation of the UGameDataEntryPbsSerializerBase class that handles data specific
/// to the UTrainerTypeRepository.
/// </summary>
[UClass]
public sealed class UTrainerTypePbsSerializer : UGameDataEntryPbsSerializerBase
{
    /// <inheritdoc />
    protected override bool Supports(TSubclassOf<UAssetGameDataRepository> repositoryClass)
    {
        return repositoryClass == typeof(UTrainerTypeRepository);
    }

    /// <inheritdoc />
    protected override void Serialize(StreamWriter streamWriter, UAssetGameDataRepository repository)
    {
        if (repository is not UTrainerTypeRepository trainerTypeRepository)
            throw new InvalidOperationException("Repository is not of type UTrainerTypeRepository");

        PbsCompiler.WritePbs(
            trainerTypeRepository.Entries.AsValueEnumerable().Select(x => x.ToTrainerTypeInfo()).ToArray(),
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
            is not IList<FTrainerType> dataEntries
        )
        {
            throw new InvalidOperationException("DataEntries was not found");
        }

        dataEntries.Clear();
        foreach (
            var entry in PbsCompiler
                .CompilePbsFile<TrainerTypeInfo>(streamReader)
                .Select(x => x.Value)
                .Select(x => x.ToTrainerType())
        )
        {
            dataEntries.Add(entry);
        }
    }
}
