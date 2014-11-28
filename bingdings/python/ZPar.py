#!/usr/bin/env python

from ctypes import *
import sys

class ZPar:
  def __init__(self, model, task, lang, libpath=None, lib="libzpar.so"):
    self.__closed__ = False
    self.libzpar = cdll.LoadLibrary(lib)
    self.lang = lang
    model = c_char_p(model)
    task = c_char_p(task)
    if self.lang == "english":
      self._open = self.libzpar.zpar_open_english
      self._close = self.libzpar.zpar_close_english
      self._process = self.libzpar.zpar_process_english
      self._release = self.libzpar.zpar_release_english
      self._batch = self.libzpar.zpar_batch_english
    elif self.lang == "chinese":
      self._open = self.libzpar.zpar_open_chinese
      self._close = self.libzpar.zpar_close_chinese
      self._process = self.libzpar.zpar_process_chinese
      self._release = self.libzpar.zpar_release_chinese
      self._batch = self.libzpar.zpar_batch_chinese
    elif self.lang == "generic":
      self._open = self.libzpar.zpar_open_generic
      self._close = self.libzpar.zpar_close_generic
      self._process = self.libzpar.zpar_process_generic
      self._release = self.libzpar.zpar_release_generic
      self._batch = self.libzpar.zpar_batch_generic
    else :
      raise "No such language: " + lang + " in ZPar."
    self._open.argtypes = [POINTER(c_char), POINTER(c_char)]
    self._open.restype = c_void_p
    self._process.argtypes = [ c_void_p, POINTER(c_char), POINTER(c_size_t)]
    self._process.restype = POINTER(c_char)
    self._release.argtypes = [POINTER(c_char)]
    self._release.restype = c_int
    self._close.argtypes = [c_void_p]
    self._close.restype = c_int
    self._batch.argtypes = [c_void_p, POINTER(c_char), POINTER(c_char)]
    self._batch.restype = c_int

    self.ZPAR = self._open(model, task)
  def process(self, line):
    sz = c_size_t(len(line))
    _res = self._process(self.ZPAR, c_char_p(line), pointer(sz) )
    res = cast(_res, c_char_p).value
    self._release(_res)
    return res
  def batch(self, inf, outf):
    return self._batch(self.ZPAR, c_char_p(inf), c_char_p(outf))
  def close(self):
    if self.__closed__:
      return
    self.__closed__ = True
    self._close(self.ZPAR)
  def __del__(self):
    self.close()
  def __enter__(self):
    return self
  def __exit__(self, type, value, traceback):
    self.close()

if __name__ == "__main__":
  with ZPar( sys.argv[1], sys.argv[2], sys.argv[3]) as zpar:
    if len(sys.argv) > 4:
      zpar.batch(sys.argv[4], sys.argv[5])
    else:
      for line in sys.stdin:
        print zpar.process(line)
