//
// Created by Gabrielle Brandenburg dos Anjos on 30/04/20.
//
/* A path opening in using the  \|/ adjacency relation.
* [5]  [6]  [7]
*    \  |  /
* [8]  [p]  [4]
*
* [1]  [2]  [3]
*/
#ifndef PROJECT_VERTICALOPERATOR_H
#define PROJECT_VERTICALOPERATOR_H

#include "BaseOperator.h"


class VerticalOperator: public BaseOperator {
public:
    VerticalOperator(unsigned char * input_image, int * sorted_indices, int nx, int ny, int L, int K);

    ~VerticalOperator() = default;

    void pathOpen() override;

private:
    void initChainLength() override;
};


#endif //PROJECT_VERTICALOPERATOR_H
