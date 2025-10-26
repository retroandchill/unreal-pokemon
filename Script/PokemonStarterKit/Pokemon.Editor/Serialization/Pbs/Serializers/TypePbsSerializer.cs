using System.Reflection;
using GameDataAccessTools.Core.Views;
using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Mappers;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameDataAccessTools;
using ZLinq;
using TypeInfo = Pokemon.Editor.Serialization.Model.Pbs.TypeInfo;

namespace Pokemon.Editor.Serialization.Pbs.Serializers;

/// <summary>
/// A serializer for handling Pokémon type data in the PBS format.
/// This class extends the base functionality provided by UGameDataEntryPbsSerializerBase
/// to support the UTypeRepository and its specific data serialization and deserialization needs.
/// </summary>
[UClass]
public sealed partial class UTypePbsSerializer : UGameDataEntryPbsSerializerBase
{
    /// <inheritdoc />
    protected override bool Supports_Implementation(TSubclassOf<UAssetGameDataRepository> repositoryClass)
    {
        return repositoryClass == typeof(UTypeRepository);
    }

    /// <inheritdoc />
    protected override void Serialize(StreamWriter streamWriter, UAssetGameDataRepository repository)
    {
        if (repository is not UTypeRepository typeRepository)
            throw new InvalidOperationException("Repository is not of type UTypeRepository");

        PbsCompiler.WritePbs(
            typeRepository.Entries.AsValueEnumerable().Select(x => x.ToTypeInfo()).ToArray(),
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
            is not IList<FType> dataEntries
        )
        {
            throw new InvalidOperationException("DataEntries was not found");
        }

        dataEntries.Clear();
        foreach (
            var entry in PbsCompiler.CompilePbsFile<TypeInfo>(streamReader).Select(x => x.Value).Select(x => x.ToType())
        )
        {
            dataEntries.Add(entry);
        }
    }
}
