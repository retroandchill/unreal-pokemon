using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using NUnit.Framework;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.Test.Runner;

namespace GameDataAccessTools.Test;

[UStruct]
[CreateStructView]
public readonly partial struct FTestGameEntry : IGameDataEntry
{
    [field: UProperty]
    public required FName ID { get; init; }

    [field: UProperty]
    public int RowIndex { get; init; }

    [UProperty]
    public required int Value { get; init; }

    [UProperty]
    public required bool IsActive { get; init; }
}

[UClass]
[GameDataRepository<FTestGameEntry>]
public partial class UTestGameDataRepository : UStaticGameDataRepository;

public class GameDataRepositoryTest
{
    private TStrongObjectPtr<UTestGameDataRepository> _repository;

    [SetUp]
    public void SetUp()
    {
        var repository = UObject.NewObject<UTestGameDataRepository>();
        _repository = new TStrongObjectPtr<UTestGameDataRepository>(repository);

        repository.RegisterEntry(
            new FTestGameEntry
            {
                ID = "test_item_1",
                Value = 100,
                IsActive = true,
            }
        );
        repository.RegisterEntry(
            new FTestGameEntry
            {
                ID = "test_item_2",
                Value = 200,
                IsActive = true,
            }
        );
        repository.RegisterEntry(
            new FTestGameEntry
            {
                ID = "invalid_item",
                Value = -1,
                IsActive = false,
            }
        );
    }

    [TearDown]
    public void TearDown()
    {
        _repository.Dispose();
    }

    [Test]
    public void GetEntryFromRepository()
    {
        Assert.That(_repository.Value, Is.Not.Null);

        var entry1 = _repository.Value.GetEntry("test_item_1");
        Assert.That(entry1.ID, Is.EqualTo(new FName("test_item_1")));
        Assert.That(entry1.RowIndex, Is.EqualTo(0));
        Assert.That(entry1.Value, Is.EqualTo(100));
        Assert.That(entry1.IsActive, Is.True);

        var entry2 = _repository.Value.GetEntry("test_item_2");
        Assert.That(entry2.ID, Is.EqualTo(new FName("test_item_2")));
        Assert.That(entry2.RowIndex, Is.EqualTo(1));
        Assert.That(entry2.Value, Is.EqualTo(200));
        Assert.That(entry2.IsActive, Is.True);

        var entry3 = _repository.Value.GetEntry("invalid_item");
        Assert.That(entry3.ID, Is.EqualTo(new FName("invalid_item")));
        Assert.That(entry3.RowIndex, Is.EqualTo(2));
        Assert.That(entry3.Value, Is.EqualTo(-1));
        Assert.That(entry3.IsActive, Is.False);
    }

    [Test]
    public void RepeatEntryIdsNotAllowed()
    {
        Assert.That(_repository.Value, Is.Not.Null);

        var sizeBefore = _repository.Value.NumEntries;
        Expect.Error("Cannot use ID 'test_item_1', as it is already in use", occurrences: 2);
        Expect.Error("ID 'test_item_1' is already in use");
        Assert.Throws<InvalidOperationException>(() =>
        {
            _repository.Value.RegisterEntry(
                new FTestGameEntry
                {
                    ID = "test_item_1",
                    Value = 100,
                    IsActive = true,
                }
            );
        });

        Assert.That(
            _repository.Value.TryRegisterEntry(
                new FTestGameEntry
                {
                    ID = "test_item_1",
                    Value = 100,
                    IsActive = true,
                }
            ),
            Is.False
        );

        Assert.That(_repository.Value.NumEntries, Is.EqualTo(sizeBefore));
    }

    [Test]
    public void CanRemoveEntries()
    {
        Assert.That(_repository.Value, Is.Not.Null);

        var sizeBefore = _repository.Value.NumEntries;
        _repository.Value.UnregisterEntry("test_item_1");
        Assert.That(_repository.Value.NumEntries, Is.EqualTo(sizeBefore - 1));

        Assert.That(_repository.Value.TryUnregisterEntry("test_item_2"), Is.True);
        Assert.That(_repository.Value.NumEntries, Is.EqualTo(sizeBefore - 2));

        Assert.Throws<InvalidOperationException>(() =>
        {
            _repository.Value.UnregisterEntry("test_item_1");
        });
        Assert.That(_repository.Value.TryUnregisterEntry("test_item_1"), Is.False);
    }
}
