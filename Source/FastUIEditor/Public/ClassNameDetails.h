#pragma once
#include "FastUIWidget.h"
#include "IDetailCustomization.h"

class FClassNameDetails : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** ILayoutDetails interface */
	virtual void CustomizeDetails(class IDetailLayoutBuilder& DetailBuilder) override;

	void OnClassNameAdded(FString InClassName);
	void OnClassNameRemoved(FString InClassName);

	TWeakObjectPtr<UFastUIWidget> GetFastUIWidget();

private:

private:
	/** Associated detail layout builder */
	IDetailLayoutBuilder* DetailLayoutBuilder;
	TSharedPtr<class ISinglePropertyView> PropertyView;
		
	
};
