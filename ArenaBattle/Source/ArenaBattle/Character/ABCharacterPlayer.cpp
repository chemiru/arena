#include "Character/ABCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "UI/ABHUDWidget.h"
#include "ABCharacterControlData.h"
#include "CharacterStat/ABCharacterStatComponent.h"


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
   

    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump'"));
    if (InputActionJumpRef.Succeeded())
    {
        JumpAction = InputActionJumpRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef
    (TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
    if (InputChangeActionControlRef.Succeeded())
    {
        ChangeControlAction = InputChangeActionControlRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef
    (TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShoulerMove.IA_ShoulerMove'"));
    if (InputActionShoulderMoveRef.Succeeded())
    {
        ShoulderMoveAction = InputActionShoulderMoveRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef
    (TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
    if (InputActionShoulderLookRef.Succeeded())
    {
        ShoulderLookAction = InputActionShoulderLookRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef
    (TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
    if (InputActionQuaterMoveRef.Succeeded())
    {
        QuaterMoveAction = InputActionQuaterMoveRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef
    (TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Attack.IA_Attack'"));
    if (InputActionAttackRef.Succeeded())
    {
        AttackAction = InputActionAttackRef.Object;
    }

    CurrentCharacterControlType = ECharacterControlType::Quater;
}

void AABCharacterPlayer::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController)
    {
        EnableInput(PlayerController);

    }
    SetCharacterControl(CurrentCharacterControlType);

}

void AABCharacterPlayer::SetDead()
{
    Super::SetDead();

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController)
    {
        DisableInput(PlayerController);

    }
}

void AABCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
    EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ChangeCharacterControl);

    EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderMove);
    EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderLook);
    EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::QuaterMove);
    EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Attack);


}

void AABCharacterPlayer::ChangeCharacterControl()
{
    if (CurrentCharacterControlType == ECharacterControlType::Quater)
    {
        SetCharacterControl(ECharacterControlType::Shoulder);
    }
    else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
    {
        SetCharacterControl(ECharacterControlType::Quater);
    }
}

void AABCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
    UABCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
    check(NewCharacterControl);
    SetCharacterControlData(NewCharacterControl);

    APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
    if (PlayerController)
    {
        ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

        if (LocalPlayer)
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                Subsystem->ClearAllMappings();
                UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
                if (NewMappingContext)
                {
                    Subsystem->AddMappingContext(NewMappingContext, 0);
                }

            }
        }
    }
    CurrentCharacterControlType = NewCharacterControlType;
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

void AABCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
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

void AABCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    // 값 제한 (최대 ±100)
   /* LookAxisVector.X = FMath::Clamp(LookAxisVector.X, -100.f, 100.f);
    LookAxisVector.Y = FMath::Clamp(LookAxisVector.Y, -100.f, 100.f);*/

    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
}

void AABCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
   
    FVector2D MovementVector = Value.Get<FVector2D>();
    float InputSizeSquared = MovementVector.SquaredLength();
    float MovementVectorSize = 1.0f;
    float MovementVectorSizeSquared = MovementVector.SquaredLength();
    if (MovementVectorSizeSquared > 1.0f)
    {
        MovementVector.Normalize();
        MovementVectorSizeSquared = 1.0f;
    }
    else
    {
        MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
    }
    FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
    GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
    AddMovementInput(MoveDirection, MovementVectorSize);
   
}

void AABCharacterPlayer::Attack()
{
    ProcessComboCommand();
}

void AABCharacterPlayer::SetupHUDWidget(UABHUDWidget* InHUDWidget)
{

    if (InHUDWidget)
    {
        InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
        InHUDWidget->UpdateHpBar(Stat->GetCurrentHp());

        Stat->OnStatChanged.AddUObject(InHUDWidget, &UABHUDWidget::UpdateStat);
        Stat->OnHpChanged.AddUObject(InHUDWidget, &UABHUDWidget::UpdateHpBar);
    }
}


