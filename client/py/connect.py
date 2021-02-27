import sys

from PySide2.QtUiTools import QUiLoader
from PySide2.QtWidgets import QWidget, QLabel, QPushButton, QLineEdit, QCheckBox
from PySide2.QtCore import QObject
from PySide2.QtGui import QMovie, QPixmap

from socket import *

import common
import constants

class ConnectDialog:
    def __init__(self, parentWindow, address=None, port=None, use_audio=False, rtsp_port=None):
        self.widget = parentWindow.connectPage
        self.errorLabel = parentWindow.connect_errorLabel
        self.confirmButton = parentWindow.connect_confirmButton
        self.connectIndicator = parentWindow.connect_indicator
        self.hostLineEdit = parentWindow.connect_hostnameLineEdit
        self.portLineEdit = parentWindow.connect_portLineEdit
        self.logo = parentWindow.connect_logoImage
        self.hostLabel = parentWindow.hostLabel
        self.portLabel = parentWindow.hostPortLabel
        self.audioCheckBox = parentWindow.connect_audioCheckBox
        self.rtspPortPanel = parentWindow.connect_rtspPortPanel
        self.rtspPortLineEdit = parentWindow.connect_rtspPortLineEdit

        self.logo.setPixmap(QPixmap("resources/icon-256.png"))
        self.logo.setVisible(False)

        movie = QMovie("resources/loader-small.gif")
        movie.start()
        self.connectIndicator.setMovie(movie)
        self.connectIndicator.setVisible(False)

        self.errorLabel.setVisible(False)

        self.hostLineEdit.setText(constants.DEFAULT_ADDRESS)
        self.portLineEdit.setText(str(constants.DEFAULT_TCP_PORT))

        self.confirmButton.clicked.connect(self.onConfirm)
        self.hostLineEdit.returnPressed.connect(self.onConfirm)
        self.portLineEdit.returnPressed.connect(self.onConfirm)

        self.rtspPortPanel.setVisible(False)

    def onConfirm(self):
        host = self.hostLineEdit.text()
        port = int(self.portLineEdit.text())
        audio = self.audioCheckBox.isChecked()
        rtsp_port = int(self.rtspPortLineEdit.text())
        self.tryConnect(host, port, audio, rtsp_port)

    def tryConnect(self, address, port, use_audio=False, rtsp_port=8554):
        print('connect confirm')
        try:
            self.connectIndicator.setVisible(True)
            self.errorLabel.setVisible(False)
            self.hostLineEdit.setText(address)
            self.portLineEdit.setText(str(port))
            self.audioCheckBox.setChecked(use_audio)
            self.rtspPortLineEdit.setText(str(rtsp_port))
            self.widget.repaint()
            
            print('Connecting to ', address, ':', port)
            dest = (address, port)

            sock = socket(AF_INET, SOCK_STREAM)
            sock.connect(dest)
            print('Connection success')

            self.connectIndicator.setVisible(False)

            common.getApp().completeConnection(sock, address, use_audio, rtsp_port)
        except ConnectionRefusedError:
            self.connectFailed('Connect failed - Connection refused')
        except gaierror as err:
            self.connectFailed('Connect failed - ' + str(err))
        except TimeoutError:
            self.connectFailed('Connect failed - Timed out')
        except:
            e = sys.exc_info()[0]
            self.connectFailed('Connect failed - Unhandled exception: ' + str(e))
            

    def contextWait(self):
        self.connectIndicator.setVisible(True)
        self.errorLabel.setVisible(False)
        self.hostLineEdit.setVisible(False)
        self.portLineEdit.setVisible(False)
        self.confirmButton.setVisible(False)
        self.hostLabel.setVisible(False)
        self.portLabel.setVisible(False)

    def connectFailed(self, message):
        self.connectIndicator.setVisible(False)
        self.errorLabel.setText(str(message))
        self.errorLabel.setVisible(True)
        self.hostLineEdit.setVisible(True)
        self.portLineEdit.setVisible(True)
        self.confirmButton.setVisible(True)
        self.hostLabel.setVisible(True)
        self.portLabel.setVisible(True)





