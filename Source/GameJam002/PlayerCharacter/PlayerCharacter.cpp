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
      Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
   }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
   AddMovementInput(FVector(Value.Get<FVector2D>().X, Value.Get<FVector2D>().Y, 0.f));
}
