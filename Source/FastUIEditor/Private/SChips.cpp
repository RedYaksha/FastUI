// Fill out your copyright notice in the Description page of Project Settings.


#include "SChips.h"

#include "FastUIEngineSubsystem.h"
#include "SFastUIButton.h"
#include "SFastUIEditableText.h"
#include "SFastUIIcon.h"
#include "SFastUITextBlock.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SResponsiveGridPanel.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

TMap<FString, FString> SChips::CategoryToColor = {
	{ "Spacing", "bg-violet-300" },
	{ "Sizing", "bg-lime-300" },
	{ "Background Style", "bg-emerald-300" },
	{ "Typography", "bg-fuchsia-300" },
	{ "Layout", "bg-blue-300" },
};

void SChips::Construct(const FArguments& InArgs)
{
	OnClassNameAdded = InArgs._OnClassNameAdded;
	OnClassNameRemoved = InArgs._OnClassNameRemoved;
	
	SAssignNew(EditableText, SFastUIEditableText)
	.ClassNames(TArray<FString>({"m-1", "px-2", "py-1", "rounded", "bg-neutral-600", "text-neutral-100", "font-helveticaneue", "text-xs"}))
	.HintText(FText::FromString("Enter class name..."))
	.OnTextCommitted(this, &SChips::OnEditableTextCommit)
	.ClearKeyboardFocusOnCommit(false)
	;

	{
		for(int i = 0 ; i < 20; i++)
		{
			TSharedPtr<FClassNameSearchItem> Item = MakeShareable(new FClassNameSearchItem)	;
			Item->ClassName = "Class Name " + FString::FromInt(i);
			
			FilteredClassNames.Add(Item);
		}
	}
	SAssignNew(ClassNameSearchListView, SListView<TSharedPtr<FClassNameSearchItem>>)
	.ItemHeight(18)
	.ListItemsSource(&FilteredClassNames)
	.OnGenerateRow(this, &SChips::OnGeneratedWidgetForSearchItem)
	;
	
	
	
	Entries.Empty();
	//WidgetsInGrid.Empty();
	//WidgetsInGrid.Add(EditableText.ToSharedRef());
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 10, 0,12)
		[
			SNew(SBox)
			.WidthOverride(400)
			[
				EditableText.ToSharedRef()
			]
		]
		/*
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			ClassNameSearchListView.ToSharedRef()
		]
		*/
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(2,10)
		[
			SAssignNew(MainVerticalBox, SVerticalBox)
		]
	];


	RebuildGridSlots();
}

FReply SChips::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if(InKeyEvent.GetKey() == EKeys::Enter)
	{
		UE_LOG(LogTemp, Log, TEXT("Enter pressed for chips"))
	}
	return FReply::Handled();
}

void SChips::OnEditableTextCommit(const FText& Text, ETextCommit::Type CommitType)
{
	if(CommitType == ETextCommit::OnEnter)
	{
		AddClassName(Text.ToString(), true, true);
		
		//WidgetsInGrid.Add(NewWidget);
			
		// clear the editable text
		EditableText->SetText(FText::FromString(""));
		
	}
}

void SChips::SetClassNames(TArray<FString> InClassNames)
{
	for(auto C : InClassNames)
	{
		AddClassName(C, false, false);
	}

	RebuildGridSlots();
}

void SChips::AddClassName(FString InClassName, bool bRebuildGridSlots, bool bDoCallback)
{
	UE_LOG(LogTemp, Log, TEXT("Commited class %s"), *InClassName)

	FString Category = UFastUIEngineSubsystem::Get()->GetCategoryForClass(InClassName);

	FString BGColor = "bg-neutral-500";
	if(CategoryToColor.Contains(Category))
	{
		BGColor = CategoryToColor[Category];
	}

	TSharedRef<SWidget> NewWidget =
		SNew(SFastUIWidget)
		.ClassNames(TArray<FString>({
			"hbox",
			"m-1", "px-2", "py-1",
			BGColor, "rounded-lg",
		}))
		[{
			SNew(SFastUITextBlock)
			.ClassNames(TArray<FString>({
				"mx-1",
				"font-helveticaneue", "text-xs", "text-neutral-800"
			}))
			.Text(FText::FromString(InClassName)),

			SNew(SFastUIButton)
			.ClassNames(TArray<FString>({
				"mx-1", "rounded-lg",
				"hbox"
			}))
			.OnClicked(this, &SChips::OnRemoveClassName, InClassName)
			[{
				SNew(SFastUIIcon)
				.ClassNames(TArray<FString>({"i-x", "v-center", "icon-neutral-600"}))
			}],
		}]
	;

	FClassNameEntry Entry;
	Entry.ClassName = InClassName;
	Entry.Widget = NewWidget;

	Entries.Add(Entry);

	if(bRebuildGridSlots)
	{
		RebuildGridSlots();
	}

	if(bDoCallback)
	{
		OnClassNameAdded.ExecuteIfBound(InClassName);
	}
}

void SChips::RemoveClassName(FString InClassName)
{
	int IndToRemove = -1;
	for(int i = 0; i < Entries.Num(); i++)
	{
		if(Entries[i].ClassName == InClassName)
		{
			IndToRemove = i;
			break;
		}
	}

	if(IndToRemove > -1)
	{
		Entries.RemoveAt(IndToRemove);
	}
	
	RebuildGridSlots();
	OnClassNameRemoved.ExecuteIfBound(InClassName);
}

FReply SChips::OnRemoveClassName(FString ClassName)
{
	RemoveClassName(ClassName);
	return FReply::Handled();
}

void SChips::RebuildGridSlots()
{
	check(MainVerticalBox)
	//check(WidgetsInGrid.Num() > 0);
	
	MainVerticalBox->ClearChildren();

	if(Entries.Num() == 0)
	{
		MainVerticalBox->AddSlot()
		[
			SNew(SFastUITextBlock)
			.ClassNames(TArray<FString>({
				"mx-1",
				"font-helveticaneue", "text-lg", "text-neutral-300"
			}))
			.Text(FText::FromString("No class names added"))
		];

		return;
	}
	
	// sort entries by Category
	TArray<FClassNameEntry> SpacingCat;
	TArray<FClassNameEntry> LayoutCat;
	TArray<FClassNameEntry> StyleCat;
	TArray<FClassNameEntry> SizingCat;
	TArray<FClassNameEntry> TypographyCat;
	TArray<FClassNameEntry> UndefinedCat;

	UFastUIEngineSubsystem* FastUIEngineSubsystem = UFastUIEngineSubsystem::Get();
	for(FClassNameEntry& Entry : Entries)
	{
		FString Cat = FastUIEngineSubsystem->GetCategoryForClass(Entry.ClassName);
		if(Cat == "Spacing") { SpacingCat.Add(Entry); }
		else if(Cat == "Layout") { LayoutCat.Add(Entry); }
		else if(Cat == "Background Style") { StyleCat.Add(Entry); }
		else if(Cat == "Typography") { TypographyCat.Add(Entry); }
		else if(Cat == "Sizing") { SizingCat.Add(Entry); }
		else { UndefinedCat.Add(Entry); }
	}
	
	int CurCol = -1;
	int CurRow = 0;

	// add chips one by one, increasing CurRow if we can't fit any chips anymore
	const FGeometry& Geometry = GetPaintSpaceGeometry();
	const FVector2D AllocatedSize = Geometry.Size;
	float WidthUsedSoFar = 0.f;
	
	TSharedRef<SHorizontalBox> CurHorizontalBox = SNew(SHorizontalBox);
	MainVerticalBox->AddSlot()
	.AutoHeight()
	[
		CurHorizontalBox
	];

	AddGridSlots(LayoutCat, CurHorizontalBox, AllocatedSize.X, WidthUsedSoFar, CurCol, CurRow);
	AddGridSlots(SizingCat, CurHorizontalBox, AllocatedSize.X, WidthUsedSoFar, CurCol, CurRow);
	AddGridSlots(SpacingCat, CurHorizontalBox, AllocatedSize.X, WidthUsedSoFar, CurCol, CurRow);
	AddGridSlots(TypographyCat, CurHorizontalBox, AllocatedSize.X, WidthUsedSoFar, CurCol, CurRow);
	AddGridSlots(StyleCat, CurHorizontalBox, AllocatedSize.X, WidthUsedSoFar, CurCol, CurRow);
	AddGridSlots(UndefinedCat, CurHorizontalBox, AllocatedSize.X, WidthUsedSoFar, CurCol, CurRow);
	
}

bool SChips::ProcessAddingGridSlot(TSharedRef<SWidget> Widget, float MaxWidth, float& WidthSoFar, int& CurCol, int& CurRow)
{
	Widget->SlatePrepass();
	const FVector2D WidgetDesiredSize = Widget->GetDesiredSize();
	
	// if adding this widget exceeds our allocated width, increased the row
	bool bIncreasedRow = false;
	UE_LOG(LogTemp, Log, TEXT("WidthSoFar: %f | WidgetDesiredWidth: %f | MaxWidth: %f"), WidthSoFar, WidgetDesiredSize.X, MaxWidth);
	if(WidthSoFar + WidgetDesiredSize.X > MaxWidth)
	{
		CurCol = 0;
		CurRow += 1;
		bIncreasedRow = true;
	}
	else
	{
		CurCol++;
		// row stays the same
	}

	if(bIncreasedRow)
	{
		WidthSoFar = WidgetDesiredSize.X;
	}
	else
	{
		WidthSoFar += WidgetDesiredSize.X;
	}
	
	return bIncreasedRow;
}

TSharedRef<ITableRow> SChips::OnGeneratedWidgetForSearchItem(TSharedPtr<FClassNameSearchItem> Message,
	const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FClassNameSearchItem> >, OwnerTable)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(0)
			[
				SNew(SFastUITextBlock)
				.Text(FText::FromString(Message->ClassName))
			]
		];
}

void SChips::AddGridSlots(TArray<FClassNameEntry> InEntries, TSharedRef<SHorizontalBox>& CurHorizontalBox, float MaxWidth, float& WidthSoFar, int& CurCol,
	int& CurRow)
{
	for(int i = 0; i < InEntries.Num(); i++)
	{
		FClassNameEntry& Entry = InEntries[i];
		TSharedRef<SWidget> WidgetRef = Entry.Widget.ToSharedRef(); //getsInGrid[i];
		
		const bool bIncreasedRow = ProcessAddingGridSlot(WidgetRef, MaxWidth, WidthSoFar, CurCol, CurRow);
		
		if(bIncreasedRow)
		{
			CurHorizontalBox = SNew(SHorizontalBox);
			MainVerticalBox->AddSlot()
			.AutoHeight()
			[
				CurHorizontalBox
			];
		}

		CurHorizontalBox->AddSlot()
		.AutoWidth()
		[
			WidgetRef
		];

		UE_LOG(LogTemp, Log, TEXT("Adding Slot to Col:%d Row:%d"), CurCol, CurRow);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
