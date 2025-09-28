using System.Runtime.InteropServices;
using UnrealSharp.Core;

namespace UnrealSharp.PokemonBattle;

[StructLayout(LayoutKind.Sequential)]
internal struct NativeEnumStorage
{
    public byte Value { get; init; }

    public IntPtr NativeEnum { get; init; }
}

[StructLayout(LayoutKind.Sequential)]
internal struct NativeSmallStructStorage
{
    public FSmallStructBuffer Data { get; init; }

    public IntPtr NativeStruct { get; init; }
}

[StructLayout(LayoutKind.Explicit)]
internal struct BattleEffectNativeData
{
    [field: FieldOffset(0)]
    public NativeBool BoolValue { get; init; }

    [field: FieldOffset(0)]
    public int IntValue { get; init; }

    [field: FieldOffset(0)]
    public float FloatValue { get; init; }

    [field: FieldOffset(0)]
    public FName NameValue { get; init; }

    [field: FieldOffset(0)]
    public NativeEnumStorage EnumValue { get; init; }

    [field: FieldOffset(0)]
    public NativeSmallStructStorage SmallStructValue { get; init; }
}

public partial struct FBattleEffectData
{
    private BattleEffectNativeData _nativeData;

    public EBattleEffectDataType Type { get; }

    public FBattleEffectData(bool value)
    {
        _nativeData = new BattleEffectNativeData { BoolValue = value.ToNativeBool() };
        Type = EBattleEffectDataType.Bool;
    }

    public FBattleEffectData(int value)
    {
        _nativeData = new BattleEffectNativeData() { IntValue = value };
        Type = EBattleEffectDataType.Int32;
    }

    public FBattleEffectData(float value)
    {
        _nativeData = new BattleEffectNativeData() { FloatValue = value };
        Type = EBattleEffectDataType.Int32;
    }

    public FBattleEffectData(FName value)
    {
        _nativeData = new BattleEffectNativeData() { NameValue = value };
        Type = EBattleEffectDataType.Int32;
    }
}
