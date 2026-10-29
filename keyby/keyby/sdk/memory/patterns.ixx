module;
export module patts;

export namespace patts {
    inline constexpr const auto* GetAimPunch = "48 89 5C 24 08 55 56 57 48 8D 6C 24 B9 48 81 EC";
    inline constexpr const auto* GetBoneId = "40 53 48 83 EC 20 48 8B 89 30 03 00 00 48 8B DA 48 8B 01 FF 50 68 48 8B C8";
    inline constexpr const auto* CalcBones = "48 89 4C 24 08 55 53 56 57 41 54 41 55 41 56 41 57 B8 98 42 00 00";
    inline constexpr const auto* AllTypeScopes = "48 8B 05 ? ? ? ? 48 8B D6 0F B7 CB 48 8B 3C C8";
	inline constexpr const auto* PresentHookTarget = "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 41 8B F0";
	inline constexpr const auto* ResizeTarget = "40 53 55 56 57 41 54 41 56 41 57 48 83 EC ? 44 8B E2";
	inline constexpr const auto* WorldToScree = "48 89 74 24 ? 57 48 83 EC ? 48 83 3D ? ? ? ? ? 48 8B FA";
	inline constexpr const auto* EntityList = "48 8B 0D ? ? ? ? 8B D3 E8 ? ? ? ? 48 8B F0";
	inline constexpr const auto* GetBaseEntity = "4C 8D 49 10 81 FA ? ? 00 00 77 ? 8B CA C1 F9 09";
	inline constexpr const auto* GetLocalPlayerController = "E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 44 8B 45 17 48 8B 38 4C 8B 55 0F 8B 75 0B 44 89 45 BB C7 45 B7 FF FF FF FF";
	inline constexpr const auto* IsInGame = "48 8B ? ? ? ? ? 48 85 C0 74 15 80 B8 ? ? ? ? ? 75 0C 83 B8 ? ? ? ? 06";
	inline constexpr const auto* IsConnected = "48 8B 05 ? ? ? ? 48 85 C0 74 ? 83 B8 ? ? ? ? ? 0F 9D C0";
	inline constexpr const auto* OverrideView = "40 53 57 48 83 EC 58 48 8B FA E8 ? ? ? ? 48";
}
