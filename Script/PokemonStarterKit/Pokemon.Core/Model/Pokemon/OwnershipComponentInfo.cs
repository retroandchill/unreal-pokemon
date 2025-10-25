using LanguageExt;
using Pokemon.Core.Components.Pokemon;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;

namespace Pokemon.Core.Model.Pokemon;

/// <summary>
/// Represents ownership details of a Pokémon, encapsulating various metadata such as trainer information,
/// acquisition details, and additional data pertinent to the Pokémon's ownership history.
/// </summary>
[UStruct]
public readonly partial record struct FOwnershipComponentInfo
{
    /// <summary>
    /// Represents the unique identifier for a Pokémon entity within the ownership data.
    /// This ID is used to distinguish individual Pokémon in the context of ownership-related operations.
    /// </summary>
    [UProperty(PropertyFlags.SaveGame)]
    public required int ID { get; init; }

    /// <summary>
    /// Represents a secret identifier associated with the ownership of a Pokémon entity.
    /// This ID is used as an additional unique attribute for differentiating Pokémon
    /// beyond the primary ownership or general identifiers.
    /// </summary>
    [UProperty(PropertyFlags.SaveGame)]
    public required int SecretID { get; init; }

    /// <summary>
    /// Represents the name of the Pokémon's Original Trainer (OT).
    /// This property identifies the trainer who initially owned the Pokémon.
    /// </summary>
    [UProperty(PropertyFlags.SaveGame)]
    public required FText OTName { get; init; }

    /// <summary>
    /// Indicates the gender of the original trainer (OT) associated with the Pokémon.
    /// This property provides information about the trainer's gender, which can be Male, Female, Unknown, or Mixed.
    /// </summary>
    [UProperty(PropertyFlags.SaveGame)]
    public required ETrainerGender OTGender { get; init; }

    /// <summary>
    /// Specifies the method through which the Pokémon was originally obtained.
    /// This property indicates factors such as whether the Pokémon was hatched from an egg,
    /// received via trade, obtained in a fateful encounter, or other possible acquisition methods.
    /// </summary>
    [UProperty(PropertyFlags.SaveGame)]
    public required EObtainMethod ObtainMethod { get; init; }

    /// <summary>
    /// Represents the level at which the Pokémon was initially encountered or obtained.
    /// This property provides insight into the Pokémon's starting experience and growth context within its ownership history.
    /// </summary>
    [UProperty(PropertyFlags.SaveGame)]
    public required int LevelMet { get; init; }

    /// <summary>
    /// Represents the exact date and time when the Pokémon was received by its current owner.
    /// This property is used for maintaining historical data about a Pokémon's ownership
    /// and acquisition timeline.
    /// </summary>
    [UProperty(PropertyFlags.SaveGame)]
    public required FDateTime TimeReceived { get; init; }

    /// <summary>
    /// Describes the textual information regarding how the Pokémon was obtained.
    /// This property encapsulates context or narrative details about the acquisition of the Pokémon.
    /// </summary>
    [UProperty(PropertyFlags.SaveGame)]
    public required FText ObtainText { get; init; }

    /// <summary>
    /// Provides information about the hatching of a Pokémon, if applicable.
    /// Includes details such as the time the Pokémon hatched and any associated descriptive text.
    /// </summary>
    [UProperty(PropertyFlags.SaveGame)]
    public required Option<FHatchInformation> HatchInformation { get; init; }
}
