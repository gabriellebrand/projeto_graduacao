//
// Created by Gabrielle Brandenburg dos Anjos on 29/04/20.
//

#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H

#include <opencv2/core.hpp>

extern "C" {
#include "path_support.h"
}

namespace utils
{
    /*! flipImageHorizontal - flip along horizontal axis
     *
     * @param input_image - original image
     * @param nx          - number of columns
     * @param ny          - number of rows
     * @return            - new allocated array with flipped image
     */
    template <typename T>
    inline T * flipImageHorizontal(T * input_image, int nx, int ny)
    {
        size_t size = ny*nx;
        //Aloca novo espaço de memória para não sobreescrever o endereço da imagem original
        T* input_flipped = new T[size];
        int x, y, new_index;
        for(int i = 0; i < size; ++i)
        {
            x = i % nx;
            y = i / nx;
            new_index = (nx - 1 - x) + nx*y;
            input_flipped[new_index] = input_image[i];
        }

        return input_flipped;
    }

    /*! flipImageHorizontal - flip along vertical axis
     *
     * @param input_image - original image
     * @param nx          - number of columns
     * @param ny          - number of rows
     * @return            - new allocated array with flipped image
     */
    template <typename T>
    inline T * flipImageVertical(T * input_image, int nx, int ny)
    {
        size_t size = ny*nx;
        //Aloca novo espaço de memória para não sobreescrever o endereço da imagem original
        T* flipped = new T[size];

        flip_image(input_image, nx, ny, sizeof(T), flipped);

        return flipped;
    }

    /*! transposeImage
     *
     * @param input_image - original image
     * @param nx          - number of columns
     * @param ny          - number of rows
     * @return            - new allocated array with transposed image
     */
    template <typename T>
    inline T * transposeImage(T * input_image, int nx, int ny)
    {
        size_t size = ny*nx;
        //Aloca novo espaço de memória para não sobreescrever o endereço da imagem original
        T* transposed = new T[size];

        transpose_image(input_image, nx, ny, sizeof(T), transposed);

        return transposed;
    }

    /*! imageSort - Sort an image by its pixel values (monotonic transform to 0, 1, ...)
     *
     * @param input_image
     * @param num_pixels
     * @return new allocated array of same size as input image with sorted indices
     */
    inline int * imageSort(GPOT_PIX_TYPE * input_image, int num_pixels)
    {
        int * sorted_indices = new int [num_pixels];
        image_sort(input_image, num_pixels, sorted_indices);
        return sorted_indices;
    }

    inline int * flipIndicesHorizontal(int * input_indices, int nx, int ny)
    {
        int * output_indices = new int[nx*ny];
        flip_indices_horizontal(input_indices, nx, ny, output_indices);
        return output_indices;
    }

    inline int * flipIndicesVertical(int * input_indices, int nx, int ny)
    {
        int * output_indices = new int[nx*ny];
        flip_indices(input_indices, nx, ny, output_indices);
        return output_indices;
    }

    inline int * transposeIndices(int * input_indices, int nx, int ny)
    {
        int * output_indices = new int[nx*ny];
        transpose_indices(input_indices, nx, ny, output_indices);
        return output_indices;
    }

}

#endif //PROJECT_UTILS_H
