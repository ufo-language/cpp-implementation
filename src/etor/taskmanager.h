namespace ufo {

    class Evaluator;

    class TaskManager {
    public:
        TaskManager();
        ~TaskManager();

        void add(Evaluator* etor);
        void block(Evaluator* etor, Any* blockingObject);
        void unblock(Evaluator* etor);

    protected:

        Evaluator* _head;
        Evaluator* _tail;

    };
}
