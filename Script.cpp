#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"
#include "Image.hpp"
#include <algorithm>

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save") {
                save();
                continue;

            } 
            if (command == "invert") {
                invert();
                continue;
            }
            if (command == "to_gray_scale") {
                to_gray_scale();
                continue;
            }
            if (command == "replace") {
                int r1, g1, b1, r2, g2, b2;
                input >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;
                replace(r1, g1, b1, r2, g2, b2);
                continue;
            }
            if (command == "fill") {
                int x,y,w,h,r,g,b;
                input >> x >> y >> w >> h >> r >> g >> b;
                fill(x,y,w,h,r,g,b);
                continue;
            }
            if (command == "h_mirror") {
                h_mirror();
                continue;
            }
            if (command == "v_mirror") {
                v_mirror();
                continue;
            }
            if (command == "add") {
                string filename;
                int r, g, b, x, y;
                input >> filename >> r >> g >> b >> x >> y;
                add(filename, r, g, b, x, y);
                continue;
            }
            if (command == "crop") {
                 int x, y, w, h;
                 input >> x >> y >> w >> h;
                 crop(x, y, w, h);
                continue;
            }
            if (command == "rotate_left") {
                rotate_left();
                continue;
            }
            if (command == "rotate_right") {
                rotate_right();
                 continue;
            }
             if (command == "median_filter") {
                int ws;
                input >> ws;
                median_filter(ws);
                continue;
            }


    }
    }
    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }


    // Inverte a cor dos píxeis da imagem
    void Script::invert(){

    // Uso do "image" - pointer para um objeto da classe "Image" - para definir a largura e altura da imagem, através das funções "width" e "height"
    int largura = image->width();
    int altura = image->height();

    //Loop que percorre a altura da imagem, incrementando o valor do y
    for (int y = 0; y < altura; y++)
    {
        // (Nested) Loop que percorre a largura da imagem, incrementando o valor do x
        for (int x = 0; x < largura; x++)
        {
            // Uso do pointer "image" para definir o pixel através da função "at"
            Color &pixel = image->at(x, y);
            // Inverte os valores dos canais RGB
            pixel.red() = 255 - pixel.red();
            pixel.green() = 255 - pixel.green();
            pixel.blue() = 255 - pixel.blue();
        }
    }
    }

    // Transforma as cores da imagem em tons de cinzento
    void Script::to_gray_scale(){

    // Uso do "image" - pointer para um objeto da classe "Image" - para definir a largura e altura da imagem, através das funções "width" e "height"
    int largura = image->width();
    int altura = image->height();
    //Loop que percorre a altura da imagem, incrementando o valor do y
    for (int y = 0; y < altura; y++)
    {
        // (Nested) Loop que percorre a largura da imagem, incrementando o valor do x
        for (int x = 0; x < largura; x++)
        {
            // Uso do pointer "image" para definir o pixel através da função "at"
            Color &pixel = image->at(x, y);
            // Calculo da média dos valores dos canais RGB
            int v = (pixel.red() + pixel.green() + pixel.blue()) / 3;
            // Substituir o valor de cada canal pela média para obter um valor na escala de cinzento
            pixel.red() = v;
            pixel.green() = v;
            pixel.blue() = v;
        }
    }
    }

// Substiui uma cor por outra 
void Script::replace(int r1, int g1, int b1, int r2, int g2, int b2)
{
    // Uso do "image" - pointer para um objeto da classe "Image" - para definir a largura e altura da imagem, através das funções "width" e "height"
    int largura = image->width();
    int altura = image->height();
    //Loop que percorre a altura da imagem, incrementando o valor do y
    for (int y = 0; y < altura; y++)
    {
        // (Nested) Loop que percorre a largura da imagem, incrementando o valor do x
        for (int x = 0; x < largura; x++)
        {
            // Uso do pointer "image" para definir o pixel através da função "at"
            Color &pixel = image->at(x, y);
            // Uso do "if" para determinar quais os píxeis a ser alterados, comparando o valor de cada canal com os dados inicialmente (r1,g1,b1)
            if (pixel.red() == r1 && pixel.green() == g1 && pixel.blue() == b1)
            {
                //Substitui os valores dos canais dos píxeis determinados pelos valores novos (r2,g2,b2)
                pixel.red() = r2;
                pixel.green() = g2;
                pixel.blue() = b2;
            }
        }
    }
}

//Altera a cor dos píxeis num certo retângulo 
//(x, y) representa o canto superior esquerdo, h a altura e w a largura
void Script::fill(int x, int y, int w, int h, int r, int g, int b)
{
    
    // Uso do "image" - pointer para um objeto da classe "Image" - para definir a largura e altura da imagem, através das funções "width" e "height"
    int largura = image->width();
    int altura = image->height();

    // Loop que percorre a altura do retângulo (h), começando no canto superior esquerdo
    for (int i = y; i < y + h; i++)
    {
        // (Nested) Loop que percorre a largura do retângulo (w), começando no canto superior esquerdo
        for (int j = x; j < x + w; j++)
        {
            //Uso do "if" para verificar se os píxeis se encontram nas dimensôes do retângulo
            if (j >= 0 && j < largura && i >= 0 && i < altura)
            {
                // Uso do pointer "image" para definir o pixel através da função "at"
                Color &pixel = image->at(j, i);
                
                //Substitui os valores dos canais dos píxeis determinados pelos valores novos (r,g,b)
                pixel.red() = r;
                pixel.green() = g;
                pixel.blue() = b;
            }
        }
    }
}

// Espelha a imagem horizontalmente
void Script::h_mirror()
{
    // Uso do "image" para definir a largura e altura da imagem, através das funções "width" e "height"
    int largura = image->width();
    int altura = image->height();

    //Loop que percorre a altura da imagem, incrementando o valor do y
    for (int y = 0; y < altura; y++)
    {
        // (Nested) Loop que percorre metada da largura da imagem, incrementando o valor do x
        for (int x = 0; x < largura / 2; x++)
        {
            // Troca a posição de um pixel com o pixel correspondente do lado oposto da imagem
            Color &pixel1 = image->at(x, y);
            Color &pixel2 = image->at(largura - 1 - x, y);
            swap(pixel1, pixel2); 
        }
    }
}

// Espelha a imagem verticalmente
void Script::v_mirror() {

    // Uso do "image" para definir a largura e altura da imagem, através das funções "width" e "height"
    int largura = image->width();
    int altura = image->height();

    //Loop que percorre metade da  altura da imagem, incrementando o valor do y
    for (int y = 0; y < altura / 2; y++) {

        // (Nested) Loop que percorre a largura da imagem, incrementando o valor do x
        for (int x = 0; x < largura; x++) {

            // Troca a posição de um pixel com o pixel correspondente do lado oposto da imagem
            Color& pixel1 = image->at(x, y);
            Color& pixel2 = image->at(x, altura - 1 - y);
            std::swap(pixel1, pixel2);
        }
    }
}

// Copia os píxeis de uma imagem exceto os que têm cor "neutra" (r, g, b) para um retângulo da imagem atual
// (x, y) representa o canto superior esquerdo da imagem atual
void Script::add(const std::string& filename, int r, int g, int b, int x, int y) {
    
    // Carrega uma imagem armazenada num ficheiro PNG e guarda a sua referência em "imagem_png" - pointer para um objeto da classe "Image"
    Image* imagem_png = loadFromPNG(filename);

    // Uso do "image_png" para definir a largura e altura da imagem armazenada no ficheiro PNG, através das funções "width" e "height"
    int largura_png = imagem_png->width();
    int altura_png = imagem_png->height();

     // Loop que percorre a altura da imagem (PNG), incrementando o valor do y_png  
    for (int y_png = 0; y_png < altura_png; y_png++) {

        // (Nested) Loop que percorre a largura da imagem (PNG), incrementando o valor do x_png
        for (int x_png = 0; x_png < largura_png; x_png++) {

            // Uso do pointer "image_png" para definir o pixel da imagem (PNG) através da função "at"
            Color& pixel_png = imagem_png->at(x_png, y_png);
            
            // Verifica se os valores dos canais RGB dos píxeis não correspondem aos da cor "neutra" (r, g, b)
            if (pixel_png.red() != r || pixel_png.green() != g || pixel_png.blue() != b) {

                // Soma para encontrar a coordenada do pixel a substituir
                int x_atual = x + x_png;
                int y_atual = y + y_png;
                
                // Uso do pointer "image" para definir o pixel atual da imagem através da função "at"
                Color& pixel_atual = image->at(x_atual, y_atual);
                
                // Substitui o pixel da imagem atual pelo pixel da imagem (PNG)
                pixel_atual = pixel_png;
            }
        }
    }
}

void Script::crop(int x, int y, int w, int h) {

    // Uso do pointer "imagem_nova", criando uma nova imagem com as dimensões dadas (w,h) através do operador "new"
    Image* imagem_nova = new Image(w, h);

    // Copia os píxeis da imagem atual para a nova imagem
    for (int dy = 0; dy < h; dy++) {
        for (int dx = 0; dx < w; dx++) {
            int x_atual = x + dx;
            int y_atual = y + dy;

            // Uso do pointer image para definir o pixel atual e o novo pixel
            Color& pixel_atual = image->at(x_atual, y_atual);
            Color& pixel_novo = imagem_nova->at(dx, dy);

            pixel_novo = pixel_atual;
        }
    }

    // Substitui a imagem pela nova imagem (recortada)
    image = imagem_nova;
}

// Roda a imagem 90 graus para a esquerda
void Script::rotate_left() {

    // Define as dimensões da nova imagem através do uso do pointer "image"
    // A altura passa a ser a largura e vice-versa
    int largura_nova = image->height();
    int altura_nova = image->width();
    
    // Cria uma nova imagem com as novas dimensões, através do operador "new"
    Image* imagem_final = new Image(largura_nova, altura_nova);

    // Copia os píxeis da imagem original para a nova imagem (rodada)
    for (int y = 0; y < image->height(); y++) {
        for (int x = 0; x < image->width(); x++) {
            int x_final = y;
            int y_final = altura_nova - 1 - x; 
            Color& pixel_inicial = image->at(x, y);
            Color& pixel_final = imagem_final->at(x_final, y_final);
            pixel_final = pixel_inicial;
        }
    }

    // Substitui a imagem pela nova imagem (rodada)
    image = imagem_final;
}

// Roda a imagem 90 graus para a direita
void Script::rotate_right() {

    /// Define as dimensões da nova imagem através do uso do pointer "image"
    // A altura passa a ser a largura e vice-versa
    int largura_nova = image->height();
    int altura_nova = image->width();

    // Cria uma nova imagem com as novas dimensões, através do operador "new"
    Image* imagem_final = new Image(largura_nova, altura_nova);

    // Copia os píxeis da imagem original para a nova imagem (rodada)
    for (int y = 0; y < image->height(); y++) {
        for (int x = 0; x < image->width(); x++) {
            int x_final = largura_nova - 1 - y;
            int y_final = x;
            Color& pixel_inicial = image->at(x, y);
            Color& pixel_final = imagem_final->at(x_final, y_final);
            pixel_final = pixel_inicial;
        }
    }

    // Substitui a imagem pela nova imagem (rodada)
    image = imagem_final;
}

void Script::median_filter(int ws) {

    // Uso do "image" para definir a largura e altura da imagem, através das funções "width" e "height"
    int largura = image->width();
    int altura = image->height();

    rgb_value mr, mg, mb;

    // Cria uma nova imagem temporária para armazenar o resultado filtrado, através do operador "new"
    Image* imagem_filtrada = new Image(largura, altura);

    // Calcula a mediana para o window size indicado
    int mediana = ws / 2;

    // Loop que percorre a altura da imagem, incrementando o valor do y
    for (int y = 0; y < altura; y++) {
        // (Nested) Loop que percorre a largura da imagem, incrementando o valor do x
        for (int x = 0; x < largura; x++) {

            // Inicialização do vetor para armazenar os valores do píxel na vizinhança
            vector<Color> vizinhanca;

            // Loop que percorre os píxeis na vizinhança e adiciona o píxel ao vetor da vizinhança ("vizinhanca")
            for (int ny = max(0, y - mediana); ny <= min(altura - 1, y + mediana); ny++) {
                for (int nx = max(0, x - mediana); nx <= min(largura - 1, x + mediana); nx++) {
                    vizinhanca.push_back(image->at(nx, ny));
                }
            }

            // Ordena de forma crescente os valores do canal vermelho (RGB), através do uso do sort no vetor "vizinhanca"
            sort(vizinhanca.begin(), vizinhanca.end(), [](const Color& a, const Color& b) {
                return a.red() < b.red();
            });

            // Obtêm o "median pixel" do canal vermelho (RGB) pelo vetor já ordenado e armazena-o na variável "mr"
            if (vizinhanca.size() % 2 == 1) {
                mr = vizinhanca[vizinhanca.size() / 2].red();
            } else {
                mr = (vizinhanca[vizinhanca.size() / 2].red() + vizinhanca[vizinhanca.size() / 2  -1].red() ) / 2;
            }
            
            // Ordena de forma crescente os valores do canal verde (RGB), através do uso do sort no vetor "vizinhanca"
            sort(vizinhanca.begin(), vizinhanca.end(), [](const Color& a, const Color& b) {
                return a.green() < b.green();
            });

            // Obtêm o "median pixel" do canal verde (RGB) pelo vetor já ordenado e armazena-o na variável "mg"
            if (vizinhanca.size() % 2 == 1) {
                mg = vizinhanca[vizinhanca.size() / 2].green();
            } else {
                mg = (vizinhanca[vizinhanca.size() / 2].green() + vizinhanca[vizinhanca.size() / 2  -1].green() ) / 2;
            }
            
            // Ordena de forma crescente os valores do canal azul (RGB), através do uso do sort no vetor "vizinhanca"
            sort(vizinhanca.begin(), vizinhanca.end(), [](const Color& a, const Color& b) {
                return a.blue() < b.blue();
            });

            // Obtêm o "median pixel" do canal azul (RGB) pelo vetor já ordenado e armazena-o na variável "mb"
            if (vizinhanca.size() % 2 == 1) {
                mb = vizinhanca[vizinhanca.size() / 2].blue();
            } else {
                mb = (vizinhanca[vizinhanca.size() / 2].blue() + vizinhanca[vizinhanca.size() / 2  -1].blue() ) / 2;
            }
            
            // Atribui os valores do "median pixel" ao seu píxel correspondente na imagem filtrada
            Color& median_pixel = imagem_filtrada->at(x, y);
            median_pixel.red()=mr;
            median_pixel.green()=mg;
            median_pixel.blue()=mb;

        }
    }

    // Substitui a imagem pela nova imagem (filtrada)
    image = imagem_filtrada;
}




}
