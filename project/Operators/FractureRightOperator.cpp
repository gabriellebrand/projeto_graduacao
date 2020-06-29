//
// Created by Gabrielle Brandenburg dos Anjos on 07/05/20.
//

#include "FractureRightOperator.h"
#include "../Utils.h"

FractureRightOperator::FractureRightOperator(unsigned char * input_image, int * sorted_indices, int nx, int ny, int L, int K)
        :FractureOperator(input_image, sorted_indices, nx, ny, L, K)
{
    //Flipa imagem para funcionar como \|/_
    this->input_image = utils::flipImageHorizontal(input_image, nx, ny);
    this->sorted_indices = utils::flipIndicesHorizontal(sorted_indices, nx, ny);
}

FractureRightOperator::~FractureRightOperator()
{
    delete [] input_image;
    delete [] sorted_indices;
}

unsigned int* FractureRightOperator::openingTransform(int threshold)
{
    if (!opening_transform[threshold]) return nullptr;

    return utils::flipImageHorizontal<unsigned int>(opening_transform[threshold], _nx, _ny);
}