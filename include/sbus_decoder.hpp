#include <include/sbus.hpp>

#include <optional>
#include <algorithm>

class SbusDecoder
{
    struct SbusFrame
    {
        static constexpr uint8_t expectedStartMarker {0x0F};
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
    std::optional<SbusChannels>
    consumeChar(uint8_t const& byte);

private:

    static constexpr
    SbusChannels
    interpretFrame(const SbusFrame& frame)
    {
        constexpr size_t dataBytesPerChannel = 11;

        SbusChannels ret{};
        // TODO: Make out a nice rule
        // for (size_t channelPos = 0; channelPos < ret.axis.size(); channelPos++)
        // {
        //     const size_t frameBitStart = channelPos * dataBytesPerChannel;
        //     const size_t bytePos = frameBitStart / 8;
        //     const size_t bitOffs = frameBitStart % 8;

        //     const size_t firstByteValidBits = 8 - bitOffs; // guaranteed no overflow due to modulo 8
        //     ret.axis[channelPos] = frame.data[bytePos] >> bitOffs;

        //     // channels[0] = (uint16_t)((data[0] | data[1] << 8) & 0x07FF);
        //     // channels[1] = (uint16_t)((data[1] >> 3 | data[2] << 5) & 0x07FF);
        //     // channels[2] = (uint16_t)((data[2] >> 6 | data[3] << 2 | data[4] << 10) & 0x07FF);
        // }

        // TODO: Remove this copypasta
        // see https://platformio.org/lib/show/5622/Bolder%20Flight%20Systems%20SBUS
        ret.axis[0] = (uint16_t)((frame.data[0] | frame.data[1] << 8) & 0x07FF);
        ret.axis[1] = (uint16_t)((frame.data[1] >> 3 | frame.data[2] << 5) & 0x07FF);
        ret.axis[2] = (uint16_t)((frame.data[2] >> 6 | frame.data[3] << 2 | frame.data[4] << 10) & 0x07FF);
        ret.axis[3] = (uint16_t)((frame.data[4] >> 1 | frame.data[5] << 7) & 0x07FF);
        ret.axis[4] = (uint16_t)((frame.data[5] >> 4 | frame.data[6] << 4) & 0x07FF);
        ret.axis[5] = (uint16_t)((frame.data[6] >> 7 | frame.data[7] << 1 | frame.data[8] << 9) & 0x07FF);
        ret.axis[6] = (uint16_t)((frame.data[8] >> 2 | frame.data[9] << 6) & 0x07FF);
        ret.axis[7] = (uint16_t)((frame.data[9] >> 5 | frame.data[10] << 3) & 0x07FF);
        ret.axis[8] = (uint16_t)((frame.data[11] | frame.data[12] << 8) & 0x07FF);
        ret.axis[9] = (uint16_t)((frame.data[12] >> 3 | frame.data[13] << 5) & 0x07FF);
        ret.axis[10] = (uint16_t)((frame.data[13] >> 6 | frame.data[14] << 2 | frame.data[15] << 10) & 0x07FF);
        ret.axis[11] = (uint16_t)((frame.data[15] >> 1 | frame.data[16] << 7) & 0x07FF);
        ret.axis[12] = (uint16_t)((frame.data[16] >> 4 | frame.data[17] << 4) & 0x07FF);
        ret.axis[13] = (uint16_t)((frame.data[17] >> 7 | frame.data[18] << 1 | frame.data[19] << 9) & 0x07FF);
        ret.axis[14] = (uint16_t)((frame.data[19] >> 2 | frame.data[20] << 6) & 0x07FF);
        ret.axis[15] = (uint16_t)((frame.data[20] >> 5 | frame.data[21] << 3) & 0x07FF);


        ret.flags = frame.flags;
        return ret;
    }


    uint8_t mCurrPos{0};
    uint8_t mUnsyncPackets{0};
    SbusFrame mCurrentFrame{};
};