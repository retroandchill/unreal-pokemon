using System.Text.RegularExpressions;
using GameAccessTools.SourceGenerator.Model;
using Microsoft.CodeAnalysis;

namespace GameAccessTools.SourceGenerator.Utilities;

public static class PropertyAccessUtilities
{
    public static AccessiblePropertyInfo GetPropertyInfo(this IPropertySymbol propertySymbol)
    {
        return new AccessiblePropertyInfo(propertySymbol.Type, propertySymbol.Name)
        {
            MarshallerInfo = propertySymbol.Type.GetMarshallerName(),
        };
    }

    private static MarshallerInfo GetMarshallerName(this ITypeSymbol typeSymbol)
    {
        switch (typeSymbol.SpecialType)
        {
            case SpecialType.System_String:
                return new MarshallerInfo("StringMarshaller");
            case SpecialType.System_Boolean:
                return new MarshallerInfo("BoolMarshaller");
            case SpecialType.System_Byte
            or SpecialType.System_SByte
            or SpecialType.System_Int32
            or SpecialType.System_UInt32
            or SpecialType.System_Int64
            or SpecialType.System_UInt64
            or SpecialType.System_Single
            or SpecialType.System_Double:
                return new MarshallerInfo($"BlittableMarshaller<{typeSymbol.ToDisplayString()}>");
            default:
            {
                if (typeSymbol is INamedTypeSymbol { IsGenericType: true } genericType)
                {
                    var typeArguments = genericType.TypeArguments.ToList();
                    switch (genericType.MetadataName)
                    {
                        case "IReadOnlyList`1" or "IList`1":
                            return new MarshallerInfo(
                                $"ArrayCopyMarshaller<{typeArguments[0]}>",
                                typeArguments[0].GetMarshallerName().Name
                            );
                        case "TNativeArray`1" or "ReadOnlySpan`1":
                            return new MarshallerInfo(
                                $"NativeArrayMarshaller<{typeArguments[0]}>",
                                typeArguments[0].GetMarshallerName().Name
                            );
                        case "IReadOnlyDictionary`2" or "IDictionary`2":
                            return new MarshallerInfo(
                                $"MapCopyMarshaller<{typeArguments[0]}, {typeArguments[1]}>",
                                typeArguments[0].GetMarshallerName().Name,
                                typeArguments[1].GetMarshallerName().Name
                            );
                        case "IReadOnlySet`1" or "ISet`1":
                            return new MarshallerInfo(
                                $"SetCopyMarshaller<{typeArguments[0]}>",
                                typeArguments[0].GetMarshallerName().Name
                            );
                        case "TSubclassOf`1":
                            return new MarshallerInfo($"SubclassOfMarshaller<{typeArguments[0]}>");
                        case "TWeakObjectPtr`1":
                            return new MarshallerInfo(
                                $"BlittableMarshaller<{typeSymbol.ToDisplayString()}>"
                            );
                        case "TSoftObjectPtr`1":
                            return new MarshallerInfo($"SoftObjectMarshaller<{typeArguments[0]}>");
                        case "TSoftClassPtr`1":
                            return new MarshallerInfo($"SoftClassMarshaller<{typeArguments[0]}>");
                        case "Option`1":
                            return new MarshallerInfo(
                                $"OptionMarshaller<{typeArguments[0]}>",
                                typeArguments[0].GetMarshallerName().Name
                            );
                    }
                }

                if (typeSymbol.TypeKind == TypeKind.Enum)
                {
                    return new MarshallerInfo($"EnumMarshaller<{typeSymbol.ToDisplayString()}>");
                }

                if (typeSymbol.TypeKind == TypeKind.Interface)
                {
                    return new MarshallerInfo($"{typeSymbol.Name}Marshaller");
                }

                if (typeSymbol.ToDisplayString() == "UnrealSharp.FText")
                {
                    return new MarshallerInfo("TextMarshaller");
                }

                if (typeSymbol.ToDisplayString() == "UnrealSharp.FName")
                {
                    return new MarshallerInfo("BlittableMarshaller<FName>");
                }

                if (typeSymbol.MetadataName == "TMulticastDelegate`1")
                {
                    return new MarshallerInfo(
                        $"MulticastDelegateMarshaller<{typeSymbol.ToDisplayString()}>"
                    );
                }

                if (typeSymbol.MetadataName == "TDelegate`1")
                {
                    return new MarshallerInfo(
                        $"SingleDelegateMarshaller<{typeSymbol.ToDisplayString()}>"
                    );
                }

                var structAttribute = typeSymbol
                    .GetAttributes()
                    .SingleOrDefault(attr =>
                        attr.AttributeClass?.ToDisplayString()
                        == "UnrealSharp.Attributes.UStructAttribute"
                    );
                if (structAttribute is null)
                {
                    return typeSymbol.IsUnmanagedType
                        ? new MarshallerInfo(
                            $"UnmanagedTypeMarshaller<{typeSymbol.ToDisplayString()}>"
                        )
                        : new MarshallerInfo(
                            $"ManagedObjectMarshaller<{typeSymbol.ToDisplayString()}>"
                        );
                }

                return new MarshallerInfo($"StructMarshaller<{typeSymbol.ToDisplayString()}>");
            }
        }
    }
}
