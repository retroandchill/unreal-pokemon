using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core.Views;
using GameDataAccessTools.Test.Utilities;
using JetBrains.Annotations;
using NUnit.Framework;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using ZLinq;

namespace GameDataAccessTools.Test.Views;

[UStruct]
[CreateStructView]
public readonly partial record struct FCustomStruct
{
    [field: UProperty]
    public float Value { get; init; }
    
    [field: UProperty]
    public int Count { get; init; }
    
    [field: UProperty]
    [UsedImplicitly]
    public bool IsEnabled { get; init; }
}

[UStruct]
[CreateStructView]
public readonly partial record struct FCollectionTestStruct
{
    [field: UProperty]
    [UsedImplicitly]
    public IReadOnlyList<int> IntList { get; init; }
    
    [field: UProperty]
    [UsedImplicitly]
    public IReadOnlyList<FName> NameList { get; init; }
    
    [field: UProperty]
    [UsedImplicitly]
    public IReadOnlyList<FVector> VectorList { get; init; }
    
    [field: UProperty]
    [UsedImplicitly]
    public IReadOnlyList<FCustomStruct> CustomStructList { get; init; }
    
    [field: UProperty]
    [UsedImplicitly]
    public IReadOnlyList<string> StringList { get; init; }
}

[UClass]
[UsedImplicitly]
public class UCollectionTestWrapper : UObject
{
    [UProperty]
    public FCollectionTestStruct Value { get; set; }
}

public class ArrayViewTest
{
    private static readonly int[] TestInts = [1, 2, 3, 42, 100];
    private static readonly FName[] TestNames = [new("FirstName"), new("SecondName"), new("ThirdName")];
    private static readonly FVector[] TestVectors = [new(1, 0, 0), new(0, 1, 0), new(0, 0, 1), new(1, 1, 1)];
    private static readonly FCustomStruct[] TestCustomStructs = [
        new() { Value = 1.5f, Count = 10, IsEnabled = true },
        new() { Value = 2.5f, Count = 20, IsEnabled = false },
        new() { Value = 3.5f, Count = 30, IsEnabled = true }
    ];
    private static readonly string[] TestStrings = ["Hello", "World", "Test", "Collection"];

    private static UCollectionTestWrapper CreateWrapper()
    {
        var wrapper = UObject.NewObject<UCollectionTestWrapper>();
        wrapper.Value = new FCollectionTestStruct
        {
            IntList = TestInts,
            NameList = TestNames,
            VectorList = TestVectors,
            CustomStructList = TestCustomStructs,
            StringList = TestStrings
        };
        return wrapper;
    }

    private static StructView<FCollectionTestStruct> GetStructView(UCollectionTestWrapper wrapper)
    {
        return wrapper.GetView(x => x.Value);
    }

    [Test]
    public void IntListMarshalledAsSpan()
    {
        var wrapper = CreateWrapper();
        var view = GetStructView(wrapper);

        var intSpan = view.IntList;

        using (Assert.EnterMultipleScope())
        {
            Assert.That(intSpan.Length, Is.EqualTo(TestInts.Length));

            // Verify all values are correct
            for (int i = 0; i < TestInts.Length; i++)
            {
                Assert.That(intSpan[i], Is.EqualTo(TestInts[i]));
            }
        }
    }

    [Test]
    public void NameListMarshalledAsSpan()
    {
        var wrapper = CreateWrapper();
        var view = GetStructView(wrapper);

        var nameSpan = view.NameList;

        using (Assert.EnterMultipleScope())
        {
            Assert.That(nameSpan.Length, Is.EqualTo(TestNames.Length));

            // Verify all values are correct
            for (int i = 0; i < TestNames.Length; i++)
            {
                Assert.That(nameSpan[i], Is.EqualTo(TestNames[i]));
            }
        }
    }

    [Test]
    public void VectorListMarshalledAsSpan()
    {
        var wrapper = CreateWrapper();
        var view = GetStructView(wrapper);

        var vectorSpan = view.VectorList;

        using (Assert.EnterMultipleScope())
        {
            Assert.That(vectorSpan.Length, Is.EqualTo(TestVectors.Length));

            // Verify all values are correct
            for (int i = 0; i < TestVectors.Length; i++)
            {
                Assert.That(vectorSpan[i], Is.EqualTo(TestVectors[i]));
            }
        }
    }

    [Test]
    public void CustomStructListMarshalledAsArrayView()
    {
        var wrapper = CreateWrapper();
        var view = GetStructView(wrapper);

        var customStructArrayView = view.CustomStructList;

        using (Assert.EnterMultipleScope())
        {
            Assert.That(customStructArrayView.Length, Is.EqualTo(TestCustomStructs.Length));
            Assert.That(customStructArrayView.IsEmpty, Is.False);

            // Verify all values are correct
            for (int i = 0; i < TestCustomStructs.Length; i++)
            {
                var structView = customStructArrayView[i];
                Assert.That(structView.Value, Is.EqualTo(TestCustomStructs[i].Value));
                Assert.That(structView.Count, Is.EqualTo(TestCustomStructs[i].Count));
            }
        }
    }

    [Test]
    public void StringListMarshalledAsArrayView()
    {
        var wrapper = CreateWrapper();
        var view = GetStructView(wrapper);

        var stringArrayView = view.StringList;

        using (Assert.EnterMultipleScope())
        {
            Assert.That(stringArrayView.Length, Is.EqualTo(TestStrings.Length));
            Assert.That(stringArrayView.IsEmpty, Is.False);

            // Verify all values are correct
            for (int i = 0; i < TestStrings.Length; i++)
            {
                Assert.That(stringArrayView[i], Is.EqualTo(TestStrings[i]));
            }
        }
    }

    [Test]
    public void EmptyCollectionsHandling()
    {
        var wrapper = UObject.NewObject<UCollectionTestWrapper>();
        wrapper.Value = new FCollectionTestStruct
        {
            IntList = new List<int>(),
            NameList = new List<FName>(),
            VectorList = new List<FVector>(),
            CustomStructList = new List<FCustomStruct>(),
            StringList = new List<string>()
        };

        var view = GetStructView(wrapper);

        using (Assert.EnterMultipleScope())
        {
            // All spans should have length 0
            Assert.That(view.IntList.Length, Is.EqualTo(0));
            Assert.That(view.NameList.Length, Is.EqualTo(0));
            Assert.That(view.VectorList.Length, Is.EqualTo(0));

            // All array views should be empty
            Assert.That(view.CustomStructList.Length, Is.EqualTo(0));
            Assert.That(view.CustomStructList.IsEmpty, Is.True);
            Assert.That(view.StringList.Length, Is.EqualTo(0));
            Assert.That(view.StringList.IsEmpty, Is.True);
        }
    }

    [Test]
    public void ArrayViewEnumerationForCustomStructs()
    {
        var wrapper = CreateWrapper();
        var view = GetStructView(wrapper);

        var customStructArrayView = view.CustomStructList;
        var enumeratedStructs = new List<FCustomStruct>();

        foreach (var structView in customStructArrayView)
        {
            enumeratedStructs.Add(structView.Copy());
        }

        using (Assert.EnterMultipleScope())
        {
            Assert.That(enumeratedStructs, Has.Count.EqualTo(TestCustomStructs.Length));
            
            for (var i = 0; i < TestCustomStructs.Length; i++)
            {
                Assert.That(enumeratedStructs[i].Value, Is.EqualTo(TestCustomStructs[i].Value));
                Assert.That(enumeratedStructs[i].Count, Is.EqualTo(TestCustomStructs[i].Count));
            }
        }
    }

    [Test]
    public void ArrayViewEnumerationForStrings()
    {
        var wrapper = CreateWrapper();
        var view = GetStructView(wrapper);

        var stringArrayView = view.StringList;
        var enumeratedStrings = new List<string>();

        foreach (var str in stringArrayView)
        {
            enumeratedStrings.Add(str);
        }

        using (Assert.EnterMultipleScope())
        {
            Assert.That(enumeratedStrings.Count, Is.EqualTo(TestStrings.Length));
            
            for (int i = 0; i < TestStrings.Length; i++)
            {
                Assert.That(enumeratedStrings[i], Is.EqualTo(TestStrings[i]));
            }
        }
    }

    [Test]
    public void ArrayViewValueEnumerableForCustomStructs()
    {
        var wrapper = CreateWrapper();
        var view = GetStructView(wrapper);

        var customStructArrayView = view.CustomStructList;
        var valueEnumerable = customStructArrayView.AsValueEnumerable();

        var enumeratedStructs = new List<FCustomStruct>();
        foreach (var customStruct in valueEnumerable)
        {
            enumeratedStructs.Add(customStruct);
        }

        using (Assert.EnterMultipleScope())
        {
            Assert.That(enumeratedStructs.Count, Is.EqualTo(TestCustomStructs.Length));
            
            for (int i = 0; i < TestCustomStructs.Length; i++)
            {
                Assert.That(enumeratedStructs[i].Value, Is.EqualTo(TestCustomStructs[i].Value));
                Assert.That(enumeratedStructs[i].Count, Is.EqualTo(TestCustomStructs[i].Count));
            }
        }
    }

    [Test]
    public void ArrayViewValueEnumerableForStrings()
    {
        var wrapper = CreateWrapper();
        var view = GetStructView(wrapper);

        var stringArrayView = view.StringList;
        var valueEnumerable = stringArrayView.AsValueEnumerable();

        var enumeratedStrings = new List<string>();
        foreach (var str in valueEnumerable)
        {
            enumeratedStrings.Add(str);
        }

        using (Assert.EnterMultipleScope())
        {
            Assert.That(enumeratedStrings.Count, Is.EqualTo(TestStrings.Length));
            
            for (int i = 0; i < TestStrings.Length; i++)
            {
                Assert.That(enumeratedStrings[i], Is.EqualTo(TestStrings[i]));
            }
        }
    }

    [Test]
    public void ArrayViewIndexerAccess()
    {
        var wrapper = CreateWrapper();
        var view = GetStructView(wrapper);

        using (Assert.EnterMultipleScope())
        {
            // Test custom struct array view indexer
            var customStructArrayView = view.CustomStructList;
            for (int i = 0; i < TestCustomStructs.Length; i++)
            {
                var structView = customStructArrayView[i];
                Assert.That(structView.Value, Is.EqualTo(TestCustomStructs[i].Value));
                Assert.That(structView.Count, Is.EqualTo(TestCustomStructs[i].Count));
            }

            // Test string array view indexer
            var stringArrayView = view.StringList;
            for (int i = 0; i < TestStrings.Length; i++)
            {
                Assert.That(stringArrayView[i], Is.EqualTo(TestStrings[i]));
            }
        }
    }

    [Test]
    public void ArrayViewBoundsChecking()
    {
        var wrapper = CreateWrapper();

        using (Assert.EnterMultipleScope())
        {
            // Test negative index throws
            Assert.Throws<ArgumentOutOfRangeException>(() =>
            {
                var view = GetStructView(wrapper);
                var customStructArrayView = view.CustomStructList;
                
                _ = customStructArrayView[-1];
            });
            Assert.Throws<ArgumentOutOfRangeException>(() =>
            {
                var view = GetStructView(wrapper);
                var stringArrayView = view.StringList;
                
                _ = stringArrayView[-1];
            });

            // Test out of bounds index throws
            Assert.Throws<ArgumentOutOfRangeException>(() =>
            {
                var view = GetStructView(wrapper);
                var customStructArrayView = view.CustomStructList;
                
                _ = customStructArrayView[TestCustomStructs.Length];
            });
            Assert.Throws<ArgumentOutOfRangeException>(() =>
            {
                var view = GetStructView(wrapper);
                var stringArrayView = view.StringList;
                
                _ = stringArrayView[TestStrings.Length];
            });
        }
    }

    [Test]
    public void ViewUpdatesWithCollectionChanges()
    {
        var wrapper = CreateWrapper();
        var view = GetStructView(wrapper);

        // Initial verification
        Assert.That(view.IntList.Length, Is.EqualTo(TestInts.Length));
        Assert.That(view.StringList.Length, Is.EqualTo(TestStrings.Length));

        // Update the wrapper with different data
        wrapper.Value = new FCollectionTestStruct
        {
            IntList = new List<int>([999]),
            NameList = new List<FName>([new("NewName")]),
            VectorList = new List<FVector>([new(10, 20, 30)]),
            CustomStructList = new List<FCustomStruct>([new() { Value = 99.9f, Count = 999 }]),
            StringList = new List<string>(["UpdatedString"])
        };

        // Verify the view reflects the changes
        using (Assert.EnterMultipleScope())
        {
            Assert.That(view.IntList.Length, Is.EqualTo(1));
            Assert.That(view.IntList[0], Is.EqualTo(999));

            Assert.That(view.NameList.Length, Is.EqualTo(1));
            Assert.That(view.NameList[0], Is.EqualTo(new FName("NewName")));

            Assert.That(view.VectorList.Length, Is.EqualTo(1));
            Assert.That(view.VectorList[0], Is.EqualTo(new FVector(10, 20, 30)));

            Assert.That(view.CustomStructList.Length, Is.EqualTo(1));
            var updatedStruct = view.CustomStructList[0];
            Assert.That(updatedStruct.Value, Is.EqualTo(99.9f));
            Assert.That(updatedStruct.Count, Is.EqualTo(999));

            Assert.That(view.StringList.Length, Is.EqualTo(1));
            Assert.That(view.StringList[0], Is.EqualTo("UpdatedString"));
        }
    }

    [Test]
    public void SpanSlicingOperations()
    {
        var wrapper = CreateWrapper();
        var view = GetStructView(wrapper);

        using (Assert.EnterMultipleScope())
        {
            // Test slicing int span
            var intSpan = view.IntList;
            var intSlice = intSpan.Slice(1, 3);
            Assert.That(intSlice.Length, Is.EqualTo(3));
            Assert.That(intSlice[0], Is.EqualTo(TestInts[1]));
            Assert.That(intSlice[1], Is.EqualTo(TestInts[2]));
            Assert.That(intSlice[2], Is.EqualTo(TestInts[3]));

            // Test slicing vector span
            var vectorSpan = view.VectorList;
            var vectorSlice = vectorSpan.Slice(0, 2);
            Assert.That(vectorSlice.Length, Is.EqualTo(2));
            Assert.That(vectorSlice[0], Is.EqualTo(TestVectors[0]));
            Assert.That(vectorSlice[1], Is.EqualTo(TestVectors[1]));
        }
    }
}