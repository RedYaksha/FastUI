// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExecutorHandler.h"


/**
 * 
 */
class FASTUI_API SFastUIWidget : public SCompoundWidget
{
public:

	class FSlot : public TBasicLayoutWidgetSlot<FSlot>
	{
	public:
		SLATE_SLOT_BEGIN_ARGS(FSlot, TBasicLayoutWidgetSlot<FSlot>)
		
			SLATE_ATTRIBUTE( TArray<FString>, ClassNames )

			FSlotArguments& operator[](const TSharedRef<SWidget>& InChildWidget)
			{
				AttachWidget(InChildWidget);
				
				return Me();
			}
		
			TArray<typename FSlot::FSlotArguments> _ChildSlots;
			WidgetArgsType& operator [](const TArray<FSlotArguments>& InArgs)
			{
				_ChildSlots = InArgs;
				return Me();
			}
		
			WidgetArgsType& operator [](const TArray<TSharedRef<SWidget>>& WidgetRefs)
			{
				for(auto WidgetRef : WidgetRefs)
				{
					FSlotArguments NewSlot(MakeUnique<FSlot>());
					NewSlot.AttachWidget(WidgetRef);
					_ChildSlots.Add(NewSlot);
				}
				return Me();
			}

			WidgetArgsType& AddSlot() 
			{
				return Me();	
			}

			FSlotArguments(const FSlotArguments& OtherArgs) : FSlotArguments(MakeUnique<FSlot>())
			{
				if(OtherArgs.GetAttachedWidget())
					AttachWidget(OtherArgs.GetAttachedWidget().ToSharedRef());
					
				_ClassNames = OtherArgs._ClassNames;

				for(auto CurChildSlot : OtherArgs._ChildSlots)
				{
					_ChildSlots.Add(MoveTemp(const_cast<FSlotArguments&>(CurChildSlot)));
				}
			}
		SLATE_SLOT_END_ARGS()

			void Construct(const FChildren& SlotOwner, FSlotArguments&& InArgs)
			{
				TBasicLayoutWidgetSlot<FSlot>::Construct(SlotOwner, MoveTemp(InArgs));
				ClassNames = MoveTemp(InArgs._ClassNames);
			}
		
			/** Default values for a slot. */
			FSlot()
				: TBasicLayoutWidgetSlot<FSlot>(HAlign_Fill, VAlign_Fill)
			, ClassNames()
			{ }
		
	private:
		TAttribute<TArray<FString>> ClassNames;
	};
	// ~end FSlot definition

	static FSlot::FSlotArguments Slot()
	{
		return FSlot::FSlotArguments(MakeUnique<FSlot>());
	}

	SLATE_BEGIN_ARGS(SFastUIWidget)
	{}
		SLATE_ATTRIBUTE(TArray<FString>, ClassNames);
	
		SLATE_ARGUMENT(TSharedPtr<SWidget>, MainWidget);
		SLATE_ARGUMENT(TSharedPtr<SBorder>, ParentInnerBorder);

		//
		TArray<typename FSlot::FSlotArguments> _Slots;
		WidgetArgsType& operator + (typename FSlot::FSlotArguments& SlotToAdd) { _Slots.Add( MoveTemp(SlotToAdd) ); return static_cast<WidgetArgsType*>(this)->Me(); }
		WidgetArgsType& operator + (typename FSlot::FSlotArguments&& SlotToAdd) { _Slots.Add( MoveTemp(SlotToAdd) ); return static_cast<WidgetArgsType*>(this)->Me(); }

		WidgetArgsType& operator [](const TArray<FSlot::FSlotArguments>& InArgs)
		{
			_Slots = InArgs;
			return Me();
		}

		WidgetArgsType& operator [](const TArray<TSharedRef<SWidget>>& WidgetRefs)
		{
			for(auto WidgetRef : WidgetRefs)
			{
				FSlot::FSlotArguments NewSlot(MakeUnique<FSlot>());
				NewSlot.AttachWidget(WidgetRef);
				_Slots.Add(NewSlot);
			}
			return Me();
		}
		
		SLATE_ARGUMENT( TArray<FSlot::FSlotArguments>, GenSlots )
	SLATE_END_ARGS()
	
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	TOptional<EHorizontalAlignment> GetHorizontalAlignment() { return HorizontalAlignment; };
	TOptional<EVerticalAlignment> GetVerticalAlignment() { return VerticalAlignment; };

	void AddSlot(FSlot::FSlotArguments& Args);
	void AddSlot(TSharedRef<SWidget> InWidget);
	void RemoveSlot(TSharedRef<SWidget> InWidget);

	void CheckReferences();

	

	//virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	//virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	
	//~ Begin SWidget interface
	virtual int32 OnPaint( const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const override;
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	
	//~ End SWidget interface

	virtual void ShowHoveredStyle();
	virtual void ShowNormalStyle();
	virtual void ShowDisabledStyle();
	virtual void ShowPressedStyle();
protected:
	TSharedPtr<SBorder> OuterBorder;
	TSharedPtr<SBorder> InnerBorder;

	TOptional<EHorizontalAlignment> HorizontalAlignment;
	TOptional<EVerticalAlignment> VerticalAlignment;
	TOptional<FMargin> Padding;
	TOptional<FMargin> Margin;
	TOptional<FBackgroundInfo> BackgroundInfo;
	TOptional<FAnimationInfo> AnimationInfo;

	// TODO: do a GetTypographyInfoReference() so not all SFastUIWidgets need to carry this struct
	TOptional<FTypographyInfo> TypographyInfo;
	TOptional<FIconInfo> IconInfo;

	TSharedPtr<FSlateRoundedBoxBrush> BackgroundBrush;
	TSharedPtr<FSlateRoundedBoxBrush> HoverBrush;
	TSharedPtr<FSlateRoundedBoxBrush> PressedBrush;
	TSharedPtr<FSlateRoundedBoxBrush> DisabledBrush;
	
	TSharedPtr<FExecutorHandler> ExecutorHandler;

	FSlateRenderTransform CurAnimatedTransform;
	float CurAnimatedOpacity;

	// Animation
	FCurveSequence Sequence;
	FCurveHandle Curve;
};
