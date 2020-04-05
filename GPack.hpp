#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace sf;

namespace gpack {
    // Get vector from Image. Mode "gray", "red", "green", "blue" return different val pixels
    // Получает вектор значений серых пикселей из изображения с иcпользованием режима.
    // Режимы: "gray", "red", "green", "blue"
    vector <short> imageToVector(Image img, string mode) {
        Vector2u imgsize = img.getSize();
        vector <short> result(imgsize.x * imgsize.y);
        
        // Перевожу строку mode в intmode через блок if'ов
        short intmode = 0;
        if(mode == "gray" || mode == "grey") { intmode = 0;}
        else if(mode == "red") intmode = 1;
        else if(mode == "green") intmode = 2;
        else if(mode == "blue") intmode = 3;

        short val = 0;
        // Перебираю пиксели по горизонтали; слева направо,  сверху вниз
        for(int y=0; y < imgsize.y; y++) {
            for(int x=0; x < imgsize.x; x++) {
                Color pixel = img.getPixel(x, y);
                // В зависимости от режима возращаю значение пикселя
                switch (intmode) {
                    case 0:
                        // Without '1' pixel in RGB (255, 255, 254) will return Gray with value 254, but more logic value is 255
                        // Added '1' make val more smoothed
                        val = ((short)pixel.r + (short)pixel.g + (short)pixel.b + 1)/3;
                        break;
                    case 1:
                        val = (short)pixel.r;
                        break;
                    case 2:
                        val = (short)pixel.g;
                        break;
                    case 3:
                        val = (short)pixel.b;
                        break;
                    default:
                        cerr << "Error in switch: strange intmode\n";
                        break;
                }
                // Добавляем новый элемент в вектор. imgsize.x * y + x т.к. перебираем горизонтально и вектор линейный.
                result[imgsize.x * y + x] = val;
            }
        }
        return result;
    }
    // Перегруженная версия ImageToVector, в которой можно передавать не само изображение, а путь на изображение.
    // Думаю, он менее эффективный, так как он два раза возращает вектор.
    // Но пусть будет.
    vector <short> imageToVector(string imgpath, string mode) {
        Image img;
        img.loadFromFile(imgpath);
        return imageToVector(img, mode);
    }
    // interpolationMode:
    // "neighbor" - very fast, but may bugs in image;
    // HAVEN'T REALISED!!! supersampling" - slower, but more trouth and Smoothed
    Image resizeImage(Image img, int newWidth, int newHeight, string interpolationMode = "neighbor") {
        short intMode = 0;
        if(interpolationMode == "supersampling") { intMode = 0;}
        else if(interpolationMode == "neighbor") { intMode = 1;}

        Image imgResult;
        imgResult.create(newWidth, newHeight, Color::Black);

        int imgWidth = img.getSize().x;
        int imgHeight = img.getSize().y;
        switch(intMode) {
            case 0:
                cerr << "Supersampling Not ready\n";
            case 1:
                for(int j=0; j < newHeight; j++) {
                    for(int i=0; i < newWidth; i++) {
                        imgResult.setPixel(i, j,
                            img.getPixel(i*imgWidth/newWidth, j*imgHeight/newHeight)
                        );
                    }
                }
        }
        return imgResult;
    }
    Image resizeImage(string imgpath, int newWidth, int newHeight, string interpolationMode = "neighbor") {
        Image img;
        img.loadFromFile(imgpath);

        return resizeImage(img, newWidth, newHeight, interpolationMode);        
    }
}

using namespace gpack;

/* Просто код, которым я проверял работоспособность программы.
int main() {
    Image img;
    string inputImagePath;
    string outputImagePath;
    cin >> inputImagePath;
    cin >> outputImagePath;

    img.loadFromFile(inputImagePath);
    Image newimg = resizeImage(img, 150, 100, "neighbor");
    newimg.saveToFile(outputImagePath);

    return 0;
}
*/