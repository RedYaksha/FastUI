#include "Core/ExecutorHandler.h"
#include "FastUI.h"
#include "FastUIEngineSubsystem.h"

void FExecutorHandler::Execute(TArray<FString> ClassNames, FWidgetReferences WidgetReferences)
{
	TSharedPtr<FLayoutExecutor> FoundLayoutExecutor = nullptr;
	AnimationExecutors.Empty();
	
	UFastUIEngineSubsystem* FastUIEngineSubsystem = UFastUIEngineSubsystem::Get();
	if(!FastUIEngineSubsystem)
	{
		return;
	}
	
	for(FString Str : ClassNames)
	{
		if(!FastUIEngineSubsystem->DoesClassExist(Str))
		{
			UE_LOG(LogFastUI, Log, TEXT("%s not registered"), *Str);
			continue;
		}

		// UE_LOG(LogFastUI, Log, TEXT("Executing: %s"), *Str);

		TSharedPtr<FFastUIClassExecutor> Executor = FastUIEngineSubsystem->GetExecutor(Str);
		
		if(Executor->IsLayout())
		{
			FoundLayoutExecutor = StaticCastSharedPtr<FLayoutExecutor>(Executor);
		}
		if(Executor->IsAnimation())
		{
			TSharedPtr<FAnimationExecutor> AnimationExecutor = StaticCastSharedPtr<FAnimationExecutor>(Executor);
			AnimationExecutors.Add(AnimationExecutor);
		}
		
		Executor->PerformAction(WidgetReferences);
	}

	if(FoundLayoutExecutor.IsValid())
	{
		LayoutExecutor = FoundLayoutExecutor;
	}
	else
	{
		// no layout? assume vbox
		LayoutExecutor = StaticCastSharedPtr<FLayoutExecutor>(FastUIEngineSubsystem->GetExecutor("vbox"));
		if(LayoutExecutor)
		{
			LayoutExecutor->PerformAction(WidgetReferences);
		}
	}
}

bool FExecutorHandler::TryRemoveSlot(TSharedRef<SWidget> InSlot)
{
	if(LayoutExecutor.IsValid())
	{
		LayoutExecutor->RemoveSlot(InSlot);
		return true;
	}
	return false;
}

void FExecutorHandler::CreateAnimationData(const FGeometry& AllottedGeometry, FSlateRenderTransform& OutTransform,
	float& OutOpacity, float Alpha)
{
	for(auto AnimExec : AnimationExecutors)
	{
		AnimExec->CreateAnimationData(AllottedGeometry, OutTransform, OutOpacity, Alpha);
	}
}

bool FExecutorHandler::TryAddSlot(TSharedRef<SWidget> InWidget)
{
	if(LayoutExecutor.IsValid())
	{
		LayoutExecutor->AddSlot(InWidget);
		return true;
	}
	return false;
}
