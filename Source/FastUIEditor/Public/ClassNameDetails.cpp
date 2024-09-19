#include "ClassNameDetails.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "FastUIWidget.h"
#include "ISinglePropertyView.h"
#include "SChips.h"
#include "SFastUITextBlock.h"
#include "SFastUIWidget.h"
#include "WidgetBlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "FastUIClassNameDetails"
TSharedRef<IDetailCustomization> FClassNameDetails::MakeInstance()
{
	return MakeShareable(new FClassNameDetails);
}

void FClassNameDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	DetailLayoutBuilder = &DetailBuilder;

	IDetailCategoryBuilder& ClassNameCategory = DetailBuilder.EditCategory(TEXT("Fast UI ClassNames"));
	ClassNameCategory.SetSortOrder(0);
	ClassNameCategory.InitiallyCollapsed(false);
	
	ClassNameCategory.HeaderContent(
		SNew(SFastUIWidget)
		.ClassNames(TArray<FString>({"hbox", "m-2", "p-1", "bg-neutral-600", "rounded-lg"}))
		[{
			SNew(SFastUITextBlock)
			.Text(FText::FromString("Class Name Editor"))
			.ClassNames(TArray<FString>({"text-bold", "text-neutral-200", "text-sm", "font-helveticaneue"})),
		}]
	);

	TSharedPtr<SChips> ChipsWidget;
	
	TWeakObjectPtr<UFastUIWidget> FastUIWidget = GetFastUIWidget();
	
	FSinglePropertyParams Params;
	Params.Font = FAppStyle::GetFontStyle("Bold");
	Params.NamePlacement = EPropertyNamePlacement::Left;
	
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>( TEXT( "PropertyEditor" ) );
	PropertyView = PropertyEditor.CreateSingleProperty(
		FastUIWidget.Get(),
		GET_MEMBER_NAME_CHECKED( UFastUIWidget, ClassNames),
		Params
	);

	// create a chip like ui with tags, categories, color based on category
	// can also click from a tooltip to insert class names
	ClassNameCategory.AddCustomRow(LOCTEXT("Test", "Test"), false)
	.WholeRowContent()
	[
		SAssignNew(ChipsWidget, SChips)
		.OnClassNameAdded_Raw(this, &FClassNameDetails::OnClassNameAdded)
		.OnClassNameRemoved_Raw(this, &FClassNameDetails::OnClassNameRemoved)
	];

	if(FastUIWidget.IsValid())
	{
		ChipsWidget->SetClassNames(FastUIWidget->GetClassNames());
	}
}

void FClassNameDetails::OnClassNameAdded(FString InClassName)
{
	TWeakObjectPtr<UFastUIWidget> FastUIWidget = GetFastUIWidget();
	if(FastUIWidget.IsValid())
	{
		FastUIWidget->AddClassName(InClassName);
		

		auto P = PropertyView->GetPropertyHandle();
		auto PP = P->GetProperty();

		
		//FPropertyChangedEvent Event(PP);
		//FastUIWidget->PostEditChangeProperty(Event);
		
		//CustomizableObject->PostEditChangeProperty(Event);
		
		
		FastUIWidget->Modify();
	}
}

void FClassNameDetails::OnClassNameRemoved(FString InClassName)
{
	TWeakObjectPtr<UFastUIWidget> FastUIWidget = GetFastUIWidget();
	if(FastUIWidget.IsValid())
	{
		FastUIWidget->RemoveClassName(InClassName);
		FastUIWidget->Modify();
	}
	
}

TWeakObjectPtr<UFastUIWidget> FClassNameDetails::GetFastUIWidget()
{
	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailLayoutBuilder->GetObjectsBeingCustomized(Objects);

	for(auto O : Objects)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *O->GetFullName());
		TWeakObjectPtr<UFastUIWidget> Widget = Cast<UFastUIWidget>(O);
		if(Widget.IsValid())
		{
			return Widget;
		}
	}

	return nullptr;
}

#undef LOCTEXT_NAMESPACE
