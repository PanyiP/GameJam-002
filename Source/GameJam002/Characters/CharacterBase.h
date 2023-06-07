#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "CharacterTypes.h"
#include "CharacterBase.generated.h"

class UBoxComponent;
class UTextRenderComponent;

UCLASS()
class GAMEJAM002_API ACharacterBase : public APaperCharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

	FORCEINLINE float GetCausedDamage() const { return (FMath::RandRange(BaseMinimumDamage, BaseMaximumDamage) + AdditionalDamage) * DamageMultiplier; }
	FORCEINLINE float GetMaxHealth() const { return (BaseHealth + AdditionalHealth) * HealthMultiplier; }

protected:
	virtual void BeginPlay() override;

	/*
	* Character Stats
	*/
	UPROPERTY(EditAnywhere, Category = "Character Stats")
	float BaseMinimumDamage = 2.f;
	UPROPERTY(EditAnywhere, Category = "Character Stats")
	float BaseMaximumDamage = 6.f;
	UPROPERTY(EditAnywhere, Category = "Character Stats")
	float AdditionalDamage = 0.f;
	UPROPERTY(EditAnywhere, Category = "Character Stats")
	float DamageMultiplier = 1.f;

	UPROPERTY(EditAnywhere, Category = "Character Stats")
	float BaseHealth = 20.f;
	UPROPERTY(EditAnywhere, Category = "Character Stats")
	float AdditionalHealth = 0.f;
	UPROPERTY(EditAnywhere, Category = "Character Stats")
	float HealthMultiplier = 1.f;
	float Health = 1.f;

	/*
	* Misc
	*/
	ECharacterDirection CharacterDirection = ECharacterDirection::ECD_Right;
	ECharacterState CharacterState = ECharacterState::ECS_Idle;

	UPROPERTY(EditAnywhere)
	UBoxComponent* HitBox;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* DamageTakenText;
	void DamageTakenTimerHandleCallout();

	UFUNCTION()
	virtual void TakeDamageCallout(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

private:
	FTimerHandle DamageTakenTimerHandle;
};
