using System.Collections.Immutable;
using System.Reflection;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using GameDataAccessTools.Core.Views;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameDataAccessToolsEditor;
using ZLinq;

namespace Pokemon.Editor.Serialization.Pbs.Serializers;

[UClass(ClassFlags.Abstract)]
[UsedImplicitly]
public class UGameDataEntryPbsSerializerBase : UCSGameDataEntrySerializer
{

    protected override FText GetFormatName()
    {
        return FText.Localized("Pokemon.Editor", "PbsFormatName", "PBS");
    }

    protected override string GetFileExtensionText()
    {
        return "PBS file |*.txt|";
    }

    protected override bool Serialize(
        string filePath,
        UAssetGameDataRepository repository,
        out string errorMessage
    )
    {
        try
        {
            using var fileStream = File.OpenWrite(filePath);
            using var streamWriter = new StreamWriter(fileStream);
            Serialize(streamWriter, repository);
            errorMessage = string.Empty;
            return true;
        }
        catch (Exception e)
        {
            if (e is TargetInvocationException { InnerException: not null } tie)
            {
                e = tie.InnerException;
            }

            errorMessage = e.Message;
            return false;
        }
    }

    protected override bool Deserialize(
        string filePath,
        UAssetGameDataRepository repository,
        out string errorMessage
    )
    {
        try
        {
            using var streamReader = File.OpenText(filePath);
            Deserialize(streamReader, repository);
            errorMessage = string.Empty;
            return true;
        }
        catch (Exception e)
        {
            if (e is TargetInvocationException { InnerException: not null } tie)
            {
                e = tie.InnerException;
            }

            errorMessage = e.Message;
            return false;
        }
    }

    protected virtual void Serialize(StreamWriter streamWriter, UAssetGameDataRepository repository)
    {
        throw new InvalidOperationException("Not implemented");
    }

    protected virtual void Deserialize(StreamReader streamReader, UAssetGameDataRepository repository)
    {
        throw new InvalidOperationException("Not implemented");
    }
}
