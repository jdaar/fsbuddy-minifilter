template <typename T>
class RingBuffer {
    private:
        int size;
        int head;
        int tail;
        T* buffer;
    public:
        RingBuffer(int size);
        ~RingBuffer();
        void enqueue(T item);
        void dequeue(T item);
        void peek();

        void resize(int size);
};