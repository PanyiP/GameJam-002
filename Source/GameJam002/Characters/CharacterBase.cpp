#include "CharacterBase.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	Health = GetMaxHealth();
}

void ACharacterBase::TakeDamageCallout(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0, GetMaxHealth());
	UE_LOG(LogTemp, Warning, TEXT("%s health: %.2f"), *DamagedActor->GetActorNameOrLabel(), Health);
}
