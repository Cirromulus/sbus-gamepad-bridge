#include <include/sbus_decoder.hpp>

std::optional<SbusChannels>
SbusDecoder::consumeChar(uint8_t const& byte)
{
    if (mCurrPos == 0)
    {
        // Start.
        if (byte != SbusFrame::expectedStartMarker)
        {
            mUnsyncPackets++;
            mCurrPos = 0;
            return std::nullopt;
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
        mCurrPos = 0;


        if (byte != SbusFrame::expectedEndMarker)
        {
            mUnsyncPackets++;
            return std::nullopt;
        }
        // else

        return interpretFrame(mCurrentFrame);
    }

    // successfully consumed mid-protocol character.
    mCurrPos++;
    return std::nullopt;
}