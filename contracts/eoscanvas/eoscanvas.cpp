#include <eosiolib/eosio.hpp>

using namespace eosio;
class eoscanvas : public eosio::contract
{

  private:
    static uint64_t getXY(uint32_t x, uint32_t y)
    {
        // Первые 16 бит 32 битного числа отдаем под X координату, последние - под Y координату
        return ((uint64_t)x << 32) + y;
    }

  public:
    using contract::contract;

    eoscanvas(account_name self) : contract(self), pixels(self, self) {}

    /// @abi table
    struct pixel
    {
        // ключ - для ячейки - ее порядковый номер, если считать слево-направо, сверху вниз
        uint64_t key;
        uint32_t x;
        uint32_t y;
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;

        auto primary_key() const { return key; };
        EOSLIB_SERIALIZE(pixel, (key)(r)(g)(b))
    };

    typedef eosio::multi_index<N(pixel), pixel> pixelsstore;
    pixelsstore pixels;

    /// @abi action
    void setpixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
    {
        pixels.emplace(get_self(), [&](pixel &p) {
            p.key = getXY(x, y);
            p.x = x;
            p.y = y;
            p.r = r;
            p.g = g;
            p.b = b;
        });
    }
};

EOSIO_ABI(eoscanvas, (setpixel))
