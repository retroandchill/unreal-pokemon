using GameDataAccessTools.Core.Marshallers;
using GameDataAccessTools.Core.Views;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;
using ZLinq;
using ZLinq.Linq;

namespace GameDataAccessTools.Core.Utilities;

/// <summary>
/// Provides extension methods for operations related to GameplayTagContainer objects.
/// </summary>
public static class GameplayTagContainerExtensions
{
    /// <summary>
    /// Extensions for FGameplayTagContainer.
    /// </summary>
    /// <param name="container">The container type</param>
    extension(StructView<FGameplayTagContainer> container)
    {
        /// <summary>
        /// Retrieves the first gameplay tag from the container.
        /// Provides access to the first tag in the tag container if available.
        /// </summary>
        /// <value>
        /// The first <see cref="FGameplayTag"/> in the container. If the container is empty, behavior may depend on implementation specifics.
        /// </value>
        public FGameplayTag First => UGameplayTagContainerBlueprintLibrary.First(container.NativePtr);

        /// <summary>
        /// Retrieves the last gameplay tag from the container.
        /// Provides access to the last tag in the tag container if available.
        /// </summary>
        /// <value>
        /// The last <see cref="FGameplayTag"/> in the container. If the container is empty, behavior may depend on implementation specifics.
        /// </value>
        public FGameplayTag Last => UGameplayTagContainerBlueprintLibrary.Last(container.NativePtr);

        /// <summary>
        /// Retrieves the total number of gameplay tags in the container.
        /// Provides access to the count of tags stored within the tag container.
        /// </summary>
        /// <value>
        /// The total number of gameplay tags present in this container. If the container is empty, the count will be zero.
        /// </value>
        public int Count => UGameplayTagContainerBlueprintLibrary.Num(container.NativePtr);

        /// <summary>
        /// Indicates whether the container contains any gameplay tags.
        /// Provides a boolean value that identifies if the tag container is empty or not.
        /// </summary>
        /// <value>
        /// True if the container does not contain any gameplay tags; otherwise, false.
        /// </value>
        public bool IsEmpty => UGameplayTagContainerBlueprintLibrary.IsEmpty(container.NativePtr);

        /// <summary>
        /// Provides a read-only view of the gameplay tags within the container.
        /// Allows enumeration or span-based access to all <see cref="FGameplayTag"/> elements contained in this view.
        /// </summary>
        /// <value>
        /// A read-only span of <see cref="FGameplayTag"/>, representing the gameplay tags in the container.
        /// The returned span is directly tied to the container's native data and may reflect changes in the underlying structure.
        /// </value>
        public ReadOnlySpan<FGameplayTag> GameplayTags
        {
            get
            {
                var arrayView = UGameplayTagContainerBlueprintLibrary.GetGameplayTagArray(container.NativePtr);
                return SpanMarshaller<FGameplayTag>.FromNative(arrayView.Ptr, 0);
            }
        }

        /// <summary>
        /// Retrieves the parent gameplay tags from the tag container.
        /// Provides access to all parent tags for a given tag or set of tags within the container.
        /// </summary>
        /// <value>
        /// A <see cref="FGameplayTagContainer"/> containing the parent tags associated with the tags in the container.
        /// If no parent tags are available, the container will be empty.
        /// </value>
        public FGameplayTagContainer Parents =>
            UGameplayTagContainerBlueprintLibrary.GetGameplayTagParents(container.NativePtr);

        /// <summary>
        /// Retrieves a gameplay tag at the specified index from the container.
        /// </summary>
        /// <param name="index">The zero-based index of the gameplay tag to retrieve.</param>
        /// <returns>The gameplay tag located at the specified index.</returns>
        public FGameplayTag GetByIndex(int index) =>
            UGameplayTagContainerBlueprintLibrary.GetByIndex(container.NativePtr, index);

        /// <summary>
        /// Determines whether the specified index is valid for the current gameplay tag container.
        /// </summary>
        /// <param name="index">The zero-based index to check.</param>
        /// <returns>True if the specified index is within the bounds of the gameplay tag container; otherwise, false.</returns>
        public bool IsValidIndex(int index) =>
            UGameplayTagContainerBlueprintLibrary.IsValidIndex(container.NativePtr, index);

        /// <summary>
        /// Determines whether the container includes all gameplay tags specified in another container.
        /// </summary>
        /// <param name="other">The gameplay tag container whose tags are to be checked against.</param>
        /// <returns>True if all the gameplay tags in the specified container are present in this container; otherwise, false.</returns>
        public bool HasAll(FGameplayTagContainer other) =>
            UGameplayTagContainerBlueprintLibrary.HasAll(container.NativePtr, other);

        /// <summary>
        /// Determines whether the current gameplay tag container contains all the gameplay tags present in the specified container.
        /// </summary>
        /// <param name="other">The gameplay tag container to compare against.</param>
        /// <returns>True if the current container contains all tags from the specified container; otherwise, false.</returns>
        public bool HasAll(StructView<FGameplayTagContainer> other) =>
            UGameplayTagContainerBlueprintLibrary.HasAllInView(container.NativePtr, other.NativePtr);

        /// <summary>
        /// Determines whether the container includes all gameplay tags from another container,
        /// ensuring an exact match for each tag.
        /// </summary>
        /// <param name="other">The container from which gameplay tags are compared.</param>
        /// <returns>True if the container contains all tags from the given container with an exact match; otherwise, false.</returns>
        public bool HasAllExact(FGameplayTagContainer other) =>
            UGameplayTagContainerBlueprintLibrary.HasAllExact(container.NativePtr, other);

        /// <summary>
        /// Checks if the container contains all exact gameplay tags from another container represented as a StructView.
        /// </summary>
        /// <param name="other">The other container, represented as a StructView, whose gameplay tags are to be checked.</param>
        /// <returns>True if the container contains all exact gameplay tags from the specified other container; otherwise, false.</returns>
        public bool HasAllExact(StructView<FGameplayTagContainer> other) =>
            UGameplayTagContainerBlueprintLibrary.HasAllExactInView(container.NativePtr, other.NativePtr);

        /// <summary>
        /// Checks if the container has at least one gameplay tag that exists in the specified gameplay tag container.
        /// </summary>
        /// <param name="other">The gameplay tag container to compare against.</param>
        /// <returns>True if the container has any tag from the specified container; otherwise, false.</returns>
        public bool HasAny(FGameplayTagContainer other) =>
            UGameplayTagContainerBlueprintLibrary.HasAny(container.NativePtr, other);

        /// <summary>
        /// Determines if the gameplay tag container contains any of the gameplay tags in the specified container.
        /// </summary>
        /// <param name="other">The container of gameplay tags to check against.</param>
        /// <returns>True if any gameplay tag in the specified container is present in this container; otherwise, false.</returns>
        public bool HasAny(StructView<FGameplayTagContainer> other) =>
            UGameplayTagContainerBlueprintLibrary.HasAnyInView(container.NativePtr, other.NativePtr);

        /// <summary>
        /// Determines whether the gameplay tag container contains the specified gameplay tag.
        /// </summary>
        /// <param name="tag">The gameplay tag to check for in the container.</param>
        /// <returns>True if the container contains the specified tag, otherwise false.</returns>
        public bool HasTag(FGameplayTag tag) => UGameplayTagContainerBlueprintLibrary.HasTag(container.NativePtr, tag);

        /// <summary>
        /// Checks if the container contains the specified gameplay tag with an exact match.
        /// </summary>
        /// <param name="tag">The gameplay tag to check for in the container.</param>
        /// <returns>True if the container contains the exact gameplay tag, otherwise false.</returns>
        public bool HasTagExact(FGameplayTag tag) =>
            UGameplayTagContainerBlueprintLibrary.HasTagExact(container.NativePtr, tag);

        /// <summary>
        /// Determines whether the specified gameplay tag container matches the provided gameplay tag query.
        /// </summary>
        /// <param name="query">The gameplay tag query to evaluate against the container.</param>
        /// <returns>True if the container matches the query; otherwise, false.</returns>
        public bool MatchesQuery(FGameplayTagQuery query) =>
            UGameplayTagContainerBlueprintLibrary.MatchesQuery(container.NativePtr, query);

        /// <summary>
        /// Filters the tags in the current container to only include those that match the tags in the specified container.
        /// </summary>
        /// <param name="other">The container whose tags are used as a filter.</param>
        /// <returns>A new container containing only the tags that match the specified container.</returns>
        public FGameplayTagContainer Filter(FGameplayTagContainer other)
        {
            return UGameplayTagContainerBlueprintLibrary.Filter(container.NativePtr, other);
        }

        /// <summary>
        /// Creates a new gameplay tag container containing only tags that exist in both the current container and the specified container.
        /// </summary>
        /// <param name="other">The container to compare against for filtering common tags.</param>
        /// <returns>A new gameplay tag container containing tags present in both containers.</returns>
        public FGameplayTagContainer Filter(StructView<FGameplayTagContainer> other)
        {
            return UGameplayTagContainerBlueprintLibrary.FilterView(container.NativePtr, other.NativePtr);
        }

        /// <summary>
        /// Filters the current gameplay tag container to include only the exact tags that also exist in the specified container.
        /// </summary>
        /// <param name="other">The gameplay tag container used for filtering. Only tags present in both containers will be included in the result.</param>
        /// <returns>A new gameplay tag container containing the exact tags matched between the current container and the specified container.</returns>
        public FGameplayTagContainer FilterExact(FGameplayTagContainer other)
        {
            return UGameplayTagContainerBlueprintLibrary.FilterExact(container.NativePtr, other);
        }

        /// <summary>
        /// Filters the gameplay tags in the container, retaining only the exact matches
        /// from the specified other container.
        /// </summary>
        /// <param name="other">The other gameplay tag container to compare against.</param>
        /// <returns>A new gameplay tag container containing only the exact matching tags.</returns>
        public FGameplayTagContainer FilterExact(StructView<FGameplayTagContainer> other)
        {
            return UGameplayTagContainerBlueprintLibrary.FilterViewExact(container.NativePtr, other.NativePtr);
        }

        /// <summary>
        /// Returns an enumerator that iterates through the gameplay tags in the container.
        /// </summary>
        /// <returns>An enumerator for the gameplay tags in the container.</returns>
        public ReadOnlySpan<FGameplayTag>.Enumerator GetEnumerator() => container.GameplayTags.GetEnumerator();

        /// <summary>
        /// Converts the gameplay tag span to a value enumerable, enabling efficient enumeration of the contained gameplay tags.
        /// </summary>
        /// <returns>A value enumerable of the gameplay tags in the container.</returns>
        public ValueEnumerable<FromSpan<FGameplayTag>, FGameplayTag> AsValueEnumerable() =>
            container.GameplayTags.AsValueEnumerable();
    }
}
