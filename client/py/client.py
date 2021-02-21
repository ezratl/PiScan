#!/usr/bin/python3

import sys, getopt

from PySide2 import QtWidgets
from PySide2.QtUiTools import QUiLoader
from PySide2.QtWidgets import *
from PySide2.QtCore import *

from socket import *
from threading import Thread

from time import sleep

import audio_manager
import common
import constants
import connect
import dialogs
import scanner

sys.path.insert(1, '../../proto')
import messages_pb2
import context_pb2
import request_pb2

import google.protobuf.message as proto

class PiScanClient(QWidget, common.AppInterface):
    dataReceived = Signal(bytes)
    manualDisconnectInitiated = False
    manualDisconnectMessage = None

    def __init__(self, parent=None, address=None, port=None, use_audio=False, rtsp_port=None):
        super(PiScanClient, self).__init__(parent)
        common.setInstance(self)
        ui_file = 'scan_client.ui'
        ui_file = QFile(ui_file)
        ui_file.open(QFile.ReadOnly)

        loader = QUiLoader()
        self.window = loader.load(ui_file)
        ui_file.close()

        self.parentWindow = parent
        #layout = QGridLayout(self)
        #layout.setContentsMargins(0, 0, 0, 0)
        #layout.addWidget(self.window)

        self.dataReceived.connect(self.handleReceived)

        self.connectDialog = connect.ConnectDialog(self.window, address, port)
        self.scanner = scanner.Scanner(self.window)
        self.dialogs = dialogs.Dialogs(self.window)

        self.contextStack = self.window.findChild(QStackedWidget, 'contextStack')
        #self.setWindowMode(common.WindowMode.CONNECT)
        self.showConnectDialog()
        #self.setWindowMode(common.WindowMode.SCANNER)

        #self.window.show()

        self.inmsg = messages_pb2.ServerToClient()

        self.use_audio = use_audio
        self.audio = audio_manager.AudioManager()

    def mainWidget(self):
        return self.window

    def setWindowMode(self, mode):
        self.mode = mode

        self.contextStack.setCurrentIndex(mode.value)

    def showDialog(self, dialog):
        self.dialogs.setDialog(dialog)
        self.setWindowMode(common.WindowMode.DIALOG)

    def receive(self):
        disconnectMessage = None

        while True:
            try:
                data = self.sock.recv(2048)
                if not data:
                    disconnectMessage = 'Connection closed by host'
                    break
                self.dataReceived.emit(data)
            except ConnectionAbortedError:
                if not self.manualDisconnectInitiated:
                    disconnectMessage = 'Connection aborted'
                break
            except OSError as err:
                if not self.manualDisconnectInitiated:
                    disconnectMessage = str(err)
                break
            except:
                e = sys.exc_info()[0]
                disconnectMessage = 'Unhandled exception: ' + str(e)
                break

        if not disconnectMessage:
            disconnectMessage = self.manualDisconnectMessage

        if disconnectMessage:
            print('Closing connection. Reason: ' + disconnectMessage)
        else:
            print('Disconnecting...')
        
        if self.use_audio:
            self.audio.stopRtspAudioStream()
        self.manualDisconnectInitiated = False
        self.showConnectDialog(disconnectMessage)

    def handleReceived(self, data):
        self.inmsg.ParseFromString(data)
        self.decodeMessage(message=self.inmsg)

    def decodeMessage(self, message):
        #print(message)
        if message.WhichOneof('content') == 'systemInfo':
            #print('system info')
            self.scanner.setSquelchRange(message.systemInfo.squelchScaleMin, message.systemInfo.squelchScaleMax)
            for mode in message.systemInfo.supportedModulations:
                self.dialogs.manualEntry.addModulation(mode)
        elif message.type == messages_pb2.ServerToClient.Type.Value('SCANNER_CONTEXT'):
            #print('scanner context')
            self.scanner.updateScanContext(message.scannerContext)
            if self.contextWait:
                self.setWindowMode(common.WindowMode.SCANNER)
                self.contextWait = False
        elif message.type == messages_pb2.ServerToClient.Type.Value('DEMOD_CONTEXT'):
            #print('demod context')
            self.scanner.updateDemodContext(message.demodContext)
        #elif message.type == messages_pb2.ServerToClient.Type.Value('GENERAL_MESSAGE'):
        elif message.WhichOneof('content') == 'signalLevel':
            self.scanner.updateSignalIndicator(message.signalLevel.level)
            

    def closeEvent(self, event):
        print('Exiting...')
        self.disconnect()

    ## AppInterface methods
    #def attemptConnection(self, sock):


    def completeConnection(self, sock, host, use_audio, audio_port):
        self.sock = sock
        self.rcv_thread = Thread(target=self.receive, name='socket monitor')
        self.rcv_thread.start()
        self.contextWait = True
        self.connectDialog.contextWait()  
        sleep(1) ## TODO sync issue - the responses are received before the application is ready to process them
        message1 = messages_pb2.ClientToServer()
        message1.type = messages_pb2.ClientToServer.Type.Value('GENERAL_REQUEST')
        message1.generalRequest.type = request_pb2.GeneralRequest.RequestType.Value('SCANNER_CONTEXT')
        self.sendData(message1.SerializeToString())
        sleep(0.25)
        message2 = messages_pb2.ClientToServer()
        message2.type = messages_pb2.ClientToServer.Type.Value('GENERAL_REQUEST')
        message2.generalRequest.type = request_pb2.GeneralRequest.RequestType.Value('DEMOD_CONTEXT')
        self.sendData(message2.SerializeToString())
        sleep(0.25)
        message3 = messages_pb2.ClientToServer()
        message3.type = messages_pb2.ClientToServer.Type.Value('GENERAL_REQUEST')
        message3.generalRequest.type = request_pb2.GeneralRequest.RequestType.Value('SYSTEM_INFO')
        self.sendData(message3.SerializeToString()) 

        if use_audio:
            self.use_audio = self.audio.startRtspAudioStream(host, audio_port) 
            #self.setAudioVolume(50) 
        else:
            self.use_audio = False
        self.scanner.setVolumeVisible(self.use_audio)

    def scan(self):
        message = messages_pb2.ClientToServer()
        message.type = messages_pb2.ClientToServer.Type.Value('SCANNER_STATE_REQUEST')
        message.scanStateRequest.state = request_pb2.ScannerStateRequest.NewState.Value('SCAN')
        self.sendData(message.SerializeToString())

    def hold(self):
        message = messages_pb2.ClientToServer()
        message.type = messages_pb2.ClientToServer.Type.Value('SCANNER_STATE_REQUEST')
        message.scanStateRequest.state = request_pb2.ScannerStateRequest.NewState.Value('HOLD')
        self.sendData(message.SerializeToString())

    def manualEntry(self, frequency, modulation):
        message = messages_pb2.ClientToServer()
        message.type = messages_pb2.ClientToServer.Type.Value('SCANNER_STATE_REQUEST')
        message.scanStateRequest.state = request_pb2.ScannerStateRequest.NewState.Value('MANUAL')
        message.scanStateRequest.manFreq = frequency
        message.scanStateRequest.manModulation = modulation
        self.sendData(message.SerializeToString())

    def showConnectDialog(self, errorMessage = ''):
        self.clearWindowTitleInfo()
        self.connectDialog.connectFailed(errorMessage)
        self.setWindowMode(common.WindowMode.CONNECT)

    def showManualEntryDialog(self):
        self.lastMode = self.mode
        self.showDialog(common.DialogMode.MANUAL_ENTRY)

    def showEditEntryDialog(self):
        self.lastMode = self.mode
        self.showDialog(common.DialogMode.EDIT_ENTRY)

    def showEntryBrowser(self):
        self.lastMode = self.mode
        self.showDialog(common.DialogMode.SYSTEM_BROWSER)

    def showSettingsDialog(self):
        self.lastMode = self.mode
        self.showDialog(common.DialogMode.SYSTEM_SETTINGS)

    def setGain(self, value):
        message = messages_pb2.ClientToServer()
        message.type = messages_pb2.ClientToServer.Type.Value('DEMOD_REQUEST')
        message.demodRequest.type = request_pb2.DemodRequest.DemodFunc.Value('SET_GAIN')
        message.demodRequest.level = value
        self.sendData(message.SerializeToString())

    def setSquelch(self, value):
        message = messages_pb2.ClientToServer()
        message.type = messages_pb2.ClientToServer.Type.Value('DEMOD_REQUEST')
        message.demodRequest.type = request_pb2.DemodRequest.DemodFunc.Value('SET_SQUELCH')
        message.demodRequest.level = value
        self.sendData(message.SerializeToString())

    def dialogClosed(self):
        self.setWindowMode(self.lastMode)

    def tryConnect(self, address, port, use_audio, rtsp_port):
        self.connectDialog.tryConnect(address, port, use_audio, rtsp_port)

    def setWindowTitleInfo(self, message):
        if isinstance(self.parentWindow, QtWidgets.QMainWindow):
            self.parentWindow.setWindowTitle('PiScan | ' + message)
        else:
            self.setWindowTitle('PiScan | ' + message)

    def clearWindowTitleInfo(self):
        if isinstance(self.parentWindow, QtWidgets.QMainWindow):
            self.parentWindow.setWindowTitle('PiScan')
        else:
            self.setWindowTitle('PiScan')

    def disconnect(self, message=''):
        self.manualDisconnectInitiated = True
        self.manualDisconnectMessage = message
        try:
            if self.sock:
                self.sock.close()
                #self.rcv_thread.join()
        except Exception as e:
            print(e)

    def setAudioVolume(self, level):
        if self.use_audio:
            self.audio.setAudioVolume(level)

    def setAudioMute(self, mute):
        if self.use_audio:
            self.audio.setAudioMute(mute)

    def sendData(self, message):
        try:
            self.sock.send(message)
        except OSError as err:
            self.disconnect('Connection error: ' + str(err))
        except Exception as e:
            print(e)

class HostWindow(QtWidgets.QMainWindow):
    def __init__(self, parent=None, address=None, port=None, use_audio=False, rtsp_port=None):
        super(HostWindow, self).__init__(parent)

        form = PiScanClient(self, address, port, use_audio, rtsp_port)

        mainWidget = form.mainWidget()
        self.setPalette(mainWidget.palette())
        self.setGeometry(mainWidget.geometry())
        self.setWindowTitle(mainWidget.windowTitle())

        self.setCentralWidget(mainWidget)
        #self.actionQuit.triggered.connect(self.closeEvent)

        #self.show()

        if address:
            if not port:
                port = constants.DEFAULT_TCP_PORT
            form.tryConnect(address, port, use_audio, rtsp_port)

    def closeEvent(self, event):
        #print('exit')
        common.getApp().closeEvent(event)
        event.accept()

if __name__ == '__main__':
    shortOpts = 'la:p:wsr:'
    longOpts = ['--local', '--address', '--port', '--pi_mode', '--audio', '--rtsp_port']

    options, remainder = getopt.getopt(sys.argv[1:], shortOpts, longOpts)

    address = None
    port = None
    piMode = False
    use_audio = False
    rtsp_port = 8554

    for opt, arg in options:
        if opt in ('-l', '--local'):
            address = 'localhost'
        elif opt in ('-a', '--address'):
            address = arg
        elif opt in ('-p', '--port'):
            port = int(arg)
        elif opt in ('-w', '--pi_mode'):
            piMode = True
        elif opt in ('-s', '--audio'):
            use_audio = True
        elif opt in ('-r', '--rtsp_port'):
            rtsp_port = int(arg)

    app = QApplication(sys.argv)
    window = HostWindow(address=address, port=port, use_audio=use_audio, rtsp_port=rtsp_port)
    if piMode:
        flags = Qt.WindowFlags(Qt.CustomizeWindowHint | Qt.FramelessWindowHint | Qt.Tool)
        window.setWindowFlags(flags)
        window.showMaximized()
    else:
        window.show()

    sys.exit(app.exec_())


