#pragma once

#include "CoreMinimal.h"
#include "GameJam002\Characters\CharacterBase.h"
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

UENUM()
enum class ECharacterState
{
	ECS_Idle = 0,
	ECS_Running,
	ECS_Attacking,
	ECD_MAX
};

UCLASS()
class GAMEJAM002_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

	/*
	* Animations
	*/
	UPROPERTY(EditAnywhere, Category = "Animation")
	UPaperFlipbook* IdleAnimation;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UPaperFlipbook* RunLeftAnimation;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UPaperFlipbook* RunRightAnimation;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UPaperFlipbook* HitTakenAnimation;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UPaperFlipbook* DeathAnimation;
	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<UPaperFlipbook*> AttackAnimations;

	void UpdateAnimation();
	void IsMoving();

	/*
	* Misc
	*/
	ECharacterDirection CharacterDirection = ECharacterDirection::ECD_Right;
	ECharacterState CharacterState = ECharacterState::ECS_Idle;

	virtual void TakeDamageCallout(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) override;

private:
	UPROPERTY()
	USpringArmComponent* CameraBoom;

	UPROPERTY()
	UCameraComponent* Camera;
};
