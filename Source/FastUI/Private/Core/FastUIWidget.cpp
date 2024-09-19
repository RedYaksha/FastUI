// Fill out your copyright notice in the Description page of Project Settings.


#include "FastUIWidget.h"

#include "BlueprintEditorModule.h"
#include "WidgetBlueprint.h"
#include "WidgetBlueprintEditor.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

#define LOCTEXT_NAMESPACE "FastUI"

UFastUIWidget::UFastUIWidget(FObjectInitializer const&)
{
	ClassNames = "";
	ClassNamesArr = TArray<FString>({});
}

void UFastUIWidget::AddClassName(FString InClassName)
{
	UObject* SourceAsset = GetSourceAssetOrClass();
	UE_LOG(LogTemp, Log, TEXT("Full name: %s"), *GetFullName());
	
	if(SourceAsset == nullptr)
	{
		SourceAsset = CachedSourceOrAsset;
		if(SourceAsset == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Source asset null!!!"))
			return;
		}
	}
	
	ClassNamesArr.Add(InClassName);
	ClassNames += InClassName;
	TSharedPtr<IBlueprintEditor> Editor = FKismetEditorUtilities::GetIBlueprintEditorForObject(SourceAsset, false);
	if(TSharedPtr<FWidgetBlueprintEditor> BPEditor = StaticCastSharedPtr<FWidgetBlueprintEditor>(Editor))
	{
		if(!BPEditor->GetPreview())
			return;

		FWidgetReference WidgetReference = BPEditor->GetReferenceFromPreview(this);

		UWidget* TemplateWidgetRaw = WidgetReference.GetTemplate();
		UFastUIWidget* TemplateWidget = Cast<UFastUIWidget>(TemplateWidgetRaw);
		if(TemplateWidget)
		{
			TemplateWidget->ClassNamesArr.Add(InClassName);
		}
		
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BPEditor->GetWidgetBlueprintObj());
	}
}

void UFastUIWidget::RemoveClassName(FString InClassName)
{
	UObject* SourceAsset = GetSourceAssetOrClass();
	UE_LOG(LogTemp, Log, TEXT("Full name: %s"), *GetFullName());
	
	if(SourceAsset == nullptr)
	{
		SourceAsset = CachedSourceOrAsset;
		if(SourceAsset == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Source asset null!!!"))
			return;
		}
	}
	
	ClassNamesArr.Add(InClassName);
	ClassNames += InClassName;
	TSharedPtr<IBlueprintEditor> Editor = FKismetEditorUtilities::GetIBlueprintEditorForObject(SourceAsset, false);
	if(TSharedPtr<FWidgetBlueprintEditor> BPEditor = StaticCastSharedPtr<FWidgetBlueprintEditor>(Editor))
	{
		if(!BPEditor->GetPreview())
			return;

		FWidgetReference WidgetReference = BPEditor->GetReferenceFromPreview(this);

		UWidget* TemplateWidgetRaw = WidgetReference.GetTemplate();
		UFastUIWidget* TemplateWidget = Cast<UFastUIWidget>(TemplateWidgetRaw);
		if(TemplateWidget)
		{
			TemplateWidget->ClassNamesArr.Remove(InClassName);
		}
		
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BPEditor->GetWidgetBlueprintObj());
	}
}

void UFastUIWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if( ! FastUIWidget.IsValid() )
	{
		return;
	}

	// 
	RebuildWidget();
}

const FText UFastUIWidget::GetPaletteCategory()
{
	return LOCTEXT("FastUI", "Fast UI");
}

void UFastUIWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	FastUIWidget.Reset();
}

UClass* UFastUIWidget::GetSlotClass() const
{
	return Super::GetSlotClass();
}

void UFastUIWidget::OnSlotAdded(UPanelSlot* InSlot)
{
	if( FastUIWidget.IsValid() )
	{
		TSharedPtr<SWidget> Widget = InSlot->Content->TakeWidget();
		
		FastUIWidget->AddSlot(
			SFastUIWidget::Slot()[
				Widget.ToSharedRef()
			]
		);
	}
}

void UFastUIWidget::OnSlotRemoved(UPanelSlot* InSlot)
{
	if ( FastUIWidget.IsValid() && InSlot->Content )
	{
		TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if ( Widget.IsValid() )
		{
			FastUIWidget->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

TSharedRef<SWidget> UFastUIWidget::RebuildWidget()
{
	SAssignNew(FastUIWidget, SFastUIWidget)
	.ClassNames(ClassNamesArr);

	for ( UPanelSlot* PanelSlot : Slots )
	{
		TSharedPtr<SWidget> Widget = PanelSlot->Content->TakeWidget();
		
		FastUIWidget->AddSlot(
			Widget.ToSharedRef()
		);
	}

	return FastUIWidget.ToSharedRef();
}

#undef LOCTEXT_NAMESPACE
