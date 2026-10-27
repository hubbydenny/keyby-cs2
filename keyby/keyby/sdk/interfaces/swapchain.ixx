module;
export module swapchain_interface;
import memory;

class IDXGISwapChain;
export class swap_chain_interface {
public:
	IDXGISwapChain* get_swap;
private: mem::padding<0x170> m_pad;
};
