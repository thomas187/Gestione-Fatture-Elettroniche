#include "xml-p7m-file.h"

QString XmlP7mFile::openData()
{
    CkBinData bd;
    bool success = bd.LoadFile(this->path().toStdString().c_str());
    if (success != true) {
        qDebug() << "Failed to load the .p7m file";
        return "";
    }

    CkCrypt2 crypt;

    // Verify and extrct the payload contained within the .p7m.
    // In this case, the payload is the FatturaPA XML.
    // If successful, the resulting bd will contain only the XML.
    bool bVerified = crypt.OpaqueVerifyBd(bd);
    if (bVerified != true) {
        qDebug() << crypt.lastErrorText();
        qDebug() << "Failed to extract and verify.";
        return "";
    }

    // Alternatively, load into an XML object and emit.
    CkXml xml;
    xml.LoadXml(bd.getString("utf-8"));
    auto xmlData = xml.getXml();
    return xmlData;
}
