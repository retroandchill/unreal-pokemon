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
public sealed class UBerryPlantPbsSerializer : UGameDataEntryPbsSerializerBase
{
    protected override bool Supports(TSubclassOf<UAssetGameDataRepository> repositoryClass)
    {
        return repositoryClass == typeof(UBerryPlantRepository);
    }

    protected override void Serialize(
        StreamWriter streamWriter,
        UAssetGameDataRepository repository
    )
    {
        if (repository is not UBerryPlantRepository abilityRepository)
            throw new InvalidOperationException("Repository is not of type UBerryPlantRepository");

        PbsCompiler.WritePbs(
            abilityRepository
                .Entries.AsValueEnumerable()
                .Select(x => x.ToBerryPlantInfo())
                .ToArray(),
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
            is not IList<FBerryPlant> dataEntries
        )
        {
            throw new InvalidOperationException("DataEntries was not found");
        }

        dataEntries.Clear();
        foreach (
            var entry in PbsCompiler
                .CompilePbsFile<BerryPlantInfo>(streamReader)
                .Select(x => x.Value)
                .Select(x => x.ToBerryPlant())
        )
        {
            dataEntries.Add(entry);
        }
    }
}
