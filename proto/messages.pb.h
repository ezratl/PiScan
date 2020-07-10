// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: messages.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_messages_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_messages_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3007000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3007000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
#include "request.pb.h"
#include "context.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_messages_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_messages_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
void AddDescriptors_messages_2eproto();
namespace piscan_pb {
class ClientToServer;
class ClientToServerDefaultTypeInternal;
extern ClientToServerDefaultTypeInternal _ClientToServer_default_instance_;
class ServerToClient;
class ServerToClientDefaultTypeInternal;
extern ServerToClientDefaultTypeInternal _ServerToClient_default_instance_;
}  // namespace piscan_pb
PROTOBUF_NAMESPACE_OPEN
template<> ::piscan_pb::ClientToServer* Arena::CreateMaybeMessage<::piscan_pb::ClientToServer>(Arena*);
template<> ::piscan_pb::ServerToClient* Arena::CreateMaybeMessage<::piscan_pb::ServerToClient>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace piscan_pb {

enum ClientToServer_Type {
  ClientToServer_Type_GENERAL_REQUEST = 0,
  ClientToServer_Type_SCANNER_STATE_REQUEST = 1,
  ClientToServer_Type_DEMOD_REQUEST = 2,
  ClientToServer_Type_LOCKOUT_REQUEST = 3,
  ClientToServer_Type_ClientToServer_Type_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  ClientToServer_Type_ClientToServer_Type_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool ClientToServer_Type_IsValid(int value);
constexpr ClientToServer_Type ClientToServer_Type_Type_MIN = ClientToServer_Type_GENERAL_REQUEST;
constexpr ClientToServer_Type ClientToServer_Type_Type_MAX = ClientToServer_Type_LOCKOUT_REQUEST;
constexpr int ClientToServer_Type_Type_ARRAYSIZE = ClientToServer_Type_Type_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ClientToServer_Type_descriptor();
inline const std::string& ClientToServer_Type_Name(ClientToServer_Type value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    ClientToServer_Type_descriptor(), value);
}
inline bool ClientToServer_Type_Parse(
    const std::string& name, ClientToServer_Type* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<ClientToServer_Type>(
    ClientToServer_Type_descriptor(), name, value);
}
enum ServerToClient_Type {
  ServerToClient_Type_REQUEST_RESPONSE = 0,
  ServerToClient_Type_SCANNER_CONTEXT = 1,
  ServerToClient_Type_DEMOD_CONTEXT = 2,
  ServerToClient_Type_GENERAL_MESSAGE = 3,
  ServerToClient_Type_SIGNAL_LEVEL = 4,
  ServerToClient_Type_ServerToClient_Type_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  ServerToClient_Type_ServerToClient_Type_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool ServerToClient_Type_IsValid(int value);
constexpr ServerToClient_Type ServerToClient_Type_Type_MIN = ServerToClient_Type_REQUEST_RESPONSE;
constexpr ServerToClient_Type ServerToClient_Type_Type_MAX = ServerToClient_Type_SIGNAL_LEVEL;
constexpr int ServerToClient_Type_Type_ARRAYSIZE = ServerToClient_Type_Type_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ServerToClient_Type_descriptor();
inline const std::string& ServerToClient_Type_Name(ServerToClient_Type value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    ServerToClient_Type_descriptor(), value);
}
inline bool ServerToClient_Type_Parse(
    const std::string& name, ServerToClient_Type* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<ServerToClient_Type>(
    ServerToClient_Type_descriptor(), name, value);
}
// ===================================================================

class ClientToServer final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:piscan_pb.ClientToServer) */ {
 public:
  ClientToServer();
  virtual ~ClientToServer();

  ClientToServer(const ClientToServer& from);
  ClientToServer(ClientToServer&& from) noexcept
    : ClientToServer() {
    *this = ::std::move(from);
  }

  inline ClientToServer& operator=(const ClientToServer& from) {
    CopyFrom(from);
    return *this;
  }
  inline ClientToServer& operator=(ClientToServer&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return default_instance().GetDescriptor();
  }
  static const ClientToServer& default_instance();

  enum ContentCase {
    kGeneralRequest = 2,
    kScanStateRequest = 3,
    kDemodRequest = 4,
    kLockoutRequest = 5,
    CONTENT_NOT_SET = 0,
  };

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ClientToServer* internal_default_instance() {
    return reinterpret_cast<const ClientToServer*>(
               &_ClientToServer_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(ClientToServer* other);
  friend void swap(ClientToServer& a, ClientToServer& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline ClientToServer* New() const final {
    return CreateMaybeMessage<ClientToServer>(nullptr);
  }

  ClientToServer* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<ClientToServer>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const ClientToServer& from);
  void MergeFrom(const ClientToServer& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  #else
  bool MergePartialFromCodedStream(
      ::PROTOBUF_NAMESPACE_ID::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::PROTOBUF_NAMESPACE_ID::io::CodedOutputStream* output) const final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(ClientToServer* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "piscan_pb.ClientToServer";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  typedef ClientToServer_Type Type;
  static constexpr Type GENERAL_REQUEST =
    ClientToServer_Type_GENERAL_REQUEST;
  static constexpr Type SCANNER_STATE_REQUEST =
    ClientToServer_Type_SCANNER_STATE_REQUEST;
  static constexpr Type DEMOD_REQUEST =
    ClientToServer_Type_DEMOD_REQUEST;
  static constexpr Type LOCKOUT_REQUEST =
    ClientToServer_Type_LOCKOUT_REQUEST;
  static inline bool Type_IsValid(int value) {
    return ClientToServer_Type_IsValid(value);
  }
  static constexpr Type Type_MIN =
    ClientToServer_Type_Type_MIN;
  static constexpr Type Type_MAX =
    ClientToServer_Type_Type_MAX;
  static constexpr int Type_ARRAYSIZE =
    ClientToServer_Type_Type_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  Type_descriptor() {
    return ClientToServer_Type_descriptor();
  }
  static inline const std::string& Type_Name(Type value) {
    return ClientToServer_Type_Name(value);
  }
  static inline bool Type_Parse(const std::string& name,
      Type* value) {
    return ClientToServer_Type_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // .piscan_pb.ClientToServer.Type type = 1;
  void clear_type();
  static const int kTypeFieldNumber = 1;
  ::piscan_pb::ClientToServer_Type type() const;
  void set_type(::piscan_pb::ClientToServer_Type value);

  // .piscan_pb.GeneralRequest generalRequest = 2;
  bool has_generalrequest() const;
  void clear_generalrequest();
  static const int kGeneralRequestFieldNumber = 2;
  const ::piscan_pb::GeneralRequest& generalrequest() const;
  ::piscan_pb::GeneralRequest* release_generalrequest();
  ::piscan_pb::GeneralRequest* mutable_generalrequest();
  void set_allocated_generalrequest(::piscan_pb::GeneralRequest* generalrequest);

  // .piscan_pb.ScannerStateRequest scanStateRequest = 3;
  bool has_scanstaterequest() const;
  void clear_scanstaterequest();
  static const int kScanStateRequestFieldNumber = 3;
  const ::piscan_pb::ScannerStateRequest& scanstaterequest() const;
  ::piscan_pb::ScannerStateRequest* release_scanstaterequest();
  ::piscan_pb::ScannerStateRequest* mutable_scanstaterequest();
  void set_allocated_scanstaterequest(::piscan_pb::ScannerStateRequest* scanstaterequest);

  // .piscan_pb.DemodRequest demodRequest = 4;
  bool has_demodrequest() const;
  void clear_demodrequest();
  static const int kDemodRequestFieldNumber = 4;
  const ::piscan_pb::DemodRequest& demodrequest() const;
  ::piscan_pb::DemodRequest* release_demodrequest();
  ::piscan_pb::DemodRequest* mutable_demodrequest();
  void set_allocated_demodrequest(::piscan_pb::DemodRequest* demodrequest);

  // .piscan_pb.LockoutRequest lockoutRequest = 5;
  bool has_lockoutrequest() const;
  void clear_lockoutrequest();
  static const int kLockoutRequestFieldNumber = 5;
  const ::piscan_pb::LockoutRequest& lockoutrequest() const;
  ::piscan_pb::LockoutRequest* release_lockoutrequest();
  ::piscan_pb::LockoutRequest* mutable_lockoutrequest();
  void set_allocated_lockoutrequest(::piscan_pb::LockoutRequest* lockoutrequest);

  void clear_content();
  ContentCase content_case() const;
  // @@protoc_insertion_point(class_scope:piscan_pb.ClientToServer)
 private:
  class HasBitSetters;
  void set_has_generalrequest();
  void set_has_scanstaterequest();
  void set_has_demodrequest();
  void set_has_lockoutrequest();

  inline bool has_content() const;
  inline void clear_has_content();

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  int type_;
  union ContentUnion {
    ContentUnion() {}
    ::piscan_pb::GeneralRequest* generalrequest_;
    ::piscan_pb::ScannerStateRequest* scanstaterequest_;
    ::piscan_pb::DemodRequest* demodrequest_;
    ::piscan_pb::LockoutRequest* lockoutrequest_;
  } content_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::uint32 _oneof_case_[1];

  friend struct ::TableStruct_messages_2eproto;
};
// -------------------------------------------------------------------

class ServerToClient final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:piscan_pb.ServerToClient) */ {
 public:
  ServerToClient();
  virtual ~ServerToClient();

  ServerToClient(const ServerToClient& from);
  ServerToClient(ServerToClient&& from) noexcept
    : ServerToClient() {
    *this = ::std::move(from);
  }

  inline ServerToClient& operator=(const ServerToClient& from) {
    CopyFrom(from);
    return *this;
  }
  inline ServerToClient& operator=(ServerToClient&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return default_instance().GetDescriptor();
  }
  static const ServerToClient& default_instance();

  enum ContentCase {
    kScannerContext = 3,
    kDemodContext = 4,
    kGeneralMessage = 5,
    kSystemInfo = 6,
    kSignalLevel = 7,
    CONTENT_NOT_SET = 0,
  };

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ServerToClient* internal_default_instance() {
    return reinterpret_cast<const ServerToClient*>(
               &_ServerToClient_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(ServerToClient* other);
  friend void swap(ServerToClient& a, ServerToClient& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline ServerToClient* New() const final {
    return CreateMaybeMessage<ServerToClient>(nullptr);
  }

  ServerToClient* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<ServerToClient>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const ServerToClient& from);
  void MergeFrom(const ServerToClient& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  #else
  bool MergePartialFromCodedStream(
      ::PROTOBUF_NAMESPACE_ID::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::PROTOBUF_NAMESPACE_ID::io::CodedOutputStream* output) const final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(ServerToClient* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "piscan_pb.ServerToClient";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  typedef ServerToClient_Type Type;
  static constexpr Type REQUEST_RESPONSE =
    ServerToClient_Type_REQUEST_RESPONSE;
  static constexpr Type SCANNER_CONTEXT =
    ServerToClient_Type_SCANNER_CONTEXT;
  static constexpr Type DEMOD_CONTEXT =
    ServerToClient_Type_DEMOD_CONTEXT;
  static constexpr Type GENERAL_MESSAGE =
    ServerToClient_Type_GENERAL_MESSAGE;
  static constexpr Type SIGNAL_LEVEL =
    ServerToClient_Type_SIGNAL_LEVEL;
  static inline bool Type_IsValid(int value) {
    return ServerToClient_Type_IsValid(value);
  }
  static constexpr Type Type_MIN =
    ServerToClient_Type_Type_MIN;
  static constexpr Type Type_MAX =
    ServerToClient_Type_Type_MAX;
  static constexpr int Type_ARRAYSIZE =
    ServerToClient_Type_Type_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  Type_descriptor() {
    return ServerToClient_Type_descriptor();
  }
  static inline const std::string& Type_Name(Type value) {
    return ServerToClient_Type_Name(value);
  }
  static inline bool Type_Parse(const std::string& name,
      Type* value) {
    return ServerToClient_Type_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // int32 handle = 1;
  void clear_handle();
  static const int kHandleFieldNumber = 1;
  ::PROTOBUF_NAMESPACE_ID::int32 handle() const;
  void set_handle(::PROTOBUF_NAMESPACE_ID::int32 value);

  // .piscan_pb.ServerToClient.Type type = 2;
  void clear_type();
  static const int kTypeFieldNumber = 2;
  ::piscan_pb::ServerToClient_Type type() const;
  void set_type(::piscan_pb::ServerToClient_Type value);

  // .piscan_pb.ScannerContext scannerContext = 3;
  bool has_scannercontext() const;
  void clear_scannercontext();
  static const int kScannerContextFieldNumber = 3;
  const ::piscan_pb::ScannerContext& scannercontext() const;
  ::piscan_pb::ScannerContext* release_scannercontext();
  ::piscan_pb::ScannerContext* mutable_scannercontext();
  void set_allocated_scannercontext(::piscan_pb::ScannerContext* scannercontext);

  // .piscan_pb.DemodContext demodContext = 4;
  bool has_demodcontext() const;
  void clear_demodcontext();
  static const int kDemodContextFieldNumber = 4;
  const ::piscan_pb::DemodContext& demodcontext() const;
  ::piscan_pb::DemodContext* release_demodcontext();
  ::piscan_pb::DemodContext* mutable_demodcontext();
  void set_allocated_demodcontext(::piscan_pb::DemodContext* demodcontext);

  // .piscan_pb.GeneralMessage generalMessage = 5;
  bool has_generalmessage() const;
  void clear_generalmessage();
  static const int kGeneralMessageFieldNumber = 5;
  const ::piscan_pb::GeneralMessage& generalmessage() const;
  ::piscan_pb::GeneralMessage* release_generalmessage();
  ::piscan_pb::GeneralMessage* mutable_generalmessage();
  void set_allocated_generalmessage(::piscan_pb::GeneralMessage* generalmessage);

  // .piscan_pb.SystemInfo systemInfo = 6;
  bool has_systeminfo() const;
  void clear_systeminfo();
  static const int kSystemInfoFieldNumber = 6;
  const ::piscan_pb::SystemInfo& systeminfo() const;
  ::piscan_pb::SystemInfo* release_systeminfo();
  ::piscan_pb::SystemInfo* mutable_systeminfo();
  void set_allocated_systeminfo(::piscan_pb::SystemInfo* systeminfo);

  // .piscan_pb.SignalLevel signalLevel = 7;
  bool has_signallevel() const;
  void clear_signallevel();
  static const int kSignalLevelFieldNumber = 7;
  const ::piscan_pb::SignalLevel& signallevel() const;
  ::piscan_pb::SignalLevel* release_signallevel();
  ::piscan_pb::SignalLevel* mutable_signallevel();
  void set_allocated_signallevel(::piscan_pb::SignalLevel* signallevel);

  void clear_content();
  ContentCase content_case() const;
  // @@protoc_insertion_point(class_scope:piscan_pb.ServerToClient)
 private:
  class HasBitSetters;
  void set_has_scannercontext();
  void set_has_demodcontext();
  void set_has_generalmessage();
  void set_has_systeminfo();
  void set_has_signallevel();

  inline bool has_content() const;
  inline void clear_has_content();

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::int32 handle_;
  int type_;
  union ContentUnion {
    ContentUnion() {}
    ::piscan_pb::ScannerContext* scannercontext_;
    ::piscan_pb::DemodContext* demodcontext_;
    ::piscan_pb::GeneralMessage* generalmessage_;
    ::piscan_pb::SystemInfo* systeminfo_;
    ::piscan_pb::SignalLevel* signallevel_;
  } content_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::uint32 _oneof_case_[1];

  friend struct ::TableStruct_messages_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// ClientToServer

// .piscan_pb.ClientToServer.Type type = 1;
inline void ClientToServer::clear_type() {
  type_ = 0;
}
inline ::piscan_pb::ClientToServer_Type ClientToServer::type() const {
  // @@protoc_insertion_point(field_get:piscan_pb.ClientToServer.type)
  return static_cast< ::piscan_pb::ClientToServer_Type >(type_);
}
inline void ClientToServer::set_type(::piscan_pb::ClientToServer_Type value) {
  
  type_ = value;
  // @@protoc_insertion_point(field_set:piscan_pb.ClientToServer.type)
}

// .piscan_pb.GeneralRequest generalRequest = 2;
inline bool ClientToServer::has_generalrequest() const {
  return content_case() == kGeneralRequest;
}
inline void ClientToServer::set_has_generalrequest() {
  _oneof_case_[0] = kGeneralRequest;
}
inline ::piscan_pb::GeneralRequest* ClientToServer::release_generalrequest() {
  // @@protoc_insertion_point(field_release:piscan_pb.ClientToServer.generalRequest)
  if (has_generalrequest()) {
    clear_has_content();
      ::piscan_pb::GeneralRequest* temp = content_.generalrequest_;
    content_.generalrequest_ = nullptr;
    return temp;
  } else {
    return nullptr;
  }
}
inline const ::piscan_pb::GeneralRequest& ClientToServer::generalrequest() const {
  // @@protoc_insertion_point(field_get:piscan_pb.ClientToServer.generalRequest)
  return has_generalrequest()
      ? *content_.generalrequest_
      : *reinterpret_cast< ::piscan_pb::GeneralRequest*>(&::piscan_pb::_GeneralRequest_default_instance_);
}
inline ::piscan_pb::GeneralRequest* ClientToServer::mutable_generalrequest() {
  if (!has_generalrequest()) {
    clear_content();
    set_has_generalrequest();
    content_.generalrequest_ = CreateMaybeMessage< ::piscan_pb::GeneralRequest >(
        GetArenaNoVirtual());
  }
  // @@protoc_insertion_point(field_mutable:piscan_pb.ClientToServer.generalRequest)
  return content_.generalrequest_;
}

// .piscan_pb.ScannerStateRequest scanStateRequest = 3;
inline bool ClientToServer::has_scanstaterequest() const {
  return content_case() == kScanStateRequest;
}
inline void ClientToServer::set_has_scanstaterequest() {
  _oneof_case_[0] = kScanStateRequest;
}
inline ::piscan_pb::ScannerStateRequest* ClientToServer::release_scanstaterequest() {
  // @@protoc_insertion_point(field_release:piscan_pb.ClientToServer.scanStateRequest)
  if (has_scanstaterequest()) {
    clear_has_content();
      ::piscan_pb::ScannerStateRequest* temp = content_.scanstaterequest_;
    content_.scanstaterequest_ = nullptr;
    return temp;
  } else {
    return nullptr;
  }
}
inline const ::piscan_pb::ScannerStateRequest& ClientToServer::scanstaterequest() const {
  // @@protoc_insertion_point(field_get:piscan_pb.ClientToServer.scanStateRequest)
  return has_scanstaterequest()
      ? *content_.scanstaterequest_
      : *reinterpret_cast< ::piscan_pb::ScannerStateRequest*>(&::piscan_pb::_ScannerStateRequest_default_instance_);
}
inline ::piscan_pb::ScannerStateRequest* ClientToServer::mutable_scanstaterequest() {
  if (!has_scanstaterequest()) {
    clear_content();
    set_has_scanstaterequest();
    content_.scanstaterequest_ = CreateMaybeMessage< ::piscan_pb::ScannerStateRequest >(
        GetArenaNoVirtual());
  }
  // @@protoc_insertion_point(field_mutable:piscan_pb.ClientToServer.scanStateRequest)
  return content_.scanstaterequest_;
}

// .piscan_pb.DemodRequest demodRequest = 4;
inline bool ClientToServer::has_demodrequest() const {
  return content_case() == kDemodRequest;
}
inline void ClientToServer::set_has_demodrequest() {
  _oneof_case_[0] = kDemodRequest;
}
inline ::piscan_pb::DemodRequest* ClientToServer::release_demodrequest() {
  // @@protoc_insertion_point(field_release:piscan_pb.ClientToServer.demodRequest)
  if (has_demodrequest()) {
    clear_has_content();
      ::piscan_pb::DemodRequest* temp = content_.demodrequest_;
    content_.demodrequest_ = nullptr;
    return temp;
  } else {
    return nullptr;
  }
}
inline const ::piscan_pb::DemodRequest& ClientToServer::demodrequest() const {
  // @@protoc_insertion_point(field_get:piscan_pb.ClientToServer.demodRequest)
  return has_demodrequest()
      ? *content_.demodrequest_
      : *reinterpret_cast< ::piscan_pb::DemodRequest*>(&::piscan_pb::_DemodRequest_default_instance_);
}
inline ::piscan_pb::DemodRequest* ClientToServer::mutable_demodrequest() {
  if (!has_demodrequest()) {
    clear_content();
    set_has_demodrequest();
    content_.demodrequest_ = CreateMaybeMessage< ::piscan_pb::DemodRequest >(
        GetArenaNoVirtual());
  }
  // @@protoc_insertion_point(field_mutable:piscan_pb.ClientToServer.demodRequest)
  return content_.demodrequest_;
}

// .piscan_pb.LockoutRequest lockoutRequest = 5;
inline bool ClientToServer::has_lockoutrequest() const {
  return content_case() == kLockoutRequest;
}
inline void ClientToServer::set_has_lockoutrequest() {
  _oneof_case_[0] = kLockoutRequest;
}
inline ::piscan_pb::LockoutRequest* ClientToServer::release_lockoutrequest() {
  // @@protoc_insertion_point(field_release:piscan_pb.ClientToServer.lockoutRequest)
  if (has_lockoutrequest()) {
    clear_has_content();
      ::piscan_pb::LockoutRequest* temp = content_.lockoutrequest_;
    content_.lockoutrequest_ = nullptr;
    return temp;
  } else {
    return nullptr;
  }
}
inline const ::piscan_pb::LockoutRequest& ClientToServer::lockoutrequest() const {
  // @@protoc_insertion_point(field_get:piscan_pb.ClientToServer.lockoutRequest)
  return has_lockoutrequest()
      ? *content_.lockoutrequest_
      : *reinterpret_cast< ::piscan_pb::LockoutRequest*>(&::piscan_pb::_LockoutRequest_default_instance_);
}
inline ::piscan_pb::LockoutRequest* ClientToServer::mutable_lockoutrequest() {
  if (!has_lockoutrequest()) {
    clear_content();
    set_has_lockoutrequest();
    content_.lockoutrequest_ = CreateMaybeMessage< ::piscan_pb::LockoutRequest >(
        GetArenaNoVirtual());
  }
  // @@protoc_insertion_point(field_mutable:piscan_pb.ClientToServer.lockoutRequest)
  return content_.lockoutrequest_;
}

inline bool ClientToServer::has_content() const {
  return content_case() != CONTENT_NOT_SET;
}
inline void ClientToServer::clear_has_content() {
  _oneof_case_[0] = CONTENT_NOT_SET;
}
inline ClientToServer::ContentCase ClientToServer::content_case() const {
  return ClientToServer::ContentCase(_oneof_case_[0]);
}
// -------------------------------------------------------------------

// ServerToClient

// int32 handle = 1;
inline void ServerToClient::clear_handle() {
  handle_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 ServerToClient::handle() const {
  // @@protoc_insertion_point(field_get:piscan_pb.ServerToClient.handle)
  return handle_;
}
inline void ServerToClient::set_handle(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  handle_ = value;
  // @@protoc_insertion_point(field_set:piscan_pb.ServerToClient.handle)
}

// .piscan_pb.ServerToClient.Type type = 2;
inline void ServerToClient::clear_type() {
  type_ = 0;
}
inline ::piscan_pb::ServerToClient_Type ServerToClient::type() const {
  // @@protoc_insertion_point(field_get:piscan_pb.ServerToClient.type)
  return static_cast< ::piscan_pb::ServerToClient_Type >(type_);
}
inline void ServerToClient::set_type(::piscan_pb::ServerToClient_Type value) {
  
  type_ = value;
  // @@protoc_insertion_point(field_set:piscan_pb.ServerToClient.type)
}

// .piscan_pb.ScannerContext scannerContext = 3;
inline bool ServerToClient::has_scannercontext() const {
  return content_case() == kScannerContext;
}
inline void ServerToClient::set_has_scannercontext() {
  _oneof_case_[0] = kScannerContext;
}
inline ::piscan_pb::ScannerContext* ServerToClient::release_scannercontext() {
  // @@protoc_insertion_point(field_release:piscan_pb.ServerToClient.scannerContext)
  if (has_scannercontext()) {
    clear_has_content();
      ::piscan_pb::ScannerContext* temp = content_.scannercontext_;
    content_.scannercontext_ = nullptr;
    return temp;
  } else {
    return nullptr;
  }
}
inline const ::piscan_pb::ScannerContext& ServerToClient::scannercontext() const {
  // @@protoc_insertion_point(field_get:piscan_pb.ServerToClient.scannerContext)
  return has_scannercontext()
      ? *content_.scannercontext_
      : *reinterpret_cast< ::piscan_pb::ScannerContext*>(&::piscan_pb::_ScannerContext_default_instance_);
}
inline ::piscan_pb::ScannerContext* ServerToClient::mutable_scannercontext() {
  if (!has_scannercontext()) {
    clear_content();
    set_has_scannercontext();
    content_.scannercontext_ = CreateMaybeMessage< ::piscan_pb::ScannerContext >(
        GetArenaNoVirtual());
  }
  // @@protoc_insertion_point(field_mutable:piscan_pb.ServerToClient.scannerContext)
  return content_.scannercontext_;
}

// .piscan_pb.DemodContext demodContext = 4;
inline bool ServerToClient::has_demodcontext() const {
  return content_case() == kDemodContext;
}
inline void ServerToClient::set_has_demodcontext() {
  _oneof_case_[0] = kDemodContext;
}
inline ::piscan_pb::DemodContext* ServerToClient::release_demodcontext() {
  // @@protoc_insertion_point(field_release:piscan_pb.ServerToClient.demodContext)
  if (has_demodcontext()) {
    clear_has_content();
      ::piscan_pb::DemodContext* temp = content_.demodcontext_;
    content_.demodcontext_ = nullptr;
    return temp;
  } else {
    return nullptr;
  }
}
inline const ::piscan_pb::DemodContext& ServerToClient::demodcontext() const {
  // @@protoc_insertion_point(field_get:piscan_pb.ServerToClient.demodContext)
  return has_demodcontext()
      ? *content_.demodcontext_
      : *reinterpret_cast< ::piscan_pb::DemodContext*>(&::piscan_pb::_DemodContext_default_instance_);
}
inline ::piscan_pb::DemodContext* ServerToClient::mutable_demodcontext() {
  if (!has_demodcontext()) {
    clear_content();
    set_has_demodcontext();
    content_.demodcontext_ = CreateMaybeMessage< ::piscan_pb::DemodContext >(
        GetArenaNoVirtual());
  }
  // @@protoc_insertion_point(field_mutable:piscan_pb.ServerToClient.demodContext)
  return content_.demodcontext_;
}

// .piscan_pb.GeneralMessage generalMessage = 5;
inline bool ServerToClient::has_generalmessage() const {
  return content_case() == kGeneralMessage;
}
inline void ServerToClient::set_has_generalmessage() {
  _oneof_case_[0] = kGeneralMessage;
}
inline ::piscan_pb::GeneralMessage* ServerToClient::release_generalmessage() {
  // @@protoc_insertion_point(field_release:piscan_pb.ServerToClient.generalMessage)
  if (has_generalmessage()) {
    clear_has_content();
      ::piscan_pb::GeneralMessage* temp = content_.generalmessage_;
    content_.generalmessage_ = nullptr;
    return temp;
  } else {
    return nullptr;
  }
}
inline const ::piscan_pb::GeneralMessage& ServerToClient::generalmessage() const {
  // @@protoc_insertion_point(field_get:piscan_pb.ServerToClient.generalMessage)
  return has_generalmessage()
      ? *content_.generalmessage_
      : *reinterpret_cast< ::piscan_pb::GeneralMessage*>(&::piscan_pb::_GeneralMessage_default_instance_);
}
inline ::piscan_pb::GeneralMessage* ServerToClient::mutable_generalmessage() {
  if (!has_generalmessage()) {
    clear_content();
    set_has_generalmessage();
    content_.generalmessage_ = CreateMaybeMessage< ::piscan_pb::GeneralMessage >(
        GetArenaNoVirtual());
  }
  // @@protoc_insertion_point(field_mutable:piscan_pb.ServerToClient.generalMessage)
  return content_.generalmessage_;
}

// .piscan_pb.SystemInfo systemInfo = 6;
inline bool ServerToClient::has_systeminfo() const {
  return content_case() == kSystemInfo;
}
inline void ServerToClient::set_has_systeminfo() {
  _oneof_case_[0] = kSystemInfo;
}
inline ::piscan_pb::SystemInfo* ServerToClient::release_systeminfo() {
  // @@protoc_insertion_point(field_release:piscan_pb.ServerToClient.systemInfo)
  if (has_systeminfo()) {
    clear_has_content();
      ::piscan_pb::SystemInfo* temp = content_.systeminfo_;
    content_.systeminfo_ = nullptr;
    return temp;
  } else {
    return nullptr;
  }
}
inline const ::piscan_pb::SystemInfo& ServerToClient::systeminfo() const {
  // @@protoc_insertion_point(field_get:piscan_pb.ServerToClient.systemInfo)
  return has_systeminfo()
      ? *content_.systeminfo_
      : *reinterpret_cast< ::piscan_pb::SystemInfo*>(&::piscan_pb::_SystemInfo_default_instance_);
}
inline ::piscan_pb::SystemInfo* ServerToClient::mutable_systeminfo() {
  if (!has_systeminfo()) {
    clear_content();
    set_has_systeminfo();
    content_.systeminfo_ = CreateMaybeMessage< ::piscan_pb::SystemInfo >(
        GetArenaNoVirtual());
  }
  // @@protoc_insertion_point(field_mutable:piscan_pb.ServerToClient.systemInfo)
  return content_.systeminfo_;
}

// .piscan_pb.SignalLevel signalLevel = 7;
inline bool ServerToClient::has_signallevel() const {
  return content_case() == kSignalLevel;
}
inline void ServerToClient::set_has_signallevel() {
  _oneof_case_[0] = kSignalLevel;
}
inline ::piscan_pb::SignalLevel* ServerToClient::release_signallevel() {
  // @@protoc_insertion_point(field_release:piscan_pb.ServerToClient.signalLevel)
  if (has_signallevel()) {
    clear_has_content();
      ::piscan_pb::SignalLevel* temp = content_.signallevel_;
    content_.signallevel_ = nullptr;
    return temp;
  } else {
    return nullptr;
  }
}
inline const ::piscan_pb::SignalLevel& ServerToClient::signallevel() const {
  // @@protoc_insertion_point(field_get:piscan_pb.ServerToClient.signalLevel)
  return has_signallevel()
      ? *content_.signallevel_
      : *reinterpret_cast< ::piscan_pb::SignalLevel*>(&::piscan_pb::_SignalLevel_default_instance_);
}
inline ::piscan_pb::SignalLevel* ServerToClient::mutable_signallevel() {
  if (!has_signallevel()) {
    clear_content();
    set_has_signallevel();
    content_.signallevel_ = CreateMaybeMessage< ::piscan_pb::SignalLevel >(
        GetArenaNoVirtual());
  }
  // @@protoc_insertion_point(field_mutable:piscan_pb.ServerToClient.signalLevel)
  return content_.signallevel_;
}

inline bool ServerToClient::has_content() const {
  return content_case() != CONTENT_NOT_SET;
}
inline void ServerToClient::clear_has_content() {
  _oneof_case_[0] = CONTENT_NOT_SET;
}
inline ServerToClient::ContentCase ServerToClient::content_case() const {
  return ServerToClient::ContentCase(_oneof_case_[0]);
}
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace piscan_pb

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::piscan_pb::ClientToServer_Type> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::piscan_pb::ClientToServer_Type>() {
  return ::piscan_pb::ClientToServer_Type_descriptor();
}
template <> struct is_proto_enum< ::piscan_pb::ServerToClient_Type> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::piscan_pb::ServerToClient_Type>() {
  return ::piscan_pb::ServerToClient_Type_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_messages_2eproto
