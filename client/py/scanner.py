from PySide2.QtWidgets import *
from PySide2.QtCore import *
from PySide2.QtGui import *

import common

import sys
sys.path.insert(1, '../../proto')
import messages_pb2
import context_pb2
import request_pb2

import google.protobuf.message as proto

class Scanner:
    gainFromContext = False
    squelchFromContext = False

    def __init__(self, parentWindow):
        self.widget = parentWindow.findChild(QWidget, 'scannerPage')
        self.contextStack = parentWindow.findChild(QStackedWidget, 'scanContextStack')
        self.holdPage = parentWindow.findChild(QWidget, 'scanner_holdPage')
        self.scanPage = parentWindow.findChild(QWidget, 'scanner_scanPage')
        self.entryTagLabel = parentWindow.findChild(QLabel, 'scanner_entryTagLabel')
        self.frequencyLabel = parentWindow.findChild(QLabel, 'scanner_frequencyLabel')
        self.modulationLabel = parentWindow.findChild(QLabel, 'scanner_modulationLabel')
        self.systemTagLabel = parentWindow.findChild(QLabel, 'scanner_systemTagLabel')
        self.entryNumLabel = parentWindow.findChild(QLabel, 'scanner_entryNumLabel')
        self.lockoutDurationLabel = parentWindow.findChild(QLabel, 'scanner_lockoutDurationLabel')
        self.lockoutDurationButton = parentWindow.findChild(QPushButton, 'scanner_lockoutDurationButton')
        self.scanIndicator = parentWindow.findChild(QLabel, 'scanner_scanIndicator')
        self.gainSlider = parentWindow.findChild(QSlider, 'scanner_gainSlider')
        ##self.gainLabel = parentWindow.findChild(QLabel, 'scanner_gainLabel')
        self.sigStrengthBar = parentWindow.findChild(QProgressBar, 'scanner_sigStrengthBar')
        self.squelchSlider = parentWindow.findChild(QSlider, 'scanner_squelchSlider')
        ##self.squelchLabel = parentWindow.findChild(QLabel, 'scanner_squelchLabel')
        self.fnButtonsWidget = parentWindow.findChild(QWidget, 'scanner_fnButtonsWidget')
        self.fnButton1 = parentWindow.findChild(QPushButton, 'scanner_fnButton1')
        self.fnButton2 = parentWindow.findChild(QPushButton, 'scanner_fnButton2')
        self.fnButton3 = parentWindow.findChild(QPushButton, 'scanner_fnButton3')
        self.fnButton4 = parentWindow.findChild(QPushButton, 'scanner_fnButton4')

        self.sidebarToggleButton = parentWindow.findChild(QToolButton, 'scanner_sidebarToggle')
        self.sidebarPanel = parentWindow.findChild(QWidget, 'scanner_sidebarPanel')

        self.disconnectButton = parentWindow.findChild(QToolButton, 'scanner_disconnectButton')
        self.settingsButton = parentWindow.findChild(QToolButton, 'scanner_settingsButton')
        self.connectInfoButton = parentWindow.findChild(QToolButton, 'scanner_connectInfoButton')

        self.volumeControlPanel = parentWindow.findChild(QWidget, 'scanner_volumeControl')
        self.volumeSlider = parentWindow.findChild(QWidget, 'scanner_volumeSlider')
        self.muteButton = parentWindow.findChild(QWidget, 'scanner_volumeMute')

        self.entryEditButton = parentWindow.findChild(QToolButton, 'scanner_entryEditButton')

        self.fnButton1.clicked.connect(self.onFnButton1)
        self.fnButton2.clicked.connect(self.onFnButton2)
        self.fnButton3.clicked.connect(self.onFnButton3)
        self.fnButton4.clicked.connect(self.onFnButton4)
        self.gainSlider.valueChanged.connect(self.ongainSlider)
        self.squelchSlider.valueChanged.connect(self.onsquelchSlider)

        self.sidebarToggleButton.clicked.connect(self.onSidebarToggle)
        self.sidebarOpen = False
        self.sidebarPanel.setVisible(False)

        self.disconnectButton.clicked.connect(self.onDisconnectButton)

        self.volumeSlider.valueChanged.connect(self.onVolumeSlider)
        self.muteButton.toggled.connect(self.onMuteButton)

        #temporary since settins dialog is not yet implemented
        self.volumeControlPanel.setVisible(False)
        self.lockoutDurationButton.setVisible(False)
        self.lockoutDurationLabel.setVisible(False)
        self.fnButton4.setVisible(False)
        self.settingsButton.setVisible(False)
        self.connectInfoButton.setVisible(False)
        self.entryEditButton.setVisible(False)

        movie = QMovie("resources/bar-scan.gif")
        movie.start()
        self.scanIndicator.setMovie(movie)

        labelPalette = QPalette()
        labelPalette.setColor(QPalette.WindowText, Qt.white)
        labelFont = QFont()
        labelFont.setPointSize(12)
        squelchLayout = QHBoxLayout(self.squelchSlider)
        squelchLayout.setContentsMargins(0, 0, 0, 0)
        self.squelchLabel = QLabel()
        self.squelchLabel.setAlignment(Qt.AlignTop)
        self.squelchLabel.setPalette(labelPalette)
        self.squelchLabel.setFont(labelFont)
        self.squelchLabel.setText(str(0))
        squelchLayout.addWidget(self.squelchLabel)

        gainLayout = QHBoxLayout(self.gainSlider)
        gainLayout.setContentsMargins(0, 0, 0, 0)
        self.gainLabel = QLabel()
        self.gainLabel.setAlignment(Qt.AlignTop)
        self.gainLabel.setPalette(labelPalette)
        self.gainLabel.setFont(labelFont)
        self.gainLabel.setText(str(0))
        gainLayout.addWidget(self.gainLabel)

        #self.setMode(common.ScannerMode.SCANNING)
        self.contextStack.setCurrentWidget(self.scanPage)

        self.lastGainVal = 0
        self.lastSquelchSlide = 0
        self.gainTimer = QTimer()
        self.gainTimer.setInterval(100)
        self.gainTimer.setSingleShot(True)
        self.gainTimer.timeout.connect(self.setGain)

    #def setMode(self, mode):
    #    self.contextStack.setCurrentIndex(mode.value)

    def updateSignalIndicator(self, level):
        self.sigStrengthBar.setValue(level)

    def updateScanContext(self, context):
        if context.state == context_pb2.ScannerContext.State.Value('SCAN'):
            #self.setMode(common.ScannerMode.SCANNING)
            self.contextStack.setCurrentWidget(self.scanPage)
            self.sigStrengthBar.setValue(0)
            common.getApp().setWindowTitleInfo('Scanning')
        else:
            try:
                entry = context.entryData
                self.entryNumLabel.setText(entry.entryIndex)
                self.systemTagLabel.setText(entry.systemTag)
                self.entryTagLabel.setText(entry.entryTag)
                freq_formatted = str(float(entry.freq) / 1000000)
                self.frequencyLabel.setText((freq_formatted + " MHz"))
                self.modulationLabel.setText(entry.modulation)

                common.getApp().setWindowTitleInfo(entry.entryIndex + ': ' + entry.entryTag)
            except:
                print('problem setting values')

            
            if context.state == context_pb2.ScannerContext.State.Value('HOLD'):
                self.sigStrengthBar.setValue(0)
                #self.setMode(common.ScannerMode.HOLD)
                self.contextStack.setCurrentWidget(self.holdPage)
            elif context.state == context_pb2.ScannerContext.State.Value('RECEIVE'):
                #self.setMode(common.ScannerMode.RECEIVE)
                self.contextStack.setCurrentWidget(self.holdPage)

    def updateDemodContext(self, context):
        squelchFromContext = True
        gainFromContext = True
        #SquelchScale.configure(value=context.squelch)
        self.squelchSlider.setValue(context.squelch)
        self.squelchLabel.setText(str(context.squelch))
        #GainScale.configure(value=context.gain)
        self.gainSlider.setValue(context.gain)
        #self.gainLabel.setText(context.gain)

        if context.gain < 0:
            #GainLabel.configure(text='Auto')
            self.gainLabel.setText('Auto')
        else:
            #GainLabel.configure(text=context.gain)
            self.gainLabel.setText(str(int(context.gain)))

    def onFnButton1(self):
        common.getApp().scan()

    def onFnButton2(self):
        common.getApp().hold()

    def onFnButton3(self):
        common.getApp().showManualEntryDialog()

    def onFnButton4(self):
        common.getApp().showSettingsDialog()

    def ongainSlider(self):
        if self.gainFromContext:
            self.gainFromContext = False
        else:
            self.lastGainVal = self.gainSlider.value()
            self.gainTimer.start()

    def setGain(self):
        self._job = None
        val = float(self.lastGainVal)
        common.getApp().setGain(val)

    def onsquelchSlider(self):
        if self.squelchFromContext:
            self.squelchFromContext = False
        else:
            val = float(self.squelchSlider.value())
            #if abs(val - self.lastSquelchSlide) < 0.5:
            common.getApp().setSquelch(val)
            self.lastSquelchSlide = val

    def onSidebarToggle(self):
        if self.sidebarOpen:
            self.sidebarToggleButton.setText('<')
            self.sidebarPanel.setVisible(False)
            self.sidebarOpen = False
        else:
            self.sidebarToggleButton.setText('>')
            self.sidebarPanel.setVisible(True)
            self.sidebarOpen = True

    def setSquelchRange(self, minimum, maximum):
        self.squelchSlider.setMinimum(minimum)
        self.squelchSlider.setMaximum(maximum)

    def setGainRange(self, minimum, maximum):
        self.gainSlider.setMinimum(minimum)
        self.gainSlider.setMaximum(maximum)

    def onDisconnectButton(self):
        common.getApp().disconnect()

    def onVolumeSlider(self, value):
        common.getApp().setAudioVolume(value)

    def onMuteButton(self, value):
        self.volumeSlider.setEnabled(not value)
        common.getApp().setAudioMute(value)

    def setVolumeVisible(self, visible):
        self.volumeControlPanel.setVisible(visible)
