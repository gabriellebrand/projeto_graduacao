//
// Created by Gabrielle Brandenburg dos Anjos on 06/05/20.
//

#ifndef PROJECT_ADJACENCY_H
#define PROJECT_ADJACENCY_H

enum Adjacency {
    Vertical = 0x1,
    Horizontal = 0x2,
    DiagonalLeft = 0x4,
    DiagonalRight = 0x8,
    FractureLeft = 0x10,
    FractureRight = 0x20
};

inline Adjacency operator|(Adjacency a, Adjacency b)
{
    return static_cast<Adjacency>(static_cast<int>(a) | static_cast<int>(b));
}

inline Adjacency operator&(Adjacency a, Adjacency b)
{
    return static_cast<Adjacency>(static_cast<int>(a) & static_cast<int>(b));
}

#endif //PROJECT_ADJACENCY_H
