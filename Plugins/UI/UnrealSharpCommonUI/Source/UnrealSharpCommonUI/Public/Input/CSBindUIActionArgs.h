// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Input/CommonUIInputTypes.h"
#include "UObject/Object.h"

#include "CSBindUIActionArgs.generated.h"

/**
 * @class FCSBindUIActionArgs
 * @brief Represents the arguments required for binding actions to a user interface element.
 *
 * This class encapsulates the information needed for associating specific
 * actions or behaviors with UI components in the application. It is designed
 * to provide a consistent and structured mechanism for managing UI action
 * bindings.
 *
 * The primary use case is to pass the necessary parameters when linking
 * actions, such as button clicks or command invocations, to their respective
 * handlers or implementations within the application logic.
 *
 * Features:
 * - Encapsulation of all necessary parameters for UI action binding.
 * - Simplifies and structures the process of connecting UI with actions.
 *
 * Use cases for this class may include:
 * - Dynamically binding actions to UI components at runtime.
 * - Managing associations between UI elements and their respective business logic.
 */
USTRUCT(meta = (RecordStruct))
struct UNREALSHARPCOMMONUI_API FCSBindUIActionArgs
{
    GENERATED_BODY()

    /**
     * @brief Default constructor for the FCSBindUIActionArgs struct.
     *
     * Initializes a new instance of the FCSBindUIActionArgs struct with default values.
     *
     * @return A default-initialized instance of FCSBindUIActionArgs.
     */
    FCSBindUIActionArgs() = default;

    /**
     * @brief Constructs an instance of FCSBindUIActionArgs using the provided arguments.
     *
     * This constructor initializes the FCSBindUIActionArgs object with a shared reference
     * to a FBindUIActionArgs instance.
     *
     * @param InArgs A shared reference to an instance of FBindUIActionArgs, which contains
     *               the details necessary for binding user interface actions.
     * @return A fully initialized instance of FCSBindUIActionArgs.
     */
    explicit(false) FCSBindUIActionArgs(const TSharedRef<FBindUIActionArgs> &InArgs) : Args(InArgs)
    {
    }

    /**
     * @brief Assignment operator for FCSBindUIActionArgs.
     *
     * Reassigns the internal arguments with the provided shared reference to
     * an instance of FBindUIActionArgs.
     *
     * @param InArgs A shared reference to an instance of FBindUIActionArgs that
     *               contains the details necessary for binding user interface actions.
     * @return A reference to the current instance of FCSBindUIActionArgs after assignment.
     */
    FCSBindUIActionArgs &operator=(const TSharedRef<FBindUIActionArgs> &InArgs)
    {
        Args = InArgs;
        return *this;
    }

    /**
     * @brief Checks if the internal arguments are valid.
     *
     * This method evaluates whether the encapsulated shared pointer to
     * FBindUIActionArgs is valid. A valid state signifies that the
     * structure has been properly initialized and contains meaningful data.
     *
     * @return True if the internal arguments (Args) are valid; otherwise, false.
     */
    bool IsValid() const
    {
        return Args.IsValid();
    }

    /**
     * @brief Retrieves the encapsulated FBindUIActionArgs instance.
     *
     * This method returns a raw pointer to the internal FBindUIActionArgs
     * instance managed by the FCSBindUIActionArgs object. It provides access
     * to the underlying data used for binding user interface actions.
     *
     * @return A pointer to the FBindUIActionArgs instance if valid; otherwise, nullptr.
     */
    FBindUIActionArgs *GetArgs() const
    {
        return Args.Get();
    }

  private:
    TSharedPtr<FBindUIActionArgs> Args;
};
