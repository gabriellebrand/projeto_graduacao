//
// Created by Gabrielle Brandenburg dos Anjos on 01/05/20.
//
/* A path opening in using the  \|/ adjacency relation.
* [5]  [6]  [7]
*    \  |
* [8]--[p]  [4]
*
* [1]  [2]  [3]
*/
#ifndef PROJECT_DIAGONALLEFTOPERATOR_H
#define PROJECT_DIAGONALLEFTOPERATOR_H

#include "BaseOperator.h"

class DiagonalLeftOperator: public BaseOperator {
public:
    DiagonalLeftOperator(unsigned char * input_image, int * sorted_indices, int nx, int ny, int L, int K);

    ~DiagonalLeftOperator() = default;

    void pathOpen() override;

private:
    void initChainLength() override;
};


#endif //PROJECT_DIAGONALLEFTOPERATOR_H
