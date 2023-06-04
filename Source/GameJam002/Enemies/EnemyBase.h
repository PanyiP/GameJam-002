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

	float GetCausedDamage() const;
	float GetMaxHealth() const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakeDamageCallout(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

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

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* HitBox;
};
