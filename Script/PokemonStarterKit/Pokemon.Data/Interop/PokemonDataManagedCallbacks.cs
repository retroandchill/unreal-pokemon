using System.Runtime.InteropServices;
using JetBrains.Annotations;
using Pokemon.Data.Model.HardCoded;
using UnrealSharp.Core;
using UnrealSharp.Core.Marshallers;
using UnrealSharp.CoreUObject;

namespace Pokemon.Data.Interop;

/// <summary>
/// Represents a structure containing function pointers for managing data-related actions.
/// </summary>
/// <remarks>
/// This struct is used to encapsulate unmanaged delegates that facilitate various operations
/// related to the Pokemon data management system. The functions specified in this structure are
/// expected to be manually assigned or utilized for interop purposes in unmanaged contexts.
/// </remarks>
public readonly unsafe struct PokemonDataManagedActions
{
    /// <summary>
    /// A delegate pointer to an unmanaged function that initializes the data management system for the Pokémon module.
    /// </summary>
    /// <remarks>
    /// This property holds a function pointer to the <c>StartDataManager</c> method, which is responsible for
    /// setting up the game data manager within the Pokémon data management system. The function initializes various
    /// Pokémon-related data components, such as growth rates, gender ratios, habitats, and other gameplay-related
    /// configurations.
    /// The method associated with this delegate pointer is invoked in unmanaged contexts and is essential for integrating
    /// the Pokémon data management infrastructure.
    /// </remarks>
    /// <seealso cref="PokemonDataManagedCallbacks.StartDataManager"/>
    /// <seealso cref="FPokemonDataModule.CreateGameDataManager"/>
    [UsedImplicitly]
    public required delegate* unmanaged<void> StartDataManager { get; init; }

    /// <summary>
    /// A delegate pointer to an unmanaged function that retrieves the evolution condition class based on specific parameters.
    /// </summary>
    /// <remarks>
    /// This property holds a function pointer to the <c>GetEvolutionConditionClass</c> method. It is responsible for
    /// processing evolution conditions by interacting with unmanaged data and returning the appropriate evolution class
    /// information. The method identified by this function pointer is called in unmanaged contexts to facilitate the
    /// evaluation and handling of evolution criteria.
    /// The associated function uses internal data structures and helpers to map evolution parameters and generate a
    /// corresponding evolution class result.
    /// </remarks>
    /// <seealso cref="PokemonDataManagedCallbacks.GetEvolutionConditionClass"/>
    /// <seealso cref="FEvolutionHandle"/>
    [UsedImplicitly]
    public required delegate* unmanaged<IntPtr, IntPtr, IntPtr, NativeBool> GetEvolutionConditionClass { get; init; }

    /// <summary>
    /// Creates and initializes an instance of <see cref="PokemonDataManagedActions"/> with the necessary unmanaged function pointers.
    /// </summary>
    /// <returns>
    /// An instance of <see cref="PokemonDataManagedActions"/> populated with the required function pointers.
    /// </returns>
    public static PokemonDataManagedActions Create()
    {
        return new PokemonDataManagedActions
        {
            StartDataManager = &PokemonDataManagedCallbacks.StartDataManager,
            GetEvolutionConditionClass = &PokemonDataManagedCallbacks.GetEvolutionConditionClass,
        };
    }
}

/// <summary>
/// Provides a static class containing callback methods utilized in the Pokémon data management system.
/// </summary>
/// <remarks>
/// This class includes methods designed for integration with unmanaged contexts, particularly for handling
/// Pokémon data-related operations such as initialization and retrieval processes. Each method in this class
/// is marked with the <c>[UnmanagedCallersOnly]</c> attribute to ensure compatibility with unmanaged
/// function calls.
/// </remarks>
public static class PokemonDataManagedCallbacks
{
    /// <summary>
    /// Initializes and sets up the game data management system for the Pokémon module.
    /// </summary>
    /// <remarks>
    /// This method is invoked in unmanaged contexts via a function pointer and is responsible for creating
    /// and configuring the game data manager, which handles core Pokémon-related data components.
    /// Examples include growth rate configurations, gender distributions, habitat data, and
    /// other gameplay configurations necessary for the Pokémon module's functionality.
    /// </remarks>
    [UnmanagedCallersOnly]
    public static void StartDataManager()
    {
        FPokemonDataModule.Instance.CreateGameDataManager();
    }

    /// <summary>
    /// Retrieves the evolution condition class for a specified handle and evolution method parameters.
    /// </summary>
    /// <param name="handlePtr">A pointer to the handle associated with the evolution condition request.</param>
    /// <param name="evolutionMethodResult">A pointer to the result object that stores the evolution method details.</param>
    /// <param name="resultString">A pointer to a string buffer for capturing error messages, if any occur during execution.</param>
    /// <returns>
    /// A <see cref="NativeBool"/> indicating the success or failure of the operation.
    /// Returns <c>True</c> if the evolution condition class was successfully retrieved; otherwise, <c>False</c>.
    /// </returns>
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
