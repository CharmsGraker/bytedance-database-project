/*****************************************************************************
 * Copyright (c) 2022, Bytedance Inc. All rights reserved.
 *
 * @file: univ.h
 *
 * @description: Define some common constants & macros.
 *
 * @created_at: 2021/12
 *
 * Managed by database team @ByteDance.
 *****************************************************************************/
#pragma once
#include <cstdint>
#include <exception>
#include <limits>
namespace bytedance_db_project {

#ifndef FIXED_FIELD_LEN
#define FIXED_FIELD_LEN (4)

// define MAX adn MIN
#ifdef FIXED_FIELD_LEN
#define FIXED_FIELD_MAX std::numeric_limits<int32_t>::max()
#define FIXED_FIELD_MIN std::numeric_limits<int32_t>::min()
#endif
#define WRITE_BYTE(dest,src,len) (std::memcpy(dest,src,len))

#define DECODE_INT32(char_addr) (*(int32_t *)(char_addr))
#define ENCODE_INT32(x)((char *)&(x))

#endif
}
