package me.shadaj.scalapy.interpreter

import com.github.sbt.jni.nativeLoader
import com.sun.jna.{Memory, Native, NativeLong, WString}

import scala.util.{Failure, Properties, Success}

@nativeLoader("scalapy0")
class CPythonAPIInterface {
//  val pythonLibrariesToTry =
//    Option(System.getenv("SCALAPY_PYTHON_LIBRARY"))
//      .orElse(Properties.propOrNone("scalapy.python.library"))
//      .toSeq ++
//      Seq(
//        "python3",
//        "python3.7", "python3.7m",
//        "python3.8", "python3.8m",
//        "python3.9", "python3.9m"
//      )
//
//  val loadAttempts = pythonLibrariesToTry.toStream.map(n => try {
//    Native.register(n)
//    Success(true)
//  } catch {
//    case t: Throwable => Failure(t)
//  })
//
//  loadAttempts.find(_.isSuccess).getOrElse {
//    loadAttempts.foreach(_.failed.get.printStackTrace())
//    throw new Exception(s"Unable to locate Python library, tried ${pythonLibrariesToTry.mkString(", ")}")
//  }

  @native def Py_SetProgramName(str: WString): Unit
  @native def Py_Initialize(): Unit

  @native def Py_DecodeLocale(str: String, size: Platform.Pointer): WString

  @native def PyMem_RawFree(p: Platform.Pointer): Unit

  @native def PyEval_SaveThread(): Platform.Pointer
  @native def PyGILState_Ensure(): Int
  @native def PyGILState_Release(state: Int): Unit

  @native def PyRun_String(str: String, start: Int, globals: Platform.Pointer, locals: Platform.Pointer): Platform.Pointer

  @native def PyUnicode_FromString(cStr: String): Platform.Pointer
  @native def PyUnicode_AsUTF8(pyString: Platform.Pointer): Platform.Pointer

  @native def PyBool_FromLong(long: NativeLong): Platform.Pointer

  @native def PyNumber_Negative(o1: Platform.Pointer): Platform.Pointer
  @native def PyNumber_Positive(o1: Platform.Pointer): Platform.Pointer
  @native def PyNumber_Add(o1: Platform.Pointer, o2: Platform.Pointer): Platform.Pointer
  @native def PyNumber_Subtract(o1: Platform.Pointer, o2: Platform.Pointer): Platform.Pointer
  @native def PyNumber_Multiply(o1: Platform.Pointer, o2: Platform.Pointer): Platform.Pointer
  @native def PyNumber_TrueDivide(o1: Platform.Pointer, o2: Platform.Pointer): Platform.Pointer
  @native def PyNumber_Remainder(o1: Platform.Pointer, o2: Platform.Pointer): Platform.Pointer

  @native def PyLong_FromLongLong(long: Long): Platform.Pointer
  @native def PyLong_AsLong(pyLong: Platform.Pointer): Int
  @native def PyLong_AsLongLong(pyLong: Platform.Pointer): Long

  @native def PyFloat_FromDouble(double: Double): Platform.Pointer
  @native def PyFloat_AsDouble(float: Platform.Pointer): Double

  @native def PyDict_New(): Platform.Pointer
  @native def PyDict_SetItem(dict: Platform.Pointer, key: Platform.Pointer, value: Platform.Pointer): Int
  @native def PyDict_SetItemString(dict: Platform.Pointer, key: String, value: Platform.Pointer): Int
  @native def PyDict_Contains(dict: Platform.Pointer, key: Platform.Pointer): Int
  @native def PyDict_GetItem(dict: Platform.Pointer, key: Platform.Pointer): Platform.Pointer
  @native def PyDict_GetItemString(dict: Platform.Pointer, key: String): Platform.Pointer
  @native def PyDict_GetItemWithError(dict: Platform.Pointer, key: Platform.Pointer): Platform.Pointer
  @native def PyDict_DelItemString(dict: Platform.Pointer, key: String): Int
  @native def PyDict_Keys(dict: Platform.Pointer): Platform.Pointer

  @native def PyList_New(size: Int): Platform.Pointer
  @native def PyList_Size(list: Platform.Pointer): NativeLong
  @native def PyList_GetItem(list: Platform.Pointer, index: NativeLong): Platform.Pointer
  @native def PyList_SetItem(list: Platform.Pointer, index: NativeLong, item: Platform.Pointer): Int

  @native def PyTuple_New(size: Int): Platform.Pointer
  @native def PyTuple_Size(tuple: Platform.Pointer): NativeLong
  @native def PyTuple_GetItem(tuple: Platform.Pointer, index: NativeLong): Platform.Pointer
  @native def PyTuple_SetItem(tuple: Platform.Pointer, index: NativeLong, item: Platform.Pointer): Int

  @native def PyObject_Str(obj: Platform.Pointer): Platform.Pointer
  @native def PyObject_GetItem(obj: Platform.Pointer, idx: Platform.Pointer): Platform.Pointer
  @native def PyObject_SetItem(obj: Platform.Pointer, key: Platform.Pointer, newValue: Platform.Pointer): Int
  @native def PyObject_DelItem(obj: Platform.Pointer, idx: Platform.Pointer): Int
  @native def PyObject_GetAttr(obj: Platform.Pointer, name: Platform.Pointer): Platform.Pointer
  @native def PyObject_GetAttrString(obj: Platform.Pointer, name: String): Platform.Pointer
  @native def PyObject_SetAttr(obj: Platform.Pointer, name: Platform.Pointer, newValue: Platform.Pointer): Int
  @native def PyObject_SetAttrString(obj: Platform.Pointer, name: String, newValue: Platform.Pointer): Int
  @native def PyObject_Call(obj: Platform.Pointer, args: Platform.Pointer, kwArgs: Platform.Pointer): Platform.Pointer
  @native def PyObject_Length(obj: Platform.Pointer): NativeLong

  @native def PySequence_GetItem(obj: Platform.Pointer, idx: Int): Platform.Pointer
  @native def PySequence_SetItem(obj: Platform.Pointer, idx: Int, v: Platform.Pointer): Int
  @native def PySequence_Length(obj: Platform.Pointer): NativeLong

  @native def PyErr_Occurred(): Platform.Pointer
  @native def PyErr_Fetch(pType: Platform.PointerToPointer, pValue: Platform.PointerToPointer, pTraceback: Platform.PointerToPointer): Unit
  @native def PyErr_Print(): Unit
  @native def PyErr_Clear(): Unit

  @native def PyEval_GetBuiltins(): Platform.Pointer

  @native def Py_BuildValue(str: String): Platform.Pointer

  @native def PyLong_FromVoidPtr(ptr: Platform.Pointer): Unit
  @native def PyCFunction_NewEx(ptr: Platform.Pointer, self: Platform.Pointer, module: Platform.Pointer): Platform.Pointer
  @native def PyImport_ImportModule(str: String): Platform.Pointer

  @native def PyErr_SetString(tpe: Platform.Pointer, message: String): Unit

  @native def Py_IncRef(ptr: Platform.Pointer): Unit
  @native def Py_DecRef(ptr: Platform.Pointer): Unit
}

object CPythonAPI extends CPythonAPIInterface
