using System.Collections.Immutable;
using System.Reflection;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using GameDataAccessTools.Core.Views;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameDataAccessToolsEditor;
using ZLinq;

namespace Pokemon.Editor.Serialization.Pbs.Serializers;

/// <summary>
/// A base class for serializers implementing the PBS format for game data entries.
/// This abstract class provides core serialization and deserialization logic while allowing customization
/// of format-specific details such as file extensions and format names.
/// </summary>
[UClass(ClassFlags.Abstract)]
public partial class UGameDataEntryPbsSerializerBase : UCSGameDataEntrySerializer
{
    /// <inheritdoc />
    protected override FText GetFormatName_Implementation()
    {
        return FText.Localized("Pokemon.Editor", "PbsFormatName", "PBS");
    }

    /// <inheritdoc />
    protected override string GetFileExtensionText_Implementation()
    {
        return "PBS file |*.txt|";
    }

    /// <inheritdoc />
    protected override bool Serialize_Implementation(string filePath, UAssetGameDataRepository repository, out string errorMessage)
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

    /// <inheritdoc />
    protected override bool Deserialize_Implementation(string filePath, UAssetGameDataRepository repository, out string errorMessage)
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

    /// <summary>
    /// Serializes the provided repository data into the specified output stream.
    /// </summary>
    /// <param name="streamWriter">The StreamWriter instance to write serialized data to.</param>
    /// <param name="repository">The data repository containing the information to be serialized.</param>
    protected virtual void Serialize(StreamWriter streamWriter, UAssetGameDataRepository repository)
    {
        throw new InvalidOperationException("Not implemented");
    }

    /// <summary>
    /// Deserializes the data from a specified file into the provided game data repository.
    /// </summary>
    /// <param name="streamReader">The stream to read the data from.</param>
    /// <param name="repository">The game data repository to populate with deserialized data.</param>
    /// <returns>True if deserialization is successful; otherwise, false.</returns>
    protected virtual void Deserialize(StreamReader streamReader, UAssetGameDataRepository repository)
    {
        throw new InvalidOperationException("Not implemented");
    }
}
