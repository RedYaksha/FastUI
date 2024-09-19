// Fill out your copyright notice in the Description page of Project Settings.


#include "FastUITextBlock.h"

#include "SFastUITextBlock.h"

#define LOCTEXT_NAMESPACE "FastUI"

UFastUITextBlock::UFastUITextBlock(FObjectInitializer const& OI)
	: UFastUIWidget(OI)
{
	Text = FText::FromString("Default Text");
}

void UFastUITextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	RebuildWidget();
}

const FText UFastUITextBlock::GetPaletteCategory()
{
	return LOCTEXT("FastUI", "Fast UI");
}

void UFastUITextBlock::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	
	FastUIWidget.Reset();
	FastUITextBlock.Reset();
}

TSharedRef<SWidget> UFastUITextBlock::RebuildWidget()
{
	//TArray<FString> ClassNamesArr;
	//ClassNames.ParseIntoArray(ClassNamesArr, TEXT(" "));
	
	SAssignNew(FastUITextBlock, SFastUITextBlock)
	.Text(Text)
	.ClassNames(ClassNamesArr);

	FastUIWidget = FastUITextBlock;
	return FastUIWidget.ToSharedRef();
}

#undef LOCTEXT_NAMESPACE