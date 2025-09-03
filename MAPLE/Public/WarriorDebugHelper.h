#pragma once

namespace Debug
{
	static void print(const  FString& Msg, const FColor& color = FColor::MakeRandomColor(), int32 InKey = -1) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(InKey, 7.f, color, Msg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}
	static void print(const FString& FloatTitle, float FloatValueToPrint, int32 InKey = -1, const FColor& Color = FColor::MakeRandomColor()) {
		if (GEngine) {
			const FString FinalMsg = FloatTitle + TEXT(": ") + FString::SanitizeFloat(FloatValueToPrint);

			GEngine->AddOnScreenDebugMessage(InKey, 7.f, Color, FinalMsg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMsg);
		}
	}
}