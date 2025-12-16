#include <commands/base.hpp>
#include <core/shell.hpp>

namespace Commands {
    int exit(Command command){
        ::exit(0);
    }
}