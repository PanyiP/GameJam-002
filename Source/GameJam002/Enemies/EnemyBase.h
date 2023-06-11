#pragma once

#include "CoreMinimal.h"
#include "GameJam002\Characters\CharacterBase.h"
#include "EnemyBase.generated.h"

UCLASS()
class GAMEJAM002_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemyBase();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	/*
	* Misc
	*/
	virtual void TakeDamageCallout(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, Category = "Character Stats")
	float OnDeathBaseExperience = 5.f;
	FORCEINLINE float GetOnDeathExperience() const { return OnDeathBaseExperience * CharacterLevel; }

	UFUNCTION()
	void DeathAnimationFinished();

private:

};
