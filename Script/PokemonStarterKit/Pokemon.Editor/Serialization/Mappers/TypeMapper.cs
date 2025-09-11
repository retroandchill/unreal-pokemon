using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Model.Pbs;
using Riok.Mapperly.Abstractions;

namespace Pokemon.Editor.Serialization.Mappers;

[Mapper(
    RequiredMappingStrategy = RequiredMappingStrategy.Target,
    PreferParameterlessConstructors = false
)]
public static partial class TypeMapper
{
    public static partial FType ToType(this TypeInfo typeInfo);
    
    public static partial TypeInfo ToTypeInfo(this FType type);
}