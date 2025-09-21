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

[UClass]
public sealed class UTypePbsSerializer : UGameDataEntryPbsSerializerBase
{
    protected override bool Supports(TSubclassOf<UAssetGameDataRepository> repositoryClass)
    {
        return repositoryClass == typeof(UTypeRepository);
    }

    protected override void Serialize(StreamWriter streamWriter, UAssetGameDataRepository repository)
    {
        if (repository is not UTypeRepository typeRepository)
            throw new InvalidOperationException("Repository is not of type UTypeRepository");

        PbsCompiler.WritePbs(typeRepository.Entries.AsValueEnumerable()
                .Select(x => x.ToTypeInfo())
                .ToArray(),
            streamWriter);
    }

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
        foreach (var entry in PbsCompiler
                     .CompilePbsFile<TypeInfo>(streamReader)
                     .Select(x => x.Value)
                     .Select(x => x.ToType()))
        {
            dataEntries.Add(entry);
        }
    }
}
