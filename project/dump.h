//
// Created by Gabrielle Brandenburg dos Anjos on 29/04/20.
//

#ifndef PROJECT_DUMP_H
#define PROJECT_DUMP_H

#include "path_queue.h"
#include <iostream>

using namespace std;

/* dump_state:
	Nifty debugging function
	- dump the state of the algorithm to the terminal for verification.
*/
inline void dumpState(
        int nx,									// Image dimensions etc.
        int ny,
        int nk,
        Path_Queue & path_queue_up,				// Queueing structures
        Path_Queue & path_queue_down,
        char * in_queue_up,
        char * in_queue_down,
        char * bin_input_image,					// Input/output binary images
        char * bin_output_image_array,
        char * bin_output_image_count,
        int * chain_image_up,					// Up/down chain lengths
        int * chain_image_down
)
{
    int x, y, k;

    cout << endl << "STATE DUMP" << endl;

    // Observe the input binary image
    cout << "bin_input_image" << endl;
    for (y = 0; y < ny; ++y) {
        cout << "\t" << y << ":\t";
        for (x = 0; x < nx; ++x) {
            cout << (int)bin_input_image[x + nx * y] << "\t";
        }
        cout << endl;
    }
    // Observe the bin_output_image_array
    cout << "bin_output_image_array" << endl;
    for (k = 0; k < nk; ++k) {
        cout << "k = " << k << endl;
        for (y = 0; y < ny; ++y) {
            cout << "\t" << y << ":\t";
            for (x = 0; x < nx; ++x) {
                cout << (int)bin_output_image_array[k + nk * (x + nx * y)] << "\t";
            }
            cout << endl;
        }
    }
    // Observe the bin_output_image_count
    cout << "bin_output_image_count" << endl;
    for (y = 0; y < ny; ++y) {
        cout << "\t" << y << ":\t";
        for (x = 0; x < nx; ++x) {
            cout << (int)bin_output_image_count[x + nx * y] << "\t";
        }
        cout << endl;
    }

    // Observe the chain images
    // Down
    cout << "chain_image_down:" << endl;
    for (k = 0; k < nk; ++k) {
        cout << "k = " << k << endl;
        for (y = 0; y < ny; ++y) {
            cout << "\t" << y << ":\t";
            for (x = 0; x < nx; ++x) {
                cout << chain_image_down[k + nk * (x + nx * y)] << "\t";
            }
            cout << endl;
        }
    }
    // Up
    cout << "chain_image_up:" << endl;
    for (k = 0; k < nk; ++k) {
        cout << "k = " << k << endl;
        for (y = 0; y < ny; ++y) {
            cout << "\t" << y << ":\t";
            for (x = 0; x < nx; ++x) {
                cout << chain_image_up[k + nk * (x + nx * y)] << "\t";
            }
            cout << endl;
        }
    }

    // Up
    cout << "L:" << endl;
    for (k = 0; k < nk; ++k) {
        cout << "k = " << k << endl;
        for (y = 0; y < ny; ++y) {
            cout << "\t" << y << ":\t";
            for (x = 0; x < nx; ++x) {
                cout << chain_image_down[k + nk * (x + nx * y)] + chain_image_up[k + nk * (x + nx * y)] + 1 << "\t";
            }
            cout << endl;
        }
    }

    // Observe the path queues
    /*
    cout << "path_queue_down:" << endl;
    for (k = 0; k < nk; ++k) {
        cout << "k = " << k << endl;
        for (y = 0; y < ny; ++y) {
            vector<PIXEL_INDEX_TYPE> & q = path_queue_down.q[k][y];
            if (q.size() == 0) continue;

            cout << "\t" << y << ":";
            for (i = 0; i < q.size(); ++i) {
                cout <<	q[i] << " ";
            }
            cout << endl;
        }
    }
    */
    cout << "in_queue_down" << endl;
    for (k = 0; k < nk; ++k) {
        cout << "k = " << k << endl;
        for (y = 0; y < ny; ++y) {
            cout << "\t" << y << ":\t";
            for (x = 0; x < nx; ++x) {
                cout << (int)in_queue_down[k + nk * (x + nx * y)] << "\t";
            }
            cout << endl;
        }
    }
    /*
    cout << "path_queue_up:" << endl;
    for (k = 0; k < nk; ++k) {
        cout << "k = " << k << endl;
        for (y = 0; y < ny; ++y) {
            vector<PIXEL_INDEX_TYPE> & q = path_queue_up.q[k][y];
            if (q.size() == 0) continue;

            cout << "\t" << y << ":";
            for (i = 0; i < q.size(); ++i) {
                cout <<	q[i] << " ";
            }
            cout << endl;
        }
    }
    */
    cout << "in_queue_up" << endl;
    for (k = 0; k < nk; ++k) {
        cout << "k = " << k << endl;
        for (y = 0; y < ny; ++y) {
            cout << "\t" << y << ":\t";
            for (x = 0; x < nx; ++x) {
                cout << (int)in_queue_up[k + nk * (x + nx * y)] << "\t";
            }
            cout << endl;
        }
    }

    /*
    cout << "getch()" << endl;
    getch();
    */
}

#endif //PROJECT_DUMP_H
