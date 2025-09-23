using LanguageExt.UnsafeValueAccess;
using Pokemon.Core.Components.Pokemon;
using Pokemon.Core.Model.Pokemon;
using Pokemon.Data.Model.HardCoded;
using Riok.Mapperly.Abstractions;
using RPG.SourceGenerator.Attributes;
using UnrealSharp.Attributes;
using UnrealSharp.RPGSaving;
using ZLinq;

namespace Pokemon.Core.Savers.Pokemon;

[UClass]
[Mapper(
    RequiredMappingStrategy = RequiredMappingStrategy.Target,
    PreferParameterlessConstructors = false
)]
[ComponentSaver<UStatComponent, FStatComponentInfo>(LoggerClass = typeof(LogPokemonCore))]
public partial class UStatComponentSaver : UComponentSaver
{
    [MapPropertyFromSource(nameof(FStatComponentInfo.IV), Use = nameof(GetIV))]
    [MapPropertyFromSource(nameof(FStatComponentInfo.IVOverrides), Use = nameof(GetIVOverrides))]
    [MapPropertyFromSource(nameof(FStatComponentInfo.EV), Use = nameof(GetEV))]
    private partial FStatComponentInfo SaveComponent(UStatComponent component);

    private static IReadOnlyDictionary<FMainStatHandle, int> GetIV(UStatComponent component)
    {
        return FMainStatHandle
            .Entries.Select(x => x.ID)
            .ToDictionary(x => new FMainStatHandle(x), x => component.GetIV(x));
    }

    private static IReadOnlyDictionary<FMainStatHandle, int> GetIVOverrides(
        UStatComponent component
    )
    {
        return FMainStatHandle
            .Entries.Select(x => (x.ID, IV: component.GetIVOverride(x.ID)))
            .Where(x => x.IV.IsSome)
            .ToDictionary(x => new FMainStatHandle(x.ID), x => x.IV.ValueUnsafe());
    }

    private static IReadOnlyDictionary<FMainStatHandle, int> GetEV(UStatComponent component)
    {
        return FMainStatHandle
            .Entries.Select(x => x.ID)
            .ToDictionary(x => new FMainStatHandle(x), x => component.GetEV(x));
    }

    private void LoadComponent(FStatComponentInfo info, UStatComponent component)
    {
        component.SetLevel(info.Level);
        component.SetExp(info.Exp);

        component.HP = info.HP;

        foreach (var (stat, iv) in info.IV)
        {
            component.SetIV(stat, iv, false);
        }

        foreach (var (stat, iv) in info.IVOverrides)
        {
            component.SetIVOverride(stat, iv, false);
        }

        foreach (var (stat, ev) in info.EV)
        {
            component.SetEV(stat, ev, false);
        }

        component.Nature = info.Nature;
        component.NatureOverride = info.NatureOverride;

        component.RecalculateStats();
    }
}
