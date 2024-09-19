#include "FastUIMappingAssetActions.h"

#include "Styling/SlateIconFinder.h"

FText FFastUIMappingAssetActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "FastUIAssetActions", "FastUI");
}

FColor FFastUIMappingAssetActions::GetTypeColor() const
{
	return FColor::Red;
}

UClass* FFastUIMappingAssetActions::GetSupportedClass() const
{
	return UFastUIMappingDataAsset::StaticClass();
}

void FFastUIMappingAssetActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	FAssetTypeActions_Base::GetActions(InObjects, MenuBuilder);
}

uint32 FFastUIMappingAssetActions::GetCategories()
{
	return EAssetTypeCategories::UI;
}

const TArray<FText>& FFastUIMappingAssetActions::GetSubMenus() const
{
	return FAssetTypeActions_Base::GetSubMenus();
}

TSharedPtr<SWidget> FFastUIMappingAssetActions::GetThumbnailOverlay(const FAssetData& AssetData) const
{
	const FSlateBrush* Icon = FSlateIconFinder::FindIconBrushForClass(UFastUIMappingDataAsset::StaticClass());;

	return SNew(SBorder)
		.BorderImage(FAppStyle::GetNoBrush())
		.Visibility(EVisibility::HitTestInvisible)
		.Padding(FMargin(0.0f, 0.0f, 0.0f, 3.0f))
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		[
			SNew(SImage)
			.Image(Icon)
		];
}
