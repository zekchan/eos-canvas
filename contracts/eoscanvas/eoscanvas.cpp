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
        account_name owner;

        auto primary_key() const { return key; };
        EOSLIB_SERIALIZE(pixel, (key)(r)(g)(b)(owner))
    };

    typedef eosio::multi_index<N(pixel), pixel> pixelsstore;
    pixelsstore pixels;

    /// @abi action
    void setpixel(account_name user, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
    {
        require_auth(user);
        auto key = getXY(x, y);
        auto itr = pixels.find(key);
        if (itr = pixels.end())
        {
            pixels.emplace(user, [&](pixel &p) {
                p.key = getXY(x, y);
                p.owner = user;
                p.x = x;
                p.y = y;
                p.r = r;
                p.g = g;
                p.b = b;https://developers.eos.io/eosio-cpp/reference#modify
            });
        }
        else
        {
            pixels.modify(itr, user, [&](auto &p) {
                p.r = r;
                p.owner = user;
                p.g = g;
                p.b = b;
            })
        }
    }
};

EOSIO_ABI(eoscanvas, (setpixel))
