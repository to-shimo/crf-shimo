/***************************************************************************
 * @file chinese character detect
 * @author toshimo@126.com
 * @date 2016/03/03 11:24:09
 * @brief 
 *  
 **************************************************************************/

#include "chinese_util.h"

/**
 * @brief ����gbk������ַ���posλ�ÿ�ʼ��һ���ַ����ֽ���
 * @param[in] pszStr : const char* �����ַ���
 * @param[in] inLen : int �ַ�������
 * @param[in] pos : �ַ�ƫ��
 * @return ����ֵ��������ʶ����룬�򷵻س���Ϊ1
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
        if(pTmpBuf[1] >= 0x40 && pTmpBuf[1] <= 0xfe) //2���ֽ�
        {
            iRetLen = 2;
        }
        else if(pTmpBuf[1] >= 0x30 && pTmpBuf[1] <= 0x39) //4���ֽ�
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
 * @brief ����utf8������ַ���posλ�ÿ�ʼ��һ���ַ����ֽ���
 * @param[in] pszStr : const char* �����ַ���
 * @param[in] iLen : int �ַ�������
 * @param[in] pos : �ַ�ƫ��
 * @return ����ֵ��������ʶ����룬�򷵻س���Ϊ1
 **/
int crfshimo_get_hanzi_len_utf8(const char *pszStr, int iLen, int iPos)
{
    if(NULL == pszStr || iLen <= 0 || (iPos <= -1) || iPos >= iLen)
    {
        return 0;
    }
    unsigned char *pTmpBuf = (unsigned char*)(pszStr + iPos);
    int iRetLen = 1; //Ĭ�Ϸ���1

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
 * @brief �����дתСд
 * @param[in] pszStr : const char* �����ַ���
 * @param[in] iLen : int �ַ�������
 * @param[out] pszOutStr : char* ת��Сд����ַ���;��ȷ��pszOutStr�Ѿ�����ռ�
 * @param[out] iLenOut : int pszOutStr�Ŀռ��С
 * @param[in] piLenIdx : int* ȫ���ַ��������ַ������ȵ�ӳ��
 * @param [in] iLenMax : int piLenIdx�������󳤶� 
 * @return �ɹ�=true    ʧ��=false
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
 * @brief �������ַ����е�����ȫ���ַ�ת��ǣ����д�д�ַ�תСд
 *        �ú�������ԭ���ַ���ת��
 *
 * @param [in] pszSrcIn : const char* ����ԭʼ�ַ���
 * @param [in] pszSrcsize : int �ַ�������
 * @param [out] pszDest  : char* ת������ַ���
 * @param [in] iDestsize : int �洢ת�����ַ����Ŀռ��С
 * @param [in/out] piLenIdx : int* ȫ���ַ��������ַ������ȵ�ӳ�䣬���ڵ���offset��length
 * @param [in] iLenMax : int piLenIdx�������󳤶�
 * @return  ����pszDestָ��=�ɹ�    NULL=ʧ��
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
            //��Сдת��
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
            if(uiVal == 0x3000) //ȫ�ǿո�
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
            else if(uiVal >= 0xFF01 && uiVal <= 0xFF5E) //�Ƿ�Ϊ���ı�����
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
        if(pszPtr != (char*)pszSrc) //�ظ��ַ��������ƣ���ԭ��ת������ȫ���ַ�ʱ�򴥷�
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
