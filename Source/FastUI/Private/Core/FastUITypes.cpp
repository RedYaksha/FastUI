#include "FastUITypes.h"
#include "SFastUIWidget.h"

template <typename SlotType>
void FLayoutExecutor::HandleStandardSlot(SlotType* Slot, SFastUIWidget& FastUIWidget)
{
	const TOptional<EHorizontalAlignment> HorizontalAlignment = FastUIWidget.GetHorizontalAlignment();
	const TOptional<EVerticalAlignment> VerticalAlignment = FastUIWidget.GetVerticalAlignment();
	if(HorizontalAlignment.IsSet())
	{
		Slot->SetHorizontalAlignment(HorizontalAlignment.GetValue());
	}
	if(VerticalAlignment.IsSet())
	{
		Slot->SetVerticalAlignment(VerticalAlignment.GetValue());
	}
}

template <typename SlotType>
void FLayoutExecutor::AddStandardSlot(SlotType* Slot, TSharedRef<SWidget> InWidget)
{
	bool bChildIsFastUI = FFastUIUtils::WidgetIsFastUIType(InWidget);
	UE_LOG(LogTemp, Log, TEXT("Child is %s"), *InWidget->GetType().ToString())

	if(!bChildIsFastUI)
	{
		auto Children = InWidget->GetAllChildren();
		Children->ForEachWidget([&](SWidget& Widget)
		{
			UE_LOG(LogTemp, Log, TEXT("Subchild is %s"), *Widget.GetType().ToString())
			if(FFastUIUtils::WidgetIsFastUIType(Widget))
			{
				SFastUIWidget& FastUIWidget = static_cast<SFastUIWidget&>(Widget);
				FLayoutExecutor::HandleStandardSlot(Slot, FastUIWidget);
			}
		});
	}
	
	// if the child is a FastUIWidget, it may contain alignment info
	if(bChildIsFastUI)
	{
		TSharedRef<SFastUIWidget> FastUIChild = StaticCastSharedRef<SFastUIWidget>(InWidget);
		FLayoutExecutor::HandleStandardSlot(Slot, FastUIChild.Get());
	}
	
}

void FVerticalBoxExecutor::PerformAction(FWidgetReferences& WidgetReferences)
{
	TSharedPtr<SBorder> Border = StaticCastSharedPtr<SBorder>(WidgetReferences.InnerBorder);
	InnerBorder = Border;
	
	SAssignNew(Box, SVerticalBox);
	Border->SetContent(Box.ToSharedRef());

	for(auto Child : WidgetReferences.ChildWidgets)
	{
		AddSlot(Child);
	}
}

void FVerticalBoxExecutor::AddSlot(TSharedRef<SWidget> InWidget)
{
	check(Box);
	check(InnerBorder);
	TSharedRef<SWidget> Content = InnerBorder->GetContent();
	
	check(Content->GetType() == "SVerticalBox");

	TSharedRef<SVerticalBox> V = StaticCastSharedRef<SVerticalBox>(Content);
	
	SVerticalBox::FSlot* Slot;
	V->AddSlot()
	   .Expose(Slot);

	Slot->SetAutoHeight();
	Slot->AttachWidget(InWidget);

	AddStandardSlot(Slot, InWidget);
}

void FVerticalBoxExecutor::RemoveSlot(TSharedRef<SWidget> InWidget)
{
	if( Box.IsValid() )
	{
		Box->RemoveSlot(InWidget);
	}
}

void FHorizontalBoxExecutor::PerformAction(FWidgetReferences& WidgetReferences)
{
	TSharedPtr<SBorder> Border = StaticCastSharedPtr<SBorder>(WidgetReferences.InnerBorder);
	Border->SetContent(SAssignNew(Box, SHorizontalBox));

	for(auto Child : WidgetReferences.ChildWidgets)
	{
		AddSlot(Child);
	}
}

void FHorizontalBoxExecutor::AddSlot(TSharedRef<SWidget> InWidget)
{
	SHorizontalBox::FSlot* Slot;
	Box->AddSlot()
	   .Expose(Slot);

	Slot->SetAutoWidth();
	Slot->AttachWidget(InWidget);

	AddStandardSlot(Slot, InWidget);
}

void FHorizontalBoxExecutor::RemoveSlot(TSharedRef<SWidget> InWidget)
{
	if( Box.IsValid() )
	{
		Box->RemoveSlot(InWidget);
	}
	
}

void FOverlayExecutor::PerformAction(FWidgetReferences& WidgetReferences)
{
	TSharedPtr<SBorder> Border = StaticCastSharedPtr<SBorder>(WidgetReferences.InnerBorder);
	Border->SetContent(SAssignNew(Overlay, SOverlay));

	for(auto Child : WidgetReferences.ChildWidgets)
	{
		AddSlot(Child);
	}
}

void FOverlayExecutor::AddSlot(TSharedRef<SWidget> InWidget)
{
	SOverlay::FOverlaySlot* Slot;
	Overlay->AddSlot()
	   .Expose(Slot);

	Slot->AttachWidget(InWidget);
	
	AddStandardSlot(Slot, InWidget);
}

void FOverlayExecutor::RemoveSlot(TSharedRef<SWidget> InWidget)
{
	if( Overlay.IsValid() )
	{
		Overlay->RemoveSlot(InWidget);
	}
}

void FSpinAnimationExecutor::CreateAnimationData(const FGeometry& AllottedGeometry, FSlateRenderTransform& OutTransform,
	float& OutOpacity, float Alpha)
{
	const float AngleDeg = FMath::Lerp(0.0, 360.0f, Alpha);
	OutTransform = FSlateRenderTransform(FQuat2D(FMath::DegreesToRadians(AngleDeg)));
}

void FPingAnimationExecutor::CreateAnimationData(const FGeometry& AllottedGeometry, FSlateRenderTransform& OutTransform,
	float& OutOpacity, float Alpha)
{
	const float Scale = FMath::Lerp(1, 1.7, Alpha);
	OutTransform = FSlateRenderTransform(Scale);
	OutOpacity = FMath::Lerp(0, 1., Alpha);
}

void FPulseAnimationExecutor::CreateAnimationData(const FGeometry& AllottedGeometry,
	FSlateRenderTransform& OutTransform, float& OutOpacity, float Alpha)
{
	OutOpacity = FMath::Lerp(0, 1., Alpha);
}
