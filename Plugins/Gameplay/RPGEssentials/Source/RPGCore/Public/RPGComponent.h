// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Structs/UnrealStruct.h"
#include "StructUtils/StructView.h"
#include "UObject/Object.h"

#include "RPGComponent.generated.h"

class URPGComponent;
class URPGEntity;

/**
 * @brief Utility class for initializing RPG components.
 *
 * Provides functionality to set up and configure RPG component instances during initialization. This
 * class is intended to standardize the component initialization process and ensure proper configuration
 * of dependencies and properties required by the components.
 *
 * @note This class is used internally within the RPG framework to streamline component setup.
 */
USTRUCT(DisplayName = "RPG Component Initializer")
struct RPGCORE_API FRPGComponentInitializer
{
    GENERATED_BODY()

    /**
     * @brief Default constructor for FRPGComponentInitializer.
     *
     * Initializes a default instance of the FRPGComponentInitializer class. This constructor
     * does not set up any initialization function and is primarily used for creating a basic,
     * unconfigured initializer instance.
     *
     * @return An uninitialized instance of FRPGComponentInitializer.
     */
    FRPGComponentInitializer() = default;

    /**
     * @brief Constructs an FRPGComponentInitializer with a specified initialization function.
     *
     * This constructor sets up the FRPGComponentInitializer instance with a specific
     * UFunction pointer, allowing it to execute the initialization logic defined
     * by the provided function during component setup.
     *
     * @param InitFunction A pointer to the UFunction that contains the initialization logic for the component.
     */
    explicit FRPGComponentInitializer(UFunction *InitFunction);

    /**
     * @brief Overloading operator to define custom behavior.
     *
     * Allows customization of the behavior of a specific operator for a class or object.
     * Operator overloading enhances code readability and provides a natural syntax
     * for performing operations with class instances.
     *
     * @param lhs The left-hand side operand involved in the operation.
     * @param rhs The right-hand side operand involved in the operation.
     * @return The result of the operation as defined by the custom implementation.
     */
    FRPGComponentInitializer &operator=(UFunction *Function);

    /**
     * @brief Executes the initialization function for the specified component.
     *
     * This method invokes the initialization function associated with the instance of
     * FRPGComponentInitializer, applying it to the provided component instance with the
     * supplied initialization data. If no initialization function exists, the method
     * performs no operation.
     *
     * @param Component A pointer to the component instance to be initialized.
     * @param Data A structured view representing the initialization data to be passed
     * to the component during setup.
     *
     * @note This function relies on the @c InitFunction being set to a valid UFunction.
     * If @c InitFunction is null, the execution will immediately return without any operation.
     */
    void Execute(URPGComponent *Component, FStructView Data) const;

  private:
    UPROPERTY()
    TObjectPtr<UFunction> InitFunction;

#if WITH_EDITOR
    friend class FRPGComponentInitializerCustomization;
#endif
};

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
/**
 * @brief Binds a function to a component initializer.
 *
 * This macro is used to bind a function to a component initializer. It is intended to be used
 * in conjunction with the URPGComponent class.
 */
#define BindInitFunctionDynamic(Type, FunctionName) BindInitFunction(FindFieldChecked<UFunction>(Type::StaticClass(), &Type::FunctionName, GET_FUNCTION_NAME_CHECKED(Type, FunctionName))

/**
 * @brief Abstract base class for RPG components.
 *
 * Represents a modular and extendable component system for RPG entities. It is designed to be used
 * as a blueprintable base class for defining various types of components that interact with RPG entities.
 *
 * @note This class should not be instantiated directly. Extend it to define specific components.
 */
UCLASS(Abstract, DisplayName = "RPG Component", EditInlineNew)
class RPGCORE_API URPGComponent : public UObject
{
    GENERATED_BODY()

  public:
    /**
     * @brief Pre-initializes the RPG component with the specified owning entity.
     *
     * This function sets up the component by associating it with its owning entity and invoking
     * pre-initialization routines. It ensures that both native and blueprint pre-initialization
     * logic are executed in the correct sequence.
     *
     * @param InOwningEntity The entity that owns this component.
     */
    void PreInitialize(URPGEntity *InOwningEntity);

    /**
     * @brief Initializes the RPG component with the provided data.
     *
     * This function executes the initialization logic for the component using data provided during
     * the initialization process. It ensures that the component is properly configured and ready for use.
     *
     * @param Data The configuration or initialization data used to set up the component.
     */
    void Initialize(const FStructView Data)
    {
        InitFunction.Execute(this, Data);
    }

    /**
     * @brief Binds the initialization function for the RPG component.
     *
     * Associates a specified function with the component's initialization process. This allows
     * customization of the initialization behavior by referencing a specific UFunction.
     *
     * @param Function The function to bind as the component's initialization function.
     */
    void BindInitFunction(UFunction *Function)
    {
        InitFunction = Function;
    }

    /**
     * @brief Binds a user-defined initialization function for a specific RPG component class.
     *
     * This function links a user-defined method to be executed during the initialization process
     * of a component of the specified class. The method must conform to the expected signature
     * and be identified by the provided function name within the specified class.
     *
     * @tparam UserClass The class type containing the method to be bound. Must derive from URPGComponent.
     * @tparam T The expected type of the parameter for the bound function.
     * @param ComponentClass The class of the RPG component for which the initialization function is being defined.
     * @param Function A pointer to the user-defined function within the UserClass to be executed during initialization.
     * @param FunctionName The name of the function to be bound for initialization within the specified UserClass.
     */
    template <std::derived_from<URPGComponent> UserClass, UEStruct T>
    void BindInitFunction(const TSubclassOf<URPGComponent> ComponentClass,
                          [[maybe_unused]] void (UserClass::*Function)(T), const FName FunctionName)
    {
        BindInitFunction(FindFieldChecked<UFunction>(ComponentClass, FunctionName));
    }

    /**
     * @brief Retrieves the entity that owns this RPG component.
     *
     * This function provides access to the entity associated with the component, allowing other
     * parts of the system to query or interact with the owning entity if needed.
     *
     * @return A pointer to the URPGEntity instance that owns this component.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    URPGEntity *GetOwningEntity() const
    {
        return OwningEntity;
    }

    /**
     * @brief Retrieves a sibling component of the specified type.
     *
     * Searches for and returns a sibling component of the same owner that matches the provided class type.
     * This function enables access to components of related types within the same entity or owner context.
     *
     * @tparam T The expected type of the sibling component to retrieve. Must derive from URPGComponent.
     * @param ComponentClass The class type of the sibling component to find. Defaults to the static class of T.
     * @return A pointer to the sibling component of the specified type. Throws if the cast fails.
     */
    template <std::derived_from<URPGComponent> T = URPGComponent>
    T *GetSiblingComponent(const TSubclassOf<URPGComponent> ComponentClass = T::StaticClass()) const
    {
        return CastChecked<T>(GetSiblingComponentInternal(ComponentClass));
    }

  protected:
    /**
     * @brief Executes native pre-initialization logic for the RPG component.
     *
     * This function is called internally during the component's pre-initialization process
     * to handle native-specific setup. It is intended to be overridden by derived classes
     * to implement custom initialization behavior as needed.
     *
     * @note This function is invoked before any blueprint-specific pre-initialization logic
     *       (e.g., K2_PreInitialize) to ensure proper sequencing.
     */
    virtual void NativePreInitialize()
    {
    }

    /**
     * @brief Event triggered before the component is initialized.
     *
     * Intended to provide an opportunity for any pre-initialization logic to be defined in Blueprints.
     * This event can be used to set up necessary configurations or prepare the component state prior
     * to its full initialization process.
     *
     * @note This function is exposed to Blueprints and must be implemented in derived Blueprint classes.
     */
    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Pre-Initialize", Category = "RPG Component",
              meta = (ScriptName = "PreInitialize"))
    void K2_PreInitialize();

  private:
    UFUNCTION(meta = (ScriptMethod, DeterminesOutputType = ComponentClass, DynamicOutputParam = ReturnValue))
    URPGComponent *GetSiblingComponentInternal(TSubclassOf<URPGComponent> ComponentClass) const;

    UFUNCTION(meta = (ScriptMethod))
    UFunction *BindInitFunctionInternal(const FName FunctionName);

    /**
     * @brief Specifies the initializer function for RPG components.
     *
     * This property holds an instance of FRPGComponentInitializer, which defines the setup and
     * configuration logic for RPG components. It allows for proper initialization of components
     * during runtime, ensuring dependencies and properties are correctly established.
     *
     * @note Editable in the editor for customization within the specified category.
     */
    UPROPERTY(EditAnywhere, Category = "Initialization")
    FRPGComponentInitializer InitFunction;

    /**
     * @brief Represents the owning entity associated with this RPG component.
     *
     * This variable holds a reference to the URPGEntity instance that owns or is directly associated
     * with the current RPG component instance. It acts as a link between the component and the entity
     * to ensure proper interaction and data sharing within the RPG system.
     *
     * @note This property can be accessed externally through the GetOwningEntity blueprint getter.
     */
    UPROPERTY(BlueprintGetter = GetOwningEntity, Category = "RPG Component")
    TObjectPtr<URPGEntity> OwningEntity;
};
