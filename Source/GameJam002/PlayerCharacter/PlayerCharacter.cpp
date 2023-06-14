#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameJam002/Enemies/EnemyBase.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"
#include "GameJam002/UI/CharacterOverlay.h"

APlayerCharacter::APlayerCharacter()
{
   PrimaryActorTick.bCanEverTick = true;

   CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName(TEXT("CameraBoom")));
   CameraBoom->SetupAttachment(GetRootComponent());
   CameraBoom->AddRelativeRotation(FRotator(-90.f, 0.f, -90.f));

   Camera = CreateDefaultSubobject<UCameraComponent>(FName(TEXT("Camera")));
   Camera->SetupAttachment(CameraBoom);

   HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
   HitBox->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
   HitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
   HitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
}

void APlayerCharacter::BeginPlay()
{
   Super::BeginPlay();

   APlayerController* PlayerController = Cast<APlayerController>(GetController());

   if (PlayerController)
   {
      if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
      {
         if (InputMappingContext)
         {
            InputSystem->AddMappingContext(InputMappingContext, 0);
         }
      }

      if (HUDClass)
      {
         HUD = CreateWidget<UCharacterOverlay>(PlayerController, HUDClass);
         HUD->AddToViewport();
         UpdateHUD();
      }
   }

   this->OnTakeAnyDamage.AddDynamic(this, &ThisClass::TakeDamageCallout);
}

void APlayerCharacter::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);

   IsMoving();
   UpdateAnimation();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
   Super::SetupPlayerInputComponent(PlayerInputComponent);

   if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
   {
      Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
      Input->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::Attack);
      Input->BindAction(OpenPauseMenuAction, ETriggerEvent::Triggered, this, &ThisClass::OpenPauseMenu);
   }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
   if (CharacterState == ECharacterState::ECS_Attacking) return;

   AddMovementInput(FVector(Value.Get<FVector2D>().X, Value.Get<FVector2D>().Y, 0.f));

   if (Value.Get<FVector2D>().X > 0.f)
   {
      CharacterDirection = ECharacterDirection::ECD_Right;
   }
   else if (Value.Get<FVector2D>().X < 0.f)
   {
      CharacterDirection = ECharacterDirection::ECD_Left;
   }
}

void APlayerCharacter::Attack(const FInputActionValue& Value)
{
   if (CharacterState != ECharacterState::ECS_Attacking)
   {
      CharacterState = ECharacterState::ECS_Attacking;
      GetSprite()->SetFlipbook(AttackAnimations[FMath::RandRange(0, AttackAnimations.Num() - 1)]);
      GetSprite()->OnFinishedPlaying.AddDynamic(this, &ThisClass::AttackFinished);
      if (AttackSound) UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation());
      AttackHitCheck();
   }
}

void APlayerCharacter::AttackFinished()
{
   GetSprite()->OnFinishedPlaying.RemoveDynamic(this, &ThisClass::AttackFinished);
   CharacterState = ECharacterState::ECS_Idle;
}

void APlayerCharacter::AttackHitCheck()
{
   TArray<AActor*> OverlappingActors;
   HitBox->GetOverlappingActors(OverlappingActors, AEnemyBase::StaticClass());
   for (AActor* OverlappingActor : OverlappingActors)
   {
      if (AEnemyBase* DamagedActor = Cast<AEnemyBase>(OverlappingActor))
      {
         if (AController* InstigatorController = DamagedActor->GetController())
         {
            UGameplayStatics::ApplyDamage(
               DamagedActor,
               GetCausedDamage(),
               InstigatorController,
               this,
               UDamageType::StaticClass()
            );
         }
      }
   }
}

void APlayerCharacter::OpenPauseMenu(const FInputActionValue& Value)
{
   APlayerController* PlayerController = Cast<APlayerController>(GetController());

   if (PauseMenuClass && PlayerController)
   {
      HUD->RemoveFromParent();
      PauseMenu = CreateWidget<UCharacterOverlay>(PlayerController, PauseMenuClass);
      PauseMenu->AddToViewport();
      PlayerController->SetShowMouseCursor(true);
   }
}

void APlayerCharacter::UpdateAnimation()
{
   switch (CharacterState)
   {
   case ECharacterState::ECS_Idle:
      if (IdleAnimation)
      {
         GetSprite()->SetLooping(true);
         GetSprite()->SetFlipbook(IdleAnimation);
         if (!GetSprite()->IsPlaying()) GetSprite()->Play();
      }
      break;
   case ECharacterState::ECS_Running:
      GetSprite()->SetLooping(true);
      PlayRunningSound();
      switch (CharacterDirection)
      {
      case ECharacterDirection::ECD_Left:
         if (RunLeftAnimation) GetSprite()->SetFlipbook(RunLeftAnimation);
         if (!GetSprite()->IsPlaying()) GetSprite()->Play();
         break;
      case ECharacterDirection::ECD_Right:
         if (RunRightAnimation) GetSprite()->SetFlipbook(RunRightAnimation);
         if (!GetSprite()->IsPlaying()) GetSprite()->Play();
         break;
      default:
         break;
      }
      break;
   case ECharacterState::ECS_Attacking:
      if (!AttackAnimations.IsEmpty())
      {
         CharacterState = ECharacterState::ECS_Attacking;
         GetSprite()->SetLooping(false);
         if (!GetSprite()->IsPlaying()) GetSprite()->Play();
      }
      break;
   default:
      break;
   }
}

void APlayerCharacter::IsMoving()
{
   if (CharacterState == ECharacterState::ECS_Attacking) return;

   if (GetMovementComponent() && GetMovementComponent()->Velocity.Length() > 0.f)
   {
      CharacterState = ECharacterState::ECS_Running;
   }
   else
   {
      CharacterState = ECharacterState::ECS_Idle;
   }
}

void APlayerCharacter::PlayRunningSound()
{
   if (bIsPlayingRunningSound || RunningSound == nullptr) return;

   UGameplayStatics::PlaySoundAtLocation(this, RunningSound, GetActorLocation());
   GetWorldTimerManager().SetTimer(RunningSoundTimerHandle, this, &ThisClass::RunningSoundTimerHandleCallout, RunningSoundFrequency);
   bIsPlayingRunningSound = true;
}

void APlayerCharacter::RunningSoundTimerHandleCallout()
{
   bIsPlayingRunningSound = false;
}

void APlayerCharacter::TakeDamageCallout(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
   if (bIsDead) return;

   Super::TakeDamageCallout(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);

   UpdateHUD();

   if (Health > 0)
   {
      GetSprite()->SetLooping(false);
      if (RunRightAnimation) GetSprite()->SetFlipbook(HitTakenAnimation);
      if (!GetSprite()->IsPlaying()) GetSprite()->Play();
   }
   else
   {
      bIsDead = true;
      GetSprite()->SetLooping(false);
      if (RunRightAnimation) GetSprite()->SetFlipbook(DeathAnimation);
      if (!GetSprite()->IsPlaying()) GetSprite()->Play();

      this->Destroy();
   }
}

void APlayerCharacter::GainExperience(float Experience)
{
   CharacterExperience += Experience;

   UpdateHUD();

   if (CharacterLevelMap::CharLvlXpMap.Contains(CharacterLevel + 1))
   {
      if (CharacterExperience >= CharacterLevelMap::CharLvlXpMap[CharacterLevel + 1])
      {
         GainLevel();
      }
   }
}

void APlayerCharacter::GainLevel()
{
   CharacterLevel++;

   BaseMinimumDamage++;
   BaseMaximumDamage++;

   Health = GetMaxHealth();

   UpdateHUD();
}

void APlayerCharacter::UpdateHUD()
{
   if (HUD)
   {
      HUD->HealthValueText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), FMath::FloorToInt(Health), FMath::FloorToInt(GetMaxHealth()))));
      HUD->HealthBar->SetPercent(Health / GetMaxHealth());

      if (CharacterLevelMap::CharLvlXpMap.Contains(CharacterLevel + 1))
      {
         float XpSinceLastLevelUp = CharacterExperience - CharacterLevelMap::CharLvlXpMap[CharacterLevel];
         float TotalXpForNextLevel = CharacterLevelMap::CharLvlXpMap[CharacterLevel + 1] - CharacterLevelMap::CharLvlXpMap[CharacterLevel];
         HUD->XPBar->SetPercent(XpSinceLastLevelUp / TotalXpForNextLevel);
         HUD->XPValueText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), FMath::FloorToInt(XpSinceLastLevelUp), FMath::FloorToInt(TotalXpForNextLevel))));
      }

      HUD->CharLvlText->SetText(FText::FromString(FString::Printf(TEXT("Character level: %d"), GetCharacterLevel())));
      HUD->DamageText->SetText(FText::FromString(FString::Printf(TEXT("Damage: %.1f - %.1f"), (BaseMinimumDamage + AdditionalDamage) * DamageMultiplier, (BaseMaximumDamage + AdditionalDamage) * DamageMultiplier)));
   }
}
