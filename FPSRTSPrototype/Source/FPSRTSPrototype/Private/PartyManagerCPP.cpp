// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSRTSPrototype.h"
#include "PartyManagerCPP.h"
#include "RTSControllerCPP.h"
#include "FPSRTSPrototypeCharacter.h"
#include "AllyMember.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


// Sets default values
APartyManagerCPP::APartyManagerCPP()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isInOverview = false;
}

// Called when the game starts or when spawned
void APartyManagerCPP::BeginPlay()
{
	Super::BeginPlay();
	gamemode = Cast<ARTSProtoGameMode>(UGameplayStatics::GetGameMode(this));
	if (gamemode == nullptr) {
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Blue, "RTS GameMode does not exist!");
	}
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
	AAllyMember* firstAlly = FindPartyMembers(false, nullptr);
	SetAllyInCommand(firstAlly);
}

// Called every frame
void APartyManagerCPP::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void APartyManagerCPP::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAxis("MoveForward", this, &APartyManagerCPP::AllyCommandMoveForward);
	InputComponent->BindAxis("MoveRight", this, &APartyManagerCPP::AllyCommandMoveRight);
	InputComponent->BindAction("Jump", IE_Pressed, this, &APartyManagerCPP::AllyCommandJump);
	InputComponent->BindAction("Jump", IE_Released, this, &APartyManagerCPP::AllyCommandStopJumping);
	InputComponent->BindAction("Fire", IE_Pressed, this, &APartyManagerCPP::AllyCommandFire);
	InputComponent->BindAction("PossessAllyAdd", IE_Pressed, this, &APartyManagerCPP::PossessAllyAdd);
	InputComponent->BindAction("PossessAllySubtract", IE_Pressed, this, &APartyManagerCPP::PossessAllySubtract);
	InputComponent->BindAction("Zoom", IE_Pressed, this, &APartyManagerCPP::CheckSelectionRaycast);
	InputComponent->BindAction("ToggleFreeMove", IE_Pressed, this, &APartyManagerCPP::AllyCommandToggleFreeMovement);
	
}

AAllyMember* APartyManagerCPP::FindPartyMembers(bool pendingAllyLeave, AAllyMember* allyToLeave) {
	PartyMembers.Empty();
	PartyActorArray.Empty();
	UGameplayStatics::GetAllActorsOfClass(this, AAllyMember::StaticClass(), PartyActorArray);
	for (int i = 0; i < PartyActorArray.Num(); i++) {
		if (pendingAllyLeave) {
			if (PartyActorArray[i] != allyToLeave) {
				AAllyMember* Ally = Cast<AAllyMember>(PartyActorArray[i]);
				if (Ally->generalCommander == this->generalCommander) {
					PartyMembers.Add(Ally);
				}
			}
		}
		else {
			AAllyMember* AllyMem = Cast<AAllyMember>(PartyActorArray[i]);
			if (AllyMem->generalCommander == this->generalCommander){
				PartyMembers.Add(AllyMem);
			}
		}
	
	}
	if (PartyMembers.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("No partyMembers in Scene"));
		return nullptr;
	}
	else {
		AAllyMember* FirstAllyFound = PartyMembers[0];
		return FirstAllyFound;
	}
}

void  APartyManagerCPP::SetAllyInCommand(AAllyMember* setToCommand) {
	AAllyMember* isAnAlly = Cast<AAllyMember>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (isAnAlly == nullptr) {
		int32 commandIndex = PartyMembers.Find(setToCommand);
		if (PartyMembers.IsValidIndex(commandIndex)) {
			UCameraComponent* allyCamera = setToCommand->GetFirstPersonCameraComponent();
			FTransform cameraTransform = allyCamera->GetRelativeTransform();
			FVector cameraLocation = cameraTransform.GetLocation();
			FQuat cameraRotation = cameraTransform.GetRotation();
			FVector locationAndOffset = FollowAllyCameraOffset + cameraLocation;
			AttachRootComponentTo(setToCommand->GetRootComponent(), NAME_None, EAttachLocation::KeepRelativeOffset, true);
			this->SetActorRelativeLocation(locationAndOffset);
			this->SetActorRelativeRotation(cameraRotation);

			allyInCommand = setToCommand;
			allyInCommandActor = Cast<AActor>(setToCommand);
			if(gamemode){
			if (this->generalCommander.operator==(ECommanders::Commander_01)) {
				UGameplayStatics::GetPlayerController(GetWorld(), 0)->bAutoManageActiveCameraTarget = false;
				UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTarget(allyInCommandActor);
				}
				gamemode->UpdateSpecStats();
			}
		}
	}
}
	//Input Commands
	void APartyManagerCPP::AllyCommandJump() {
		if (allyInCommand) {
			allyInCommand->Jump();
		}
	}
	void APartyManagerCPP::AllyCommandStopJumping() {
		if (allyInCommand) {
			allyInCommand->StopJumping();
		}
	}
	void APartyManagerCPP::AllyCommandFire() {
		if (allyInCommand) {
			allyInCommand->AllyFire();
		}
	}
	void APartyManagerCPP::CheckSelectionRaycast() {
		FHitResult RaycastHitResult;
		bool hasHitSomething;
		if (allyInCommand != nullptr) {
			if (!isInOverview) {
				FVector worldLocation;
				FVector worldDirection;
				UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectMousePositionToWorld(worldLocation, worldDirection);
				RaycastHitResult = allyInCommand->PerformRaycastHit(worldLocation, worldDirection, hasHitSomething);
			}
			else {
			//	hasHitSomething = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), true, RaycastHitResult);
			}
			if (hasHitSomething) {
				ARTSControllerCPP* allyController = Cast<ARTSControllerCPP>(allyInCommand->GetController());
				if (allyController != nullptr) {
					//I hit an allyMember object, check if it is my friend or enemy
					if (RaycastHitResult.Actor->IsA(AAllyMember::StaticClass())) {
						AActor* HitActor = RaycastHitResult.Actor.Get(false);
						AAllyMember* HitAlly = Cast<AAllyMember>(HitActor);
						//If I hit my own clan member, a friend and not an enemy.
						ECommanders allyCommander = allyInCommand->generalCommander.GetValue();
						if (HitAlly->generalCommander.operator==(allyCommander)) {
							allyController->BlackBoardComp->SetValueAsBool(allyController->isTargetingEnemy, false);
							allyInCommand->isTargetingEnemy = false;
						}
						else if (!(HitAlly->generalCommander.operator==(allyCommander))) {
							//If I hit an enemy clan member and not a friend.
							//Get AI Controller from allyincommand, get the blackboard
							//set isTargettingEnemy to true and targettedEnemy to HitActor
							//then set these local variables inside of allyOfCommand
							bool isTargeting = allyController->BlackBoardComp->GetValueAsBool(allyController->isTargetingEnemy);
							AActor* enemyActorHit = RaycastHitResult.Actor.Get(false);
							if (!isTargeting) {
								allyController->BlackBoardComp->SetValueAsObject(allyController->targetedEnemy, enemyActorHit);
								allyController->BlackBoardComp->SetValueAsBool(allyController->isTargetingEnemy, true);
								allyInCommand->isTargetingEnemy = true;
								allyInCommand->targetedEnemy = Cast<AAllyMember>(enemyActorHit);
							}
							else {
								allyController->BlackBoardComp->SetValueAsObject(allyController->targetedEnemy, enemyActorHit);
								allyInCommand->targetedEnemy = Cast<AAllyMember>(enemyActorHit);
							}
						}
						else {
							//TESTING PURPOSES ONLY!!
						}
					}
					else {
						//This happens if I did not hit any allymember, cover, or other important object.
						if (FNavigationSystem::IsValidLocation(RaycastHitResult.Location)) {
							allyController->BlackBoardComp->SetValueAsBool(allyController->isTargetingEnemy, false);
							allyInCommand->isTargetingEnemy = false;
							bool moveIsActive = allyController->BlackBoardComp->GetValueAsBool(allyController->moveCommandActive);
							if (moveIsActive) {
								allyController->BlackBoardComp->SetValueAsBool(allyController->moveCommandActive, false);
								allyController->BlackBoardComp->SetValueAsVector(allyController->moveCommandLocation, RaycastHitResult.Location);
								allyController->BlackBoardComp->SetValueAsBool(allyController->moveCommandActive, true);
							}
							else {
								allyController->BlackBoardComp->SetValueAsVector(allyController->moveCommandLocation, RaycastHitResult.Location);
								allyController->BlackBoardComp->SetValueAsBool(allyController->moveCommandActive, true);
							}
						}
					}
				}
			}
		}
	}

	void APartyManagerCPP::AllyCommandMoveForward(float Value) {
		if (Value != 0.0f)
		{
			if (allyInCommand) {
				allyInCommand->AllyMoveForward(Value);
			}
		}
	}
	void APartyManagerCPP::AllyCommandMoveRight(float Value) {
		if (Value != 0.0f)
		{
			if (allyInCommand) {
				allyInCommand->AllyMoveRight(Value);
			}
		}
	}

	void APartyManagerCPP::AllyCommandToggleFreeMovement() {
		if (allyInCommand) {
			allyInCommand->toggleFreeMovement();
		}
	}

	void APartyManagerCPP::PossessAllyAdd() {
		AAllyMember* isAnAlly = Cast<AAllyMember>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (isAnAlly == nullptr) {
			if (allyInCommand && PartyMembers.Num() > 0) {
				int32 commandIndex = PartyMembers.Find(allyInCommand);
				//Unreal Crashes when I increment int32 by ++ instead of +1
				//Unreal Crashes when I check array for nullpointers
				//Use IsValid Method Instead
				if (PartyMembers.IsValidIndex(commandIndex + 1)) {
					SetAllyInCommand(PartyMembers[commandIndex + 1]);
				}
				else if (PartyMembers.IsValidIndex(0)) {
					SetAllyInCommand(PartyMembers[0]);
				}
			}
		}
		
	}
	void APartyManagerCPP::PossessAllySubtract() {
		AAllyMember* isAnAlly = Cast<AAllyMember>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (isAnAlly == nullptr) {
			if (allyInCommand && PartyMembers.Num() > 0) {
				int32 commandIndex = PartyMembers.Find(allyInCommand);
				int32 endIndex = PartyMembers.Find(PartyMembers[PartyMembers.Num() - 1]);
				//Unreal Crashes when I increment int32 by ++ instead of +1
				//Unreal Crashes when I check array for nullpointers
				//Use IsValid Method Instead
				if (PartyMembers.IsValidIndex(commandIndex - 1)) {
					SetAllyInCommand(PartyMembers[commandIndex - 1]);
				}
				else if (PartyMembers.IsValidIndex(endIndex)) {
					SetAllyInCommand(PartyMembers[endIndex]);
				}
			}
		}
	}
//Kills and Points Getters
	int32 APartyManagerCPP::GetPartyKillCount() {
		int32 partyKillCount = 0;
		for (int32 i = 0; i < PartyMembers.Num(); i++) {
			partyKillCount += PartyMembers[i]->GetKillCount();
		}
		return partyKillCount;
	}
	int32 APartyManagerCPP::GetPartyPointsScored() {
		int32 partyPointsScored = 0;
		for (int32 i = 0; i < PartyMembers.Num(); i++) {
			partyPointsScored += PartyMembers[i]->GetPointsScored();
		}
		return partyPointsScored;
	}
	int32 APartyManagerCPP::GetPartyDeathCount() {
		int32 partyDeathCount = 0;
		for (int32 i = 0; i < PartyMembers.Num(); i++) {
			partyDeathCount += PartyMembers[i]->GetDeathCount();
		}
		return partyDeathCount;
	}