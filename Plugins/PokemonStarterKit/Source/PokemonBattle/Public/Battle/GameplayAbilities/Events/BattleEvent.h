// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BattleEvent.generated.h"


struct FBattleEventValues;
struct FBattleEventContext;

/**
 * Struct type that represents an event in battle, it consists of two payload structs that vary depending on the context
 * of the event in battle.
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FBattleEvent {
    GENERATED_BODY()

    /**
     * The context that the event happened under
     */
    TSharedPtr<FBattleEventContext> Context;

    /**
     * The modifiers that can be tweaked during damage calculation
     */
    TSharedPtr<FBattleEventValues> Modifiers;
    
};

/**
 * Declare the basic boilerplate for a polymorphic struct type
 */
#define DECLARE_POLYMORPHIC_STRUCT() UScriptStruct* GetStruct() const override;

/**
 * Implement all the boilerplate for a polymorphic struct.
 * @param The type of struct this is being implemented for
 */
#define IMPLEMENT_POLYMORPHIC_STRUCT(StructName) UScriptStruct* StructName::GetStruct() const { return StaticStruct(); }

/**
 * Parent struct definition of the context for an event in battle
 */
USTRUCT()
struct POKEMONBATTLE_API FBattleEventContext {
    GENERATED_BODY()
    
    virtual ~FBattleEventContext() = default;
    
    /**
     * Get the type of struct that is contained within this one
     * @return The type of struct
     */
    virtual UScriptStruct* GetStruct() const;
};

/**
 * Parent struct definition of values that are modified by a battle event
 */
USTRUCT()
struct POKEMONBATTLE_API FBattleEventValues {
    GENERATED_BODY()

    virtual ~FBattleEventValues() = default;
    
    /**
     * Get the type of struct that is contained within this one
     * @return The type of struct
     */
    virtual UScriptStruct* GetStruct() const;
    
};

/**
 * Wrapper around a property for an event property on 
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FEventValue {
    GENERATED_BODY()

    /**
     * The type of the attribute that is held by the owner.
     */
    UPROPERTY(Category = GameplayAttribute, VisibleAnywhere)
    TObjectPtr<UScriptStruct> AttributeOwner;

    /**
     * The property that is pointed to in this event
     */
    UPROPERTY(Category = GameplayAttribute, EditAnywhere)
    TFieldPath<FProperty> Attribute;

    /**
     * The name of the attribute as held by the game
     */
    UPROPERTY(Category = GameplayAttribute, EditAnywhere)
    FString AttributeName;

    static TArray<FProperty*> GetAllViableProperties();
};