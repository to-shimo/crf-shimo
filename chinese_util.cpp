/***************************************************************************
 * @file chinese character detect
 * @author toshimo@126.com
 * @date 2016/03/03 11:24:09
 * @brief 
 *  
 **************************************************************************/

#include "chinese_util.h"

/**
 * @brief 返回gbk编码的字符串pos位置开始第一个字符的字节数
 * @param[in] pszStr : const char* 输入字符串
 * @param[in] inLen : int 字符串长度
 * @param[in] pos : 字符偏移
 * @return 长度值，若不能识别编码，则返回长度为1
 **/
int crfshimo_get_hanzi_len_gbk(const char *pszStr, int iLen, int iPos)
{
    if(NULL == pszStr || (iLen <= 0) || (iPos <= -1) || (iPos >= iLen))
    {
        return 0;
    }
    unsigned char *pTmpBuf = (unsigned char*)(pszStr + iPos);
    int iRetLen = 1;
    
    if((iPos + 1 < iLen) && (pTmpBuf[0] >= 0x81 && pTmpBuf[0] <= 0xfe))
    {    
        if(pTmpBuf[1] >= 0x40 && pTmpBuf[1] <= 0xfe) //2个字节
        {
            iRetLen = 2;
        }
        else if(pTmpBuf[1] >= 0x30 && pTmpBuf[1] <= 0x39) //4个字节
        {
            if((iPos + 3 < iLen) && (pTmpBuf[2] >= 0x81 && pTmpBuf[2] <= 0xfe)
                && (pTmpBuf[3] >= 0x30 && pTmpBuf[3] <= 0x39))
            {
                iRetLen = 4;
            }
        }
    }
    
    return iRetLen;
}


/**
 * @brief 返回utf8编码的字符串pos位置开始第一个字符的字节数
 * @param[in] pszStr : const char* 输入字符串
 * @param[in] iLen : int 字符串长度
 * @param[in] pos : 字符偏移
 * @return 长度值，若不能识别编码，则返回长度为1
 **/
int crfshimo_get_hanzi_len_utf8(const char *pszStr, int iLen, int iPos)
{
    if(NULL == pszStr || iLen <= 0 || (iPos <= -1) || iPos >= iLen)
    {
        return 0;
    }
    unsigned char *pTmpBuf = (unsigned char*)(pszStr + iPos);
    int iRetLen = 1; //默认返回1

    if(pTmpBuf[0] < 0x80)
    {
        iRetLen = 1;
    }
    else if(iPos + 1 < iLen && pTmpBuf[0] >= 0xC2 && pTmpBuf[0] < 0xE0 && pTmpBuf[1] >>6 == 2)
    {
        iRetLen = 2;
    }
    else if(iPos + 2 < iLen && pTmpBuf[0] >>4 == 14 && pTmpBuf[1] >> 6 == 2 &&
        pTmpBuf[2] >> 6 == 2 && (pTmpBuf[0] > 0xE0 || pTmpBuf[1] >= 0xA0))
    {
        iRetLen = 3;
    }
    else if(iPos + 3 < iLen && pTmpBuf[0] >> 3 == 30 && pTmpBuf[1] >> 6 == 2 &&
        pTmpBuf[2] >> 6 == 2 && pTmpBuf[3] >> 6 == 2 && pTmpBuf[0] <= 0xF4 &&
        (pTmpBuf[0] > 0xF0 || pTmpBuf[1] >= 0x90))
    {
        iRetLen = 4;
    }

    return iRetLen;
}



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
    char* pszOutStr, int iLenOut, int* piLenIdx, int iLenMax)
{
    if(NULL == pszStr || NULL == pszOutStr || NULL == piLenIdx
        || (iLen <= 0) || (iLen > iLenOut) || (iLenMax <= 0))
    {
        return false;
    }

    if(NULL == crfshimo_to_half_lower(pszStr, iLen, pszOutStr, iLenOut, piLenIdx, iLenMax))
    {
        return false;
    }
    return true;
}


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
    char* pszDest, int iDestsize, int* piLenIdx, int iLenMax)
{
    if(NULL == pszSrcIn || NULL == pszDest || NULL == piLenIdx
        || iSrcsize <= 0 || iDestsize <= 0 || iSrcsize > iDestsize || iLenMax <= 0)
    {
        return NULL;
    }

    const unsigned char* pszSrc = (const unsigned char *)pszSrcIn;
    unsigned int uiVal;
    int iCharlen;
    int iSum = 0;
    int iTmpsum = 0;
    char* pszPtr;
    unsigned int uiRet;
    unsigned char ucLow;
    unsigned char ucHigh;
    
    pszPtr = pszDest;
    while(*pszSrc && iSrcsize > 0 && iDestsize > 0) 
    {
        iCharlen = crfshimo_get_hanzi_len_utf8((const char*)pszSrc, iSrcsize, 0);
        if(iCharlen <= 0) 
        {
            break;
        }

        iSum += iCharlen;

        if(iCharlen == 1) 
        {
            //大小写转换
            if(*pszSrc >= 'A' && *pszSrc <= 'Z') 
            {
                *pszPtr = *pszSrc + 32;
                pszPtr ++;
                pszSrc ++;
                iSrcsize --;
                iDestsize --;

                iTmpsum += 1;
                if(iTmpsum >= iLenMax)
                {
                    return NULL;
                }
                piLenIdx[iTmpsum] = iSum;

                continue;
            }           
        }
        if(iCharlen == 3) 
        {
            uiVal = ((pszSrc[0] & 0x0F) << 12) | ((pszSrc[1] & 0x3F) << 6) | (pszSrc[2] & 0x3F);
            ucHigh = uiVal >> 8;
            ucLow = uiVal & 0xFF;
            if(uiVal == 0x3000) //全角空格
            {
                *pszPtr++ = ' ';
                iDestsize--;
                pszSrc += 3;
                iSrcsize -= 3;

                iTmpsum += 1;
                if(iTmpsum >= iLenMax)
                {
                    return NULL;
                }                
                piLenIdx[iTmpsum] = iSum;

                continue;
            }
            else if(uiVal >= 0xFF01 && uiVal <= 0xFF5E) //是否为中文标点符号
            {
                *pszPtr = (uiVal & 0xFF) + 32;
                if(*pszPtr >= 0x41 && *pszPtr <= 0x5A) 
                {
                    *pszPtr += 32;
                }
                pszPtr++;
                pszSrc += 3;
                iSrcsize -= 3;
                iDestsize--;

                iTmpsum += 1;
                if(iTmpsum >= iLenMax)
                {
                    return NULL;
                }                
                piLenIdx[iTmpsum] = iSum;

                continue;
            }
            else if((ucHigh == 0x20 && (uiRet = tohalf_20[ucLow]) != ucLow)
                    || (ucHigh == 0x30 && (uiRet = tohalf_30[ucLow]) != ucLow)
                    || (ucHigh == 0xFF && (uiRet = tohalf_FF[ucLow]) != ucLow))
            {
                *pszPtr++ = uiRet;
                pszSrc += 3;
                iSrcsize -= 3;
                iDestsize--;

                iTmpsum += 1;
                if(iTmpsum >= iLenMax)
                {
                    return NULL;
                }                
                piLenIdx[iTmpsum] = iSum;

                continue;
             }
        }
        
        if(iDestsize <= iCharlen) 
        {
            return NULL;
        }
        if(pszPtr != (char*)pszSrc) //重复字符串不复制，当原地转换且无全角字符时候触发
        {
            memcpy(pszPtr, pszSrc, iCharlen);
        }
        pszSrc += iCharlen;
        pszPtr += iCharlen;
        iSrcsize -= iCharlen;
        iDestsize -= iCharlen;

        iTmpsum += iCharlen;
        if(iTmpsum >= iLenMax)
        {
            return NULL;
        }        
        piLenIdx[iTmpsum] = iSum;
    }
    if(iDestsize > 0) 
    {
        *pszPtr = '\0';
        return pszDest;
    }
    else 
    {
        return NULL;
    }

}




/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
