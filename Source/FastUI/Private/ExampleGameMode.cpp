// Fill out your copyright notice in the Description page of Project Settings.


#include "ExampleGameMode.h"

#include "SFastUIButton.h"
#include "SFastUITextBlock.h"
#include "SFastUIWidget.h"


// Sets default values
AExampleGameMode::AExampleGameMode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AExampleGameMode::BeginPlay()
{
	Super::BeginPlay();

	TSharedPtr<SFastUIWidget> Root;
	GetWorld()->GetGameViewport()->AddViewportWidgetContent(
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SAssignNew(Root, SFastUIWidget)
			.ClassNames(TArray<FString>({"m-2", "p-9", "bg-red-500"}))
		]
	,0);

	Root->CheckReferences();

	TSharedRef<SWidget> Child1 = 
		SNew(SFastUITextBlock)
		.Text(FText::FromString("Helvetica Text-3xl"))
		.ClassNames(TArray<FString>({"px-4", "bg-red", "rounded", "text-3xl", "font-helveticaneue"}))
	;
	
	TSharedRef<SWidget> Child2= 
		SNew(STextBlock)
		.Text(FText::FromString("HHH"))
	;

	/*
		SNew(SFastUIWidgetSwitcher<TabType>)
		.ClassNames({}) // propogates to the icons or labels
		.Labels()
		.Icons()
		.UseGeneratedTabs(true)
		.UseIconTabs()
		.UseTextTabs()
		.Orientation()
		.OnGenerateTab() // UWidget: UClass replaces this
		.WidgetIndex()
		[{
			SNew(SWidget),
			SNew(SWidget),
			SNew(SWidget),
			SNew(SWidget),
			SNew(SWidget),
		}]

		SNew(SFastUITabs)
		.WidgetSwitcher()

		// generates preview list data
		// 
		SNew(SFastUIListView)
		[
		

		]
	 
	 */
	
	Root->CheckReferences();

	Root->AddSlot(
		Child1
	);
}

// Called every frame
void AExampleGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

