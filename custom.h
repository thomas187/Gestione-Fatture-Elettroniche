#ifndef CUSTOM_H
#define CUSTOM_H

#include <Archivio>
#include <XmlList>

namespace CustomTypes{

    inline void setup()
    {
        qmlRegisterSingletonType<Archivio>("Archivio", 1, 0, "Archivio", &Archivio::qmlInstance);
        qRegisterMetaType<XmlList>("XmlList");
        qmlRegisterType<XmlList>("XmlList", 1, 0, "XmlList");
        qmlRegisterType<XmlFile>("XmlFile", 1, 0, "XmlFile");
        qRegisterMetaType<XmlList::XmlFileRoles>("XmlList::XmlFileRoles");
        qRegisterMetaType<XmlFile::SYNC_STATUS>("XmlFile::SYNC_STATUS");
	}
}

#endif // CUSTOM_H
