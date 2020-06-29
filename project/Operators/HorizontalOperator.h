//
// Created by Gabrielle Brandenburg dos Anjos on 30/04/20.
//
/* A path opening in using the  \|/ adjacency relation.
* [5]  [6]  [7]
*     \
* [8]--[p]  [4]
*     /
* [1]  [2]  [3]
*/
#ifndef PROJECT_HORIZONTALOPERATOR_H
#define PROJECT_HORIZONTALOPERATOR_H

#include "VerticalOperator.h"

class HorizontalOperator: public VerticalOperator {
public:
    HorizontalOperator(unsigned char * input_image, int * sorted_indices, int nx, int ny, int L, int K);

    ~HorizontalOperator();

    unsigned int* openingTransform(int threshold) override;
};


#endif //PROJECT_HORIZONTALOPERATOR_H
