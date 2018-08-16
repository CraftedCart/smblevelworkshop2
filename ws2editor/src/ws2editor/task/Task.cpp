#include "ws2editor/task/Task.hpp"

namespace WS2Editor {
    namespace Task {
        void Task::run() {
            runTask(prog);
            emit onFinished(this);
        }

        void Task::setProgress(Progress *prog) {
            this->prog = prog;
        }
    }
}

