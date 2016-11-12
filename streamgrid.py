#!/usr/bin/env python3

import sys

from PyQt5.QtCore import Qt, QUrl, pyqtSignal
from PyQt5.QtWidgets import QApplication, QWidget, QGridLayout, QStackedLayout, QLabel
from PyQt5.QtGui import QMovie, QPalette
from PyQt5.QtMultimedia import QMediaPlayer, QMediaContent
from PyQt5.QtMultimediaWidgets import QVideoWidget


class StreamGrid(QWidget):
    streams_ready = pyqtSignal()

    def __init__(self, streams):
        self.streams = streams
        super().__init__()

        self.setAttribute(Qt.WA_QuitOnClose)
        self.setWindowFlags(Qt.FramelessWindowHint)

        palette = QPalette()
        palette.setColor(QPalette.Background, Qt.white)
        self.setPalette(palette)

        self.layout = QStackedLayout()
        self.setLayout(self.layout)

        label = QLabel()
        label.setAlignment(Qt.AlignCenter)
        loading = QMovie("loading.gif")
        loading.start()
        label.setMovie(loading)
        self.layout.insertWidget(0, label)

        self.players = []
        for stream in self.streams:
            player = QMediaPlayer()
            player.setMedia(QMediaContent(stream))
            player.mediaStatusChanged.connect(self.changedMediaStatus)
            player.play()
            self.players.append(player)

        grid_layout = QGridLayout()
        grid_layout.setSpacing(0)
        grid_layout.setContentsMargins(0, 0, 0, 0)
        self.grid_widget = QWidget()
        self.grid_widget.setLayout(grid_layout)

        i = 0
        self.grid_widgets = []
        for row in range(2):
            for column in range(2):
                video_widget = QVideoWidget()
                self.players[i].setVideoOutput(video_widget)
                self.grid_widgets.append(video_widget)
                grid_layout.addWidget(video_widget, row, column)
                i+=1
        self.layout.insertWidget(1, self.grid_widget)

        video_widget = QVideoWidget()
        self.players[4].setVideoOutput(video_widget)
        self.layout.insertWidget(2, video_widget)

    def showStreamFullScreen(self, index):
        if index == 4:
            self.layout.setCurrentIndex(2)
        else:
            self.grid_widgets[index].show()
            for i, widget in enumerate(self.grid_widgets):
                if i != index:
                    widget.hide()
            self.layout.setCurrentIndex(1)

    def showStreamGrid(self):
        for widget in self.grid_widgets:
            widget.show()
        self.layout.setCurrentIndex(1)

    def keyPressEvent(self, event):
        key = event.key()
        if key == Qt.Key_Escape:
            self.close()
        elif key in (Qt.Key_1, Qt.Key_2, Qt.Key_3, Qt.Key_4, Qt.Key_5):
            self.showStreamFullScreen(key - Qt.Key_1)
        elif key == (Qt.Key_0):
            self.showStreamGrid()

    def changedMediaStatus(self, state):
        if all((player.mediaStatus() == QMediaPlayer.BufferedMedia) or
               (player.state() == QMediaPlayer.StoppedState) for player in self.players):
            self.streams_ready.emit()


STREAM_IDS = [
    '04asu087t_1@409528',
    '074tzyqju_1@394719',
    '0i1p9ue46_1@409669',
    '04et6qkjl_1@409668',
    '0xorfu8d8_1@308855'
]

STREAM_URL_FMT = "http://streamamghdl-lh.akamaihd.net/i/{}/index_1_av-p.m3u8"

streams = [QUrl(STREAM_URL_FMT.format(id)) for id in STREAM_IDS]

app = QApplication(sys.argv)

viewer = StreamGrid(streams)
viewer.showFullScreen()
viewer.streams_ready.connect(viewer.showStreamGrid)

app.exec_()
