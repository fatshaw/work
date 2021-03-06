/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 */
#include "mis_types.h"



const char* MISRequest::ascii_fingerprint = "BFF0E21728CB005F9AA5774A41542B8D";
const uint8_t MISRequest::binary_fingerprint[16] = {0xBF,0xF0,0xE2,0x17,0x28,0xCB,0x00,0x5F,0x9A,0xA5,0x77,0x4A,0x41,0x54,0x2B,0x8D};

uint32_t MISRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->record_id);
          this->__isset.record_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->user_id);
          this->__isset.user_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->ip);
          this->__isset.ip = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->channel);
          this->__isset.channel = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->content);
          this->__isset.content = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t MISRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("MISRequest");
  xfer += oprot->writeFieldBegin("record_id", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->record_id);
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldBegin("user_id", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->user_id);
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldBegin("ip", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->ip);
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldBegin("channel", ::apache::thrift::protocol::T_STRING, 4);
  xfer += oprot->writeString(this->channel);
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldBegin("content", ::apache::thrift::protocol::T_STRING, 5);
  xfer += oprot->writeString(this->content);
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

const char* MISResponse::ascii_fingerprint = "A6609EAC7CD2A291DFDA43A4A8D65B03";
const uint8_t MISResponse::binary_fingerprint[16] = {0xA6,0x60,0x9E,0xAC,0x7C,0xD2,0xA2,0x91,0xDF,0xDA,0x43,0xA4,0xA8,0xD6,0x5B,0x03};

uint32_t MISResponse::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_BYTE) {
          xfer += iprot->readByte(this->result);
          this->__isset.result = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->reason);
          this->__isset.reason = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t MISResponse::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("MISResponse");
  xfer += oprot->writeFieldBegin("result", ::apache::thrift::protocol::T_BYTE, 1);
  xfer += oprot->writeByte(this->result);
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldBegin("reason", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->reason);
  xfer += oprot->writeFieldEnd();
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}


