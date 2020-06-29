//
// Created by Gabrielle Brandenburg dos Anjos on 29/04/20.
//

#ifndef PROJECT_BASEOPERATOR_H
#define PROJECT_BASEOPERATOR_H

#include <array>

class BaseOperator {
public:
    BaseOperator(unsigned char * input_image, int * sorted_indices, int nx, int ny, int L, int K);

    virtual ~BaseOperator();

    virtual void pathOpen() = 0;

    /*! openingTransform
     *
     * @param threshold - threshold level [0,maxVal(input_image)]
     * @return - the opening transform for the given threshold level
     *         - nullptr, if threshold is out of valid range
     */
    virtual unsigned int* openingTransform(int threshold);

    void dump();

protected:
    virtual void initChainLength() = 0;

    unsigned char * input_image;    /* The input image */
    int * sorted_indices;           /* Monotonic transform to [0, 1, ...] of input image */
    int _nx; int _ny;               /* Image dimensions */
    int L;                          /* The threshold line length */
    int K;                          /* The maximum gap number */
    int _nk;                        /* Number of possible paths (K+1) */
    int * chain_image_up;           /* Max upward path */
    int * chain_image_down;         /* Max downward path */
    int num_pixels;                 /* ny x nx */
    unsigned char * output_image;
    std::array<unsigned int*, 256> opening_transform;
};


#endif //PROJECT_BASEOPERATOR_H
