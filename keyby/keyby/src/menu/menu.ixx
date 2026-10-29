module;
#include "../../includes.hpp"
export module menu;
export import error;
export namespace menu {
    error::Result<void> create();
    bool destroy();
    void render();
    void set_mouse_mode_handler(void (*handler)(bool));
}
