// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "PlatformTrigger.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
    ConstructorHelpers::FClassFinder<UUserWidget> MenuSystemWBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));

    if (MenuSystemWBPClass.Class != nullptr)
    {
        MenuClass = MenuSystemWBPClass.Class;
    }    
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
    if (MenuClass == nullptr) return;
    
    UUserWidget* Menu = CreateWidget<UUserWidget>(this, MenuClass);
    if (Menu == nullptr) return;
       
    Menu->AddToViewport();
    
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if(PlayerController == nullptr) return;
    
    TSharedRef<SWidget> MenuInput = Menu->TakeWidget();

    FInputModeUIOnly InputModeData;
    InputModeData.SetWidgetToFocus(MenuInput);
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = true;

}


void UPuzzlePlatformsGameInstance::Init()
{
    UE_LOG(LogTemp, Warning, TEXT("Init() Log Message"));
    UE_LOG(LogTemp, Warning, TEXT("Found Class: %s"), *MenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::Host()
{
    UEngine* Engine = GetEngine();

    if(Engine != nullptr)
    {
        Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));
    }

    UWorld* World = GetWorld();
    if (World != nullptr)
    {
        World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
    }
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
    UEngine* Engine = GetEngine();

    if(Engine != nullptr)
    {
        Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));
    }

    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if(PlayerController != nullptr)
    {
        PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
    }
}
   

