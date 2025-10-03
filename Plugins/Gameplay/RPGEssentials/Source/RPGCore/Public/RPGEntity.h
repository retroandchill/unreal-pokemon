// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "RangeV3.h"
#include "Structs/UnrealStruct.h"
#include "StructUtils/StructView.h"
#include "UObject/Object.h"

#include "RPGEntity.generated.h"

class URPGComponent;

/**
 * @class URPGEntity
 * @brief Represents a base class for entities within an RPG system.
 *
 * The URPGEntity class serves as a generic foundation for creating various
 * entities in a role-playing game. Entities can include players, NPCs,
 * monsters, or other interactable objects within the game world.
 *
 * This class is designed to be extended in order to provide specialized
 * functionality for specific entity types. It encapsulates common
 * properties and behaviors shared by all RPG entities, such as attributes,
 * status effects, and interactions.
 */
UCLASS(Abstract, DisplayName = "RPG Entity")
class RPGCORE_API URPGEntity : public UObject
{
    GENERATED_BODY()

  public:
    /**
     * @brief Creates an uninitialized instance of URPGEntity.
     *
     * This static method is used to create an instance of a URPGEntity subclass
     * without fully initializing it. The created entity will execute pre-initialization
     * logic but will not be fully initialized. This is useful when requiring an entity
     * instance with its components ready for customization prior to complete setup.
     *
     * @param Object The outer object that will own the newly created entity.
     * @param EntityClass The class type of the URPGEntity to instantiate.
     * @return A pointer to the newly created instance of URPGEntity or its derived type.
     */
    UFUNCTION(meta = (ScriptMethod, DynamicOutputParam = ReturnValue, DeterminesOutputType = EntityClass))
    static URPGEntity *CreateUninitialized(UObject *Object, TSubclassOf<URPGEntity> EntityClass);

    /**
     * @brief Retrieves the script structure associated with the RPG entity.
     *
     * This method returns the script structure that represents the current RPG entity.
     * It first attempts to retrieve the structure via a blueprint-implementable method
     * (K2_GetEntityStruct). If no structure is provided by the blueprint, it falls back
     * to the native C++ implementation (NativeGetEntityStruct).
     *
     * @return A pointer to the UScriptStruct representing the entity's structure, or null
     *         if no valid structure is defined.
     */
    UFUNCTION(BlueprintPure, Category = "RPG Entity")
    const UScriptStruct *GetEntityStruct() const;

    /**
     * @brief Retrieves a component of a specified type from the RPG entity.
     *
     * This method fetches a component of the specified class or subclass that belongs to the current RPG entity.
     * It ensures the returned component is of the requested type and throws an error if the cast is invalid.
     *
     * @tparam T The type or subclass type of URPGComponent to retrieve.
     * @param ComponentClass The class type or subclass type of the component to be retrieved. Defaults to the static
     * class of the template type.
     * @return A pointer to the component of type T, or an assertion failure if no component of the specified type is
     * present.
     */
    template <std::derived_from<URPGComponent> T = URPGComponent>
    T *GetComponent(const TSubclassOf<URPGComponent> ComponentClass = T::StaticClass()) const
    {
        return CastChecked<T>(GetComponentInternal(ComponentClass));
    }

    /**
     * @brief Retrieves the list of required components for the RPG entity.
     *
     * This method provides access to the components that are essential for the entity's functionality.
     * Required components are typically predefined and managed by the entity to ensure proper operation.
     *
     * @return A constant reference to an array containing pointers to the required components of the RPG entity.
     */
    const TArray<TObjectPtr<URPGComponent>> &GetRequiredComponents() const
    {
        return RequiredComponents;
    }

    /**
     * @brief Retrieves the list of additional components associated with the RPG entity.
     *
     * This method provides access to the components that are not part of the core functionality
     * of the entity but are instead dynamically added for extended behavior or customization.
     *
     * @return A constant reference to an array containing pointers to the additional components
     *         of the RPG entity.
     */
    const TArray<TObjectPtr<URPGComponent>> &GetAdditionalComponents() const
    {
        return AdditionalComponents;
    }

    /**
     * @brief Retrieves all components associated with the calling object.
     *
     * This method collects and returns a list of all components currently
     * attached to the calling object. Components can include various game-specific
     * functionalities, such as visual elements, audio emitters, or physical
     * behavior, and are defined as part of the object's structure.
     *
     * @return An array containing references to all components associated
     *         with the calling object.
     */
    FORCEINLINE auto GetAllComponents() const
    {
        return ranges::views::concat(RequiredComponents, AdditionalComponents);
    }

  protected:
    void PostInitProperties() override;
    void PostLoad() override;

    /**
     * @brief Specifies an event for creating required components for an RPG entity.
     *
     * This function serves as a blueprint implementable event, intended to be customized
     * within Blueprints. It is used to define the creation and initialization of specific
     * components that an RPG entity may require for functionality, such as rendering, input,
     * or behavior-related components.
     *
     * Implement this function in a Blueprint to add and configure entity-specific components
     * dynamically during runtime.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "RPG Entity")
    void CreateRequiredComponents();

    /**
     * @brief Creates and initializes an RPG component of a specified class for the entity.
     *
     * This function constructs a new instance of a component class that inherits from URPGComponent and
     * associates it with the calling entity. It is useful for dynamically adding specialized behavior
     * or functionality to individual RPG entities at runtime.
     *
     * @param ComponentClass The class type of the RPG component to create. This should be a subclass of URPGComponent.
     * @return A pointer to the newly created RPG component instance.
     */
    UFUNCTION(meta = (ScriptMethod, DeterminesOutputType = ComponentClass, DynamicOutputParam = ReturnValue))
    URPGComponent *CreateComponent(TSubclassOf<URPGComponent> ComponentClass);

    /**
     * @brief Retrieves the script structure associated with the entity.
     *
     * This method is intended to be overridden by derived classes to return
     * the specific UScriptStruct that represents the entity's data structure.
     * Provides a means of accessing type information for serialization
     * or other reflection-based operations in Unreal Engine.
     *
     * @return A pointer to the UScriptStruct representing the entity's structure,
     *         or nullptr if not implemented in the derived class.
     */
    virtual const UScriptStruct *NativeGetEntityStruct() const
    {
        return nullptr;
    }

    /**
     * @brief Retrieves the script structure associated with the entity.
     *
     * This method is intended to be implemented in Blueprints to provide the specific
     * UScriptStruct representation associated with the entity. It allows extension and
     * customization of entity struct details in a Blueprint context.
     *
     * @return A pointer to the UScriptStruct representing the entity's structure,
     *         allowing interaction with the entity's data blueprint-defined structure.
     */
    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Get Entity Struct", Category = "RPG Entity",
              meta = (ScriptName = "ManagedGetEntityStruct"))
    const UScriptStruct *K2_GetEntityStruct() const;

#if WITH_EDITOR
    EDataValidationResult IsDataValid(FDataValidationContext &Context) const override;

    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

    /**
     * @brief Prepares the RPG entity for initialization before its components are fully initialized.
     *
     * This method is called before the component initialization process begins. It allows
     * for any required pre-initialization logic specific to the RPG entity to execute.
     *
     * The function invokes native and blueprint-implemented hooks, ensuring both core
     * and user-defined behaviors are executed during the pre-initialization phase.
     */
    void PreInitializeComponents();

    /**
     * @brief Initializes components or performs setup before the components' initialization.
     *
     * This method is intended to be overridden in derived classes to provide
     * any custom pre-initialization behavior necessary before the regular
     * component initialization process occurs. It can be used to ensure
     * that prerequisites or dependencies are configured appropriately.
     *
     * By default, this method does not perform any operations.
     *
     * This method is typically used in game systems to prepare components
     * within an entity for interaction, ensuring a well-defined initialization
     * order.
     *
     * Override with caution to maintain initialization consistency across
     * derived classes.
     */
    virtual void NativePreInitializeComponents()
    {
    }

    /**
     * @brief Provides a blueprint-implementable event for pre-initialization of components.
     *
     * This method serves as a hook for derived classes to implement custom behavior
     * that should be executed before components are initialized. It allows customization
     * of the entity's setup process in Blueprint scripting.
     *
     * The event is intended to be overridden in Blueprints to handle any specific
     * logic required prior to the component initialization phase.
     */
    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Pre-Initialize Components", Category = "RPG Entity",
              meta = (ScriptName = "PreInitializeComponents"))
    void K2_PreInitializeComponents();

    /**
     * @brief Initializes all components associated with the RPG entity.
     *
     * This function performs the initialization process for all components
     * within the RPG entity. It ensures that pre-initialization logic is
     * executed, validates the provided parameters, and invokes the
     * initialization process on each component accordingly.
     *
     * The initialization involves the following steps:
     * - Execution of pre-initialization logic via PreInitializeComponents.
     * - Validation of the provided parameters against the entity's expected structure.
     * - Conditional component initialization based on the validity of the provided parameters.
     * - Execution of post-initialization logic via PostInitializeComponents.
     *
     * @param Params A structured view of parameters used for initialization.
     * The structure must match the expected type for the associated entity.
     */
    UFUNCTION(CustomThunk, meta = (ScriptMethod, CustomStructureParam = Params))
    void InitializeComponents(const int32 &Params);
    DECLARE_FUNCTION(execInitializeComponents);

    /**
     * @brief Initializes the components associated with an RPG entity.
     *
     * This method sets up and configures the components for the entity, ensuring
     * they are prepared for use within the game. It provides a fundamental step
     * in the entity's setup process by invoking the internal component
     * initialization logic.
     *
     * This function can be extended or overridden by derived classes to customize
     * the initialization process for specific component requirements.
     */
    void InitializeComponents();

    /**
     * @brief Initializes all components associated with the RPG entity.
     *
     * This function performs the initialization process for all components
     * within the RPG entity. It ensures that pre-initialization logic is
     * executed, validates the provided parameters, and invokes the
     * initialization process on each component accordingly.
     *
     * The initialization involves the following steps:
     * - Execution of pre-initialization logic via PreInitializeComponents.
     * - Validation of the provided parameters against the entity's expected structure.
     * - Conditional component initialization based on the validity of the provided parameters.
     * - Execution of post-initialization logic via PostInitializeComponents.
     *
     * @param Params A structured view of parameters used for initialization.
     * The structure must match the expected type for the associated entity.
     */
    void InitializeComponents(FStructView Params);

    /**
     * @brief Initializes all components associated with the RPG entity.
     *
     * This function performs the initialization process for all components
     * within the RPG entity. It ensures that pre-initialization logic is
     * executed, validates the provided parameters, and invokes the
     * initialization process on each component accordingly.
     *
     * The initialization involves the following steps:
     * - Execution of pre-initialization logic via PreInitializeComponents.
     * - Validation of the provided parameters against the entity's expected structure.
     * - Conditional component initialization based on the validity of the provided parameters.
     * - Execution of post-initialization logic via PostInitializeComponents.
     *
     * @tparam T The specific struct being used for initialization
     * @param Struct A structured view of parameters used for initialization.
     * The structure must match the expected type for the associated entity.
     */
    template <UEStruct T>
    void InitializeComponents(T &Struct)
    {
        InitializeComponents(FStructView(GetScriptStruct<T>(), &Struct));
    }

    /**
     * @brief Handles post-initialization logic for RPG entities.
     *
     * This method is invoked after all components associated with the entity
     * have been initialized. It provides an opportunity to perform additional
     * setup or customization in preparation for the entity's functionality
     * within the game. The default implementation integrates both native
     * and blueprint-defined initialization routines.
     *
     * Override this method to implement custom post-initialization behavior
     * for specific RPG entity subclasses.
     */
    void PostInitializeComponents();

    /**
     * @brief Called after the actor's components have been initialized.
     *
     * This virtual method is invoked to allow for additional setup or initialization
     * after all components of the actor have been created and initialized. Subclasses
     * can override this method to implement custom post-initialization behavior.
     *
     * This method is designed to be extended as needed without changing the base
     * functionality. It does not perform any actions by default.
     */
    virtual void NativePostInitializeComponents()
    {
    }

    /**
     * @brief Blueprint event triggered after components are initialized.
     *
     * This event is designed for extending classes to perform additional
     * logic or setup steps after the entity's components have been initialized.
     * It provides a customizable hook for modifying or preparing the entity
     * based on its runtime configuration.
     *
     * This function is meant to be overridden in Blueprints to define specific
     * behavior tailored to the needs of derived RPG entity types.
     */
    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Post-Initialize Components", Category = "RPG Entity",
              meta = (ScriptName = "PostInitializeComponents"))
    void K2_PostInitializeComponents();

  private:
    UFUNCTION(BlueprintPure, Category = "RPG Entity",
              meta = (DeterminesOutputType = ComponentClass, DynamicOutputParam = ReturnValue))
    URPGComponent *GetComponentInternal(TSubclassOf<URPGComponent> ComponentClass) const;

    UFUNCTION(meta = (ScriptMethod))
    void DefaultInitializeComponents();

    void GatherComponentReferences();

    UPROPERTY()
    TArray<TObjectPtr<URPGComponent>> RequiredComponents;

    /**
     * @var AdditionalComponents
     * @brief A collection of additional components associated with the RPG entity.
     *
     * This variable defines an array of RPG-related components that can be
     * attached to the entity to provide extended or customizable functionality.
     * These components may represent unique abilities, behaviors, or modular
     * features that enhance the core capabilities of the entity.
     *
     * The components listed in this array are instanced and editable within
     * defaults, allowing designers to configure them as needed in game configuration
     * or asset editing workflows.
     */
    UPROPERTY(EditDefaultsOnly, Instanced, Category = "Components")
    TArray<TObjectPtr<URPGComponent>> AdditionalComponents;
};
