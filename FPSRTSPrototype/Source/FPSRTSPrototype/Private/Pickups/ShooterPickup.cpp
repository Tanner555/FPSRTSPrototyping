// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSRTSPrototype.h"
#include "AllyMember.h"
#include "RTSProtoGameMode.h"
#include "ShooterPickup.h"


// Sets default values
//AShooterPickup::AShooterPickup()
//{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

//}

AShooterPickup::AShooterPickup(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UCapsuleComponent* CollisionComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("CollisionComp"));
	CollisionComp->InitCapsuleSize(40.0f, 50.0f);
	CollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_Visibility);//COLLISION_PICKUP
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = CollisionComp;

	PickupPSC = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("PickupFX"));
	PickupPSC->bAutoActivate = false;
	PickupPSC->bAutoDestroy = false;
	PickupPSC->AttachParent = RootComponent;

	RespawnTime = 10.0f;
	bIsActive = false;
	PickedUpBy = NULL;

	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
}

void AShooterPickup::BeginPlay()
{
	Super::BeginPlay();

	RespawnPickup();

	// register on pickup list (server only), don't care about unregistering (in FinishDestroy) - no streaming
	ARTSProtoGameMode* GameMode = GetWorld()->GetAuthGameMode<ARTSProtoGameMode>();
	if (GameMode)
	{
		GameMode->LevelPickups.Add(this);
	}
}

void AShooterPickup::NotifyActorBeginOverlap(class AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);
	PickupOnTouch(Cast<AAllyMember>(Other));
}

bool AShooterPickup::CanBePickedUp(class AAllyMember* TestPawn) const
{
	return TestPawn && TestPawn->isAlive();
}

void AShooterPickup::GivePickupTo(class AAllyMember* Pawn)
{
}

void AShooterPickup::PickupOnTouch(class AAllyMember* Pawn)
{
	if (bIsActive && Pawn && Pawn->isAlive()/* && !IsPendingKill()*/)
	{
		if (CanBePickedUp(Pawn))
		{
			GivePickupTo(Pawn);
			PickedUpBy = Pawn;

			if (!IsPendingKill())
			{
				bIsActive = false;
				OnPickedUp();

				if (RespawnTime > 0.0f)
				{
					GetWorldTimerManager().SetTimer(TimerHandle_RespawnPickup, this, &AShooterPickup::RespawnPickup, RespawnTime, false);
				}
			}
		}
	}
}

void AShooterPickup::RespawnPickup()
{
	bIsActive = true;
	PickedUpBy = NULL;
	OnRespawned();

	TArray<AActor*> OverlappingPawns;
	GetOverlappingActors(OverlappingPawns, AAllyMember::StaticClass());

	for (int32 i = 0; i < OverlappingPawns.Num(); i++)
	{
		PickupOnTouch(Cast<AAllyMember>(OverlappingPawns[i]));
	}
}

void AShooterPickup::OnPickedUp()
{
	if (RespawningFX)
	{
		PickupPSC->SetTemplate(RespawningFX);
		PickupPSC->ActivateSystem();
	}
	else
	{
		PickupPSC->DeactivateSystem();
	}

	if (PickupSound && PickedUpBy)
	{
		UGameplayStatics::SpawnSoundAttached(PickupSound, PickedUpBy->GetRootComponent());
	}

	OnPickedUpEvent();
}

void AShooterPickup::OnRespawned()
{
	if (ActiveFX)
	{
		PickupPSC->SetTemplate(ActiveFX);
		PickupPSC->ActivateSystem();
	}
	else
	{
		PickupPSC->DeactivateSystem();
	}

	const bool bJustSpawned = CreationTime <= (GetWorld()->GetTimeSeconds() + 5.0f);
	if (RespawnSound && !bJustSpawned)
	{
		UGameplayStatics::PlaySoundAtLocation(this, RespawnSound, GetActorLocation());
	}

	OnRespawnEvent();
}

void AShooterPickup::OnRep_IsActive()
{
	if (bIsActive)
	{
		OnRespawned();
	}
	else
	{
		OnPickedUp();
	}
}

void AShooterPickup::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AShooterPickup, bIsActive);
	//DOREPLIFETIME(AShooterPickup, PickedUpBy);
}