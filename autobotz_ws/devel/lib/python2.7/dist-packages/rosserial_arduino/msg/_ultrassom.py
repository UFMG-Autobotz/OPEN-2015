"""autogenerated by genpy from rosserial_arduino/ultrassom.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class ultrassom(genpy.Message):
  _md5sum = "fb6e69afd8489e9696f90b5f332c934f"
  _type = "rosserial_arduino/ultrassom"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """float32 dir
float32 esq
float32 frente
float32 tras

"""
  __slots__ = ['dir','esq','frente','tras']
  _slot_types = ['float32','float32','float32','float32']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       dir,esq,frente,tras

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(ultrassom, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.dir is None:
        self.dir = 0.
      if self.esq is None:
        self.esq = 0.
      if self.frente is None:
        self.frente = 0.
      if self.tras is None:
        self.tras = 0.
    else:
      self.dir = 0.
      self.esq = 0.
      self.frente = 0.
      self.tras = 0.

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      _x = self
      buff.write(_struct_4f.pack(_x.dir, _x.esq, _x.frente, _x.tras))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      end = 0
      _x = self
      start = end
      end += 16
      (_x.dir, _x.esq, _x.frente, _x.tras,) = _struct_4f.unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      _x = self
      buff.write(_struct_4f.pack(_x.dir, _x.esq, _x.frente, _x.tras))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      end = 0
      _x = self
      start = end
      end += 16
      (_x.dir, _x.esq, _x.frente, _x.tras,) = _struct_4f.unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_4f = struct.Struct("<4f")
