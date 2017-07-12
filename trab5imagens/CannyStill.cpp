// CannyStill.cpp

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<conio.h>           

using namespace cv;
using namespace std;

int acabou,opcaoEscolhida;

void filtroWiener() {

}

void filtroMediana() {

	cout << "\n == FILTRO DA MEDIANA == \n";
	cout << "- inserir nome da imagem: ";
	string nomeImagem;
	cin >> nomeImagem;

	Mat image = imread("ruidos1/" + nomeImagem + ".png", 1);
	Mat image1;

	cvtColor(image, image1, COLOR_BGR2GRAY);
	imshow("Imagem Original", image1);
	uchar * data = image1.data;
	vector<int> values;

	for (auto index = 0; index < (image1.rows*image1.cols); index++)
	{
		for (size_t i = 0; i < 13; i++) // 13 is the element to get the median from...
		{
			values.push_back(data[index + i]);

		}
		std::sort(values.begin(), values.end());

		image1.data[index] = values[6]; // 6 is the median  of 13 plus the index number (0-6) ...
		values.clear();
	}

	imshow("Imagem filtro mediana", image1);
	waitKey(0);
}

void menu() {
	cout << "\n\n : ";

	cout << "TRABALHO 5 - FILTROS DE IMAGEM";

	cout << "\n 1- Filtro da mediana";
	cout << "\n 2- Filtro de Wiener";
	cout << "\n 3- Sair";
	cout << "\n\n : ";
	int opcao;
	cin >> opcao;

	if (opcao == 3)
		acabou = 1;
	else
		opcaoEscolhida = opcao;
}

int main() {

	/*

	//teste de filtro exemplo 

	cv::Mat imgOriginal;        // input image
	cv::Mat imgGrayscale;       // grayscale of input image
	cv::Mat imgBlurred;         // intermediate blured image
	cv::Mat imgCanny;           // Canny edge image

	imgOriginal = cv::imread("image.jpg");          // open image

	if (imgOriginal.empty()) {                                  // if unable to open image
		std::cout << "error: image not read from file\n\n";     // show error message on command line
		_getch();                                               // may have to modify this line if not using Windows
		return(0);                                              // and exit program
	}

	cv::cvtColor(imgOriginal, imgGrayscale, CV_BGR2GRAY);       // convert to grayscale

	cv::GaussianBlur(imgGrayscale,          // input image
		imgBlurred,                         // output image
		cv::Size(5, 5),                     // smoothing window width and height in pixels
		1.5);                               // sigma value, determines how much the image will be blurred

	cv::Canny(imgBlurred,           // input image
		imgCanny,                   // output image
		100,                        // low threshold
		200);                       // high threshold

									// declare windows
	cv::namedWindow("imgOriginal", CV_WINDOW_AUTOSIZE);     // note: you can use CV_WINDOW_NORMAL which allows resizing the window
	cv::namedWindow("imgCanny", CV_WINDOW_AUTOSIZE);        // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
															// CV_WINDOW_AUTOSIZE is the default
	cv::imshow("imgOriginal", imgOriginal);     // show windows
	cv::imshow("imgCanny", imgCanny);

	cv::waitKey(0);                 // hold windows open until user presses a key

	return(0);
	*/

/*
//Hello world
	Mat img = imread("image.jpg", CV_LOAD_IMAGE_UNCHANGED); //read the image data in the file "MyPic.JPG" and store it in 'img'

	if (img.empty()) //check whether the image is loaded or not
	{
		cout << "Error : Image cannot be loaded..!!" << endl;
		//system("pause"); //wait for a key press
		return -1;
	}

	namedWindow("MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
	imshow("MyWindow", img); //display the image which is stored in the 'img' in the "MyWindow" window

	waitKey(0); //wait infinite time for a keypress

	destroyWindow("MyWindow"); //destroy the window with the name, "MyWindow"

	return 0;

	*/

	//Projeto 5
	acabou = 0;

	while (!acabou) {
		menu();
		if (acabou != 1) {
			if (opcaoEscolhida == 1)//filtro mediana
				filtroMediana();
			else if (opcaoEscolhida == 2)//filtro winenr
				filtroWiener();
		}
	}
		cout << "\n\n\n";
		system("PAUSE");
}