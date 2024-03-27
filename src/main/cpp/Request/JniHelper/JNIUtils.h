//
// Created by tanzi on 16/12/2019.
//
#include <jni.h>
#include <map>
#include <vector>
#include <string>
#include <android/log.h>
#include <obfuscate.h>

#ifndef VIRTUAL_JNIUTILS_H
#define VIRTUAL_JNIUTILS_H


using namespace std;

class JNIUtils {
private:
    JNIEnv *env;

public:
    JNIUtils(JNIEnv *e);
    jboolean isValidVersion();
    jboolean isSafeConnection();
    jstring GetBuildBoard();
    jstring GetBuildBootloader();
    jstring GetBuildDevice();
    jstring GetBuildFingerprint();
    jstring GetBuildDisplay();
    jstring GetBuildHost();
    jstring GetBuildManufacturer();
    jstring GetBuildID();
    jstring GetBuildBrand();
    jstring GetBuildHardware();
    string BuildJSON(map<jstring, jstring> params);
    string GetJsonString(jstring src, jstring key);
    jint GetJsonInt(jstring src, jstring key);
    jboolean GetJsonBoolean(jstring src, jstring key);
    string jstring2string(jstring src);
    vector<string> split (string s, string delimiter);
    string http_post(string site,string data, string token);
    string GetPath();
    string GetPathLogs(jobject jthiz);
    jstring GetSecureID(jobject jthiz);

    void DrawLine(jobject thiz, jobject  canvas, jint a, jint r, jint g, jint b, jfloat strokewidth, jfloat startX, jfloat startY, jfloat stopX, jfloat stopY);
    void DrawText(jobject thiz, jobject canvas, jint a, jint r, jint g, jint b, jstring txt, jfloat posX, jfloat posY, jfloat size);
    void DrawRect(jobject thiz, jobject canvas, jint a, jint r, jint g, jint b, jint strokewidth, jfloat X, jfloat Y, jfloat width, jfloat height);
    void ClearCanvas(jobject thiz, jobject  canvas);

};


#endif //VIRTUAL_JNIUTILS_H
