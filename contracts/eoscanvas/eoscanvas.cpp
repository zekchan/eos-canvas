#include <eosiolib/eosio.hpp>

using namespace eosio;

class picture : public eosio::contract {

private:

    auto validateInput(char x, char y, char r,char  g,char  b) {
        eosio_assert(x < picture_width, "X must be less then 100");
        eosio_assert(y < picture_height, "X must be less then 100");
    }
public:
    using contract::contract;

    picture( account_name self ):contract(self), pixels(self, self){}
    // параметры пикчи, мб пригодится
    static const char picture_width = 5;
    static const char picture_height = picture_width;
    // Структура - описание пикселя
    /// @abi table
    struct pixel {

        // ключ - для ячейки - ее порядковый номер, если считать слево-направо, сверху вниз
        uint64_t primary_key() const { return key; };
        uint64_t key;
        char X;
        char Y;
        char R = 0;
        char G = 0;
        char B = 0;

        EOSLIB_SERIALIZE(pixel,(key)(X)(Y)(R)(G)(B))
    };
    // Объявим таблицу, хранящуюю пиксели
    typedef eosio::multi_index<N(pixel), pixel> pixelsstore;
    pixelsstore pixels;

    /// @abi action
    void setpixel(char x, char y, char r, char g, char b) {

        validateInput(x, y, r, g, b);
        pixels.emplace(get_self(), [&](pixel& p)
        {
            p.key = y * picture_width + x;
            p.X = x;
            p.Y = y;
            p.R = r;
            p.G = g;
            p.B = b;
        });
    }
};

EOSIO_ABI( picture, (setpixel))
