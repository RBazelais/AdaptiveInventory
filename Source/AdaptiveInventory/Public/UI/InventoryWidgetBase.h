// InventoryWidgetBase.h
// Base class for all inventory UI widgets
// Provides common functionality: subsystem access, event binding, lifecycle management

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidgetBase.generated.h"

// Forward declarations
class UInventoryManagerSubsystem;
class UInventoryItemData;

/**
 * Base class for all inventory-related widgets
 * Handles subsystem caching, event binding/unbinding, and common refresh patterns
 * 
 * Inherit from this for: InventoryGrid, ItemSlot, DetailPanel, etc.
 */
UCLASS(Abstract, Blueprintable)
class ADAPTIVEINVENTORY_API UInventoryWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInventoryWidgetBase(const FObjectInitializer& ObjectInitializer);
	
	// Begin UUserWidget Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// End UUserWidget Interface
	
	/**
	 * Refresh the widget display
	 * Override in child classes to update visuals
	 * Called automatically when inventory changes (if bAutoRefresh is true)
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Widget")
	void RefreshWidget();
	virtual void RefreshWidget_Implementation();
	
protected:
	// SUBSYSTEM ACCESS
	
	/**
	 * Get the cached inventory manager subsystem
	 * @return The inventory subsystem, or nullptr if not available
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory Widget")
	UInventoryManagerSubsystem* GetInventoryManager() const;
	
	// CONFIGURATION
	
	/** Whether to automatically refresh when inventory changes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Widget|Config")
	bool bAutoRefresh = true;
	
	/** Whether to bind to inventory events on construct */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Widget|Config")
	bool bBindEventsOnConstruct = true;

	// EVENT BINDING
	
	/**
	 * Bind to all inventory events
	 * Override to add custom event bindings
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory Widget")
	void BindInventoryEvents();
	virtual void BindInventoryEvents_Implementation();
	
	/**
	 * Unbind from all inventory events
	 * Override to remove custom event bindings
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory Widget")
	void UnbindInventoryEvents();
	virtual void UnbindInventoryEvents_Implementation();
	
	// EVENT HANDLERS - Override in child classes
	
	/** Called when any inventory change occurs */
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory Widget|Events")
	void OnInventoryChanged();
	virtual void OnInventoryChanged_Implementation();
	
	/** Called when an item is added */
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory Widget|Events")
	void OnItemAdded(UInventoryItemData* Item);
	virtual void OnItemAdded_Implementation(UInventoryItemData* Item);  // Match parameter name
	
	/** Called when an item is removed */
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory Widget|Events")
	void OnItemRemoved(FGuid ItemGUID);
	virtual void OnItemRemoved_Implementation(FGuid ItemGUID);
	
	/** Called when an item's stack size changes */
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory Widget|Events")
	void OnItemStackChanged(FGuid ItemGUID, int32 NewStackSize);
	virtual void OnItemStackChanged_Implementation(FGuid ItemGUID, int32 NewStackSize);
	
private:
	/** Cached reference to the inventory manager subsystem */
	UPROPERTY()
	TObjectPtr<UInventoryManagerSubsystem> CachedInventoryManager;
	
	/** Are events currently bound */
	bool bEventsBound = false;
	
	/** Cache the subsystem references */
	void CacheInventoryManager();
};