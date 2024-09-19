// Fill out your copyright notice in the Description page of Project Settings.


#include "FastUIEngineSubsystem.h"

#include "FastUI.h"
#include "FastUITypes.h"

void UFastUIEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UE_LOG(LogFastUI, Log, TEXT("Initializing FastUI Subsystem..."));

	// takes all loaded in colors, and generates all color related class names
	// (e.g. bg- text- fg- border- )
	bool bLoadStandardColorClassNames;
	GConfig->GetBool(TEXT("FastUI"), TEXT("bLoadStandardColorClassNames"), bLoadStandardColorClassNames, GGameIni);

	bool bLoadStandardClassNames;
	GConfig->GetBool(TEXT("FastUI"), TEXT("bLoadStandardClassNames"), bLoadStandardClassNames, GGameIni);

	// load colors
	TArray<FString> ColorPaths;
	GConfig->GetArray(TEXT("FastUI"), TEXT("ColorPaths"), ColorPaths, GGameIni);

	for(const FString& Path : ColorPaths)
	{
		FString FileContents;
		if(FFileHelper::LoadFileToString(FileContents, *Path))
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContents);
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				TSharedPtr<FJsonObject> ColorSet = JsonObject->GetObjectField("Colors");
				FString ColorSetName = JsonObject->GetStringField("Name");

				TMap<FString, TSharedPtr<FJsonValue>> Values = ColorSet->Values;

				for(const TTuple<FString, TSharedPtr<FJsonValue>> V : Values)
				{
					const FString MainColorName = V.Key;
					TMap<FString, TSharedPtr<FJsonValue>> Colors = V.Value->AsObject()->Values;

					UE_LOG(LogTemp, Log, TEXT("%s"), *MainColorName);

					// create FColorInfo
					TMap<FString, FLinearColor> ColorMap;
					for(auto C : Colors)
					{
						const FString ColorSubName = C.Key;
						const FString Hex = C.Value->AsString();

						FLinearColor Color = FColor::FromHex(Hex);
						
						UE_LOG(LogTemp, Log, TEXT("%s %s"), *ColorSubName, *Hex);

						ColorMap.Add(ColorSubName, Color);
					}

					FColorInfo NewColorInfo;
					NewColorInfo.Name = MainColorName;
					NewColorInfo.ColorMap = ColorMap;
					NewColorInfo.SourcePath = Path;

					RegisterColor(MainColorName, NewColorInfo);
				}
			}
			else
			{
				UE_LOG(LogFastUI, Error, TEXT("Failed to deserialize color definition file: %s"), *Path);
			}
		}
		else
		{
			UE_LOG(LogFastUI, Error, TEXT("Failed to loaded specified color definition file: %s"), *Path);
		}
	}
	
	// load standard class names
	if(bLoadStandardClassNames)
	{
		UE_LOG(LogFastUI, Log, TEXT("Loading CPP Standard Class Names"))

#define FUNSET TOptional<float>()
#define IUNSET TOptional<int>()
#define ADD_CLASS(ClassName, ExecutorType, ParamType, ...) \
	ExecutorMap.Add(ClassName, MakeShareable(new ExecutorType(ParamType(__VA_ARGS__))));

#define ADD_MARGIN_CLASS(ClassName, ...) ADD_CLASS(ClassName, FMarginExecutor, FMarginExecutorParams, __VA_ARGS__)
		
#define ADD_PADDING_CLASS(ClassName, ...) ADD_CLASS(ClassName, FPaddingExecutor, FPaddingExecutorParams, __VA_ARGS__)
	
		// margin/padding
		for(int i = 1; i < 20; i++)
		{
			const float V = i * 5;
			ADD_MARGIN_CLASS("m-" + FString::FromInt(i), V, V, V, V)
			ADD_MARGIN_CLASS("mx-" + FString::FromInt(i), V, V, FUNSET, FUNSET)
			ADD_MARGIN_CLASS("my-" + FString::FromInt(i), FUNSET, FUNSET, V, V)
			ADD_MARGIN_CLASS("ml-" + FString::FromInt(i), V, FUNSET, FUNSET, FUNSET)
			ADD_MARGIN_CLASS("mr-" + FString::FromInt(i), FUNSET, V, FUNSET, FUNSET)
			ADD_MARGIN_CLASS("mt-" + FString::FromInt(i), FUNSET, FUNSET, V, FUNSET)
			ADD_MARGIN_CLASS("mb-" + FString::FromInt(i), FUNSET, FUNSET, FUNSET, V)
			
			ADD_PADDING_CLASS("p-" + FString::FromInt(i), V, V, V, V)
			ADD_PADDING_CLASS("px-" + FString::FromInt(i), V, V, FUNSET, FUNSET)
			ADD_PADDING_CLASS("py-" + FString::FromInt(i), FUNSET, FUNSET, V, V)
			ADD_PADDING_CLASS("pl-" + FString::FromInt(i), V, FUNSET, FUNSET, FUNSET)
			ADD_PADDING_CLASS("pr-" + FString::FromInt(i), FUNSET, V, FUNSET, FUNSET)
			ADD_PADDING_CLASS("pt-" + FString::FromInt(i), FUNSET, FUNSET, V, FUNSET)
			ADD_PADDING_CLASS("pb-" + FString::FromInt(i), FUNSET, FUNSET, FUNSET, V)
		}
		
		// rounded
#define ADD_ROUNDED_CLASS(ClassName, ...) ADD_CLASS(ClassName, FRoundedBackgroundExecutor, FRoundedRadiiInfo, __VA_ARGS__)
		{
			struct FLocal
			{
				FLocal(float Value, const FString& Name)
					: Value(Value),
					  Name(Name)
				{}

				float Value;
				FString Name;
			};

			TArray Locals = {
				FLocal(2, "sm"),
				FLocal(4, ""),
				FLocal(6, "md"),
				FLocal(8, "lg"),
				FLocal(12, "xl"),
				FLocal(16, "2xl"),
				FLocal(24, "3xl"),
			};

			for(auto L : Locals)
			{
				FString Suffix = L.Name == ""? "" : "-" + L.Name;
				int V = L.Value;
				
				ADD_ROUNDED_CLASS("rounded" + Suffix, V, V, V, V)
				ADD_ROUNDED_CLASS("rounded-t" + Suffix, V, V, FUNSET, FUNSET)
				ADD_ROUNDED_CLASS("rounded-r" + Suffix, FUNSET, V, V, FUNSET)
				ADD_ROUNDED_CLASS("rounded-b" + Suffix, FUNSET, FUNSET, V, V)
				ADD_ROUNDED_CLASS("rounded-l" + Suffix, V, FUNSET, FUNSET, V)
				ADD_ROUNDED_CLASS("rounded-tl" + Suffix, V, FUNSET, FUNSET, FUNSET)
				ADD_ROUNDED_CLASS("rounded-tr" + Suffix, FUNSET, V, FUNSET, FUNSET)
				ADD_ROUNDED_CLASS("rounded-br" + Suffix, FUNSET, FUNSET, V, FUNSET)
				ADD_ROUNDED_CLASS("rounded-bl" + Suffix, FUNSET, FUNSET, FUNSET, V)
			}
		}

		// border (TODO) (need a way to render only top border, bottom border, etc.)

		// outline
#define CUNSET TOptional<FLinearColor>()
#define RRUNSET TOptional<FRoundedRadiiInfo>()
		
#define ADD_OUTLINE_CLASS(ClassName, ...) ADD_CLASS(ClassName, FBackgroundExecutor, FBackgroundInfo, __VA_ARGS__)
	{
		TArray<int> OutlineWidths = {2, 4, 8};

		ADD_OUTLINE_CLASS("outline", CUNSET, RRUNSET, 1, CUNSET);
		
		for(auto W : OutlineWidths)
		{
			ADD_OUTLINE_CLASS("outline", CUNSET, RRUNSET, W, CUNSET);
		}
	}
		

		// layout
		ExecutorMap.Add("vbox", MakeShareable(new FVerticalBoxExecutor));
		ExecutorMap.Add("hbox", MakeShareable(new FHorizontalBoxExecutor));
		ExecutorMap.Add("overlay", MakeShareable(new FOverlayExecutor));

		// layout slot alignment
#define HAUNSET TOptional<EHorizontalAlignment>()
#define VAUNSET TOptional<EVerticalAlignment>()
#define ADD_SLOT_ALIGN_CLASS(ClassName, ...) ADD_CLASS(ClassName, FAlignmentExecutor, FAlignmentParams, __VA_ARGS__)

		ADD_SLOT_ALIGN_CLASS("h-center", HAlign_Center, VAUNSET)
		ADD_SLOT_ALIGN_CLASS("h-left", HAlign_Left, VAUNSET)
		ADD_SLOT_ALIGN_CLASS("h-right", HAlign_Right, VAUNSET)
		ADD_SLOT_ALIGN_CLASS("h-fill", HAlign_Fill, VAUNSET)
		
		ADD_SLOT_ALIGN_CLASS("v-center", HAUNSET, VAlign_Center)
		ADD_SLOT_ALIGN_CLASS("v-top", HAUNSET, VAlign_Top)
		ADD_SLOT_ALIGN_CLASS("v-bottom", HAUNSET, VAlign_Bottom)
		ADD_SLOT_ALIGN_CLASS("v-fill", HAUNSET, VAlign_Fill)

		// typography
#define ADD_TYPOGRAPHY_CLASS(ClassName, ...) ADD_CLASS(ClassName, FTypographyExecutor, FTypographyInfo, __VA_ARGS__)
#define SUNSET TOptional<FString>()
#define BUNSET TOptional<bool>()
#define FWUNSET TOptional<EFontWeight>()
#define JUNSET TOptional<ETextJustify::Type>()

		// iterate through all Content/Slate/Fonts/
		// and procedurally add fonts for italic/bold/normal
		const FString FontsDir = FPaths::ProjectContentDir() / TEXT("Slate/Fonts");

		//
		TArray<FString> FontFolders;
		IFileManager::Get().FindFiles(FontFolders, *(FontsDir / "*"), true, true);

		for(auto Folder : FontFolders)
		{
			TArray<FString> FontFiles;
			IFileManager::Get().FindFiles(FontFiles, *(FontsDir / Folder / "*"), true, false);

			for(auto File : FontFiles)
			{
				FString FullFilePath = FontsDir / Folder / File;
				// UE_LOG(LogTemp, Log, TEXT("%s"), *FullFilePath)

				const FString Ext = FPaths::GetExtension(FullFilePath, false).ToLower();
				if(Ext != "otf" && Ext != "ttf")
				{
					continue;
				}

				FString FileNoExt = File.Left(File.Len() - 4);

				TArray<FString> FileArr;
				FileNoExt.ParseIntoArray(FileArr, TEXT("-"));

				if(FileArr.Num() != 2)
				{
					UE_LOG(LogFastUI, Warning, TEXT("Found a font file %s but it's formatted incorrectly."), *FullFilePath)
					continue;
				}

				const FString FontName = FileArr[0];
				FString FontType = FileArr[1];
				
				const bool bIsItalic = FontType.Contains("Italic");

				if(!RegisteredFonts.Contains(FontName))
				{
					RegisteredFonts.Add(FontName, FTypographyFontInfo());
				}

				RegisteredFonts[FontName].FontPathMap.Add(FontType, FullFilePath);

				ADD_TYPOGRAPHY_CLASS("font-" + FontName.ToLower(), FontName, FWUNSET, BUNSET, IUNSET, CUNSET, JUNSET);
			}
		}

		// font weights
		ADD_TYPOGRAPHY_CLASS("font-thin", SUNSET, EFontWeight::THIN, BUNSET, IUNSET, CUNSET, JUNSET);
		ADD_TYPOGRAPHY_CLASS("font-extralight", SUNSET, EFontWeight::EXTRALIGHT, BUNSET, IUNSET, CUNSET, JUNSET);
		ADD_TYPOGRAPHY_CLASS("font-light", SUNSET, EFontWeight::LIGHT, BUNSET, IUNSET, CUNSET, JUNSET);
		ADD_TYPOGRAPHY_CLASS("font-normal", SUNSET, EFontWeight::NORMAL, BUNSET, IUNSET, CUNSET, JUNSET);
		ADD_TYPOGRAPHY_CLASS("font-medium", SUNSET, EFontWeight::MEDIUM, BUNSET, IUNSET, CUNSET, JUNSET);
		ADD_TYPOGRAPHY_CLASS("font-semibold", SUNSET, EFontWeight::SEMIBOLD, BUNSET, IUNSET, CUNSET, JUNSET);
		ADD_TYPOGRAPHY_CLASS("font-bold", SUNSET, EFontWeight::BOLD, BUNSET, IUNSET, CUNSET, JUNSET);
		ADD_TYPOGRAPHY_CLASS("font-extrabold", SUNSET, EFontWeight::EXTRABOLD, BUNSET, IUNSET, CUNSET, JUNSET);
		ADD_TYPOGRAPHY_CLASS("font-black", SUNSET, EFontWeight::BLACK, BUNSET, IUNSET, CUNSET, JUNSET);

		// italic / not italic
		ADD_TYPOGRAPHY_CLASS("italic", SUNSET, FWUNSET, true, IUNSET, CUNSET, JUNSET);
		ADD_TYPOGRAPHY_CLASS("not-italic", SUNSET, FWUNSET, false, IUNSET, CUNSET, JUNSET);

		// justification
		ADD_TYPOGRAPHY_CLASS("text-left", SUNSET, FWUNSET, BUNSET, IUNSET, CUNSET, ETextJustify::Left);
		ADD_TYPOGRAPHY_CLASS("text-center", SUNSET, FWUNSET, BUNSET, IUNSET, CUNSET, ETextJustify::Center);
		ADD_TYPOGRAPHY_CLASS("text-right", SUNSET, FWUNSET, BUNSET, IUNSET, CUNSET, ETextJustify::Right);
		// ADD_TYPOGRAPHY_CLASS("text-justify", SUNSET, FWUNSET, false, IUNSET, CUNSET, JUNSET);

		// font size
		{
			struct FLocal
			{
				FLocal(const FString& ClassName, float Size)
					: ClassName(ClassName),
					  Size(Size)
				{}

				FString ClassName;
				int Size;
			};
			
			TArray<FLocal> Locals= {
				FLocal("text-xs", 12),
				FLocal("text-sm", 14),
				FLocal("text-lg", 18),
				FLocal("text-xl", 20),
				FLocal("text-2xl", 24),
				FLocal("text-3xl", 30),
				FLocal("text-4xl", 36),
				FLocal("text-5xl", 48),
				FLocal("text-6xl", 60),
				FLocal("text-7xl", 72),
				FLocal("text-8xl", 96),
				FLocal("text-9xl", 128),
			};
			
			for(auto L : Locals)
			{
				ADD_TYPOGRAPHY_CLASS(L.ClassName, SUNSET, FWUNSET, BUNSET, L.Size, CUNSET, JUNSET)
			}
			
		}
	}

// load standard color class names

#define ADD_BACKGROUND_CLASS(ClassName, ...) ADD_CLASS(ClassName, FBackgroundExecutor, FBackgroundInfo, __VA_ARGS__)
#define ADD_ICON_CLASS(ClassName, ...) ADD_CLASS(ClassName, FIconExecutor, FIconInfo, __VA_ARGS__)
	
	if(bLoadStandardColorClassNames)
	{
		UE_LOG(LogFastUI, Log, TEXT("Loading CPP Standard Class Names"))

		for(auto Tuple : RegisteredColors)
		{
			const FString ColorName = Tuple.Key;
			const TMap<FString, FLinearColor> ColorMap = Tuple.Value.ColorMap;

			for(auto ColorMapTuple : ColorMap)
			{
				const FString SubName = ColorMapTuple.Key;
				
				// e.g. rose-500
				const FString MainClassName = ColorName + "-" + SubName;
				const FLinearColor Color = ColorMapTuple.Value;
				
				// bg
				ADD_BACKGROUND_CLASS("bg-" + MainClassName, Color, RRUNSET, IUNSET, CUNSET)
				
				// outline color
				ADD_BACKGROUND_CLASS("outline-" + MainClassName, CUNSET, RRUNSET, IUNSET, Color)

				// text color
				ADD_TYPOGRAPHY_CLASS("text-" + MainClassName, SUNSET, FWUNSET, BUNSET, IUNSET, Color, JUNSET)

				// icon color
				ADD_ICON_CLASS("icon-" + MainClassName, SUNSET, Color)
				
			}
		}
	}

	RegisterStandardIcons();
	AddStandardAnimations();
	AddStandardTransitionClasses();
}

void UFastUIEngineSubsystem::Deinitialize()
{
	Super::Deinitialize();
	UE_LOG(LogFastUI, Log, TEXT("Deinitializing FastUI Subsystem"));
}

UFastUIEngineSubsystem* UFastUIEngineSubsystem::Get()
{
	return GEngine->GetEngineSubsystem<UFastUIEngineSubsystem>();
}

bool UFastUIEngineSubsystem::DoesClassExist(FString InClassName)
{
	return ExecutorMap.Contains(InClassName);
}

bool UFastUIEngineSubsystem::AddClassName(FString ClassName, TSharedPtr<FFastUIClassExecutor> Executor)
{
	if(DoesClassExist(ClassName))
	{
		UE_LOG(LogFastUI, Warning, TEXT("Attempted to add an existing class name: %s"), *ClassName);
		return false;
	}

	ExecutorMap.Add(ClassName, Executor);
	return true;
}

bool UFastUIEngineSubsystem::RemoveClassName(FString ClassName)
{
	if(!DoesClassExist(ClassName))
	{
		UE_LOG(LogFastUI, Warning, TEXT("Attempted to remove a non existent class name: %s"), *ClassName);
		return false;
	}

	ExecutorMap.Remove(ClassName);
	return true;
}

bool UFastUIEngineSubsystem::IsFontRegistered(FString FontName)
{
	return RegisteredFonts.Contains(FontName);
}

FString UFastUIEngineSubsystem::GetFontPath(FString FontName, EFontWeight FontWeight, bool bIsItalic)
{
	if(!IsFontRegistered(FontName))
	{
		return "";
	}

	FTypographyFontInfo& Info = RegisteredFonts[FontName];
	TMap<EFontWeight, FString> WeightToString = {
		{EFontWeight::THIN, "Thin"},
		{EFontWeight::EXTRALIGHT, "ExtraLight"},
		{EFontWeight::LIGHT, "Light"},
		{EFontWeight::NORMAL, "Regular"},
		{EFontWeight::MEDIUM, "Medium"},
		{EFontWeight::SEMIBOLD, "SemiBold"},
		{EFontWeight::BOLD, "Bold"},
		{EFontWeight::EXTRABOLD, "ExtraBold"},
		{EFontWeight::BLACK, "Black"},
	};

	FString FontType = WeightToString[FontWeight] + (bIsItalic? "Italic" : "");
	if(!Info.FontPathMap.Contains(FontType))
	{
		UE_LOG(LogFastUI, Warning, TEXT("Couldn't find font type %s for %s. Falling back."), *FontType, *FontName)
		if(!Info.FontPathMap.Contains("Regular"))
		{
			UE_LOG(LogTemp, Error, TEXT("Font %s does not have a regular font weight"), *FontName);
			return "";
		}
		return Info.FontPathMap["Regular"];
	}
	return Info.FontPathMap[FontType];
}

bool UFastUIEngineSubsystem::IsIconRegistered(FString IconName) const
{
	return RegisteredIcons.Contains(IconName);
}

FSlateBrush* UFastUIEngineSubsystem::GetIconBrush(FString IconName)
{
	if(IsIconRegistered(IconName))
	{
		return RegisteredIcons[IconName].Brush.Get();
	}
	return nullptr;
}

bool UFastUIEngineSubsystem::IsColorRegistered(FString Name)
{
	return RegisteredColors.Contains(Name); 
}

bool UFastUIEngineSubsystem::RegisterColor(FString Name, FColorInfo InColorInfo)
{
	if(IsColorRegistered(Name))
	{
		const FColorInfo& ColorInfo = RegisteredColors[Name];
		UE_LOG(LogFastUI, Warning, TEXT("Color entry %s already defined in %s"), *Name, *ColorInfo.SourcePath);
		return false;
	}

	RegisteredColors.Add(Name, InColorInfo);
	return true;
	
}

FString UFastUIEngineSubsystem::GetCategoryForClass(FString ClassName)
{
	if(ExecutorMap.Contains(ClassName))
	{
		return ExecutorMap[ClassName]->GetCategory();
	}
	return "";
}

void UFastUIEngineSubsystem::RegisterStandardIcons()
{
	// iterate through all Content/Slate/Fonts/
	// and procedurally add fonts for italic/bold/normal
	const FString IconsDir = FPaths::ProjectContentDir() / TEXT("Slate/Icons");

	//
	TArray<FString> SVGFiles;
	
	IFileManager::Get().FindFilesRecursive(SVGFiles, *IconsDir, TEXT("*.svg"), true, false);

	for(auto FilePath : SVGFiles)
	{
		FString IconName = FPaths::GetBaseFilename(FilePath);
		
		FVector2D IconSize = FVector2D(25, 25);
		FString SVGFileContent;
		if(FFileHelper::LoadFileToString(SVGFileContent, *FilePath))
		{
			// TODO: this is very specific to FontAwesome SVGs...
			FString PropName = "viewBox";
			int Ind = SVGFileContent.Find(PropName);

			int StartQuoteInd = Ind + PropName.Len() + 2;
			int EndQuoteInd = SVGFileContent.Find("\"", ESearchCase::IgnoreCase, ESearchDir::FromStart, StartQuoteInd);
			FString ViewBoxContents = SVGFileContent.Mid(StartQuoteInd, EndQuoteInd - StartQuoteInd);

			UE_LOG(LogTemp, Log, TEXT("S: %d E: %d Str: %s"), StartQuoteInd, EndQuoteInd, *ViewBoxContents);

			TArray<FString> ViewBoxNums;
			ViewBoxContents.ParseIntoArray(ViewBoxNums, TEXT(" "));

			if(ViewBoxNums.Num() == 4)
			{
				int SizeX = FCString::Atoi(*ViewBoxNums[2]); 
				int SizeY = FCString::Atoi(*ViewBoxNums[3]);

				IconSize = FVector2D(SizeX, SizeY) * 0.025;
				UE_LOG(LogTemp, Log, TEXT("Extracted Icon Size: %s"), *IconSize.ToString());
			}
		}
		

		FIconEngineInfo IconInfo;
		IconInfo.Source = FilePath;
		IconInfo.Brush = MakeShareable<FSlateVectorImageBrush>(new FSlateVectorImageBrush(FilePath, IconSize));
		
		
		RegisteredIcons.Add(IconName, IconInfo);

		// i-<icon-name> can then map to the RegisteredIcons entry
		ADD_ICON_CLASS("i-" + IconName, IconName, CUNSET)
	}

	UE_LOG(LogTemp, Log, TEXT("Icons registered..."))
	
}

void UFastUIEngineSubsystem::AddStandardAnimations()
{
	ExecutorMap.Add("animate-spin", MakeShareable(new FSpinAnimationExecutor));
	ExecutorMap.Add("animate-pulse", MakeShareable(new FPulseAnimationExecutor));
	ExecutorMap.Add("animate-ping", MakeShareable(new FPingAnimationExecutor));
}

void UFastUIEngineSubsystem::AddStandardTransitionClasses()
{
#define ADD_ANIMATION_INFO_CLASS(ClassName, ...) ADD_CLASS(ClassName, FAnimationInfoExecutor, FAnimationInfo, __VA_ARGS__)
#define TFUNSET TOptional<ETimingFunction>()
#define ADUNSET TOptional<EAnimationDirection>()

	TArray<int> Durations = {0, 75, 100, 150, 200, 300, 500, 700, 1000};
	for(auto D : Durations)
	{
		ADD_ANIMATION_INFO_CLASS("duration-" + FString::FromInt(D), D, IUNSET, TFUNSET, IUNSET, ADUNSET)
		ADD_ANIMATION_INFO_CLASS("delay-" + FString::FromInt(D), IUNSET, D, TFUNSET, IUNSET, ADUNSET)
	}

	ADD_ANIMATION_INFO_CLASS("ease-linear", IUNSET, IUNSET, ETimingFunction::EASE_LINEAR, IUNSET, ADUNSET)
	ADD_ANIMATION_INFO_CLASS("ease-in", IUNSET, IUNSET, ETimingFunction::EASE_IN, IUNSET, ADUNSET)
	ADD_ANIMATION_INFO_CLASS("ease-out", IUNSET, IUNSET, ETimingFunction::EASE_OUT, IUNSET, ADUNSET)
	ADD_ANIMATION_INFO_CLASS("ease-in-out", IUNSET, IUNSET, ETimingFunction::EASE_IN_OUT, IUNSET, ADUNSET)
	
	ADD_ANIMATION_INFO_CLASS("iter-1", IUNSET, IUNSET, TFUNSET, 1, ADUNSET)
	ADD_ANIMATION_INFO_CLASS("iter-2", IUNSET, IUNSET, TFUNSET, 2, ADUNSET)
	ADD_ANIMATION_INFO_CLASS("iter-3", IUNSET, IUNSET, TFUNSET, 3, ADUNSET)
	ADD_ANIMATION_INFO_CLASS("iter-inf", IUNSET, IUNSET, TFUNSET, -1, ADUNSET)
	
	ADD_ANIMATION_INFO_CLASS("animate-normal", IUNSET, IUNSET, TFUNSET, IUNSET, EAnimationDirection::NORMAL)
	ADD_ANIMATION_INFO_CLASS("animate-reverse", IUNSET, IUNSET, TFUNSET, IUNSET, EAnimationDirection::REVERSE)
	ADD_ANIMATION_INFO_CLASS("animate-alternate", IUNSET, IUNSET, TFUNSET, IUNSET, EAnimationDirection::ALTERNATE)
	ADD_ANIMATION_INFO_CLASS("animate-alternate-reverse", IUNSET, IUNSET, TFUNSET, IUNSET, EAnimationDirection::ALTERNATE_REVERSE)
	
}
