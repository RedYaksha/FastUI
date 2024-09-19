// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FastUIMappingDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FASTUI_API UFastUIMappingDataAsset : public UPrimaryDataAsset
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Core")
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Core")
	TMap<FString, FString> Mapping;
};
