module;
#include "../../includes.hpp"
export module hooks;
export import error;
export namespace hooks {
    error::Result<void> start();
    bool stop();
}