#include "Character/ABCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "ABCharacterControlData.h"


AABCharacterPlayer::AABCharacterPlayer()
{
    // Camera Setup
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // Input Mapping Setup
    static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/ArenaBattle/Input/IMC_Default.IMC_Default'"));
    if (InputMappingContextRef.Succeeded())
    {
        DefaultMappingContext = InputMappingContextRef.Object;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load Input Mapping Context"));
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Move.IA_Move'"));
    if (InputActionMoveRef.Succeeded())
    {
        MoveAction = InputActionMoveRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump'"));
    if (InputActionJumpRef.Succeeded())
    {
        JumpAction = InputActionJumpRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Look.IA_Look'"));
    if (InputActionLookRef.Succeeded())
    {
        LookAction = InputActionLookRef.Object;
    }
}

void AABCharacterPlayer::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController)
    {
        ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
        
        if (LocalPlayer)
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
        }
    }
}

void AABCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Move);
    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Look);
}

void AABCharacterPlayer::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
    Super::SetCharacterControlData(CharacterControlData);

    CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
    CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
    CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
    CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
    CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
    CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
    CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;



}

void AABCharacterPlayer::Move(const FInputActionValue& Value)
{
    if (!Value.IsNonZero())  // 입력이 없는 경우 무시
    {
        return;
    }

    FVector2D MovementVector = Value.Get<FVector2D>();

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDirection, MovementVector.X);
    AddMovementInput(RightDirection, MovementVector.Y);
}

void AABCharacterPlayer::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    // 값 제한 (최대 ±100)
    LookAxisVector.X = FMath::Clamp(LookAxisVector.X, -100.f, 100.f);
    LookAxisVector.Y = FMath::Clamp(LookAxisVector.Y, -100.f, 100.f);

    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
}




// 강의대로 해서 실패한 거임
// Fill out your copyright notice in the Description page of Project Settings.


//#include "Character/ABCharacterPlayer.h"
//#include "Camera/CameraComponent.h"
//#include "GameFramework/SpringArmComponent.h"
//#include "InputMappingContext.h"
//#include "EnhancedInputComponent.h"
//#include "EnhancedInputSubsystems.h"
//#include "GameFramework/PlayerController.h"
//
//
//AABCharacterPlayer::AABCharacterPlayer()
//{
//    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
//    CameraBoom->SetupAttachment(RootComponent);
//    CameraBoom->TargetArmLength = 400.0f;
//    CameraBoom->bUsePawnControlRotation = true;
//
//    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
//    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
//    FollowCamera->bUsePawnControlRotation = false;
//
//    //Input
//    static ConstructorHelpers::FObjectFinder<UInputMappingContext>
//        InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/ThirdPerson/Input/IMC_Default.IMC_Default'"));
//    if (nullptr != InputMappingContextRef.Object)
//    {
//        DefaultMappingContext = InputMappingContextRef.Object;
//    }
//
//    static ConstructorHelpers::FObjectFinder<UInputAction>
//        InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Move.IA_Move'"));
//    if (nullptr != InputActionMoveRef.Object)
//    {
//        MoveAction = InputActionMoveRef.Object;
//    }
//
//    static ConstructorHelpers::FObjectFinder<UInputAction>
//        InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Jump.IA_Jump'"));
//    if (nullptr != InputActionJumpRef.Object)
//    {
//        JumpAction = InputActionJumpRef.Object;
//    }
//
//    static ConstructorHelpers::FObjectFinder<UInputAction>
//        InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look'"));
//    if (nullptr != InputActionLookRef.Object)
//    {
//        LookAction = InputActionLookRef.Object;
//    }
//
//
//
//}

//void AABCharacterPlayer::BeginPlay()
//{
//    Super::BeginPlay();
//
//    APlayerController* PlayerController =
//        CastChecked<APlayerController>(GetController());
//    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController))
//    {
//        Subsystem->AddMappingContext(DefaultMappingContext, 0);
//
//    }
//
//}
//
//void AABCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
//{
//    Super::SetupPlayerInputComponent(PlayerInputComponent);
//    UEnhancedInputComponent* EnhancedInputComponent =
//        CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
//
//    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::
//        Triggered, this, &ACharacter::Jump);
//    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::
//        Completed, this, &ACharacter::StopJumping);
//    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::
//        Triggered, this, &AABCharacterPlayer::Move);
//    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::
//        Triggered, this, &AABCharacterPlayer::Look);
//
//}
//
//void AABCharacterPlayer::Move(const FInputActionValue& Value)
//{
//    FVector2D MovementVector = Value.Get<FVector2D>();
//
//    const FRotator Rotation = Controller->GetControlRotation();
//    const FRotator YawRotation(0, Rotation.Yaw, 0);
//
//    const FVector ForwardDirection = FRotationMatrix(YawRotation).
//        GetUnitAxis(EAxis::X);
//    const FVector RightDirection = FRotationMatrix(YawRotation).
//        GetUnitAxis(EAxis::Y);
//
//    AddMovementInput(ForwardDirection, MovementVector.Y);
//    AddMovementInput(RightDirection, MovementVector.X);
//}
//
//void AABCharacterPlayer::Look(const FInputActionValue& Value)
//{
//    FVector2D LookAxisVector = Value.Get<FVector2D>();
//
//    AddControllerYawInput(LookAxisVector.X);
//    AddControllerPitchInput(LookAxisVector.Y);
//}
