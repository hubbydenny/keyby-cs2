module;
export module viewsetup_class;
import memory;
import math;

export {
	class CViewSetup {
	public:
		char pad1[0x498];
		float fov;
		char pad2[0x4A0];
		vec3_t m_vecOrigin; // world eye origin
		char pad3[0x4B8];
		vec3_t m_angViewAngles;
	};
}