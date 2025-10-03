using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Model.Pbs;
using Riok.Mapperly.Abstractions;
using UnrealSharp.CoreUObject;

namespace Pokemon.Editor.Serialization.Mappers;

/// <summary>
/// Provides mapping extensions between <see cref="BerryPlantInfo"/> and <see cref="FBerryPlant"/>.
/// </summary>
/// <remarks>
/// The BerryPlantMapper class is a static partial class utilizing Mapperly for automated object mappings.
/// It includes methods for converting data models between the serialization layer and the core game data model.
/// </remarks>
[Mapper(RequiredMappingStrategy = RequiredMappingStrategy.Target, PreferParameterlessConstructors = false)]
public static partial class BerryPlantMapper
{
    /// <summary>
    /// Converts a <see cref="BerryPlantInfo"/> instance to an <see cref="FBerryPlant"/> instance.
    /// </summary>
    /// <param name="berryPlantInfo">The instance of <see cref="BerryPlantInfo"/> to convert.</param>
    /// <returns>An instance of <see cref="FBerryPlant"/> mapped from the provided <see cref="BerryPlantInfo"/>.</returns>
    public static partial FBerryPlant ToBerryPlant(this BerryPlantInfo berryPlantInfo);

    /// <summary>
    /// Converts an <see cref="FBerryPlant"/> instance to a <see cref="BerryPlantInfo"/> instance.
    /// </summary>
    /// <param name="ability">The instance of <see cref="FBerryPlant"/> to convert.</param>
    /// <returns>An instance of <see cref="BerryPlantInfo"/> mapped from the provided <see cref="FBerryPlant"/>.</returns>
    public static partial BerryPlantInfo ToBerryPlantInfo(this FBerryPlant ability);

    [MapProperty(nameof(FInt32Range.LowerBound.Value), nameof(IntBounds.Min))]
    [MapProperty(nameof(FInt32Range.UpperBound.Value), nameof(IntBounds.Max))]
    private static partial IntBounds ToIntBounds(this FInt32Range bounds);

    [MapProperty(nameof(IntBounds.Min), nameof(FInt32Range.LowerBound))]
    [MapProperty(nameof(IntBounds.Max), nameof(FInt32Range.UpperBound))]
    private static partial FInt32Range ToInt32Range(this IntBounds bounds);

    private static FInt32RangeBound ToInt32RangeBound(this int bound)
    {
        return new FInt32RangeBound { Value = bound, Type = ERangeBoundTypes.Inclusive };
    }
}
