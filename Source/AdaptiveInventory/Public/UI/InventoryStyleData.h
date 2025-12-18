// InventoryStyleData.h
// Data Asset for inventory UI style presets
// Designers can create presets in Content Browser and assign to widgets

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UI/InventoryStyleTypes.h"
#include "InventoryStyleData.generated.h"

/**
 * Data Asset containing inventory UI style configuration
 * 
 * Usage:
 * 1. Right-click Content Browser → Miscellaneous → Data Asset
 * 2. Select InventoryStyleData
 * 3. Configure colors, dimensions, etc.
 * 4. Assign to inventory widgets via their StyleData property
 * 
 * Create multiple presets for different themes (Dark, Light, Colorblind, etc.)
 * 
 * Default Color Palette (hex values):
 * - Background:     #0a0e1a → #131824 (gradient)
 * - Panel:          #1e2738 @ 40% opacity
 * - Border:         #2a3447
 * - Accent:         #4facfe (cyan blue)
 * - Accent Alt:     #00f2fe (teal)
 * - Highlight:      #ffd700 (gold)
 * - Text Primary:   #ffffff
 * - Text Secondary: #8b95a5
 * - Text Muted:     #6b7785
 * 
 * Rarity Colors:
 * - Common:     #2a3447 (gray)
 * - Uncommon:   #4ade80 (green)
 * - Rare:       #4facfe (blue)
 * - Epic:       #a855f7 (purple)
 * - Legendary:  #ff8c00 (orange)
 */

UCLASS(BlueprintType)
class ADAPTIVEINVENTORY_API UInventoryStyleData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	/** Display name for this style preset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FString StyleName = TEXT("Default");

	/** Complete style configuration */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FInventoryUIStyle Style;

	// ----------------------------------------
	// Quick Access Helpers
	// ----------------------------------------

	/** Get slot style configuration */
	UFUNCTION(BlueprintCallable, Category = "Style")
	const FInventorySlotStyle& GetSlotStyle() const { return Style.SlotStyle; }
	
	/** Get grid style configuration */
	UFUNCTION(BlueprintCallable, Category = "Style")
	const FInventoryGridStyle& GetGridStyle() const { return Style.GridStyle; }
	
	/** Get detail panel style configuration */
	UFUNCTION(BlueprintCallable, Category = "Style")
	const FInventoryDetailStyle& GetDetailStyle() const { return Style.DetailStyle; }
	
	/** Get quick slot style configuration */
	UFUNCTION(BlueprintCallable, Category = "Style")
	const FInventoryQuickSlotStyle& GetQuickSlotStyle() const { return Style.QuickSlotStyle; }

	/** Get rarity colors for specific rarity */
	UFUNCTION(BlueprintCallable, Category = "Style")
	FRarityColorSet GetRarityColors(EItemRarity Rarity) const
	{
		return Style.SlotStyle.GetRarityColors(Rarity);
	}

	/** Get primary rarity color (border/accent) */
	UFUNCTION(BlueprintCallable, Category = "Style")
	FLinearColor GetRarityPrimaryColor(EItemRarity Rarity) const
	{
		return Style.SlotStyle.GetRarityColors(Rarity).Primary;
	}
	
	/** Get rarity background tint */
	UFUNCTION(BlueprintCallable, Category = "Style")
	FLinearColor GetRarityBackgroundTint(EItemRarity Rarity) const
	{
		return Style.SlotStyle.GetRarityColors(Rarity).BackgroundTint;
	}
};