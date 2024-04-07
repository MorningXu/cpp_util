//
// @Author: MorningXu 
// @Description:
// @Date: 2023-07-06
//

#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

namespace lanyueuav {
    class BitConverter {
    public:
        static bool i16_to_bytes(short value, bool is_big_endian, std::vector<unsigned char> &out) {
            if (is_big_endian) {
                out.push_back(value >> 8);
                out.push_back(value & 0xFF);
            } else {
                out.push_back(value & 0xFF);
                out.push_back(value >> 8);
            }
            return true;
        }

        static bool u16_to_bytes(unsigned short value, bool is_big_endian, std::vector<unsigned char> &out) {
            return i16_to_bytes(value, is_big_endian, out);
        }

        static void i32_to_bytes(int32_t value, bool is_big_endian, std::vector<unsigned char> &out) {
            if (is_big_endian) {
                for (int i = 0; i < static_cast<int>(sizeof(int32_t)); i++) {
                    out.push_back(static_cast<uint8_t>((value >> (24 - i * 8)) & 0xFF));
                }
            } else {
                for (int i = static_cast<int>(sizeof(int32_t)) - 1; i >= 0; i--) {
                    out.push_back(static_cast<uint8_t>((value >> (24 - i * 8)) & 0xFF));
                }
            }
        }

        static void u32_to_bytes(uint32_t value, bool is_big_endian, std::vector<unsigned char> &out) {
            return i32_to_bytes(value, is_big_endian, out);
        }

        static void i64_to_bytes(int64_t value, bool is_big_endian, std::vector<unsigned char> &out) {
            if (is_big_endian) {
                for (int i = 0; i < static_cast<int>(sizeof(int64_t)); i++) {
                    out.push_back(static_cast<uint8_t>((value >> (56 - i * 8)) & 0xFF));
                }
            } else {
                for (int i = static_cast<int>(sizeof(int64_t)) - 1; i >= 0; i--) {
                    out.push_back(static_cast<uint8_t>((value >> (56 - i * 8)) & 0xFF));
                }
            }
        }

        static void u64_to_bytes(uint64_t value, bool is_big_endian, std::vector<unsigned char> &out) {
            return i64_to_bytes(value, is_big_endian, out);
        }

        static void create_bytes_from_bits(const std::vector<bool> &bits, std::vector<unsigned char> &out) {
            for (int i = 0; i < static_cast<int>(bits.size() / 8); i++) {
                uint8_t b = 0;
                for (int j = 0; j < 8; j++) {
                    b = b + (bits[i * 8 + j] << j);
                }
                out.push_back(b);
            }
        }

        static void f32_to_bytes(float_t value, bool is_big_endian, std::vector<unsigned char> &out) {
            std::vector<bool> bits;
            bits.reserve(sizeof(float_t) * 8);
            bits.push_back(value < 0);

            int exponent;
            float_t mantissa = std::frexp(value, &exponent);
            exponent = (exponent - 1) + 127;

            for (int i = 0; i < 8; i++) {
                bits.push_back(exponent % 2);
                exponent = exponent / 2;
            }
            std::reverse(bits.begin() + 1, bits.begin() + 1 + 8);
            mantissa = mantissa * 2 - 1;
            for (int i = 0; i < 23; i++) {
                mantissa = mantissa * 2;
                if (mantissa >= 1.0) {
                    mantissa = mantissa - 1.0;
                    bits.push_back(true);
                } else {
                    bits.push_back(false);
                }
            }
            if (is_big_endian) {
                return create_bytes_from_bits(bits, out);
            } else {
                std::reverse(bits.begin(), bits.end());
                return create_bytes_from_bits(bits, out);
            }
        }


        static void f64_to_bytes(double_t value, bool is_big_endian, std::vector<unsigned char> &out) {
            std::vector<bool> bits;
            bits.reserve(sizeof(double_t) * 8);
            bits.push_back(value < 0);

            int exponent;
            double_t mantissa = std::frexp(value, &exponent);
            exponent = (exponent - 1) + 1023;

            for (int i = 0; i < 11; i++) {
                bits.push_back(exponent % 2);
                exponent = exponent / 2;
            }
            std::reverse(bits.begin() + 1, bits.begin() + 1 + 11);
            mantissa = mantissa * 2 - 1;
            for (int i = 0; i < 52; i++) {
                mantissa = mantissa * 2;
                if (mantissa >= 1.0) {
                    mantissa = mantissa - 1.0;
                    bits.push_back(true);
                } else {
                    bits.push_back(false);
                }
            }
            if (is_big_endian) {
                return create_bytes_from_bits(bits, out);
            } else {
                std::reverse(bits.begin(), bits.end());
                return create_bytes_from_bits(bits, out);
            }
        }

        static short bytes_to_i16(const std::vector<unsigned char> &input_it, int startIndex, bool is_big_endian) {
            short result;
            if (is_big_endian) {
                result = input_it[startIndex];
                result = (result << 8) + input_it[startIndex + 1];
                return result;
            } else {
                result = input_it[startIndex];
                result = result + (input_it[startIndex + 1] << 8);
                return result;
            }
        }


        static unsigned short
        bytes_to_u16(const std::vector<unsigned char> &input_it, int startIndex, bool is_big_endian) {
            return static_cast<unsigned short>(bytes_to_i16(input_it, startIndex, is_big_endian));
        }


        static int bytes_to_i32(const std::vector<unsigned char> &input_it, int startIndex, bool is_big_endian) {
            int32_t result = 0;
            if (is_big_endian) {
                for (int i = 0; i < 4; i++) {
                    result = result + (input_it[startIndex + i] << (8 * (4 - 1 - i)));
                }
                return result;
            } else {
                for (int i = 0; i < 4; i++) {
                    result = result + (input_it[startIndex + i] << (8 * i));
                }
                return result;
            }
        }

        static unsigned int
        bytes_to_u32(const std::vector<unsigned char> &input_it, int startIndex, bool is_big_endian) {
            return static_cast<unsigned int>(bytes_to_i32(input_it, startIndex, is_big_endian));
        }

        static long bytes_to_i64(const std::vector<unsigned char> &input_it, int startIndex, bool is_big_endian) {
            int64_t result = 0;
            if (is_big_endian) {
                for (int i = 0; i < 8; i++) {
                    result = result + (input_it[startIndex + i] << (8 * (8 - 1 - i)));
                }
                return result;
            } else {
                for (int i = 0; i < 8; i++) {
                    result = result + (input_it[startIndex + i] << (8 * i));
                }
                return result;
            }
        }

        static unsigned long
        bytes_to_u64(const std::vector<unsigned char> &input_it, int startIndex, bool is_big_endian) {
            return static_cast<unsigned long>(bytes_to_i64(input_it, startIndex, is_big_endian));
        }

        static float bytes_to_f32(const std::vector<unsigned char> &input_it, int startIndex, bool is_big_endian) {
            std::vector<bool> bits;
            bits.reserve(sizeof(float_t) * 8);
            for (int i = 0; i < static_cast<int>(sizeof(float_t)); i++) {
                uint8_t b = input_it[startIndex + i];
                for (int j = 0; j < 8; j++) {
                    bits.push_back(b % 2);
                    b = b / 2;
                }
            }
            if (!is_big_endian) {
                std::reverse(bits.begin(), bits.end());
            }
            int sign = bits[0] ? (-1) : (+1);
            int exponent = 0;
            for (int i = 0; i < 8; i++) {
                exponent = exponent + bits[1 + i] * (1 << (8 - 1 - i));
            }
            exponent = exponent - 127;
            float_t mantissa = 1.0;
            float_t cur = 0.5;
            for (int i = 0; i < 23; i++) {
                mantissa = mantissa + bits[1 + 8 + i] * cur;
                cur = cur / 2;
            }
            return sign * std::ldexp(mantissa, exponent);
        }

        static double bytes_to_f64(const std::vector<unsigned char> &input_it, int startIndex, bool is_big_endian) {
            std::vector<bool> bits;
            bits.reserve(sizeof(double_t) * 8);
            for (int i = 0; i < static_cast<int>(sizeof(double_t)); i++) {
                uint8_t b = input_it[startIndex + i];
                for (int j = 0; j < 8; j++) {
                    bits.push_back(b % 2);
                    b = b / 2;
                }
            }
            if (!is_big_endian) {
                std::reverse(bits.begin(), bits.end());
            }
            int sign = bits[0] ? (-1) : (+1);
            int exponent = 0;
            for (int i = 0; i < 11; i++) {
                exponent = exponent + bits[1 + i] * (1 << (11 - 1 - i));
            }
            exponent = exponent - 1023;
            double_t mantissa = 1.0;
            double_t cur = 0.5;
            for (int i = 0; i < 52; i++) {
                mantissa = mantissa + bits[1 + 11 + i] * cur;
                cur = cur / 2;
            }
            return sign * std::ldexp(mantissa, exponent);
        }
    };
}
