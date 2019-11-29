/* Copyright (C) HyperVerge, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef NPD_MODEL_H
#define NPD_MODEL_H

typedef struct NpdModel
{
    int objSize;
    int numStages;
    int numBranchNodes;
    int numLeafNodes;
    
    float stageThreshold[72];
    int treeRoot[72];

    int pixel1[1018][33];
    int pixel2[1018][33];

    int cutpoint[1018][2];
    int leftChild[1018];
    int rightChild[1018];
    float fit[1090];

    int npdTable[256][256];

    int scaleFactor;
    int numScales;
    int winSize[33];
} NPDmodel;

typedef struct NpdRect
{
    double row;
    double col;
    double size;
    double neighbors;
    double score;
    int width;
    int height;
} NPDrect;
#endif

