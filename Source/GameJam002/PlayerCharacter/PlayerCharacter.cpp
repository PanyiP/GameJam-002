#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "PaperFlipbookComponent.h"

APlayerCharacter::APlayerCharacter()
{
   PrimaryActorTick.bCanEverTick = true;

   CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName(TEXT("CameraBoom")));
   CameraBoom->SetupAttachment(GetRootComponent());
   CameraBoom->AddRelativeRotation(FRotator(-90.f, 0.f, -90.f));

   Camera = CreateDefaultSubobject<UCameraComponent>(FName(TEXT("Camera")));
   Camera->SetupAttachment(CameraBoom);
}

void APlayerCharacter::BeginPlay()
{
   Super::BeginPlay();

   if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
   {
      if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
      {
         if (InputMappingContext)
         {
            InputSystem->AddMappingContext(InputMappingContext, 0);
         }
      }
   }
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
   }
}

void APlayerCharacter::AttackFinished()
{
   GetSprite()->OnFinishedPlaying.RemoveDynamic(this, &ThisClass::AttackFinished);
   CharacterState = ECharacterState::ECS_Idle;
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
