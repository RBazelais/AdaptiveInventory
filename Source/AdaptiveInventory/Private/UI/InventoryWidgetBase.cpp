// InventoryWidgetBase.cpp
// Base class implementation for inventory widgets

#include "UI/InventoryWidgetBase.h"
#include "Core/InventoryManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

UInventoryWidgetBase::UInventoryWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Defaults
	bAutoRefresh = true;
	bBindEventsOnConstruct = true;
}

void UInventoryWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Cache the subsystem reference
	CacheInventoryManager();
	
	// Bind to inventory events if configured
	if (bBindEventsOnConstruct)
	{
		BindInventoryEvents();
	}
	
	// Initial refresh
	RefreshWidget();
}

void UInventoryWidgetBase::NativeDestruct()
{
	// Always unbind events to prevent dangling references/delegates
	UnbindInventoryEvents();
	Super::NativeDestruct();
}

void UInventoryWidgetBase::CacheInventoryManager()
{
	if (!CachedInventoryManager)
	{
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
		if (GameInstance)
		{
			CachedInventoryManager = GameInstance->GetSubsystem<UInventoryManagerSubsystem>();
		}
	}
	
	if (!CachedInventoryManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidgetBase: Failed to cache InventoryManagerSubsystem"));
	}
}

UInventoryManagerSubsystem* UInventoryWidgetBase::GetInventoryManager() const
{
	return CachedInventoryManager;
}

void UInventoryWidgetBase::RefreshWidget_Implementation()
{
	// Default implementation does nothing
	// Child classes should override to update their specific visuals
}

void UInventoryWidgetBase::BindInventoryEvents_Implementation()
{
	if (bEventsBound)
	{
		return; // Already bound
	}

	UInventoryManagerSubsystem* InventoryManager = GetInventoryManager();
	if (!InventoryManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidgetBase: Cannot bind events - InventoryManagerSubsystem is null"));
		return;
	}
	
	// Bind to Inventory Events
	InventoryManager->OnInventoryChanged.AddDynamic(this, &UInventoryWidgetBase::OnInventoryChanged);
	InventoryManager->OnItemAdded.AddDynamic(this, &UInventoryWidgetBase::OnItemAdded);
	InventoryManager->OnItemRemoved.AddDynamic(this, &UInventoryWidgetBase::OnItemRemoved);
	InventoryManager->OnItemStackChanged.AddDynamic(this, &UInventoryWidgetBase::OnItemStackChanged);

	bEventsBound = true;

	UE_LOG(LogTemp, Log, TEXT("InventoryWidgetBase: Events bound successfully"));
}

void UInventoryWidgetBase::UnbindInventoryEvents_Implementation()
{
	if (!bEventsBound)
	{
		return; // Not bound
	}
	UInventoryManagerSubsystem* InventoryManager = GetInventoryManager();
	if (InventoryManager)
	{
		// Unbind from inventory events
		InventoryManager->OnInventoryChanged.RemoveDynamic(this, &UInventoryWidgetBase::OnInventoryChanged);
		InventoryManager->OnItemAdded.RemoveDynamic(this, &UInventoryWidgetBase::OnItemAdded);
		InventoryManager->OnItemRemoved.RemoveDynamic(this, &UInventoryWidgetBase::OnItemRemoved);
		InventoryManager->OnItemStackChanged.RemoveDynamic(this, &UInventoryWidgetBase::OnItemStackChanged);
	}
	
	bEventsBound = false;
	
	UE_LOG(LogTemp, Log, TEXT("InventoryWidgetBase: Events unbound successfully"));
}

void UInventoryWidgetBase::OnInventoryChanged_Implementation()
{
	if (bAutoRefresh)
	{
		RefreshWidget();
	}
}

void UInventoryWidgetBase::OnItemAdded_Implementation(UInventoryItemData* Item)
{
	// Base implementation delegates to general refresh
	// Child classes can override for specific add animations/behavior
}

void UInventoryWidgetBase::OnItemRemoved_Implementation(FGuid ItemGUID)
{
	// Base implementation delegates to general refresh
	// Child classes can override for specific remove animations/behavior
}

void UInventoryWidgetBase::OnItemStackChanged_Implementation(FGuid ItemGUID, int32 NewStackSize)
{
	// Base implementation delegates to general refresh
	// Child classes can override for specific stack update behavior
}
