#include <include/sbus.hpp>

class SbusDecode
{
    struct SbusFrame
    {
        static constexpr uint8_t expectedStartMarker {0xF0};
        static constexpr uint8_t expectedEndMarker{0x00};
        static constexpr size_t dataLen{22};
        static constexpr size_t endOfStart{1};
        static constexpr size_t endOfData{endOfStart + dataLen};
        static constexpr size_t endOfFlags{endOfData + 1};
        static constexpr size_t totalLen{endOfFlags + 1};

        std::array<uint8_t, dataLen> data;
        uint8_t flags;
    };

public:
    void
    consumeChar(uint8_t const& byte)
    {
        if (mCurrPos == 0)
        {
            // Start.
            if (byte != SbusFrame::expectedStartMarker)
            {
                mUnsyncPackets++;
                mCurrPos = 0;
                return;
            }
        }
        else if (mCurrPos < SbusFrame::endOfData)
        {
            mCurrentFrame.data[mCurrPos - SbusFrame::endOfStart] = byte;
        }
        else if (mCurrPos < SbusFrame::endOfFlags)
        {
            mCurrentFrame.flags = byte;
        }
        else if (mCurrPos < SbusFrame::totalLen)
        {
            if (byte != SbusFrame::expectedEndMarker)
            {
                mUnsyncPackets++;
            }
            else
            {
                mLastValidState = interpretFrame(mCurrentFrame);
            }

            mCurrPos = 0;
            return;
        }

        // successfully consumed mid-protocol character.
        mCurrPos++;
    }
private:
    static constexpr
    SbusChannels
    interpretFrame(const SbusFrame& frame)
    {
        // TODO
        SbusChannels ret{};
        for (size_t bitCounter = 0; bitCounter < frame.data.size() * 8; bitCounter++)
        {
            // BOOP
        }

        ret.flags = frame.flags;
        return ret;
    }


    uint8_t mCurrPos{0};
    uint8_t mUnsyncPackets{0};
    SbusChannels mLastValidState{};
    SbusFrame mCurrentFrame{};
};