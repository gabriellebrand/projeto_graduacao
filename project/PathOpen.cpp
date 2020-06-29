//
// Created by Gabrielle Brandenburg dos Anjos on 07/04/20.
//

//#define DEBUGGING
#define PATHOPEN_LENGTH_HEURISTIC
#define CENTRE_PIXEL_FIX

#include "PathOpen.h"
#include "Operators/FractureOperator.h"
#include "Operators/FractureRightOperator.h"
#include "Operators/VerticalOperator.h"
#include "Operators/HorizontalOperator.h"
#include "Operators/DiagonalLeftOperator.h"
#include "Operators/DiagonalRightOperator.h"
#include "Utils.h"

PathOpen::PathOpen(unsigned char *input_image, int nx, int ny, int L, int K, Adjacency flags)
    :
    input_image(input_image),
    _nx(nx),
    _ny(ny),
    L(L),
    K(K),
    nk(K + 1),
    num_pixels(nx*ny),
    adjacency_flags(flags)
{
    opening_transform = new unsigned int[num_pixels];
    sorted_indices = utils::imageSort(input_image, nx*ny);

    if (adjacency_flags & Adjacency::FractureLeft)
        operators.push_back(new FractureOperator(input_image, sorted_indices, nx, ny, L, K));
    if (adjacency_flags & Adjacency::FractureRight)
        operators.push_back(new FractureRightOperator(input_image, sorted_indices, nx, ny, L, K));
    if (adjacency_flags & Adjacency::Vertical)
        operators.push_back(new VerticalOperator(input_image, sorted_indices, nx, ny, L, K));
    if (adjacency_flags & Adjacency::Horizontal)
        operators.push_back(new HorizontalOperator(input_image, sorted_indices, nx, ny, L, K));
    if (adjacency_flags & Adjacency::DiagonalLeft)
        operators.push_back(new DiagonalLeftOperator(input_image, sorted_indices, nx, ny, L, K));
    if (adjacency_flags & Adjacency::DiagonalRight)
        operators.push_back(new DiagonalRightOperator(input_image, sorted_indices, nx, ny, L, K));
}

PathOpen::~PathOpen()
{
    for(auto* op : operators)
        if (op) delete op;
}

void PathOpen::opening()
{
    for (auto* op : operators)
        op->pathOpen();
}

const unsigned int* PathOpen::getOpeningTransform(int threshold)
{
    memset(opening_transform, 0, sizeof(unsigned int)*num_pixels);
    for (auto* op : operators)
    {
        const auto* result = op->openingTransform(threshold);
        for (int i = 0; i < num_pixels; i++)
            opening_transform[i] = std::max(opening_transform[i], result[i]);
    }

    return opening_transform;
}

const unsigned int* PathOpen::getFilteredOpTransf(int threshold)
{
    memset(opening_transform, 0, sizeof(unsigned int)*num_pixels);
    for (auto* op : operators)
    {
        const auto* result = op->openingTransform(threshold);
        for (int i = 0; i < num_pixels; i++)
            opening_transform[i] = input_image[i] > 0 ? std::max(opening_transform[i], result[i]) : 0;
    }

    return opening_transform;
}

const int PathOpen::nx()
{
    return _nx;
}

const int PathOpen::ny()
{
    return _ny;
}
