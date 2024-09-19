// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FastUITypes.h"
#include "Subsystems/EngineSubsystem.h"
#include "FastUIEngineSubsystem.generated.h"

class FFastUIClassExecutor;
/**
 * 
 */
UCLASS()
class FASTUI_API UFastUIEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	
	struct FColorInfo
	{
		FString SourcePath;
		FString Name;
		TMap<FString, FLinearColor> ColorMap; // "500" -> #FFFFFF
	};

	struct FTypographyFontInfo
	{
		TMap<FString, FString> FontPathMap;
	};

	struct FIconEngineInfo
	{
		FString Source;
		TSharedPtr<FSlateBrush> Brush;
	};

	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;
	
	/**
	* Static wrapper for getting this engine subsystem. Will return nullptr
	* if the SubobjectDataInterface module has not been loaded yet.
	*/
	static UFastUIEngineSubsystem* Get();

	bool DoesClassExist(FString InClassName);
	bool AddClassName(FString ClassName, TSharedPtr<FFastUIClassExecutor> Executor);
	bool RemoveClassName(FString ClassName);
	TSharedPtr<FFastUIClassExecutor> GetExecutor(FString ClassName)
	{
		if(DoesClassExist(ClassName))
		{
			TSharedPtr<FFastUIClassExecutor>& Executor = ExecutorMap[ClassName];
			
			if(Executor->HasState())
			{
				return Executor->Copy();
			}
			return Executor;
		}
		return nullptr;
	}

	bool IsFontRegistered(FString FontName);
	FString GetFontPath(FString FontName, EFontWeight FontWeight, bool bIsItalic);
	bool IsIconRegistered(FString IconName) const;
	FSlateBrush* GetIconBrush(FString IconName);

	bool IsColorRegistered(FString Name);
	bool RegisterColor(FString Name, FColorInfo InColorInfo);
	FString GetCategoryForClass(FString ClassName);

protected:
	void RegisterStandardIcons();
	void AddStandardAnimations();
	void AddStandardTransitionClasses();

	
	TMap<FString, TSharedPtr<FFastUIClassExecutor>> ExecutorMap;
	TMap<FString, FTypographyFontInfo> RegisteredFonts;
	TMap<FString, FColorInfo> RegisteredColors;
	TMap<FString, FIconEngineInfo> RegisteredIcons;

	// global state for state class names
	
};
