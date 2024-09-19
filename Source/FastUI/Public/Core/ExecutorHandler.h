#pragma once
#include "Core/FastUITypes.h"


class FExecutorHandler
{
public:
	virtual ~FExecutorHandler() = default;

	FExecutorHandler()
	{}

	void Execute(TArray<FString> ClassNames, FWidgetReferences WidgetReferences);
	
	bool TryAddSlot(TSharedRef<SWidget> InWidget);
	bool TryRemoveSlot(TSharedRef<SWidget> InWidget);
	void CreateAnimationData(const FGeometry& AllottedGeometry, FSlateRenderTransform& OutGeometry, float& OutOpacity, float Alpha);
	bool HasAnimationExecutors() const { return AnimationExecutors.Num() > 0; }
	
	TSharedRef<SWidget> GetLayoutWidget() { return LayoutExecutor->GetLayoutWidget(); }
	TSharedRef<SBorder> GetParentBorder() { return LayoutExecutor->GetParentBorder(); }
	
	TMap<FString, TSharedPtr<FFastUIClassExecutor>> ExecutorMap;

	TSharedPtr<FLayoutExecutor> LayoutExecutor;
	TArray<TSharedPtr<FAnimationExecutor>> AnimationExecutors;
};

