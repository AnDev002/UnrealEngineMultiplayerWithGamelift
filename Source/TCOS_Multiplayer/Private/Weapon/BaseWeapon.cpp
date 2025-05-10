// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"
#include "AbilitySystem/TCOSAbilitySystemComponent.h"
#include "DataAssets/WeaponInfo.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Character/TCOS_MultiplayerCharacter.h"
#include "Weapon/WeaponTypes.h"


// Sets default values
ABaseWeapon::ABaseWeapon()
{
    bReplicates = true;
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 
    WeaponMesh->SetCollisionObjectType(ECC_WorldDynamic);
    WeaponMesh->SetGenerateOverlapEvents(true);
    WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
    AreaSphere->SetupAttachment(RootComponent);
    AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    AbilitySystem = CreateDefaultSubobject<UTCOSAbilitySystemComponent>(TEXT("AbilitySystem"));
    AbilitySystem->SetIsReplicated(true);
    PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
    PickupWidget->SetupAttachment(RootComponent);
}

void ABaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    if (WeaponData && HasAuthority())
    {
        WeaponMesh->SetSkeletalMesh(WeaponData->WeaponMesh);
        //ComboMontages = WeaponData->ComboMontages; 
        //DamageEffect = WeaponData->DamageEffect;
        MulticastUpdateVisual();

        AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
        AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnSphereOverlap);
        AreaSphere->OnComponentEndOverlap.AddDynamic(this, &ABaseWeapon::OnSphereEndOverlap);
    }
    if (PickupWidget)
    {
        PickupWidget->SetVisibility(false);
    }
}

TSoftObjectPtr<UAnimMontage> ABaseWeapon::GetHitReactionMontageByDirection(EHitDirection Direction) const
{
    if (TSoftObjectPtr<UAnimMontage>* MontagePtr = WeaponData->HitReactionMontages.Find(Direction))
    {
        if (MontagePtr->IsValid()) 
        {
            return *MontagePtr;
        }
        else
        {
            UAnimMontage* LoadedMontage = MontagePtr->LoadSynchronous();
            if (LoadedMontage)
            {
                return *MontagePtr;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Montage failed to load."));
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Dont have any Montage matched!"));
    }
    return nullptr;
}

void ABaseWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ATCOS_MultiplayerCharacter* PlayerCharacter = Cast<ATCOS_MultiplayerCharacter>(OtherActor);

    if (PlayerCharacter)
    {
        PlayerCharacter->SetOverlappingWeapon(this);
    }
}

void ABaseWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ATCOS_MultiplayerCharacter* PlayerCharacter = Cast<ATCOS_MultiplayerCharacter>(OtherActor);

    if (PlayerCharacter)
    {
        PlayerCharacter->SetOverlappingWeapon(nullptr);
    }
}

void ABaseWeapon::OnRep_WeaponState()
{
    switch (WeaponState)
    {
    case EWeaponState::EWS_Equipped:
        ShowPickupWidget(false);
        AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        break;
    }
}

void ABaseWeapon::SetWeaponState(EWeaponState State)
{
    WeaponState = State;
    switch (WeaponState)
    {
    case EWeaponState::EWS_Equipped:
        ShowPickupWidget(false);
        AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        break;
    }
}

void ABaseWeapon::ShowPickupWidget(bool bShowWidget)
{
    if (PickupWidget)
    {
        PickupWidget->SetVisibility(bShowWidget);
    }
}

void ABaseWeapon::MulticastUpdateVisual_Implementation()
{
    if (WeaponData)
    {
        WeaponMesh->SetSkeletalMesh(WeaponData->WeaponMesh);
    }
}

UAbilitySystemComponent* ABaseWeapon::GetAbilitySystemComponent() const
{
    return AbilitySystem;
}

void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ABaseWeapon, WeaponData);
}
