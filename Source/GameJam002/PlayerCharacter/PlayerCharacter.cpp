#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "PaperFlipbookComponent.h"
#include "PlayerCharacter.h"

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

   UpdateAnimation();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

   if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
   {
      Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
   }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
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

void APlayerCharacter::UpdateAnimation()
{
   if (GetMovementComponent() && GetMovementComponent()->Velocity.Length() > 0.f)
   {
      switch (CharacterDirection)
      {
      case ECharacterDirection::ECD_Left:
         if (RunLeftAnimation) GetSprite()->SetFlipbook(RunLeftAnimation);
         break;
      case ECharacterDirection::ECD_Right:
         if (RunRightAnimation) GetSprite()->SetFlipbook(RunRightAnimation);
         break;
      default:
         break;
      }
   }
   else
   {
      if (IdleAnimation) GetSprite()->SetFlipbook(IdleAnimation);
   }
}
