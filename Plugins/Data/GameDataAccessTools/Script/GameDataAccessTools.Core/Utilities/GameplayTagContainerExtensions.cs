using System.Runtime.InteropServices.Marshalling;
using GameDataAccessTools.Core.Marshallers;
using GameDataAccessTools.Core.Views;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;
using ZLinq;
using ZLinq.Linq;

namespace GameDataAccessTools.Core.Utilities;

public static class GameplayTagContainerExtensions
{
    extension(StructView<FGameplayTagContainer> container)
    {
        public FGameplayTag First =>
            UGameplayTagContainerBlueprintLibrary.First(container.NativePtr);

        public FGameplayTag Last => UGameplayTagContainerBlueprintLibrary.Last(container.NativePtr);

        public int Count => UGameplayTagContainerBlueprintLibrary.Num(container.NativePtr);

        public bool IsEmpty => UGameplayTagContainerBlueprintLibrary.IsEmpty(container.NativePtr);

        public ReadOnlySpan<FGameplayTag> GameplayTags
        {
            get
            {
                var arrayView = UGameplayTagContainerBlueprintLibrary.GetGameplayTagArray(
                    container.NativePtr
                );
                return SpanMarshaller<FGameplayTag>.FromNative(arrayView.Ptr, 0);
            }
        }

        public FGameplayTagContainer Parents =>
            UGameplayTagContainerBlueprintLibrary.GetGameplayTagParents(container.NativePtr);

        public FGameplayTag GetByIndex(int index) =>
            UGameplayTagContainerBlueprintLibrary.GetByIndex(container.NativePtr, index);

        public bool IsValidIndex(int index) =>
            UGameplayTagContainerBlueprintLibrary.IsValidIndex(container.NativePtr, index);

        public bool HasAll(FGameplayTagContainer other) =>
            UGameplayTagContainerBlueprintLibrary.HasAll(container.NativePtr, other);

        public bool HasAll(StructView<FGameplayTagContainer> other) =>
            UGameplayTagContainerBlueprintLibrary.HasAllInView(
                container.NativePtr,
                other.NativePtr
            );

        public bool HasAllExact(FGameplayTagContainer other) =>
            UGameplayTagContainerBlueprintLibrary.HasAllExact(container.NativePtr, other);

        public bool HasAllExact(StructView<FGameplayTagContainer> other) =>
            UGameplayTagContainerBlueprintLibrary.HasAllExactInView(
                container.NativePtr,
                other.NativePtr
            );

        public bool HasAny(FGameplayTagContainer other) =>
            UGameplayTagContainerBlueprintLibrary.HasAny(container.NativePtr, other);

        public bool HasAny(StructView<FGameplayTagContainer> other) =>
            UGameplayTagContainerBlueprintLibrary.HasAnyInView(
                container.NativePtr,
                other.NativePtr
            );

        public bool HasTag(FGameplayTag tag) =>
            UGameplayTagContainerBlueprintLibrary.HasTag(container.NativePtr, tag);

        public bool HasTagExact(FGameplayTag tag) =>
            UGameplayTagContainerBlueprintLibrary.HasTagExact(container.NativePtr, tag);

        public bool MatchesQuery(FGameplayTagQuery query) =>
            UGameplayTagContainerBlueprintLibrary.MatchesQuery(container.NativePtr, query);

        public FGameplayTagContainer Filter(FGameplayTagContainer other)
        {
            return UGameplayTagContainerBlueprintLibrary.Filter(container.NativePtr, other);
        }

        public FGameplayTagContainer Filter(StructView<FGameplayTagContainer> other)
        {
            return UGameplayTagContainerBlueprintLibrary.FilterView(
                container.NativePtr,
                other.NativePtr
            );
        }

        public FGameplayTagContainer FilterExact(FGameplayTagContainer other)
        {
            return UGameplayTagContainerBlueprintLibrary.FilterExact(container.NativePtr, other);
        }

        public FGameplayTagContainer FilterExact(StructView<FGameplayTagContainer> other)
        {
            return UGameplayTagContainerBlueprintLibrary.FilterViewExact(
                container.NativePtr,
                other.NativePtr
            );
        }

        public ReadOnlySpan<FGameplayTag>.Enumerator GetEnumerator() =>
            container.GameplayTags.GetEnumerator();

        public ValueEnumerable<FromSpan<FGameplayTag>, FGameplayTag> AsValueEnumerable() =>
            container.GameplayTags.AsValueEnumerable();
    }
}
