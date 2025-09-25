namespace RPG.Battle.Traits;

public interface ITraitSource
{
    IEnumerable<ITrait> ActiveTraits { get; }
}