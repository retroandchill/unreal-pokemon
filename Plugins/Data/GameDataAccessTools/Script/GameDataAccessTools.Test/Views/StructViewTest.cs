using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core.Utilities;
using GameDataAccessTools.Core.Views;
using GameDataAccessTools.Test.Utilities;
using JetBrains.Annotations;
using NUnit.Framework;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;

namespace GameDataAccessTools.Test.Views;

[UStruct]
[CreateStructView]
public readonly partial record struct FSimpleViewableStruct
{
    [UProperty]
    [UsedImplicitly]
    public int Value { get; init; }

    [UProperty]
    [UsedImplicitly]
    public bool IsActive { get; init; }

    [UProperty]
    [UsedImplicitly]
    public string? Name { get; init; }
}

[UClass]
[UsedImplicitly]
public partial class UTestStructView : UObject
{
    [UProperty]
    public partial FSimpleViewableStruct Value { get; set; }
}

public class StructViewTest
{
    [Test]
    public void ViewUpdatesWithStructChanges()
    {
        var instance = UObject.NewObject<UTestStructView>();
        instance.Value = new FSimpleViewableStruct
        {
            Value = 1,
            IsActive = true,
            Name = "Test",
        };

        var view = instance.GetView(x => x.Value);
        using (Assert.EnterMultipleScope())
        {
            Assert.That(view.Value, Is.EqualTo(1));
            Assert.That(view.IsActive, Is.True);
            Assert.That(view.Name, Is.EqualTo("Test"));
        }

        instance.Value = instance.Value with { Value = 2 };
        Assert.That(view.Value, Is.EqualTo(2));
    }

    [Test]
    public unsafe void MarshallToAndFromNative()
    {
        var vector = new FVector(1, 2, 3);

        var nativeVector = stackalloc byte[FVector.GetNativeDataSize()];
        vector.ToNative((IntPtr)nativeVector);

        var structView = StructView<FVector>.FromNative((IntPtr)nativeVector);
        Assert.That(vector, Is.EqualTo(structView.Copy()));

        var secondVector = stackalloc byte[FVector.GetNativeDataSize()];
        structView.ToNative((IntPtr)secondVector);

        Assert.That(vector, Is.EqualTo(StructView<FVector>.FromNative((IntPtr)secondVector)));
    }

    [Test]
    public unsafe void ImplicitConversionMarshallsFromNative()
    {
        var vector = new FVector(0, 4, 90);

        var nativeVector = stackalloc byte[FVector.GetNativeDataSize()];
        vector.ToNative((IntPtr)nativeVector);

        var structView = StructView<FVector>.FromNative((IntPtr)nativeVector);
        var marshalled = FVector.FromNative((IntPtr)nativeVector);
        FVector converted = structView;

        Assert.That(converted, Is.EqualTo(marshalled));
    }

    [Test]
    public void StaticStructPropertyGetsStaticStruct()
    {
        Assert.That(FVector.StaticStruct, Is.EqualTo(StructView<FVector>.ScriptStruct));
    }
}
