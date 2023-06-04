#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

class UBoxComponent;

UCLASS()
class GAMEJAM002_API AEnemyBase : public APaperCharacter
{
	GENERATED_BODY()

public:
	AEnemyBase();

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE float GetCausedDamage() const { return (BaseDamage + AdditionalDamage) * DamageMultiplier; }
	FORCEINLINE float GetMaxHealth() const { return (BaseHealth + AdditionalHealth) * HealthMultiplier; }

protected:
	virtual void BeginPlay() override;

	/*
	* Character Stats
	*/
	UPROPERTY(EditAnywhere, Category = "Character Stats")
	float BaseDamage = 5.f;
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
	UFUNCTION()
	void TakeDamageCallout(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* HitBox;
};
