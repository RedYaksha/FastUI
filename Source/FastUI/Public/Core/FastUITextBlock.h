// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FastUIWidget.h"
#include "SFastUITextBlock.h"
#include "FastUITextBlock.generated.h"

/**
 * 
 */
UCLASS()
class FASTUI_API UFastUITextBlock : public UFastUIWidget
{
	GENERATED_BODY()

public:
	UFastUITextBlock(FObjectInitializer const&);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Core")
	FText Text;
	
	// UWidget interface
	virtual void SynchronizeProperties() override;
	// ~end UWidget interface
	
#if WITH_EDITOR
	// UWidget interface
	virtual const FText GetPaletteCategory() override;
	// End UWidget interface
#endif

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
protected:
	
	virtual TSharedRef< SWidget > RebuildWidget() override;
	
protected:
	TSharedPtr<SFastUITextBlock> FastUITextBlock;
};
