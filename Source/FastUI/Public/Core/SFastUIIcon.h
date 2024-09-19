// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SFastUIWidget.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class FASTUI_API SFastUIIcon : public SFastUIWidget 
{
public:
	SLATE_BEGIN_ARGS(SFastUIIcon)
		{
		}

		SLATE_ATTRIBUTE( TArray<FString>, ClassNames )
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	
public:

	TSharedPtr<SImage> ImageWidget;
	TSharedPtr<SBorder> Container;
	
};
