#include <pico/mutex.h>


template <typename T>
class SynchronizerBuffer
{
    T mBuffer;
    T mCopy;

    mutex_t mMutex;

public:
    SynchronizerBuffer() :
        mBuffer{},
        mCopy{mBuffer},
        mMutex{}
    {
        mutex_init(&mMutex);
    }

    const T&
    getLatest()
    {
        // TODO: Find out whether the read needs to be instantaneous.
        mutex_enter_blocking(&mMutex);
        mCopy = mBuffer;
        mutex_exit(&mMutex);
        return mCopy;
    }

    // false: no update happened. Try again!
    bool
    setLatest(T const& toSet)
    {
        // TODO: Find out whether the write needs to be instantaneous.
        // could do a try_enter and return false on block.
        mutex_enter_blocking(&mMutex);
        mBuffer = toSet;
        mutex_exit(&mMutex);
        return true;
    }
};