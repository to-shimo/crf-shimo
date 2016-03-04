#include <stdlib.h>
#include <string.h>

#include "tohalf.h"

/**
 * @brief 返回gbk编码的字符串pos位置开始第一个字符的字节数
 * @param[in] pszStr : const char* 输入字符串
 * @param[in] inLen : int 字符串长度
 * @param[in] pos : 字符偏移
 * @return 长度值，若不能识别编码，则返回长度为1
 **/
int crfshimo_get_hanzi_len_gbk(const char *pszStr, int iLen, int iPos);

/**
 * @brief 返回utf8编码的字符串pos位置开始第一个字符的字节数
 * @param[in] pszStr : const char* 输入字符串
 * @param[in] iLen : int 字符串长度
 * @param[in] pos : 字符偏移
 * @return 长度值，若不能识别编码，则返回长度为1
 **/
int crfshimo_get_hanzi_len_utf8(const char *pszStr, int iLen, int iPos);

/**
 * @brief 输入大写转小写
 * @param[in] pszStr : const char* 输入字符串
 * @param[in] iLen : int 字符串长度
 * @param[out] pszOutStr : char* 转换小写后的字符串;请确保pszOutStr已经申请空间
 * @param[out] iLenOut : int pszOutStr的空间大小
 * @param[in] piLenIdx : int* 全角字符串与半角字符串长度的映射
 * @param [in] iLenMax : int piLenIdx数组的最大长度 
 * @return 成功=true    失败=false
 **/
bool crfshimo_string_normalize(const char* pszStr, int iLen, 
    char* pszOutStr, int iLenOut, int* piLenIdx, int iLenMax);
	

/**
 * @brief 将输入字符串中的所有全角字符转半角，所有大写字符转小写
 *        该函数可以原地字符串转换
 *
 * @param [in] pszSrcIn : const char* 输入原始字符串
 * @param [in] pszSrcsize : int 字符串长度
 * @param [out] pszDest  : char* 转换后的字符串
 * @param [in] iDestsize : int 存储转换后字符串的空间大小
 * @param [in/out] piLenIdx : int* 全角字符串与半角字符串长度的映射，用于调整offset与length
 * @param [in] iLenMax : int piLenIdx数组的最大长度
 * @return  返回pszDest指针=成功    NULL=失败
**/
char* crfshimo_to_half_lower(const char* pszSrcIn, int iSrcsize, 
    char* pszDest, int iDestsize, int* piLenIdx, int iLenMax);
