// Fill out your copyright notice in the Description page of Project Settings.


#include "SFastUIIcon.h"

#include "FastUIEngineSubsystem.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SFastUIIcon::Construct(const FArguments& InArgs)
{
	SAssignNew(ImageWidget, SImage);
	
	SAssignNew(Container, SBorder)
	//.BorderImage(nullptr)
	.Padding(0)
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill)
	[
		ImageWidget.ToSharedRef()
	]
	;
	

	SFastUIWidget::Construct(
		SFastUIWidget::FArguments()
		.ClassNames(InArgs._ClassNames)
		.MainWidget(Container.ToSharedRef())
	);

	UFastUIEngineSubsystem* FastUIEngineSubsystem = UFastUIEngineSubsystem::Get();
	if(FastUIEngineSubsystem && IconInfo.IsSet())
	{
		FString IconName = "fast-ui-default";
		FLinearColor Color = FLinearColor::White;
		
		FIconInfo& II = IconInfo.GetValue();
		if(II.IconName.IsSet())
		{
			IconName = II.IconName.GetValue();
		}
		if(II.Color.IsSet())
		{
			Color = II.Color.GetValue();
		}
			
		ImageWidget->SetImage(FastUIEngineSubsystem->GetIconBrush(IconName));
		ImageWidget->SetColorAndOpacity(Color);
		
		Container->SetDesiredSizeScale(FVector2D(0.8));
	}
}

void SFastUIIcon::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SFastUIWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
