//
// Created by Gabrielle Brandenburg dos Anjos on 30/04/20.
//

#include "HorizontalOperator.h"
#include "../Utils.h"

HorizontalOperator::HorizontalOperator(unsigned char * input_image, int * sorted_indices, int nx, int ny, int L, int K)
        :VerticalOperator(input_image, sorted_indices, ny, nx, L, K)
{
    //Transpõe imagem para o vertical path funcionar como horizontal.
    this->input_image = utils::transposeImage<unsigned char>(input_image, nx, ny);
    this->sorted_indices = utils::transposeIndices(sorted_indices, nx, ny);
}

HorizontalOperator::~HorizontalOperator()
{
    delete [] input_image;
    delete [] sorted_indices;
}

unsigned int * HorizontalOperator::openingTransform(int threshold)
{
    if (!opening_transform[threshold]) return nullptr;

    /*FIXME: problema - aloca e copia resultado da transformada
     * para novo endereço de memoria (esse ponteiro não é liberado
     * no destrutor dessa classe) */
    return utils::transposeImage<unsigned int>(opening_transform[threshold], _nx, _ny);
}