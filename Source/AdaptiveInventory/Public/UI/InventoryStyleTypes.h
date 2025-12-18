// InventoryStyleTypes.h
// Centralized style configuration for inventory UI
// Colors extracted from Adaptive Inventory mockup

#pragma once

#include "CoreMinimal.h"
#include "Core/InventoryItemData.h"
#include "InventoryStyleTypes.generated.h"

/** Rarity-specific color configuration */
USTRUCT(BlueprintType)
struct FRarityColorSet
{
	GENERATED_BODY()

	/** Main rarity color (borders, accents) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FLinearColor Primary = FLinearColor::White;

	/** Glow/shadow color for hover effects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FLinearColor Glow = FLinearColor(1.0f, 1.0f, 1.0f, 0.4f);

	/** Background tint when slot contains item of this rarity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FLinearColor BackgroundTint = FLinearColor(1.0f, 1.0f, 1.0f, 0.1f);
};

/**
 * Slot widget visual configuration
 */
USTRUCT(BlueprintType)
struct FInventorySlotStyle
{
	GENERATED_BODY()

	// ----------------------------------------
	// Dimensions
	// ----------------------------------------

	/** Slot size in pixels */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
	float SlotSize = 80.0f;

	/** Space between slots */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
	float SlotPadding = 12.0f;

	/** Border thickness */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
	float BorderWidth = 2.0f;

	/** Corner rounding radius */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
	float CornerRadius = 10.0f;

	/** Height of rarity bar at bottom */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
	float RarityBarHeight = 4.0f;

	// ----------------------------------------
	// Colors - Base State
	// ----------------------------------------

	/** Empty slot background - #1a1f2e with 60% opacity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Base")
	FLinearColor EmptyBackground = FLinearColor(0.102f, 0.122f, 0.180f, 0.6f);

	/** Empty slot border - #2a3447 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Base")
	FLinearColor EmptyBorder = FLinearColor(0.165f, 0.204f, 0.278f, 1.0f);

	/** Empty slot dashed indicator - #2a3447 with 40% opacity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Base")
	FLinearColor EmptyIndicator = FLinearColor(0.165f, 0.204f, 0.278f, 0.4f);

	// ----------------------------------------
	// Colors - Interactive States
	// ----------------------------------------

	/** Selection border color - #ffffff */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|States")
	FLinearColor SelectedBorder = FLinearColor::White;

	/** Selection glow color - #ffffff with 50% opacity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|States")
	FLinearColor SelectedGlow = FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);

	/** Hover scale multiplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|States")
	float HoverScale = 1.05f;

	/** Hover Y offset (translateY effect) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|States")
	float HoverOffsetY = -4.0f;

	// ----------------------------------------
	// Colors - Stack Count
	// ----------------------------------------

	/** Stack count background - #000000 with 60% opacity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|StackCount")
	FLinearColor StackCountBackground = FLinearColor(0.0f, 0.0f, 0.0f, 0.6f);

	/** Stack count text (normal) - #ffffff */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|StackCount")
	FLinearColor StackCountText = FLinearColor::White;

	/** Stack count text (full stack) - #4ade80 (Uncommon green) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|StackCount")
	FLinearColor StackCountFullText = FLinearColor(0.290f, 0.871f, 0.502f, 1.0f);

	// ----------------------------------------
	// Rarity Colors
	// ----------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Rarity")
	FRarityColorSet CommonColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Rarity")
	FRarityColorSet UncommonColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Rarity")
	FRarityColorSet RareColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Rarity")
	FRarityColorSet EpicColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Rarity")
	FRarityColorSet LegendaryColors;

	// ----------------------------------------
	// Constructor with mockup defaults
	// ----------------------------------------

	FInventorySlotStyle()
	{
		// Common - #2a3447 (gray, border only, no background tint)
		CommonColors.Primary = FLinearColor(0.165f, 0.204f, 0.278f, 1.0f);
		CommonColors.Glow = FLinearColor(0.165f, 0.204f, 0.278f, 0.4f);
		CommonColors.BackgroundTint = FLinearColor(0.165f, 0.204f, 0.278f, 0.0f);

		// Uncommon - #4ade80 (green)
		UncommonColors.Primary = FLinearColor(0.290f, 0.871f, 0.502f, 1.0f);
		UncommonColors.Glow = FLinearColor(0.290f, 0.871f, 0.502f, 0.4f);
		UncommonColors.BackgroundTint = FLinearColor(0.290f, 0.871f, 0.502f, 0.1f);

		// Rare - #4facfe (blue)
		RareColors.Primary = FLinearColor(0.310f, 0.675f, 0.996f, 1.0f);
		RareColors.Glow = FLinearColor(0.310f, 0.675f, 0.996f, 0.4f);
		RareColors.BackgroundTint = FLinearColor(0.310f, 0.675f, 0.996f, 0.1f);

		// Epic - #a855f7 (purple)
		EpicColors.Primary = FLinearColor(0.659f, 0.333f, 0.969f, 1.0f);
		EpicColors.Glow = FLinearColor(0.659f, 0.333f, 0.969f, 0.4f);
		EpicColors.BackgroundTint = FLinearColor(0.659f, 0.333f, 0.969f, 0.1f);

		// Legendary - #ff8c00 (orange)
		LegendaryColors.Primary = FLinearColor(1.0f, 0.549f, 0.0f, 1.0f);
		LegendaryColors.Glow = FLinearColor(1.0f, 0.549f, 0.0f, 0.4f);
		LegendaryColors.BackgroundTint = FLinearColor(1.0f, 0.549f, 0.0f, 0.1f);
	}

	/** Get colors for a specific rarity */
	const FRarityColorSet& GetRarityColors(EItemRarity Rarity) const
	{
		switch (Rarity)
		{
		case EItemRarity::Common:     return CommonColors;
		case EItemRarity::Uncommon:   return UncommonColors;
		case EItemRarity::Rare:       return RareColors;
		case EItemRarity::Epic:       return EpicColors;
		case EItemRarity::Legendary:  return LegendaryColors;
		default:                      return CommonColors;
		}
	}
};

/** Grid widget visual configuration */
USTRUCT(BlueprintType)
struct FInventoryGridStyle
{
	GENERATED_BODY()

	// ----------------------------------------
	// Layout
	// ----------------------------------------

	/** Number of columns */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	int32 GridColumns = 6;

	/** Total visible slots */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	int32 TotalSlots = 18;

	/** Padding inside the grid container */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	float GridPadding = 20.0f;

	/** Gap between slots */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	float SlotGap = 10.0f;

	// ----------------------------------------
	// Panel Colors
	// ----------------------------------------

	/** Main panel background - #1e2738 with 40% opacity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor PanelBackground = FLinearColor(0.118f, 0.153f, 0.220f, 0.4f);

	/** Panel border color - #2a3447 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor PanelBorder = FLinearColor(0.165f, 0.204f, 0.278f, 1.0f);

	/** Panel corner radius */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	float PanelCornerRadius = 12.0f;

	// ----------------------------------------
	// Background Colors
	// ----------------------------------------

	/** Main background gradient start - #0a0e1a */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Background")
	FLinearColor BackgroundStart = FLinearColor(0.039f, 0.055f, 0.102f, 1.0f);

	/** Main background gradient end - #131824 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Background")
	FLinearColor BackgroundEnd = FLinearColor(0.075f, 0.094f, 0.141f, 1.0f);

	// ----------------------------------------
	// Tab Bar Colors
	// ----------------------------------------

	/** Tab bar background - #000000 with 20% opacity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Tabs")
	FLinearColor TabBarBackground = FLinearColor(0.0f, 0.0f, 0.0f, 0.2f);

	/** Active tab background - #4facfe */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Tabs")
	FLinearColor TabActiveBackground = FLinearColor(0.310f, 0.675f, 0.996f, 1.0f);

	/** Active tab text - #0a0e1a (dark) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Tabs")
	FLinearColor TabActiveText = FLinearColor(0.039f, 0.055f, 0.102f, 1.0f);

	/** Inactive tab border - #2a3447 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Tabs")
	FLinearColor TabInactiveBorder = FLinearColor(0.165f, 0.204f, 0.278f, 1.0f);

	/** Inactive tab text - #6b7785 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Tabs")
	FLinearColor TabInactiveText = FLinearColor(0.420f, 0.467f, 0.522f, 1.0f);

	// ----------------------------------------
	// Header Colors
	// ----------------------------------------

	/** Header title gradient start - #4facfe */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Header")
	FLinearColor HeaderGradientStart = FLinearColor(0.310f, 0.675f, 0.996f, 1.0f);

	/** Header title gradient end - #00f2fe */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Header")
	FLinearColor HeaderGradientEnd = FLinearColor(0.0f, 0.949f, 0.996f, 1.0f);

	/** Badge background - #ffd700 (gold) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Header")
	FLinearColor BadgeBackground = FLinearColor(1.0f, 0.843f, 0.0f, 1.0f);

	/** Badge text - #0a0e1a (dark) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Header")
	FLinearColor BadgeText = FLinearColor(0.039f, 0.055f, 0.102f, 1.0f);

	/** FPS indicator dot - #4ade80 (green) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Header")
	FLinearColor StatusGood = FLinearColor(0.290f, 0.871f, 0.502f, 1.0f);

	// ----------------------------------------
	// Search Box Colors
	// ----------------------------------------

	/** Search box background - #0d1117 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Search")
	FLinearColor SearchBackground = FLinearColor(0.051f, 0.067f, 0.090f, 1.0f);

	/** Search box border - #2a3447 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Search")
	FLinearColor SearchBorder = FLinearColor(0.165f, 0.204f, 0.278f, 1.0f);

	/** Search box focus border - #4facfe */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Search")
	FLinearColor SearchFocusBorder = FLinearColor(0.310f, 0.675f, 0.996f, 1.0f);

	/** Search placeholder text - #6b7785 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors|Search")
	FLinearColor SearchPlaceholder = FLinearColor(0.420f, 0.467f, 0.522f, 1.0f);
};

/** Detail panel visual configuration */
USTRUCT(BlueprintType)
struct FInventoryDetailStyle
{
	GENERATED_BODY()

	// ----------------------------------------
	// Dimensions
	// ----------------------------------------

	/** Panel width */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
	float PanelWidth = 320.0f;

	/** Item preview area size */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
	float PreviewSize = 200.0f;

	/** Preview border width */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
	float PreviewBorderWidth = 3.0f;

	// ----------------------------------------
	// Colors
	// ----------------------------------------

	/** Panel background - #1e2738 with 40% opacity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor PanelBackground = FLinearColor(0.118f, 0.153f, 0.220f, 0.4f);

	/** Panel border - #2a3447 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor PanelBorder = FLinearColor(0.165f, 0.204f, 0.278f, 1.0f);

	/** Section header text - #4facfe */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor SectionHeaderColor = FLinearColor(0.310f, 0.675f, 0.996f, 1.0f);

	/** Item title text - #ffffff */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor ItemTitleColor = FLinearColor::White;

	/** Description text - #8b95a5 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor DescriptionColor = FLinearColor(0.545f, 0.584f, 0.647f, 1.0f);

	/** Stats panel background - #0d1117 with 60% opacity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor StatsBackground = FLinearColor(0.051f, 0.067f, 0.090f, 0.6f);

	/** Stats panel border - #2a3447 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor StatsBorder = FLinearColor(0.165f, 0.204f, 0.278f, 1.0f);

	/** Stat label text - #6b7785 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor StatLabelColor = FLinearColor(0.420f, 0.467f, 0.522f, 1.0f);

	/** Stat value text - #4facfe */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor StatValueColor = FLinearColor(0.310f, 0.675f, 0.996f, 1.0f);

	/** Stat row divider - #1e2738 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor StatDivider = FLinearColor(0.118f, 0.153f, 0.220f, 1.0f);

	/** Empty state text - #ffffff with 30% opacity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor EmptyStateText = FLinearColor(1.0f, 1.0f, 1.0f, 0.3f);
};

/**Quick slot bar visual configuration */
USTRUCT(BlueprintType)
struct FInventoryQuickSlotStyle
{
	GENERATED_BODY()

	// ----------------------------------------
	// Dimensions
	// ----------------------------------------

	/** Individual slot size 80px 80px*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
	float SlotSize = 50.0f;

	/** Gap between slots 10px*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
	float SlotGap = 10.0f;

	/** Bar padding 12px*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
	float BarPadding = 12.0f;

	/** Bar corner radius 12px*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
	float BarCornerRadius = 12.0f;

	// ----------------------------------------
	// Colors
	// ----------------------------------------

	/** Bar background - #0d1117 with 95% opacity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor BarBackground = FLinearColor(0.051f, 0.067f, 0.090f, 0.95f);

	/** Bar border - #2a3447 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor BarBorder = FLinearColor(0.165f, 0.204f, 0.278f, 1.0f);

	/** Slot background - #1a1f2e */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor SlotBackground = FLinearColor(0.102f, 0.122f, 0.180f, 1.0f);

	/** Slot border - #2a3447 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor SlotBorder = FLinearColor(0.165f, 0.204f, 0.278f, 1.0f);

	/** Slot number text - #6b7785 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor SlotNumberColor = FLinearColor(0.420f, 0.467f, 0.522f, 1.0f);

	/** Label background - #ffd700 (gold) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor LabelBackground = FLinearColor(1.0f, 0.843f, 0.0f, 1.0f);

	/** Label text - #0a0e1a (dark) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor LabelText = FLinearColor(0.039f, 0.055f, 0.102f, 1.0f);
};

/** Complete inventory UI style configuration */
USTRUCT(BlueprintType)
struct FInventoryUIStyle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FInventorySlotStyle SlotStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FInventoryGridStyle GridStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FInventoryDetailStyle DetailStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FInventoryQuickSlotStyle QuickSlotStyle;
};