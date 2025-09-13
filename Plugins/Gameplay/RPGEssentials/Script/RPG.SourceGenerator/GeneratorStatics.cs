using Microsoft.CodeAnalysis;

namespace TurnBased.SourceGenerator;

public static class GeneratorStatics
{
    public const string RPGEntity = "UnrealSharp.RPGCore.URPGEntity";
    public const string RPGComponent = "UnrealSharp.RPGCore.URPGComponent";

    public const string UObject = "UnrealSharp.CoreUObject.UObject";

    public const string UStructAttribute = "UnrealSharp.Attributes.UStructAttribute";
    public const string UPropertyAttribute = "UnrealSharp.Attributes.UPropertyAttribute";
    public const string UFunctionAttribute = "UnrealSharp.Attributes.UFunctionAttribute";

    public static bool IsTurnBaseUnitOrComponent(this ITypeSymbol symbol)
    {
        for (var baseType = symbol; baseType != null; baseType = baseType.BaseType)
        {
            if (baseType.ToDisplayString() is RPGEntity or RPGComponent)
                return true;
        }

        return false;
    }

    public static bool IsTurnBasedUnit(this ITypeSymbol symbol)
    {
        for (var baseType = symbol; baseType != null; baseType = baseType.BaseType)
        {
            if (baseType.ToDisplayString() is RPGEntity)
                return true;
        }

        return false;
    }

    public static bool IsTurnBasedUnitComponent(this ITypeSymbol symbol)
    {
        for (var baseType = symbol; baseType != null; baseType = baseType.BaseType)
        {
            if (baseType.ToDisplayString() is RPGComponent)
                return true;
        }

        return false;
    }

    public static bool IsUObject(this ITypeSymbol symbol)
    {
        for (var baseType = symbol; baseType != null; baseType = baseType.BaseType)
        {
            if (baseType.ToDisplayString() is UObject)
                return true;
        }

        return false;
    }
}
