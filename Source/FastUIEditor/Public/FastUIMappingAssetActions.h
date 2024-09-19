#pragma once

#include "AssetTypeActions_Base.h"
#include "FastUIMappingDataAsset.h"

class FFastUIMappingAssetActions : public FAssetTypeActions_Base
{
public:
	// IAssetTypeActions Implementation
	FText GetName() const override;
	FColor GetTypeColor() const override;
	UClass* GetSupportedClass() const override;	
	void GetActions( const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder ) override;
	uint32 GetCategories() override;
	const TArray<FText>& GetSubMenus() const override;
	TSharedPtr<SWidget> GetThumbnailOverlay(const FAssetData& AssetData) const override;
	
};
