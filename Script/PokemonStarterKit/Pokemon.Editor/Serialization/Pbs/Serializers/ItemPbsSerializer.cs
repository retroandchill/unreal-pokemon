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
/// A serializer for handling the PBS (Pokémon Battle System) format specifically for item data within the game.
/// </summary>
/// <remarks>
/// This class extends the functionality of the base PBS serializer to provide support for serializing
/// and deserializing item-related data from and to the game repository system.
/// </remarks>
[UClass]
public sealed partial class UItemPbsSerializer : UGameDataEntryPbsSerializerBase
{
    /// <inheritdoc />
    protected override bool Supports_Implementation(TSubclassOf<UAssetGameDataRepository> repositoryClass)
    {
        return repositoryClass == typeof(UItemRepository);
    }

    /// <inheritdoc />
    protected override void Serialize(StreamWriter streamWriter, UAssetGameDataRepository repository)
    {
        if (repository is not UItemRepository itemRepository)
            throw new InvalidOperationException("Repository is not of type UItemRepository");

        PbsCompiler.WritePbs(
            itemRepository.Entries.AsValueEnumerable().Select(x => x.ToItemInfo()).ToArray(),
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
            is not IList<FItem> dataEntries
        )
        {
            throw new InvalidOperationException("DataEntries was not found");
        }

        dataEntries.Clear();
        foreach (
            var entry in PbsCompiler.CompilePbsFile<ItemInfo>(streamReader).Select(x => x.Value).Select(x => x.ToItem())
        )
        {
            dataEntries.Add(entry);
        }
    }
}
