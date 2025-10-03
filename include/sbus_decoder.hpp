#include <include/sbus.hpp>

#include <optional>

class SbusDecoder
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
    std::optional<SbusChannels>
    consumeChar(uint8_t const& byte);

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
    SbusFrame mCurrentFrame{};
};