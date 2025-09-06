using System.Text.RegularExpressions;
using GameAccessTools.SourceGenerator.Model;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;

namespace GameAccessTools.SourceGenerator.Utilities;

public static class PropertyAccessUtilities
{
    public static AccessiblePropertyInfo GetPropertyInfo(this ISymbol symbol)
    {
        var propertyType = symbol switch
        {
            IFieldSymbol fieldSymbol => fieldSymbol.Type,
            IPropertySymbol propertySymbol => propertySymbol.Type,
            _ => throw new InvalidOperationException(),
        };
        var (type, marshaller) = propertyType.GetMarshallerName();
        return new AccessiblePropertyInfo(
            symbol.DeclaredAccessibility.GetAccessModifier(),
            type,
            symbol.Name
        )
        {
            MarshallerInfo = marshaller,
        };
    }

    public static string GetAccessModifier(this Accessibility accessibility)
    {
        return accessibility switch
        {
            Accessibility.Public => "public ",
            Accessibility.Protected => "protected ",
            Accessibility.Internal => "internal ",
            Accessibility.Private => "private ",
            Accessibility.ProtectedOrInternal => "protected internal ",
            Accessibility.ProtectedAndInternal => "protected internal ",
            Accessibility.NotApplicable => "",
            _ => throw new InvalidOperationException(),
        };
    }

    public static bool IsAutoProperty(this IPropertySymbol propertySymbol)
    {
        // If we don't know the syntax, assume it's not an auto property if there is a compiler-generated backing field.
        if (propertySymbol.DeclaringSyntaxReferences.Length == 0)
        {
            return propertySymbol
                .ContainingType.GetMembers()
                .OfType<IFieldSymbol>()
                .Any(f =>
                    SymbolEqualityComparer.Default.Equals(f.AssociatedSymbol, propertySymbol)
                );
        }

        return propertySymbol
            .DeclaringSyntaxReferences.Select(p => p.GetSyntax())
            .OfType<PropertyDeclarationSyntax>()
            .Select(p =>
                p.AccessorList?.Accessors.FirstOrDefault(a => a.Keyword.ValueText == "get")
            )
            .Any(a => a is not null && a.Body is null);
    }

    private static MarshalledPropertyInfo GetMarshallerName(this ITypeSymbol typeSymbol)
    {
        switch (typeSymbol.SpecialType)
        {
            case SpecialType.System_String:
                return new MarshalledPropertyInfo(
                    typeSymbol.ToDisplayString(),
                    new MarshallerInfo("StringMarshaller")
                );
            case SpecialType.System_Boolean:
                return new MarshalledPropertyInfo(
                    typeSymbol.ToDisplayString(),
                    new MarshallerInfo("BoolMarshaller")
                );
            case SpecialType.System_Byte
            or SpecialType.System_SByte
            or SpecialType.System_Int32
            or SpecialType.System_UInt32
            or SpecialType.System_Int64
            or SpecialType.System_UInt64
            or SpecialType.System_Single
            or SpecialType.System_Double:
                return new MarshalledPropertyInfo(
                    typeSymbol.ToDisplayString(),
                    new MarshallerInfo($"BlittableMarshaller<{typeSymbol.ToDisplayString()}>")
                );
            default:
            {
                if (typeSymbol is INamedTypeSymbol { IsGenericType: true } genericType)
                {
                    var typeArguments = genericType.TypeArguments.ToList();
                    switch (genericType.MetadataName)
                    {
                        case "IReadOnlyList`1" or "IList`1":
                            if (typeArguments[0].IsBlittableType())
                            {
                                return new MarshalledPropertyInfo(
                                    $"ReadOnlySpan<{typeArguments[0]}>",
                                    new MarshallerInfo(
                                        $"SpanMarshaller<{typeArguments[0]}>",
                                        MarshallerType.Regular,
                                        typeArguments[0].GetMarshallerName().MarshallerInfo.Name
                                    )
                                );
                            }

                            return new MarshalledPropertyInfo(
                                $"ArrayView<{typeArguments[0]}>",
                                new MarshallerInfo(
                                    $"ArrayViewMarshaller<{typeArguments[0]}>",
                                    MarshallerType.View,
                                    typeArguments[0].GetMarshallerName().MarshallerInfo.Name
                                )
                            );
                        case "TNativeArray`1" or "ReadOnlySpan`1":
                            return new MarshalledPropertyInfo(
                                $"ReadOnlySpan<{typeArguments[0]}>",
                                new MarshallerInfo(
                                    $"SpanMarshaller<{typeArguments[0]}>",
                                    MarshallerType.Regular,
                                    typeArguments[0].GetMarshallerName().MarshallerInfo.Name
                                )
                            );
                        case "IReadOnlyDictionary`2" or "IDictionary`2":
                            return new MarshalledPropertyInfo(
                                $"IReadOnlyDictionary<{typeArguments[0]}, {typeArguments[1]}>",
                                new MarshallerInfo(
                                    $"MapCopyMarshaller<{typeArguments[0]}, {typeArguments[1]}>",
                                    typeArguments[0].GetMarshallerName().MarshallerInfo.Name,
                                    typeArguments[1].GetMarshallerName().MarshallerInfo.Name
                                )
                            );
                        case "IReadOnlySet`1" or "ISet`1":
                            return new MarshalledPropertyInfo(
                                $"IReadOnlySet<{typeArguments[0]}>",
                                new MarshallerInfo(
                                    $"SetCopyMarshaller<{typeArguments[0]}>",
                                    typeArguments[0].GetMarshallerName().MarshallerInfo.Name
                                )
                            );
                        case "TSubclassOf`1":
                            return new MarshalledPropertyInfo(
                                typeSymbol.ToDisplayString(),
                                new MarshallerInfo($"SubclassOfMarshaller<{typeArguments[0]}>")
                            );
                        case "TWeakObjectPtr`1":
                            return new MarshalledPropertyInfo(
                                typeSymbol.ToDisplayString(),
                                new MarshallerInfo(
                                    $"BlittableMarshaller<{typeSymbol.ToDisplayString()}>"
                                )
                            );
                        case "TSoftObjectPtr`1":
                            return new MarshalledPropertyInfo(
                                typeSymbol.ToDisplayString(),
                                new MarshallerInfo($"SoftObjectMarshaller<{typeArguments[0]}>")
                            );
                        case "TSoftClassPtr`1":
                            return new MarshalledPropertyInfo(
                                typeSymbol.ToDisplayString(),
                                new MarshallerInfo($"SoftClassMarshaller<{typeArguments[0]}>")
                            );
                        case "Option`1":
                            return new MarshalledPropertyInfo(
                                typeSymbol.ToDisplayString(),
                                new MarshallerInfo(
                                    $"OptionMarshaller<{typeArguments[0]}>",
                                    typeArguments[0].GetMarshallerName().MarshallerInfo.Name
                                )
                            );
                    }
                }

                if (typeSymbol.TypeKind == TypeKind.Enum)
                {
                    return new MarshalledPropertyInfo(
                        typeSymbol.ToDisplayString(),
                        new MarshallerInfo($"EnumMarshaller<{typeSymbol.ToDisplayString()}>")
                    );
                }

                if (typeSymbol.TypeKind == TypeKind.Interface)
                {
                    return new MarshalledPropertyInfo(
                        typeSymbol.ToDisplayString(),
                        new MarshallerInfo($"{typeSymbol.Name}Marshaller")
                    );
                }

                if (typeSymbol.ToDisplayString() == "UnrealSharp.FText")
                {
                    return new MarshalledPropertyInfo(
                        typeSymbol.ToDisplayString(),
                        new MarshallerInfo("TextMarshaller")
                    );
                }

                if (typeSymbol.ToDisplayString() == "UnrealSharp.FName")
                {
                    return new MarshalledPropertyInfo(
                        typeSymbol.ToDisplayString(),
                        new MarshallerInfo("BlittableMarshaller<FName>")
                    );
                }

                if (typeSymbol.MetadataName == "TMulticastDelegate`1")
                {
                    return new MarshalledPropertyInfo(
                        typeSymbol.ToDisplayString(),
                        new MarshallerInfo(
                            $"MulticastDelegateMarshaller<{typeSymbol.ToDisplayString()}>"
                        )
                    );
                }

                if (typeSymbol.MetadataName == "TDelegate`1")
                {
                    return new MarshalledPropertyInfo(
                        typeSymbol.ToDisplayString(),
                        new MarshallerInfo(
                            $"SingleDelegateMarshaller<{typeSymbol.ToDisplayString()}>"
                        )
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
                    return new MarshalledPropertyInfo(
                        typeSymbol.ToDisplayString(),
                        typeSymbol.IsUnmanagedType
                            ? new MarshallerInfo(
                                $"UnmanagedTypeMarshaller<{typeSymbol.ToDisplayString()}>"
                            )
                            : new MarshallerInfo(
                                $"ManagedObjectMarshaller<{typeSymbol.ToDisplayString()}>"
                            )
                    );
                }

                return new MarshalledPropertyInfo(
                    $"StructView<{typeSymbol}>",
                    new MarshallerInfo($"StructViewMarshaller<{typeSymbol.ToDisplayString()}>")
                );
            }
        }
    }

    private static bool IsBlittableType(this ITypeSymbol typeSymbol)
    {
        if (!typeSymbol.IsUnmanagedType)
        {
            return false;
        }

        if (
            typeSymbol.SpecialType
            is SpecialType.System_Byte
                or SpecialType.System_SByte
                or SpecialType.System_Int32
                or SpecialType.System_UInt32
                or SpecialType.System_Int64
                or SpecialType.System_UInt64
                or SpecialType.System_Single
                or SpecialType.System_Double
        )
        {
            return true;
        }

        if (
            typeSymbol is INamedTypeSymbol { IsGenericType: true, MetadataName: "TWeakObjectPtr`1" }
        )
            return true;
        if (typeSymbol.ToDisplayString() == "UnrealSharp.FName")
            return true;

        var structAttribute = typeSymbol
            .GetAttributes()
            .SingleOrDefault(attr =>
                attr.AttributeClass?.ToDisplayString() == "UnrealSharp.Attributes.UStructAttribute"
            );
        if (structAttribute is null)
            return false;

        var blittableAttribute = typeSymbol
            .GetAttributes()
            .SingleOrDefault(attr =>
                attr.AttributeClass?.ToDisplayString()
                == "UnrealSharp.Core.Attributes.BlittableTypeAttribute"
            );
        return blittableAttribute is not null;
    }
}
