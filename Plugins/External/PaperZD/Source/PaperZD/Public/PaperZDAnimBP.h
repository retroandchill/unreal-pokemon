// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "PaperZDAnimBP.generated.h"

class UPaperZDAnimationSource;

/**
 * Class responsible of driving animation for 2d characters.
 * Compiles into PaperZDAnimInstance, which is the runtime compiled class that works in-game.
 */
UCLASS(Blueprintable)
class PAPERZD_API UPaperZDAnimBP : public UBlueprint
{
	GENERATED_UCLASS_BODY()

#if WITH_EDITORONLY_DATA
public:
	/* The animation graph. */
	UPROPERTY()
	TObjectPtr<class UEdGraph> AnimationGraph = nullptr;

	/* Animation source that we're implementing. */
	UPROPERTY(AssetRegistrySearchable, VisibleDefaultsOnly, Category = "PaperZD")
	TObjectPtr<UPaperZDAnimationSource> SupportedAnimationSource = nullptr;
	
private:
	/* Names of the registered notifies. */
	UPROPERTY()
	TArray<FName> RegisteredNotifyNames_DEPRECATED;
#endif

public:
	virtual void PostLoad() override;
	virtual void Serialize(FArchive& Ar) override;
#if WITH_EDITOR
	virtual void PostInitProperties() override;

	/* Returns the class to use for this blueprint. */
	virtual UClass* GetBlueprintClass() const override;

	/* Returns the Animation Source that this blueprint is supporting. */
	UPaperZDAnimationSource* GetSupportedAnimationSource() const { return SupportedAnimationSource; }

	/* Returns the animation source member name. */
	static FName GetAnimationSourceMemberName() { return GET_MEMBER_NAME_CHECKED(UPaperZDAnimBP, SupportedAnimationSource); }
#endif

public:	
#if WITH_EDITOR
	
	//Create the basic sound graph
	void CreateGraph();

	// Get the EdGraph of Animation
	class UEdGraph* GetGraph();

	/* Called just before compiling this AnimBP, gives the chance to order some functions for the compiler. */
	void OnPreCompile();

	/* Called just after compiling this AnimBP, gives the chance to order some functions for the compiler. */
	void OnPostCompile();

	/* Checks if the version of this AnimBP is compatible with the compiler. */
	bool IsVersionCompatibleForCompilation() const;

	/* Notifies that a custom notify has just been renamed. */
	void OnRenameCustomNotify(FName OldName, FName NewName);

private:
	//Template to use when naming a new AnimSequence
	static const FString SequenceNameTemplate;

	//Friendship for access to version updating
	friend class FPaperZDRuntimeEditorProxy;
#endif
};
