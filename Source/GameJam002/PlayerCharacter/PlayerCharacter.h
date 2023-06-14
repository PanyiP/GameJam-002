#pragma once

#include "CoreMinimal.h"
#include "GameJam002\Characters\CharacterBase.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UCharacterOverlay;
class UInputMappingContext;
class UInputAction;
class UPaperFlipbook;
class USpringArmComponent;

UCLASS()
class GAMEJAM002_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GainExperience(float Experience);

protected:
	virtual void BeginPlay() override;

	/*
	* Input
	*/
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;
	void Move(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;
	void Attack(const FInputActionValue& Value);
	UFUNCTION()
	void AttackFinished();
	UFUNCTION()
	void AttackHitCheck();

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* OpenPauseMenuAction;
	void OpenPauseMenu(const FInputActionValue& Value);

	/*
	* Animations
	*/
	UPROPERTY(EditAnywhere, Category = "Animation")
	UPaperFlipbook* HitTakenAnimation;
	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<UPaperFlipbook*> AttackAnimations;

	void UpdateAnimation();
	void IsMoving();

	/*
	* Sounds
	*/
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* RunningSound;
	void PlayRunningSound();
	bool bIsPlayingRunningSound = false;
	UPROPERTY(EditAnywhere, Category = "Sound")
	float RunningSoundFrequency = 0.5f;

	UFUNCTION()
	void RunningSoundTimerHandleCallout();

	/*
	* Misc
	*/
	virtual void TakeDamageCallout(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) override;

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, category = "HUD")
	TSubclassOf<class UUserWidget> HUDClass;
	UPROPERTY()
	UCharacterOverlay* HUD;
	void UpdateHUD();

	UPROPERTY(EditAnywhere, category = "HUD")
	TSubclassOf<class UUserWidget> PauseMenuClass;
	UPROPERTY()
	UCharacterOverlay* PauseMenu;

	FTimerHandle RunningSoundTimerHandle;

	void GainLevel();
};
