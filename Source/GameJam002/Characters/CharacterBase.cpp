#include "CharacterBase.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "TimerManager.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("HitBox")));
	HitBox->SetupAttachment(GetRootComponent());

	DamageTakenText = CreateDefaultSubobject<UTextRenderComponent>(FName(TEXT("DamageTakenText")));
	DamageTakenText->SetupAttachment(GetRootComponent());
	DamageTakenText->AddRelativeRotation(FRotator(90.f, 0.f, -90.f));
	DamageTakenText->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	DamageTakenText->WorldSize = 12.f;
	DamageTakenText->SetRelativeLocation(FVector(0.f, -15.f, 0.f));
	DamageTakenText->SetText(FText::GetEmpty());
	DamageTakenText->SetTextRenderColor(FColor::FColor(255, 90, 90));
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	Health = GetMaxHealth();
}

void ACharacterBase::TakeDamageCallout(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamageTakenText)
	{
		GetWorldTimerManager().SetTimer(DamageTakenTimerHandle, this, &ThisClass::DamageTakenTimerHandleCallout, 1.0f);
		DamageTakenText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), Damage)));
	}
	Health = FMath::Clamp(Health - Damage, 0, GetMaxHealth());
	UE_LOG(LogTemp, Warning, TEXT("%s health: %.2f"), *DamagedActor->GetActorNameOrLabel(), Health);
}

void ACharacterBase::DamageTakenTimerHandleCallout()
{
	DamageTakenText->SetText(FText::GetEmpty());
}
