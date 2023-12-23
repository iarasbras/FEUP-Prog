#include "Image.hpp"

namespace prog
{

  // Cria uma imagem de largura w, altura h, em que todos os píxeis têm uma cor dada
  // Pixel(w*h,fill) representa uma imagem com "w*h" píxeis com a cor "fill"
  Image::Image(int w, int h, const Color &fill):altura(h),largura(w),pixel(w*h,fill) {}

  // Liberta a memória dinamicamente alocada
  Image::~Image()
  {
    pixel.clear(); // Destrói todos os elementos do vetor pixel, ficando com tamanho = 0
  }

  // Retorna a largura da imagem
  int Image::width() const
  {
    return largura;
  }

  // Retorna a altura da imagem
  int Image::height() const
  {
    return altura;
  }

  // Retorna uma referência mutável do valor do pixel
  Color& Image::at(int x, int y)
  {

    /* Para obter a posição do pixel, multiplicar o y pela largura  (número de píxeis que estavam nas linhas anteriores),
    somar x (número de píxeis anteriores que estam na mesma linha) */
    int posicao = y * largura + x;

    // Retorna uma referência mutável do valor do pixel na determinada posição
    return pixel[posicao];

  }

  // Retorna o valor do pixel
  const Color& Image::at(int x, int y) const
  {
    int posicao = y * largura + x;
    return pixel[posicao];
  }
}
