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
/// A PBS serializer designed specifically for handling move-related game data.
/// This class serializes and deserializes data to and from the Pokémon PBS text format.
/// </summary>
/// <remarks>
/// This serializer is tailored to work exclusively with instances of UMoveRepository,
/// ensuring that only move data is serialized and deserialized.
/// </remarks>
[UClass]
public sealed partial class UMovePbsSerializer : UGameDataEntryPbsSerializerBase
{
    /// <inheritdoc />
    protected override bool Supports_Implementation(TSubclassOf<UAssetGameDataRepository> repositoryClass)
    {
        return repositoryClass == typeof(UMoveRepository);
    }

    /// <inheritdoc />
    protected override void Serialize(StreamWriter streamWriter, UAssetGameDataRepository repository)
    {
        if (repository is not UMoveRepository moveRepository)
            throw new InvalidOperationException("Repository is not of type UMoveRepository");

        PbsCompiler.WritePbs(
            moveRepository.Entries.AsValueEnumerable().Select(x => x.ToMoveInfo()).ToArray(),
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
            is not IList<FMove> dataEntries
        )
        {
            throw new InvalidOperationException("DataEntries was not found");
        }

        dataEntries.Clear();
        foreach (
            var entry in PbsCompiler.CompilePbsFile<MoveInfo>(streamReader).Select(x => x.Value).Select(x => x.ToMove())
        )
        {
            dataEntries.Add(entry);
        }
    }
}
