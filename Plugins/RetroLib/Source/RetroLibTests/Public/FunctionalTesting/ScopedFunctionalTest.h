// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "EditorFunctionalTest.h"
#include "RetroLib/Utils/UniqueAny.h"

#include "ScopedFunctionalTest.generated.h"

/**
 * @class AScopedFunctionalTest
 * @brief Abstract class for editor functional tests with scoped test objects and data.
 *
 * This class is a base class for creating editor functional tests that require specific test objects
 * or data to be scoped within the test. It inherits from AEditorFunctionalTest and adds functionalities
 * for preparing and finishing the test, as well as managing scoped test objects and data.
 */
UCLASS(Abstract)
class RETROLIBTESTS_API AScopedFunctionalTest : public AEditorFunctionalTest
{
    GENERATED_BODY()

  public:
    void PrepareTest() override;
    void CleanUp() override;

    /**
     * @brief Adds data scoped for the functional test.
     *
     * This method emplaces data into the ScopedData array using the provided arguments.
     *
     * @tparam T The type of data to add
     * @tparam A The types of the constructor arguments
     * @param Args Variadic template arguments that are forwarded to construct the data of type T placed within the
     * functional test scope.
     */
    template <typename T, typename... A>
        requires std::constructible_from<Retro::FUniqueAny, std::in_place_type_t<T>, A...>
    void AddScopedData(A &&...Args)
    {
        ScopedData.Emplace(std::in_place_type<T>, std::forward<A>(Args)...);
    }

  private:
    TArray<Retro::FUniqueAny> ScopedData;
};