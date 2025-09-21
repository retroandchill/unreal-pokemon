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

[UClass]
public sealed class UItemPbsSerializer : UGameDataEntryPbsSerializerBase
{
    protected override bool Supports(TSubclassOf<UAssetGameDataRepository> repositoryClass)
    {
        return repositoryClass == typeof(UItemRepository);
    }

    protected override void Serialize(
        StreamWriter streamWriter,
        UAssetGameDataRepository repository
    )
    {
        if (repository is not UItemRepository itemRepository)
            throw new InvalidOperationException("Repository is not of type UItemRepository");

        PbsCompiler.WritePbs(
            itemRepository.Entries.AsValueEnumerable().Select(x => x.ToItemInfo()).ToArray(),
            streamWriter
        );
    }

    protected override void Deserialize(
        StreamReader streamReader,
        UAssetGameDataRepository repository
    )
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
            var entry in PbsCompiler
                .CompilePbsFile<ItemInfo>(streamReader)
                .Select(x => x.Value)
                .Select(x => x.ToItem())
        )
        {
            dataEntries.Add(entry);
        }
    }
}
