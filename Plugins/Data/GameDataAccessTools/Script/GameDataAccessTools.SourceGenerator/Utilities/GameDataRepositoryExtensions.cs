using GameAccessTools.SourceGenerator.Attributes;
using GameAccessTools.SourceGenerator.Model;
using Microsoft.CodeAnalysis;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;

namespace GameAccessTools.SourceGenerator.Utilities;

public static class GameDataRepositoryExtensions
{
    public static bool IsGameDataRepository(this INamedTypeSymbol type)
    {
        if (
            !type.AllInterfaces.Any(i =>
                i.IsGenericType
                && i.ConstructedFrom.ToDisplayString() == SourceContextNames.IGameDataRepository
            ) && !type.HasAttribute(typeof(GameDataRepositoryAttribute<>))
        )
        {
            return false;
        }

        for (var baseType = type.BaseType; baseType is not null; baseType = baseType.BaseType)
        {
            if (baseType.ToDisplayString() == SourceContextNames.UGameDataRepository)
            {
                return true;
            }
        }

        return false;
    }

    public static ITypeSymbol GetEntryType(this ITypeSymbol type)
    {
        return type.AllInterfaces.Where(i =>
                    i.IsGenericType
                    && i.ConstructedFrom.ToDisplayString() == SourceContextNames.IGameDataRepository
                )
                .Select(i => i.TypeArguments.Single())
                .SingleOrDefault()
            ?? type.GetAttributes().GetGameDataRepositoryInfos().Single().EntryType;
    }
}
