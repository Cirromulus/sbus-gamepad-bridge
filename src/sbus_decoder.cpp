#include <include/sbus_decoder.hpp>

using namespace std;

expected<SbusChannels, SbusDecoder::DecodeState>
SbusDecoder::consumeChar(uint8_t const& byte)
{
    if (mCurrPos == 0)
    {
        // Start.
        if (byte != SbusFrame::expectedStartMarker)
        {
            mUnsyncPackets++;
            mCurrPos = 0;
            return unexpected(DecodeState::startMarkerNotAtStart);
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
    else // mCurrPos >= SbusFrame::totalLen
    {
        mCurrPos = 0;

        if (byte != SbusFrame::expectedEndMarker)
        {
            mUnsyncPackets++;
            return unexpected(DecodeState::endMarkerNotAtEnd);
        }
        // else

        return interpretFrame(mCurrentFrame);
    }

    // successfully consumed mid-protocol character.
    mCurrPos++;
    // this is actually not "unexpected", but oh well
    return unexpected(DecodeState::consumed);
}