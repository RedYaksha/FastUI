// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SFastUITextBlock.h"

#include "FastUIEngineSubsystem.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SFastUITextBlock::Construct(const FArguments& InArgs)
{
	SAssignNew(MainTextBlock, STextBlock)
	.Text(InArgs._Text)
	;
	
	SFastUIWidget::Construct(
		SFastUIWidget::FArguments()
		.ClassNames(InArgs._ClassNames)
		.MainWidget(MainTextBlock.ToSharedRef())
	);

	// default font path must be set somewhere... probably in config
	FString FontPath = FPaths::ProjectContentDir() / TEXT("Slate/Fonts/Roboto/Roboto-Black.ttf");
	FString FontName = "Roboto";
	ETextJustify::Type Justification = ETextJustify::Left;
	EFontWeight FontWeight = EFontWeight::NORMAL;
	bool bIsItalic = false;
	int Size = 24;
	
	FLinearColor Color = FLinearColor::White;
	
	UFastUIEngineSubsystem* FastUIEngineSubsystem = UFastUIEngineSubsystem::Get();
	if(FastUIEngineSubsystem && TypographyInfo.IsSet())
	{
		FTypographyInfo& TI = TypographyInfo.GetValue();

		if(TI.bIsItalic.IsSet())
		{
			bIsItalic = TI.bIsItalic.GetValue();
		}
		if(TI.FontWeight.IsSet())
		{
			FontWeight = TI.FontWeight.GetValue();
		}
		if(TI.FontName.IsSet())
		{
			FontName = TI.FontName.GetValue();
		}
		if(TI.Size.IsSet())
		{
			Size = TI.Size.GetValue();
		}
		if(TI.Color.IsSet())
		{
			Color = TI.Color.GetValue();
		}
		if(TI.Justification.IsSet())
		{
			Justification = TI.Justification.GetValue();
		}
	}
	
	FString QueriedFontPath = FastUIEngineSubsystem->GetFontPath(FontName, FontWeight, bIsItalic);
	if(QueriedFontPath != "")
	{
		FontPath = QueriedFontPath;
	}
	
	FontInfo = FSlateFontInfo(FontPath, Size);
	
	MainTextBlock->SetFont(FontInfo);
	MainTextBlock->SetColorAndOpacity(Color);
	MainTextBlock->SetJustification(Justification);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
