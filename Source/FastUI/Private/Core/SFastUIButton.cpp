// Fill out your copyright notice in the Description page of Project Settings.
#include "SFastUIButton.h"
#include "FastUI.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SFastUIButton::Construct(const FArguments& InArgs)
{
	Args = InArgs;
	
	ButtonStyle = MakeShareable(new FButtonStyle);
	SAssignNew(MainButton, SButton)
	.ContentPadding(0)
	.ButtonColorAndOpacity(FLinearColor::Transparent)
	.ButtonStyle(ButtonStyle.Get())
	.OnClicked(this, &SFastUIButton::OnClickedInternal)
	.OnHovered(this, &SFastUIButton::OnHoveredInternal)
	.OnUnhovered(this, &SFastUIButton::OnUnhoveredInternal)
	.OnReleased(this, &SFastUIButton::OnReleasedInternal)
	.OnPressed(this, &SFastUIButton::OnPressedInternal)
	;

	SFastUIWidget::Construct(
		SFastUIWidget::FArguments()
		.ClassNames(InArgs._ClassNames)
		.ParentInnerBorder(MainButton.ToSharedRef())
		.GenSlots(InArgs._Slots)
	);
}

FReply SFastUIButton::OnClickedInternal()
{
	UE_LOG(LogFastUI, Log, TEXT("Clicked"))
	if(Args._OnClicked.IsBound())
	{
		return Args._OnClicked.Execute();
	}
	return FReply::Handled();
}

void SFastUIButton::OnHoveredInternal()
{
	ShowHoveredStyle();
	UE_LOG(LogFastUI, Log, TEXT("OnHovered"))
}

void SFastUIButton::OnUnhoveredInternal()
{
	ShowNormalStyle();
	UE_LOG(LogFastUI, Log, TEXT("Unhovered"))
}

void SFastUIButton::OnPressedInternal()
{
	ShowPressedStyle();
	UE_LOG(LogFastUI, Log, TEXT("Pressed"))
}

void SFastUIButton::OnReleasedInternal()
{
	UE_LOG(LogFastUI, Log, TEXT("Released"))
	if(MainButton->IsHovered())
	{
		ShowHoveredStyle();
	}
	else
	{
		ShowNormalStyle();
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
