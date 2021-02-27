from PySide2.QtWidgets import *
from PySide2.QtCore import *
from PySide2.QtGui import *

from collections import deque

import common

class Dialogs:
    dialogStack = deque()

    def __init__(self, parent):
        self.widget = parent.dialogsPage
        self.header = parent.dialogHeader
        self.backButton = parent.dialog_backButton
        self.titleLabel = parent.dialog_titleLabel
        self.dialogStackWidget = parent.dialogsStack

        self.backButton.clicked.connect(self.dialogReturn)
        self.dialogStackWidget.currentChanged.connect(self.updateTitle)

        self.manualEntry = self.ManualEntry(parent, self)

    def setDialog(self, dialog):
        self.dialogStackWidget.setCurrentIndex(dialog.value)
        self.dialogStack.append(dialog.value)
        self.updateTitle()

    def updateTitle(self):
        val = self.dialogStackWidget.currentIndex()
        if val == common.DialogMode.MANUAL_ENTRY.value:
            self.titleLabel.setText('Manual Entry')
        elif val == common.DialogMode.EDIT_ENTRY.value:
            self.titleLabel.setText('Edit Entry')
        elif val == common.DialogMode.SYSTEM_BROWSER.value:
            self.titleLabel.setText('Browse')
        elif val == common.DialogMode.SYSTEM_SETTINGS.value:
            self.titleLabel.setText('Settings')

    def dialogReturn(self):
        if self.dialogStack:
            self.dialogStackWidget.setCurrentIndex(self.dialogStack.pop())
            if not self.dialogStack:
                common.getApp().dialogClosed()
        else:
            common.getApp().dialogClosed()

    class ManualEntry:
        def __init__(self, parentWindow, dialogHost):
            self.widget = parentWindow.manualEntryDialog
            self.confirmButton = parentWindow.manual_confirmButton
            self.freqLineEdit = parentWindow.manual_freqLineEdit
            self.modulationCombo = parentWindow.manual_modulationCombo
            self.host = dialogHost

            self.confirmButton.clicked.connect(self.onConfirm)
            self.freqLineEdit.returnPressed.connect(self.onConfirm)

        def onConfirm(self):
            freq = self.freqLineEdit.text()
            freq = int(float(freq) * 1000000)
            mode = self.modulationCombo.currentText()
            common.getApp().manualEntry(freq, mode)
            self.host.dialogReturn()

        def addModulation(self, mode):
            self.modulationCombo.addItem(mode)

    class EditEntry:
        def __init__(self, parent):
            self.widget = parent.editEntryDialog
            self.codeLineEdit = parent.entry_codeLineEdit
            self.delayLineEdit = parent.entry_delayLineEdit
            self.freqLineEdit = parent.entry_freqLineEdit
            self.lockoutCheckbox = parent.entry_lockoutCheckbox
            self.modulationCombo = parent.entry_modulationCombo
            self.systemCombo = parent.entry_systemCombo
            self.tagLineEdit = parent.entry_tagLineEdit

