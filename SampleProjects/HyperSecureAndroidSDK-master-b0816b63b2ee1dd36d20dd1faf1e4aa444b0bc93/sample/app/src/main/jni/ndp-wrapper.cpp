#include <jni.h>
#include "NPDmodel.h"
#include "NPDFaceDetector.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <android/bitmap.h>
#include <iostream>
#include <vector>
#include <sys/time.h>


#include "json/json.h"

#define LOG_E(...)    __android_log_print(ANDROID_LOG_ERROR,"genmax",__VA_ARGS__)

#define COEFF_R        0.299
#define COEFF_G        0.587
#define COEFF_B        0.114

#ifdef __cplusplus
extern "C" {
#endif

long long currentTimeInMilliseconds()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;

} argb;


std::string detect_faces(AndroidBitmapInfo *infogray, void *pixelsgray, NPDmodel *model)
{
    int height = infogray->height;
    int width = infogray->width;

    //LOG_E("Yello height = %d, width = %d, objSize = %d", height, width, model->objSize);

    argb *pixeldata;
    pixeldata = ((argb *) pixelsgray);

    // Loading onto an array and changing from row major to column major form
    int num_pixels = width * height;
    unsigned char* img_char = new unsigned char[width * height]();

    for(int y = 0; y < width * height; y++)
    {
        int red = pixeldata[y].red;
        int green = pixeldata[y].green;
        int blue = pixeldata[y].blue;

        int gray = red * COEFF_R + green * COEFF_G + blue * COEFF_B;
        int i = y / width;
        int j = y % width;
        img_char[j * height + i] = (unsigned char) gray;
    }

    // Initializing detector
    NPDFaceDetector detector(model, 12, 4000);

    // Detecting faces
    std::vector<NPDrect> faces = detector.detectFaces(img_char, width, height);

    // Writing onto a JSON array
    Json::Value faces_array(Json::arrayValue);
    for(int i = 0; i < faces.size(); i++)
    {
        Json::Value face(Json::objectValue);
        if ((int) faces[i].score > 10)
        {
            face["y"] = (int) faces[i].row;
            face["x"] = (int) faces[i].col;
            face["w"] = (int) faces[i].width;
            face["h"] = (int) faces[i].height;
            faces_array.append(face);
        }
    }
    Json::FastWriter writer;
    std::string json_value = writer.write(faces_array);
    delete[] img_char;

    return json_value;
}

JNIEXPORT jlong JNICALL
        Java_co_hypersecure_FaceDetection_Detectors_NDPDetector_loadModel(JNIEnv *env, jobject instance, jobject assetManager)
{
    // Loading model from assets
    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
    AAsset* asset = AAssetManager_open(mgr, "model_frontal_scaled.bin", AASSET_MODE_STREAMING);

    NPDmodel* model_loaded = new NPDmodel;
    int nb_read = 0;
    nb_read = AAsset_read(asset, (char *)model_loaded, sizeof(NPDmodel));
    return (long) model_loaded;
}

JNIEXPORT void JNICALL
Java_co_hypersecure_FaceDetection_Detectors_NDPDetector_releaseModel(JNIEnv *env, jobject instance, jlong loaded_model)
{
    // Releasing model
    delete (NPDmodel *) loaded_model;
}

JNIEXPORT jstring JNICALL
Java_co_hypersecure_FaceDetection_Detectors_NDPDetector_detectFaces(JNIEnv *env, jobject instance,
                                               jobject bitmapIn, jlong loaded_model)
{

    NPDmodel *model = (NPDmodel *) loaded_model;

    // Detecting faces
    long long t1 = currentTimeInMilliseconds();

    // Loading bitmap
    AndroidBitmapInfo infogray;
    void *pixelsgray;
    int ret;
    uint8_t save;

    if ((ret = AndroidBitmap_getInfo(env, bitmapIn, &infogray)) < 0) {
        LOG_E("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return 0;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmapIn, &pixelsgray)) < 0) {
        LOG_E("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    // Detecting faces
    std::string response = detect_faces(&infogray, pixelsgray,model);

    AndroidBitmap_unlockPixels(env, bitmapIn);

    long long t2 = currentTimeInMilliseconds();
    //LOG_E("Detection time = %lld", t2 - t1);

    // Returning JSON response
    return env->NewStringUTF(response.c_str());
}

#ifdef __cplusplus
}
#endif
