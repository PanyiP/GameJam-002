#include "PlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
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
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

   if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
   {
      Input->BindAction(MoveUpAction, ETriggerEvent::Triggered, this, &ThisClass::MoveUp);
      Input->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ThisClass::MoveRight);
   }
}

void APlayerCharacter::MoveUp(const FInputActionValue& Value)
{
   AddMovementInput(FVector(0.f, Value.Get<float>(), 0.f));
}

void APlayerCharacter::MoveRight(const FInputActionValue& Value)
{
   AddMovementInput(FVector(Value.Get<float>(), 0.f, 0.f));
}
