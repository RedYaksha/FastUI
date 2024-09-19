// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SFastUIWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "FastUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class FASTUI_API UFastUIWidget : public UPanelWidget 
{
	GENERATED_BODY()

public:
	/** UUserWidget interface */
	/** ~end UUserWidget interface */
	UFastUIWidget(FObjectInitializer const&);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Core")
	FString ClassNames;

	TArray<FString> GetClassNames() const { return ClassNamesArr; }
	void AddClassName(FString InClassName);
	void RemoveClassName(FString InClassName);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Core")
	TArray<FString> ClassNamesArr;


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
	// UPanelWidget
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* Slot) override;
	virtual void OnSlotRemoved(UPanelSlot* Slot) override;
	// End UPanelWidget
	
	virtual TSharedRef< SWidget > RebuildWidget() override;
	
protected:
	TSharedPtr<SFastUIWidget> FastUIWidget;
	
	UPROPERTY()
	UObject* CachedSourceOrAsset;
	
};

