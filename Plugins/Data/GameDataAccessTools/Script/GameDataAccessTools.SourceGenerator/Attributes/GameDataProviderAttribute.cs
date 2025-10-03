#if GAME_DATA_ACCESS_TOOLS_GENERATOR
using RhoMicro.CodeAnalysis;
#else
using UnrealSharp;
using GameDataAccessTools.Core;
#endif

namespace GameAccessTools.SourceGenerator.Attributes;

/// <summary>
/// Represents an attribute used to mark a class as a game data provider.
/// </summary>
/// <remarks>
/// Classes annotated with this attribute are typically used to provide game-related data
/// and are leveraged during source generation to identify essential data provider types.
/// </remarks>
/// <example>
/// Use this attribute to mark a class that is intended to act as a data provider
/// in the game's data access layer.
/// </example>
[AttributeUsage(AttributeTargets.Class)]
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
[IncludeFile]
#endif
internal class GameDataProviderAttribute : Attribute
{
    /// <summary>
    /// Gets or sets the display name associated with the annotated game data provider.
    /// </summary>
    /// <remarks>
    /// This property is intended to provide a human-readable name for the data provider,
    /// which can be utilized for identification or display purposes in the context
    /// of game data access.
    /// </remarks>
    public string? DisplayName { get; init; }

    /// <summary>
    /// Gets or sets the category associated with the annotated game data provider.
    /// </summary>
    /// <remarks>
    /// This property is used to specify a classification or grouping for the game data provider,
    /// assisting in organizing and identifying providers according to their functionality or context.
    /// </remarks>
    public string Category { get; init; } = "Game";
}
