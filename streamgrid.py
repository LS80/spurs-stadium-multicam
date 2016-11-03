#!/usr/bin/env python3

import sys

from PyQt5.QtCore import QUrl, pyqtSignal
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QGridLayout, QAction
from PyQt5.QtMultimedia import QMediaPlayer, QMediaContent
from PyQt5.QtMultimediaWidgets import QVideoWidget


class StreamGrid(QMainWindow):
    STREAMS = [
        ('04asu087t_1@409528', 0, 0),
        ('074tzyqju_1@394719', 0, 1),
        ('0i1p9ue46_1@409669', 1, 0),
        ('04et6qkjl_1@409668', 1, 1)
    ]

    ready = pyqtSignal()

    def __init__(self, parent=None):
        super().__init__(parent)
        widget = QWidget()
        self.setCentralWidget(widget)
        layout = QGridLayout()
        layout.setSpacing(0)
        layout.setContentsMargins(0, 0, 0, 0)
        widget.setLayout(layout)

        close_action = QAction("Close", self)
        close_action.setShortcut("ESC")
        close_action.triggered.connect(self.close)
        self.addAction(close_action)

        widgets = []
        self.players = []
        for stream, row, column in self.STREAMS:
            player = QMediaPlayer()
            url = "http://streamamghdl-lh.akamaihd.net/i/{}/index_1_av-p.m3u8".format(stream)
            player.setMedia(QMediaContent(QUrl(url)))
            videoWidget = QVideoWidget()
            player.setVideoOutput(videoWidget)
            player.mediaStatusChanged.connect(self.changedMediaStatus)
            self.players.append(player)
            layout.addWidget(videoWidget, row, column)

    def changedMediaStatus(self, state):
        if all(player.mediaStatus() == QMediaPlayer.BufferedMedia for player in self.players):
            self.ready.emit()

app = QApplication(sys.argv)
main = StreamGrid()
main.ready.connect(main.showFullScreen)
[player.play() for player in main.players]

app.exec_()
