#include <eosiolib/eosio.hpp>

using namespace eosio;
class picture : public eosio::contract
{

  private:
    uint32_t getXY(uint16_t x, uint16_t y)
    {
        // Первые 16 бит 32 битного числа отдаем под X координату, последние - под Y координату
        return (uint32_t)(x << 16) + y;
    }
    uint16_t getX(uint32_t xy)
    {
        return (uint16_t)(xy >> 16);
    }
    uint16_t getY(uint32_t xy)
    {
        return (uint16_t)(xy & 0xFFFF);
    }

  public:
    using contract::contract;

    picture(account_name self) : contract(self), pixels(self, self) {}

    /// @abi tablexw
    struct pixel
    {
        // ключ - для ячейки - ее порядковый номер, если считать слево-направо, сверху вниз
        uint64_t key;
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;

        auto primary_key() const { return key; };
        EOSLIB_SERIALIZE(pixel, (key)(r)(g)(b))
    };

    typedef eosio::multi_index<N(pixel), pixel> pixelsstore;
    pixelsstore pixels;

    /// @abi action
    void setpixel(char x, char y, char r, char g, char b)
    {
        pixels.emplace(get_self(), [&](pixel &p) {
            p.key = getXY(x, y);
            p.r = r;
            p.g = g;
            p.b = b;
        });
    }
};

EOSIO_ABI(picture, (setpixel))
