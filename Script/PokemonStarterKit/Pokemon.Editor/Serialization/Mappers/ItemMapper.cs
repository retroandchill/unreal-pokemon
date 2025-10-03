using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Model.Pbs;
using Riok.Mapperly.Abstractions;

namespace Pokemon.Editor.Serialization.Mappers;

/// <summary>
/// Provides mapping functionality between <see cref="ItemInfo"/> and <see cref="FItem"/>.
/// This class leverages the Mapperly library for the mapping operations,
/// enabling conversions between models used in the serialization process and the domain entities.
/// </summary>
[Mapper(RequiredMappingStrategy = RequiredMappingStrategy.Target, PreferParameterlessConstructors = false)]
public static partial class ItemMapper
{
    /// <summary>
    /// Maps an <see cref="ItemInfo"/> object to an <see cref="FItem"/> object.
    /// </summary>
    /// <param name="itemInfo">The <see cref="ItemInfo"/> instance to map from.</param>
    /// <returns>Returns the mapped <see cref="FItem"/> object.</returns>
    [MapPropertyFromSource(nameof(FItem.SellPrice), Use = nameof(MapSellPrice))]
    [MapPropertyFromSource(nameof(FItem.CanSell), Use = nameof(GetCanSell))]
    [MapPropertyFromSource(nameof(FItem.ShowQuantity), Use = nameof(GetShowQuality))]
    [MapPropertyFromSource(nameof(FItem.IsConsumable), Use = nameof(GetConsumable))]
    public static partial FItem ToItem(this ItemInfo itemInfo);

    /// <summary>
    /// Maps an <see cref="FItem"/> object to an <see cref="ItemInfo"/> object.
    /// </summary>
    /// <param name="item">The <see cref="FItem"/> instance to map from.</param>
    /// <returns>Returns the mapped <see cref="ItemInfo"/> object.</returns>
    [MapPropertyFromSource(nameof(ItemInfo.SellPrice), Use = nameof(MapOutputSellPrice))]
    [MapPropertyFromSource(nameof(ItemInfo.Price), Use = nameof(MapPrice))]
    public static partial ItemInfo ToItemInfo(this FItem item);

    private static int MapPrice(in FItem item)
    {
        return item.CanSell ? item.Price : 0;
    }

    private static int MapSellPrice(ItemInfo item)
    {
        return item.SellPrice ?? item.Price / 2;
    }

    private static int? MapOutputSellPrice(in FItem item)
    {
        return item.SellPrice == item.Price / 2 ? null : item.SellPrice;
    }

    private static bool GetCanSell(ItemInfo item)
    {
        return item.Price > 0 || item.SellPrice > 0;
    }

    private static bool GetShowQuality(ItemInfo item)
    {
        return item.ShowQuantity
            ?? (
                item.FieldUse is not EFieldUse.TM and not EFieldUse.HM
            //&& !item.Tags.HasTag(GameplayTags.Pokemon_Metadata_Items_KeyItem)
            );
    }

    private static bool GetConsumable(ItemInfo item)
    {
        return item.IsConsumable
            ?? (
                item.FieldUse is not EFieldUse.TM and not EFieldUse.HM
            //&& !item.Tags.HasTag(GameplayTags.Pokemon_Metadata_Items_KeyItem)
            );
    }
}
