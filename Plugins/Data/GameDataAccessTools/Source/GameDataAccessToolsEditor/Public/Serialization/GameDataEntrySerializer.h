// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameDataEntrySerializer.generated.h"

class UGameDataRepository;

// This class does not need to be modified.
UINTERFACE()
class UGameDataEntrySerializer : public UInterface {
    GENERATED_BODY()
};

/**
 * 
 */
class GAMEDATAACCESSTOOLSEDITOR_API IGameDataEntrySerializer {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintNativeEvent, Category = "Serialization")
    bool Supports(TSubclassOf<UGameDataRepository> RepositoryClass);
    
    UFUNCTION(BlueprintNativeEvent, Category = "Serialization")
    FText GetFormatName() const;
    
    UFUNCTION(BlueprintNativeEvent, Category = "Serialization")
    FString GetFileExtensionText() const;

    UFUNCTION(BlueprintNativeEvent, Category = "Serialization")
    bool Serialize(const FString& FilePath, const UGameDataRepository* Repository, FString& ErrorMessage) const;

    UFUNCTION(BlueprintNativeEvent, Category = "Serialization")
    bool Deserialize(const FString& FilePath, UGameDataRepository* Repository, FString& ErrorMessage) const;
};

class FGameDataEntrySerializerPtr {

public:
    explicit FGameDataEntrySerializerPtr(const UObject* InSerializer)
        : Serializer(InSerializer)
    {
        check(InSerializer != nullptr && InSerializer->Implements<UGameDataEntrySerializer>())
    }

    FText GetFormatName() const {
        return IGameDataEntrySerializer::Execute_GetFormatName(Serializer.Get());
    }

    FString GetFileExtensionText() const {
        return IGameDataEntrySerializer::Execute_GetFileExtensionText(Serializer.Get());
    }

    bool Serialize(const FString& FilePath, const UGameDataRepository* Repository, FString& ErrorMessage) const {
        return IGameDataEntrySerializer::Execute_Serialize(Serializer.Get(), FilePath, Repository, ErrorMessage);
    }

    bool Deserialize(const FString& FilePath, UGameDataRepository* Repository, FString& ErrorMessage) const {
        return IGameDataEntrySerializer::Execute_Deserialize(Serializer.Get(), FilePath, Repository, ErrorMessage);
    }

private:
    TStrongObjectPtr<const UObject> Serializer;
};