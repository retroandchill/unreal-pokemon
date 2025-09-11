using GameAccessTools.SourceGenerator.Attributes;
using GameAccessTools.SourceGenerator.Model;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;

namespace GameAccessTools.SourceGenerator.Utilities;

public enum CollectionType
{
    List,
    Set,
    Dictionary,
    Span,
    Optional,
}

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

        var (type, marshaller) = propertyType.GetMarshallerName(
            symbol.HasAttribute<AsValueAttribute>()
        );
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

    private static MarshalledPropertyInfo GetMarshallerName(
        this ITypeSymbol typeSymbol,
        bool asValue
    )
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
                            return typeArguments[0]
                                .GetCollectionMarshallerInfo(
                                    typeArguments[0].IsBlittableType()
                                        ? CollectionType.Span
                                        : CollectionType.List,
                                    asValue
                                );
                        case "TNativeArray`1" or "ReadOnlySpan`1":
                            return typeArguments[0]
                                .GetCollectionMarshallerInfo(CollectionType.Span, asValue);
                        case "IReadOnlyDictionary`2" or "IDictionary`2":
                            return typeArguments[0]
                                .GetCollectionMarshallerInfo(
                                    CollectionType.Dictionary,
                                    asValue,
                                    typeArguments[1]
                                );
                        case "IReadOnlySet`1" or "ISet`1":
                            return typeArguments[0]
                                .GetCollectionMarshallerInfo(CollectionType.Set, asValue);
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
                            return typeArguments[0]
                                .GetCollectionMarshallerInfo(CollectionType.Optional, true);
                    }
                }

                switch (typeSymbol.TypeKind)
                {
                    case TypeKind.Enum:
                        return new MarshalledPropertyInfo(
                            typeSymbol.ToDisplayString(),
                            new MarshallerInfo($"EnumMarshaller<{typeSymbol.ToDisplayString()}>")
                        );
                    case TypeKind.Interface:
                        return new MarshalledPropertyInfo(
                            typeSymbol.ToDisplayString(),
                            new MarshallerInfo($"{typeSymbol.Name}Marshaller")
                        );
                    case TypeKind.Unknown:
                    case TypeKind.Array:
                    case TypeKind.Class:
                    case TypeKind.Delegate:
                    case TypeKind.Dynamic:
                    case TypeKind.Error:
                    case TypeKind.Module:
                    case TypeKind.Pointer:
                    case TypeKind.Struct:
                    case TypeKind.TypeParameter:
                    case TypeKind.Submission:
                    case TypeKind.FunctionPointer:
                    case TypeKind.Extension:
                    default:
                        break;
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

                switch (typeSymbol.MetadataName)
                {
                    case "TMulticastDelegate`1":
                        return new MarshalledPropertyInfo(
                            typeSymbol.ToDisplayString(),
                            new MarshallerInfo(
                                $"MulticastDelegateMarshaller<{typeSymbol.ToDisplayString()}>"
                            )
                        );
                    case "TDelegate`1":
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
                        == SourceContextNames.UStructAttribute
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

                if (asValue)
                {
                    return new MarshalledPropertyInfo(
                        typeSymbol.ToDisplayString(),
                        new MarshallerInfo($"StructMarshaller<{typeSymbol.ToDisplayString()}>")
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
                attr.AttributeClass?.ToDisplayString() == SourceContextNames.UStructAttribute
            );
        if (structAttribute is null)
            return false;

        var blittableAttribute = typeSymbol
            .GetAttributes()
            .SingleOrDefault(attr =>
                attr.AttributeClass?.ToDisplayString() == SourceContextNames.BlittableTypeAttribute
            );
        if (blittableAttribute is not null)
        {
            return true;
        }

        var generatedTypeAttribute = typeSymbol
            .GetAttributes()
            .SingleOrDefault(attr =>
                attr.AttributeClass?.ToDisplayString() == SourceContextNames.GeneratedTypeAttribute
            );

        if (generatedTypeAttribute is not null)
        {
            return false;
        }

        return typeSymbol
            .GetMembers()
            .OfType<IFieldSymbol>()
            .All(f =>
                f.GetAttributes()
                    .Any(a =>
                        a.AttributeClass?.ToDisplayString() == SourceContextNames.UPropertyAttribute
                    ) && f.Type.IsBlittableType()
            );
    }

    private static MarshalledPropertyInfo GetCollectionMarshallerInfo(
        this ITypeSymbol innerType,
        CollectionType type,
        bool asValue,
        ITypeSymbol? valueType = null
    )
    {
        switch (type)
        {
            case CollectionType.List:
                if (asValue)
                {
                    return new MarshalledPropertyInfo(
                        $"IReadOnlyList<{innerType.TransformedType(asValue)}>",
                        new MarshallerInfo(
                            $"ArrayCopyMarshaller<{innerType.TransformedType(asValue)}>",
                            MarshallerType.Instanced,
                            innerType.GetMarshallerName(true).MarshallerInfo.Name
                        )
                    );
                }

                return new MarshalledPropertyInfo(
                    $"ArrayView<{innerType.TransformedType(asValue)}>",
                    new MarshallerInfo(
                        $"ArrayViewMarshaller<{innerType.TransformedType(asValue)}>",
                        MarshallerType.View,
                        innerType.GetMarshallerName(asValue).MarshallerInfo.Name
                    )
                );
            case CollectionType.Set:
                return new MarshalledPropertyInfo(
                    $"IReadOnlySet<{innerType.TransformedType(true)}>",
                    new MarshallerInfo(
                        $"SetCopyMarshaller<{innerType.TransformedType(true)}>",
                        innerType.GetMarshallerName(true).MarshallerInfo.Name
                    )
                );
            case CollectionType.Dictionary:
                if (valueType is null)
                {
                    throw new ArgumentException("Value type must be specified for dictionary");
                }
                return new MarshalledPropertyInfo(
                    $"IReadOnlyDictionary<{innerType.TransformedType(true)}, {valueType.TransformedType(true)}>",
                    new MarshallerInfo(
                        $"MapCopyMarshaller<{innerType.TransformedType(true)}, {valueType.TransformedType(true)}>",
                        innerType.GetMarshallerName(true).MarshallerInfo.Name,
                        valueType.GetMarshallerName(true).MarshallerInfo.Name
                    )
                );
            case CollectionType.Span:
                return new MarshalledPropertyInfo(
                    $"ReadOnlySpan<{innerType}>",
                    new MarshallerInfo(
                        $"SpanMarshaller<{innerType}>",
                        MarshallerType.Regular,
                        innerType.GetMarshallerName(true).MarshallerInfo.Name
                    )
                );
            case CollectionType.Optional:
                return new MarshalledPropertyInfo(
                    $"Option<{innerType}>",
                    new MarshallerInfo(
                        $"OptionMarshaller<{innerType}>",
                        innerType.GetMarshallerName(true).MarshallerInfo.Name
                    )
                );
            default:
                throw new ArgumentOutOfRangeException(nameof(type), type, null);
        }
    }

    private static string TransformedType(this ITypeSymbol typeSymbol, bool asValue)
    {
        if (asValue)
            return typeSymbol.ToDisplayString();

        var structAttribute = typeSymbol
            .GetAttributes()
            .SingleOrDefault(attr =>
                attr.AttributeClass?.ToDisplayString() == SourceContextNames.UStructAttribute
            );
        return structAttribute is not null
            ? $"StructView<{typeSymbol}>"
            : typeSymbol.ToDisplayString();
    }
}
