// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItemData.generated.h"

/**
 *  Item Rarity
 */

UENUM (BlueprintType)
enum class EItemRarity : uint8
{
    Common      UMETA(DisplayName = "Common"),
	Uncommon    UMETA(DisplayName = "Uncommon"),
	Rare        UMETA(DisplayName = "Rare"),
	Epic        UMETA(DisplayName = "Epic"),
	Legendary   UMETA(DisplayName = "Legendary")
};

/**
 * Item categories for filtering
 */
UENUM (BlueprintType)
enum class EItemCategory : uint8
{
	Weapon      UMETA(DisplayName = "Weapon"),
	Consumable  UMETA(DisplayName = "Consumable"),
	Material    UMETA(DisplayName = "Material"),
	Equipment   UMETA(DisplayName = "Equipment"),
	Quest       UMETA(DisplayName = "Quest")
};

/**
 * Data class encapsulating all information about an inventory item
 * Follows Epic's UMG Best Practices - separates data from UI
 */
UCLASS(BlueprintType, Blueprintable)
class ADAPTIVEINVENTORY_API UInventoryItemData : public UObject
{
    GENERATED_BODY()

public:
    UInventoryItemData();

    // Basic Item Info
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
    FText ItemDescription;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
    UTexture2D* ItemIcon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
    EItemRarity ItemRarity;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
    EItemCategory ItemCategory;

    // Stack Info
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
    int32 CurrentStackSize;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
    int32 MaxStackSize;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
    bool bIsStackable;

    // Stats (for weapons/equipment)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Stats")
    float MinDamage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Stats")
    float MaxDamage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Stats")
    float AttackSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Stats")
    float CurrentDurability;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Stats")
    float MaxDurability;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Stats")
    float Weight;

    // Unique ID for this item instance
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Info")
    FGuid ItemGUID;

    // Public API Functions
    UFUNCTION(BlueprintCallable, Category = "Item Data")
    FText GetItemName() const { return ItemName; }

    UFUNCTION(BlueprintCallable, Category = "Item Data")
    FText GetItemDescription() const { return ItemDescription; }

    UFUNCTION(BlueprintCallable, Category = "Item Data")
    UTexture2D* GetItemIcon() const { return ItemIcon; }

    UFUNCTION(BlueprintCallable, Category = "Item Data")
    EItemRarity GetItemRarity() const { return ItemRarity; }

    UFUNCTION(BlueprintCallable, Category = "Item Data")
    EItemCategory GetItemCategory() const { return ItemCategory; }

    UFUNCTION(BlueprintCallable, Category = "Item Data")
    int32 GetCurrentStackSize() const { return CurrentStackSize; }

    UFUNCTION(BlueprintCallable, Category = "Item Data")
    int32 GetMaxStackSize() const { return MaxStackSize; }

    UFUNCTION(BlueprintCallable, Category = "Item Data")
    bool CanStack() const { return bIsStackable; }

    UFUNCTION(BlueprintCallable, Category = "Item Data")
    FGuid GetItemGUID() const { return ItemGUID; }

    // Stack management
    UFUNCTION(BlueprintCallable, Category = "Item Data")
    bool AddToStack(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Item Data")
    bool RemoveFromStack(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Item Data")
    bool IsStackFull() const;

    /**
     * Set the current stack size directly (clamped to valid range)
     * @param NewSize - The new stack size
     */
    UFUNCTION(BlueprintCallable, Category = "Item Data")
    void SetStackSize(int32 NewSize);

    // Initialization
    virtual void PostInitProperties() override;
};