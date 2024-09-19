#include "FastUIEditor.h"

#include "AssetToolsModule.h"
#include "ClassNameDetails.h"
#include "FastUIMappingAssetActions.h"
#include "FastUIWidget.h"

void FFastUIEditorModule::StartupModule()
{
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked< FAssetToolsModule >("AssetTools");	
	TSharedPtr<FFastUIMappingAssetActions> FastUITypeActions = MakeShareable( new FFastUIMappingAssetActions);
	AssetToolsModule.Get().RegisterAssetTypeActions(FastUITypeActions.ToSharedRef() );
	
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(
		UFastUIWidget::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FClassNameDetails::MakeInstance)
	);
}

void FFastUIEditorModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
IMPLEMENT_MODULE(FFastUIEditorModule, FastUIEditor)
