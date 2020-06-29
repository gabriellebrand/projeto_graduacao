//
// Created by Gabrielle Brandenburg dos Anjos on 01/05/20.
//

#ifndef PROJECT_DIAGONALRIGHTOPERATOR_H
#define PROJECT_DIAGONALRIGHTOPERATOR_H

#include "DiagonalLeftOperator.h"

class DiagonalRightOperator: public DiagonalLeftOperator {
public:
    DiagonalRightOperator(unsigned char * input_image, int * sorted_indices, int nx, int ny, int L, int K);

    ~DiagonalRightOperator();

    unsigned int* openingTransform(int threshold) override;
};


#endif //PROJECT_DIAGONALRIGHTOPERATOR_H
