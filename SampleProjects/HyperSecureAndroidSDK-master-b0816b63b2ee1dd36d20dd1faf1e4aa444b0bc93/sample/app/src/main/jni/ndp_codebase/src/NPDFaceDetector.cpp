/* Copyright (C) HyperVerge, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "NPDFaceDetector.h"
#include "NPDmodel.h"
#include "graph.h"
#include <algorithm>
#include <fstream>
#include <math.h>
#include "omp.h"

using namespace std;

std::vector<NPDrect> NPDFaceDetector::NPDScan(const unsigned char *I, int width, int height)
{
    int minFace, maxFace;

    minFace = max(this->minFace, model->objSize);
    maxFace = min(this->maxFace, min(height, width));

    if(min(height, width) < minFace)
    {
        NPDrect rect;
        rect.row = 0;
        rect.col = 0;
        rect.size = 0;
        rect.score = 0;

        std::vector<NPDrect> vecrect;
        vecrect.push_back(rect);
        return vecrect;
    }  

    // containers for the detected faces
    vector<double> row, col, size, score;

    omp_set_num_threads(4);

    for(int k = 0; k < model->numScales; k++) // process each scale
    {
        int winSizek = model->winSize[k];

        if(winSizek < minFace) continue;
        else if(winSizek > maxFace) break;
        
        // determine the step of the sliding subwindow
        int winStep = (int) floor(winSizek * 0.1);
        if(winSizek > 40) winStep = (int) floor(winSizek * 0.05);

        // calculate the offset values of each pixel in a subwindow
        // pre-determined offset of pixels in a subwindow
        vector<int> offset(winSizek * winSizek);
        int p1 = 0, p2 = 0, gap = height - winSizek;
        for(int j=0; j < winSizek; j++) // column coordinate
        {
            for(int i = 0; i < winSizek; i++) // row coordinate
            {
                offset[p1++] = p2++;
            }
            
            p2 += gap;
        }
        
        int colMax = width - winSizek + 1;
        int rowMax = height - winSizek + 1;
        
        // process each subwindow
        #pragma omp parallel for
        for(int c = 0; c < colMax; c += winStep) // slide in column
        {
            const unsigned char *pPixel = I + c * height;

            for(int r = 0; r < rowMax; r += winStep, pPixel += winStep) // slide in row
            {
                int treeIndex = 0;
                float _score = 0;
                int s;

                // test each tree classifier
                for(s = 0; s < model->numStages; s++)
                {
                    int node = model->treeRoot[treeIndex];

                    // test the current tree classifier
                    while(node > -1) // branch node
                    {
                        int p1 = pPixel[offset[model->pixel1[node][k]]];
                        int p2 = pPixel[offset[model->pixel2[node][k]]];
                        int fea = model->npdTable[p1][p2];

                        if(fea < model->cutpoint[node][0] || fea > model->cutpoint[node][1]) node = model->leftChild[node];
                        else node = model->rightChild[node];
                    }

                    // leaf node
                    node = - node - 1;
                    _score = _score + model->fit[node];
                    treeIndex++;

                    if(_score < model->stageThreshold[s]) break; // negative samples
                }
                
                if(s == model->numStages) // a face detected
                {
                    double _row = r + 1.00;
                    double _col = c + 1.00;
                    double _size = winSizek;

                    #pragma omp critical // modify the record by a single thread
                    {
                        row.push_back(_row);
                        col.push_back(_col);
                        size.push_back(_size);
                        score.push_back(_score);
                    }
                }
            }
        }
    }
    
    int numFaces = (int) row.size();
    std::vector<NPDrect> faces;
    for(int i = 0; i < numFaces; i++)
    {
        NPDrect rect;
        rect.row = row[i];
        rect.col = col[i];
        rect.size = size[i];
        rect.score = score[i];
        faces.push_back(rect);
    }
    return faces;
}

bool NPDFaceDetector::mergeCriterion(NPDrect rect1, NPDrect rect2)
{

    float h = min(rect1.row + rect1.size, rect2.row + rect2.size) - max(rect1.row, rect2.row);
    float w = min(rect1.col + rect1.size, rect2.col + rect2.size) - max(rect1.col, rect2.col);
    float s = max(h, (float) 0) * max(w, (float) 0);

    float u = (rect1.size * rect1.size + rect2.size * rect2.size - s);
    float iou = 1.0 * s / u;

    if (iou >= this->overlappingThreshold)
        return true;
    else
        return false;
}

std::vector<std::vector<int> > NPDFaceDetector::groupDetections(std::vector<NPDrect> detections)
{
    // Creating adjacency matrix
    GraphConnected graph(detections.size());

    omp_set_num_threads(4);
    #pragma omp parallel for
    for(int i = 0; i < detections.size(); i++)
        for(int j = i + 1; j < detections.size(); j++)
            if(mergeCriterion(detections[i], detections[j]))
                graph.addEdge(i, j);
    
    return graph.connectedComponents();
}

float NPDFaceDetector::logistic(float input)
{
    return log(1 + exp(input));
}

std::vector<NPDrect> NPDFaceDetector::detectFaces(const unsigned char *img, int width_image, int height_image)
{

    // Scan for candidates
    std::vector<NPDrect> candidates = NPDScan(img, width_image, height_image);

    // Group detections
    std::vector<std::vector<int> > groups = groupDetections(candidates);

    std::vector<NPDrect> faces(groups.size() - 1);

    omp_set_num_threads(4);
    // Eliminate a few groups
    #pragma omp parallel for
    for(int i = 0; i < groups.size() - 1; i++)
    {
        int groups_size = groups[i].size();

        std::vector<float> weights(groups_size);
        for(int j = 0; j < groups_size; j++)
            weights[j] = logistic(candidates[groups[i][j]].score);

        // Find sum of weights
        float sum_weight = 0;
        for(int j = 0; j < groups_size; j++)
            sum_weight += weights[j];
        faces[i].score = sum_weight;

        if(sum_weight == 0)
        {
            for(int j = 0; j < groups_size; j++)
                weights[j] = 1.0 / groups_size;
        }
        else
        {
            for(int j = 0; j < groups_size; j++)
                weights[j] = weights[j] / sum_weight;
        }

        // Find cross product
        float cross_score = 0;
        float cross_col = 0;
        float cross_row = 0;
        for(int j = 0; j < groups_size; j++)
        {
            cross_score += candidates[groups[i][j]].size * weights[j];
            cross_col += (candidates[groups[i][j]].col + candidates[groups[i][j]].size * 0.5) 
                * weights[j];
            cross_row += (candidates[groups[i][j]].row + candidates[groups[i][j]].size * 0.5) 
                * weights[j];
        }


        faces[i].size = floor(cross_score);
        faces[i].row = floor(cross_row - faces[i].size * 1.0 / 2);
        faces[i].col = floor(cross_col - faces[i].size * 1.0 / 2);

        int size = (int) faces[i].size;
        int x = (int) faces[i].col;
        int y  = (int) faces[i].row;
        int width = size;
        int height = size;

        int padding_up = size;
        int padding_down = size/4;
        int padding_left = size/2;
        int padding_right = size/2;

        if(x - padding_left > 0 && x  + size + padding_right < width_image)
        {
            x -= padding_left;
            width += padding_left + padding_right;
        }
        else if(x - padding_left > 0 && x + size + padding_right >= width_image)
        {
            width += padding_left + (width_image - x - size);
            x -= padding_left;
        }
        else if(x - padding_left <= 0 && x + size + padding_right < width_image)
        {
            width += x + padding_right;
            x = 0;
        }
        else
        {
            x = 0;
            width = width_image;
        }

        if(y - padding_up > 0 && y + size + padding_down < height_image)
        {
            y -= padding_up;
            height += padding_up + padding_down;
        }
        else if(y - padding_up > 0 && y + size + padding_down >= height_image)
        {
            height += padding_up + (height_image - y - size);
            y -= padding_up;
        }
        else if(y - padding_up <= 0 && y + size + padding_down < height_image)
        {
            height += y + padding_down;
            y = 0;
        }
        else
        {
            y = 0;
            height = height_image;
        }

        faces[i].row = y;
        faces[i].col = x;
        faces[i].neighbors = groups[i].size();
        faces[i].width = width;
        faces[i].height = height;
    }
    return faces;
}

