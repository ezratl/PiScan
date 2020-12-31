# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: request.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='request.proto',
  package='piscan_pb',
  syntax='proto3',
  serialized_options=None,
  serialized_pb=_b('\n\rrequest.proto\x12\tpiscan_pb\"\x9d\x01\n\x0eGeneralRequest\x12\x0e\n\x06handle\x18\x01 \x01(\x05\x12\x33\n\x04type\x18\x02 \x01(\x0e\x32%.piscan_pb.GeneralRequest.RequestType\"F\n\x0bRequestType\x12\x13\n\x0fSCANNER_CONTEXT\x10\x00\x12\x11\n\rDEMOD_CONTEXT\x10\x01\x12\x0f\n\x0bSYSTEM_INFO\x10\x02\"\xc4\x01\n\x13ScannerStateRequest\x12\x0e\n\x06handle\x18\x01 \x01(\x05\x12\x36\n\x05state\x18\x02 \x01(\x0e\x32\'.piscan_pb.ScannerStateRequest.NewState\x12\x0f\n\x07manFreq\x18\x03 \x01(\x03\x12\x15\n\rmanModulation\x18\x04 \x01(\t\x12\x11\n\tholdIndex\x18\x05 \x01(\t\"*\n\x08NewState\x12\x08\n\x04SCAN\x10\x00\x12\x08\n\x04HOLD\x10\x01\x12\n\n\x06MANUAL\x10\x02\"\x8a\x01\n\x0c\x44\x65modRequest\x12\x0e\n\x06handle\x18\x01 \x01(\x05\x12/\n\x04type\x18\x02 \x01(\x0e\x32!.piscan_pb.DemodRequest.DemodFunc\x12\r\n\x05level\x18\x03 \x01(\x02\"*\n\tDemodFunc\x12\x0f\n\x0bSET_SQUELCH\x10\x00\x12\x0c\n\x08SET_GAIN\x10\x01\"\xb2\x01\n\x0eLockoutRequest\x12\x12\n\nentryIndex\x18\x01 \x01(\t\x12\x36\n\x0blockoutType\x18\x02 \x01(\x0e\x32!.piscan_pb.LockoutRequest.Lockout\x12\x17\n\x0flockoutDuration\x18\x03 \x01(\x05\";\n\x07Lockout\x12\x08\n\x04NONE\x10\x00\x12\t\n\x05TIMED\x10\x01\x12\x0b\n\x07SESSION\x10\x02\x12\x0e\n\nPERSISTENT\x10\x03\x62\x06proto3')
)



_GENERALREQUEST_REQUESTTYPE = _descriptor.EnumDescriptor(
  name='RequestType',
  full_name='piscan_pb.GeneralRequest.RequestType',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='SCANNER_CONTEXT', index=0, number=0,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='DEMOD_CONTEXT', index=1, number=1,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='SYSTEM_INFO', index=2, number=2,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=116,
  serialized_end=186,
)
_sym_db.RegisterEnumDescriptor(_GENERALREQUEST_REQUESTTYPE)

_SCANNERSTATEREQUEST_NEWSTATE = _descriptor.EnumDescriptor(
  name='NewState',
  full_name='piscan_pb.ScannerStateRequest.NewState',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='SCAN', index=0, number=0,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='HOLD', index=1, number=1,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MANUAL', index=2, number=2,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=343,
  serialized_end=385,
)
_sym_db.RegisterEnumDescriptor(_SCANNERSTATEREQUEST_NEWSTATE)

_DEMODREQUEST_DEMODFUNC = _descriptor.EnumDescriptor(
  name='DemodFunc',
  full_name='piscan_pb.DemodRequest.DemodFunc',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='SET_SQUELCH', index=0, number=0,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='SET_GAIN', index=1, number=1,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=484,
  serialized_end=526,
)
_sym_db.RegisterEnumDescriptor(_DEMODREQUEST_DEMODFUNC)

_LOCKOUTREQUEST_LOCKOUT = _descriptor.EnumDescriptor(
  name='Lockout',
  full_name='piscan_pb.LockoutRequest.Lockout',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='NONE', index=0, number=0,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TIMED', index=1, number=1,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='SESSION', index=2, number=2,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='PERSISTENT', index=3, number=3,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=648,
  serialized_end=707,
)
_sym_db.RegisterEnumDescriptor(_LOCKOUTREQUEST_LOCKOUT)


_GENERALREQUEST = _descriptor.Descriptor(
  name='GeneralRequest',
  full_name='piscan_pb.GeneralRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='handle', full_name='piscan_pb.GeneralRequest.handle', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='type', full_name='piscan_pb.GeneralRequest.type', index=1,
      number=2, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
    _GENERALREQUEST_REQUESTTYPE,
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=29,
  serialized_end=186,
)


_SCANNERSTATEREQUEST = _descriptor.Descriptor(
  name='ScannerStateRequest',
  full_name='piscan_pb.ScannerStateRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='handle', full_name='piscan_pb.ScannerStateRequest.handle', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='state', full_name='piscan_pb.ScannerStateRequest.state', index=1,
      number=2, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='manFreq', full_name='piscan_pb.ScannerStateRequest.manFreq', index=2,
      number=3, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='manModulation', full_name='piscan_pb.ScannerStateRequest.manModulation', index=3,
      number=4, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='holdIndex', full_name='piscan_pb.ScannerStateRequest.holdIndex', index=4,
      number=5, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
    _SCANNERSTATEREQUEST_NEWSTATE,
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=189,
  serialized_end=385,
)


_DEMODREQUEST = _descriptor.Descriptor(
  name='DemodRequest',
  full_name='piscan_pb.DemodRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='handle', full_name='piscan_pb.DemodRequest.handle', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='type', full_name='piscan_pb.DemodRequest.type', index=1,
      number=2, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='level', full_name='piscan_pb.DemodRequest.level', index=2,
      number=3, type=2, cpp_type=6, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
    _DEMODREQUEST_DEMODFUNC,
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=388,
  serialized_end=526,
)


_LOCKOUTREQUEST = _descriptor.Descriptor(
  name='LockoutRequest',
  full_name='piscan_pb.LockoutRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='entryIndex', full_name='piscan_pb.LockoutRequest.entryIndex', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='lockoutType', full_name='piscan_pb.LockoutRequest.lockoutType', index=1,
      number=2, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='lockoutDuration', full_name='piscan_pb.LockoutRequest.lockoutDuration', index=2,
      number=3, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
    _LOCKOUTREQUEST_LOCKOUT,
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=529,
  serialized_end=707,
)

_GENERALREQUEST.fields_by_name['type'].enum_type = _GENERALREQUEST_REQUESTTYPE
_GENERALREQUEST_REQUESTTYPE.containing_type = _GENERALREQUEST
_SCANNERSTATEREQUEST.fields_by_name['state'].enum_type = _SCANNERSTATEREQUEST_NEWSTATE
_SCANNERSTATEREQUEST_NEWSTATE.containing_type = _SCANNERSTATEREQUEST
_DEMODREQUEST.fields_by_name['type'].enum_type = _DEMODREQUEST_DEMODFUNC
_DEMODREQUEST_DEMODFUNC.containing_type = _DEMODREQUEST
_LOCKOUTREQUEST.fields_by_name['lockoutType'].enum_type = _LOCKOUTREQUEST_LOCKOUT
_LOCKOUTREQUEST_LOCKOUT.containing_type = _LOCKOUTREQUEST
DESCRIPTOR.message_types_by_name['GeneralRequest'] = _GENERALREQUEST
DESCRIPTOR.message_types_by_name['ScannerStateRequest'] = _SCANNERSTATEREQUEST
DESCRIPTOR.message_types_by_name['DemodRequest'] = _DEMODREQUEST
DESCRIPTOR.message_types_by_name['LockoutRequest'] = _LOCKOUTREQUEST
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

GeneralRequest = _reflection.GeneratedProtocolMessageType('GeneralRequest', (_message.Message,), {
  'DESCRIPTOR' : _GENERALREQUEST,
  '__module__' : 'request_pb2'
  # @@protoc_insertion_point(class_scope:piscan_pb.GeneralRequest)
  })
_sym_db.RegisterMessage(GeneralRequest)

ScannerStateRequest = _reflection.GeneratedProtocolMessageType('ScannerStateRequest', (_message.Message,), {
  'DESCRIPTOR' : _SCANNERSTATEREQUEST,
  '__module__' : 'request_pb2'
  # @@protoc_insertion_point(class_scope:piscan_pb.ScannerStateRequest)
  })
_sym_db.RegisterMessage(ScannerStateRequest)

DemodRequest = _reflection.GeneratedProtocolMessageType('DemodRequest', (_message.Message,), {
  'DESCRIPTOR' : _DEMODREQUEST,
  '__module__' : 'request_pb2'
  # @@protoc_insertion_point(class_scope:piscan_pb.DemodRequest)
  })
_sym_db.RegisterMessage(DemodRequest)

LockoutRequest = _reflection.GeneratedProtocolMessageType('LockoutRequest', (_message.Message,), {
  'DESCRIPTOR' : _LOCKOUTREQUEST,
  '__module__' : 'request_pb2'
  # @@protoc_insertion_point(class_scope:piscan_pb.LockoutRequest)
  })
_sym_db.RegisterMessage(LockoutRequest)


# @@protoc_insertion_point(module_scope)