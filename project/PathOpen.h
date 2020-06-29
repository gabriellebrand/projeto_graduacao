//
// Created by Gabrielle Brandenburg dos Anjos on 07/04/20.
//
#ifndef PROJECT_PATHOPEN_H
#define PROJECT_PATHOPEN_H

#include <array>
#include <memory>
#include <utility>
#include <vector>
#include "Adjacency.h"

class BaseOperator;

class PathOpen {

public:
    PathOpen(unsigned char * input_image, int nx, int ny, int L, int K, Adjacency flags);

    ~PathOpen();

    void opening();

    const unsigned int* getOpeningTransform(int threshold);

    const unsigned int* getFilteredOpTransf(int threshold);

    const int nx();

    const int ny();

private:
    unsigned char * input_image; /* The input image */
    int * sorted_indices;		 /* Monotonic transform to [0, 1, ...] of input image */
    int _nx; int _ny;			 /* Image dimensions */
    int L;						 /* The threshold line length */
    int K;						 /* The maximum gap number */
    int nk;
    int num_pixels;
    Adjacency adjacency_flags;
    unsigned int* opening_transform;

    std::vector<BaseOperator*> operators;
};



#endif //PROJECT_PATHOPEN_H
