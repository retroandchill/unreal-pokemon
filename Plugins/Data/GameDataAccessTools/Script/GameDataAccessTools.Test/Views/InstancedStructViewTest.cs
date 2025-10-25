using GameDataAccessTools.Core.Utilities;
using GameDataAccessTools.Core.Views;
using GameDataAccessTools.Test.Utilities;
using JetBrains.Annotations;
using NUnit.Framework;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;

namespace GameDataAccessTools.Test.Views;

[UStruct]
public readonly partial record struct FTestStruct
{
    [UProperty]
    public int Value { get; init; }

    [UProperty]
    public string? Name { get; init; }
}

[UStruct]
public readonly partial record struct FAlternativeStruct
{
    [UProperty]
    public float FloatValue { get; init; }

    [UProperty]
    public bool BoolValue { get; init; }
}

[UClass]
[UsedImplicitly]
public partial class UInstancedStructViewWrapper : UObject
{
    [UProperty]
    public partial FInstancedStruct Value { get; set; }
}

public class InstancedStructViewTest
{
    private static readonly FTestStruct TestStructValue = new() { Value = 42, Name = "Test" };
    private static readonly FAlternativeStruct AlternativeStructValue = new() { FloatValue = 3.14f, BoolValue = true };

    private static UInstancedStructViewWrapper CreateWrapper(object structValue)
    {
        var wrapper = UObject.NewObject<UInstancedStructViewWrapper>();

        wrapper.Value = structValue switch
        {
            // Initialize with the provided struct value
            FTestStruct testStruct => FInstancedStruct.Make(testStruct),
            FAlternativeStruct alternativeStruct => FInstancedStruct.Make(alternativeStruct),
            _ => wrapper.Value,
        };

        return wrapper;
    }

    private static StructView<FInstancedStruct> GetInstancedStructView(UInstancedStructViewWrapper wrapper)
    {
        return wrapper.GetView(x => x.Value);
    }

    [Test]
    public void GetStructTypeInformation()
    {
        var wrapper = CreateWrapper(TestStructValue);
        var view = GetInstancedStructView(wrapper);

        using (Assert.EnterMultipleScope())
        {
            var structType = view.StructType;
            Assert.That(structType, Is.Not.Null);

            // Test the extension method on FInstancedStruct directly
            var directStructType = wrapper.Value.StructType;
            Assert.That(directStructType, Is.EqualTo(structType));
        }
    }

    [Test]
    public void IsAOperationWithScriptStruct()
    {
        var wrapper = CreateWrapper(TestStructValue);
        var view = GetInstancedStructView(wrapper);

        var testScriptStruct = FTestStruct.StaticStruct;
        var alternativeScriptStruct = FAlternativeStruct.StaticStruct;

        using (Assert.EnterMultipleScope())
        {
            Assert.That(view.IsA(testScriptStruct), Is.True);
            Assert.That(view.IsA(alternativeScriptStruct), Is.False);
        }
    }

    [Test]
    public void IsAOperationWithGenericType()
    {
        var wrapper = CreateWrapper(TestStructValue);
        var view = GetInstancedStructView(wrapper);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(view.IsA<FTestStruct>(), Is.True);
            Assert.That(view.IsA<FAlternativeStruct>(), Is.False);
            Assert.That(view.IsA<FVector>(), Is.False);
        }
    }

    [Test]
    public void GetOperationSucceedsWithCorrectType()
    {
        var wrapper = CreateWrapper(TestStructValue);
        var view = GetInstancedStructView(wrapper);

        var retrievedStruct = view.Get<FTestStruct>();

        using (Assert.EnterMultipleScope())
        {
            Assert.That(retrievedStruct.Value, Is.EqualTo(TestStructValue.Value));
            Assert.That(retrievedStruct.Name, Is.EqualTo(TestStructValue.Name));
        }
    }

    [Test]
    public void GetOperationThrowsWithIncorrectType()
    {
        var wrapper = CreateWrapper(TestStructValue);

        Assert.Throws<InvalidOperationException>(() =>
        {
            var view = GetInstancedStructView(wrapper);
            view.Get<FAlternativeStruct>();
        });

        Assert.Throws<InvalidOperationException>(() =>
        {
            var view = GetInstancedStructView(wrapper);
            view.Get<FVector>();
        });
    }

    [Test]
    public void TryGetOperationSucceedsWithCorrectType()
    {
        var wrapper = CreateWrapper(TestStructValue);
        var view = GetInstancedStructView(wrapper);

        var success = view.TryGet<FTestStruct>(out var retrievedStruct);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(success, Is.True);
            Assert.That(retrievedStruct.Value, Is.EqualTo(TestStructValue.Value));
            Assert.That(retrievedStruct.Name, Is.EqualTo(TestStructValue.Name));
        }
    }

    [Test]
    public void TryGetOperationFailsWithIncorrectType()
    {
        var wrapper = CreateWrapper(TestStructValue);
        var view = GetInstancedStructView(wrapper);

        using (Assert.EnterMultipleScope())
        {
            var success1 = view.TryGet<FAlternativeStruct>(out var altStruct);
            Assert.That(success1, Is.False);
            Assert.That(altStruct, Is.EqualTo(default(FAlternativeStruct)));

            var success2 = view.TryGet<FVector>(out var vector);
            Assert.That(success2, Is.False);
            Assert.That(vector, Is.EqualTo(default(FVector)));
        }
    }

    [Test]
    public void TypeConsistencyAcrossDifferentStructTypes()
    {
        var testWrapper = CreateWrapper(TestStructValue);
        var testView = GetInstancedStructView(testWrapper);

        var altWrapper = CreateWrapper(AlternativeStructValue);
        var altView = GetInstancedStructView(altWrapper);

        using (Assert.EnterMultipleScope())
        {
            // Test wrapper should only match FTestStruct
            Assert.That(testView.IsA<FTestStruct>(), Is.True);
            Assert.That(testView.IsA<FAlternativeStruct>(), Is.False);

            // Alternative wrapper should only match FAlternativeStruct
            Assert.That(altView.IsA<FAlternativeStruct>(), Is.True);
            Assert.That(altView.IsA<FTestStruct>(), Is.False);
        }
    }

    [Test]
    public void StructTypePropertyConsistency()
    {
        var testWrapper = CreateWrapper(TestStructValue);
        var testView = GetInstancedStructView(testWrapper);

        var altWrapper = CreateWrapper(AlternativeStructValue);
        var altView = GetInstancedStructView(altWrapper);

        using (Assert.EnterMultipleScope())
        {
            var testStructType = testView.StructType;
            var altStructType = altView.StructType;

            Assert.That(testStructType, Is.Not.Null);
            Assert.That(altStructType, Is.Not.Null);
            Assert.That(testStructType, Is.Not.EqualTo(altStructType));

            // Verify consistency with static struct references
            Assert.That(testStructType, Is.EqualTo(FTestStruct.StaticStruct));
            Assert.That(altStructType, Is.EqualTo(FAlternativeStruct.StaticStruct));
        }
    }

    [Test]
    public void ViewUpdatesWithInstancedStructChanges()
    {
        var wrapper = CreateWrapper(TestStructValue);
        var view = GetInstancedStructView(wrapper);

        // Initial state verification
        Assert.That(view.IsA<FTestStruct>(), Is.True);
        var initialStruct = view.Get<FTestStruct>();
        Assert.That(initialStruct.Value, Is.EqualTo(TestStructValue.Value));

        // Change the instanced struct to a different type
        wrapper = CreateWrapper(AlternativeStructValue);
        view = GetInstancedStructView(wrapper);

        // Verify the view now reflects the new struct type
        using (Assert.EnterMultipleScope())
        {
            Assert.That(view.IsA<FAlternativeStruct>(), Is.True);
            Assert.That(view.IsA<FTestStruct>(), Is.False);

            var newStruct = view.Get<FAlternativeStruct>();
            Assert.That(newStruct.FloatValue, Is.EqualTo(AlternativeStructValue.FloatValue));
            Assert.That(newStruct.BoolValue, Is.EqualTo(AlternativeStructValue.BoolValue));
        }
    }

    [Test]
    public void EmptyInstancedStructHandling()
    {
        var wrapper = UObject.NewObject<UInstancedStructViewWrapper>();
        wrapper.Value = new FInstancedStruct(); // Empty/default instanced struct
        var view = GetInstancedStructView(wrapper);

        using (Assert.EnterMultipleScope())
        {
            // Verify that operations on empty instanced struct behave correctly
            var structType = view.StructType;

            // Empty instanced structs should have null struct type
            Assert.That(structType, Is.Null);

            // IsA operations should return false for empty structs
            Assert.That(view.IsA<FTestStruct>(), Is.False);
            Assert.That(view.IsA<FAlternativeStruct>(), Is.False);
            Assert.That(view.IsA<FVector>(), Is.False);

            // TryGet operations should fail for empty structs
            Assert.That(view.TryGet<FTestStruct>(out _), Is.False);
            Assert.That(view.TryGet<FAlternativeStruct>(out _), Is.False);
        }
    }
}
