#ifndef XMLP7MFILE_H
#define XMLP7MFILE_H

#include <XmlFile>
#include <CkBinData.h>
#include <CkCrypt2.h>
#include <CkXml.h>

class XmlP7mFile : public XmlFile
{
    Q_OBJECT
public:
    explicit XmlP7mFile(QObject *parent = nullptr);

protected:
    virtual QString openData() override;
};

#endif // XMLP7MFILE_H
