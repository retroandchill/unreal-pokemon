using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Model.Pbs;
using Riok.Mapperly.Abstractions;
using UnrealSharp.CoreUObject;

namespace Pokemon.Editor.Serialization.Mappers;

[Mapper(
    RequiredMappingStrategy = RequiredMappingStrategy.Target,
    PreferParameterlessConstructors = false
)]
public static partial class BerryPlantMapper
{
    public static partial FBerryPlant ToBerryPlant(this BerryPlantInfo berryPlantInfo);
    
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