import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import Qt.labs.platform 1.1

import Archivio 1.0
import XmlList 1.0
import XmlFile 1.0

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Gestione Fatture")

    property date expFromDate : {
        var date = Date.fromLocaleDateString(Qt.locale("it_IT"),combo.currentText,"MMMM yy")
        date.setYear(date.getYear() + 2000)
        return date
    }
    property date expToDate : {
        var date = expFromDate
        date.setMonth(date.getMonth() + spin.value)
        return date
    }

    property string expFromStrDate : ""
    property string expToStrDate : ""
    onExpFromDateChanged: updateDateStrings()
    onExpToDateChanged: updateDateStrings()
    Component.onCompleted: updateDateStrings()

    function updateDateStrings(){
        expFromStrDate = expFromDate.toLocaleString(Qt.locale("it_IT"),"yyyy-MM-dd")
        expToStrDate = expToDate.toLocaleString(Qt.locale("it_IT"),"yyyy-MM-dd")
    }



    FolderDialog {
        id: folderDialogXlsx
        currentFolder: Archivio.xmlFolder
        folder: Archivio.xmlFolder
        onAccepted: Archivio.xlsxExport(folder, expFromStrDate, expToStrDate)
    }

    FolderDialog {
        id: folderDialogCsv
        currentFolder: Archivio.xmlFolder
        folder: Archivio.xmlFolder
        onAccepted: Archivio.csvExport(folder, expFromStrDate, expToStrDate)
    }

    Control{
        anchors.fill: parent
        padding: 20

        contentItem: ColumnLayout{

            RowLayout{
                Layout.fillWidth: true
                Layout.preferredHeight: 30

                Label{
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    text: "Percorso fatture"
                    verticalAlignment: Qt.AlignBottom
                }

                Label{
                    Layout.preferredWidth: 150
                    Layout.preferredHeight: 30
                    text: "Data inizio"
                    verticalAlignment: Qt.AlignBottom
                }

                Label{
                    Layout.preferredWidth: 150
                    Layout.preferredHeight: 30
                    text: "N°mesi"
                    verticalAlignment: Qt.AlignBottom
                }

                Label {
                    Layout.preferredWidth: 300
                    Layout.preferredHeight: 30
                    text: {
                        var testo = "Estrazione da "
                        testo += combo.currentText+" a "
                        testo += expToDate.toLocaleString(Qt.locale("it_IT"),"MMMM yy")+""
                        return testo.toUpperCase()
                    }
                    verticalAlignment: Qt.AlignBottom
                }
            }

            RowLayout{
                Layout.fillWidth: true
                Layout.preferredHeight: 30

                TextField{
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    text: Archivio.xmlFolder
                    selectByMouse: true
                    persistentSelection: true
                    placeholderText: "Inserire percorso fatture"
                    onTextChanged: {
                        Archivio.xmlFolder = text
                    }
                }

                ComboBox{
                    id: combo
                    Layout.preferredWidth: 150
                    Layout.preferredHeight: 30
                    font.capitalization: Font.Capitalize
                    model: {
                        var date = new Date()
                        var arr = []
                        for(var i=0; i<12; i++){
                            date.setMonth(date.getMonth() - 1)
                            arr.push(date.toLocaleString(Qt.locale("it_IT"),"MMMM yy"))
                        }
                        return arr
                    }
                }

                SpinBox{
                    id: spin
                    Layout.preferredWidth: 150
                    Layout.preferredHeight: 30
                    from: 2
                    to: 12
                }

                Button {
                    Layout.preferredWidth: 150
                    Layout.preferredHeight: 30
                    text: qsTr("Esporta CSV")
                    onClicked: folderDialogCsv.open()
                }
                Button {
                    Layout.preferredWidth: 150
                    Layout.preferredHeight: 30
                    text: qsTr("Esporta Xlsx")
                    onClicked: folderDialogXlsx.open()
                }
            }

            RowLayout {
                RadioButton {
                    checked: true
                    text: qsTr("Intestazione")
                    onCheckedChanged: if(checked){ Archivio.xmlList.filterRole = XmlList.IntestazioneFixedRole }
                }
                RadioButton {
                    text: qsTr("Nome file")
                    onCheckedChanged: if(checked){ Archivio.xmlList.filterRole = XmlList.FileNameFixedRole }
                }
                RadioButton {
                    text: qsTr("Data")
                    onCheckedChanged: if(checked){ Archivio.xmlList.filterRole = XmlList.VDataFixedRole }
                }
                RadioButton {
                    text: qsTr("Partita IVA")
                    onCheckedChanged: if(checked){ Archivio.xmlList.filterRole = XmlList.PartIvaFixedRole }
                }
                RadioButton {
                    text: qsTr("Tipo documento")
                    onCheckedChanged: if(checked){ Archivio.xmlList.filterRole = XmlList.TipoStringaFixedRole }
                }
            }

            TextField{
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                text: Archivio.xmlList.filterString
                placeholderText: "Inserire testo da cercare"
                onTextChanged: {
                    Archivio.xmlList.filterString = text
                }
            }

            Control{
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                background: Rectangle{ color: "#afb8c1" }
                padding: 0

                contentItem: RowLayout{
                    spacing: 20
                    Item{
                        Layout.maximumWidth: 18
                        Layout.preferredWidth: 18
                        Layout.maximumHeight: 18
                        Layout.preferredHeight: 18
                    }
                    Label{
                        Layout.preferredWidth: 150
                        Layout.fillHeight: true
                        verticalAlignment: Label.AlignVCenter
                        horizontalAlignment: Label.AlignHCenter
                        text: "Nome file"
                    }
                    Label{
                        Layout.preferredWidth: 100
                        Layout.fillHeight: true
                        verticalAlignment: Label.AlignVCenter
                        horizontalAlignment: Label.AlignHCenter
                        text: "P.IVA"
                        elide: Label.ElideRight
                    }
                    Label{
                        Layout.preferredWidth: 60
                        Layout.fillHeight: true
                        verticalAlignment: Label.AlignVCenter
                        horizontalAlignment: Label.AlignHCenter
                        text: "Data"
                        elide: Label.ElideRight
                    }
                    Label{
                        Layout.preferredWidth: 150
                        Layout.fillHeight: true
                        verticalAlignment: Label.AlignVCenter
                        horizontalAlignment: Label.AlignHCenter
                        text: "Tipo documento"
                        elide: Label.ElideRight
                    }
                    Label{
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        verticalAlignment: Label.AlignVCenter
                        horizontalAlignment: Label.AlignHCenter
                        text: "Intestazione"
                        elide: Label.ElideRight
                    }
                    Label{
                        Layout.preferredWidth: 90
                        Layout.fillHeight: true
                        verticalAlignment: Label.AlignVCenter
                        horizontalAlignment: Label.AlignHCenter
                        text: "Tot. Fatt."
                        font.bold: true
                    }
                    Label{
                        Layout.preferredWidth: 90
                        Layout.fillHeight: true
                        verticalAlignment: Label.AlignVCenter
                        horizontalAlignment: Label.AlignHCenter
                        text: "Imponibile (4%)"
                    }
                    Label{
                        Layout.preferredWidth: 90
                        Layout.fillHeight: true
                        verticalAlignment: Label.AlignVCenter
                        horizontalAlignment: Label.AlignHCenter
                        text: "Imposta (4%)"
                    }
                    Label{
                        Layout.preferredWidth: 90
                        Layout.fillHeight: true
                        verticalAlignment: Label.AlignVCenter
                        horizontalAlignment: Label.AlignHCenter
                        text: "Imponibile (5%)"
                    }
                    Label{
                        Layout.preferredWidth: 90
                        Layout.fillHeight: true
                        verticalAlignment: Label.AlignVCenter
                        horizontalAlignment: Label.AlignHCenter
                        text: "Imposta (5%)"
                    }
                    Label{
                        Layout.preferredWidth: 90
                        Layout.fillHeight: true
                        verticalAlignment: Label.AlignVCenter
                        horizontalAlignment: Label.AlignHCenter
                        text: "Imponibile (10%)"
                    }
                    Label{
                        Layout.preferredWidth: 90
                        Layout.fillHeight: true
                        verticalAlignment: Label.AlignVCenter
                        horizontalAlignment: Label.AlignHCenter
                        text: "Imposta (10%)"
                    }
                    Label{
                        Layout.preferredWidth: 90
                        Layout.fillHeight: true
                        verticalAlignment: Label.AlignVCenter
                        horizontalAlignment: Label.AlignHCenter
                        text: "Imponibile (22%)"
                    }
                    Label{
                        Layout.preferredWidth: 90
                        Layout.fillHeight: true
                        verticalAlignment: Label.AlignVCenter
                        horizontalAlignment: Label.AlignHCenter
                        text: "Imposta (22%)"
                    }
                }
            }

            ListView{
                id: listView
                model: Archivio.xmlList.proxy
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                spacing: 10
                delegate: Button{
                    width: listView.width
                    height: 30
                    background: Rectangle{ color: index===listView.currentIndex ? "wheat" : "transparent" }
                    contentItem: RowLayout{
                        spacing: 20
                        Rectangle{
                            Layout.maximumWidth: 18
                            Layout.preferredWidth: 18
                            Layout.maximumHeight: 18
                            Layout.preferredHeight: 18
                            color: {
                                if(sync===XmlFile.OFF)
                                    return "gray"
                                if(sync===XmlFile.LOADING)
                                    return "orange"
                                if(sync===XmlFile.ERROR)
                                    return "red"
                                if(sync===XmlFile.READY)
                                    return "green"
                                console.log("Impossible to check sync-state")
                                return "pink"
                            }
                            radius: height/2
                        }
                        TextEdit{
                            Layout.preferredWidth: 150
                            Layout.fillHeight: true
                            verticalAlignment: Label.AlignVCenter
                            text: fileName
                            readOnly: true
                            selectByMouse: true
                            persistentSelection: true
                            onFocusChanged: {
                                if(focus)
                                    listView.currentIndex = index
                            }
                        }
                        Label{
                            Layout.preferredWidth: 100
                            Layout.fillHeight: true
                            verticalAlignment: Label.AlignVCenter
                            text: partIva
                            elide: Label.ElideRight
                        }
                        Label{
                            Layout.preferredWidth: 60
                            Layout.fillHeight: true
                            verticalAlignment: Label.AlignVCenter
                            text: vData
                            elide: Label.ElideRight
                        }
                        Label{
                            Layout.preferredWidth: 150
                            Layout.fillHeight: true
                            verticalAlignment: Label.AlignVCenter
                            text: tipoStringa
                            elide: Label.ElideRight
                        }
                        Label{
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            verticalAlignment: Label.AlignVCenter
                            text: intestazione
                            elide: Label.ElideRight
                        }
                        Label{
                            Layout.preferredWidth: 90
                            Layout.fillHeight: true
                            verticalAlignment: Label.AlignVCenter
                            horizontalAlignment: Label.AlignHCenter
                            text: totale
                            font.bold: true
                        }
                        Label{
                            Layout.preferredWidth: 90
                            Layout.fillHeight: true
                            verticalAlignment: Label.AlignVCenter
                            horizontalAlignment: Label.AlignHCenter
                            text: imponibile_4
                        }
                        Label{
                            Layout.preferredWidth: 90
                            Layout.fillHeight: true
                            verticalAlignment: Label.AlignVCenter
                            horizontalAlignment: Label.AlignHCenter
                            text: imposta_4
                        }
                        Label{
                            Layout.preferredWidth: 90
                            Layout.fillHeight: true
                            verticalAlignment: Label.AlignVCenter
                            horizontalAlignment: Label.AlignHCenter
                            text: imponibile_5
                        }
                        Label{
                            Layout.preferredWidth: 90
                            Layout.fillHeight: true
                            verticalAlignment: Label.AlignVCenter
                            horizontalAlignment: Label.AlignHCenter
                            text: imposta_5
                        }
                        Label{
                            Layout.preferredWidth: 90
                            Layout.fillHeight: true
                            verticalAlignment: Label.AlignVCenter
                            horizontalAlignment: Label.AlignHCenter
                            text: imponibile_10
                        }
                        Label{
                            Layout.preferredWidth: 90
                            Layout.fillHeight: true
                            verticalAlignment: Label.AlignVCenter
                            horizontalAlignment: Label.AlignHCenter
                            text: imposta_10
                        }
                        Label{
                            Layout.preferredWidth: 90
                            Layout.fillHeight: true
                            verticalAlignment: Label.AlignVCenter
                            horizontalAlignment: Label.AlignHCenter
                            text: imponibile_22
                        }
                        Label{
                            Layout.preferredWidth: 90
                            Layout.fillHeight: true
                            verticalAlignment: Label.AlignVCenter
                            horizontalAlignment: Label.AlignHCenter
                            text: imposta_22
                        }
                    }
                    onClicked: listView.currentIndex = index
                    onDoubleClicked: {
                        listView.currentIndex = index
                        var proxyIndex = Archivio.xmlList.proxy.index(index,0)
                        var sourceIndex = Archivio.xmlList.proxy.mapToSource(proxyIndex)
                        Qt.openUrlExternally(Archivio.xmlList.get(sourceIndex.row).path)
                    }
                }

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AlwaysOn
                    active: ScrollBar.AlwaysOn
                }

            }

            ProgressBar{
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                value: Archivio.xmlList.loading
            }

            Label{
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                verticalAlignment: Label.AlignVCenter
                horizontalAlignment: Label.AlignHCenter
                text: "N° elementi: "+Archivio.xmlList.count
            }


        }
    }


}
