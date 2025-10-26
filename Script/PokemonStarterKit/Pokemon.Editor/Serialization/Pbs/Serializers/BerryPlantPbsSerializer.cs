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
/// A serializer class responsible for handling the serialization and deserialization of Berry Plant data
/// in the Pokemon Editor's PBS (Pokemon Battle System) format. It extends the functionality of
/// <see cref="UGameDataEntryPbsSerializerBase"/> to provide specific support for the Berry Plant type.
/// </summary>
[UClass]
public sealed partial class UBerryPlantPbsSerializer : UGameDataEntryPbsSerializerBase
{
    /// <inheritdoc />
    protected override bool Supports_Implementation(TSubclassOf<UAssetGameDataRepository> repositoryClass)
    {
        return repositoryClass == typeof(UBerryPlantRepository);
    }

    /// <inheritdoc />
    protected override void Serialize(StreamWriter streamWriter, UAssetGameDataRepository repository)
    {
        if (repository is not UBerryPlantRepository abilityRepository)
            throw new InvalidOperationException("Repository is not of type UBerryPlantRepository");

        PbsCompiler.WritePbs(
            abilityRepository.Entries.AsValueEnumerable().Select(x => x.ToBerryPlantInfo()).ToArray(),
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
