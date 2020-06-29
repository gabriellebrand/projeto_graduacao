//
// Created by Gabrielle Brandenburg dos Anjos on 29/04/20.
//

#include "BaseOperator.h"

#include <iostream>
#include <sstream>

BaseOperator::BaseOperator(unsigned char *input_image, int *sorted_indices, int nx, int ny, int L, int K)
        :
        input_image(input_image),
        sorted_indices(sorted_indices),
        _nx(nx),
        _ny(ny),
        L(L),
        K(K)
{
    _nk = K + 1;
    num_pixels = _nx * _ny;

    /* Chain length images [k + nk * pixel_index].  These don't include the current pixel. */
    chain_image_up = new int[num_pixels];
    chain_image_down = new int[num_pixels];

    memset(opening_transform.data(), '\0', opening_transform.size() * sizeof(opening_transform[0]));

    output_image = new unsigned char[num_pixels];
}

BaseOperator::~BaseOperator()
{
    delete [] chain_image_up;
    delete [] chain_image_down;
    delete [] output_image;
}

unsigned int * BaseOperator::openingTransform(int threshold)
{
    return opening_transform[threshold] ? opening_transform[threshold] : nullptr;
}

void BaseOperator::dump()
{
    std::stringstream ss_chain_up, ss_chain_down, ss_max_length;

    ss_chain_up << "Chain Image Up:" << std::endl;
    ss_chain_down << "Chain Image Down:" << std::endl;
    ss_max_length << "Path Length:" << std::endl;

    for (int y = 0; y < _ny; ++y)
    {
        for (int x = 0; x < _nx; ++x)
        {
//            ss_chain_up << "[ ";
//            ss_chain_down << "[ ";
            int index = x + _nx * y;
            int maxPath = 0;
            for (int k = 0; k < _nk; ++k)
            {
                ss_chain_up << chain_image_up[k + _nk * index] << "\t";
                ss_chain_down << chain_image_down[k + _nk * index] << "\t";
                int sumPath = chain_image_up[k + _nk * index] + chain_image_down[k + _nk * index] + 1;
                if (sumPath > maxPath)
                    maxPath = sumPath;
            }
//            ss_chain_up << "]";
//            ss_chain_down << "]";
            ss_max_length << maxPath << "\t";
        }
        ss_chain_up << std::endl;
        ss_chain_down << std::endl;
        ss_max_length << std::endl;
    }
    std::cout << ss_chain_up.str() << std::endl;
    std::cout << ss_chain_down.str() << std::endl;
    std::cout << ss_max_length.str() << std::endl;
}