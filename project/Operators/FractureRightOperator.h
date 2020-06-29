//
// Created by Gabrielle Brandenburg dos Anjos on 07/05/20.
//
/* A path opening in using the  _\|/ adjacency relation.
 * [5]  [6]  [7]
 *    \  |  /
 * [8]  [p] -- [4]
 *
 * [1]  [2]  [3]
 */

#ifndef PROJECT_FRACTURERIGHTOPERATOR_H
#define PROJECT_FRACTURERIGHTOPERATOR_H

#include "FractureOperator.h"

class FractureRightOperator: public FractureOperator {
public:
    FractureRightOperator(unsigned char * input_image, int * sorted_indices, int nx, int ny, int L, int K);

    ~FractureRightOperator();

    unsigned int* openingTransform(int threshold) override;
};


#endif //PROJECT_FRACTURERIGHTOPERATOR_H
