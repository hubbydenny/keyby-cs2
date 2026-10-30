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
	inline constexpr const auto* CreateMove = "85 D2 0F 85 ? ? ? ? 48 8B C4 44 88 40 18";
	inline constexpr const auto* GetUserCmdTick = "48 83 EC ? 4C 8B 0D ? ? ? ? 4C 8B DA";
	inline constexpr const auto* GetUserCmdArray = "48 89 4C 24 ? 41 56 41 57";
	inline constexpr const auto* GetUserCmdBySequence = "40 53 48 83 EC ? 8B DA E8 ? ? ? ? 4C 8B C0";
	inline constexpr const auto* FirstUserCmdArray = "48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B CF 4C 8B F8";
	inline constexpr const auto* GetViewAngles = "4C 8B C1 85 D2 74 ? 48 8D 05";
	inline constexpr const auto* SetViewAngles = "85 D2 75 ? 48 63 81";
	inline constexpr const auto* ValidateUserCommand = "85 D2 0F 85 ? ? ? ? 48 89 5C 24 ? 56";
	inline constexpr const auto* FindHudElememnt = "40 53 48 83 EC 20 48 8B 05 ? ? ? ? 48 8B D9 48 85 C0 74 ? 48 89 5C 24 38";
	inline constexpr const auto* VoidPrintCall = "E8 ? ? ? ? 4C 39 74 24 ? 74 ? 48 8D 4C 24 ? FF 15 ? ? ? ? 33 D2";
	inline constexpr const auto* SerializeMoveCrc = "48 89 5C 24 20 55 56 57 48 83 EC 30 49 8B C0";
	inline constexpr const auto* CreateSubtickMoveStep = "E8 ? ? ? ? 48 8B D0 48 8B CE E8 ? ? ? ? 48 8B C8";
	inline constexpr const auto* ProtoBuffAddPtrEtc = "E8 ? ? ? ? 4C 8B D0 45 8B 4A 10";
	inline constexpr const auto* TraceManager = "48 8B 1D ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 4C 8B 0B 4C 8D 44 24 ? 48 8B C8";
	inline constexpr const auto* TraceShape = "48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 41 54 41 56 41 57 48 8D AC 24 ? ? ? ? B8";
	inline constexpr const auto* TraceFilterInit = "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 0F B6 41 ? 33 FF 24";
}
