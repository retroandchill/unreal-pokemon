using System.Runtime.InteropServices;
using JetBrains.Annotations;
using Pokemon.Data.Model.HardCoded;
using UnrealSharp.Core;
using UnrealSharp.Core.Marshallers;
using UnrealSharp.CoreUObject;

namespace Pokemon.Data.Interop;

public readonly unsafe struct PokemonDataManagedActions
{
    [UsedImplicitly]
    public required delegate* unmanaged<void> StartDataManager { get; init; }

    [UsedImplicitly]
    public required delegate* unmanaged<
        IntPtr,
        IntPtr,
        IntPtr,
        NativeBool> GetEvolutionConditionClass { get; init; }

    public static PokemonDataManagedActions Create()
    {
        return new PokemonDataManagedActions
        {
            StartDataManager = &PokemonDataManagedCallbacks.StartDataManager,
            GetEvolutionConditionClass = &PokemonDataManagedCallbacks.GetEvolutionConditionClass,
        };
    }
}

public static class PokemonDataManagedCallbacks
{
    [UnmanagedCallersOnly]
    public static void StartDataManager()
    {
        FPokemonDataModule.Instance.CreateGameDataManager();
    }

    [UnmanagedCallersOnly]
    public static NativeBool GetEvolutionConditionClass(
        IntPtr handlePtr,
        IntPtr evolutionMethodResult,
        IntPtr resultString
    )
    {
        try
        {
            var handle = FEvolutionHandle.FromNative(handlePtr);
            var evolutionMethodTag = handle.IsValid ? handle.Entry.Parameter : null;
            ObjectMarshaller<UScriptStruct>.ToNative(evolutionMethodResult, 0, evolutionMethodTag!);
            return NativeBool.True;
        }
        catch (Exception e)
        {
            StringMarshaller.ToNative(resultString, 0, $"{e.Message}\n{e.StackTrace}");
            return NativeBool.False;
        }
    }
}
