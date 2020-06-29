#include <iostream>
#include <ctime>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv/cv.hpp>

#include "PathOpen.h"

extern "C" {
#include "path_support.h"
}

//void test_pathopen(cv::Mat image)
//{
//    int L = 70, K = 1;
//    int ny = image.rows;
//    int nx = image.cols;
//
//    PATHOPEN_PIX_TYPE * input_image = image.data;
//    auto * output_image = new PATHOPEN_PIX_TYPE[nx * ny];
//
//    std::cout << "Starting path opening..." << std::endl;
//    auto start = std::clock();
//    pathopen(
//            input_image, /* The input image */
//            nx, ny,	 /* Image dimensions */
//            L,		 /* The threshold line length */
//            K,		 /* The maximum number of gaps in the path */
//            output_image /* Output image */
//    );
//    auto stop = std::clock();
//    std::cout << "pathopen() returned! CPU time elapsed:" << ((double)stop-start)/CLOCKS_PER_SEC << std::endl;
//
//    cv::Mat output(ny,nx, CV_8UC1, (void*)output_image);
//
//    cv::imshow("input", image);
//    cv::imshow("result", output);
//    cv::waitKey(0);
//
//    delete[] output_image;
//}

template<typename T>
void print_arr(const T& arr, size_t nx, size_t ny)
{
    std::cout << std::endl;
    for (int y = 0; y < ny; ++y)
    {
        for (int x = 0; x < nx; ++x)
            std::cout << (int) arr[y * nx + x] << "\t";
        std::cout << std::endl;
    }
}


void write_output_data(PathOpen& po, std::string id_name)
{
    /* convert data to unsigned short to write file with opencv*/
    int size = po.ny()*po.nx();
    auto* opening_transform = po.getFilteredOpTransf(0);
    unsigned short * data_ushort = new unsigned short[size];
    for (int i = 0; i < size; i++)
        data_ushort[i] = opening_transform[i];

    cv::Mat op_transform(po.ny(),po.nx(), CV_16UC1, data_ushort);
//    print_arr(op_transform.data, po.nx(), po.ny());

    std::string filename = "../../results/opening_transform_" + id_name + ".tif";
    cv::imwrite(filename, op_transform);
}

/* Normaliza valores de um array para o range de 0-255 */
template<typename T>
unsigned char* normalize_to_8bit(const T* arr, size_t length)
{
    T maxVal = 0;
    for (int i = 0; i < length; i++)
        if (maxVal < arr[i])
            maxVal = arr[i];

    unsigned char *out = new unsigned char[length];

    for(int i = 0; i < length; i++)
    {
        float val = (((float)arr[i])/maxVal)*255;
        out[i] = (unsigned char) val;
    }

    return out;
}

int main()
{

    std::string img_name = "anp_1_32k_40k";
    std::string suffix = "_horizontal";

    std::string img_path = "../../images/" + img_name + ".tif";

    cv::Mat image, image_inv;
    image = cv::imread(img_path, cv::IMREAD_GRAYSCALE);

    int nx = image.cols;
    int ny = image.rows;
    int L = 100, K = 0;

    Adjacency dilation = Adjacency::Horizontal;

    auto pathopen = new PathOpen(image.data, nx, ny, L, K, dilation);

    std::cout << "Starting path opening..." << std::endl;
    auto start = std::clock();
    pathopen->opening();
    auto stop = std::clock();
    std::cout << "pathopen() returned! CPU time elapsed:" << ((double)stop-start)/CLOCKS_PER_SEC << std::endl;

    auto result = pathopen->getFilteredOpTransf(0);
//    auto result = pathopen->getOpeningTransform(0);

    write_output_data(*pathopen, img_name+suffix);

    std::cout << std::endl << std::endl;

    auto output_data = normalize_to_8bit(result, nx*ny);
    cv::Mat output(ny,nx, CV_8UC1, (void*)output_data);
    cv::applyColorMap(output, output, cv::COLORMAP_HOT);

    cv::imshow("input", image);
    cv::imshow("result", output);
    cv::waitKey(0);

    delete pathopen;

    return 0;
}
