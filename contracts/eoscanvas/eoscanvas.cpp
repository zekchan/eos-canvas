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

    struct pixel
    {
        uint32_t x;
        uint32_t y;
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        account_name owner;

        auto primary_key() const { return getXY(x, y); };
    };

    typedef eosio::multi_index<N(pixel), pixel> pixelsstore;
    pixelsstore pixels;

    void setpixel(account_name account, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
    {
        require_auth(account);
        auto key = getXY(x, y);
        auto itr = pixels.find(key);
        if (itr == pixels.end())
        {
            pixels.emplace(_self, [&](pixel &p) {
                p.owner = account;
                p.x = x;
                p.y = y;
                p.r = r;
                p.g = g;
                p.b = b;
            });
        }
        else
        {
            pixels.modify(itr, _self, [&](auto &p) {
                p.r = r;
                p.owner = account;
                p.g = g;
                p.b = b;
            });
        }
    }
};

EOSIO_ABI(eoscanvas, (setpixel))
