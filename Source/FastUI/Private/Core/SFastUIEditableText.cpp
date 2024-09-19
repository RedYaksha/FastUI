// Fill out your copyright notice in the Description page of Project Settings.


#include "SFastUIEditableText.h"

#include "FastUIEngineSubsystem.h"
#include "SFastUIWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SFastUIEditableText::Construct(const FArguments& InArgs)
{
	SAssignNew(EditableText, SEditableText)
	.HintText(InArgs._HintText)
	.OnTextCommitted(InArgs._OnTextCommitted)
	.OnTextChanged(InArgs._OnTextChanged)
	.ClearKeyboardFocusOnCommit(InArgs._ClearKeyboardFocusOnCommit)
	;

	SFastUIWidget::Construct(
		SFastUIWidget::FArguments()
		.ClassNames(InArgs._ClassNames)
		.MainWidget(EditableText.ToSharedRef())
	);

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

	EditableText->SetFont(FontInfo);
	EditableText->SetColorAndOpacity(Color);
	EditableText->SetJustification(Justification);
}

FReply SFastUIEditableText::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	if(EditableText)
	{
		UE_LOG(LogTemp, Log, TEXT("FASTUI Received Focus!"))
		FSlateApplication::Get().SetKeyboardFocus(EditableText);
	}
	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
