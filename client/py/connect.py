import sys

from PySide2.QtUiTools import QUiLoader
from PySide2.QtWidgets import QWidget, QLabel, QPushButton, QLineEdit
from PySide2.QtCore import QObject
from PySide2.QtGui import QMovie, QPixmap

from socket import *

import common
import constants

class ConnectDialog:
    def __init__(self, parentWindow, address=None, port=None):
        self.widget = parentWindow.findChild(QWidget, 'connectPage')
        self.errorLabel = parentWindow.findChild(QLabel, 'connect_errorLabel')
        self.confirmButton = parentWindow.findChild(QPushButton, 'connect_confirmButton')
        self.connectIndicator = parentWindow.findChild(QLabel, 'connect_indicator')
        self.hostLineEdit = parentWindow.findChild(QLineEdit, 'connect_hostnameLineEdit')
        self.portLineEdit = parentWindow.findChild(QLineEdit, 'connect_portLineEdit')
        self.logo = parentWindow.findChild(QLabel, 'connect_logoImage')
        self.hostLabel = parentWindow.findChild(QLabel, 'hostLabel')
        self.portLabel = parentWindow.findChild(QLabel, 'hostPortLabel')

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

    def onConfirm(self):
        host = self.hostLineEdit.text()
        port = int(self.portLineEdit.text())
        self.tryConnect(host, port)

    def tryConnect(self, address, port):
        print('connect confirm')
        try:
            self.connectIndicator.setVisible(True)
            self.errorLabel.setVisible(False)
            self.hostLineEdit.setText(address)
            self.portLineEdit.setText(str(port))
            self.widget.repaint()
            
            print('Connecting to ', address, ':', port)
            dest = (address, port)

            sock = socket(AF_INET, SOCK_STREAM)
            sock.connect(dest)
            print('Connection success')

            self.connectIndicator.setVisible(False)

            common.getApp().completeConnection(sock)

        except:
            e = sys.exc_info()[0]
            self.connectFailed(repr(e))
            

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





