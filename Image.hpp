#ifndef __prog_Image_hpp__
#define __prog_Image_hpp__
#include "Color.hpp"
#include <vector>

namespace prog
{
  class Image
  {
  private:
    // Representação da altura e largura da imagem
    int altura;
    int largura;
    // Representação da imagem através de um vetor
    std::vector<Color> pixel;
    // A imagem é composta por pixeis(representados por uma coordenada (x,y) = (largura,altura))
    // Cada pixel têm uma cor (representada pelos canais RGB)
  public:
    Image(int w, int h, const Color &fill = {255, 255, 255});
    ~Image();
    int width() const;
    int height() const;
    Color &at(int x, int y);
    const Color &at(int x, int y) const;
  };
}
#endif
