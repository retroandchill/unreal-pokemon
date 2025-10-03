// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeView.h"
#include "UObject/Object.h"

#include "GameplayTagContainerBlueprintLibrary.generated.h"

struct FGameplayTagContainer;

/**
 * @struct FGameplayTagArrayView
 * @brief A wrapper structure serving as a view for an array of FGameplayTag objects.
 *
 * This structure extends the functionality of TNativeView, specifically for TArray<FGameplayTag>.
 * It provides an efficient, immutable view of a gameplay tag array and enables
 * operations without requiring a copy of the underlying data.
 *
 * FGameplayTagArrayView is primarily used in conjunction with gameplay tag-related systems,
 * allowing efficient access and manipulation through various tag management APIs.
 */
USTRUCT(meta = (InternalType))
struct FGameplayTagArrayView
#if CPP
    : TNativeView<TArray<FGameplayTag>>
#endif
{
    GENERATED_BODY()

    using TNativeView::TNativeView;
};

/**
 * @struct FGameplayTagContainerView
 * @brief A lightweight view representation for FGameplayTagContainer objects.
 *
 * This structure extends the functionality of TNativeView, specifically for FGameplayTagContainer,
 * providing an efficient, immutable interface for viewing and interacting with gameplay tag containers
 * without duplicating or modifying the underlying data.
 *
 * FGameplayTagContainerView is primarily utilized in contexts where read-only access to gameplay tag
 * containers is required, allowing for optimized integration with related systems and APIs.
 */
USTRUCT(meta = (InternalType))
struct FGameplayTagContainerView
#if CPP
    : TNativeView<FGameplayTagContainer>
#endif
{
    GENERATED_BODY()

    using TNativeView::TNativeView;
};

/**
 * @class UGameplayTagContainerBlueprintLibrary
 * @brief A utility class providing blueprint-accessible functionality for manipulating FGameplayTagContainer objects.
 *
 * This class serves as a collection of static methods designed to simplify and expose operations
 * involving FGameplayTagContainer to Blueprints. It includes functionality for querying, modifying,
 * and comparing gameplay tag containers, allowing developers to effectively interact with gameplay
 * tag systems within Unreal Engine's Blueprint environment.
 *
 * UGameplayTagContainerBlueprintLibrary is essential for working with gameplay tags in a Blueprint-friendly
 * manner, enabling streamlined integration of gameplay tag logic without requiring C++ implementation.
 */
UCLASS(meta = (InternalType))
class GAMEDATAACCESSTOOLS_API UGameplayTagContainerBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * @fn UGameplayTagContainerBlueprintLibrary::Filter
     * @brief Filters the tags from the given container, returning only those matching the tags in the other container.
     *
     * This function checks all tags from the input container (`Self`) and filters them by comparing against the tags
     * present in the specified `OtherContainer`. The resulting container will include only the tags that are found
     * in both input containers.
     *
     * @param Self A view into the primary gameplay tag container to be filtered.
     * @param OtherContainer A reference to the gameplay tag container against which the tags in `Self` will be
     * filtered.
     * @return A new FGameplayTagContainer containing only the tags from `Self` that are also present in
     * `OtherContainer`.
     */
    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTagContainer Filter(FGameplayTagContainerView Self, const FGameplayTagContainer &OtherContainer);

    /**
     * @brief Filters the exact matching gameplay tags between two containers.
     *
     * This method takes a view of a gameplay tag container and filters out only the tags
     * that are found to match exactly in the provided other container. The result is a new
     * FGameplayTagContainer that contains these tags.
     *
     * @param Self A view of the FGameplayTagContainer being filtered.
     * @param OtherContainer A reference to the FGameplayTagContainer to filter against.
     * @return A new FGameplayTagContainer containing the exact matches between the two containers.
     */
    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTagContainer FilterExact(FGameplayTagContainerView Self,
                                             const FGameplayTagContainer &OtherContainer);

    /**
     * @function FilterView
     * @brief Filters the tags in the provided container by those contained in another container.
     *
     * This function returns a new FGameplayTagContainer comprising the set of tags from the
     * "Self" container that are also present in the "OtherContainer". It is particularly
     * useful for narrowing down tag sets to only those matching a specified criterion.
     *
     * @param Self The source tag container view to filter.
     * @param OtherContainer The tag container view against which the filtering is performed.
     * @return A new FGameplayTagContainer containing tags from the "Self" container that are also present in the
     * "OtherContainer".
     */
    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTagContainer FilterView(FGameplayTagContainerView Self, FGameplayTagContainerView OtherContainer);

    /**
     * @brief Filters the tags in the provided gameplay tag container view to include only exact matches with another
     * container view.
     *
     * This method creates and returns a new container with tags that exactly match between the input views.
     * It is useful for operations where precise tag alignment between different gameplay tag sets is required.
     *
     * @param Self The gameplay tag container view to filter.
     * @param OtherContainer The gameplay tag container view against which exact matches will be checked.
     * @return A new FGameplayTagContainer containing only the tags from `Self` that exactly match tags in
     * `OtherContainer`.
     */
    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTagContainer FilterViewExact(FGameplayTagContainerView Self,
                                                 FGameplayTagContainerView OtherContainer);

    /**
     * @brief Retrieves the first gameplay tag from the given tag container view.
     *
     * This function returns the first gameplay tag present in the specified
     * FGameplayTagContainerView. It is useful for accessing the initial tag
     * in a read-only manner when working with gameplay tag containers.
     *
     * @param Self A view into the gameplay tag container from which the first tag is retrieved.
     * @return The first FGameplayTag in the specified container view.
     */
    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTag First(FGameplayTagContainerView Self);

    /**
     * @brief Retrieves the gameplay tag at the specified index from the given gameplay tag container view.
     *
     * This function accesses the gameplay tag located at the provided index within the
     * specified `FGameplayTagContainerView`. The index must be valid and within the bounds
     * of the tag container view, otherwise, behavior is undefined.
     *
     * @param Self A read-only view of the gameplay tag container to retrieve the tag from.
     * @param Index The zero-based index of the tag to be retrieved.
     * @return The FGameplayTag located at the specified index within the container view.
     */
    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTag GetByIndex(FGameplayTagContainerView Self, int32 Index);

    /**
     * @brief Retrieves an array view of gameplay tags from the provided GameplayTagContainer.
     *
     * This function provides an immutable view of the FGameplayTag array contained within
     * the given FGameplayTagContainerView. It facilitates efficient access to the tags
     * without duplicating the underlying data.
     *
     * @param Self The FGameplayTagContainerView from which to obtain the array view of gameplay tags.
     * @return A FGameplayTagArrayView representing the gameplay tags in the provided container.
     */
    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTagArrayView GetGameplayTagArray(FGameplayTagContainerView Self);

    /**
     * @brief Retrieves the parent gameplay tags of the tags contained in the specified GameplayTagContainerView.
     *
     * This method takes a GameplayTagContainerView and returns a new FGameplayTagContainer
     * containing all parent tags of each gameplay tag within the provided view.
     * The parents represent the hierarchical lineage of tags in the gameplay tag system.
     *
     * @param Self The source GameplayTagContainerView whose tag parents will be retrieved.
     * @return A FGameplayTagContainer containing all parent tags of the tags in the provided view.
     */
    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTagContainer GetGameplayTagParents(FGameplayTagContainerView Self);

    /**
     * @brief Determines whether a tag container contains all tags from another container.
     *
     * This method checks if all the tags present in the specified `ContainerToCheck`
     * exist within the `Self` gameplay tag container view. It does so efficiently by
     * leveraging the immutable view provided by FGameplayTagContainerView.
     *
     * @param Self The gameplay tag container view to be checked.
     * @param ContainerToCheck The tag container whose tags need to be validated against `Self`.
     * @return true if `Self` contains all tags in `ContainerToCheck`; otherwise, false.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool HasAll(FGameplayTagContainerView Self, const FGameplayTagContainer &ContainerToCheck);

    /**
     * @brief Checks if the provided FGameplayTagContainerView contains all exact tags from another
     * FGameplayTagContainer.
     *
     * This method verifies whether every tag within the ContainerToCheck is present in
     * the target FGameplayTagContainerView (Self) without considering parent-child tag relationships.
     *
     * @param Self The FGameplayTagContainerView to be checked.
     * @param ContainerToCheck The FGameplayTagContainer whose tags need to be verified against Self.
     * @return true if all tags in ContainerToCheck are exactly present in Self, false otherwise.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool HasAllExact(FGameplayTagContainerView Self, const FGameplayTagContainer &ContainerToCheck);

    /**
     * @brief Checks if the provided container contains any tags present in the given tag container view.
     *
     * This function determines if the tag container view (`Self`) shares any common tags
     * with the container specified by `ContainerToCheck`.
     *
     * @param Self The FGameplayTagContainerView representing the tag array being checked.
     * @param ContainerToCheck The FGameplayTagContainer containing the tags to compare against.
     * @return true if at least one tag matches between `Self` and `ContainerToCheck`, false otherwise.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool HasAny(FGameplayTagContainerView Self, const FGameplayTagContainer &ContainerToCheck);

    /**
     * @brief Checks if all tags in one tag container view are present in another tag container view.
     *
     * This function compares two `FGameplayTagContainerView` instances and determines if all tags
     * in the `ContainerToCheck` are present within the `Self` container view.
     *
     * @param Self The primary tag container view to check against.
     * @param ContainerToCheck The tag container view whose tags must be completely contained in `Self`.
     * @return `true` if all tags in `ContainerToCheck` are found within `Self`, otherwise `false`.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool HasAllInView(FGameplayTagContainerView Self, FGameplayTagContainerView ContainerToCheck);

    /**
     * @brief Checks whether the container view contains all the exact tags in another container view.
     *
     * This method evaluates if all the tags present in the ContainerToCheck parameter are
     * strictly and exactly present in the Self parameter. It ensures all the tags match
     * without considering tag hierarchies or partial matches.
     *
     * @param Self The FGameplayTagContainerView to check against.
     * @param ContainerToCheck The FGameplayTagContainerView containing the tags to verify.
     * @return True if Self contains all the exact tags in ContainerToCheck; otherwise, false.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool HasAllExactInView(FGameplayTagContainerView Self, FGameplayTagContainerView ContainerToCheck);

    /**
     * @brief Checks if the specified tag container contains any of the tags present in the provided container.
     *
     * This function compares the tags in `Self` against the tags in `ContainerToCheck`
     * and determines whether at least one tag from `ContainerToCheck` exists in `Self`.
     * It provides an efficient way to query gameplay tags from views without requiring additional copies of the data.
     *
     * @param Self The tag container view to check within.
     * @param ContainerToCheck The tag container view containing tags to check against.
     * @return True if `Self` contains any tags from `ContainerToCheck`, otherwise false.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool HasAnyInView(FGameplayTagContainerView Self, FGameplayTagContainerView ContainerToCheck);

    /**
     * @brief Checks if a specified tag exists within the provided gameplay tag container view.
     *
     * This function determines whether the given FGameplayTag is present within the FGameplayTagContainerView instance.
     * It provides an efficient method for tag presence validation in gameplay tag-related systems.
     *
     * @param Self The gameplay tag container view to search within.
     * @param TagToCheck The specific gameplay tag to verify for presence in the container view.
     * @return True if the tag is found in the container view, false otherwise.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool HasTag(FGameplayTagContainerView Self, FGameplayTag TagToCheck);

    /**
     * @brief Checks if the specified gameplay tag container contains the exact tag.
     *
     * This function verifies whether the given FGameplayTagContainerView contains an exact match
     * for the specified FGameplayTag. The comparison is strictly for an exact tag match
     * rather than partial or hierarchical matches.
     *
     * @param Self The FGameplayTagContainerView to check for the tag.
     * @param TagToCheck The FGameplayTag to search for within the container.
     * @return True if the container contains the exact tag, otherwise false.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool HasTagExact(FGameplayTagContainerView Self, FGameplayTag TagToCheck);

    /**
     * @brief Determines whether the given gameplay tag container is empty.
     *
     * This static function checks if the FGameplayTagContainerView instance contains any gameplay tags.
     *
     * @param Self The FGameplayTagContainerView instance to check.
     * @return True if the container is empty, otherwise false.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool IsEmpty(FGameplayTagContainerView Self);

    /**
     * @brief Checks if the given FGameplayTagContainerView instance is valid.
     *
     * This function verifies the validity of the provided FGameplayTagContainerView by
     * examining its underlying data. A valid FGameplayTagContainerView contains properly
     * initialized data that can be used for gameplay tag operations.
     *
     * @param Self The FGameplayTagContainerView instance to validate.
     * @return True if the given FGameplayTagContainerView is valid; otherwise, false.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool IsValid(FGameplayTagContainerView Self);

    /**
     * @brief Checks whether the provided index is valid within the given GameplayTagContainerView.
     *
     * This function determines if the specified index corresponds to a valid entry
     * within the supplied FGameplayTagContainerView instance. It is useful for validating
     * indices before performing operations on the container.
     *
     * @param Self The FGameplayTagContainerView instance to check against.
     * @param Index The index to validate within the provided container view.
     * @return Returns true if the index is valid within the given container view, otherwise false.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool IsValidIndex(FGameplayTagContainerView Self, int32 Index);

    /**
     * @brief Retrieves the last gameplay tag from the specified gameplay tag container view.
     *
     * This function accesses the given FGameplayTagContainerView and returns the last
     * FGameplayTag in the view. It provides a convenient way to retrieve the final tag
     * within a container view when the order of tags is significant.
     *
     * @param Self The FGameplayTagContainerView that serves as the source for retrieving the last gameplay tag.
     * @return The last FGameplayTag contained within the provided FGameplayTagContainerView.
     */
    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTag Last(FGameplayTagContainerView Self);

    /**
     * @brief Evaluates whether a specified gameplay tag container matches a given gameplay tag query.
     *
     * This function checks if the tags within the provided FGameplayTagContainerView meet the conditions
     * defined by the specified FGameplayTagQuery. It is designed to simplify the querying mechanism
     * for gameplay tags in various gameplay systems.
     *
     * @param Self The gameplay tag container to evaluate, represented by an immutable FGameplayTagContainerView.
     * @param Query The gameplay tag query that defines the conditions to match against the tag container.
     * @return true if the tag container satisfies the conditions of the query, false otherwise.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool MatchesQuery(FGameplayTagContainerView Self, const FGameplayTagQuery &Query);

    /**
     * @function Num
     * @brief Returns the number of elements in the given FGameplayTagContainerView.
     *
     * This method provides access to the count of gameplay tags contained within the
     * supplied FGameplayTagContainerView. It is useful for determining the size of the
     * container without making modifications to its contents.
     *
     * @param Self The immutable FGameplayTagContainerView instance whose elements are to be counted.
     * @return The number of gameplay tags present in the container.
     */
    UFUNCTION(meta = (ScriptMethod))
    static int32 Num(FGameplayTagContainerView Self);
};
