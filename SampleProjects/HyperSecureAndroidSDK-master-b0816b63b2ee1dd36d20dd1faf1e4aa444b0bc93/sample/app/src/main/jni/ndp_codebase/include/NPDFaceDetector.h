/* Copyright (C) HyperVerge, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef NPD_FACE_DETECTOR_H
#define NPD_FACE_DETECTOR_H

#include "NPDmodel.h"
#include <iostream>
#include <vector>


class NPDFaceDetector
{
    private:

        NPDmodel* model;
        int minFace;
        int maxFace;
        float overlappingThreshold;

        bool mergeCriterion(NPDrect rect1, NPDrect rect2);
        std::vector<std::vector<int> > groupDetections(std::vector<NPDrect> detections);
        float logistic(float input);

    public:
        NPDFaceDetector(NPDmodel *model, int minFace, int maxFace)
        {
            this->model = model;
            this->minFace = minFace;
            this->maxFace = maxFace;
            this->overlappingThreshold = 0.3;

        };
        std::vector<NPDrect> detectFaces(const unsigned char *img, int width, int height);
        std::vector<NPDrect> NPDScan(const unsigned char *img, int width, int height);
};

#endif

