#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace sf;

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
                    val = ((short)pixel.r + (short)pixel.g + (short)pixel.b)/3;
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



/* Просто код, которым я проверял работоспособность программы.
int main() {
    Image img;
    img.loadFromFile("./image3.png");

    vector <short> vect;
    vect = imageToVector(img, "gray");
    for(int i=0; i < 5; i++) {
        for (int j=0; j < 5; j++) {
            cout << vect[5*i + j] << "\t";
        }
        cout << endl;
    }
    cout << "-------------" << endl;
    vect = imageToVector(img, "red");
    for(int i=0; i < 5; i++) {
        for (int j=0; j < 5; j++) {
            cout << vect[5*i + j] << "\t";
        }
        cout << endl;
    }
    cout << "-------------" << endl;
    vect = imageToVector(img, "green");
    for(int i=0; i < 5; i++) {
        for (int j=0; j < 5; j++) {
            cout << vect[5*i + j] << "\t";
        }
        cout << endl;
    }
    cout << "-------------" << endl;
    vect = imageToVector(img, "blue");
    for(int i=0; i < 5; i++) {
        for (int j=0; j < 5; j++) {
            cout << vect[5*i + j] << "\t";
        }
        cout << endl;
    }
    return 0;
}
*/