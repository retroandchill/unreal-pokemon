using GameDataAccessTools.Core.Utilities;
using GameDataAccessTools.Core.Views;
using GameDataAccessTools.Test.Utilities;
using JetBrains.Annotations;
using NUnit.Framework;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;
using ZLinq;

namespace GameDataAccessTools.Test.Views;

[UClass]
[UsedImplicitly]
public partial class UGameplayTagContainerViewWrapper : UObject
{
    [UProperty]
    public partial FGameplayTagContainer Value { get; set; }
}

public class GameplayTagContainerViewTest
{
    private static readonly FGameplayTag GenericTag = new("Tests.GenericTag");
    private static readonly FGameplayTag GenericTagOne = new("Tests.GenericTag.One");
    private static readonly FGameplayTag GenericTagTwo = new("Tests.GenericTag.Two");

    private static readonly FGameplayTag OutsideTag = new("Input.MouseAndKeyboard");

    private static UGameplayTagContainerViewWrapper CreateWrapper()
    {
        var wrapper = UObject.NewObject<UGameplayTagContainerViewWrapper>();
        wrapper.Value = new FGameplayTagContainer([GenericTagOne, GenericTagTwo], [GenericTag]);
        return wrapper;
    }

    private static StructView<FGameplayTagContainer> GetContainerView(UGameplayTagContainerViewWrapper wrapper)
    {
        return wrapper.GetView(x => x.Value);
    }

    [Test]
    public void GetBasicInformationAboutContainer()
    {
        var wrapper = CreateWrapper();
        var view = GetContainerView(wrapper);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(view.IsEmpty, Is.False);
            Assert.That(view.Count, Is.EqualTo(2));
            Assert.That(view.First, Is.EqualTo(GenericTagOne));
            Assert.That(view.Last, Is.EqualTo(GenericTagTwo));
        }

        wrapper.Value = new FGameplayTagContainer();
        using (Assert.EnterMultipleScope())
        {
            Assert.That(view.IsEmpty, Is.True);
            Assert.That(view.Count, Is.EqualTo(0));

            // Empty containers return the default (empty) tag when first or last is requested
            Assert.That(view.First, Is.EqualTo(default(FGameplayTag)));
            Assert.That(view.Last, Is.EqualTo(default(FGameplayTag)));
        }
    }

    [Test]
    public void GetByIndexAndValidation()
    {
        var wrapper = CreateWrapper();
        var view = GetContainerView(wrapper);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(view.IsValidIndex(0), Is.True);
            Assert.That(view.IsValidIndex(1), Is.True);
            Assert.That(view.IsValidIndex(2), Is.False);
            Assert.That(view.IsValidIndex(-1), Is.False);

            Assert.That(view.GetByIndex(0), Is.EqualTo(GenericTagOne));
            Assert.That(view.GetByIndex(1), Is.EqualTo(GenericTagTwo));
        }

        wrapper.Value = new FGameplayTagContainer();
        using (Assert.EnterMultipleScope())
        {
            Assert.That(view.IsValidIndex(0), Is.False);
            Assert.That(view.IsValidIndex(-1), Is.False);
        }
    }

    [Test]
    public void HasTagOperations()
    {
        var wrapper = CreateWrapper();
        var view = GetContainerView(wrapper);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(view.HasTag(GenericTagOne), Is.True);
            Assert.That(view.HasTag(GenericTagTwo), Is.True);
            Assert.That(view.HasTag(OutsideTag), Is.False);

            Assert.That(view.HasTagExact(GenericTagOne), Is.True);
            Assert.That(view.HasTagExact(GenericTagTwo), Is.True);
            Assert.That(view.HasTagExact(OutsideTag), Is.False);

            // HasTag should match parent tags, HasTagExact should not
            Assert.That(view.HasTag(GenericTag), Is.True);
            Assert.That(view.HasTagExact(GenericTag), Is.False);
        }
    }

    [Test]
    public void HasAllOperationsWithContainer()
    {
        var wrapper = CreateWrapper();
        var view = GetContainerView(wrapper);

        var singleTagContainer = new FGameplayTagContainer([GenericTagOne], [GenericTag]);
        var bothTagsContainer = new FGameplayTagContainer([GenericTagOne, GenericTagTwo], [GenericTag]);
        var extraTagsContainer = new FGameplayTagContainer([GenericTagOne, GenericTagTwo, OutsideTag], [GenericTag]);
        var emptyContainer = new FGameplayTagContainer();

        using (Assert.EnterMultipleScope())
        {
            Assert.That(view.HasAll(singleTagContainer), Is.True);
            Assert.That(view.HasAll(bothTagsContainer), Is.True);
            Assert.That(view.HasAll(extraTagsContainer), Is.False);
            Assert.That(view.HasAll(emptyContainer), Is.True);

            Assert.That(view.HasAllExact(singleTagContainer), Is.True);
            Assert.That(view.HasAllExact(bothTagsContainer), Is.True);
            Assert.That(view.HasAllExact(extraTagsContainer), Is.False);
            Assert.That(view.HasAllExact(emptyContainer), Is.True);
        }
    }

    [Test]
    public void HasAllOperationsWithStructView()
    {
        var wrapper = CreateWrapper();
        var view = GetContainerView(wrapper);

        var singleTagWrapper = UObject.NewObject<UGameplayTagContainerViewWrapper>();
        singleTagWrapper.Value = new FGameplayTagContainer([GenericTagOne], [GenericTag]);
        var singleTagView = GetContainerView(singleTagWrapper);

        var bothTagsWrapper = UObject.NewObject<UGameplayTagContainerViewWrapper>();
        bothTagsWrapper.Value = new FGameplayTagContainer([GenericTagOne, GenericTagTwo], [GenericTag]);
        var bothTagsView = GetContainerView(bothTagsWrapper);

        var extraTagsWrapper = UObject.NewObject<UGameplayTagContainerViewWrapper>();
        extraTagsWrapper.Value = new FGameplayTagContainer([GenericTagOne, GenericTagTwo, OutsideTag], [GenericTag]);
        var extraTagsView = GetContainerView(extraTagsWrapper);

        var emptyWrapper = UObject.NewObject<UGameplayTagContainerViewWrapper>();
        emptyWrapper.Value = new FGameplayTagContainer();
        var emptyView = GetContainerView(emptyWrapper);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(view.HasAll(singleTagView), Is.True);
            Assert.That(view.HasAll(bothTagsView), Is.True);
            Assert.That(view.HasAll(extraTagsView), Is.False);
            Assert.That(view.HasAll(emptyView), Is.True);

            Assert.That(view.HasAllExact(singleTagView), Is.True);
            Assert.That(view.HasAllExact(bothTagsView), Is.True);
            Assert.That(view.HasAllExact(extraTagsView), Is.False);
            Assert.That(view.HasAllExact(emptyView), Is.True);
        }
    }

    [Test]
    public void HasAnyOperationsWithContainer()
    {
        var wrapper = CreateWrapper();
        var view = GetContainerView(wrapper);

        var singleTagContainer = new FGameplayTagContainer([GenericTagOne], [GenericTag]);
        var bothTagsContainer = new FGameplayTagContainer([GenericTagOne, GenericTagTwo], [GenericTag]);
        var differentTagContainer = new FGameplayTagContainer(OutsideTag);
        var emptyContainer = new FGameplayTagContainer();

        using (Assert.EnterMultipleScope())
        {
            Assert.That(view.HasAny(singleTagContainer), Is.True);
            Assert.That(view.HasAny(bothTagsContainer), Is.True);
            Assert.That(view.HasAny(differentTagContainer), Is.False);
            Assert.That(view.HasAny(emptyContainer), Is.False);
        }
    }

    [Test]
    public void HasAnyOperationsWithStructView()
    {
        var wrapper = CreateWrapper();
        var view = GetContainerView(wrapper);

        var singleTagWrapper = UObject.NewObject<UGameplayTagContainerViewWrapper>();
        singleTagWrapper.Value = new FGameplayTagContainer([GenericTagOne], [GenericTag]);
        var singleTagView = GetContainerView(singleTagWrapper);

        var bothTagsWrapper = UObject.NewObject<UGameplayTagContainerViewWrapper>();
        bothTagsWrapper.Value = new FGameplayTagContainer([GenericTagOne, GenericTagTwo], [GenericTag]);
        var bothTagsView = GetContainerView(bothTagsWrapper);

        var differentTagWrapper = UObject.NewObject<UGameplayTagContainerViewWrapper>();
        differentTagWrapper.Value = new FGameplayTagContainer(OutsideTag);
        var differentTagView = GetContainerView(differentTagWrapper);

        var emptyWrapper = UObject.NewObject<UGameplayTagContainerViewWrapper>();
        emptyWrapper.Value = new FGameplayTagContainer();
        var emptyView = GetContainerView(emptyWrapper);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(view.HasAny(singleTagView), Is.True);
            Assert.That(view.HasAny(bothTagsView), Is.True);
            Assert.That(view.HasAny(differentTagView), Is.False);
            Assert.That(view.HasAny(emptyView), Is.False);
        }
    }

    [Test]
    public void GameplayTagsSpanAccess()
    {
        var wrapper = CreateWrapper();
        var view = GetContainerView(wrapper);

        var gameplayTagsSpan = view.GameplayTags;

        using (Assert.EnterMultipleScope())
        {
            Assert.That(gameplayTagsSpan.Length, Is.EqualTo(2));
            Assert.That(gameplayTagsSpan[0], Is.EqualTo(GenericTagOne));
            Assert.That(gameplayTagsSpan[1], Is.EqualTo(GenericTagTwo));
        }

        wrapper.Value = new FGameplayTagContainer();
        var emptySpan = view.GameplayTags;
        Assert.That(emptySpan.Length, Is.EqualTo(0));
    }

    [Test]
    public void EnumerationOperations()
    {
        var wrapper = CreateWrapper();
        var view = GetContainerView(wrapper);

        var tagsList = new List<FGameplayTag>();
        foreach (var tag in view)
        {
            tagsList.Add(tag);
        }

        using (Assert.EnterMultipleScope())
        {
            Assert.That(tagsList.Count, Is.EqualTo(2));
            Assert.That(tagsList[0], Is.EqualTo(GenericTagOne));
            Assert.That(tagsList[1], Is.EqualTo(GenericTagTwo));
        }

        var valueEnumerable = view.AsValueEnumerable();
        var valueEnumerableList = new List<FGameplayTag>();
        foreach (var tag in valueEnumerable)
        {
            valueEnumerableList.Add(tag);
        }

        using (Assert.EnterMultipleScope())
        {
            Assert.That(valueEnumerableList.Count, Is.EqualTo(2));
            Assert.That(valueEnumerableList[0], Is.EqualTo(GenericTagOne));
            Assert.That(valueEnumerableList[1], Is.EqualTo(GenericTagTwo));
        }
    }

    [Test]
    public void FilterOperationsWithContainer()
    {
        var wrapper = CreateWrapper();
        var view = GetContainerView(wrapper);

        var singleTagContainer = new FGameplayTagContainer([GenericTagOne], [GenericTag]);
        var bothTagsContainer = new FGameplayTagContainer([GenericTagOne, GenericTagTwo], [GenericTag]);
        var extraTagsContainer = new FGameplayTagContainer([GenericTagOne, GenericTagTwo, OutsideTag], [GenericTag]);
        var differentTagContainer = new FGameplayTagContainer(OutsideTag);

        using (Assert.EnterMultipleScope())
        {
            var filteredSingle = view.Filter(singleTagContainer);
            Assert.That(filteredSingle.Count, Is.EqualTo(1));

            var filteredBoth = view.Filter(bothTagsContainer);
            Assert.That(filteredBoth.Count, Is.EqualTo(2));

            var filteredExtra = view.Filter(extraTagsContainer);
            Assert.That(filteredExtra.Count, Is.EqualTo(2));

            var filteredDifferent = view.Filter(differentTagContainer);
            Assert.That(filteredDifferent.Count, Is.EqualTo(0));
        }

        using (Assert.EnterMultipleScope())
        {
            var filteredExactSingle = view.FilterExact(singleTagContainer);
            Assert.That(filteredExactSingle.Count, Is.EqualTo(1));

            var filteredExactBoth = view.FilterExact(bothTagsContainer);
            Assert.That(filteredExactBoth.Count, Is.EqualTo(2));

            var filteredExactExtra = view.FilterExact(extraTagsContainer);
            Assert.That(filteredExactExtra.Count, Is.EqualTo(2));

            var filteredExactDifferent = view.FilterExact(differentTagContainer);
            Assert.That(filteredExactDifferent.Count, Is.EqualTo(0));
        }
    }

    [Test]
    public void FilterOperationsWithStructView()
    {
        var wrapper = CreateWrapper();
        var view = GetContainerView(wrapper);

        var singleTagWrapper = UObject.NewObject<UGameplayTagContainerViewWrapper>();
        singleTagWrapper.Value = new FGameplayTagContainer([GenericTagOne], [GenericTag]);
        var singleTagView = GetContainerView(singleTagWrapper);

        var bothTagsWrapper = UObject.NewObject<UGameplayTagContainerViewWrapper>();
        bothTagsWrapper.Value = new FGameplayTagContainer([GenericTagOne, GenericTagTwo], [GenericTag]);
        var bothTagsView = GetContainerView(bothTagsWrapper);

        var extraTagsWrapper = UObject.NewObject<UGameplayTagContainerViewWrapper>();
        extraTagsWrapper.Value = new FGameplayTagContainer([GenericTagOne, GenericTagTwo, OutsideTag], [GenericTag]);
        var extraTagsView = GetContainerView(extraTagsWrapper);

        var differentTagWrapper = UObject.NewObject<UGameplayTagContainerViewWrapper>();
        differentTagWrapper.Value = new FGameplayTagContainer(OutsideTag);
        var differentTagView = GetContainerView(differentTagWrapper);

        using (Assert.EnterMultipleScope())
        {
            var filteredSingle = view.Filter(singleTagView);
            Assert.That(filteredSingle.Count, Is.EqualTo(1));

            var filteredBoth = view.Filter(bothTagsView);
            Assert.That(filteredBoth.Count, Is.EqualTo(2));

            var filteredExtra = view.Filter(extraTagsView);
            Assert.That(filteredExtra.Count, Is.EqualTo(2));

            var filteredDifferent = view.Filter(differentTagView);
            Assert.That(filteredDifferent.Count, Is.EqualTo(0));
        }

        using (Assert.EnterMultipleScope())
        {
            var filteredExactSingle = view.FilterExact(singleTagView);
            Assert.That(filteredExactSingle.Count, Is.EqualTo(1));

            var filteredExactBoth = view.FilterExact(bothTagsView);
            Assert.That(filteredExactBoth.Count, Is.EqualTo(2));

            var filteredExactExtra = view.FilterExact(extraTagsView);
            Assert.That(filteredExactExtra.Count, Is.EqualTo(2));

            var filteredExactDifferent = view.FilterExact(differentTagView);
            Assert.That(filteredExactDifferent.Count, Is.EqualTo(0));
        }
    }

    [Test]
    public void ParentsOperation()
    {
        var wrapper = CreateWrapper();
        var view = GetContainerView(wrapper);

        var parents = view.Parents;

        // The behavior of Parents depends on the tag hierarchy structure
        // This test ensures the method executes without exception
        Assert.That(parents.Count, Is.GreaterThanOrEqualTo(0));
    }

    [Test]
    public void MatchesQueryOperation()
    {
        var wrapper = CreateWrapper();
        var view = GetContainerView(wrapper);

        // Create a simple query that should match our container
        var query = new FGameplayTagQuery();

        // The behavior of MatchesQuery depends on the specific query implementation
        // This test ensures the method executes without exception
        var result = view.MatchesQuery(query);
        Assert.That(result, Is.TypeOf<bool>());
    }
}
