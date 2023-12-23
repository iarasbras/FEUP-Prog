#include "Color.hpp"

namespace prog {

    // Construtor default que corresponde à cor preta (0,0,0)
    Color::Color():vermelho(0),verde(0),azul(0) {}

    // Construtor que copia uma cor para outra
    Color::Color(const Color& c):vermelho(c.red()),verde(c.green()),azul(c.blue()) {}
    
    // Construtor que define uma cor com valores dados
    Color::Color(rgb_value red, rgb_value green, rgb_value blue):vermelho(red),verde(green),azul(blue) {}

    // Retorna o valor do canal vermelho
    rgb_value Color::red() const {
        return vermelho;
    }

    // Retorna o valor do canal verde
    rgb_value Color::green() const {
        return verde;
    }

    // Retorna o valor do canal azul
    rgb_value Color::blue() const {
        return azul;
    }

    // Retorna uma referência mutável do canal vermelho
    rgb_value& Color::red()  {
        return vermelho;
    }

    // Retorna uma referência mutável do canal verde
    rgb_value& Color::green()  {
        return verde;
    }

    // Retorna uma referência mutável do canal azul
    rgb_value& Color::blue()  {
        return azul;
    }
}
