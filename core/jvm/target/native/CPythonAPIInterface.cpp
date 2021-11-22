#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>

#include "me_shadaj_scalapy_interpreter_CPythonAPIInterface.h"

const void* getNativePointerFromJavaPointer(JNIEnv *env, jobject pointer) {
  jclass cls = env->GetObjectClass(pointer);
  jfieldID fid = env->GetFieldID(cls, "peer", "J");
  jlong javaPointer = env->GetLongField(pointer, fid);
  const void *nativePointer = (void *)javaPointer;
  return nativePointer;
}

jobject getJavaPointerFromNativePointer(JNIEnv *env, const void *pointer) {
    uintptr_t p = reinterpret_cast<std::uintptr_t>(pointer);

    jclass pointerClass = env->FindClass("com/sun/jna/Pointer");
    if (pointerClass == 0)
      return NULL;
    jmethodID pointerInit = env->GetMethodID(pointerClass, "createConstant", "(J)Lcom/sun/jna/Pointer;");
    jobject javaPointer = env->NewObject(pointerClass, pointerInit, p);
    return javaPointer;
}

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     Py_1SetProgramName
 * Signature:  (Lcom/sun/jna/WString;)V
 */
JNIEXPORT void JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_Py_1SetProgramName
  (JNIEnv *env, jobject obj, jobject wString) {
    jclass wStringClass = env->GetObjectClass(wString);
    jmethodID wStringToString = env->GetMethodID(wStringClass, "toString", "()Ljava/lang/String;");
    if (wStringClass == 0)
      std::cout << "This should not happen, might need to throw exception";
    jstring javaString = (jstring)env->CallObjectMethod(wString, wStringToString);
    const char* str = env->GetStringUTFChars(javaString, NULL);
    size_t len = env->GetStringUTFLength(javaString);
    Py_SetProgramName(Py_DecodeLocale(str, &len));
    env->ReleaseStringUTFChars(javaString, str);
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     Py_1Initialize
 * Signature:  ()V
 */
JNIEXPORT void JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_Py_1Initialize
  (JNIEnv *env, jobject obj) {
    std::cout << "Just checking if this works";
    Py_Initialize();
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     Py_1DecodeLocale
 * Signature:  (Ljava/lang/String;Lcom/sun/jna/Pointer;)Lcom/sun/jna/WString;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_Py_1DecodeLocale
  (JNIEnv *env, jobject obj, jstring str, jobject size) {
    jclass wStringClass = env->FindClass("com/sun/jna/WString");
    if (wStringClass == 0)
      return NULL;
    jmethodID wStringInit = env->GetMethodID(wStringClass, "<init>", "(Ljava/lang/String;)V");
    jobject wString = env->NewObject(wStringClass, wStringInit, str);
    return wString;
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyMem_1RawFree
 * Signature:  (Lcom/sun/jna/Pointer;)V
 */
JNIEXPORT void JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyMem_1RawFree
  (JNIEnv *env, jobject obj, jobject p) {
    PyMem_RawFree(p); // Mostly sure that this is not how it is done, though not sure what to do with the const when using getNativePointerFromJavaPointer
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyEval_1SaveThread
 * Signature:  ()Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyEval_1SaveThread
  (JNIEnv *env, jobject thisObject) {
    return getJavaPointerFromNativePointer(env, PyEval_SaveThread());
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyGILState_1Ensure
 * Signature:  ()I
 */
JNIEXPORT jint JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyGILState_1Ensure
  (JNIEnv *env, jobject obj) {
    return PyGILState_Ensure();
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyGILState_1Release
 * Signature:  (I)V
 */
JNIEXPORT void JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyGILState_1Release
  (JNIEnv *env, jobject obj, jint state) {
    PyGILState_Release((PyGILState_STATE)state);
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyRun_1String
 * Signature:  (Ljava/lang/String;ILcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyRun_1String
  (JNIEnv *env, jobject obj, jstring javaString, jint start, jobject globals, jobject locals) {
    const char* str = env->GetStringUTFChars(javaString, NULL);
    PyObject* glob = (PyObject*) getNativePointerFromJavaPointer(env, globals);
    PyObject* loc = (PyObject*) getNativePointerFromJavaPointer(env, locals);
    jobject javaPointer = getJavaPointerFromNativePointer(env, PyRun_String(str, start, glob, loc));
    env->ReleaseStringUTFChars(javaString, str);
    return javaPointer;
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyUnicode_1FromString
 * Signature:  (Ljava/lang/String;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyUnicode_1FromString
  (JNIEnv *env, jobject obj, jstring javaString) {
    const char* str = env->GetStringUTFChars(javaString, NULL);
    jobject javaPointer = getJavaPointerFromNativePointer(env, PyUnicode_FromString(str));
    env->ReleaseStringUTFChars(javaString, str);
    return javaPointer;
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyUnicode_1AsUTF8
 * Signature:  (Lcom/sun/jna/Pointer;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyUnicode_1AsUTF8
  (JNIEnv *env, jobject obj, jobject pointer) {
    PyObject* unicode = (PyObject*) getNativePointerFromJavaPointer(env, pointer);
    return getJavaPointerFromNativePointer(env, PyUnicode_AsUTF8(unicode));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyBool_1FromLong
 * Signature:  (Lcom/sun/jna/NativeLong;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyBool_1FromLong
  (JNIEnv *env, jobject obj, jobject nativeLong) {
    jclass nativeLongClass = env->GetObjectClass(nativeLong);
    if (nativeLongClass == 0)
      return NULL;
    jmethodID valueMethod = env->GetMethodID(nativeLongClass, "longValue", "()J");
    jlong longValue = env->CallLongMethod(nativeLongClass, valueMethod);
    return getJavaPointerFromNativePointer(env, PyBool_FromLong(longValue));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyNumber_1Negative
 * Signature:  (Lcom/sun/jna/Pointer;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyNumber_1Negative
  (JNIEnv *env, jobject obj, jobject o1) {
    PyObject *num = (PyObject*) getNativePointerFromJavaPointer(env, o1);
    return getJavaPointerFromNativePointer(env, PyNumber_Negative(num));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyNumber_1Positive
 * Signature:  (Lcom/sun/jna/Pointer;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyNumber_1Positive
  (JNIEnv *env, jobject obj, jobject o1) {
    PyObject *num = (PyObject*) getNativePointerFromJavaPointer(env, o1);
    return getJavaPointerFromNativePointer(env, PyNumber_Positive(num));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyNumber_1Add
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyNumber_1Add
  (JNIEnv *env, jobject obj, jobject o1, jobject o2) {
    PyObject *num1 = (PyObject*) getNativePointerFromJavaPointer(env, o1);
    PyObject *num2 = (PyObject*) getNativePointerFromJavaPointer(env, o2);
    return getJavaPointerFromNativePointer(env, PyNumber_Add(num1, num2));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyNumber_1Subtract
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyNumber_1Subtract
  (JNIEnv *env, jobject obj, jobject o1, jobject o2) {
    PyObject *num1 = (PyObject*) getNativePointerFromJavaPointer(env, o1);
    PyObject *num2 = (PyObject*) getNativePointerFromJavaPointer(env, o2);
    return getJavaPointerFromNativePointer(env, PyNumber_Subtract(num1, num2));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyNumber_1Multiply
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyNumber_1Multiply
  (JNIEnv *env, jobject obj, jobject o1, jobject o2) {
    PyObject *num1 = (PyObject*) getNativePointerFromJavaPointer(env, o1);
    PyObject *num2 = (PyObject*) getNativePointerFromJavaPointer(env, o2);
    return getJavaPointerFromNativePointer(env, PyNumber_Multiply(num1, num2));
  }
/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyNumber_1TrueDivide
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyNumber_1TrueDivide
  (JNIEnv *env, jobject obj, jobject o1, jobject o2) {
    PyObject *num1 = (PyObject*) getNativePointerFromJavaPointer(env, o1);
    PyObject *num2 = (PyObject*) getNativePointerFromJavaPointer(env, o2);
    return getJavaPointerFromNativePointer(env, PyNumber_TrueDivide(num1, num2));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyNumber_1Remainder
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyNumber_1Remainder
  (JNIEnv *env, jobject obj, jobject o1, jobject o2) {
    PyObject *num1 = (PyObject*) getNativePointerFromJavaPointer(env, o1);
    PyObject *num2 = (PyObject*) getNativePointerFromJavaPointer(env, o2);
    return getJavaPointerFromNativePointer(env, PyNumber_Remainder(num1, num2));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyLong_1FromLongLong
 * Signature:  (J)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyLong_1FromLongLong
  (JNIEnv *env, jobject obj, jlong o1) {
    return getJavaPointerFromNativePointer(env, PyLong_FromLongLong(o1));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyLong_1AsLong
 * Signature:  (Lcom/sun/jna/Pointer;)I
 */
JNIEXPORT jint JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyLong_1AsLong
  (JNIEnv *env, jobject obj, jobject pyLong) {
    PyObject *num = (PyObject*) getNativePointerFromJavaPointer(env, pyLong);
    return PyLong_AsLong(num);
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyLong_1AsLongLong
 * Signature:  (Lcom/sun/jna/Pointer;)J
 */
JNIEXPORT jlong JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyLong_1AsLongLong
  (JNIEnv *env, jobject obj, jobject pyLong) {
    PyObject *num = (PyObject*) getNativePointerFromJavaPointer(env, pyLong);
    return PyLong_AsLongLong(num);
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyFloat_1FromDouble
 * Signature:  (D)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyFloat_1FromDouble
  (JNIEnv *env, jobject obj, jdouble o1) {
    return getJavaPointerFromNativePointer(env, PyFloat_FromDouble(o1));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyFloat_1AsDouble
 * Signature:  (Lcom/sun/jna/Pointer;)D
 */
JNIEXPORT jdouble JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyFloat_1AsDouble
  (JNIEnv *env, jobject obj, jobject pyDouble) {
    PyObject *num = (PyObject*) getNativePointerFromJavaPointer(env, pyDouble);
    return PyFloat_AsDouble(num);
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyDict_1New
 * Signature:  ()Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyDict_1New
  (JNIEnv *env, jobject obj) {
    return getJavaPointerFromNativePointer(env, PyDict_New());
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyDict_1SetItem
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)I
 */
JNIEXPORT jint JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyDict_1SetItem
  (JNIEnv *env, jobject obj, jobject dict, jobject key, jobject value) {
    PyObject *dictObj = (PyObject*) getNativePointerFromJavaPointer(env, dict);
    PyObject *keyObj = (PyObject*) getNativePointerFromJavaPointer(env, key);
    PyObject *valueObj = (PyObject*) getNativePointerFromJavaPointer(env, value);
    return PyDict_SetItem(dictObj, keyObj, valueObj);
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyDict_1SetItemString
 * Signature:  (Lcom/sun/jna/Pointer;Ljava/lang/String;Lcom/sun/jna/Pointer;)I
 */
JNIEXPORT jint JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyDict_1SetItemString
  (JNIEnv *env, jobject obj, jobject dict, jstring key, jobject value) {
    PyObject *dictObj = (PyObject*) getNativePointerFromJavaPointer(env, dict);
    const char* keyStr = env->GetStringUTFChars(key, NULL);
    PyObject *valueObj = (PyObject*) getNativePointerFromJavaPointer(env, value);
    int i = PyDict_SetItemString(dictObj, keyStr, valueObj);
    env->ReleaseStringUTFChars(key, keyStr);
    return i;
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyDict_1Contains
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)I
 */
JNIEXPORT jint JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyDict_1Contains
  (JNIEnv *env, jobject obj, jobject dict, jobject key) {
    PyObject *dictObj = (PyObject*) getNativePointerFromJavaPointer(env, dict);
    PyObject *keyObj = (PyObject*) getNativePointerFromJavaPointer(env, key);
    return PyDict_Contains(dictObj, keyObj);
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyDict_1GetItem
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyDict_1GetItem
  (JNIEnv *env, jobject obj, jobject dict, jobject key) {
    PyObject *dictObj = (PyObject*) getNativePointerFromJavaPointer(env, dict);
    PyObject *keyObj = (PyObject*) getNativePointerFromJavaPointer(env, key);
    return getJavaPointerFromNativePointer(env, PyDict_GetItem(dictObj, keyObj));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyDict_1GetItemString
 * Signature:  (Lcom/sun/jna/Pointer;Ljava/lang/String;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyDict_1GetItemString
  (JNIEnv *env, jobject obj, jobject dict, jstring key) {
    PyObject *dictObj = (PyObject*) getNativePointerFromJavaPointer(env, dict);
    const char* keyStr = env->GetStringUTFChars(key, NULL);
    jobject pointer = getJavaPointerFromNativePointer(env, PyDict_GetItemString(dictObj, keyStr));
    env->ReleaseStringUTFChars(key, keyStr);
    return pointer;
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyDict_1GetItemWithError
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyDict_1GetItemWithError
  (JNIEnv *env, jobject obj, jobject dict, jobject key) {
    PyObject *dictObj = (PyObject*) getNativePointerFromJavaPointer(env, dict);
    PyObject *keyObj = (PyObject*) getNativePointerFromJavaPointer(env, key);
    return getJavaPointerFromNativePointer(env, PyDict_GetItemWithError(dictObj, keyObj));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyDict_1DelItemString
 * Signature:  (Lcom/sun/jna/Pointer;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyDict_1DelItemString
  (JNIEnv *env, jobject obj, jobject dict, jstring key) {
    PyObject *dictObj = (PyObject*) getNativePointerFromJavaPointer(env, dict);
    const char* keyStr = env->GetStringUTFChars(key, NULL);
    int i = PyDict_DelItemString(dictObj, keyStr);
    env->ReleaseStringUTFChars(key, keyStr);
    return i;
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyDict_1Keys
 * Signature:  (Lcom/sun/jna/Pointer;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyDict_1Keys
  (JNIEnv *env, jobject obj, jobject dict) {
    PyObject *dictObj = (PyObject*) getNativePointerFromJavaPointer(env, dict);
    return getJavaPointerFromNativePointer(env, PyDict_Keys(dictObj));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyList_1New
 * Signature:  (I)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyList_1New
  (JNIEnv *env, jobject obj, jint size) {
    return getJavaPointerFromNativePointer(env, PyList_New(size));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyList_1Size
 * Signature:  (Lcom/sun/jna/Pointer;)Lcom/sun/jna/NativeLong;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyList_1Size
  (JNIEnv *env, jobject obj, jobject list) {
    PyObject *listObj = (PyObject*) getNativePointerFromJavaPointer(env, list);
    auto size = PyList_Size(listObj);

    jclass nativeLongClass = env->FindClass("com/sun/jna/NativeLong");
    if (nativeLongClass == 0)
      return NULL;
    jmethodID longInit = env->GetMethodID(nativeLongClass, "<init>", "(J)V;");
    jobject nativeLong = env->NewObject(nativeLongClass, longInit, size);
    return nativeLong;
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyList_1GetItem
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/NativeLong;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyList_1GetItem
  (JNIEnv *env, jobject obj, jobject list, jobject index) {
    jclass nativeLongClass = env->GetObjectClass(index);
    if (nativeLongClass == 0)
      return NULL;
    jmethodID valueMethod = env->GetMethodID(nativeLongClass, "longValue", "()J");
    jlong longValue = env->CallLongMethod(nativeLongClass, valueMethod);

    PyObject *listObj = (PyObject*) getNativePointerFromJavaPointer(env, list);
    return getJavaPointerFromNativePointer(env, PyList_GetItem(listObj, longValue));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyList_1SetItem
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/NativeLong;Lcom/sun/jna/Pointer;)I
 */
JNIEXPORT jint JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyList_1SetItem
  (JNIEnv *env, jobject obj, jobject list, jobject index, jobject item) {
    jclass nativeLongClass = env->GetObjectClass(index);
    if (nativeLongClass == 0)
      return NULL;
    jmethodID valueMethod = env->GetMethodID(nativeLongClass, "longValue", "()J");
    jlong longValue = env->CallLongMethod(nativeLongClass, valueMethod);

    PyObject *listObj = (PyObject*) getNativePointerFromJavaPointer(env, list);
    PyObject *itemObj = (PyObject*) getNativePointerFromJavaPointer(env, item);
    return PyList_SetItem(listObj, longValue, itemObj);
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyTuple_1New
 * Signature:  (I)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyTuple_1New
  (JNIEnv *env, jobject obj, jint size) {
    return getJavaPointerFromNativePointer(env, PyTuple_New(size));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyTuple_1Size
 * Signature:  (Lcom/sun/jna/Pointer;)Lcom/sun/jna/NativeLong;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyTuple_1Size
  (JNIEnv *env, jobject obj, jobject tuple) {
    PyObject *tupleObj = (PyObject*) getNativePointerFromJavaPointer(env, tuple);
    auto size = PyTuple_Size(tupleObj);

    jclass nativeLongClass = env->FindClass("com/sun/jna/NativeLong");
    if (nativeLongClass == 0)
      return NULL;
    jmethodID longInit = env->GetMethodID(nativeLongClass, "<init>", "(J)V;");
    jobject nativeLong = env->NewObject(nativeLongClass, longInit, size);
    return nativeLong;
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyTuple_1GetItem
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/NativeLong;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyTuple_1GetItem
  (JNIEnv *env, jobject obj, jobject tuple, jobject index) {
    jclass nativeLongClass = env->GetObjectClass(index);
    if (nativeLongClass == 0)
      return NULL;
    jmethodID valueMethod = env->GetMethodID(nativeLongClass, "longValue", "()J");
    jlong longValue = env->CallLongMethod(nativeLongClass, valueMethod);

    PyObject *tupleObj = (PyObject*) getNativePointerFromJavaPointer(env, tuple);
    return getJavaPointerFromNativePointer(env, PyTuple_GetItem(tupleObj, longValue));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyTuple_1SetItem
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/NativeLong;Lcom/sun/jna/Pointer;)I
 */
JNIEXPORT jint JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyTuple_1SetItem
  (JNIEnv *env, jobject obj, jobject tuple, jobject index, jobject item) {
    jclass nativeLongClass = env->GetObjectClass(index);
    if (nativeLongClass == 0)
      return NULL;
    jmethodID valueMethod = env->GetMethodID(nativeLongClass, "longValue", "()J");
    jlong longValue = env->CallLongMethod(nativeLongClass, valueMethod);

    PyObject *tupleObj = (PyObject*) getNativePointerFromJavaPointer(env, tuple);
    PyObject *itemObj = (PyObject*) getNativePointerFromJavaPointer(env, item);
    return PyList_SetItem(tupleObj, longValue, itemObj);
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyObject_1Str
 * Signature:  (Lcom/sun/jna/Pointer;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyObject_1Str
  (JNIEnv *env, jobject obj, jobject object) {
    PyObject *objObj = (PyObject*) getNativePointerFromJavaPointer(env, object);
    return getJavaPointerFromNativePointer(env, PyObject_Str(objObj));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyObject_1GetItem
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyObject_1GetItem
  (JNIEnv *env, jobject obj, jobject object, jobject idx) {
    PyObject *objObj = (PyObject*) getNativePointerFromJavaPointer(env, object);
    PyObject *idxObj = (PyObject*) getNativePointerFromJavaPointer(env, idx);
    return getJavaPointerFromNativePointer(env, PyObject_GetItem(objObj, idxObj));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyObject_1SetItem
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)I
 */
JNIEXPORT jint JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyObject_1SetItem
  (JNIEnv *env, jobject obj, jobject object, jobject idx, jobject newValue) {
    PyObject *objObj = (PyObject*) getNativePointerFromJavaPointer(env, object);
    PyObject *idxObj = (PyObject*) getNativePointerFromJavaPointer(env, idx);
    PyObject *valueObj = (PyObject*) getNativePointerFromJavaPointer(env, newValue);
    return PyObject_SetItem(objObj, idxObj, valueObj);
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyObject_1DelItem
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)I
 */
JNIEXPORT jint JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyObject_1DelItem
  (JNIEnv *env, jobject obj, jobject object, jobject idx) {
    PyObject *objObj = (PyObject*) getNativePointerFromJavaPointer(env, object);
    PyObject *idxObj = (PyObject*) getNativePointerFromJavaPointer(env, idx);
    return PyObject_DelItem(objObj, idxObj);
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyObject_1GetAttr
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyObject_1GetAttr
  (JNIEnv *env, jobject obj, jobject object, jobject idx) {
    PyObject *objObj = (PyObject*) getNativePointerFromJavaPointer(env, object);
    PyObject *idxObj = (PyObject*) getNativePointerFromJavaPointer(env, idx);
    return getJavaPointerFromNativePointer(env, PyObject_GetAttr(objObj, idxObj));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyObject_1GetAttrString
 * Signature:  (Lcom/sun/jna/Pointer;Ljava/lang/String;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyObject_1GetAttrString
  (JNIEnv *env, jobject obj, jobject object, jstring name) {
    PyObject *objObj = (PyObject*) getNativePointerFromJavaPointer(env, object);
    const char *str = env->GetStringUTFChars(name, NULL);
    jobject p = getJavaPointerFromNativePointer(env, PyObject_GetAttrString(objObj, str));
    env->ReleaseStringUTFChars(name, str);
    return p;
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyObject_1SetAttr
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)I;
 */
JNIEXPORT jint JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyObject_1SetAttr
  (JNIEnv *env, jobject obj, jobject object, jobject name, jobject newValue) {
    PyObject *objObj = (PyObject*) getNativePointerFromJavaPointer(env, object);
    PyObject *nameObj = (PyObject*) getNativePointerFromJavaPointer(env, name);
    PyObject *valueObj = (PyObject*) getNativePointerFromJavaPointer(env, newValue);
    return PyObject_SetAttr(objObj, nameObj, valueObj);
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyObject_1SetAttrString
 * Signature:  (Lcom/sun/jna/Pointer;Ljava/lang/String;Lcom/sun/jna/Pointer;)I;
 */
JNIEXPORT jint JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyObject_1SetAttrString
  (JNIEnv *env, jobject obj, jobject object, jstring name, jobject newValue) {
    PyObject *objObj = (PyObject*) getNativePointerFromJavaPointer(env, object);
    const char *str = env->GetStringUTFChars(name, NULL);
    PyObject *valueObj = (PyObject*) getNativePointerFromJavaPointer(env, newValue);
    jint i = PyObject_SetAttrString(objObj, str, valueObj);
    env->ReleaseStringUTFChars(name, str);
    return i;
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyObject_1Call
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyObject_1Call
  (JNIEnv *env, jobject obj, jobject object, jobject args, jobject kwargs) {
    PyObject *objObj = (PyObject*) getNativePointerFromJavaPointer(env, object);
    PyObject *argsObj = (PyObject*) getNativePointerFromJavaPointer(env, args);
    PyObject *kwargsObj = (PyObject*) getNativePointerFromJavaPointer(env, kwargs);
    return getJavaPointerFromNativePointer(env, PyObject_Call(objObj, argsObj, kwargsObj));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyObject_1Length
 * Signature:  (Lcom/sun/jna/Pointer;)Lcom/sun/jna/NativeLong;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyObject_1Length
  (JNIEnv *env, jobject obj, jobject object) {
    PyObject *objObj = (PyObject*) getNativePointerFromJavaPointer(env, object);
    auto size = PyObject_Length(objObj);

    jclass nativeLongClass = env->FindClass("com/sun/jna/NativeLong");
    if (nativeLongClass == 0)
      return NULL;
    jmethodID longInit = env->GetMethodID(nativeLongClass, "<init>", "(J)V;");
    jobject nativeLong = env->NewObject(nativeLongClass, longInit, size);
    return nativeLong;
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PySequence_1GetItem
 * Signature:  (Lcom/sun/jna/Pointer;I)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PySequence_1GetItem
  (JNIEnv *env, jobject obj, jobject seq, jint idx) {
    PyObject *seqObj = (PyObject*) getNativePointerFromJavaPointer(env, seq);
    return getJavaPointerFromNativePointer(env, PySequence_GetItem(seqObj, idx));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PySequence_1SetItem
 * Signature:  (Lcom/sun/jna/Pointer;ILcom/sun/jna/Pointer;)I;
 */
JNIEXPORT jint JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PySequence_1SetItem
  (JNIEnv *env, jobject obj, jobject seq, jint idx, jobject v) {
    PyObject *seqObj = (PyObject*) getNativePointerFromJavaPointer(env, seq);
    PyObject *vObj = (PyObject*) getNativePointerFromJavaPointer(env, v);
    return PySequence_SetItem(seqObj, idx, vObj);
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PySequence_1Length
 * Signature:  (Lcom/sun/jna/Pointer;)Lcom/sun/jna/NativeLong;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PySequence_1Length
  (JNIEnv *env, jobject obj, jobject seq) {
    PyObject *seqObj = (PyObject*) getNativePointerFromJavaPointer(env, seq);
    auto size = PySequence_Length(seqObj);

    jclass nativeLongClass = env->FindClass("com/sun/jna/NativeLong");
    if (nativeLongClass == 0)
      return NULL;
    jmethodID longInit = env->GetMethodID(nativeLongClass, "<init>", "(J)V;");
    jobject nativeLong = env->NewObject(nativeLongClass, longInit, size);
    return nativeLong;
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyErr_1Occurred
 * Signature:  ()Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyErr_1Occurred
  (JNIEnv *env, jobject obj) {
    return getJavaPointerFromNativePointer(env, PyErr_Occurred());
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyErr_1Fetch
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)V
 */
JNIEXPORT void JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyErr_1Fetch
  (JNIEnv *env, jobject obj, jobject pType, jobject pValue, jobject pTraceback) {
    PyObject *pTypeObj = (PyObject*) getNativePointerFromJavaPointer(env, pType);
    PyObject *pValueObj = (PyObject*) getNativePointerFromJavaPointer(env, pValue);
    PyObject *pTracebackObj = (PyObject*) getNativePointerFromJavaPointer(env, pTraceback);
    PyErr_Fetch(&pTypeObj, &pValueObj, &pTracebackObj);
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyErr_1Print
 * Signature:  ()V
 */
JNIEXPORT void JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyErr_1Print
  (JNIEnv *env, jobject obj) {
    PyErr_Print();
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyErr_1Clear
 * Signature:  ()V
 */
JNIEXPORT void JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyErr_1Clear
  (JNIEnv *env, jobject obj) {
    PyErr_Clear();
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyEval_1GetBuiltins
 * Signature:  ()Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyEval_1GetBuiltins
  (JNIEnv *env, jobject obj) {
    return getJavaPointerFromNativePointer(env, PyEval_GetBuiltins());
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     Py_1BuildValue
 * Signature:  (Ljava/lang/String;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_Py_1BuildValue
  (JNIEnv *env, jobject obj, jstring javaString) {
    const char *str = env->GetStringUTFChars(javaString, NULL);
    jobject p = getJavaPointerFromNativePointer(env, Py_BuildValue(str));
    env->ReleaseStringUTFChars(javaString, str);
    return p;
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyLong_1FromVoidPtr
 * Signature:  (Lcom/sun/jna/Pointer;)V
 */
JNIEXPORT void JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyLong_1FromVoidPtr
  (JNIEnv *env, jobject obj, jobject ptr) {
    PyLong_FromVoidPtr((void*)getNativePointerFromJavaPointer(env, ptr));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyCFunction_1NewEx
 * Signature:  (Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;Lcom/sun/jna/Pointer;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyCFunction_1NewEx
  (JNIEnv *env, jobject obj, jobject ptr, jobject self, jobject module) {
    PyMethodDef *ptrObj = (PyMethodDef*) getNativePointerFromJavaPointer(env, ptr);
    PyObject *selfObj = (PyObject*) getNativePointerFromJavaPointer(env, self);
    PyObject *moduleObj = (PyObject*) getNativePointerFromJavaPointer(env, module);
    return getJavaPointerFromNativePointer(env, PyCFunction_NewEx(ptrObj, selfObj, moduleObj));
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyImport_1ImportModule
 * Signature:  (Ljava/lang/String;)Lcom/sun/jna/Pointer;
 */
JNIEXPORT jobject JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyImport_1ImportModule
  (JNIEnv *env, jobject obj, jstring name) {
    const char *str = env->GetStringUTFChars(name, NULL);
    jobject p = getJavaPointerFromNativePointer(env, PyImport_ImportModule(str));
    env->ReleaseStringUTFChars(name, str);
    return p;
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     PyErr_1SetString
 * Signature:  (Lcom/sun/jna/Pointer;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_PyErr_1SetString
  (JNIEnv *env, jobject obj, jobject tpe, jstring message) {
    const char *str = env->GetStringUTFChars(message, NULL);
    PyObject *tpeObj = (PyObject*) getNativePointerFromJavaPointer(env, tpe);
    PyErr_SetString(tpeObj, str);
    env->ReleaseStringUTFChars(message, str);
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     Py_1IncRef
 * Signature:  (Lcom/sun/jna/Pointer;)V
 */
JNIEXPORT void JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_Py_1IncRef
  (JNIEnv *env, jobject obj, jobject ptr) {
    PyObject *ptrObj = (PyObject*) getNativePointerFromJavaPointer(env, ptr);
    Py_IncRef(ptrObj);
  }

/*
 * Class:      me_shadaj_scalapy_interpreter_CPythonAPIInterface
 * Method:     Py_1DecRef
 * Signature:  (Lcom/sun/jna/Pointer;)V
 */
JNIEXPORT void JNICALL Java_me_shadaj_scalapy_interpreter_CPythonAPIInterface_Py_1DecRef
  (JNIEnv *env, jobject obj, jobject ptr) {
    PyObject *ptrObj = (PyObject*) getNativePointerFromJavaPointer(env, ptr);
    Py_DecRef(ptrObj);
  }
