#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class GAMEJAM002_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthValueText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* XPBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* XPValueText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharLvlText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageText;
};
