#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UPaperFlipbook;
class USpringArmComponent;

UENUM()
enum class ECharacterDirection
{
	ECD_Left = 0,
	ECD_Right,
	ECD_MAX
};

UCLASS()
class GAMEJAM002_API APlayerCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;
	void Move(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "Animation")
	UPaperFlipbook* IdleAnimation;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UPaperFlipbook* RunLeftAnimation;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UPaperFlipbook* RunRightAnimation;
	void UpdateAnimation();

private:
	UPROPERTY()
	USpringArmComponent* CameraBoom;

	UPROPERTY()
	UCameraComponent* Camera;

	ECharacterDirection CharacterDirection = ECharacterDirection::ECD_Right;
};
