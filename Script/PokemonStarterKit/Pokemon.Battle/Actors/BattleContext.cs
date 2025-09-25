using Pokemon.Battle.Entities;
using UnrealSharp.Attributes;
using UnrealSharp.Engine;

namespace Pokemon.Battle.Actors;

[UClass]
public class ABattleContext : AInfo
{
    public IEnumerable<UBattler> AllBattlers => throw new NotImplementedException();
}
