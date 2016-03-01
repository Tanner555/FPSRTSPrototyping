// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSRTSPrototype.h"
#include "AllyMember.h"
#include "PartyManagerCPP.h"
#include "RTSControllerCPP.h"
#include "FPSRTSPrototypeCharacter.h"
#include "Kismet/KismetMathLibrary.h"

AAllyMember::AAllyMember() :Super() {
	AllyMaxHealth = 100;
	AllyHealth = AllyMaxHealth;
	baseEnemyDamage = 25;
	CurrentAmmo = 20;
	MaxAmmo = 100;
	AllyCanFire = true;
	ExecutingShootingBehavior = false;
	wantsFreedomToMove = false;
	ResetAllyStats();
}

void AAllyMember::BeginPlay() {
	Super::BeginPlay();
	gamemode = Cast<ARTSProtoGameMode>(UGameplayStatics::GetGameMode(this));
	if (gamemode == nullptr) {
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Blue, "RTS GameMode does not exist!");
	}
}

void AAllyMember::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (isTargetingEnemy) {
		Cast<AAllyMember>(targetedEnemy);
		if (targetedEnemy != nullptr) {
			FRotator enemyRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), targetedEnemy->GetActorLocation());
			this->SetActorRotation(enemyRotation.Quaternion());
		}
		else { //Enemy does not exist, is no longer targeting enemy
			isTargetingEnemy = false;
		}
	}
}

float AAllyMember::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		addAllyHealth(-Damage);
		if (AllyHealth <= 0) {
			if (deathSparks != nullptr && deathSound != nullptr) {
				UGameplayStatics::SpawnEmitterAtLocation(this, deathSparks, this->GetActorLocation(), FRotator::ZeroRotator, true);
				UGameplayStatics::PlaySoundAtLocation(this, deathSound, this->GetActorLocation());
			}
			if (gamemode != nullptr) {
				for (int i = 0; i < gamemode->GeneralMembers.Num();i++) {
					if (gamemode->GeneralMembers[i]->generalCommander.operator==(generalCommander.GetValue())) {
						AAllyMember* firstally = gamemode->GeneralMembers[i]->FindPartyMembers(true, this);
						if (firstally != nullptr){
							gamemode->GeneralMembers[i]->SetAllyInCommand(firstally);
							Destroy(false, true);
						}
						else {
							//gamemode->CallGameOverEvent(generalCommander);
							Destroy(false, true);
						}
					}
					//Add to death count
					this->AddToDeathCount(1);
					//Check rewards for instigator
					AAllyMember* enemyAlly = Cast<AAllyMember>(DamageCauser);
					if (enemyAlly != nullptr) {
						//Killed enemy, give reward
						if (isEnemyFor(enemyAlly)){
						TEnumAsByte<ERTSRewardTypes> enemyReward;
						enemyReward.operator=(ERTSRewardTypes::Reward_Kill);
						int32 giveReward = gamemode->GetPendingReward(enemyAlly, enemyReward);
						enemyAlly->AddToPointsScored(giveReward);
						enemyAlly->AddToKillCount(1);
						}
						else {
							//Friendly Kill, give punishment
							TEnumAsByte<ERTSPunishmentTypes> friendlyPunishment;
							friendlyPunishment.operator=(ERTSPunishmentTypes::Punishment_KilledAnAlly);
							int32 givePunishment = gamemode->GetPendingPunishment(enemyAlly, friendlyPunishment);
							enemyAlly->AddToPointsScored(givePunishment);
						}
					}
				}
				//Update GameModeStats in the end
				gamemode->UpdateGameModeStats();
			}
		}
	return ActualDamage;
}

float AAllyMember::GetDamageRate(AActor* attacker) {
	return baseEnemyDamage;
}

void AAllyMember::AllyFire() {
	if (CurrentAmmo <= 0 || AllyCanFire == false)
		return;

	this->OnFire();
	FHitResult RaycastHitResult;
	UCameraComponent* fpscamera = this->GetFirstPersonCameraComponent();
	USceneComponent* cameracomp = Cast<USceneComponent>(fpscamera);
	FVector start = cameracomp->GetComponentLocation();
	FVector FwdVector = UKismetMathLibrary::GetForwardVector(this->GetActorRotation());
	FVector end = start + (FwdVector * 6000);
	FCollisionQueryParams traceparams;
	traceparams.AddIgnoredActor(Cast<AActor>(this));
	FCollisionResponseParams responseparams;
	bool hasHitSomething = GetWorld()->LineTraceSingleByChannel(RaycastHitResult, start, end,ECollisionChannel::ECC_Visibility,traceparams,responseparams);
	if (hasHitSomething) {
		AActor* hitActor = RaycastHitResult.Actor.Get(false);
		if (hitActor->IsA(AAllyMember::StaticClass())) {
			AAllyMember* ally = Cast<AAllyMember>(hitActor);
			if (ally != nullptr) {
				FPointDamageEvent AllyDmgEvent;
				AllyDmgEvent.Damage = ally->GetDamageRate(hitActor);
				AllyDmgEvent.DamageTypeClass = AllyDamageType;
				ally->TakeDamage(AllyDmgEvent.Damage, AllyDmgEvent, this->GetController(), this);
			}
		}
	}
}

void AAllyMember::AllyMoveForward(float Val) {
	this->MoveForward(Val);
}

void AAllyMember::AllyMoveRight(float Val) {
	this->MoveRight(Val);
}

FHitResult AAllyMember::PerformRaycastHit(FVector WorldLocation, FVector WorldDirection, bool& isSuccessful) {
	FVector startPos = WorldLocation;
	FVector endPos = WorldLocation + (WorldDirection * 10000);
	FCollisionQueryParams traceparams;
	traceparams.AddIgnoredActor(Cast<AActor>(this));
	FCollisionResponseParams responseparams;
	FHitResult RaycastHitResult;
	bool hasHitSomething = GetWorld()->LineTraceSingleByChannel(RaycastHitResult, startPos, endPos, ECollisionChannel::ECC_Visibility, traceparams, responseparams);
	if (hasHitSomething) {
		isSuccessful = true;
		return RaycastHitResult;
	}
	else {
		isSuccessful = false;
		return RaycastHitResult;
	}
}

void AAllyMember::StartShootingBehavior() {
	if (ExecutingShootingBehavior)
		return;

	ExecutingShootingBehavior = true;
	float shootingLowRange = 1.0f;
	float shootingHighRange = 2.0f;
	DefaultShootDelay = FMath::FRandRange(shootingLowRange, shootingHighRange);
	GetWorldTimerManager().SetTimer(defaultShootTimer, this, &AAllyMember::AllyFire, DefaultShootDelay,true);
}

void AAllyMember::StopShootingBehavior(){
	if (!ExecutingShootingBehavior)
		return;

	ExecutingShootingBehavior = false;
	GetWorldTimerManager().ClearTimer(defaultShootTimer);
	
}

//AllyAttributes

bool AAllyMember::isAlive() const { return AllyHealth > 0 ? true : false; }
bool AAllyMember::CanFire() const { return AllyCanFire; }
float AAllyMember::getAllyHealth() const { return AllyHealth; }
float AAllyMember::getAllyMaxHealth() const { return AllyMaxHealth; }
int32 AAllyMember::getCurrentAmmo() const { return CurrentAmmo; }
int32 AAllyMember::GetMaxAmmo() const { return MaxAmmo; }
bool AAllyMember::isExeShootBehavior() const { return ExecutingShootingBehavior; }
//Easier Replacement Add Health and Ammo Functions
void AAllyMember::addAllyHealth(float addToHealth) {
	if (AllyHealth + addToHealth <= 0) {
		AllyHealth = 0;
	}
	else if(AllyHealth + addToHealth >= getAllyMaxHealth()) {
		AllyHealth = AllyMaxHealth;
	}
	else {
		AllyHealth += addToHealth;
	}
}

void AAllyMember::addAllyCurrentAmmo(int32 addToCurrentAmmo) {
	int32 initialResult = CurrentAmmo + addToCurrentAmmo;
	if (initialResult <= 0) {
		CurrentAmmo = 0;
	}
	else if (initialResult >= GetMaxAmmo()) {
		CurrentAmmo = MaxAmmo;
	}
	else {
		CurrentAmmo = initialResult;
	}
}
//End

bool AAllyMember::isEnemyFor(AAllyMember* player) {
	if (this->getAllyFaction().operator==(player->getAllyFaction().GetValue())) {
		return false;
	}
	else {
		return true;
	}
}

bool AAllyMember::wantsFreeMovement() const { return wantsFreedomToMove; }

void AAllyMember::toggleFreeMovement() {
	bool shouldFree = !wantsFreedomToMove;
	wantsFreedomToMove = shouldFree;
	ARTSControllerCPP* myController = Cast<ARTSControllerCPP>(this->GetController());
	if (myController) {
		myController->setWantsToMove(shouldFree);
	}
}

TEnumAsByte<EFactions> AAllyMember::getAllyFaction() {
	if (gamemode != nullptr) {
		return gamemode->getAllyFaction(Cast<AAllyMember>(this));
	}
	return EFactions::Faction_Default;
}

APartyManagerCPP* AAllyMember::getPartyManager() {
	if (gamemode != nullptr) {
		return gamemode->getPartyManager(Cast<AAllyMember>(this));
	}
	return nullptr;
}

int32 AAllyMember::GetFactionPlayerCount() {
	if (gamemode != nullptr) {
		return gamemode->GetAllyFactionPlayerCount(Cast<AAllyMember>(this));
	}
	return 0;
}

int32 AAllyMember::GetGeneralPlayerCount() {
	if (gamemode != nullptr) {
		return gamemode->GetAllyGeneralPlayerCount(Cast<AAllyMember>(this));
	}
	return 0;
}

//Kills and Points
//Getters
int32 AAllyMember::GetKillCount() { return Kills; }
int32 AAllyMember::GetPointsScored() { return Points; }
int32 AAllyMember::GetDeathCount() { return Deaths; }
//Setters
void AAllyMember::AddToKillCount(int32 killNum) { Kills + killNum > 0 ? Kills += killNum : Kills = 0; }
void AAllyMember::AddToPointsScored(int32 scoreNum) { Points + scoreNum > 0 ? Points += scoreNum : Points = 0; }
void AAllyMember::AddToDeathCount(int32 deathNum) { Deaths + deathNum > 0 ? Deaths += deathNum : Deaths = 0; }
//Reset The Stats
void AAllyMember::ResetAllyStats() {
	Kills = 0;
	Points = 0;
	Deaths = 0;
}