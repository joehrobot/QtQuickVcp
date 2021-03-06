/****************************************************************************
**
** Copyright (C) 2014 Alexander Rössler
** License: LGPL version 2.1
**
** This file is part of QtQuickVcp.
**
** All rights reserved. This program and the accompanying materials
** are made available under the terms of the GNU Lesser General Public License
** (LGPL) version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** Contributors:
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/
import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("ValueChart")

        SectionLayout {
            Label {
                text: qsTr("Time span")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.timeSpan
                    minimumValue: 1
                    maximumValue: 99999999999999999999
                    decimals: 0
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Minimum value")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.minimumValue
                    minimumValue: -999999999999
                    maximumValue: 999999999999
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Maximum value")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.maximumValue
                    minimumValue: -999999999999
                    maximumValue: 999999999999
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("X grid")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.xGrid
                    minimumValue: 0.01
                    maximumValue: 999999999999
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Y grid")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.yGrid
                    minimumValue: 0.01
                    maximumValue: 999999999999
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Signal line width")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.signalLineWidth
                    minimumValue: 1
                    maximumValue: 999999999999
                    decimals: 0
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Grid line width")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.gridLineWidth
                    minimumValue: 1
                    maximumValue: 999999999999
                    decimals: 0
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Change graph scale")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.changeGraphScale
                    minimumValue: 0.01
                    maximumValue: 999999999999
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Change graph enabled")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.changeGraphEnabled
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Auto scroll")
            }

            SecondColumnLayout {
                CheckBox {
                    backendValue: backendValues.autoScroll
                    implicitWidth: 180
                }
                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Scroll zoom factor")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.scrollZoomFactor
                    minimumValue: 0.01
                    maximumValue: 999999999999
                    decimals: 2
                }
                ExpandingSpacer {
                }
            }
        }
    }

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Text")

        SectionLayout {
            Label {
                text: qsTr("Decimals")
            }

            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.decimals
                    minimumValue: 0
                    maximumValue: 10
                    decimals: 0
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Prefix")
            }

            SecondColumnLayout {
                LineEdit {
                    backendValue: backendValues.prefix
                    Layout.fillWidth: true
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Suffix")
            }

            SecondColumnLayout {
                LineEdit {
                    backendValue: backendValues.suffix
                    Layout.fillWidth: true
                }
                ExpandingSpacer {
                }
            }

            Label {
                text: qsTr("Visibility")
            }

            SecondColumnLayout {
                ColumnLayout {
                    CheckBox {
                        text: qsTr("Left text")
                        backendValue: backendValues.leftTextVisible
                    }
                    CheckBox {
                        text: qsTr("Right text")
                        backendValue: backendValues.rightTextVisible
                    }
                }

                ExpandingSpacer {
                }
            }
        }
    }

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Background color")

        ColorEditor {
                    caption: qsTr("Background color")
                    backendValue: backendValues.backgroundColor
                    supportGradient: false
                }
    }

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Grid color")

        ColorEditor {
                    caption: qsTr("Grid color")
                    backendValue: backendValues.gridColor
                    supportGradient: false
                }
    }

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Text color")

        ColorEditor {
                    caption: qsTr("Text color")
                    backendValue: backendValues.textColor
                    supportGradient: false
                }
    }

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Signal color")

        ColorEditor {
                    caption: qsTr("Signal color")
                    backendValue: backendValues.signalColor
                    supportGradient: false
                }
    }

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("H line color")

        ColorEditor {
                    caption: qsTr("H line color")
                    backendValue: backendValues.hLineColor
                    supportGradient: false
                }
    }

    Section {
            anchors.left: parent.left
            anchors.right: parent.right
            caption: qsTr("Positive change color")

            ColorEditor {
                        caption: qsTr("PositiveChange color")
                        backendValue: backendValues.positiveChangeColor
                        supportGradient: false
                    }
    }

    Section {
            anchors.left: parent.left
            anchors.right: parent.right
            caption: qsTr("Negative change color")

            ColorEditor {
                        caption: qsTr("Negative change color")
                        backendValue: backendValues.negativeChangeColor
                        supportGradient: false
                    }
    }
}
