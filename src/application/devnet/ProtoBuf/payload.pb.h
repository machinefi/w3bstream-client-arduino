/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.7 */

#ifndef PB_PAYLOAD_PB_H_INCLUDED
#define PB_PAYLOAD_PB_H_INCLUDED
#include "pb.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _Payload_UserDataType {
    Payload_UserDataType_JSON = 0,
    Payload_UserDataType_PB = 1,
    Payload_UserDataType_RAW = 2
} Payload_UserDataType;

/* Struct definitions */
typedef PB_BYTES_ARRAY_T(64) Payload_sign_t;
typedef PB_BYTES_ARRAY_T(320) Payload_user_t;
typedef struct _Payload {
    Payload_sign_t sign;
    Payload_UserDataType type;
    Payload_user_t user;
} Payload;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _Payload_UserDataType_MIN Payload_UserDataType_JSON
#define _Payload_UserDataType_MAX Payload_UserDataType_RAW
#define _Payload_UserDataType_ARRAYSIZE ((Payload_UserDataType)(Payload_UserDataType_RAW+1))

#define Payload_type_ENUMTYPE Payload_UserDataType


/* Initializer values for message structs */
#define Payload_init_default                     {{0, {0}}, _Payload_UserDataType_MIN, {0, {0}}}
#define Payload_init_zero                        {{0, {0}}, _Payload_UserDataType_MIN, {0, {0}}}

/* Field tags (for use in manual encoding/decoding) */
#define Payload_sign_tag                         1
#define Payload_type_tag                         2
#define Payload_user_tag                         3

/* Struct field encoding specification for nanopb */
#define Payload_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, BYTES,    sign,              1) \
X(a, STATIC,   SINGULAR, UENUM,    type,              2) \
X(a, STATIC,   SINGULAR, BYTES,    user,              3)
#define Payload_CALLBACK NULL
#define Payload_DEFAULT NULL

extern const pb_msgdesc_t Payload_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define Payload_fields &Payload_msg

/* Maximum encoded size of messages (where known) */
#define Payload_size                             391

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif