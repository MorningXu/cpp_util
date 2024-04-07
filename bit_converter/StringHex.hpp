//
// @Author: MorningXu 
// @Description:
// @Date: 2023-07-04
//

#pragma once

#include <string>
#include <vector>
#include <iostream>

namespace lanyueuav {
    class StringHex {
    public:
        /**
        *string 转 hex 两个16位字符 中间已空格隔开
        *比如 02 02 00 01 00 01
        *最多1024个字符
        *  @return: int 数据字节数
        */
        static int stringToHex(const std::string &str, std::vector<unsigned char> &out) {
            unsigned char hexdata, lowhexdata;
            int len = str.length();

            if (len > 1024) { //限制大小
                len = 1024;
            }

            char lstr, hstr;

            int counter = 0;

            for (int i = 0; i < len; i++) {
                //char lstr,
                hstr = str[i];   //高位
                if (hstr == ' ') {//跳过空格
                    continue;
                }
                i++;
                lstr = str[i]; //低位
                hexdata = convertCharToHex(hstr);
                lowhexdata = convertCharToHex(lstr);
                if ((hexdata == 16) || (lowhexdata == 16))
                    break;
                else {
                    hexdata = hexdata * 16 + lowhexdata;
                    counter++;
                }
                out.push_back(hexdata);
            }
            return counter;
        }

        /**
         * @brief char 转 string
         *
         * @param bytes
         * @param len
         * @return string
         */
        static std::string charToHexString(const std::vector<unsigned char> &bytes, int len) {
            const char HEX[] = "0123456789ABCDEF";
            std::string lStr;
            for (int i = 0; i < len; i++) {
                char lTemp = bytes[i];
                lStr.push_back(HEX[(lTemp & 0xf0) >> 4]);
                lStr.push_back(HEX[lTemp & 0x0f]);
                if (i < len - 1) {
                    lStr += " ";
                }
            }
            return lStr;
        }

        /**
         * char 转 string 中间不留空格
         * @param bytes
         * @param len
         * @return
         */
        static std::string charToHexStringWithoutSpace(const std::vector<unsigned char> &bytes, int len) {
            const char HEX[] = "0123456789ABCDEF";
            std::string lStr;
            for (int i = 0; i < len; i++) {
                char lTemp = bytes[i];
                lStr.push_back(HEX[(lTemp & 0xf0) >> 4]);
                lStr.push_back(HEX[lTemp & 0x0f]);
            }
            return lStr;
        }

        /**
         * @brief 在buffer最后两位添加crc16
         *
         * @param buffer
         * @param len
         * @return true
         * @return false
         */
        static bool crc16(std::vector<unsigned char> &buffer, int len) {
            int ret = 0;
            unsigned short CRC = 0xffff;
            unsigned short POLYNOMIAL = 0xa001;
            int i, j;

            if (len == 0) {
                return false;
            }

            for (i = 0; i < len; i++) {
                CRC ^= buffer[i];
                for (j = 0; j < 8; j++) {
                    if ((CRC & 0x0001) != 0) {
                        CRC >>= 1;
                        CRC ^= POLYNOMIAL;
                    } else {
                        CRC >>= 1;
                    }
                }
            }

            buffer.push_back((unsigned char) (CRC & 0x00ff));
            buffer.push_back((unsigned char) (CRC >> 8));
            return true;
        }

        static std::string charToString(const std::vector<unsigned char> &in, int start, int len) {
            std::string result;
            for (int i = 0; i < len; i++) {
                result.push_back(in[start + i]);
            }
            return result;
        }

        static char convertCharToHex(char ch) {
            if ((ch >= '0') && (ch <= '9'))
                return ch - 0x30;
            else if ((ch >= 'A') && (ch <= 'F'))
                return ch - 'A' + 10;
            else if ((ch >= 'a') && (ch <= 'f'))
                return ch - 'a' + 10;
            else
                return (-1);
        }
    };
}