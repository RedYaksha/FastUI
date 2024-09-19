// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SFastUIEditableText.h"
#include "Widgets/SCompoundWidget.h"


struct FClassNameSearchItem
{
	FString ClassName;
};

DECLARE_DELEGATE_OneParam(FOnClassNameAdded, FString)
DECLARE_DELEGATE_OneParam(FOnClassNameRemoved, FString)

/**
 * 
 */
class FASTUIEDITOR_API SChips : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SChips)
		{}

	SLATE_EVENT(FOnClassNameAdded, OnClassNameAdded)
	SLATE_EVENT(FOnClassNameRemoved, OnClassNameRemoved)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	static TMap<FString, FString> CategoryToColor;

	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	void OnEditableTextCommit(const FText& Text, ETextCommit::Type CommitType);

	void SetClassNames(TArray<FString> InClassNames);
	void AddClassName(FString InClassName, bool bRebuildGridSlot=false, bool bDoCallback=false);
	void RemoveClassName(FString InClassName);

	FReply OnRemoveClassName(FString ClassName);

	void RebuildGridSlots();

protected:
	static bool ProcessAddingGridSlot(TSharedRef<SWidget> Widget, float MaxWidth, float& WidthSoFar, int& CurCol, int& CurRow);
	TSharedRef<ITableRow> OnGeneratedWidgetForSearchItem(TSharedPtr<FClassNameSearchItem> Message, const TSharedRef<STableViewBase>& OwnerTable);
	
protected:
	struct FClassNameEntry
	{
		FString ClassName;
		TSharedPtr<SWidget> Widget;
	};
	
	void AddGridSlots(TArray<FClassNameEntry> InEntries, TSharedRef<SHorizontalBox>& CurHorizontalBox, float MaxWidth, float& WidthSoFar, int& CurCol, int& CurRow);

	TArray<FClassNameEntry> Entries;
	
	// TArray<TSharedRef<SWidget>> WidgetsInGrid;
	//TSharedPtr<SGridPanel> GridPanel;
	TSharedPtr<SFastUIEditableText> EditableText;
	TSharedPtr<SSearchBox> SearchBox;
	TSharedPtr<SListView<TSharedPtr<FClassNameSearchItem>>> ClassNameSearchListView;
	TArray<TSharedPtr<FClassNameSearchItem>> FilteredClassNames;

	TSharedPtr<SVerticalBox> MainVerticalBox;

	FOnClassNameAdded OnClassNameAdded;
	FOnClassNameRemoved OnClassNameRemoved;
};
