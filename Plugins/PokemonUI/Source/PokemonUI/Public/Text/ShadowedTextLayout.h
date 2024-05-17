// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Text/SlateTextLayout.h"

/**
 * Determines the layout for a Shadowed Text component
 */
class POKEMONUI_API FShadowedTextLayout : public FSlateTextLayout {
  public:
    /**
     * Construct a new layout
     * @param InOwner The owning widget for this one
     * @param InDefaultTextStyle The default text style used by this layout widget
     */
    FShadowedTextLayout(SWidget *InOwner, const FTextBlockStyle &InDefaultTextStyle);

  protected:
    TSharedRef<IRun> CreateDefaultTextRun(const TSharedRef<FString> &NewText,
                                          const FTextRange &NewRange) const override;
};
