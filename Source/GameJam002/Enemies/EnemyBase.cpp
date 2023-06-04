#include "EnemyBase.h"
#include "Components/BoxComponent.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

   HitBox = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("HitBox")));
   HitBox->SetupAttachment(GetRootComponent());
   HitBox->SetBoxExtent(FVector(30.f, 25.f, 1.f));
   HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
   HitBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
   HitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
   HitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

   Health = GetMaxHealth();
   this->OnTakeAnyDamage.AddDynamic(this, &ThisClass::TakeDamageCallout);
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AEnemyBase::GetCausedDamage() const
{
   return (BaseDamage + AdditionalDamage) * DamageMultiplier;
}

float AEnemyBase::GetMaxHealth() const
{
   return (BaseHealth + AdditionalHealth) * HealthMultiplier;
}

void AEnemyBase::TakeDamageCallout(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
   Health = FMath::Clamp(Health - Damage, 0, GetMaxHealth());
   UE_LOG(LogTemp, Warning, TEXT("Enemy health: %.2f"), Health);
   if (Health == 0) this->Destroy();
}
