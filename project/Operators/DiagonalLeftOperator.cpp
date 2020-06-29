//
// Created by Gabrielle Brandenburg dos Anjos on 01/05/20.
//

#include "DiagonalLeftOperator.h"


#include "../path_queue.h"
#include "../Utils.h"

//#define DEBUGGING
#define PATHOPEN_LENGTH_HEURISTIC
#define CENTRE_PIXEL_FIX

#ifdef DEBUGGING
#include "dump.h" //TODO: colocar dump dentro da classe base
#endif

DiagonalLeftOperator::DiagonalLeftOperator(unsigned char * input_image, int * sorted_indices, int nx, int ny, int L, int K)
        :BaseOperator(input_image, sorted_indices, nx, ny, L, K)
{
}

void DiagonalLeftOperator::initChainLength()
{
    /* Initialise the chain lengths */
    for (int y = 0; y < _ny; ++y) {
        for (int x = 0; x < _nx; ++x) {
            int index = x + _nx * y;
            int up_length = x + y;
            int down_length = ((_nx - 1) - x) + ((_ny - 1) - y);

#ifdef PATHOPEN_LENGTH_HEURISTIC
            /* Threshold at L - 1 */
            if (up_length > L - 1) up_length = L - 1;
            if (down_length > L - 1) down_length = L - 1;
#endif

            for (int k = 0; k < _nk; ++k)
                chain_image_up[k + _nk * index] = up_length;
            for (int k = 0; k < _nk; ++k)
                chain_image_down[k + _nk * index] = down_length;
        }
    }
}


void DiagonalLeftOperator::pathOpen()
{
    int i, k, x, y, index, new_index, sort_index;

    /************************************** Allocation **********************************************/

    /* Construct queueing system */ //NOTA: o valor de cada elemento da queue é somente a coordenada x do pixel
    Path_Queue path_queue_up(_nk, _ny, _nx);
    Path_Queue path_queue_down(_nk, _ny, _nx);

    /* Dynamic binary input image */
    char * bin_input_image = (char *)malloc(num_pixels * sizeof(char));

    /* in_queue flags */
    char * in_queue_up = (char *)malloc(num_pixels * _nk * sizeof(char));
    char * in_queue_down = (char *)malloc(num_pixels * _nk * sizeof(char));

    // At each pixel, we store the vector of binary outputs indexed by gap number of upward chain
    char * bin_output_image_array = (char *)malloc(num_pixels * _nk * sizeof(char));
    // Also count the vector of binary outputs, to note when they are all extinguished (boolean PQ!)
    char * bin_output_image_count = (char *)malloc(num_pixels * sizeof(char));

    /************************************** Initialisation **********************************************/

    initChainLength();

    /* Dynamic binary threshold image is initially all 1's */
    memset(bin_input_image, 1, num_pixels * sizeof(char));

    /* Queue initially empty */
    memset(in_queue_up, 0, num_pixels * _nk * sizeof(char));
    memset(in_queue_down, 0, num_pixels * _nk * sizeof(char));

    /* Binary output vector at each pixel */
    memset(bin_output_image_array, 1, num_pixels * _nk * sizeof(char));
    memset(bin_output_image_count, _nk, num_pixels * sizeof(char));

    /* Set output image to default value (0 here) */
    memset(output_image, 0, num_pixels * sizeof(PATHOPEN_PIX_TYPE));
    /****************************************************************************************************/

    /* For each threshold from smallest to largest */
    sort_index = 0;
    while(sort_index < num_pixels) {
        PATHOPEN_PIX_TYPE threshold;

        /*********************************** Process threshold pixels *****************************************/
        /* NOTA: aloca novo mapa de max path length para o novo valor de threshold */
        threshold = input_image[sorted_indices[sort_index]];
        if (!opening_transform[threshold])
        {
            opening_transform[threshold] = new unsigned int[num_pixels];
        }

#ifdef DEBUGGING
        cout << "Threshold = " << (int)threshold << endl;
#endif
        while(input_image[sorted_indices[sort_index]] == threshold) {
            /* Collect into rows for enqueueing */
            vector< vector<PIXEL_INDEX_TYPE> > new_row_queue_down(_nk);
            vector< vector<PIXEL_INDEX_TYPE> > new_row_queue_right(_nk);
            vector< vector<PIXEL_INDEX_TYPE> > new_row_queue_up(_nk);
            vector< vector<PIXEL_INDEX_TYPE> > new_row_queue_left(_nk);
            int row_y = sorted_indices[sort_index] / _nx;
#ifdef DEBUGGING
            cout << "y = " << row_y << endl;
#endif

            while(input_image[sorted_indices[sort_index]] == threshold
                  && sorted_indices[sort_index] / _nx == row_y) {
                /* Extract index and coordinates */
                index = sorted_indices[sort_index];
                y = index / _nx;
                x = index % _nx;

#ifdef DEBUGGING
                cout << "\tx = " << x << endl;
#endif

                /* Directly perform the changes to this threshold pixel */
                if (bin_input_image[index]) {
#ifdef DEBUGGING
                    cout << "\tRemoving..." << endl;
					cout << "\tcount = " << (int)bin_output_image_count[index] << endl;
#endif
                    // Remove this pixel from the binary input image
                    bin_input_image[index] = 0;

#ifndef CENTRE_PIXEL_FIX
                    // Update the outputs
                    if (bin_output_image_count[index] > 0) {
                        // Update the output flags for each gap index (and count)
                        bin_output_image_count[index] = 0;
                        for (k = 0; k < K; ++k) {
                            bin_output_image_array[k + _nk * index] =
                                    (chain_image_up[k + _nk * index] + chain_image_down[(K - 1 - k) + _nk * index] + 1) >= L;
                            bin_output_image_count[index] += bin_output_image_array[k + _nk * index];
                        }

                        // If all paths have been extinguished, update output
                        if (bin_output_image_count[index] == 0) {
                            output_image[index] = threshold;
                        }
                    }
#else
                    // Shut down this pixel
                    // If all paths have been extinguished, update output  //NOTA: ????
                    if (bin_output_image_count[index] > 0) {
                        bin_output_image_count[index] = 0;
                        output_image[index] = threshold;
                    }
#endif // CENTRE_PIXEL_FIX

                    /* Enqueue downward pixels for update */
                    if (y < _ny - 1) {
                        new_index = index + _nx; //pixel de baixo
                        // Enqueue this pixel for all k
                        for (k = 0; k < _nk; ++k) {
                            if (!in_queue_down[k + _nk * new_index]) {
                                in_queue_down[k + _nk * new_index] = 1;
                                new_row_queue_down[k].push_back(x);
                            }
                        }
                        if (x < _nx - 1) {
                            new_index = index + _nx + 1;

                            // Enqueue this pixel for all k
                            for (k = 0; k < _nk; ++k) {
                                if (!in_queue_down[k + _nk * new_index]) {
                                    in_queue_down[k + _nk * new_index] = 1;
                                    new_row_queue_down[k].push_back(x + 1);
                                }
                            }
                        }
                    }
                    if (x < _nx - 1) {
                        new_index = index + 1;
                        // Enqueue this pixel for all k
                        for (k = 0; k < _nk; ++k) {
                            if (!in_queue_down[k + _nk * new_index]) {
                                in_queue_down[k + _nk * new_index] = 1;
                                new_row_queue_right[k].push_back(x + 1);
                            }
                        }
                    }

                    /* Enqueue upward pixels for update */
                    if (y > 0) {
                        if (x > 0) {
                            new_index = index - _nx - 1;

                            // Enqueue this pixel for all k
                            for (k = 0; k < _nk; ++k) {
                                if (!in_queue_up[k + _nk * new_index]) {
                                    in_queue_up[k + _nk * new_index] = 1;
                                    new_row_queue_up[k].push_back(x - 1);
                                }
                            }
                        }
                        new_index = index - _nx;
                        // Enqueue this pixel for all k
                        for (k = 0; k < _nk; ++k) {
                            if (!in_queue_up[k + _nk * new_index]) {
                                in_queue_up[k + _nk * new_index] = 1;
                                new_row_queue_up[k].push_back(x);
                            }
                        }
                    }
                    if (x > 0) {
                        new_index = index - 1;

                        // Enqueue this pixel for all k
                        for (k = 0; k < _nk; ++k) {
                            if (!in_queue_up[k + _nk * new_index]) {
                                in_queue_up[k + _nk * new_index] = 1;
                                new_row_queue_left[k].push_back(x - 1);
                            }
                        }
                    }
                }

                // Next pixel in row
                ++sort_index;
                if (sort_index >= num_pixels) break;
            }
#ifdef DEBUGGING
            cout << "Merging row queues" << endl;
#endif

            // Merge row queues into path queues
#ifdef DEBUGGING
            cout << "Down queues" << endl;
#endif
            // Down
            if (row_y + 1 < _ny) {
                for (k = 0; k < _nk; ++k) {
                    if (new_row_queue_down[k].size() > 0) {
#ifdef DEBUGGING
                        cout << "k = " << k << ":";
						for (i = 0; i < new_row_queue_down[k].size(); ++i) {
							cout << new_row_queue_down[k][i] << " ";
						}
						cout << endl;
#endif
                        path_queue_down.merge_row(new_row_queue_down[k], k, row_y + 1);
                    }
                }
            }
            // Right
            for (k = 0; k < _nk; ++k) {
                if (new_row_queue_right[k].size() > 0) {
#ifdef DEBUGGING
                    cout << "k = " << k << ":";
				for (i = 0; i < new_row_queue_right[k].size(); ++i) {
					cout << new_row_queue_right[k][i] << " ";
				}
				cout << endl;
#endif
                    path_queue_down.merge_row(new_row_queue_right[k], k, row_y);
                }
            }


#ifdef DEBUGGING
            cout << "Up queues" << endl;
#endif
            // Up
            if (row_y - 1 >= 0) {
                for (k = 0; k < _nk; ++k) {
                    if (new_row_queue_up[k].size() > 0) {
#ifdef DEBUGGING
                        cout << "k = " << k << ":";
						for (i = 0; i < new_row_queue_up[k].size(); ++i) {
							cout << new_row_queue_up[k][i] << " ";
						}
						cout << endl;
#endif
                        path_queue_up.merge_row(new_row_queue_up[k], k, row_y - 1);
                    }
                }
            }
            // Left
            for (k = 0; k < _nk; ++k) {
                if (new_row_queue_left[k].size() > 0) {
#ifdef DEBUGGING
                    cout << "k = " << k << ":";
					for (i = 0; i < new_row_queue_left[k].size(); ++i) {
						cout << new_row_queue_left[k][i] << " ";
					}
					cout << endl;
#endif
                    path_queue_up.merge_row(new_row_queue_left[k], k, row_y);
                }
            }

#ifdef DEBUGGING
            cout << endl;
#endif
            if (sort_index >= num_pixels) break;
        }

        /*************************************** Downward sweep *********************************************/
        /* Propagate changes at current threshold down the image */
#ifdef DEBUGGING
        cout << "DOWNWARD SWEEP - before" << endl;
		dumpState(
			_nx,									// Image dimensions etc.
			_ny,
			_nk,
			path_queue_up,						// Queueing structures
			path_queue_down,
			in_queue_up,
			in_queue_down,
			bin_input_image,					// Input/output binary images
			bin_output_image_array,
			bin_output_image_count,
			chain_image_up,						// Up/down chain lengths
			chain_image_down
		);
#endif

        for (k = 0; k < _nk; ++k) {
#ifdef DEBUGGING
            cout << "k = " << k << endl;
#endif
            for (y = 0; y < _ny; ++y) {
                vector<PIXEL_INDEX_TYPE> & row_queue = path_queue_down.q[k][y];
                if (row_queue.size() == 0) continue;


#ifdef DEBUGGING
                cout << "\ty = " << y << endl;
#endif

                bool right_queue = false;
                vector<PIXEL_INDEX_TYPE> cur_row_queue_next_k;
                vector<PIXEL_INDEX_TYPE> new_row_queue_cur_k;
                vector<PIXEL_INDEX_TYPE> new_row_queue_next_k;

                /* Perform updates on points in row_queue, propagating changes to the new row queues */
                unsigned int ui = 0;
                x = row_queue[ui];
                while (true) {
                    /* Extract x-coordinate and pixel index */
                    index = x + _nx * y;

#ifdef DEBUGGING
                    cout << "\t\tx = " << x << endl;
#endif

                    /* Unflag -> no longer in queue */
                    in_queue_down[k + _nk * index] = 0;

                    /* Update chain length from upward neighbours */
                    int max_prev = -1;
                    // Previous level - accept a gap
                    if (k > 0) {
                        if (y > 0) {
                            if (x > 0) {
                                new_index = index - _nx - 1;
                                if (chain_image_up[k - 1 + _nk * new_index] > max_prev) {
                                    max_prev = chain_image_up[k - 1 + _nk * new_index];
                                }
                            }
                            new_index = index - _nx;
                            if (chain_image_up[k - 1 + _nk * new_index] > max_prev) {
                                max_prev = chain_image_up[k - 1 + _nk * new_index];
                            }
                        }
                        if (x > 0) {
                            new_index = index - 1;
                            if (chain_image_up[k - 1 + _nk * new_index] > max_prev) {
                                max_prev = chain_image_up[k - 1 + _nk * new_index];
                            }
                        }
                    }
                    // Current level - no gap allowed
                    if (y > 0) {
                        if (x > 0) {
                            new_index = index - _nx - 1;
                            if (bin_input_image[new_index] == 1 && chain_image_up[k + _nk * new_index] > max_prev) {
                                max_prev = chain_image_up[k + _nk * new_index];
                            }
                        }
                        new_index = index - _nx;
                        if (bin_input_image[new_index] == 1 && chain_image_up[k + _nk * new_index] > max_prev) {
                            max_prev = chain_image_up[k + _nk * new_index];
                        }
                    }
                    if (x > 0) {
                        new_index = index - 1;
                        if (bin_input_image[new_index] == 1 && chain_image_up[k + _nk * new_index] > max_prev) {
                            max_prev = chain_image_up[k + _nk * new_index];
                        }
                    }

                    /* Update chain length? */
                    if (max_prev + 1 < chain_image_up[k + _nk * index]) {
#ifdef DEBUGGING
                        cout << "New chain length is " << chain_image_up[k + _nk * index] << endl;
#endif
                        // Update chain length
                        chain_image_up[k + _nk * index] = max_prev + 1;

                        // Propagate changes to output
                        if (bin_input_image[index]) {
                            char new_bin_output_flag =
                                    (chain_image_up[k + _nk * index] + chain_image_down[(K - k) + _nk * index] + 1 >= L);
                            // Did we cross the threshold?
                            if (bin_output_image_array[k + _nk * index] && !new_bin_output_flag) {
                                // Clear the flag
                                bin_output_image_array[k + _nk * index] = 0;
                                --bin_output_image_count[index];
                                // Did this extinguish the last path?
                                if (bin_output_image_count[index] == 0) {
                                    // Write to output
                                    output_image[index] = threshold;
                                }
                            }
                        } else {
                            if (K - 1 - k >= 0) {
                                char new_bin_output_flag =
                                        (chain_image_up[k + _nk * index] + chain_image_down[(K - 1 - k) + _nk * index] + 1 >= L);
                                // Did we cross the threshold?
                                if (bin_output_image_array[k + _nk * index] && !new_bin_output_flag) {
                                    // Clear the flag
                                    bin_output_image_array[k + _nk * index] = 0;
                                    --bin_output_image_count[index];
                                    // Did this extinguish the last path?
                                    if (bin_output_image_count[index] == 0) {
                                        // Write to output
                                        output_image[index] = threshold;
                                    }
                                }
                            }
                            // Else, this pixel has already been removed
                        }

                        /* Propagate changes by enqueueing downward neighbours */
                        if (y < _ny - 1) {
                            // Same layer
                            new_index = index + _nx;
                            if (!in_queue_down[k + _nk * new_index]) {
                                new_row_queue_cur_k.push_back(x);
                                in_queue_down[k + _nk * new_index] = 1;
                            }
                            if (x < _nx - 1) {
                                new_index = index + _nx + 1;
                                if (!in_queue_down[k + _nk * new_index]) {
                                    new_row_queue_cur_k.push_back(x + 1);
                                    in_queue_down[k + _nk * new_index] = 1;
                                }
                            }
                        }
                        if (x < _nx - 1) {
                            new_index = index + 1;
                            if (!in_queue_down[k + _nk * new_index]) {
                                right_queue = true;
                                in_queue_down[k + _nk * new_index] = 1;
                            }
                        }

                        // Down one layer
                        if (k < K) {
                            if (y < _ny - 1) {
                                new_index = index + _nx;
                                if (!in_queue_down[k + 1 + _nk * new_index]) {
                                    new_row_queue_next_k.push_back(x);
                                    in_queue_down[k + 1 + _nk * new_index] = 1;
                                }
                                if (x < _nx - 1) {
                                    new_index = index + _nx + 1;
                                    if (!in_queue_down[k + 1 + _nk * new_index]) {
                                        new_row_queue_next_k.push_back(x + 1);
                                        in_queue_down[k + 1 + _nk * new_index] = 1;
                                    }
                                }
                            }
                            if (x < _nx - 1) {
                                new_index = index + 1;
                                if (!in_queue_down[k + 1 + _nk * new_index]) {
                                    cur_row_queue_next_k.push_back(x + 1);
                                    in_queue_down[k + 1 + _nk * new_index] = 1;
                                }
                            }
                        }
                    }

                    /* Select next x */
                    if (right_queue) {
                        // Go across
                        right_queue = false;
                        ++x;
                        // Test halting condition
                        if (x > _nx - 1) break;

                        // Consume the row queue?
                        if (ui + 1 < row_queue.size()) {
                            if (row_queue[ui + 1] == x)
                                ++ui;
                        }
                    } else {
                        // Halting condition
                        if (ui + 1 >= row_queue.size()) {
                            break;
                        } else {
                            ++ui;
                            x = row_queue[ui];
                        }
                    }
                }
                // Wipe old queue
                row_queue.resize(0);

                /* Merge new row queues into existing queues */
                if (y + 1 < _ny) {
                    if (new_row_queue_cur_k.size() > 0) {
                        path_queue_down.merge_row(new_row_queue_cur_k, k, y + 1);
                    }
                    if (new_row_queue_next_k.size() > 0) {
                        path_queue_down.merge_row(new_row_queue_next_k, k + 1, y + 1);
                    }
                }
                if (cur_row_queue_next_k.size() > 0) {
                    path_queue_down.merge_row(cur_row_queue_next_k, k + 1, y);
                }
            }
        }
#ifdef DEBUGGING
        cout << "DOWNWARD SWEEP - after" << endl;
		dumpState(
			_nx,									// Image dimensions etc.
			_ny,
			_nk,
			path_queue_up,						// Queueing structures
			path_queue_down,
			in_queue_up,
			in_queue_down,
			bin_input_image,					// Input/output binary images
			bin_output_image_array,
			bin_output_image_count,
			chain_image_up,						// Up/down chain lengths
			chain_image_down
		);
#endif

        /*************************************** Upward sweep *********************************************/
#ifdef DEBUGGING
        cout << "UPWARD SWEEP - before" << endl;
		dumpState(
			_nx,									// Image dimensions etc.
			_ny,
			_nk,
			path_queue_up,						// Queueing structures
			path_queue_down,
			in_queue_up,
			in_queue_down,
			bin_input_image,					// Input/output binary images
			bin_output_image_array,
			bin_output_image_count,
			chain_image_up,						// Up/down chain lengths
			chain_image_down
		);
#endif

        /* Propagate changes at current threshold up the image */
        for (k = 0; k < _nk; ++k) {
#ifdef DEBUGGING
            cout << "k = " << k << endl;
#endif
            for (y = _ny - 1; y >= 0; --y) {
                vector<PIXEL_INDEX_TYPE> & row_queue = path_queue_up.q[k][y];
                if (row_queue.size() == 0) continue;

#ifdef DEBUGGING
                cout << "\ty = " << y << endl;
#endif

                bool left_queue = false;
                vector<PIXEL_INDEX_TYPE> cur_row_queue_next_k;
                vector<PIXEL_INDEX_TYPE> new_row_queue_cur_k;
                vector<PIXEL_INDEX_TYPE> new_row_queue_next_k;

                /* Perform updates on points in row_queue, propagating changes to the new row queues */
                int i = row_queue.size() - 1; // needs to be signed
                x = row_queue[i];
                while(true) {
                    /* Extract x-coordinate and pixel index */
                    index = x + _nx * y;

#ifdef DEBUGGING
                    cout << "\t\tx = " << x << endl;
#endif

                    /* Unflag -> no longer in queue */
                    in_queue_up[k + _nk * index] = 0;

                    /* Update chain length from downward neighbours */
                    // Note: Only y < _ny - 1 may be 'updated', so we are assured of the existence of previous neighbours!
                    int max_prev = -1;
                    // Previous level - accept a gap
                    if (k > 0) {
                        if (y < _ny - 1) {
                            new_index = index + _nx;
                            if (chain_image_down[k - 1 + _nk * new_index] > max_prev) {
                                max_prev = chain_image_down[k - 1 + _nk * new_index];
                            }
                            if (x < _nx - 1) {
                                new_index = index + _nx + 1;
                                if (chain_image_down[k - 1 + _nk * new_index] > max_prev) {
                                    max_prev = chain_image_down[k - 1 + _nk * new_index];
                                }
                            }
                        }
                        if (x < _nx - 1) {
                            new_index = index + 1;
                            if (chain_image_down[k - 1 + _nk * new_index] > max_prev) {
                                max_prev = chain_image_down[k - 1 + _nk * new_index];
                            }
                        }
                    }
                    // Current level - no gap allowed
                    if (y < _ny - 1) {
                        new_index = index + _nx;
                        if (bin_input_image[new_index] == 1 && chain_image_down[k + _nk * new_index] > max_prev) {
                            max_prev = chain_image_down[k + _nk * new_index];
                        }
                        if (x < _nx - 1) {
                            new_index = index + _nx + 1;
                            if (bin_input_image[new_index] == 1 && chain_image_down[k + _nk * new_index] > max_prev) {
                                max_prev = chain_image_down[k + _nk * new_index];
                            }
                        }
                    }
                    if (x < _nx - 1) {
                        new_index = index + 1;
                        if (bin_input_image[new_index] == 1 && chain_image_down[k + _nk * new_index] > max_prev) {
                            max_prev = chain_image_down[k + _nk * new_index];
                        }
                    }

                    /* Update chain length? */
                    if (max_prev + 1 < chain_image_down[k + _nk * index]) {
                        // Update chain length
                        chain_image_down[k + _nk * index] = max_prev + 1;

                        // Propagate changes to output
                        if (bin_input_image[index]) {
                            char new_bin_output_flag =
                                    (chain_image_up[(K - k) + _nk * index] + chain_image_down[k + _nk * index] + 1 >= L);
                            // Did we cross the threshold?
                            if (bin_output_image_array[(K - k) + _nk * index] && !new_bin_output_flag) {
                                // Update flags
                                bin_output_image_array[(K - k) + _nk * index] = 0;
                                --bin_output_image_count[index];
                                // Did this extinguish the last path?
                                if (bin_output_image_count[index] == 0) {
                                    // Write to output
                                    output_image[index] = threshold;
                                }
                            }
                        } else {
                            if (K - 1 - k >= 0) {
                                char new_bin_output_flag =
                                        (chain_image_up[(K - 1 - k) + _nk * index] + chain_image_down[k + _nk * index] + 1 >= L);
                                // Did we cross the threshold?
                                if (bin_output_image_array[(K - 1 - k) + _nk * index] && !new_bin_output_flag) {
                                    // Update flags
                                    bin_output_image_array[(K - 1 - k) + _nk * index] = 0;
                                    --bin_output_image_count[index];
                                    // Did this extinguish the last path?
                                    if (bin_output_image_count[index] == 0) {
                                        // Write to output
                                        output_image[index] = threshold;
                                    }
                                }
                            }
                            // Else, this pixel has already been removed
                        }

                        /* Propagate changes by enqueueing upward neighbours */
                        if (y > 0) {
                            // Same layer
                            if (x > 0) {
                                new_index = index - _nx - 1;
                                if (!in_queue_up[k + _nk * new_index]) {
                                    new_row_queue_cur_k.push_back(x - 1);
                                    in_queue_up[k + _nk * new_index] = 1;
                                }
                            }
                            new_index = index - _nx;
                            if (!in_queue_up[k + _nk * new_index]) {
                                new_row_queue_cur_k.push_back(x);
                                in_queue_up[k + _nk * new_index] = 1;
                            }
                        }
                        if (x > 0) {
                            new_index = index - 1;
                            if (!in_queue_up[k + _nk * new_index]) {
                                left_queue = true;
                                in_queue_up[k + _nk * new_index] = 1;
                            }
                        }

                        // Down one layer
                        if (k < K) {
                            if (y > 0) {
                                if (x > 0) {
                                    new_index = index - _nx - 1;
                                    if (!in_queue_up[k + 1 + _nk * new_index]) {
                                        new_row_queue_next_k.push_back(x - 1);
                                        in_queue_up[k + 1 + _nk * new_index] = 1;
                                    }
                                }
                                new_index = index - _nx;
                                if (!in_queue_up[k + 1 + _nk * new_index]) {
                                    new_row_queue_next_k.push_back(x);
                                    in_queue_up[k + 1 + _nk * new_index] = 1;
                                }
                            }
                            if (x > 0) {
                                new_index = index - 1;
                                if (!in_queue_up[k + 1 + _nk * new_index]) {
                                    cur_row_queue_next_k.push_back(x - 1);
                                    in_queue_up[k + 1 + _nk * new_index] = 1;
                                }
                            }
                        }
                    }

                    /* Select next x */
                    if (left_queue) {
                        // Go across
                        left_queue = false;
                        --x;
                        // Test halting condition
                        if (x < 0) break;

                        // Consume the row queue?
                        if (i - 1 >= 0) {
                            if (row_queue[i - 1] == x)
                                --i;
                        }
                    } else {
                        // Halting condition
                        if (i - 1 < 0) {
                            break;
                        } else {
                            --i;
                            x = row_queue[i];
                        }
                    }
                }
                // Wipe old queue
                row_queue.resize(0);

                /* Merge new row queues into existing queues */
                if (y - 1 >= 0) {
                    if (new_row_queue_cur_k.size() > 0) {
                        reverse(new_row_queue_cur_k.begin(), new_row_queue_cur_k.end());
                        path_queue_up.merge_row(new_row_queue_cur_k, k, y - 1);
                    }
                    if (new_row_queue_next_k.size() > 0) {
                        reverse(new_row_queue_next_k.begin(), new_row_queue_next_k.end());
                        path_queue_up.merge_row(new_row_queue_next_k, k + 1, y - 1);
                    }
                }
                if (cur_row_queue_next_k.size() > 0) {
                    reverse(cur_row_queue_next_k.begin(), cur_row_queue_next_k.end());
                    path_queue_up.merge_row(cur_row_queue_next_k, k + 1, y);
                }
            }
        }
#ifdef DEBUGGING
        cout << "UPWARD SWEEP - after" << endl;
		dumpState(
			_nx,									// Image dimensions etc.
			_ny,
			_nk,
			path_queue_up,						// Queueing structures
			path_queue_down,
			in_queue_up,
			in_queue_down,
			bin_input_image,					// Input/output binary images
			bin_output_image_array,
			bin_output_image_count,
			chain_image_up,						// Up/down chain lengths
			chain_image_down
		);
#endif
        for(int i = 0; i < num_pixels; ++i)
            opening_transform[threshold][i] = (chain_image_up[i] + chain_image_down[i] + 1);

    }

    /* Free allocated memory */
    free((void *)in_queue_up);
    free((void *)in_queue_down);

    free((void *)bin_input_image);

    free((void *)bin_output_image_array);
    free((void *)bin_output_image_count);

}