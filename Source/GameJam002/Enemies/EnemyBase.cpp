#include "Components/BoxComponent.h"
#include "EnemyBase.h"
#include "GameJam002/PlayerCharacter/PlayerCharacter.h"
#include "PaperFlipbookComponent.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

   HitBox->SetBoxExtent(FVector(30.f, 25.f, 1.f));
   HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
   HitBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
   HitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
   HitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

   this->OnTakeAnyDamage.AddDynamic(this, &ThisClass::TakeDamageCallout);
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBase::TakeDamageCallout(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
   Super::TakeDamageCallout(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);

   if (Health == 0)
   {
      if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(DamageCauser))
      {
         PlayerCharacter->GainExperience(GetOnDeathExperience());
         if (DeathAnimation)
         {
            GetSprite()->SetFlipbook(DeathAnimation);
            GetSprite()->SetLooping(false);
            GetSprite()->OnFinishedPlaying.AddDynamic(this, &ThisClass::DeathAnimationFinished);
            if (!GetSprite()->IsPlaying()) GetSprite()->Play();
         }
         else
         {
            this->Destroy();
         }
      }
   }
}

void AEnemyBase::DeathAnimationFinished()
{
   GetSprite()->OnFinishedPlaying.RemoveDynamic(this, &ThisClass::DeathAnimationFinished);
   this->Destroy();
}
