//
// Created by Gabrielle Brandenburg dos Anjos on 29/04/20.
//
/* A path opening in using the  _\|/ adjacency relation.
 * [5]  [6]  [7]
 *    \  |  /
 * [8]--[p]  [4]
 *
 * [1]  [2]  [3]
 */

#ifndef PROJECT_FRACTUREOPERATOR_H
#define PROJECT_FRACTUREOPERATOR_H


#include "BaseOperator.h"

class FractureOperator: public BaseOperator {
public:
    FractureOperator(unsigned char * input_image, int * sorted_indices, int nx, int ny, int L, int K);

    ~FractureOperator() = default;

    void pathOpen() override;

private:
    void initChainLength() override;
};


#endif //PROJECT_FRACTUREOPERATOR_H
