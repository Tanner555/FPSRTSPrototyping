// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSRTSPrototype.h"
#include "RTSControllerCPP.h"
#include "AllyMember.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"


ARTSControllerCPP::ARTSControllerCPP(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	//AllyBlackBoardComp = PCIP.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("AllyBlackBoardComp"));
	//AllyBehaviorTreeComp = PCIP.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("AllyBehaviorTreeComp"));
	BehaviorTreeComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackBoardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
	//Vectors
	moveCommandLocation = "moveCommandLocation";
	LocationOfSound = "LocationOfSound";
	WanderPoint = "WanderPoint";
	WanderLocation = "WanderLocation";
	targetedEnemyLocation = "targetedEnemyLocation";
	//Bools
	isTargetingEnemy = "isTargetingEnemy";
	moveCommandActive = "moveCommandActive";
	HasHeardSound = "HasHeardSound";
	//Object Actors
	targetedEnemy = "targetedEnemy";
	SelfActor = "SelfActor";
	PlayerActor = "PlayerActor";
	AllyActor = "AllyActor";
	PatrolPoint = "PatrolPoint";
	NeedAmmo = "NeedAmmo";
	HasLosToEnemy = "HasLosToEnemy";
	Enemy = "Enemy";
	Destination = "Destination";
	WantsToMove = "WantsToMove";
	bWantsPlayerState = true;
	
}

void ARTSControllerCPP::BeginPlay() {
	Super::BeginPlay();
	
}

void ARTSControllerCPP::Possess(class APawn* InPawn) {
	Super::Possess(InPawn);

	AAllyMember* AllyMember = Cast<AAllyMember>(InPawn);
	if (AllyMember != nullptr && AllyMember->AllyBehaviorTree) {
		if (AllyMember->AllyBehaviorTree->BlackboardAsset) {
			BlackBoardComp->InitializeBlackboard(*AllyMember->AllyBehaviorTree->BlackboardAsset);
		}
		EnemyKeyID = BlackBoardComp->GetKeyID("Enemy");
		NeedAmmoKeyID = BlackBoardComp->GetKeyID("NeedAmmo");
		BehaviorTreeComp->StartTree(*AllyMember->AllyBehaviorTree);
	}
}

void ARTSControllerCPP::UnPossess() {
	Super::UnPossess();
	BehaviorTreeComp->StopTree();
}

void ARTSControllerCPP::BeginInactiveState()
{
	Super::BeginInactiveState();

	AGameState* GameState = GetWorld()->GameState;

	const float MinRespawnDelay = (GameState && GameState->GameModeClass) ? GetDefault<AGameMode>(GameState->GameModeClass)->MinRespawnDelay : 1.0f;

	//GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &AShooterAIController::Respawn, MinRespawnDelay);
}

void ARTSControllerCPP::Respawn()
{
	//GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}

void ARTSControllerCPP::FindClosestEnemy()
{
	APawn* MyBot = GetPawn();
	if (MyBot == NULL)
	{
		return;
	}

	const FVector MyLoc = MyBot->GetActorLocation();
	float BestDistSq = MAX_FLT;
	AAllyMember* BestPawn = NULL;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		AAllyMember* TestPawn = Cast<AAllyMember>(*It);
		if (TestPawn == nullptr)
			continue;

		if (TestPawn && TestPawn->isAlive() && TestPawn->isEnemyFor(Cast<AAllyMember>(this->GetPawn())))
		{
			const float DistSq = (TestPawn->GetActorLocation() - MyLoc).SizeSquared();
			if (DistSq < BestDistSq)
			{
				BestDistSq = DistSq;
				BestPawn = TestPawn;
			}
		}
	}

	if (BestPawn)
	{
		SetEnemy(BestPawn);
	}
}

bool ARTSControllerCPP::FindClosestEnemyWithLOS(AAllyMember* ExcludeEnemy)
{
	bool bGotEnemy = false;
	APawn* MyBot = GetPawn();
	if (MyBot != NULL)
	{
		const FVector MyLoc = MyBot->GetActorLocation();
		float BestDistSq = MAX_FLT;
		AAllyMember* BestPawn = NULL;

		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			AAllyMember* TestPawn = Cast<AAllyMember>(*It);
			if (TestPawn == nullptr)
				continue;

			if (TestPawn && TestPawn != ExcludeEnemy && TestPawn->isAlive() && TestPawn->isEnemyFor(Cast<AAllyMember>(this->GetPawn())))
			{
				if (HasWeaponLOSToEnemy(TestPawn, true) == true)
				{
					const float DistSq = (TestPawn->GetActorLocation() - MyLoc).SizeSquared();
					if (DistSq < BestDistSq)
					{
						BestDistSq = DistSq;
						BestPawn = TestPawn;
					}
				}
			}
		}
		if (BestPawn)
		{
			SetEnemy(BestPawn);
			bGotEnemy = true;
		}
	}
	return bGotEnemy;
}

bool ARTSControllerCPP::HasWeaponLOSToEnemy(AActor* InEnemyActor, const bool bAnyEnemy) const
{
	if (InEnemyActor == nullptr)
		return true;

	static FName LosTag = FName(TEXT("AIWeaponLosTrace"));

	AAllyMember* MyBot = Cast<AAllyMember>(GetPawn());

	bool bHasLOS = false;
	// Perform trace to retrieve hit info
	//FCollisionQueryParams TraceParams(LosTag, true, GetPawn());
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetPawn());
	TraceParams.bTraceAsyncScene = true;

	TraceParams.bReturnPhysicalMaterial = true;
	FVector StartLocation = MyBot->GetActorLocation();
	StartLocation.Z += GetPawn()->BaseEyeHeight; //look from eyes

	FHitResult Hit(ForceInit);
	const FVector EndLocation = InEnemyActor->GetActorLocation();
	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, TraceParams);
	if (Hit.bBlockingHit == true)
	{
		// Theres a blocking hit - check if its our enemy actor
		AActor* HitActor = Hit.GetActor();
		if (Hit.GetActor() != NULL)
		{
			// If the hit is our target actor consider it LOS
			if (HitActor == InEnemyActor)
			{
				bHasLOS = true;
			}
			else if(bAnyEnemy == true)
			{
				// Check the team of us against the team of the actor we hit if we are able. If they dont match good to go.
				ACharacter* HitChar = Cast<ACharacter>(HitActor);
				AAllyMember* HitAlly = Cast<AAllyMember>(HitActor);
				if (HitChar != NULL && HitAlly)
				{
					if (HitAlly->GetFactionPlayerCount() != MyBot->GetFactionPlayerCount())
					{
						bHasLOS = true;
					}
				}
			}
		}
	}
	return bHasLOS;
}

void ARTSControllerCPP::ShootEnemy()
{
	AAllyMember* MyBot = Cast<AAllyMember>(GetPawn());
	if (MyBot == nullptr)
		return;
	/*AShooterWeapon* MyWeapon = MyBot ? MyBot->GetWeapon() : NULL;
	if (MyWeapon == NULL)
	{
		return;
	}*/

	bool bCanShoot = false;
	AAllyMember* Enemy = GetEnemy();
	if (Enemy == nullptr) {
		MyBot->StopShootingBehavior();
		return;
	}

	if (Enemy && (Enemy->isAlive()) && (MyBot->getCurrentAmmo() > 0) && (MyBot->CanFire() == true))
	{
		if (LineOfSightTo(Enemy, MyBot->GetActorLocation()))
		{
			bCanShoot = true;
		}
	}

	if (bCanShoot)
	{
		MyBot->StartShootingBehavior();
	}
	else
	{
		MyBot->StopShootingBehavior();
	}
}

AAllyMember* ARTSControllerCPP::GetEnemy() const
{
	if (BlackBoardComp)
	{
		return Cast<AAllyMember>(BlackBoardComp->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));
	}

	return NULL;
}

void ARTSControllerCPP::SetEnemy(AAllyMember* AllyPawn)
{
	if (BlackBoardComp)
	{
		APawn* InPawn = Cast<AAllyMember>(AllyPawn);
		BlackBoardComp->SetValue<UBlackboardKeyType_Object>(EnemyKeyID, InPawn);
		SetFocus(InPawn);
	}
}

//const class AShooterWeapon* CurrentWeapon
void ARTSControllerCPP::CheckAmmo(AAllyMember* Ally)
{
	if (Ally && BlackBoardComp)
	{
		const int32 Ammo = Ally->getCurrentAmmo();
		const int32 MaxAmmo = Ally->GetMaxAmmo();
		const float Ratio = (float)Ammo / (float)MaxAmmo;

		BlackBoardComp->SetValue<UBlackboardKeyType_Bool>(NeedAmmoKeyID, (Ratio <= 0.1f));
	}
}

/*void ARTSControllerCPP::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
//{
	// Look toward focus
	FVector FocalPoint = GetFocalPoint();
	if (!FocalPoint.IsZero() && GetPawn())
	{
		FVector Direction = FocalPoint - GetPawn()->GetActorLocation();
		FRotator NewControlRotation = Direction.Rotation();

		NewControlRotation.Yaw = FRotator::ClampAxis(NewControlRotation.Yaw);

		SetControlRotation(NewControlRotation);

		APawn* const P = GetPawn();
		if (P && bUpdatePawn)
		{
			P->FaceRotation(NewControlRotation, DeltaTime);
		}

	}
}*/

void ARTSControllerCPP::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	// Stop the behaviour tree/logic
	BehaviorTreeComp->StopTree();

	// Stop any movement we already have
	StopMovement();

	// Cancel the repsawn timer
	GetWorldTimerManager().ClearTimer(TimerHandle_Respawn);

	// Clear any enemy
	SetEnemy(NULL);

	// Finally stop firing
	AAllyMember* MyBot = Cast<AAllyMember>(GetPawn());
	//AShooterWeapon* MyWeapon = MyBot ? MyBot->GetWeapon() : NULL;
	/*if (MyWeapon == NULL)
	{
		return;
	}*/
	MyBot->StopShootingBehavior();
}

void ARTSControllerCPP::setWantsToMove(bool shouldMove) {
	BlackBoardComp->SetValueAsBool(WantsToMove, shouldMove);
}