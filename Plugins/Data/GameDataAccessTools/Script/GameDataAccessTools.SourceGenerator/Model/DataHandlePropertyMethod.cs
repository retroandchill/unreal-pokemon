using GameAccessTools.SourceGenerator.Attributes;
using GameAccessTools.SourceGenerator.Utilities;
using Microsoft.CodeAnalysis;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;

namespace GameAccessTools.SourceGenerator.Model;

public record DataHandlePropertyMethod(IPropertySymbol Property)
{
    public string Type => Property.Type.ToDisplayString();
    
    public string Name => Property.Name;
    
    public string Prefix => Property.Type.SpecialType == SpecialType.System_Boolean && Property.Name.StartsWith("Is") ? "" : "Get";

    public string ConversionCall
    {
        get
        {
            if (Property.HasAttribute<AsValueAttribute>())
            {
                return "";
            }
            
            return Property.Type.MetadataName switch
            {
                "IReadOnlyList`1" or "IList`1" or "TNativeArray`1" or "ReadOnlySpan`1" => ".AsValueEnumerable().ToArray()",
                _ => ""
            };
        }
    }
}