
#include <chrtool.h>

#include <QByteArray>

// 密钥
static const char g_cKey[256] = {
	 124,   -16,    78,    56,   -32,   101,    74,   109,     3,     6, 
	  94,    -4,   -31,   -28,    -7,   -49,   -71,   -84,   -14,    51, 
	 117,    19,  -103,  -101,   -55,   -45,   105,     4,    41,  -113, 
	-116,    -3,   -94,    54,   -97,   -24,   -78,   -91,   123,    71, 
	 102,    48,    80,   -92,  -117,   121,   -20,   -68,    -2,  -108, 
	 108,     7,     1,    35,    55,  -126,    64,  -127,   -34,   -73, 
	 -51,    87,   -18,  -114,  -122,    18,  -111,    17,    37,   -33, 
	 -64,    39,    67,   -85,    42,   -74,   -26,   -29,    99,   -72, 
	 -93,    57,    44,   -10,   -54,  -121,  -110,  -120,   111,   -25, 
	  82,    25,  -123,   -63,    50,    36,   112,    86,   -77,    27, 
	 -37,   -27,   -19,   -47,   -96,    81,  -107,  -119,   -36,    75, 
	 -11,  -125,   -30,  -102,   -41,   113,   -95,  -105,   -90,   -44, 
	-112,   -53,   -12,    34,    91,    10,   120,    83,  -100,     5, 
	  11,   -42,    79,   -65,    88,   115,    58,   -98,    53,   -58, 
	  22,   125,   -61,   107,    21,    98,    77,  -104,   -13,    31, 
	 -35,   -23,   -88,   127,   -60,  -115,    28,    47,   -70,    26, 
	 103,    -6,    92,     8,    89,   -86,    61,    97,   -56,    73, 
	 -87,   -22,    -5,    52,    24,    20,   -69,   -52,   -89,    65, 
	  84,    96,    29,    49,     9,   -81,    14,    30,   114,   122, 
	-106,     2,  -124,  -109,   -40,    43,   -15,    15,   -38,    76, 
	 106,    90,   110,    46,    40,   116,    70,    63,    13,   -43, 
	  60,   -46,   -99,   126,    68,    33,   -82,    62,   -50,    -8, 
	  32,   -39,     0,    -9,    85,   -17,    45,   -48,   -59,    38, 
	  72,   100,    69,   118,   -80,   -75,   -21,    95,    12,   104, 
	 -76,    23,   -79,    66,    16,   -57,   -66,    93,   -62,   -67, 
	-118,   119,   -83,  -128,    -1,    59
};

ChrTool::ChrTool() {
    for(int i = 0; i < 256; ++i) {
        m_tEncryptTable.insert(i, g_cKey[i]);
        m_tEncryptTable.insert(g_cKey[i], i);
    }
}

ChrTool::~ChrTool() {
    /* Nothing */
}

void ChrTool::encryptStr(const std::string &rsSrc, QString &rsDest) {
    int iLength = rsSrc.length();
    QByteArray tByte;
    tByte.resize(iLength);
    for(int i = 0; i < iLength; ++i) {
        tByte[i] = m_tEncryptTable.value(rsSrc[i]);
    }
    rsDest = tByte;
}

void ChrTool::decryptStr(const std::string &rsCipher, QString &rsPlain) {
    int iLength = rsCipher.length();
    QByteArray tByte;
    tByte.resize(iLength);
    for(int i = 0; i < iLength; ++i) {
        tByte[i] = m_tDecryptTable.value(rsCipher[i]);
    }
    rsPlain = tByte;
}

void ChrTool::decryptAll(const QStringList &rtCipherList) {
#if 0
    QByteArray tResultArray[CHRT_CRYPT_NUMBER];
    for (int i = 0; i < CHRT_CRYPT_NUMBER; ++i) {
        tResultArray[i] = QByteArray::fromHex(rtCipherList[i].toLocal8Bit());
    }

    chrDecryptStr(tResultArray[0].toStdString(), TASK_JSON_URL);
    chrDecryptStr(tResultArray[1].toStdString(), TASK_CMD_LIST_HEAD);
    chrDecryptStr(tResultArray[2].toStdString(), TASK_CMD_KILL_HEAD);
    chrDecryptStr(tResultArray[3].toStdString(), EXAM_URL_REAR);
#ifdef _DEBUG_STATE
    qDebug() << __func__ << ":" << TASK_JSON_URL;
    qDebug() << __func__ << ":" << TASK_CMD_LIST_HEAD;
    qDebug() << __func__ << ":" << TASK_CMD_KILL_HEAD;
    qDebug() << __func__ << ":" << EXAM_URL_REAR;
#endif
#endif
}
