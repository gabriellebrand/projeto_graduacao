//
// Created by Gabrielle Brandenburg dos Anjos on 01/05/20.
//

#include "DiagonalRightOperator.h"
#include "../Utils.h"

DiagonalRightOperator::DiagonalRightOperator(unsigned char * input_image, int * sorted_indices, int nx, int ny, int L, int K)
        :DiagonalLeftOperator(input_image, sorted_indices, nx, ny, L, K)
{
    //Flipa imagem para o diagonal left path funcionar como diagonal right.
    this->input_image = utils::flipImageVertical<unsigned char>(input_image, nx, ny);
    this->sorted_indices = utils::flipIndicesVertical(sorted_indices, nx, ny);
}

DiagonalRightOperator::~DiagonalRightOperator()
{
    delete [] input_image;
    delete [] sorted_indices;
}

unsigned int * DiagonalRightOperator::openingTransform(int threshold)
{
    if (!opening_transform[threshold]) return nullptr;

    /*FIXME: problema - aloca e copia resultado da transformada
     * para novo endereço de memoria (esse ponteiro não é liberado
     * no destrutor dessa classe) */
    return utils::flipImageVertical<unsigned int>(opening_transform[threshold], _nx, _ny);
}