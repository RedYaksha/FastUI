// Fill out your copyright notice in the Description page of Project Settings.
#include "..\..\Public\Core\SFastUIWidget.h"

#include "FastUI.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SFastUIWidget::Construct(const FArguments& InArgs)
{
	SAssignNew(OuterBorder, SBorder)
	.Padding(0)
	.BorderImage(nullptr)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	;
	
	SAssignNew(InnerBorder, SBorder)
	.Padding(0)
	.BorderImage(nullptr)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	;

	OuterBorder->SetContent(InnerBorder.ToSharedRef());


	TSharedPtr<SBorder> ParentInnerBorder = InArgs._ParentInnerBorder;
	if(ParentInnerBorder.IsValid())
	{
		// set child of ParentInnerBorder to InnerBorder
		// set child of outer border to ParentInnerBorder
		ParentInnerBorder->SetContent(InnerBorder.ToSharedRef());
		OuterBorder->SetContent(ParentInnerBorder.ToSharedRef());
	}

	// class names (FString) -> executors
	ExecutorHandler = MakeShareable(new FExecutorHandler());

	FWidgetReferences WidgetReferences(HorizontalAlignment, VerticalAlignment, 
		Margin, Padding, BackgroundInfo, TypographyInfo, IconInfo, AnimationInfo);
	
	WidgetReferences.OuterBorder = OuterBorder;
	WidgetReferences.InnerBorder = InnerBorder;
	
	WidgetReferences.ChildWidgets.Empty();
	
	if(InArgs._MainWidget.IsValid())
	{
		// InnerBorder->SetContent(InArgs._MainWidget.ToSharedRef());
		WidgetReferences.ChildWidgets.Add(InArgs._MainWidget.ToSharedRef());
	}
	
	for(auto S : InArgs._Slots)
	{
		WidgetReferences.ChildWidgets.Add(S.GetAttachedWidget().ToSharedRef());
	}

	for(auto S : InArgs._GenSlots)
	{
		WidgetReferences.ChildWidgets.Add(S.GetAttachedWidget().ToSharedRef());
	}
	
	ExecutorHandler->Execute(InArgs._ClassNames.Get(), WidgetReferences);

	// because padding/margin is an accumulative mechanism, we expect the executor to
	// directly modify the Margin and Padding variables. We apply them here, if they were
	// even set in the first place
	if(Margin.IsSet())
	{
		UE_LOG(LogFastUI, Log, TEXT("Accumulated margin: %s"), *Margin.GetValue().GetDesiredSize().ToString())
		OuterBorder->SetPadding(Margin.GetValue());
	}
	if(Padding.IsSet())
	{
		UE_LOG(LogFastUI, Log, TEXT("Accumulated padding: %s"), *Padding.GetValue().GetDesiredSize().ToString())
		InnerBorder->SetPadding(Padding.GetValue());
	}
	if(BackgroundInfo.IsSet())
	{
		FBackgroundInfo& BGInfo = BackgroundInfo.GetValue();
		
		float BorderWidth = 0.f;
		FLinearColor BackgroundColor = FLinearColor::Transparent;
		FLinearColor BorderColor = FLinearColor::Transparent;
		FVector4 Radii(0.f);

		if(BGInfo.Color.IsSet()) { BackgroundColor = BGInfo.Color.GetValue(); }
		if(BGInfo.BorderWidth.IsSet()) { BorderWidth = BGInfo.BorderWidth.GetValue(); }
		if(BGInfo.BorderColor.IsSet()) { BorderColor = BGInfo.BorderColor.GetValue(); }
		
		if(BGInfo.RoundedRadiiInfo.IsSet())
		{
			FRoundedRadiiInfo& RRInfo = BGInfo.RoundedRadiiInfo.GetValue();
			if(RRInfo.TopLeft.IsSet()) { Radii.X = RRInfo.TopLeft.GetValue(); }
			if(RRInfo.TopRight.IsSet()) { Radii.Y = RRInfo.TopRight.GetValue(); }
			if(RRInfo.BottomRight.IsSet()) { Radii.Z = RRInfo.BottomRight.GetValue(); }
			if(RRInfo.BottomLeft.IsSet()) { Radii.W = RRInfo.BottomLeft.GetValue(); }
		}
		
		BackgroundBrush = MakeShareable(new FSlateRoundedBoxBrush(BackgroundColor, Radii,
			BorderColor, BorderWidth));

		// can be overridden by hover: state class name
		FLinearColor HoverColor = BackgroundColor * 0.8;
		HoverColor.A = 1.f;
		HoverBrush = MakeShareable(new FSlateRoundedBoxBrush(HoverColor, Radii,
			BorderColor, BorderWidth));
		
		FLinearColor PressedColor = BackgroundColor * 0.7;
		PressedColor.A = 1.f;
		PressedBrush = MakeShareable(new FSlateRoundedBoxBrush(PressedColor, Radii,
			BorderColor, BorderWidth));
		
		FLinearColor DisabledColor = BackgroundColor * 0.6;
		PressedColor.A = 1.f;
		PressedBrush = MakeShareable(new FSlateRoundedBoxBrush(DisabledColor, Radii,
			BorderColor, BorderWidth));

		InnerBorder->SetBorderImage(BackgroundBrush.Get());
	}
	
	ChildSlot
	[
		OuterBorder.ToSharedRef()
	];

	// animations data
	if(ExecutorHandler->HasAnimationExecutors())
	{
		int Iterations = -1;
		ETimingFunction TimingFunction = ETimingFunction::EASE_LINEAR;
		EAnimationDirection Direction = EAnimationDirection::NORMAL;
		int Duration = 1000; // ms
		int Delay = 0; // ms

		if(AnimationInfo.IsSet())
		{
			FAnimationInfo& AI = AnimationInfo.GetValue();
			if(AI.Duration.IsSet()) { Duration = AI.Duration.GetValue(); }
			if(AI.Delay.IsSet()) { Delay = AI.Delay.GetValue(); }
			if(AI.TimingFunction.IsSet()) { TimingFunction = AI.TimingFunction.GetValue(); }
			if(AI.Iterations.IsSet()) { Iterations = AI.Iterations.GetValue(); }
			if(AI.Direction.IsSet()) { Direction = AI.Direction.GetValue(); }
		}

		static TMap<ETimingFunction, ECurveEaseFunction> CurveFunctionMap = {
			{ETimingFunction::EASE_LINEAR, ECurveEaseFunction::Linear},
			{ETimingFunction::EASE_OUT, ECurveEaseFunction::CubicOut},
			{ETimingFunction::EASE_IN, ECurveEaseFunction::CubicIn},
			{ETimingFunction::EASE_IN_OUT, ECurveEaseFunction::CubicInOut},
		};
		check(CurveFunctionMap.Contains(TimingFunction));
		
		Sequence = FCurveSequence();
		
		Curve = Sequence.AddCurve(
			(float) Delay / 1000.f,
			(float) Duration / 1000.f,
						CurveFunctionMap[TimingFunction]);
		
		Sequence.Play(AsShared(), true, 0.0f, false);

		SetRenderTransformPivot(FVector2D(0.5, 0.5));
	}
	
}

void SFastUIWidget::AddSlot(FSlot::FSlotArguments& Args)
{
	if(ExecutorHandler.IsValid())
	{
		bool bSuccess = false;
		if(Args._ChildSlots.Num() == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Adding slot's widget directly."))
			bSuccess = ExecutorHandler->TryAddSlot(Args.GetAttachedWidget().ToSharedRef());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Adding slot's children..."))
			for(auto S : Args._ChildSlots)
			{
				UE_LOG(LogTemp, Log, TEXT("Adding child slot..."))
				bSuccess = ExecutorHandler->TryAddSlot(S.GetAttachedWidget().ToSharedRef());
			}
		}
		
		if(!bSuccess)
		{
			UE_LOG(LogFastUI, Warning, TEXT("AddSlot() failed."))
		}
	}
	else
	{
		UE_LOG(LogFastUI, Warning, TEXT("AddSlot() failed. No ExecutorHandler found."))
	}
}

void SFastUIWidget::AddSlot(TSharedRef<SWidget> InWidget)
{
	if(ExecutorHandler.IsValid())
	{
		const bool bSuccess = ExecutorHandler->TryAddSlot(InWidget);
		
		if(!bSuccess)
		{
			UE_LOG(LogFastUI, Warning, TEXT("AddSlot() failed."))
		}
	}
	else
	{
		UE_LOG(LogFastUI, Warning, TEXT("AddSlot() failed. No ExecutorHandler found."))
	}
}

void SFastUIWidget::RemoveSlot(TSharedRef<SWidget> InWidget)
{
	if(ExecutorHandler.IsValid())
	{
		const bool bSuccess = ExecutorHandler->TryRemoveSlot(InWidget);
		if(!bSuccess)
		{
			UE_LOG(LogFastUI, Warning, TEXT("RemoveSlot() failed."))
		}
	}
	else
	{
		UE_LOG(LogFastUI, Warning, TEXT("RemoveSlot() failed. No ExecutorHandler found."))
	}
}

void SFastUIWidget::CheckReferences()
{
	/*
	TSharedRef<SBorder> B = InnerBorder.ToSharedRef();
	TSharedRef<SBorder> BB = ExecutorHandler->GetParentBorder();
	check(B == BB);
	*/
}

int32 SFastUIWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                                bParentEnabled);
}

void SFastUIWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	TSharedRef<SWidget> Widget = ChildSlot.GetWidget();
	Widget->SlatePrepass(AllottedGeometry.GetAccumulatedLayoutTransform().GetScale());

	// More complex animations are by Material
	FGeometry ChildGeometry = AllottedGeometry.MakeChild(
		Widget,
		Widget->GetDesiredSize(),
		FSlateLayoutTransform(1)).Geometry;

	FSlateRenderTransform OutTransform;
	float OutOpacity = 1.0f;
	if(ExecutorHandler && ExecutorHandler->HasAnimationExecutors())
	{
		if(!Sequence.IsPlaying())
		{
			return;
		}
		
		ExecutorHandler->CreateAnimationData(ChildGeometry, OutTransform, OutOpacity, Sequence.GetLerp());
		
		CurAnimatedOpacity = OutOpacity;
		CurAnimatedTransform = OutTransform;
		
		SetRenderOpacity(CurAnimatedOpacity);
		SetRenderTransform(CurAnimatedTransform);
	}

}

void SFastUIWidget::ShowHoveredStyle()
{
	if(InnerBorder && HoverBrush)
	{
		InnerBorder->SetBorderImage(HoverBrush.Get());
	}
}

void SFastUIWidget::ShowNormalStyle()
{
	if(InnerBorder && BackgroundBrush)
	{
		InnerBorder->SetBorderImage(BackgroundBrush.Get());
	}
}

void SFastUIWidget::ShowDisabledStyle()
{
	if(InnerBorder && DisabledBrush)
	{
		InnerBorder->SetBorderImage(DisabledBrush.Get());
	}
}

void SFastUIWidget::ShowPressedStyle()
{
	if(InnerBorder && PressedBrush)
	{
		InnerBorder->SetBorderImage(PressedBrush.Get());
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
