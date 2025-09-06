// "Unreal Pokémon" created by Retro & Chill.

#pragma once

namespace Simple2D
{
    /**
     *
     */
    class SIMPLE2DEDITOR_API FSimple2DStyle
    {
      public:
        static void Initialize();

        static void Shutdown();

        static TSharedPtr<ISlateStyle> Get();

        static FName GetStyleSetName();

      private:
        static FString InContent(const FString &RelativePath, const ANSICHAR *Extension);

        static TSharedPtr<FSlateStyleSet> StyleSet;
    };
} // namespace Simple2D