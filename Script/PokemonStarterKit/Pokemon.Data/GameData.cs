using GameAccessTools.SourceGenerator.Attributes;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;

namespace Pokemon.Data;

/// <summary>
/// The <c>GameData</c> class serves as a centralized repository for static game data
/// related to the Pokemon game. It provides access to various repositories covering
/// different aspects of the game, such as abilities, moves, items, species, and more.
/// Each property corresponds to a specific repository containing relevant game data.
/// </summary>
/// <remarks>
/// This class is a static class and cannot be instantiated. All properties are static
/// and provide access to game data via partial repository implementations.
/// The data managed by this class is categorized for organized access within the game.
/// </remarks>
[GameDataProvider(Category = "Game")]
public static partial class GameData
{
    /// <summary>
    /// Provides access to the repository of growth rates utilized in the game.
    /// Growth rates determine the experience thresholds required for leveling up various entities.
    /// This property is a static part of the GameData class and offers retrieval and management functionality
    /// for pre-defined growth rate data.
    /// </summary>
    public static partial UGrowthRateRepository GrowthRates { get; }

    /// <summary>
    /// Provides access to the repository of gender ratios used within the game.
    /// Gender ratios define the distribution of genders for various species or entities.
    /// This property is part of the GameData class and facilitates retrieval and
    /// management of predefined gender ratio data.
    /// </summary>
    public static partial UGenderRatioRepository GenderRatios { get; }

    /// <summary>
    /// Provides access to the repository of egg groups defined in the game.
    /// Egg groups categorize Pokémon into breeding groups, which determine compatibility for producing offspring.
    /// This property is a static member of the GameData class and facilitates retrieval and management
    /// of egg group metadata used in Pokémon breeding mechanics.
    /// </summary>
    public static partial UEggGroupRepository EggGroups { get; }

    /// <summary>
    /// Provides access to the repository of body shapes utilized in the game.
    /// Body shapes categorize Pokémon based on their physical structure, aiding in classification
    /// and organization of game data. This property is a static part of the GameData class
    /// and facilitates the retrieval and management of predefined body shape data.
    /// </summary>
    public static partial UBodyShapeRepository BodyShapes { get; }

    /// <summary>
    /// Represents the repository of body colors used to categorize Pokémon appearances.
    /// The BodyColors repository maintains the predefined set of visual color
    /// classifications that are used for organizing or grouping Pokémon data.
    /// This property is a static member of the GameData class, offering consistent
    /// access to body color data for various game elements.
    /// </summary>
    public static partial UBodyColorRepository BodyColors { get; }

    /// <summary>
    /// Provides access to the repository of habitats utilized in the game.
    /// Habitats represent distinct environmental regions or ecosystems where Pokémon
    /// can naturally be found or encountered.
    /// This property is a static part of the GameData class and enables management
    /// and retrieval of predefined habitat data for various in-game purposes.
    /// </summary>
    public static partial UHabitatRepository Habitats { get; }

    /// <summary>
    /// Provides access to the repository of evolution data utilized within the game.
    /// Evolutions define the specific conditions and logic for how Pokemon can evolve.
    /// This property is a static part of the GameData class, offering retrieval and management functionality
    /// for pre-defined evolution rules and details.
    /// </summary>
    public static partial UEvolutionRepository Evolutions { get; }

    /// <summary>
    /// Represents the repository of statistical data used within the game.
    /// This property provides access to predefined base stats and related information for various entities.
    /// It plays a critical role in calculations, attributes, and mechanics associated with entities, such as
    /// battles, growth, and performance within the game environment.
    /// </summary>
    public static partial UStatRepository Stats { get; }

    /// <summary>
    /// Provides a repository of natures within the game.
    /// Natures are used to define personality traits of entities that influence stat growth and other attributes.
    /// This property is a static member of the GameData class and supports retrieval and management
    /// of pre-defined nature data utilized in gameplay mechanics.
    /// </summary>
    public static partial UNatureRepository Natures { get; }

    /// <summary>
    /// Manages the repository of status conditions used in the game.
    /// Status conditions represent various states or effects that can be applied to entities,
    /// such as paralysis, sleep, or poisoning. This property provides access to predefined
    /// data for handling and interacting with these statuses within the game.
    /// </summary>
    public static partial UStatusRepository Statuses { get; }

    /// <summary>
    /// Provides access to the repository of field weather conditions available in the game.
    /// Field weather conditions influence environmental effects and game mechanics outside of battles.
    /// This property is a static part of the GameData class and enables retrieval and management
    /// of predefined field weather data.
    /// </summary>
    public static partial UFieldWeatherRepository FieldWeathers { get; }

    /// <summary>
    /// Provides access to the repository of predefined encounter types within the game.
    /// Encounter types represent the various methods or circumstances under which entities
    /// can be encountered, such as in the wild or through specific in-game events.
    /// This property is a static part of the GameData class and facilitates retrieval
    /// and management of structured data related to encounter type definitions.
    /// </summary>
    public static partial UEncounterTypeRepository EncounterTypes { get; }

    /// <summary>
    /// Provides access to the repository of environments defined within the game data.
    /// Environments represent various gameplay contexts or settings, such as terrain or weather,
    /// which influence the appearance and behavior of entities in the game.
    /// This property serves as a static part of the GameData class, facilitating the retrieval
    /// and management of predefined environment-related information.
    /// </summary>
    public static partial UEnvironmentRepository Environments { get; }

    /// <summary>
    /// Provides access to the repository of battle weathers utilized in the game.
    /// Battle weathers affect the conditions and interactions in battles, influencing moves, abilities, and mechanics.
    /// This property is a static part of the GameData class, offering retrieval and management functionality
    /// for predefined battle weather data.
    /// </summary>
    public static partial UBattleWeatherRepository BattleWeathers { get; }

    /// <summary>
    /// Represents the repository of battle terrains utilized in the game.
    /// Battle terrains define the environmental conditions that influence
    /// combat mechanics, interactions, and effects during battles.
    /// This property is a static part of the GameData class and allows for
    /// the retrieval and management of predefined battle terrain data.
    /// </summary>
    public static partial UBattleTerrainRepository BattleTerrains { get; }

    /// <summary>
    /// Provides access to the repository of target data utilized in the game.
    /// Targets represent various targeting behaviors or mechanisms used in gameplay,
    /// such as move targeting, area effects, or specific target designation.
    /// This property is a static part of the GameData class and facilitates retrieval
    /// and management of pre-defined target data.
    /// </summary>
    public static partial UTargetRepository Targets { get; }

    /// <summary>
    /// Represents the repository of Pokémon types used within the game data framework.
    /// Types define elemental attributes assigned to Pokémon, abilities, and moves, influencing
    /// effectiveness in battles and interactions between different attacks and Pokémon.
    /// This property enables retrieval and management of type-specific data.
    /// </summary>
    public static partial UTypeRepository Types { get; }

    /// <summary>
    /// Provides access to the repository of abilities available in the game.
    /// Abilities represent unique traits or powers that can affect a Pokémon's performance in battles or other scenarios.
    /// This property is a static part of the GameData class and offers methods to manage and retrieve ability data.
    /// </summary>
    public static partial UAbilityRepository Abilities { get; }

    /// <summary>
    /// Provides access to the repository of moves utilized in the game.
    /// Moves define the actions or abilities that entities can perform in battles or encounters.
    /// This property is a static member of the GameData class and facilitates retrieval and management
    /// of predefined move-related data.
    /// </summary>
    public static partial UMoveRepository Moves { get; }

    /// <summary>
    /// Allows access to the repository of items defined within the game data.
    /// Items include various in-game objects, such as consumables, equipment, or key items,
    /// each with unique attributes and functionalities influencing gameplay mechanics.
    /// This property provides mechanisms to retrieve, manage, and interact with predefined item data.
    /// </summary>
    public static partial UItemRepository Items { get; }

    /// <summary>
    /// Provides access to the repository of berry plants used within the game.
    /// Berry plants are entities related to the cultivation and harvesting of berries,
    /// often influencing gameplay mechanics such as item acquisition and berry-related functions.
    /// This property belongs to the GameData class and facilitates retrieval and
    /// management of berry plant-related data.
    /// </summary>
    public static partial UBerryPlantRepository BerryPlants { get; }

    /// <summary>
    /// Represents the repository of Pokémon species data used within the game.
    /// This property provides access to foundational information about different species,
    /// encompassing their attributes, statistics, evolutions, and other defining characteristics.
    /// It is a static component of the GameData class and is utilized for managing and retrieving
    /// species-specific data critical for game functionality.
    /// </summary>
    public static partial USpeciesRepository Species { get; }

    /// <summary>
    /// Represents the repository of trainer types used in the game.
    /// Trainer types define categories or classifications that trainers can belong to,
    /// each affecting their behavior, appearance, or related game data.
    /// This property provides access to predefined trainer type data for retrieval and management.
    /// </summary>
    public static partial UTrainerTypeRepository TrainerTypes { get; }
}
