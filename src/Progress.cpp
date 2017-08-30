#include "Progress.hpp"

namespace WS2 {
    void Progress::begin(unsigned int stepCount) {
        ProgressOperation *op = new ProgressOperation();
        op->stepCount = stepCount;

        opStack.push(op);

        emit valueChanged(getValue());
        emit maxChanged(getMax());
    }

    void Progress::end() {
        ProgressOperation *op = opStack.pop();
        delete op;

        emit maxChanged(getMax());
        emit valueChanged(getValue());
    }

    void Progress::inc() {
        opStack.top()->currentStep += 1;

        emit valueChanged(getValue());
    }

    unsigned int Progress::getValue() {
        //TODO: Represent entire progress
        if (opStack.size() == 0) return 1;
        return opStack.top()->currentStep;
    }

    unsigned int Progress::getMax() {
        //TODO: Represent entire progress
        if (opStack.size() == 0) return 1;
        return opStack.top()->stepCount;
    }
}

