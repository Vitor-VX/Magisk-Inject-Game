//
// Created by tanzi on 16/12/2019.
//

#include "JNIUtils.h"
#include <map>
#include <string>
#include <vector>
#include <android/log.h>
#include <obfuscate.h>


#define LOG_TAG "Legendarios"

#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


JNIUtils::JNIUtils(JNIEnv *e) {
    env = e;
}

jstring JNIUtils::GetBuildBoard() {
    jclass Build = env->FindClass(OBFUSCATE("android/os/Build"));
    jfieldID  Build_board = env->GetStaticFieldID(Build, OBFUSCATE("BOARD"), OBFUSCATE("Ljava/lang/String;"));

    jstring board = reinterpret_cast<jstring >(env->GetStaticObjectField(Build, Build_board));

    return board;
}

jstring JNIUtils::GetBuildBootloader() {
    jclass Build = env->FindClass(OBFUSCATE("android/os/Build"));
    jfieldID  Build_Bootloader = env->GetStaticFieldID(Build, OBFUSCATE("BOOTLOADER"), OBFUSCATE("Ljava/lang/String;"));

    jstring Bootloader = reinterpret_cast<jstring >(env->GetStaticObjectField(Build, Build_Bootloader));

    return Bootloader;
}

jstring JNIUtils::GetBuildDevice() {
    jclass Build = env->FindClass(OBFUSCATE("android/os/Build"));
    jfieldID  _device = env->GetStaticFieldID(Build, OBFUSCATE("DEVICE"), OBFUSCATE("Ljava/lang/String;"));

    jstring device = reinterpret_cast<jstring >(env->GetStaticObjectField(Build, _device));

    return device;
}

jstring JNIUtils::GetBuildFingerprint() {
    jclass Build = env->FindClass(OBFUSCATE("android/os/Build"));
    jfieldID  _fingeprint = env->GetStaticFieldID(Build, OBFUSCATE("FINGERPRINT"), OBFUSCATE("Ljava/lang/String;"));

    jstring fingerprint = reinterpret_cast<jstring >(env->GetStaticObjectField(Build, _fingeprint));

    return fingerprint;
}

jstring JNIUtils::GetBuildDisplay() {
    jclass Build = env->FindClass(OBFUSCATE("android/os/Build"));
    jfieldID  _display = env->GetStaticFieldID(Build, OBFUSCATE("DISPLAY"), OBFUSCATE("Ljava/lang/String;"));

    jstring display = reinterpret_cast<jstring >(env->GetStaticObjectField(Build, _display));

    return display;
}

jstring JNIUtils::GetBuildHost() {
    jclass Build = env->FindClass(OBFUSCATE("android/os/Build"));
    jfieldID  _host = env->GetStaticFieldID(Build, OBFUSCATE("HOST"), OBFUSCATE("Ljava/lang/String;"));

    jstring host = reinterpret_cast<jstring >(env->GetStaticObjectField(Build, _host));

    return host;
}

jstring JNIUtils::GetBuildManufacturer() {
    jclass Build = env->FindClass(OBFUSCATE("android/os/Build"));
    jfieldID  _manufacturer = env->GetStaticFieldID(Build, OBFUSCATE("MANUFACTURER"), OBFUSCATE("Ljava/lang/String;"));

    jstring manufacturer = reinterpret_cast<jstring >(env->GetStaticObjectField(Build, _manufacturer));

    return manufacturer;
}

jstring JNIUtils::GetBuildID() {
    jclass Build = env->FindClass(OBFUSCATE("android/os/Build"));
    jfieldID  _id = env->GetStaticFieldID(Build, OBFUSCATE("ID"), OBFUSCATE("Ljava/lang/String;"));

    jstring id = reinterpret_cast<jstring >(env->GetStaticObjectField(Build, _id));

    return id;
}

jstring JNIUtils::GetBuildBrand() {
    jclass Build = env->FindClass(OBFUSCATE("android/os/Build"));
    jfieldID  _brand = env->GetStaticFieldID(Build, OBFUSCATE("BRAND"), OBFUSCATE("Ljava/lang/String;"));

    jstring brand = reinterpret_cast<jstring >(env->GetStaticObjectField(Build, _brand));

    return brand;
}

jstring JNIUtils::GetBuildHardware() {
    jclass Build = env->FindClass(OBFUSCATE("android/os/Build"));
    jfieldID  _hardware = env->GetStaticFieldID(Build, OBFUSCATE("HARDWARE"), OBFUSCATE("Ljava/lang/String;"));

    jstring hardware = reinterpret_cast<jstring >(env->GetStaticObjectField(Build, _hardware));

    return hardware;
}

string JNIUtils::BuildJSON(map<jstring, jstring> params) {
    jclass JSONObject = env->FindClass(OBFUSCATE("org/json/JSONObject"));

    jmethodID jsonobject_constructor = env->GetMethodID(JSONObject, OBFUSCATE("<init>"), OBFUSCATE("()V"));

    jobject json = env->NewObject(JSONObject, jsonobject_constructor);

    jmethodID json_put = env->GetMethodID(JSONObject, OBFUSCATE("put"), OBFUSCATE("(Ljava/lang/String;Ljava/lang/Object;)Lorg/json/JSONObject;"));

    jmethodID json_toString = env->GetMethodID(JSONObject, OBFUSCATE("toString"), OBFUSCATE("()Ljava/lang/String;"));

    map<jstring, jstring>::iterator itr;
    for (itr = params.begin(); itr != params.end(); itr++) {
        env->CallObjectMethod(json, json_put, itr->first, itr->second);
    }

    return jstring2string(reinterpret_cast<jstring>(env->CallObjectMethod(json, json_toString)));
}

string JNIUtils::GetJsonString(jstring src, jstring key) {
    jclass JSONObject = env->FindClass(OBFUSCATE("org/json/JSONObject"));

    jmethodID jsonobject_constructor = env->GetMethodID(JSONObject, OBFUSCATE("<init>"), OBFUSCATE("(Ljava/lang/String;)V"));

    jobject json = env->NewObject(JSONObject, jsonobject_constructor, src);

    jmethodID json_getString = env->GetMethodID(JSONObject, OBFUSCATE("getString"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/String;"));

    jstring value = reinterpret_cast<jstring>(env->CallObjectMethod(json, json_getString, key));

    return jstring2string(value);
}

jint JNIUtils::GetJsonInt(jstring src, jstring key) {
    jclass JSONObject = env->FindClass(OBFUSCATE("org/json/JSONObject"));

    jmethodID jsonobject_constructor = env->GetMethodID(JSONObject, OBFUSCATE("<init>"), OBFUSCATE("(Ljava/lang/String;)V"));

    jobject json = env->NewObject(JSONObject, jsonobject_constructor, src);

    jmethodID json_getString = env->GetMethodID(JSONObject, OBFUSCATE("getInt"), OBFUSCATE("(Ljava/lang/String;)I"));

    jint value = env->CallIntMethod(json, json_getString, key);

    return value;
}

jboolean JNIUtils::GetJsonBoolean(jstring src, jstring key) {
    jclass JSONObject = env->FindClass(OBFUSCATE("org/json/JSONObject"));

    jmethodID jsonobject_constructor = env->GetMethodID(JSONObject, OBFUSCATE("<init>"), OBFUSCATE("(Ljava/lang/String;)V"));

    jobject json = env->NewObject(JSONObject, jsonobject_constructor, src);

    jmethodID json_getString = env->GetMethodID(JSONObject, OBFUSCATE("getBoolean"), OBFUSCATE("(Ljava/lang/String;)Z"));

    jboolean value = env->CallBooleanMethod(json, json_getString, key);

    return value;
}

string JNIUtils::jstring2string(jstring src) {
    if (!src)
        return "";

    const jclass stringClass = env->GetObjectClass(src);
    const jmethodID getBytes = env->GetMethodID(stringClass, OBFUSCATE("getBytes"), OBFUSCATE("(Ljava/lang/String;)[B"));
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(src, getBytes, env->NewStringUTF(OBFUSCATE("UTF-8")));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}

vector<string> JNIUtils::split(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}
/*
void ANativeActivity_onCreate(ANativeActivity* app, void*, size_t) {
    const jclass contextClass = app->env->GetObjectClass(app->clazz);
    const jmethodID getApplicationContextMethod =
            app->env->GetMethodID(contextClass, OBFUSCATE("getApplicationContext"),
                                  "()Landroid/content/Context;"));
    const jobject contextObject =
            app->env->CallObjectMethod(app->clazz, getApplicationContextMethod);
    const jmethodID getApplicationInfoMethod = app->env->GetMethodID(
            contextClass, OBFUSCATE("getApplicationInfo"), OBFUSCATE("()Landroid/content/pm/ApplicationInfo;"));
    const jobject applicationInfoObject =
            app->env->CallObjectMethod(contextObject, getApplicationInfoMethod);
    const jfieldID nativeLibraryDirField = app->env->GetFieldID(
            app->env->GetObjectClass(applicationInfoObject), OBFUSCATE("nativeLibraryDir"),
            "Ljava/lang/String;"));
    const jobject nativeLibraryDirObject =
            app->env->GetObjectField(applicationInfoObject, nativeLibraryDirField);
    const jmethodID getBytesMethod = app->env->GetMethodID(
            app->env->GetObjectClass(nativeLibraryDirObject), OBFUSCATE("getBytes"), OBFUSCATE("(Ljava/lang/String;)[B"));
    const auto bytesObject = static_cast<jbyteArray>(app->env->CallObjectMethod(
            nativeLibraryDirObject, getBytesMethod, app->env->NewStringUTF(OBFUSCATE("UTF-8"))));
    const size_t length = app->env->GetArrayLength(bytesObject);
    const jbyte *const bytes = app->env->GetByteArrayElements(bytesObject, nullptr);
    const std::string libDir(reinterpret_cast<const char *>(bytes), length);

}
*/
jboolean JNIUtils::isSafeConnection() {
    jclass java_util_ArrayList  = env->FindClass(OBFUSCATE("java/util/ArrayList"));
    jmethodID java_util_ArrayList_size = env->GetMethodID (java_util_ArrayList, OBFUSCATE("size"), OBFUSCATE("()I"));
    jmethodID java_util_ArrayList_get  = env->GetMethodID(java_util_ArrayList, OBFUSCATE("get"), OBFUSCATE("(I)Ljava/lang/Object;"));
    jclass Collections = env->FindClass(OBFUSCATE("java/util/Collections"));
    jclass NetworkInterface = env->FindClass(OBFUSCATE("java/net/NetworkInterface"));

    jmethodID _getNetworkInterfaces = env->GetStaticMethodID(NetworkInterface, OBFUSCATE("getNetworkInterfaces"), OBFUSCATE("()Ljava/util/Enumeration;"));
    jobject interfaces = env->CallStaticObjectMethod(NetworkInterface, _getNetworkInterfaces);
    jmethodID _list = env->GetStaticMethodID(Collections, OBFUSCATE("list"), OBFUSCATE("(Ljava/util/Enumeration;)Ljava/util/ArrayList;"));
    jmethodID _isUp = env->GetMethodID(NetworkInterface, OBFUSCATE("isUp"), OBFUSCATE("()Z"));
    jmethodID _getName = env->GetMethodID(NetworkInterface, OBFUSCATE("getName"), OBFUSCATE("()Ljava/lang/String;"));

    jobject interfaces_asList = env->CallStaticObjectMethod(Collections, _list, interfaces);

    jint len = env->CallIntMethod(interfaces_asList,java_util_ArrayList_size );
    string name = "";
    for (jint i = 0; i < len; i++) {
        jobject network = env->CallObjectMethod(interfaces_asList, java_util_ArrayList_get, i);
        jboolean isUpped = env->CallBooleanMethod(network, _isUp);
        if (isUpped){
            jstring jname = reinterpret_cast<jstring>(env->CallObjectMethod(network, _getName));
            name = JNIUtils::jstring2string(jname);

        }

        if (name.find(OBFUSCATE("tun")) != string::npos || name.find(OBFUSCATE("ppp"))  != string::npos || name.find(OBFUSCATE("pptp"))  != string::npos){
            return JNI_FALSE;
        }
    }
    return JNI_TRUE;
}

string JNIUtils::http_post(string site, string data, string token) {

    jclass jURL = env->FindClass(OBFUSCATE("java/net/URL"));
    jmethodID jURL_init = env->GetMethodID(jURL, OBFUSCATE("<init>"), OBFUSCATE("(Ljava/lang/String;)V"));
    jobject jurl = env->NewObject(jURL, jURL_init, env->NewStringUTF(site.c_str()));
    jmethodID jURL_openConnection = env->GetMethodID(jURL, OBFUSCATE("openConnection"),OBFUSCATE("()Ljava/net/URLConnection;"));
    jclass jHttpsURLConnection = env->FindClass(OBFUSCATE("javax/net/ssl/HttpsURLConnection"));
    jmethodID jCon_SetRequestMethod = env->GetMethodID(jHttpsURLConnection, OBFUSCATE("setRequestMethod"),OBFUSCATE("(Ljava/lang/String;)V"));
    //jmethodID jCon_getRequestProperty = env->GetMethodID(jHttpsURLConnection, OBFUSCATE("getRequestProperty"),OBFUSCATE("(Ljava/lang/String;)V"));
    jmethodID jCon_SetRequestProperty = env->GetMethodID(jHttpsURLConnection, OBFUSCATE("setRequestProperty"),OBFUSCATE("(Ljava/lang/String;Ljava/lang/String;)V"));
    jmethodID jCon_getResponseCode = env->GetMethodID(jHttpsURLConnection, OBFUSCATE("getResponseCode"),OBFUSCATE("()I"));
    jmethodID jCon_getInputStream = env->GetMethodID(jHttpsURLConnection, OBFUSCATE("getInputStream"),OBFUSCATE("()Ljava/io/InputStream;"));
    jmethodID jCon_getErrorStream = env->GetMethodID(jHttpsURLConnection, OBFUSCATE("getErrorStream"),OBFUSCATE("()Ljava/io/InputStream;"));
    jmethodID jCon_getOutputStram = env->GetMethodID(jHttpsURLConnection, OBFUSCATE("getOutputStream"),OBFUSCATE("()Ljava/io/OutputStream;"));
    jmethodID jCon_SetDoOutput = env->GetMethodID(jHttpsURLConnection, OBFUSCATE("setDoOutput"), OBFUSCATE("(Z)V"));
    jobject jConnection = env->CallObjectMethod(jurl, jURL_openConnection);
    env->CallVoidMethod(jConnection, jCon_SetRequestMethod, env->NewStringUTF(OBFUSCATE("POST")));
    env->CallVoidMethod(jConnection, jCon_SetRequestProperty, env->NewStringUTF(OBFUSCATE("Content-type")), env->NewStringUTF(OBFUSCATE("application/json")));
    //env->CallVoidMethod(jConnection, jCon_SetRequestProperty, env->NewStringUTF(OBFUSCATE("User-Agent")), env->CallObjectMethod(jConnection, jCon_getRequestProperty));
    env->CallVoidMethod(jConnection, jCon_SetRequestProperty, env->NewStringUTF(OBFUSCATE("Authorization")), env->NewStringUTF(token.c_str()));
    env->CallVoidMethod(jConnection, jCon_SetDoOutput, JNI_TRUE);
    jclass jInputStreamReader = env->FindClass(OBFUSCATE("java/io/InputStreamReader"));
    jmethodID jInputStreamReader_init = env->GetMethodID(jInputStreamReader, OBFUSCATE("<init>"),OBFUSCATE("(Ljava/io/InputStream;)V"));
    jclass jBufferedReader = env->FindClass(OBFUSCATE("java/io/BufferedReader"));
    jmethodID jBufferedReader_init = env->GetMethodID(jBufferedReader, OBFUSCATE("<init>"),OBFUSCATE("(Ljava/io/Reader;)V"));
    jmethodID jBufferedReader_readLine = env->GetMethodID(jBufferedReader, OBFUSCATE("readLine"),OBFUSCATE("()Ljava/lang/String;"));
    jclass jDataOutputStream = env->FindClass(OBFUSCATE("java/io/DataOutputStream"));
    jmethodID jDataOutputStream_init = env->GetMethodID(jDataOutputStream, OBFUSCATE("<init>"),OBFUSCATE("(Ljava/io/OutputStream;)V"));
    jmethodID jDataOutputStream_writeBytes = env->GetMethodID(jDataOutputStream, OBFUSCATE("writeBytes"),OBFUSCATE("(Ljava/lang/String;)V"));
    jmethodID jDataOutputStream_flush = env->GetMethodID(jDataOutputStream, OBFUSCATE("flush"), OBFUSCATE("()V"));
    jmethodID jDataOutputStream_close = env->GetMethodID(jDataOutputStream, OBFUSCATE("close"), OBFUSCATE("()V"));
    jobject jin;
    jobject jout = env->NewObject(jDataOutputStream, jDataOutputStream_init,env->CallObjectMethod(jConnection, jCon_getOutputStram));
    env->CallVoidMethod(jout, jDataOutputStream_writeBytes, env->NewStringUTF(data.c_str()));
    env->CallVoidMethod(jout, jDataOutputStream_flush);
    env->CallVoidMethod(jout, jDataOutputStream_close);

    jint responseCode = env->CallIntMethod(jConnection, jCon_getResponseCode);
    if (responseCode >= 400) {
        jobject erroStreamReader = env->NewObject(jInputStreamReader, jInputStreamReader_init,env->CallObjectMethod(jConnection,jCon_getErrorStream));
        jin = env->NewObject(jBufferedReader, jBufferedReader_init, erroStreamReader);
    } else {
        jobject inputStreamReader = env->NewObject(jInputStreamReader, jInputStreamReader_init,env->CallObjectMethod(jConnection,jCon_getInputStream));
        jin = env->NewObject(jBufferedReader, jBufferedReader_init, inputStreamReader);
    }

    jstring line;
    jclass jStringBuilder = env->FindClass(OBFUSCATE("java/lang/StringBuilder"));
    jmethodID jStringBuilder_init = env->GetMethodID(jStringBuilder, OBFUSCATE("<init>"), OBFUSCATE("()V"));
    jmethodID jStringBuilder_append = env->GetMethodID(jStringBuilder, OBFUSCATE("append"),OBFUSCATE("(Ljava/lang/String;)Ljava/lang/StringBuilder;"));
    jmethodID jStringBuilder_toString = env->GetMethodID(jStringBuilder, OBFUSCATE("toString"),OBFUSCATE("()Ljava/lang/String;"));
    jobject jsb = env->NewObject(jStringBuilder, jStringBuilder_init);
    while ((line = static_cast<jstring>(env->CallObjectMethod(jin, jBufferedReader_readLine))) != NULL) {
        env->CallObjectMethod(jsb, jStringBuilder_append, line);
    }

    jstring jresult = static_cast<jstring>(env->CallObjectMethod(jsb, jStringBuilder_toString));
    string result = jstring2string(jresult);


    return result;

}

jstring JNIUtils::GetSecureID(jobject jthiz) {
    jclass contextClass = env->FindClass(OBFUSCATE("android/content/ContextWrapper"));

    jmethodID getContentResolverMID = env->GetMethodID(contextClass, OBFUSCATE("getContentResolver"), OBFUSCATE("()Landroid/content/ContentResolver;"));
    jobject contentResolverObj = env->CallObjectMethod(jthiz, getContentResolverMID);
    jclass settingsSecureClass = env->FindClass(OBFUSCATE("android/provider/Settings$Secure"));
    jmethodID getStringMID = env->GetStaticMethodID(settingsSecureClass, OBFUSCATE("getString"), OBFUSCATE("(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;"));
    jstring androidId = static_cast<jstring> (env->CallStaticObjectMethod(settingsSecureClass, getStringMID, contentResolverObj, env->NewStringUTF(OBFUSCATE("android_id"))));


    return (androidId);

}












