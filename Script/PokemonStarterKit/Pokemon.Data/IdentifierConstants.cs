using GameAccessTools.SourceGenerator.Attributes;
using Pokemon.Data.HardCoded;

namespace Pokemon.Data;

[StaticIdentifier<FGrowthRate>("Medium", "Erratic", "Fluctuating", "Parabolic", "Fast", "Slow")]
[StaticIdentifier<FGenderRatio>(
    "AlwaysMale",
    "AlwaysFemale",
    "Genderless",
    "FemaleOneEighth",
    "Female25Percent",
    "Female50Percent",
    "Female75Percent",
    "FemaleSevenEighths"
)]
public static partial class IdentifierConstants;
