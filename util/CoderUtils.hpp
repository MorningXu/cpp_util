//
// Created by morningxu on 2/29/24.
//

#pragma once

#include <vector>
#include "BitConverter.hpp"

namespace lanyueuav {
    class coderutils {
    public:
        /**
         * @brief 将字节数组转换为vector
         * @param buf 字节序
         * @param len 字节序长度
         * @param data_array_ 转换后的容器
         * @return
         */
        static void bytes2vector(unsigned char *buf, int len, std::vector<unsigned char> &data_array_) {
            //保存接收字节串
            for (int i = 0; i < len; i++)
                data_array_.push_back(buf[i]);
        }

        /**
         * 查找包头
         * @param data_array_
         * @return
         */
        static int findHeaderIndex(std::vector<unsigned char> &data_array_) {
            if (data_array_.size() > 2) {
                if (data_array_[0] == 0xaa) {
                    if (data_array_[1] == 0x63) {
                        return 0;
                    }
                }
                data_array_.erase(data_array_.begin());
                return findHeaderIndex(data_array_);
            }
            return -1;
        }

        /**
         * 查找包尾
         * @param data_array_
         * @param len 一个完整数据包的长度
         * @return
         */
        static int findEndIndex(std::vector<unsigned char> &data_array_, int len) {
            if (data_array_[len - 2] == 0x09 && data_array_[len - 1] == 0xd7) {
                return len - 2;
            }
            return -1;
        }

        /**
         * ISO和校验
         * @param buf
         * @param len
         * @param startIndex 起始下标
         */
        static void ISOSum(std::vector<unsigned char> &buf, int len, int startIndex) {
            int32_t k1, k2;
            int32_t c0 = 0, c1 = 0;
            unsigned char s1, s0;
            for (int i = startIndex; i < len + startIndex; ++i) {
                c0 = c0 + buf[i];
                c1 = c1 + c0;
            }
            k1 = 0xff - ((c0 + c1) % 0xff);
            k2 = c1;
            s0 = (unsigned char) (k1 % 0xff);
            s1 = (unsigned char) (k2 % 0xff);
            if (s0 == 0) s0 = 0xff;
            if (s1 == 0) s1 = 0xff;
            buf.push_back(s0);
            buf.push_back(s1);
        }

        /**
        * ISO和校验判断
        * @param buf 需要校验的数据包
        * @param len 校验的有效数据长度
        * @param start_index 校验数据域的起始下标
        * @return
        */
        static bool IsISOSum(const std::vector<unsigned char> &buf, int len , int start_index) {
            int32_t c0 = buf[len + start_index], c1 = buf[len + start_index + 1];
            if (c0 * c1 != 0) {
                c0 = 0, c1 = 0;
                for (int i = 9; i < len + 9; ++i) {
                    c0 = c0 + buf[i];
                    c1 = c1 + c0;
                }
                if (c0 % 0xff == 0 && c1 % 0xff == 0) {
                    return true;
                }
            }
            return false;
        }

        /**
         * 累加和
         * @param buf
         * @param len
         * @return
         */
        static unsigned char Sum_data(const std::vector<unsigned char> &buf, int len) {
            unsigned char Sum_data = 0;
            for (int i = 0; i < len; i++) {
                Sum_data += buf[i];
            }
            return (unsigned char) Sum_data;
        }

        /**
         * @brief 包头编码
         * @param len 数据域长
         * @param sender_group 发送端簇id
         * @param sender_id 发送端id
         * @param reciver_group 接收端簇id
         * @param reciver_id 发送端id
         * @param data_array_ 编码后的数据
         */
        static void
        header_encode(uint16_t len, uint8_t sender_group, uint8_t sender_id, uint8_t reciver_group, uint8_t reciver_id,
                      std::vector<unsigned char> &data_array_) {
            data_array_.push_back(0xaa);
            data_array_.push_back(0x63);
            lanyueuav::BitConverter::u16_to_bytes(len, false, data_array_);
            data_array_.push_back(sender_group);
            data_array_.push_back(sender_id);
            data_array_.push_back(reciver_group);
            data_array_.push_back(reciver_id);
        }
    };
}
