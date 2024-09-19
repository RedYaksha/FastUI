#pragma once
#include "CoreMinimal.h"

#define SET_OPTIONAL_VALUE(VarName, ObjName) if(Params.VarName.IsSet()) { ObjName.VarName = Params.VarName.GetValue(); }
#define CONDITIONAL_INIT_OPTIONAL_STRUCT(StructObj, StructName) if(!StructObj.IsSet()) StructObj = StructName();

UENUM()
enum class EStateQueryType
{
	WIDTH,
	HEIGHT,
};

UENUM()
enum class EFastUIClassEffectType
{
	// styling
	MARGIN,
	PADDING,
	ROUNDED_BACKGROUND,
	BORDER,
	BACKGROUND_COLOR,
	FOREGROUND_COLOR,

	// typography 
	FONT_SIZE,
	FONT_FAMILY,
	FONT_WEIGHT,
	LINE_HEIGHT,
	TEXT_ALIGN,
	
	WIDTH,
	HEIGHT,

	// layout
	VERTICAL_BOX,
	HORIZONTAL_BOX,
	GRID,
	OVERLAY,

	HALIGN_LEFT,
	HALIGN_CENTER,
	HALIGN_RIGHT,
	
	VALIGN_LEFT,
	VALIGN_CENTER,
	VALIGN_RIGHT,

	//
};

struct FRoundedRadiiInfo
{
	FRoundedRadiiInfo(const TOptional<float>& TopLeft, const TOptional<float>& TopRight,
		const TOptional<float>& BottomRight, const TOptional<float>& BottomLeft)
		: TopLeft(TopLeft),
		  TopRight(TopRight),
		  BottomRight(BottomRight),
		  BottomLeft(BottomLeft)
	{
	}

	FRoundedRadiiInfo() {}

	TOptional<float> TopLeft;
	TOptional<float> TopRight;
	TOptional<float> BottomRight;
	TOptional<float> BottomLeft;
};

struct FBackgroundInfo
{
	FBackgroundInfo(const TOptional<FLinearColor>& Color, const TOptional<FRoundedRadiiInfo>& RoundedRadiiInfo,
		const TOptional<int>& BorderWidth, const TOptional<FLinearColor>& BorderColor)
		: Color(Color),
		  RoundedRadiiInfo(RoundedRadiiInfo),
		  BorderWidth(BorderWidth),
		  BorderColor(BorderColor)
	{}

	FBackgroundInfo() {}

	TOptional<FLinearColor> Color;
	TOptional<FRoundedRadiiInfo> RoundedRadiiInfo;
	TOptional<int> BorderWidth;
	TOptional<FLinearColor> BorderColor;
	
};

enum class EFontWeight
{
	THIN,
	EXTRALIGHT,
	LIGHT,
	NORMAL,
	MEDIUM,
	SEMIBOLD,
	BOLD,
	EXTRABOLD,
	BLACK
};

struct FTypographyInfo
{
	FTypographyInfo(const TOptional<FString>& FontName, const TOptional<EFontWeight>& FontWeight,
		const TOptional<bool>& bIsItalic, const TOptional<int>& Size, const TOptional<FLinearColor>& Color,
		const TOptional<ETextJustify::Type>& Justification)
		: FontName(FontName),
		  FontWeight(FontWeight),
		  bIsItalic(bIsItalic),
		  Size(Size),
		  Color(Color),
		  Justification(Justification)
	{}

	FTypographyInfo() {}

	TOptional<FString> FontName;
	TOptional<EFontWeight> FontWeight;
	TOptional<bool> bIsItalic;
	
	TOptional<int> Size;
	TOptional<FLinearColor> Color;
	TOptional<ETextJustify::Type> Justification;
};

struct FIconInfo
{
	FIconInfo(const TOptional<FString>& IconName, const TOptional<FLinearColor>& Color)
		: IconName(IconName),
		  Color(Color)
	{}

	FIconInfo() {}

	TOptional<FString> IconName;
	TOptional<FLinearColor> Color;
};

enum class ETimingFunction
{
	EASE_LINEAR,
	EASE_IN,
	EASE_OUT,
	EASE_IN_OUT
};

enum class EAnimationDirection
{
	NORMAL,
	REVERSE,
	ALTERNATE,
	ALTERNATE_REVERSE
};

struct FAnimationInfo
{
	FAnimationInfo(const TOptional<int>& Duration, const TOptional<int>& Delay,
		const TOptional<ETimingFunction>& TimingFunction, const TOptional<int>& Iterations,
		const TOptional<EAnimationDirection>& Direction)
		: Duration(Duration),
		  Delay(Delay),
		  TimingFunction(TimingFunction),
		  Iterations(Iterations),
		  Direction(Direction)
	{}

	FAnimationInfo() = default;
	
	TOptional<int> Duration; //ms
	TOptional<int> Delay; // ms
	TOptional<ETimingFunction> TimingFunction;
	TOptional<int> Iterations; // < 0 => infinite
	TOptional<EAnimationDirection> Direction;
};

struct FWidgetReferences
{
	FWidgetReferences(TOptional<EHorizontalAlignment>& HorizontalAlignmentValue,
		TOptional<EVerticalAlignment>& VerticalAlignmentValue, TOptional<FMargin>& MarginValue,
		TOptional<FMargin>& PaddingValue, TOptional<FBackgroundInfo>& BackgroundInfo,
		TOptional<FTypographyInfo>& TypographyInfo, TOptional<FIconInfo>& IconInfo,
		TOptional<FAnimationInfo>& AnimationInfo)
		: HorizontalAlignmentValue(HorizontalAlignmentValue),
		  VerticalAlignmentValue(VerticalAlignmentValue),
		  MarginValue(MarginValue),
		  PaddingValue(PaddingValue),
		  BackgroundInfo(BackgroundInfo),
		  TypographyInfo(TypographyInfo),
		  IconInfo(IconInfo),
		  AnimationInfo(AnimationInfo)
	{
	}

	TSharedPtr<SWidget> OuterBorder;
	TSharedPtr<SWidget> InnerBorder;
	TSharedPtr<SWidget> MainWidget;

	TArray< TSharedRef<SWidget> > ChildWidgets;

	TOptional<EHorizontalAlignment>& HorizontalAlignmentValue;
	TOptional<EVerticalAlignment>& VerticalAlignmentValue;

	TOptional<FMargin>& MarginValue;
	TOptional<FMargin>& PaddingValue;

	TOptional<FBackgroundInfo>& BackgroundInfo;

	TOptional<FTypographyInfo>& TypographyInfo;
	TOptional<FIconInfo>& IconInfo;
	TOptional<FAnimationInfo>& AnimationInfo;
};

class FFastUIClassExecutor
{
public:
	FFastUIClassExecutor()
	{}
	virtual ~FFastUIClassExecutor() = default;
	
	virtual void PerformAction(FWidgetReferences& WidgetReferences) {}
	virtual bool IsLayout() { return false; }
	virtual bool IsAnimation() { return false; }
	virtual bool HasState() { return false; }
	virtual TSharedPtr<FFastUIClassExecutor> Copy() { return nullptr; }
	virtual FString GetCategory() const { return "Uncategorized"; }
};

struct FMarginExecutorParams
{
	FMarginExecutorParams(const TOptional<float>& MarginLeft, const TOptional<float>& MarginRight,
		const TOptional<float>& MarginTop, const TOptional<float>& MarginBottom)
		: MarginLeft(MarginLeft),
		  MarginRight(MarginRight),
		  MarginTop(MarginTop),
		  MarginBottom(MarginBottom)
	{}

	FMarginExecutorParams() {}

	TOptional<float> MarginLeft;
	TOptional<float> MarginRight;
	TOptional<float> MarginTop;
	TOptional<float> MarginBottom;
};

class FMarginExecutor : public FFastUIClassExecutor
{
public:
	FMarginExecutorParams Params;
	
	FMarginExecutor(FMarginExecutorParams Params)
		: Params(Params)
	{}
	
	virtual FString GetCategory() const { return "Spacing"; }

	virtual void PerformAction(FWidgetReferences& WidgetReferences) override
	{
		TSharedPtr<SBorder> Border = StaticCastSharedPtr<SBorder>(WidgetReferences.OuterBorder);
		
		if(!WidgetReferences.MarginValue.IsSet())
		{
			WidgetReferences.MarginValue = FMargin(0);
		}

		FMargin& OutMargin = WidgetReferences.MarginValue.GetValue();
		
		if(Params.MarginLeft.IsSet())
		{
			OutMargin.Left = Params.MarginLeft.GetValue();
		}
		if(Params.MarginRight.IsSet())
		{
			OutMargin.Right = Params.MarginRight.GetValue();
		}
		if(Params.MarginTop.IsSet())
		{
			OutMargin.Top = Params.MarginTop.GetValue();
			
		}
		if(Params.MarginBottom.IsSet())
		{
			OutMargin.Bottom = Params.MarginBottom.GetValue();
		}

		// Border.Pin()->SetPadding(OutMargin);
	}
};

struct FPaddingExecutorParams
{
	FPaddingExecutorParams(const TOptional<float>& PaddingLeft, const TOptional<float>& PaddingRight,
		const TOptional<float>& PaddingTop, const TOptional<float>& PaddingBottom)
		: PaddingLeft(PaddingLeft),
		  PaddingRight(PaddingRight),
		  PaddingTop(PaddingTop),
		  PaddingBottom(PaddingBottom)
	{
	}

	FPaddingExecutorParams() {}

	TOptional<float> PaddingLeft;
	TOptional<float> PaddingRight;
	TOptional<float> PaddingTop;
	TOptional<float> PaddingBottom;
};

class FPaddingExecutor : public FFastUIClassExecutor
{
public:
	FPaddingExecutorParams Params;

	FPaddingExecutor(FPaddingExecutorParams Params)
		: Params(Params)
	{}
	
	virtual FString GetCategory() const { return "Spacing"; }

	virtual void PerformAction(FWidgetReferences& WidgetReferences) override
	{
		TSharedPtr<SBorder> Border = StaticCastSharedPtr<SBorder>(WidgetReferences.InnerBorder);
		if(!Border.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("Executor expected border."))
			return;
		}
		
		if(!WidgetReferences.PaddingValue.IsSet())
		{
			WidgetReferences.PaddingValue = FMargin(0);
		}

		FMargin& OutPadding = WidgetReferences.PaddingValue.GetValue();

		if(Params.PaddingLeft.IsSet())
		{
			OutPadding.Left = Params.PaddingLeft.GetValue();
		}
		if(Params.PaddingRight.IsSet())
		{
			OutPadding.Right = Params.PaddingRight.GetValue();
		}
		if(Params.PaddingTop.IsSet())
		{
			OutPadding.Top = Params.PaddingTop.GetValue();
			
		}
		if(Params.PaddingBottom.IsSet())
		{
			OutPadding.Bottom = Params.PaddingBottom.GetValue();
		}

		// Border.Pin()->SetPadding(OutPadding);
	}
};

class FRoundedBackgroundExecutor : public FFastUIClassExecutor
{
public:
	FRoundedRadiiInfo Params;

	FRoundedBackgroundExecutor(FRoundedRadiiInfo Params) :
	Params(Params) {}
	
	virtual FString GetCategory() const { return "Background Style"; }

	virtual void PerformAction(FWidgetReferences& WidgetReferences) override
	{
		if(!WidgetReferences.BackgroundInfo.IsSet())
		{
			WidgetReferences.BackgroundInfo = FBackgroundInfo();
			WidgetReferences.BackgroundInfo.GetValue().RoundedRadiiInfo = FRoundedRadiiInfo();
		}

		FBackgroundInfo& BackgroundInfo = WidgetReferences.BackgroundInfo.GetValue();

		if(!BackgroundInfo.RoundedRadiiInfo.IsSet())
		{
			BackgroundInfo.RoundedRadiiInfo = FRoundedRadiiInfo();
		}
		
		FRoundedRadiiInfo& RoundedRadiiInfo = BackgroundInfo.RoundedRadiiInfo.GetValue();
		if(Params.TopLeft.IsSet())
		{
			RoundedRadiiInfo.TopLeft = Params.TopLeft.GetValue();
		}
		if(Params.TopRight.IsSet())
		{
			RoundedRadiiInfo.TopRight = Params.TopRight.GetValue();
		}
		if(Params.BottomRight.IsSet())
		{
			RoundedRadiiInfo.BottomRight = Params.BottomRight.GetValue();
		}
		if(Params.BottomLeft.IsSet())
		{
			RoundedRadiiInfo.BottomLeft = Params.BottomLeft.GetValue();
		}
	}

	TSharedPtr<FSlateBrush> ColorBrush;
};


struct FGenericColorParams
{
	FLinearColor Color;
};

class FBackgroundExecutor : public FFastUIClassExecutor
{
public:
	FBackgroundInfo Params;

	FBackgroundExecutor(FBackgroundInfo Params) :
	Params(Params) {}
	
	virtual FString GetCategory() const { return "Background Style"; }

	virtual void PerformAction(FWidgetReferences& WidgetReferences) override
	{
		if(!WidgetReferences.BackgroundInfo.IsSet())
		{
			WidgetReferences.BackgroundInfo = FBackgroundInfo();
		}

		FBackgroundInfo& BackgroundInfo = WidgetReferences.BackgroundInfo.GetValue();
		if(Params.Color.IsSet()) { BackgroundInfo.Color = Params.Color; }
		if(Params.BorderColor.IsSet()) { BackgroundInfo.BorderColor = Params.BorderColor; }
		if(Params.BorderWidth.IsSet()) { BackgroundInfo.BorderWidth = Params.BorderWidth; }
	}

	TSharedPtr<FSlateBrush> ColorBrush;
};

class FForegroundExecutor : public FFastUIClassExecutor
{
public:
	FGenericColorParams Params;

	FForegroundExecutor(FGenericColorParams Params) :
	Params(Params) {}

	virtual void PerformAction(FWidgetReferences& WidgetReferences) override
	{
		// TODO
		ColorBrush = MakeShareable(new FSlateColorBrush(Params.Color));
		
		TSharedPtr<SBorder> Border = StaticCastSharedPtr<SBorder>(WidgetReferences.InnerBorder);
		if(Border.IsValid())
		{
			Border->SetBorderImage(ColorBrush.Get());
		}
	}

	TSharedPtr<FSlateBrush> ColorBrush;
};


class FIconExecutor : public FFastUIClassExecutor
{
public:
	FIconInfo Params;
	
	FIconExecutor(FIconInfo Params) :
	Params(Params) {}
	
	virtual FString GetCategory() const { return "Icon"; }
	
	virtual void PerformAction(FWidgetReferences& WidgetReferences) override
	{
		if(!WidgetReferences.IconInfo.IsSet())
		{
			WidgetReferences.IconInfo = FIconInfo();
		}

		FIconInfo& IconInfo = WidgetReferences.IconInfo.GetValue();
		if(Params.IconName.IsSet())
		{
			IconInfo.IconName = Params.IconName.GetValue();
		}
		if(Params.Color.IsSet())
		{
			IconInfo.Color = Params.Color.GetValue();
		}
		
	}
};

class FTypographyExecutor : public FFastUIClassExecutor
{
public:
	FTypographyInfo Params;
	
	FTypographyExecutor(FTypographyInfo Params) :
	Params(Params) {}
	
	virtual FString GetCategory() const { return "Typography"; }

	virtual void PerformAction(FWidgetReferences& WidgetReferences) override
	{
		if(!WidgetReferences.TypographyInfo.IsSet())
		{
			WidgetReferences.TypographyInfo = FTypographyInfo();
		}

		FTypographyInfo& TypographyInfo = WidgetReferences.TypographyInfo.GetValue();
		if(Params.FontName.IsSet())
		{
			TypographyInfo.FontName = Params.FontName.GetValue();
		}
		if(Params.bIsItalic.IsSet()) 
		{
			TypographyInfo.bIsItalic = Params.bIsItalic.GetValue();
		}
		if(Params.FontWeight.IsSet())
		{
			TypographyInfo.FontWeight = Params.FontWeight.GetValue();
		}
		if(Params.Size.IsSet())
		{
			TypographyInfo.Size = Params.Size.GetValue();
		}
		if(Params.Color.IsSet())
		{
			TypographyInfo.Color = Params.Color.GetValue();
		}
		if(Params.Justification.IsSet())
		{
			TypographyInfo.Justification = Params.Justification.GetValue();
			
		}
	}
};

struct FAlignmentParams
{
	FAlignmentParams(const TOptional<EHorizontalAlignment>& HorizontalAlignment,
		const TOptional<EVerticalAlignment>& VerticalAlignment)
		: HorizontalAlignment(HorizontalAlignment),
		  VerticalAlignment(VerticalAlignment)
	{}

	FAlignmentParams() {}

	TOptional<EHorizontalAlignment> HorizontalAlignment;
	TOptional<EVerticalAlignment> VerticalAlignment;
};

class FAlignmentExecutor : public FFastUIClassExecutor
{
public:
	FAlignmentParams Params;

	FAlignmentExecutor(FAlignmentParams Params) :
	Params(Params) {}
	
	virtual FString GetCategory() const { return "Layout"; }

	virtual void PerformAction(FWidgetReferences& WidgetReferences) override
	{
		if(Params.HorizontalAlignment.IsSet())
		{
			WidgetReferences.HorizontalAlignmentValue = Params.HorizontalAlignment.GetValue();
		}
		
		if(Params.VerticalAlignment.IsSet())
		{
			WidgetReferences.VerticalAlignmentValue = Params.VerticalAlignment.GetValue();
		}
	}

	TSharedPtr<FSlateBrush> ColorBrush;
};

class FLayoutExecutor : public FFastUIClassExecutor
{
public:
	virtual FString GetCategory() const { return "Layout"; }
	virtual void AddSlot(TSharedRef<SWidget> InWidget) {};
	virtual void RemoveSlot(TSharedRef<SWidget> InWidget) {};
	virtual bool IsLayout() override { return true; }
	virtual bool HasState() override { return true; }
	
	virtual TSharedRef<SWidget> GetLayoutWidget() { return SNullWidget::NullWidget;}
	virtual TSharedRef<SBorder> GetParentBorder() { return InnerBorder.ToSharedRef();}

	template <typename SlotType>
	static void HandleStandardSlot(SlotType* Slot, class SFastUIWidget& FastUIWidget);

	template <typename SlotType>
	static void AddStandardSlot(SlotType* Slot, TSharedRef<SWidget> ChildWidget);

	TSharedPtr<SBorder> InnerBorder;
};


class FVerticalBoxExecutor : public FLayoutExecutor
{
public:
	FVerticalBoxExecutor() = default;
	
	virtual void PerformAction(FWidgetReferences& WidgetReferences) override;
	virtual void AddSlot(TSharedRef<SWidget> InWidget) override;
	virtual void RemoveSlot(TSharedRef<SWidget> InWidget) override;
	virtual TSharedPtr<FFastUIClassExecutor> Copy() { return MakeShareable(new FVerticalBoxExecutor); };
	virtual TSharedRef<SWidget> GetLayoutWidget() { return Box.ToSharedRef(); }
	
protected:
	TSharedPtr<SVerticalBox> Box;
};

class FHorizontalBoxExecutor : public FLayoutExecutor
{
public:
	FHorizontalBoxExecutor() = default;
	
	virtual void PerformAction(FWidgetReferences& WidgetReferences) override;
	virtual void AddSlot(TSharedRef<SWidget> InWidget) override;
	virtual void RemoveSlot(TSharedRef<SWidget> InWidget) override;
	virtual TSharedRef<SWidget> GetLayoutWidget() { return Box.ToSharedRef(); }
	virtual TSharedPtr<FFastUIClassExecutor> Copy() { return MakeShareable(new FHorizontalBoxExecutor); };
	
protected:
	TSharedPtr<SHorizontalBox> Box;
};

class FOverlayExecutor : public FLayoutExecutor
{
public:
	FOverlayExecutor() = default;
	
	virtual void PerformAction(FWidgetReferences& WidgetReferences) override;
	virtual void AddSlot(TSharedRef<SWidget> InWidget) override;
	virtual void RemoveSlot(TSharedRef<SWidget> InWidget) override;
	virtual TSharedRef<SWidget> GetLayoutWidget() { return Overlay.ToSharedRef(); }
	virtual TSharedPtr<FFastUIClassExecutor> Copy() { return MakeShareable(new FOverlayExecutor); };
	
protected:
	TSharedPtr<SOverlay> Overlay;
};

class FTransformExecutor : public FFastUIClassExecutor
{
public:
	
};

class FOnPaintExecutor : public FFastUIClassExecutor
{
public:
	
};

class FAnimationInfoExecutor : public FFastUIClassExecutor
{
public:
	FAnimationInfo Params;
	
	FAnimationInfoExecutor(FAnimationInfo Params) :
	Params(Params) {}
	
	virtual FString GetCategory() const { return "Animation"; }
	virtual void PerformAction(FWidgetReferences& WidgetReferences) override
	{
		CONDITIONAL_INIT_OPTIONAL_STRUCT(WidgetReferences.AnimationInfo, FAnimationInfo);
		FAnimationInfo& AnimationInfo = WidgetReferences.AnimationInfo.GetValue();
		SET_OPTIONAL_VALUE(Duration, AnimationInfo)
		SET_OPTIONAL_VALUE(Delay, AnimationInfo)
		SET_OPTIONAL_VALUE(TimingFunction, AnimationInfo)
		SET_OPTIONAL_VALUE(Iterations, AnimationInfo)
		SET_OPTIONAL_VALUE(Direction, AnimationInfo)
	}
};

class FAnimationExecutor : public FFastUIClassExecutor
{
public:
	virtual FString GetCategory() const { return "Animation"; }
	virtual bool IsAnimation() override { return true; }

	virtual void CreateAnimationData(const FGeometry& AllottedGeometry, FSlateRenderTransform& OutTransform, float& OutOpacity, float Alpha) {}
};

class FSpinAnimationExecutor : public FAnimationExecutor
{
public:
	virtual void CreateAnimationData(const FGeometry& AllottedGeometry, FSlateRenderTransform& OutTransform, float& OutOpacity, float Alpha) override;
};

class FPingAnimationExecutor : public FAnimationExecutor
{
public:
	virtual void CreateAnimationData(const FGeometry& AllottedGeometry, FSlateRenderTransform& OutTransform, float& OutOpacity, float Alpha) override;
};

class FPulseAnimationExecutor : public FAnimationExecutor
{
public:
	virtual void CreateAnimationData(const FGeometry& AllottedGeometry, FSlateRenderTransform& OutTransform, float& OutOpacity, float Alpha) override;
};

class FFastUIUtils
{
public:
	static bool WidgetIsFastUIType(SWidget& InWidget)
	{
		return (
			InWidget.GetType() == TEXT("SFastUIWidget") ||
			InWidget.GetType() == TEXT("SFastUIButton") ||
			InWidget.GetType() == TEXT("SFastUITextBlock")
		);
		
	}
	static bool WidgetIsFastUIType(TSharedRef<SWidget> InWidget)
	{
		return (
			InWidget->GetType() == TEXT("SFastUIWidget") ||
			InWidget->GetType() == TEXT("SFastUIButton") ||
			InWidget->GetType() == TEXT("SFastUITextBlock")
			);
	}
};


