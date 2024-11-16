// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimSequence.h"
#include "PaperFlipbook.h"
#include "PaperZDAnimSequence_Flipbook.generated.h"

/**
 * An AnimSequence that renders Flipbook animations.
 */
UCLASS()
class PAPERZD_API UPaperZDAnimSequence_Flipbook : public UPaperZDAnimSequence
{
	GENERATED_BODY()

	//Support for runtime proxy
	friend class FPaperZDRuntimeEditorProxy;

	//@DEPRECATED: In favor of the animation data source which now can support multi-directional flipbooks.
	UPROPERTY()
	TObjectPtr<UPaperFlipbook> Flipbook_DEPRECATED;
	
	/* Contains the render information for displaying the flipbook, multi-directional. */
	UPROPERTY(EditAnywhere, Category = "AnimSequence")
	TArray<TObjectPtr<UPaperFlipbook>> AnimDataSource;
	
public:

	//~ Begin UObject Interface
	virtual void PostLoad() override;
	virtual void Serialize(FArchive& Ar) override;
	//~ End UObject Interface

	//~ Begin UPaperZDAnimSequence Interface
	virtual float GetTotalDuration() const override;
	virtual float GetFramesPerSecond() const override;
	virtual bool IsDataSourceEntrySet(int32 EntryIndex) const override;
	//~ End UPaperZDAnimSequence Interface

private:
	/* Helper for getting the primary flipbook of the sequence. */
	FORCEINLINE UPaperFlipbook* GetPrimaryFlipbook() const { return AnimDataSource.Num() && AnimDataSource[0] ? AnimDataSource[0] : nullptr; }
};
