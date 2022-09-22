#include "data/task.h"

namespace ufo {

    D_Task* D_Task::create(Evaluator* etor) {
        return new D_Task(etor);
    }

    void D_Task::markChildren(std::queue<Any*>& markedObjects) {
        markedObjects.push(_etor);
    }

    void D_Task::show(std::ostream& stream) {
        stream << "A-TASK";
    }

}
