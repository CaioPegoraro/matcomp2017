// CannyStill.cpp

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<conio.h>           

using namespace cv;
using namespace std;

int acabou,opcaoEscolhida;
#define kappa 10000

CvMat* cvShowDFT1(IplImage* im, int dft_M, int dft_N, char* src)
{
	IplImage* realInput;
	IplImage* imaginaryInput;
	IplImage* complexInput;

	CvMat* dft_A, tmp;

	IplImage* image_Re;
	IplImage* image_Im;

	char str[80];

	double m, M;

	realInput = cvCreateImage(cvGetSize(im), IPL_DEPTH_64F, 1);
	imaginaryInput = cvCreateImage(cvGetSize(im), IPL_DEPTH_64F, 1);
	complexInput = cvCreateImage(cvGetSize(im), IPL_DEPTH_64F, 2);

	cvScale(im, realInput, 1.0, 0.0);
	cvZero(imaginaryInput);
	cvMerge(realInput, imaginaryInput, NULL, NULL, complexInput);

	dft_A = cvCreateMat(dft_M, dft_N, CV_64FC2);
	image_Re = cvCreateImage(cvSize(dft_N, dft_M), IPL_DEPTH_64F, 1);
	image_Im = cvCreateImage(cvSize(dft_N, dft_M), IPL_DEPTH_64F, 1);

	// copy A to dft_A and pad dft_A with zeros
	cvGetSubRect(dft_A, &tmp, cvRect(0, 0, im->width, im->height));
	cvCopy(complexInput, &tmp, NULL);
	if (dft_A->cols > im->width)
	{
		cvGetSubRect(dft_A, &tmp, cvRect(im->width, 0, dft_A->cols - im->width, im->height));
		cvZero(&tmp);
	}

	// no need to pad bottom part of dft_A with zeros because of
	// use nonzero_rows parameter in cvDFT() call below

	cvDFT(dft_A, dft_A, CV_DXT_FORWARD, complexInput->height);

	strcpy(str, "DFT -");
	strcat(str, src);
	//cvNamedWindow(str, 0);

	// Split Fourier in real and imaginary parts
	cvSplit(dft_A, image_Re, image_Im, 0, 0);

	// Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
	cvPow(image_Re, image_Re, 2.0);
	cvPow(image_Im, image_Im, 2.0);
	cvAdd(image_Re, image_Im, image_Re, NULL);
	cvPow(image_Re, image_Re, 0.5);

	// Compute log(1 + Mag)
	cvAddS(image_Re, cvScalarAll(1.0), image_Re, NULL); // 1 + Mag
	cvLog(image_Re, image_Re); // log(1 + Mag)

	cvMinMaxLoc(image_Re, &m, &M, NULL, NULL, NULL);
	cvScale(image_Re, image_Re, 1.0 / (M - m), 1.0*(-m) / (M - m));
	//cvShowImage(str, image_Re);
	return(dft_A);
}

void cvShowInvDFT1(IplImage* im, CvMat* dft_A, int dft_M, int dft_N, char* src)
{

	IplImage* realInput;
	IplImage* imaginaryInput;
	IplImage* complexInput;

	IplImage * image_Re;
	IplImage * image_Im;

	double m, M;
	char str[80];

	realInput = cvCreateImage(cvGetSize(im), IPL_DEPTH_64F, 1);
	imaginaryInput = cvCreateImage(cvGetSize(im), IPL_DEPTH_64F, 1);
	complexInput = cvCreateImage(cvGetSize(im), IPL_DEPTH_64F, 2);

	image_Re = cvCreateImage(cvSize(dft_N, dft_M), IPL_DEPTH_64F, 1);
	image_Im = cvCreateImage(cvSize(dft_N, dft_M), IPL_DEPTH_64F, 1);

	//cvDFT( dft_A, dft_A, CV_DXT_INV_SCALE, complexInput->height );
	cvDFT(dft_A, dft_A, CV_DXT_INV_SCALE, dft_M);
	strcpy(str, "Filtro Weiner – ");
	strcat(str, src);
	cvNamedWindow(str, 0);

	// Split Fourier in real and imaginary parts
	cvSplit(dft_A, image_Re, image_Im, 0, 0);

	// Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
	cvPow(image_Re, image_Re, 2.0);
	cvPow(image_Im, image_Im, 2.0);
	cvAdd(image_Re, image_Im, image_Re, NULL);
	cvPow(image_Re, image_Re, 0.5);

	// Compute log(1 + Mag)
	cvAddS(image_Re, cvScalarAll(1.0), image_Re, NULL); // 1 + Mag
	cvLog(image_Re, image_Re); // log(1 + Mag)

	cvMinMaxLoc(image_Re, &m, &M, NULL, NULL, NULL);
	cvScale(image_Re, image_Re, 1.0 / (M - m), 1.0*(-m) / (M - m));
	//cvCvtColor(image_Re, image_Re, CV_GRAY2RGBA);

	cvShowImage(str, image_Re);

}

//PSNR (relação sinal ruído de pico)
double PSNR(const Mat& Imagem1, const Mat& Imagem2)
{
	Mat s1;
	absdiff(Imagem1, Imagem2, s1);// |Imagem1 - Imagem2|
	s1.convertTo(s1, CV_32F);
	s1 = s1.mul(s1);// |Imagem1 - Imagem2|^2 //norma

	Scalar s = sum(s1); //soma os elementos

	double sse = s.val[0] + s.val[1] + s.val[2]; // soma os canais

	if (sse <= 1e-10)//ignora os menores valores
		return 0;
	else
	{
		double  mse = sse / (double)(Imagem1.channels() * Imagem1.total());
		double psnr = 10.0*log10((255 * 255) / mse); //formula para obter o PSNR
		return psnr;
	}
}


void filtroWiener() {
	cout << "\n == FILTRO DE WIENER (Ruido Gaussiano) == \n";
	cout << "- inserir nome da imagem: ";

	string nomeImagem;
	cin >> nomeImagem;

	Mat imagemSemRuido = imread("ruidos1/" + nomeImagem + ".png", 1);
	Mat imagemSemRuidoGrey;
	cvtColor(imagemSemRuido, imagemSemRuidoGrey, COLOR_BGR2GRAY);
	imshow("Imagem original", imagemSemRuidoGrey);

	Mat imagem2 = imread("ruidos1/" + nomeImagem + "_gaussian.png", 1);
	Mat imagem;
	cvtColor(imagem2, imagem, COLOR_BGR2GRAY);

	int rows = imagem.rows;
	int cols = imagem.cols;

	if (imagem.empty())
	{
		cout << "nao carregou a imagem!" << endl;
	}

	for (int i = 0; i < rows; i++)
	{
		Vec3b *ptr = imagem.ptr<Vec3b>(i);

		for (int j = 0; j < cols; j++)
		{
			Vec3b pixel = ptr[j];
		}
		cout << "\n";
	}

	imshow("Imagem com ruido", imagem);
	//waitKey(0);

	Mat imagem1 = imagem.clone();

	GaussianBlur(imagem, imagem1, Size(7, 7), 0, 0);
	for (int i = 0; i < rows; i++)
	{
		Vec3b *ptr = imagem1.ptr<Vec3b>(i);

		for (int j = 0; j < cols; j++)
		{
			Vec3b pixel = ptr[j];
		}
		cout << "\n";
	}

	//Mat imagem3;
	//cvtColor(imagem1, imagem3, COLOR_BGR2GRAY);

	//exibir PSNR
	double psnrRuido = PSNR(imagemSemRuidoGrey, imagem);
	cout << "PSNR na imagem com ruido: ";
	cout << psnrRuido << endl;

	double psnrMediana = PSNR(imagemSemRuidoGrey, imagem1);
	cout << "PSNR na imagem com filtro: ";
	cout << psnrMediana << endl;

	imshow("Filtro Wiener:", imagem1);
	waitKey(0);

	/* //tentativa 2
	

	cout << "\n == FILTRO DE WIENER (Ruido Gaussiano) == \n";
	cout << "- inserir nome da imagem: ";

	string nomeImagem;
	cin >> nomeImagem;

	int height, width, step, channels, depth;
	uchar* data1;

	CvMat *dft_A;
	CvMat *dft_B;

	CvMat *dft_C;
	IplImage* im;
	IplImage* im1;

	IplImage* image_ReB;
	IplImage* image_ImB;

	IplImage* image_ReC;
	IplImage* image_ImC;
	IplImage* complex_ImC;
	CvScalar val;

	IplImage* k_image_hdr;
	int i, j, k;

	FILE *fp;
	fp = fopen("test.txt", "w+");
	int dft_M, dft_N;
	int dft_M1, dft_N1;

	CvMat* cvShowDFT1(IplImage*, int, int, char*);
	void cvShowInvDFT1(IplImage*, CvMat*, int, int, char*);


	im = cvLoadImage("ruidos1/lena_gaussian.png", CV_LOAD_IMAGE_GRAYSCALE);
	//if (!im)
		//return -1;

	cvNamedWindow("Imagem original", 0);
	cvShowImage("Imagem original", im);

	IplImage* k_image;
	int rowLength = 11;
	long double kernels[11 * 11];
	CvMat kernel;
	int x, y;
	long double PI_F = 3.14159265358979;

	//long double SIGMA = 0.84089642;
	long double SIGMA = 0.014089642;
	//long double SIGMA = 0.00184089642;
	long double EPS = 2.718;
	long double numerator, denominator;
	long double value, value1;
	long double a, b, c, d;

	numerator = (pow((float)-3, 2) + pow((float)0, 2)) / (2 * pow((float)SIGMA, 2));
	denominator = sqrt((float)(2 * PI_F * pow(SIGMA, 2)));
	value = (pow((float)EPS, (float)-numerator)) / denominator;

	for (x = -5; x < 6; x++) {
		for (y = -5; y < 6; y++)
		{
			//numerator = (pow((float)x,2) + pow((float) y,2))/(2*pow((float)SIGMA,2));
			numerator = (pow((float)x, 2) + pow((float)y, 2)) / (2.0*pow(SIGMA, 2));
			denominator = sqrt((2.0 * 3.14159265358979 * pow(SIGMA, 2)));
			value = (pow(EPS, -numerator)) / denominator;
			//printf(" %1.8f ", value);
			kernels[x*rowLength + y + 55] = (float)value;

		}
		//printf("\n");
	}
	printf("---------------------------------\n");
	for (i = -5; i < 6; i++) {
		for (j = -5; j < 6; j++) {

			//printf(" %1.8f ", kernels[i*rowLength + j + 55]);
		}
		//printf("\n");
	}

	kernel = cvMat(rowLength, // number of rows
		rowLength, // number of columns
		CV_32FC1, // matrix data type
		&kernels);
	k_image_hdr = cvCreateImageHeader(cvSize(rowLength, rowLength), IPL_DEPTH_32F, 1);
	k_image = cvGetImage(&kernel, k_image_hdr);

	height = k_image->height;
	width = k_image->width;
	step = k_image->widthStep / sizeof(float);
	depth = k_image->depth;

	channels = k_image->nChannels;
	//data1 = (float *)(k_image->imageData);
	data1 = (uchar *)(k_image->imageData);
	//cvNamedWindow("blur kernel", 0);
	//cvShowImage("blur kernel", k_image);

	dft_M = cvGetOptimalDFTSize(im->height - 1);
	dft_N = cvGetOptimalDFTSize(im->width - 1);

	//dft_M1 = cvGetOptimalDFTSize( im->height+99 – 1 );
	//dft_N1 = cvGetOptimalDFTSize( im->width+99 – 1 );

	dft_M1 = cvGetOptimalDFTSize(im->height + 3 - 1);
	dft_N1 = cvGetOptimalDFTSize(im->width + 3 - 1);

	//printf("dft_N1=%d,dft_M1=%d\n", dft_N1, dft_M1);

	// Perform DFT of original image
	dft_A = cvShowDFT1(im, dft_M1, dft_N1, "original");
	//Perform inverse (check)
	//cvShowInvDFT1(im,dft_A,dft_M1,dft_N1, “original”); – Commented as it overwrites the DFT

	// Perform DFT of kernel
	dft_B = cvShowDFT1(k_image, dft_M1, dft_N1, "kernel");
	//Perform inverse of kernel (check)
	//cvShowInvDFT1(k_image,dft_B,dft_M1,dft_N1, “kernel”);- Commented as it overwrites the DFT

	// Multiply numerator with complex conjugate
	dft_C = cvCreateMat(dft_M1, dft_N1, CV_64FC2);

	//printf("%d %d %d %d\n", dft_M, dft_N, dft_M1, dft_N1);

	// Multiply DFT(blurred image) * complex conjugate of blur kernel
	cvMulSpectrums(dft_A, dft_B, dft_C, CV_DXT_MUL_CONJ);
	//cvShowInvDFT1(im,dft_C,dft_M1,dft_N1,”blur1?);

	// Split Fourier in real and imaginary parts
	image_ReC = cvCreateImage(cvSize(dft_N1, dft_M1), IPL_DEPTH_64F, 1);
	image_ImC = cvCreateImage(cvSize(dft_N1, dft_M1), IPL_DEPTH_64F, 1);
	complex_ImC = cvCreateImage(cvSize(dft_N1, dft_M1), IPL_DEPTH_64F, 2);

	//printf("%d %d %d %d\n", dft_M, dft_N, dft_M1, dft_N1);

	//cvSplit( dft_C, image_ReC, image_ImC, 0, 0 );
	cvSplit(dft_C, image_ReC, image_ImC, 0, 0);

	// Compute A^2 + B^2 of denominator or blur kernel
	image_ReB = cvCreateImage(cvSize(dft_N1, dft_M1), IPL_DEPTH_64F, 1);
	image_ImB = cvCreateImage(cvSize(dft_N1, dft_M1), IPL_DEPTH_64F, 1);

	// Split Real and imaginary parts
	cvSplit(dft_B, image_ReB, image_ImB, 0, 0);
	cvPow(image_ReB, image_ReB, 2.0);
	cvPow(image_ImB, image_ImB, 2.0);
	cvAdd(image_ReB, image_ImB, image_ReB, 0);

	val = cvScalarAll(kappa);
	cvAddS(image_ReB, val, image_ReB, 0);

	//Divide Numerator/A^2 + B^2
	cvDiv(image_ReC, image_ReB, image_ReC, 1.0);
	cvDiv(image_ImC, image_ReB, image_ImC, 1.0);

	// Merge Real and complex parts
	cvMerge(image_ReC, image_ImC, NULL, NULL, complex_ImC);

	// Perform Inverse
	cvShowInvDFT1(im, (CvMat *)complex_ImC, dft_M1, dft_N1, "Filtro Weiner");

	//return 0;
	waitKey(0);

	*/
}

void filtroMediana() {

	cout << "\n == FILTRO DA MEDIANA == \n";
	cout << "- inserir nome da imagem: ";

	string nomeImagem;
	cin >> nomeImagem;

	Mat imagemSemRuido = imread("ruidos1/" + nomeImagem + ".png", 1);
	Mat imagemSemRuidoGrey;
	cvtColor(imagemSemRuido, imagemSemRuidoGrey, COLOR_BGR2GRAY);
	imshow("Imagem original", imagemSemRuidoGrey);

	if (imagemSemRuido.empty())
	{
		cout << "nao carregou a imagem!" << endl;
	}

	Mat imagem = imread("ruidos1/" + nomeImagem + "_sal_e_pimenta.png", 1);
	Mat imagemOriginal;
	Mat imagem1;

	cvtColor(imagem, imagemOriginal, COLOR_BGR2GRAY);
	cvtColor(imagem, imagem1, COLOR_BGR2GRAY);
	imshow("Imagem com ruído", imagem1);
	uchar * data = imagem1.data;
	vector<int> valores;

	//gerar imagem pelo filtro da mediana
	for (auto index = 0; index < (imagem1.rows*imagem1.cols); index++)
	{
		for (size_t i = 0; i < 13; i++)
		{
			valores.push_back(data[index + i]);

		}
		std::sort(valores.begin(), valores.end());

		imagem1.data[index] = valores[6];
		valores.clear();
	}

	//exibir PSNR
	double psnrRuido = PSNR(imagemSemRuidoGrey, imagemOriginal);
	cout << "PSNR na imagem com ruido: ";
	cout << psnrRuido << endl;

	double psnrMediana = PSNR(imagemSemRuidoGrey, imagem1);
	cout << "PSNR na imagem com filtro: ";
	cout << psnrMediana << endl;
	

	imshow("Imagem filtro mediana", imagem1);
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
		system("cls");
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