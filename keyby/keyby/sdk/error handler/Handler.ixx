module;
#include "../../includes.hpp"

export module error;

export namespace error {

    enum class Code {
        ModuleNotFound,
        ExportNotFound,
        InterfaceNotFound,
        InitializationFailed
    };

    struct Info {
        Code code;
        std::string_view message;
    };

    template<typename T>
    using Result = std::expected<T, Info>;

}